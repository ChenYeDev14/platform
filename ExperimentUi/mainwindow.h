#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NormalBattle.h"

using namespace DS14;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_common_ai1_clicked();

    void on_pushButton_common_ai2_clicked();

    void on_pushButton_common_start_clicked();

    void show_round(int round);

    void show_winner(int winner);

    void show_path_error(int side);

private:
    Ui::MainWindow *ui;
    NormalBattle *battle;
};

#endif // MAINWINDOW_H
