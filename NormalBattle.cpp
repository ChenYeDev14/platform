#include "NormalBattle.h"
#include <QTime>
#include <QLocalServer>
#include <QLocalSocket>

 void NormalBattle::start_normal_battle(QString side1, QString side2, QString map)
{
    human = -1;
    ai[0] = side1;
    ai[1] = side2;
    map_location = map;
    检查三个文件地址是否指向一个正确的文件
    QFile file(ai[0]);
    if (!file.exists())
    {
        if (ai[0] == "\\\human")
            human = 0;
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
     if (human == -1) StartTwoAiBattle(); else StartHumanAiBattle();
 }

 void NormalBattle::StartHumanAiBattle()
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
    process.start(ai[!human], listen_name[!human]);
    process.waitForStarted();
    emit ready_for_connect(listen_name[human]);

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
    server[0].listen("THUEESASTTEAM14"+time.toString("hh"));
    server[1].listen("THUEESASTTEAM14"+time.toString("ss"));

    QProcess process[2];
    QLocalSocket* client[2];

    //连接ai
    for (int i=0;i<2;i++)
    {
        if (i==0)
            process[i].start(ai[i],QStringList() << time.toString("hh"));
        else
            process[i].start(ai[i],QStringList() << time.toString("ss"));

        process[i].waitForStarted();
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
