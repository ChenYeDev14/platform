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
    int debug; //调试模式时代表需调试Ai一方，否则为-1
    bool debug_mode;

//    Replay replay;
    void StartTwoAiBattle();
    void StartHumanAiBattle();
    void StartDebugBattle();
    void Battle();      //进行对战

protected:
    void run();

  //public接口：

public:

    QString GetReplayPath();
    void StartNormalBattle(QString side1, QString side2, QString map);//开始对战，side1,side2为Ai路径，map为地图路径


public slots:

    void connect();
    void stop();            //提供终止线程的接口
    void change_to_debug_mode();    //debugger中转换至调试模式（对exe方的数据请求不予满足，等到调试方请求数据时一起发放），
                                    //且回合停止计数（除非某一方有指令待执行）
                                    //这是为了满足选手暂停下来进行单步调试的需求
    void change_to_run_mode();      //切换回运行模式

signals:
    void path_error(int );  //当文件地址错误时被发送，0代表地图地址错误，1、2分别代表ai地址错误
    void connect_error(int ); //当ai管道连接出错时被发送，1,2分别代表两个ai
    void send_winner(int);   //判断胜利方, winner = 0 代表1号选手获胜，winner =1 代表2号选手获胜, winner = 2代表战斗平局
                            //判断胜利方，3代表1号超时2号获胜，4代表2号超时1号获胜，5代表双方超时的平局。
    void round(int);//返回当前运行到第几回合
    void ready_for_connect(QString name); //人机对战时返回服务名以便与平台连接
};


#endif //NORMALBATTLE_H

