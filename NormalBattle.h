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

//    Replay replay;
    void StartTwoAiBattle();
    void StartHumanAiBattle();
    void Battle();      //���ж�ս

protected:
    void run();

public:

    QString GetReplayPath();

public slots:
    void start_normal_battle(QString side1, QString side2, QString map);//��ʼ��ս
    void connect();
    void stop();            //�ṩ��ֹ�̵߳Ľӿ�

signals:
    void path_error(int );  //���ļ���ַ����ʱ�����ͣ�0�����ͼ��ַ����1��2�ֱ����ai��ַ����
    void connect_error(int ); //��ai�ܵ����ӳ���ʱ�����ͣ�1,2�ֱ��������ai
    void send_winner(int);   //�ж�ʤ����, winner = 0 ����1��ѡ�ֻ�ʤ��winner =1 ����2��ѡ�ֻ�ʤ, winner = 2����ս��ƽ��
                            //�ж�ʤ������3����1�ų�ʱ2�Ż�ʤ��4����2�ų�ʱ1�Ż�ʤ��5����˫����ʱ��ƽ�֡�
    void round(int);//���ص�ǰ���е��ڼ��غ�
    void ready_for_connect(QString name); //�˻���սʱ���عܵ����Ա���ƽ̨����
};


#endif //NORMALBATTLE_H

