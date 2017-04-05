#include "MessageQueue.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
using rapidjson::Document;
#include "cocos2d.h"
USING_NS_CC;
CMessageQueue::CMessageQueue()
{
#ifdef _WIN32
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif
}


CMessageQueue::~CMessageQueue()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

bool CMessageQueue::start()
{
	struct  sockaddr_in _servAddr;
	memset(&_servAddr, 0, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_port = htons(9999);
	_servAddr.sin_addr.s_addr = inet_addr(Configuration::getInstance()->getValue("host").asString().c_str());
	
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock == 0)
		return false;
	if (connect(_sock, (struct sockaddr*)&_servAddr, sizeof(_servAddr)))
		return false;

	__Dictionary *_dictMgr = __Dictionary::createWithContentsOfFile("config/record.plist");
	const char *p = _dictMgr->valueForKey("name")->getCString();
	if (p == nullptr || strlen(p) == 0) {
		ostringstream oss;
		oss << chrono::system_clock::now().time_since_epoch().count();
		__String skey(oss.str().c_str());
		_dictMgr->setObject(&skey, "name");
		_dictMgr->writeToFile("config/record.plist");
		_playkey = oss.str();
	}
	else
		_playkey = p;

	thread(&CMessageQueue::threadWork, this).detach();
	thread(&CMessageQueue::threadWork2, this).detach();
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
			MessageBox("与服务器断开连接，请重新登录!","error");
			exit(-1);
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
		MessageBox("从服务器接收数据错误, 请重新登陆!", "error");
		exit(-1);
	}
	size_t recvlen = 0;
	text.resize(int(len), 0);
	while (recvlen < len){
		n = recv(_sock, (char*)(text.c_str() + recvlen), len - recvlen, 0);
		if (n <= 0){
			MessageBox("与服务器断开连接, 请重新登陆!", "error");
			exit(-1);
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

string CMessageQueue::getKey() {
	return _playkey;
}

void CMessageQueue::threadWork()
{
	string strRecv;
	while (true){
		strRecv.clear();
		if (!recvMessage(strRecv))
			break;

		Document doc;
		doc.Parse<0>(strRecv.c_str());
		if (doc.HasParseError())
			break;

		msgptr ptr = make_shared<stmsg>();
		string opt = doc["opt"].GetString();
		ptr->opt = opt;
		if (opt == "query"){
			ptr->online = doc["online"].GetString();
			ptr->version = doc["version"].GetString();
			if (ptr->version != "46") {
				MessageBox("版本过低，请前往 http://www.yhdbt.pw 下载新版客户端!", "错误");
				exit(-1);
			}
		}else if (opt == "add"){
			ptr->desk = atoi(doc["desk"].GetString());
			ptr->site = atoi(doc["site"].GetString());
			ptr->name = doc["name"].GetString();
		}else if (opt == "change"){
			for (size_t i = 0; i < 4; ++i) {
				ptr->arrPlayInfo[i].name = doc["info"][i]["name"].GetString();
				ptr->arrPlayInfo[i].ready = atoi(doc["info"][i]["ready"].GetString());
				if (!ptr->arrPlayInfo[i].name.empty()){
					ostringstream osinfo;
					osinfo << "win:" << doc["info"][i]["win"].GetString()
						<< "  lose:" << doc["info"][i]["lose"].GetString()
						<< "  run:" << doc["info"][i]["run"].GetString();
					ptr->arrPlayInfo[i].info = osinfo.str();
					ptr->arrPlayInfo[i].name = ptr->arrPlayInfo[i].name + " " + ptr->arrPlayInfo[i].info;
				}
				else {
					ptr->arrPlayInfo[i].info = "";
				}
			}
		}else if (opt == "start") {
			ptr->cards = doc["cards"].GetString();
		}else if (opt == "over") {
			for (size_t i = 0; i < 4; ++i) {
				//ptr->arrPlayInfo[i].name = doc["info"][i]["name"].GetString();
				ptr->arrPlayInfo[i].result = atoi(doc["info"][i]["result"].GetString());
			}
		}else if (opt == "game") {
			ptr->per = atoi(doc["per"].GetString());
			ptr->surplus = doc["surplus"].GetString();
			ptr->now = atoi(doc["now"].GetString());
			ptr->cards = doc["cards"].GetString();
			ptr->score = atoi(doc["score"].GetString());
			ptr->must = atoi(doc["must"].GetString());
		}else if (opt == "score") {
			ptr->p0score = doc["p0"].GetString();
			ptr->p1score = doc["p1"].GetString();
		}else if (opt == "run"){
			ptr->site = atoi(doc["site"].GetString());
			ptr->name = doc["name"].GetString();
		}

		_muxMsg.lock();
		_lstMessage.emplace_back(ptr);
		_muxMsg.unlock();
	}
	MessageBox("与服务器断开连接, 请重新登陆!", "error");
	exit(-1);
}

void CMessageQueue::threadWork2()
{
	while (true) {
		this_thread::sleep_for(chrono::seconds(20));
		sendMessage("{\"opt\":\"heart\"}");
		sendMessage("{\"opt\":\"query\"}");
	}
}
