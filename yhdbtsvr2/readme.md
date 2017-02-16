请求信息
*{"opt":"query"}
{"opt":"query","online":"20"}
快速加入
*{"opt":"add"}
{"opt":"add","desk":"12","site":"2","name":"palyer2"}
桌子情况变化
*{"opt":"change","type":"ready","desk":"12","site":"2"}
*{"opt":"change","type":"leave","desk":"12","site":"2"}
{"opt":"change","info":[
                    {"site":"0","name":"player0","ready":"1"},
                    {"site":"1","name":"player1","ready":"0"},
                    {"site":"2","name":"","ready":""},
                    {"site":"3","name":"player3","ready":"1"}]}
游戏开始
{"opt":"start","cards":"12,22,22,32,32"}
游戏结束
{"opt":"over","info":[
                    {"site":"0","name":"player0","result":"-2"},
                    {"site":"1","name":"player1","result":"2"},
                    {"site":"2","name":"player2","result":"-2"},
                    {"site":"3","name":"player3","result":"2"}]}
出牌
*{"opt":"game","desk":"1","site":"2","cards":"12,23,23,23"}
{"opt":"game","per":"0","cards":"12,12,32,32,45,45","surplus":"34", "now":"1"}


