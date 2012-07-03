#include "NormalBattle.h"
#include <QTime>
#include <QLocalServer>
#include <QLocalSocket>

void NormalBattle::change_to_debug_mode()
{
    if (debug == -1) return;
    debug_mode = true;
}

void NormalBattle::change_to_run_mode()()
{
    if (debug == -1) return;
    debug_mode == false;
}

 void NormalBattle::StartNormalBattle(QString side1, QString side2, QString map)
{
    debug_mode = false;
    human = -1;
    debug = -1;
    ai[0] = side1;
    ai[1] = side2;
    map_location = map;
    //检查三个文件地址是否指向一个正确的文件
    QFile file(ai[0]);
    if (!file.exists())
    {
        if (ai[0] == "\\\human")
            human = 0;
        else if (ai[0] == "\\\debug")
            debug = 0;
        else
        {
            emit path_error(1);
            return;
        }
    }
    file.setFileName(ai[1]);
    if (!file.exists())
    {
        if (ai[1] == "\\\human")
            human = 1;
        else if (ai[1] == "\\\debug")
            debug = 1;
        else
        {
            emit path_error(2);
            return;
        }
    }
    file.setFileName(map_location);
    if (!file.exists())
    {
        emit path_error(0);
        return ;
    }
    start();
}


 void NormalBattle::run()
 {
     if (human == -1 && debug == -1) StartTwoAiBattle();
     else if (debug == -1) StartHumanAiBattle();
     else StartDebugBattle();
 }

 void NormalBattle::StartDebugBattle()
 {
     //初始化管道
    //处理初始化服务端。
    QTime time = QTime::currentTime();
    QLocalServer server[2];
    QString listen_name[2] = {QString("THUEESASTTEAM14"+time.toString("hh")),
                              QString("THUEESASTTEAM14"+time.toString("ss"))};
    server[0].listen(listen_name[0]);
    server[1].listen(listen_name[1]);

    QProcess process;
    QLocalSocket* client[2];

    //连接ai及界面
    process.start(ai[!debug], listen_name[!debug]);
    process.waitForStarted();
    emit ready_for_connect(listen_name[debug]);

 }

 void NormalBattle::connect()
 {
     for (int i=0; i<2; i++)
     {
         if (!server[i].waitForNewConnection(5000,timeout))
         {//等待五秒，若未能连接则返回错误并终止程序
             emit connect_error(i+1);
             return;
         }
         client[i] = server[i].nextPendingConnection();
         client[i]->waitForConnected();
     }
     Battle();
 }

 void NormalBattle::StartTwoAiBattle()
 {
     //初始化管道
    //处理初始化服务端。
    QTime time = QTime::currentTime();
    QLocalServer server[2];
    QString listen_name[2] = {QString("THUEESASTTEAM14"+time.toString("hh")),
                              QString("THUEESASTTEAM14"+time.toString("ss"))};
    server[0].listen(listen_name[0]);
    server[1].listen(listen_name[1]);

    QProcess process[2];
    QLocalSocket* client[2];

    //连接ai
    for (int i=0;i<2;i++)
    {
        process[i].start(ai[i],listen_name[i]);
        if (!server[i].waitForNewConnection(3000,timeout))
        {//等待三秒，若未能连接则返回错误并终止程序
            emit connect_error(i+1);
            return;
        }
        client[i] = server[i].nextPendingConnection();
        client[i]->waitForConnected();
    }
    Battle();
 }

 void NormalBattle::Battle()
 {

 }
