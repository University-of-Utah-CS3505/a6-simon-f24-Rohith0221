#ifndef SIMONMODEL_H
#define SIMONMODEL_H

#include <QObject>
#include <QTimer>
#include <QVector>

/*
 *
 *
 *  SimonModel.h is the header class that contains all the signal emitters and few function definitions used to simulate "Simon Game"
 *
 *
*/
class SimonModel : public QObject
{
    Q_OBJECT
public:
    SimonModel(QObject* parent = nullptr);
    void startGame();
    void playerMove(int color);

signals:

    void showColor(int color);
    void progressUpdated(int progress);
    void gameOver();
    void roundComplete();
    void sequenceStarting();
    void sequenceComplete();
    void swapButtons(bool swap); // Extra fun functionality part

private:
    void addMove(); // This function adds a new move to the sequence
    void playNextInSequence(); // This function is called to create and play the next sequence after the current sequence is solved
    void startSequence(); // Initiates sequences in the game

    QVector<int> sequence;
    int sequenceIndex;
    int playerIndex;
    QTimer* timer;
    int speed;
    bool isPlaying;
    bool isGameOver;
    bool isSwapped;
};

#endif // SIMONMODEL_H
