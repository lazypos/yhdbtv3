#ifndef _CMessageQueue_h_
#define _CMessageQueue_h_

#include <memory>
#include <thread>
#include <mutex>
#include "cocos2d.h"
#include "Singleton.h"
using namespace std;
USING_NS_CC;

#ifdef _WIN32
typedef SOCKET socket_t;
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef int socket_t;
#endif

typedef struct playinfo
{
	string name;
	int site = -1;
	int ready = 0;
	int score = 0;
	int result = 0;
	string	info;
}stplayinfo;

typedef struct stMsg {
	string opt;
	string online;
	string name;
	int desk = -1;
	int site = -1;
	string cards;
	int per = -1;
	string	surplus;
	int now = -1;
	int score = 0;
	int must = 0;
	string p0score;
	string p1score;
	string version;
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
	string  getKey();

private:
	void	threadWork();
	void	threadWork2();
	
	list<msgptr>	_lstMessage;
	mutex			_muxMsg;
	socket_t		_sock;
	string			_playkey;
};

typedef CSingleton<CMessageQueue>	messageQueue;

#endif 