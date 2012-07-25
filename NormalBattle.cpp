#include "NormalBattle.h"
#include <QTime>
#include <QLocalServer>
#include <QLocalSocket>
#include <QProcess>
#include <QFile>
#include <QFileInfo>

using namespace DS14;
NormalBattle::NormalBattle()
{
    roundTime = 1;
    for (int i=0; i<2; i++)
    {
        server[i] = new QLocalServer;
        process[i] = new QProcess;
        aiThread[i] = NULL;
    }
}

NormalBattle::~NormalBattle()
{
    for (int i=0; i<2; i++)
    {
        delete server[i];
        delete process[i];
    }

}

/**********************************************************************************************************
 共用函数
 **********************************************************************************************************/
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
    terminate();
    start();
}

int NormalBattle::WhetherWin(const Status &state)
{
    if (state.roundNumber == GAME_ROUNDS)
    {
        if (state.potInfo.AI1PotNumber > state.potInfo.AI2PotNumber) return 1;
        else if (state.potInfo.AI1PotNumber < state.potInfo.AI2PotNumber) return 2;
        else if (state.AI1gold > state.AI2gold) return 1;
        else if (state.AI1gold < state.AI2gold) return 2;
        else return 3;
    }
    else
    {
        if (state.potInfo.AI1PotNumber == 0)
            if (state.potInfo.AI2PotNumber == 0)
            {
                if (state.AI1gold > state.AI2gold) return 1;
                else if (state.AI1gold < state.AI2gold) return 2;
                else return 3;
            }
            else return 2;
        else if (state.potInfo.AI2PotNumber == 0) return 1;
    }
    return 0;
}

void NormalBattle::run()
{
    if (human == -1 && debug == -1) StartTwoAiBattle();
    else if (debug == -1) StartHumanAiBattle();
    else StartDebugBattle();
}

void NormalBattle::InitAiInfo()
{
    PlayerInfo pInfo[2];
    QString playerName[2];
    for (int i=0; i<2; i++)
    {
        if (!client[i]->waitForReadyRead())
        {
            for (int j=0; j<2; j++) process[j]->terminate();
            emit init_error(i+1);
        }
        client[i]->read((char*)&pInfo[i], sizeof(PlayerInfo));
        playerName[i] = playerName[i].fromWCharArray(pInfo[i].teamName);

    }
    _logic = new logic;
    _logic->init(map_location.toStdString());
    QFileInfo mapFile(map_location);
    rFile = new ReplayFile;
    rFile->NewFile(playerName[0], playerName[1], mapFile.baseName());
}
/**********************************************************************************************************
 调试对战函数
 **********************************************************************************************************/

void NormalBattle::StartDebugBattle()
{
    //初始化管道
   //处理初始化服务端。
   QTime time = QTime::currentTime();
   QString listen_name[2] = {QString(time.toString("hh-")),
                             QString(time.toString("ss"))};
   server[0]->listen("DS14"+listen_name[0]);
   server[1]->listen("DS14"+listen_name[1]);

   //连接ai及界面
   process[!debug]->start(ai[!debug], QStringList()<<listen_name[!debug]);
   process[!debug]->waitForStarted();
   emit ready_for_connect(listen_name[debug]); //发送管道名，显示给选手，选手手动输入Ai.exe后，通过调试器调用NormalBattle::connect进行连接
}

void NormalBattle::ready_connect()
{
    for (int i=0; i<2; i++)
    {
        if (!server[i]->waitForNewConnection(3000))
        {//等待三秒，若未能连接则返回错误并终止程序
            process[!debug]->terminate();
            emit connect_error(i+1);
            return;
        }
        client[i] = server[i]->nextPendingConnection();
        client[i]->waitForConnected();
    }

    //开始对战
    InitAiInfo();
    char f;
    PlayerCommand *command[2];
    command[0] = NULL;
    command[1] = NULL;
    QTime time;
    time.start();
    while (true)
    {
        if (debug_mode)     //单步调试模式
        {
            client[debug]->waitForReadyRead(-1);
            client[debug]->read(&f, sizeof(f));
            if (f == 'r')
            {
                GameInfo gInfo = _logic->toPlayer(debug);
                client[debug]->write((char*)&gInfo, sizeof(gInfo));
            }
            else if (f == 'w')
            {
                command[debug] = new PlayerCommand;
                client[debug]->waitForReadyRead(-1);
                client[debug]->read((char*)command[debug], sizeof(PlayerCommand));
            }

            client[!debug]->waitForReadyRead(-1);
            client[!debug]->read(&f, sizeof(f));
            if (f == 'r')
            {
                GameInfo gInfo = _logic->toPlayer(!debug);
                client[!debug]->write((char*)&gInfo, sizeof(gInfo));
            }
            else if (f == 'w')
            {
                command[!debug] = new PlayerCommand;
                client[!debug]->waitForReadyRead(-1);
                client[!debug]->read((char*)command[!debug], sizeof(PlayerCommand));
            }

            if (command[0] != NULL || command[1] != NULL)
            {
                UpDateCommand(command[0], command[1]);
                command[0] = NULL;
                command[1] = NULL;
                time.restart();
            }
        }
        else    //非单步调试，正常运行
        {
            for (int i=0; i<2; i++)
                if (client[i]->waitForReadyRead(0))
                {
                    if (f == 'r')
                    {
                        GameInfo gInfo = _logic->toPlayer(i);
                        client[i]->write((char*)&gInfo, sizeof(gInfo));
                    }
                    else if (f == 'w')
                    {
                        if (command[i] == NULL) command[i] = new PlayerCommand;
                        client[i]->waitForReadyRead(-1);
                        client[i]->read((char*)command[i], sizeof(PlayerCommand));
                    }
                }
            if (time.elapsed() >= roundTime)
            {
                UpDateCommand(command[0], command[1]);
                command[0] = NULL;
                command[1] = NULL;
                time.restart();
            }
        }
    }
}

void NormalBattle::UpDateCommand(PlayerCommand *c1, PlayerCommand *c2)
{
    _logic->update(c1, c2);
    delete c1;
    delete c2;
    Status state = _logic->getStatus();
    emit round(state.roundNumber);
    rFile->WriteRoundInfo(state);
    int winner = WhetherWin(state);
    if (winner)
    {
        rFile->WriteWinner(winner);
        emit send_winner(winner);
        delete _logic;
        _logic = NULL;
        delete rFile;
        rFile = NULL;
        return;
    }
}

void NormalBattle::change_to_debug_mode()
{
    if (debug == -1) return;
    debug_mode = true;
}

void NormalBattle::change_to_run_mode()
{
    if (debug == -1) return;
    debug_mode == false;
}


/**********************************************************************************************************
 人机对战函数
 **********************************************************************************************************/
void NormalBattle::StartHumanAiBattle()
{
}



/**********************************************************************************************************
 Ai对战函数
 **********************************************************************************************************/

void NormalBattle::StartTwoAiBattle()
{
    //初始化管道
    //处理初始化服务端。
   QTime time = QTime::currentTime();
   QString listen_name[2] = {QString(time.toString("hh-")),
                             QString(time.toString("ss"))};
   if (server[0]->isListening()) server[0]->close();
   if (server[1]->isListening()) server[1]->close();
   server[0]->listen("DS14"+listen_name[0]);
   server[1]->listen("DS14"+listen_name[1]);


   //连接ai
   for (int i=0;i<2;i++)
   {
       process[i]->start(ai[i],QStringList()<<listen_name[i]);
       process[i]->waitForStarted(-1);
       if (!server[i]->waitForNewConnection(3000))
       {//等待三秒，若未能连接则返回错误并终止程序
           for (int j=0; j<2; j++) process[j]->kill();
           emit connect_error(i+1);
           return;
       }
       client[i] = server[i]->nextPendingConnection();
       client[i]->waitForConnected();
   }
   //开始对战
   InitAiInfo();
   for (int i=0; i<2; i++)
   {
       aiThread[i] = new AiReadWriteThread(client[i], _logic->toPlayer(i+1));
       aiThread[i]->start();
   }
   game_over = false;
   time.start();
   while(true && !game_over)
       if (time.elapsed() >= roundTime)
       {
           RoundTimer();
           time.restart();
       }
}


void NormalBattle::RoundTimer()
{
    if (_logic->getStatus().roundNumber % 2)
    {
        aiThread[0]->waitForReadingCompeleted();
        aiThread[1]->waitForReadingCompeleted();
    }
    else
    {
        aiThread[1]->waitForReadingCompeleted();
        aiThread[0]->waitForReadingCompeleted();
    }
    _logic->update(aiThread[0]->getCommand(), aiThread[1]->getCommand());
    Status state = _logic->getStatus();
    emit round(state.roundNumber);
    rFile->WriteRoundInfo(state);
    int winner = WhetherWin(state);
    if (winner)
    {
        game_over = true;
        rFile->WriteWinner(winner);
        emit send_winner(winner);
        for (int i=0; i<2; i++)
        {
            aiThread[i]->terminate();
            process[i]->kill();
        }
        //killTimer(timerID);
        delete _logic;
        _logic = NULL;
        delete rFile;
        rFile = NULL;
        return;
    }
    for (int i=0; i<2; i++)
    {
        aiThread[i]->reset(_logic->toPlayer(i+1));
    }
}

void NormalBattle::stop()
{
    //killTimer(timerID);
    for (int i=0; i<2; i++)
        if (aiThread[i] != NULL)
        {
            aiThread[i]->terminate();
            delete aiThread[i];
            aiThread[i] = NULL;
        }
    if (_logic != NULL) delete _logic;
    if (rFile != NULL) delete rFile;
    _logic = NULL;
    rFile = NULL;
}

void AiReadWriteThread::run()
{
    while (true)
    {
        reading = false;
        client->waitForReadyRead(-1);
        char f;
        if (!stoped)
        {
            reading = true;
            client->read(&f, sizeof(f));
            if (f == 'r' || requested)
            {
                if (!writeAlready)
                {
                    client->write((char*)&gInfo, sizeof(gInfo));
                    writeAlready = true;
                    requested = false;
                }
                else requested = true;
            }
            else if (f == 'w')
            {
                if (aiCommand == NULL) aiCommand = new PlayerCommand;
                client->waitForReadyRead(-1);
                client->read((char*)aiCommand, sizeof(PlayerCommand));
            }
        }
    }
}

void AiReadWriteThread::waitForReadingCompeleted()
{
    stoped = true;
    while (reading);
}

void AiReadWriteThread::reset(const GameInfo &ngInfo)
{
    gInfo = ngInfo;
    if (aiCommand != NULL) delete aiCommand;
    aiCommand = NULL;
    writeAlready = false;
    stoped = false;
}

AiReadWriteThread::~AiReadWriteThread()
{
    if (aiCommand != NULL) delete aiCommand;
}


