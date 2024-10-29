#include "SimonModel.h"
#include <QRandomGenerator>


/*
 *
 *  Name : Rohith Veeramachaneni
 *  Course : CS 3505
 *  Date Published : October 28, 2024
 *  Github link : https://github.com/University-of-Utah-CS3505/a6-simon-f24-Rohith0221
 *  Github ID : Rohith0221
 *
 *   SimonModel is the model class that emits signals accordingly to the MainWindow where MainWindow based on the signals will call right functions
 *   in MainWindow.cpp
 *
 *   Creative & Fun element :
 *
 *   For making the game little complex than usual, I have added swap buttons functionality similar to shuffle/jumble such that player will be hesitant to choose a
 *   color as the red color button will go into blue color button position and vice versa still only giving players one chance to fail.
 *
 *   NOTE : Used AI help in getting the fluid animation for swapping button positions
 */

/// Default constructor
SimonModel::SimonModel(QObject* parent)
    : QObject(parent), sequenceIndex(0), playerIndex(0), speed(500), isPlaying(false), isGameOver(false)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SimonModel::playNextInSequence); // connects timeout signal to playNextSequence slot to make sure next sequence is called
                                                                             // when time for previous sequence is elapsed
}

/**
 * @brief SimonModel::startGame
 *
 *   This function resets everything in the model class
 */
void SimonModel::startGame() {

    // Reset game state
    sequence.clear();
    sequenceIndex = 0;
    playerIndex = 0;
    isPlaying = false;
    isGameOver = false;
    speed = 500;

    addMove();
    startSequence();  // Start playing the sequence
}

/**
 * @brief SimonModel::playerMove
 * @param color the color button player clicked on
 *
 *  This function validates player clicked button to the correct color button pressed by computer and if correct advances player to next
 *  sequence if so, otherwise stops the game by emitting gameOver signal
 */
void SimonModel::playerMove(int color) {
    if (isGameOver || isPlaying) {
        return;  // Ignore input if game is over or sequence is playing
    }

    if (color == sequence[playerIndex]) {
        // Correct move
        playerIndex++;
        emit progressUpdated((playerIndex * 100) / sequence.size());

        if (playerIndex == sequence.size()) {
            // Round complete
            playerIndex = 0;
            speed = qMax(100, speed - 50);  // Speed up, but not faster than 100ms
            addMove();

            // Delay before next sequence
            QTimer::singleShot(1000, this, &SimonModel::startSequence);
        }
    } else {
        // Incorrect move
        isGameOver = true;
        emit gameOver();
    }
}

/**
 * @brief SimonModel::addMove
 *
 *  Adds a new step/move to the previous sequence
 */

void SimonModel::addMove() {
    int newMove = QRandomGenerator::global()->bounded(2);  // 0 for red, 1 for blue
    sequence.append(newMove);
}

/**
 * @brief SimonModel::startSequence
 *
 *  This function starts showing the sequence of buttons pressed by computer and then executes nextSequence
 */
void SimonModel::startSequence() {
    sequenceIndex = 0;
    isPlaying = true;
    emit sequenceStarting();
    playNextInSequence();
}

/**
 * @brief SimonModel::playNextInSequence
 *
 *   Sets up next sequence and emits swapButtons and sequenceCompleted signals for the functions relevant to be connected to and executed
 */
void SimonModel::playNextInSequence() {
    if (sequenceIndex < sequence.size()) {

        if (QRandomGenerator::global()->bounded(100) < 20) {
            isSwapped = !isSwapped;
            emit swapButtons(isSwapped);
        }

        emit showColor(sequence[sequenceIndex]);
        sequenceIndex++;

        if (sequenceIndex < sequence.size()) {
            timer->start(speed);
        } else {
            timer->stop();
            isPlaying = false;

            // Add a delay before enabling player input
            QTimer::singleShot(500, this, [this]() {
                // Reset swap state before player's turn
                if (isSwapped) {
                    isSwapped = false;
                    emit swapButtons(false);
                }
                emit sequenceComplete(); // after sequence is completed,emit sequenceComplete signal
            });
        }
    }
}
