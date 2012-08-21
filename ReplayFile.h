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
    //写入
    void NewFile(QString playerName1, QString playerName2, QString mapName);//根据信息新建回放文件
    void WriteInitialInfo(const PlayerInfo &playerInfo1, const PlayerInfo &playerInfo2, const StatusMapInfo &mapInfo);
            //写入初始信息（玩家，地图）
    void WriteRoundInfo(const Status &roundStatus); //写入回合信息
    void WriteWinner(int winSide);
    //读出
    bool OpenFile(QString path); //打开回放文件，返回是否成功
    //以下函数默认文件打开成功
    void ReadInitialInfo(PlayerInfo &playerInfo1, PlayerInfo &playerInfo2, StatusMapInfo &mapInfo); //读出初始信息
    void ReadAllRoundInfo(int &roundNum, Status *statusList[]); //读出所有回合信息,statusList[]为指针数组，采用动态分配内存
    void ReadWinner(int &winSide);
private:
    QFile* file;
};
}

#endif // REPLAYFILE_H
