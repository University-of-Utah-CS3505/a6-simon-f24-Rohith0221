#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QAbstractAnimation>
#include <QEasingCurve>
#include <QPropertyAnimation>

/*
 *
 *  Name : Rohith Veeramachaneni
 *  Course : CS 3505
 *  Date Published : October 28, 2024
 *  Github link : https://github.com/University-of-Utah-CS3505/a6-simon-f24-Rohith0221
 *  Github ID : Rohith0221
 *
 *   MainWindow is the View class that creates a window where all the connections between UI elements and the functions take place.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initially disable game buttons

    ui->gameOverLabel->setVisible(false);
    ui->redButton->setStyleSheet("background-color: rgb(255, 0, 0);");
    ui->blueButton->setStyleSheet("background-color: rgb(0, 0, 255);");
    ui->redButton->setEnabled(false);
    ui->blueButton->setEnabled(false);
    ui->redButton->setVisible(false);
    ui->blueButton->setVisible(false);
    ui->progressLabel->setEnabled(false);
    ui->progressLabel->setVisible(false);
    ui->progressBar->setVisible(false);

    model = new SimonModel(this);

    // Connect signals and slots
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(ui->redButton, &QPushButton::clicked, this, &MainWindow::handlePlayerMoveRed);
    connect(ui->blueButton, &QPushButton::clicked, this, &MainWindow::handlePlayerMoveBlue);
    connect(model, &SimonModel::showColor, this, &MainWindow::flashColor);
    connect(model, &SimonModel::progressUpdated, this, &MainWindow::updateProgress);
    connect(model, &SimonModel::gameOver, this, &MainWindow::showGameOver);

    // starts the sequence and disables the red & blue buttons
    connect(model, &SimonModel::sequenceStarting, this, [this]() {
        ui->redButton->setEnabled(false);
        ui->blueButton->setEnabled(false);
    });

     // ends the sequence and enables the red & blue buttons
    connect(model, &SimonModel::sequenceComplete, this, [this]() {
        ui->redButton->setEnabled(true);
        ui->blueButton->setEnabled(true);
    });

    // Added extra functionality by using the signal emitted from SimonModel class which swaps buttons positions
    connect(model, &SimonModel::swapButtons, this, [this](bool swap) {
        if (swap) {

            // Swaps button positions
            QPoint redPos = ui->redButton->pos();
            QPoint bluePos = ui->blueButton->pos();

            // Added animation for smooth movement
            QPropertyAnimation* redAnim = new QPropertyAnimation(ui->redButton, "pos");
            redAnim->setDuration(300);
            redAnim->setStartValue(redPos);
            redAnim->setEndValue(bluePos);
            redAnim->setEasingCurve(QEasingCurve::OutBack);  // Bouncy effect

            QPropertyAnimation* blueAnim = new QPropertyAnimation(ui->blueButton, "pos");
            blueAnim->setDuration(300);
            blueAnim->setStartValue(bluePos);
            blueAnim->setEndValue(redPos);
            blueAnim->setEasingCurve(QEasingCurve::OutBack);

            redAnim->start(QAbstractAnimation::DeleteWhenStopped);
            blueAnim->start(QAbstractAnimation::DeleteWhenStopped);
        }
    });
}

/*  Destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::onStartButtonClicked
 *
 *  Slot is linked with the signal emitted from when start button in the game is clicked.It starts the game and initialises all the buttons
 *  with proper properties and values
 */
void MainWindow::onStartButtonClicked() {
    ui->startButton->setEnabled(false);
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    ui->gameOverLabel->setVisible(false);
    ui->progressLabel->setEnabled(true);
    ui->progressLabel->setVisible(true);
    ui->redButton->setEnabled(false);  // Start with buttons disabled
    ui->blueButton->setEnabled(false);
    ui->redButton->setVisible(true);
    ui->blueButton->setVisible(true);
    model->startGame();
}

/**
 * @brief MainWindow::handlePlayerMoveRed
 *
 *  This slot is executed when red button is pushed
 */
void MainWindow::handlePlayerMoveRed() {
    model->playerMove(0);
}

/**
 * @brief MainWindow::handlePlayerMoveBlue
 *
 *  This slot is executed when blue button is pushed
 */
void MainWindow::handlePlayerMoveBlue() {
    model->playerMove(1);
}

/**
 * @brief MainWindow::flashColor
 * @param color the color passed in is validated as a value against 0,1 where 0 represents red and 1 represents blue.And after validation,
 *  highlights proper button and then sets it back to its original state
 */
void MainWindow::flashColor(int color) {
    QPushButton *button = (color == 0) ? ui->redButton : ui->blueButton;
    QString originalStyle = (color == 0) ?
                                "background-color: rgb(255, 0, 0);" :
                                "background-color: rgb(0, 0, 255);";

    // Flash the button
    button->setStyleSheet("background-color: yellow;");

    QTimer::singleShot(200, this, [this, button, originalStyle]() {
        button->setStyleSheet(originalStyle);
    });
}

/**
 * @brief MainWindow::updateProgress
 * @param progress The new progress value to be set for progress bar
 */
void MainWindow::updateProgress(int progress) {
    ui->progressBar->setValue(progress);
}

/**
 * @brief MainWindow::showGameOver
 *
 *   This function is executed when players make a wrong guess.It resets everything before player restarts the game
 */
void MainWindow::showGameOver() {
    ui->startButton->setEnabled(true);
    ui->redButton->setEnabled(false);
    ui->blueButton->setEnabled(false);
    ui->gameOverLabel->setVisible(true);
    ui->progressBar->setValue(0);
    QMessageBox::information(this, "Game Over", "You lose! Try again.");
}
