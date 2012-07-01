#ifndef NORMALBATTLE_H
#define NORMALBATTLE_H
#include <QString>

class QThread;

class NormalBattle : public QThread
{
    Q_OBJECT
private:
    QString ai[2];//ai地址
    QString map_location;//地图地址
    bool is_stoped;
    int human; //人机对战时值代表人的一方，若是双Ai对战，值为-1

//    Replay replay;
    void StartTwoAiBattle();
    void StartHumanAiBattle();
    void Battle();      //进行对战

protected:
    void run();

public:

    QString GetReplayPath();

public slots:
    void start_normal_battle(QString side1, QString side2, QString map);//开始对战
    void connect();
    void stop();            //提供终止线程的接口

signals:
    void path_error(int );  //当文件地址错误时被发送，0代表地图地址错误，1、2分别代表ai地址错误
    void connect_error(int ); //当ai管道连接出错时被发送，1,2分别代表两个ai
    void send_winner(int);   //判断胜利方, winner = 0 代表1号选手获胜，winner =1 代表2号选手获胜, winner = 2代表战斗平局
                            //判断胜利方，3代表1号超时2号获胜，4代表2号超时1号获胜，5代表双方超时的平局。
    void round(int);//返回当前运行到第几回合
    void ready_for_connect(QString name); //人机对战时返回管道名以便与平台连接
};


#endif //NORMALBATTLE_H

