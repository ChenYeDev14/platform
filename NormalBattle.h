#ifndef NORMALBATTLE_H
#define NORMALBATTLE_H
#include <QString>
#include <QThread>
#include <QTimer>
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
    bool writeAlready;   //��һ�غ��Ƿ��Ѹ�Aiд���gameInfo
    bool requested;      //һ�غ���Ai�ڶ���������Ϸ��ϢʱֻΪtrue����һ�غ�һ��ʼ��������Ϸ��Ϣ
    bool stoped;         //��ͣ��д��֤ƽ̨��ȫ�ύѡ��ָ��
    bool reading;        //�Ƿ�����Ai����
    GameInfo gInfo;
public:
    AiReadWriteThread(QLocalSocket *cl, const GameInfo &initInfo) :
        client(cl), gInfo(initInfo), aiCommand(NULL), requested(false), writeAlready(false), stoped(false) {}
    ~AiReadWriteThread();
    PlayerCommand *getCommand() {return aiCommand;}
    void reset(const GameInfo &ngInfo);
    void waitForReadingCompeleted();
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
    bool game_over;
    int human; //�˻���սʱֵ�����˵�һ��������˫Ai��ս��ֵΪ-1
    int debug; //����ģʽʱ���������Aiһ��������Ϊ-1
    bool debug_mode;
   // int timerID;
    int roundTime;

    void StartTwoAiBattle();
    void StartHumanAiBattle();
    void StartDebugBattle();
    void InitAiInfo();
    int WhetherWin(const Status &state); //�ж��Ƿ���һ��Ӯ����ai1Ӯ������1��ai2Ӯ������2������Ϸ����������0������Ϸ�ѽ�������Ϊƽ��,����3
    void UpDateCommand(PlayerCommand *c1, PlayerCommand *c2);
    void RoundTimer(); //�غϽڵ�

protected:
    void run();
 //   void timerEvent(QTimerEvent *e);
  //public�ӿڣ�

public:

    NormalBattle();
    QString GetReplayPath();
    void StartNormalBattle(QString side1, QString side2, QString map);//��ʼ��ս��side1,side2ΪAi·����mapΪ��ͼ·��
    ~NormalBattle();

public slots:

    void ready_connect();
    void stop();                    //�ṩ��ֹ�̵߳Ľӿ�
    void change_to_debug_mode();    //debugger��ת��������ģʽ����exe�����������������㣬�ȵ����Է���������ʱһ�𷢷ţ���
                                    //�һغ�ֹͣ����������ĳһ����ָ���ִ�У�
                                    //����Ϊ������ѡ����ͣ�������е������Ե�����
    void change_to_run_mode();      //�л�������ģʽ

signals:
    void path_error(int );      //���ļ���ַ����ʱ�����ͣ�0�����ͼ��ַ����1��2�ֱ����ai��ַ����
    void connect_error(int ); //��ai�ܵ����ӳ���ʱ�����ͣ�1,2�ֱ��������ai
    void init_error(int );   //����ȡai��ʼ��Ϣ����ʱ�����ͣ�1,2�ֱ��������ai
    void send_winner(int);   //�ж�ʤ����, winner = 1 ����1��ѡ�ֻ�ʤ��winner =2 ����2��ѡ�ֻ�ʤ, winner = 3����ս��ƽ��
    void round(int);        //���ص�ǰ���е��ڼ��غ�
    void ready_for_connect(QString name); //�˻���սʱ���ط������Ա���ƽ̨����
};



}
#endif //NORMALBATTLE_H

