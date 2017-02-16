#ifndef _CMessageQueue_h_
#define _CMessageQueue_h_

#include <memory>
#include <thread>
#include <mutex>
#include "cocos2d.h"
#include "Singleton.h"
using namespace std;
USING_NS_CC;

typedef struct playinfo
{
	string name;
	int site = -1;
	int ready = 0;
	int score = 0;
	int result = 0;
}stplayinfo;

typedef struct stMsg {
	string opt;
	string online;
	string name;
	int desk = -1;
	int site = -1;
	string cards;
	stplayinfo arrPlayInfo[4];
}stmsg;
typedef shared_ptr<stmsg> msgptr;

class CMessageQueue
{
public:
	CMessageQueue();
	~CMessageQueue();

	bool	start();
	bool	sendMessage(const string& str);
	bool	recvMessage(string& text);
	bool	sendAndRecv(const string& strSend, string& strRecv);
	msgptr	getMessage();

private:
	void	threadWork();
	
	list<msgptr>	_lstMessage;
	mutex			_muxMsg;
	SOCKET			_sock;
};

typedef CSingleton<CMessageQueue>	messageQueue;

#endif 