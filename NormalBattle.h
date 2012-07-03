#ifndef NORMALBATTLE_H
#define NORMALBATTLE_H
#include <QString>

class QThread;

class NormalBattle : public QThread
{
    Q_OBJECT
private:
    QString ai[2];//ai��ַ
    QString map_location;//��ͼ��ַ
    bool is_stoped;
    int human; //�˻���սʱֵ�����˵�һ��������˫Ai��ս��ֵΪ-1
    int debug; //����ģʽʱ���������Aiһ��������Ϊ-1
    bool debug_mode;

//    Replay replay;
    void StartTwoAiBattle();
    void StartHumanAiBattle();
    void StartDebugBattle();
    void Battle();      //���ж�ս

protected:
    void run();

  //public�ӿڣ�

public:

    QString GetReplayPath();
    void StartNormalBattle(QString side1, QString side2, QString map);//��ʼ��ս��side1,side2ΪAi·����mapΪ��ͼ·��


public slots:

    void connect();
    void stop();            //�ṩ��ֹ�̵߳Ľӿ�
    void change_to_debug_mode();    //debugger��ת��������ģʽ����exe�����������������㣬�ȵ����Է���������ʱһ�𷢷ţ���
                                    //�һغ�ֹͣ����������ĳһ����ָ���ִ�У�
                                    //����Ϊ������ѡ����ͣ�������е������Ե�����
    void change_to_run_mode();      //�л�������ģʽ

signals:
    void path_error(int );  //���ļ���ַ����ʱ�����ͣ�0�����ͼ��ַ����1��2�ֱ����ai��ַ����
    void connect_error(int ); //��ai�ܵ����ӳ���ʱ�����ͣ�1,2�ֱ��������ai
    void send_winner(int);   //�ж�ʤ����, winner = 0 ����1��ѡ�ֻ�ʤ��winner =1 ����2��ѡ�ֻ�ʤ, winner = 2����ս��ƽ��
                            //�ж�ʤ������3����1�ų�ʱ2�Ż�ʤ��4����2�ų�ʱ1�Ż�ʤ��5����˫����ʱ��ƽ�֡�
    void round(int);//���ص�ǰ���е��ڼ��غ�
    void ready_for_connect(QString name); //�˻���սʱ���ط������Ա���ƽ̨����
};


#endif //NORMALBATTLE_H

