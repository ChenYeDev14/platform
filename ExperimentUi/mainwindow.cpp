#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    battle = new NormalBattle;
    connect(battle, SIGNAL(round(int)), this, SLOT(show_round(int)));
    connect(battle,SIGNAL(send_winner(int)), this, SLOT(show_winner(int)));
    connect(battle, SIGNAL(path_error(int)), this, SLOT(show_path_error(int)));
    ui->setupUi(this);
    ui->progressBar_common->setRange(0, 2400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_common_ai1_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "select AI", QString(), tr("EXE (*.exe)"));
    ui->lineEdit_common2 = file;
}

void MainWindow::on_pushButton_common_ai2_clicked()
{
    ui->progressBar_common->setValue(0);
    battle->StartNormalBattle(ui->lineEdit_common1, ui->lineEdit_common2, ui->lineEdit_common_map);
}

void MainWindow::on_pushButton_common_file1_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "select AI", QString(), tr("EXE (*.exe)"));
    ui->lineEdit_common1 = file;
}


void MainWindow::show_round(int round)
{
    ui->progressBar_common->setValue(round);
}

void MainWindow::show_winner(int winner)
{
    if (winner == 3) QMessageBox::information(this, tr("对战结果"), tr("     平局！     "));
    else if (winner == 1) QMessageBox::information(this, tr("对战结果"), tr("    AI1获胜！     "));
    else QMessageBox::information(this, tr("对战结果"), tr("    AI2获胜！     "));
}


