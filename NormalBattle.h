#ifndef NORMALBATTLE_H
#define NORMALBATTLE_H
#include <QString>
#include <QThread>
#include "Logic.h"
#include "replayfile.h"

class QLocalServer;
class QLocalSocket;
class QProcess;

namespace DS14
{

class AiReadWriteThread : public QThread
{
    Q_OBJECT
private:
    QLocalSocket *client;
    PlayerCommand *aiCommand;
    bool writeAlready;  //��һ�غ��Ƿ��Ѹ�Aiд���gameInfo
    bool requested;      //һ�غ���Ai�ڶ���������Ϸ��ϢʱֻΪtrue����һ�غ�һ��ʼ��������Ϸ��Ϣ
    GameInfo gInfo;
public:
    AiReadWriteThread(QLocalSocket *cl, GameInfo &initInfo) :
        client(cl), gInfo(initInfo), aiCommand(NULL), requested(false), writeAlready(false) {}
    PlayerCommand *getCommand() {return aiCommand;}
    void reset(GameInfo &ngInfo)
    {
        gInfo = ngInfo;
        if (aiCommand != NULL) delete aiCommand;
        aiCommand = NULL;
        writeAlready = false;
    }
protected:
    void run();
};

class NormalBattle : public QThread
{
    Q_OBJECT
private:
    QLocalServer *server[2];
    QLocalSocket *client[2];
    QProcess *process[2];
    logic *_logic;
    ReplayFile *rFile;
    AiReadWriteThread *aiThread[2];

    QString ai[2];//ai��ַ
    QString map_location;//��ͼ��ַ
    bool is_stoped;
    int human; //�˻���սʱֵ�����˵�һ��������˫Ai��ս��ֵΪ-1
    int debug; //����ģʽʱ���������Aiһ��������Ϊ-1
    bool debug_mode;
    int timerID;

//    Replay replay;
    void StartTwoAiBattle();
    void StartHumanAiBattle();
    void StartDebugBattle();
    void Battle();      //���ж�ս
    int WhetherWin(const Status &state); //�ж��Ƿ���һ��Ӯ����ai1Ӯ������1��ai2Ӯ������2������Ϸ����������0������Ϸ�ѽ�������Ϊƽ��,����3

protected:
    void run();
    void timerEvent(QTimerEvent *e);
  //public�ӿڣ�

public:

    NormalBattle();
    QString GetReplayPath();
    void StartNormalBattle(QString side1, QString side2, QString map);//��ʼ��ս��side1,side2ΪAi·����mapΪ��ͼ·��
    ~NormalBattle();

public slots:

    void connect();
    void stop();                    //�ṩ��ֹ�̵߳Ľӿ�
    void change_to_debug_mode();    //debugger��ת��������ģʽ����exe�����������������㣬�ȵ����Է���������ʱһ�𷢷ţ���
                                    //�һغ�ֹͣ����������ĳһ����ָ���ִ�У�
                                    //����Ϊ������ѡ����ͣ�������е������Ե�����
    void change_to_run_mode();      //�л�������ģʽ

signals:
    void path_error(int );      //���ļ���ַ����ʱ�����ͣ�0�����ͼ��ַ����1��2�ֱ����ai��ַ����
    void connect_error(int ); //��ai�ܵ����ӳ���ʱ�����ͣ�1,2�ֱ��������ai
    void init_error(int );   //����ȡai��ʼ��Ϣ����ʱ�����ͣ�1,2�ֱ��������ai
    void send_winner(int);   //�ж�ʤ����, winner = 0 ����1��ѡ�ֻ�ʤ��winner =1 ����2��ѡ�ֻ�ʤ, winner = 2����ս��ƽ��
                            //�ж�ʤ������3����1�ų�ʱ2�Ż�ʤ��4����2�ų�ʱ1�Ż�ʤ��5����˫����ʱ��ƽ�֡�
    void round(int);        //���ص�ǰ���е��ڼ��غ�
    void ready_for_connect(QString name); //�˻���սʱ���ط������Ա���ƽ̨����
};



}
#endif //NORMALBATTLE_H

