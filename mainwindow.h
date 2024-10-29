#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include "SimonModel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SimonModel* model; // model object
    QProgressBar* progressBar;

private slots:
    void onStartButtonClicked();
    void handlePlayerMoveRed();
    void handlePlayerMoveBlue();
    void flashColor(int color);
    void updateProgress(int progress);
    void showGameOver();
};

#endif // MAINWINDOW_H
