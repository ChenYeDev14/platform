实验平台实现：
———————————————————————————————————————————————————————————————————————————————————

Ai对战：
用户输入两个Ai的路径QString side1,side2 以及地图的路径QString map，
开始战争：NormalBattle::StartNormalBattle(side1, side2, map))
处理显示NormalBattle对象发出的各种消息：SIGNAL(path_error(int ));  //当文件地址错误时被发送，0代表地图地址错误，1、2分别代表ai地址错误
   					SIGNAL(connect_error(int )); //当ai管道连接出错时被发送，1,2分别代表两个ai
    					SIGNAL(send_winner(int));   //判断胜利方, winner = 0 代表1号选手获胜，winner =1 代表2号选手获胜, winner = 2代表战斗平局
                            					//判断胜利方，3代表1号超时2号获胜，4代表2号超时1号获胜，5代表双方超时的平局。
					SIGNAL(round(int)) //目前回合数  （用来做进度条，这个先不做也行）

———————————————————————————————————————————————————————————————————————————————————

Debbuger(这个做得漂亮点，以后可以直接发给选手）：
用户输入一个Ai（exe)的路径QString exe_side，选择需要调试的Ai的side，以及地图的路径QString map，
单击“开始调试”，NormalBattle::StartNormalBattle(QString("\\\debug"), exe_side, map)) 
  	      或者NormalBattle::StartNormalBattle(exe_side, QString("\\\debug"), map))  //根据用户选的side确定

模式切换（运行/调试），初始为运行模式
		切换至调试模式：SLOT(change_to_debug_mode())
		切换回运行模式：SLOT(change_to_run_mode())
    说明：这是新增的，因为今年的执行模式是为连续时间，0.1秒一回合，Ai随时可向平台请求数据，而指令可以一次性提交多个回合的，如果某一回合没有指令则维持原状但不判超时。所以如果进行单步调试的话，调试一方给出的指令的速度会远远慢于正常的AI，所以设计了调试模式，在这一模式下，非调试AI的数据请求不会立即得到满足，而是等到调试一方请求数据时一起发放，而且回合暂停计数除非某一方有指令。

单击“建立连接”，连接到槽：SLOT(connect())
处理显示消息：  SIGNAL(ready_for_connect(QString )) //将返回的管道名称输出，提示用户运行调试版Ai（以后提供特殊的main.cpp）并输入管道名称，然后单击“建立连接”
		SIGNAL(path_error(int )) //当文件地址错误时被发送，0代表地图地址错误，1、2分别代表ai地址错误，若错误的一边是要调试的Ai一边，再次提示用户上述内容
		SIGNAL(connect_error(int )); //当ai管道连接出错时被发送，1,2分别代表两个ai
    		SIGNAL(send_winner(int));   //判断胜利方, winner = 0 代表1号选手获胜，winner =1 代表2号选手获胜, winner = 2代表战斗平局
                            					//判断胜利方，3代表1号超时2号获胜，4代表2号超时1号获胜，5代表双方超时的平局。