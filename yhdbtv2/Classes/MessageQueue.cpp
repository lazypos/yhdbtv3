#include "MessageQueue.h"
#include "cocos2d/external/json/document.h"
#include "cocos2d/external/json/stringbuffer.h"
#include "cocos2d/external/json/writer.h"
using rapidjson::Document;

CMessageQueue::CMessageQueue()
{
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
}


CMessageQueue::~CMessageQueue()
{
	WSACleanup();
}

bool CMessageQueue::start()
{
	/*string strRecv = "{qwe:\"123\",ww:\"222\"}";
	Document doc;
	doc.Parse<0>(strRecv.c_str());
	if (doc.HasParseError()) 
		return false;
	string s = doc["qwe"].GetString();*/

	struct  sockaddr_in _servAddr;
	memset(&_servAddr, 0, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_port = htons(9999);
	_servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock == 0)
		return false;
	if (connect(_sock, (struct sockaddr*)&_servAddr, sizeof(_servAddr)))
		return false;

	thread(&CMessageQueue::threadWork, this).detach();
	return true;
}

bool CMessageQueue::sendMessage(const string& str)
{
	size_t len = 0;
	size_t nSend = str.length();
	string strReply;
	strReply.resize(4);
	memcpy((char*)strReply.c_str(), &nSend, 4);
	strReply += str;
	while (len < strReply.length()) {
		int n = send(_sock, strReply.c_str() + len, strReply.length() - len, 0);
		if (n <= 0){
			MessageBox("断开连接，请重新登录!","error");
			return false;
		}
		len += n;
	}
	return true;
}

bool CMessageQueue::recvMessage(string& text)
{
	size_t len = 0;
	int n = recv(_sock, (char*)&len, 4, 0);
	if (n != 4 || len <= 0) {
		MessageBox("从服务器接收数据错误.", "error");
		return false;
	}
	size_t recvlen = 0;
	text.resize(int(len), 0);
	while (recvlen < len){
		n = recv(_sock, (char*)(text.c_str() + recvlen), len - recvlen, 0);
		if (n <= 0){
			MessageBox("与服务器断开连接!", "error");
			return false;
		}
		recvlen += n;
	}
	return true;
}

bool CMessageQueue::sendAndRecv(const string& strSend, string& strRecv)
{
	if (sendMessage(strSend))
		return recvMessage(strRecv);
	return false;
}

msgptr	CMessageQueue::getMessage() {
	unique_lock<mutex> ul(_muxMsg);
	if (_lstMessage.empty()){
		return nullptr;
	}
	msgptr ptr = _lstMessage.front();
	_lstMessage.pop_front();
	return ptr;
}

void CMessageQueue::threadWork()
{
	string strRecv;
	while (true){
		strRecv.clear();
		if (!recvMessage(strRecv))
			break;

		msgptr ptr = make_shared<stmsg>();
		_muxMsg.lock();
		_lstMessage.emplace_back(ptr);
		_muxMsg.unlock();
	}
}
