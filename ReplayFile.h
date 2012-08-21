#ifndef REPLAYFILE_H
#define REPLAYFILE_H
#include "Basic.h"
#include "Logic.h"
#include "Status.h"
#include <QString>


class QFile;

namespace DS14
{
class ReplayFile
{
public:
    ReplayFile();
    //д��
    void NewFile(QString playerName1, QString playerName2, QString mapName);//������Ϣ�½��ط��ļ�
    void WriteInitialInfo(const PlayerInfo &playerInfo1, const PlayerInfo &playerInfo2, const StatusMapInfo &mapInfo);
            //д���ʼ��Ϣ����ң���ͼ��
    void WriteRoundInfo(const Status &roundStatus); //д��غ���Ϣ
    void WriteWinner(int winSide);
    //����
    bool OpenFile(QString path); //�򿪻ط��ļ��������Ƿ�ɹ�
    //���º���Ĭ���ļ��򿪳ɹ�
    void ReadInitialInfo(PlayerInfo &playerInfo1, PlayerInfo &playerInfo2, StatusMapInfo &mapInfo); //������ʼ��Ϣ
    void ReadAllRoundInfo(int &roundNum, Status *statusList[]); //�������лغ���Ϣ,statusList[]Ϊָ�����飬���ö�̬�����ڴ�
    void ReadWinner(int &winSide);
private:
    QFile* file;
};
}

#endif // REPLAYFILE_H
