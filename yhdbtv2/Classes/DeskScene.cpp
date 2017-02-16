#include "DeskScene.h"
#include "SimpleAudioEngine.h"
#include "CardSprite.h"
#include "ResourceManager.h"
#include "ui/UIButton.h"
#include "HallScene.h"
#include "MessageQueue.h"
#include "DBTRule.h"
#include "CommonFunction.h"

USING_NS_CC;

#define  DEF_cardsep 18
string 						_recvType;
map<string, string>			_mapRecv;
mutex						_muxRecv;
int							_recvtype = 0;
bool						_clear = false;
bool						_over = false;

Scene* CDeskScene::createScene(int desk, int site, const string& name)
{
	auto _scene = Scene::create();
    auto layer = CDeskScene::create();
	layer->setDeskInfo(desk, site, name);
	_scene->addChild(layer);
    return _scene;
}

bool CDeskScene::init()
{
    if (!Layer::init())
        return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	//���ذ�ť
	_btReturn = ui::Button::create("return.png", "return_press.png", "return_press.png");
	_btReturn->setPosition(Vec2(visibleSize.width - _btReturn->getContentSize().width / 2, 
		visibleSize.height - _btReturn->getContentSize().height / 2));
	_btReturn->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onReturn, this));
	this->addChild(_btReturn);
	//��ǰ�������
	Sprite* score = Sprite::create();
	score->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("score"));
	score->setPosition(Vec2(score->getContentSize().width / 2, visibleSize.height - score->getContentSize().height / 2));
	this->addChild(score);
	//���ư�ť
	_btPut = ui::Button::create("put.png", "put_press.png", "put_press.png");
	_btPut->setPosition(Vec2(visibleSize.width / 2 - 100, 170));
	_btPut->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onPut, this));
	_btPut->setScale(float(0.8));
	_btPut->setVisible(false);
	this->addChild(_btPut,2);
	_btNoput = ui::Button::create("noput.png", "noput_press.png", "noput_press.png");
	_btNoput->setPosition(Vec2(visibleSize.width / 2 + 100, 170));
	_btNoput->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onNoput, this));
	_btNoput->setScale(float(0.8));
	_btNoput->setVisible(false);
	this->addChild(_btNoput,2);
	_btReady = ui::Button::create("ready.png", "ready_press.png", "ready_press.png");
	_btReady->setPosition(Vec2(visibleSize.width / 2, 170));
	_btReady->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onReady, this));
	_btReady->setScale(float(0.8));
	_btReady->setVisible(true);
	this->addChild(_btReady, 2);

	//�����ĸ��˵���Ϣ
	Sprite* userInfo0 = Sprite::create();
	userInfo0->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nickname"));
	userInfo0->setPosition(Vec2(visibleSize.width / 2, userInfo0->getContentSize().height / 2));
	this->addChild(userInfo0, 2);
	Sprite* userInfo1 = Sprite::create();
	userInfo1->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nickname"));
	userInfo1->setPosition(Vec2(visibleSize.width - userInfo1->getContentSize().width / 2, 
		600 + userInfo1->getContentSize().height));
	this->addChild(userInfo1);
	Sprite* userInfo2 = Sprite::create();
	userInfo2->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nickname"));
	userInfo2->setPosition(Vec2(userInfo2->getContentSize().width / 2,
		600 + userInfo2->getContentSize().height));
	this->addChild(userInfo2);
	Sprite* userInfo3 = Sprite::create();
	userInfo3->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nickname"));
	userInfo3->setPosition(Vec2(visibleSize.width / 2,
		visibleSize.height - userInfo3->getContentSize().height / 2));
	this->addChild(userInfo3);

	//����
	_labelDeskNum = Label::createWithTTF("0", "fonts/arial.ttf", 20);
	_labelDeskNum->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_labelDeskNum);
	_labelWeScore = Label::createWithTTF("0", "fonts/arial.ttf", 25);
	_labelWeScore->setPosition(Vec2(80, visibleSize.height - score->getContentSize().height / 2));
	_labelWeScore->setColor(Color3B::BLACK);
	this->addChild(_labelWeScore);
	_labelTheyScore = Label::createWithTTF("0", "fonts/arial.ttf", 25);
	_labelTheyScore->setPosition(Vec2(170, visibleSize.height - score->getContentSize().height / 2));
	_labelTheyScore->setColor(Color3B::BLACK);
	this->addChild(_labelTheyScore);

	//�ĸ���->�Լ�
	playerPtr ptr = make_shared<st_player_info>();
	ptr->_nickName = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr->_nickName->setPosition(Vec2(visibleSize.width / 2 - 135,
		userInfo0->getContentSize().height / 2));
	ptr->_nickName->setColor(Color3B::BLACK);
	this->addChild(ptr->_nickName,3);
	ptr->_surplus = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr->_surplus->setVisible(false);
	this->addChild(ptr->_surplus);
	ptr->_time = LabelAtlas::create("30", "num.png", 14, 21, '0');
	ptr->_time->setPosition(Vec2(visibleSize.width / 2 + 300, 200));
	ptr->_time->setVisible(false);
	this->addChild(ptr->_time);
	ptr->_ready = Sprite::create();
	ptr->_ready->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("zhunbei"));
	ptr->_ready->setPosition(Vec2(visibleSize.width / 2, 
		(visibleSize.height / 2) - 200));
	ptr->_ready->setVisible(false);
	this->addChild(ptr->_ready);
	ptr->_gone = LabelAtlas::create("0", "num.png", 14, 21, '0');
	ptr->_gone->setPosition(Vec2(visibleSize.width / 2 + 300, 200));
	ptr->_gone->setVisible(false);
	this->addChild(ptr->_gone);
	ptr->_x = visibleSize.width / 2 - 200;
	ptr->_y = 250;
	_vecPlayers.push_back(ptr);

	//->�¼�
	playerPtr ptr1 = make_shared<st_player_info>();
	ptr1->_nickName = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr1->_nickName->setPosition(Vec2(visibleSize.width - 135,
		userInfo1->getPosition().y));
	ptr1->_nickName->setColor(Color3B::BLACK);
	this->addChild(ptr1->_nickName);
	ptr1->_surplus = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr1->_surplus->setPosition(Vec2(visibleSize.width - 135,
		userInfo1->getPosition().y));
	ptr1->_surplus->setColor(Color3B::BLACK);
	this->addChild(ptr1->_surplus);
	ptr1->_time = LabelAtlas::create("30", "num.png", 14, 21, '0');
	ptr1->_time->setPosition(Vec2(visibleSize.width / 2 + 260, 600));
	ptr1->_time->setVisible(false);
	this->addChild(ptr1->_time);
	ptr1->_ready = Sprite::create();
	ptr1->_ready->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("zhunbei"));
	ptr1->_ready->setPosition(Vec2(visibleSize.width / 2+350,
		(visibleSize.height / 2)));
	ptr1->_ready->setVisible(false);
	this->addChild(ptr1->_ready);
	ptr1->_gone = LabelAtlas::create("0", "num.png", 14, 21, '0');
	ptr1->_gone->setPosition(Vec2(visibleSize.width / 2 + 260, 600));
	ptr1->_gone->setVisible(false);
	this->addChild(ptr1->_gone);
	ptr1->_x = visibleSize.width / 2;
	ptr1->_y = visibleSize.height / 2;
	_vecPlayers.push_back(ptr1);

	//�Լ�
	playerPtr ptr2 = make_shared<st_player_info>();
	ptr2->_nickName = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr2->_nickName->setPosition(Vec2(visibleSize.width / 2 - 135,
		userInfo3->getPosition().y));
	ptr2->_nickName->setColor(Color3B::BLACK);
	this->addChild(ptr2->_nickName);
	ptr2->_surplus = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr2->_surplus->setPosition(Vec2(visibleSize.width /2 - 35,
		userInfo3->getPosition().y));
	ptr2->_surplus->setColor(Color3B::BLACK);
	this->addChild(ptr2->_surplus);
	ptr2->_time = LabelAtlas::create("30", "num.png", 14, 21, '0');
	ptr2->_time->setPosition(Vec2(visibleSize.width / 2 + 140,
		userInfo3->getPosition().y - 20));
	ptr2->_time->setVisible(false);
	this->addChild(ptr2->_time);
	ptr2->_ready = Sprite::create();
	ptr2->_ready->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("zhunbei"));
	ptr2->_ready->setPosition(Vec2(visibleSize.width / 2,
		(visibleSize.height / 2) + 200));
	ptr2->_ready->setVisible(false);
	this->addChild(ptr2->_ready);
	ptr2->_gone = LabelAtlas::create("0", "num.png", 14, 21, '0');
	ptr2->_gone->setPosition(Vec2(visibleSize.width / 2 + 140,
		userInfo3->getPosition().y - 20));
	ptr2->_gone->setVisible(false);
	this->addChild(ptr2->_gone);
	ptr2->_x = visibleSize.width / 2 - 200;
	ptr2->_y = visibleSize.height / 2 + 200;
	_vecPlayers.push_back(ptr2);
	//�ϼ�
	playerPtr ptr3 = make_shared<st_player_info>();
	ptr3->_nickName = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr3->_nickName->setPosition(Vec2(70,
		userInfo1->getPosition().y));
	ptr3->_nickName->setColor(Color3B::BLACK);
	this->addChild(ptr3->_nickName);
	ptr3->_surplus = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr3->_surplus->setPosition(Vec2(70,
		userInfo1->getPosition().y));
	ptr3->_surplus->setColor(Color3B::BLACK);
	this->addChild(ptr3->_surplus);
	ptr3->_time = LabelAtlas::create("30", "num.png", 14, 21, '0');
	ptr3->_time->setPosition(Vec2(visibleSize.width / 2 - 280, 600));
	ptr3->_time->setVisible(false);
	this->addChild(ptr3->_time);
	ptr3->_ready = Sprite::create();
	ptr3->_ready->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("zhunbei"));
	ptr3->_ready->setPosition(Vec2(visibleSize.width / 2-350,
		visibleSize.height / 2));
	ptr3->_ready->setVisible(false);
	this->addChild(ptr3->_ready);
	ptr3->_gone = LabelAtlas::create("0", "num.png", 14, 21, '0');
	ptr3->_gone->setPosition(Vec2(visibleSize.width / 2 - 280, 600));
	ptr3->_gone->setVisible(false);
	this->addChild(ptr3->_gone);
	ptr3->_x = 0;
	ptr3->_y = visibleSize.height / 2;
	_vecPlayers.push_back(ptr3);

	this->scheduleUpdate();
	this->schedule(CC_SCHEDULE_SELECTOR(CDeskScene::playerSchedule), 1.0);
	this->schedule(CC_SCHEDULE_SELECTOR(CDeskScene::deskSchedule), 1.0);
	//ע�ᴥ���¼�
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CDeskScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CDeskScene::onTouchMoved, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CDeskScene::onReady(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		_btReady->setVisible(false);
		_mapPlayers[_seatNum]->_ready->setVisible(true);
		ostringstream os;
		os << "{\"opt\":\"change\",\"type\":\"ready\",\"desk\":\"" << _deskNum << "\",\"site\":\"" << _seatNum << "\"}";
		messageQueue::instance()->sendMessage(os.str());
		//������
		clearDesk(-1);
	}
}

void CDeskScene::onReturn(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		if (_isSatrting && !_isExit){
			MessageBox("��Ϸ��δ���������Ҫ�˳����ٴε�����ذ�ť��ǿ���˳��ص�������","����");
			_isExit = true;
			return;
		}

		ostringstream os;
		if (_isSatrting) 
			os << "{\"opt\":\"run\", \"site\":\"" << _seatNum << "\", \"name\":\""<< _playerName <<"\"}";
		else 
			os << "{\"opt\":\"change\",\"type\":\"leave\",\"desk\":\""<< _deskNum <<"\",\"site\":\""<< _seatNum <<"\"}";
		messageQueue::instance()->sendMessage(os.str());
		Scene *hScene = CHallScene::createScene();
		Director::getInstance()->replaceScene(hScene);
	}
}

void CDeskScene::onNoput(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		_btPut->setVisible(false);
		_btNoput->setVisible(false);
		messageQueue::instance()->sendMessage("{\"opt\":\"game\"}");
	}
}

void CDeskScene::onPut(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		vector<int> nowCards = getSelectCardList();
		if (!nowCards.empty() && CDBTRule::isBigger(_vecPerCards, nowCards)){
			_btPut->setVisible(false);
			_btNoput->setVisible(false);
			ostringstream os;
			os << "{\"opt\":\"put\",card=\"";
			for (const auto &it : nowCards)
				os << it << ",";
			os << "\"}";
			messageQueue::instance()->sendMessage(os.str());
			return;
		}
		MessageBox("���Ʋ����Ϲ���.","��ʾ");
	}
}

void CDeskScene::update(float dt)
{
	msgptr ptr = messageQueue::instance()->getMessage();
	if (ptr != nullptr){
		if (ptr->opt == "over"){
		}
	}


	lock_guard<mutex> lg(_muxRecv);
	if (!_recvType.empty()){
		if (_recvType == "getmsg"){
			auto it = _mapRecv.find("desknum");//����
			if (it != _mapRecv.end())
				_labelDeskNum->setString(it->second);
			_seatNum = atoi(_mapRecv["id"].c_str());
			int i = _seatNum;
			for (auto it : _vecPlayers) {
				_mapPlayers[i % 4] = it;
				it->id = i % 4;
				i++;
			}
			string players = _mapRecv["player"];
			//��ǰ�����ϵ���Ա��Ϣ
			if (players.length() > 0) {
				list<string> tmp;
				stringToList(players, tmp, "|");
				for (const auto& it : tmp)
					playerAdd(it);
			}
		}else if (_recvType == "play"){
			playerPtr ptr = _mapPlayers[atoi(_mapRecv["id"].c_str())];
			//׼��
			if (_mapRecv["state"] == "ready") {
				ptr->_ready->setVisible(true);
			}
			//��Ϸ��ʼ
			else if (_mapRecv["state"] == "start") {
				_isSatrting = true;
				_btReady->setVisible(false);
				for (const auto&it : _mapPlayers) {
					it.second->_surplus->setString("54");
					it.second->_ready->setVisible(false);
				}
				vector<string> ctmp;
				stringToVector(_mapRecv["cards"], ctmp, ",");
				for (auto it : ctmp)
					_vecStartCards.push_back(atoi(it.c_str()));
				sort(_vecStartCards.begin(), _vecStartCards.end());
				_recvtype = recv_start;
				//˭����
				int putid = atoi(_mapRecv["now"].c_str());
				putCard(putid, true);
			}
			//����뿪
			else if (_mapRecv["state"] == "leave") {
				playerLeave(ptr);
			}
			//��Ҽ���
			else if (_mapRecv["state"] == "add") {
				playerAdd(_mapRecv["player"]);
			}
			//��ҳ���/����
			else if (_mapRecv["state"] == "put") {
				string cards = _mapRecv["cards"];
				int perid = atoi(_mapRecv["per"].c_str());
				int putid = atoi(_mapRecv["now"].c_str());
				int surplus = atoi(_mapRecv["surplus"].c_str());
				putPerCard(perid, cards, surplus, _mapRecv["go"]);
				if (putid != -1)
					putCard(putid, _mapRecv["clear"] == "1");
			}
			//��ҵ÷�
			else if (_mapRecv["state"] == "score") {
				if (_seatNum % 2 == 0) {
					_labelWeScore->setString(_mapRecv["0"]);
					_labelTheyScore->setString(_mapRecv["1"]);
				}
				else {
					_labelWeScore->setString(_mapRecv["1"]);
					_labelTheyScore->setString(_mapRecv["0"]);
				}
			}
			//��Ϸ����
			else if (_mapRecv["state"] == "over") {
				//��һЩ״̬���
				_isSatrting = false;
				_card_select_per = nullptr;
				_isExit = false;
				updateScore(0, atoi(_mapRecv["s0"].c_str()));
				updateScore(1, atoi(_mapRecv["s1"].c_str()));
				updateScore(2, atoi(_mapRecv["s2"].c_str()));
				updateScore(3, atoi(_mapRecv["s3"].c_str()));
				ostringstream os;
				os << _mapRecv["n0"] << " " << _mapRecv["s0"] << "\r\n"
					<< _mapRecv["n1"] << " " << _mapRecv["s1"] << "\r\n"
					<< _mapRecv["n2"] << " " << _mapRecv["s2"] << "\r\n"
					<< _mapRecv["n3"] << " " << _mapRecv["s3"] << "\r\n";
				MessageBox(os.str().c_str(), "��Ϸ����");
				_over = true;
				_clear = true;
			}
		}
		_recvType.clear();
	}

	//һ��ʼ������
	if (_recvtype == recv_start){
		_recvtype = 0;
		auto visibleSize = Director::getInstance()->getVisibleSize();
		for (size_t i = 0; i < _vecStartCards.size(); i++) {
			auto cardSprite = CardSprite::createCardSprite(_vecStartCards[i]);
			cardSprite->setPosition(Vec2(visibleSize.width / 2 - (_vecStartCards.size()-1) * DEF_cardsep / 2 + i * DEF_cardsep + cardSprite->getContentSize().width / 4,
				45));
			this->addChild(cardSprite, 1);
			_lstCards.push_back(cardSprite);
		}
	}
	//������������
	if (_recvtype == recv_review){
		_recvtype = 0;
		auto visibleSize = Director::getInstance()->getVisibleSize();
		int i = 0;
		for (auto &it : _lstCards){
			it->setPosition(Vec2(visibleSize.width / 2 - (_lstCards.size() - 1) * DEF_cardsep / 2 + i * DEF_cardsep + it->getContentSize().width / 4,
				45));
			i++;
		}
	}
	//��һ�γ���
	if (_perptr){
		int i = 0;
		for (auto it:_vecPerCards){
			auto cardSprite = CardSprite::createCardSprite(it);
			cardSprite->setPosition(Vec2(_perptr->_x+cardSprite->getContentSize().width + i*DEF_cardsep,
				_perptr->_y));
			_perptr->perCards.push_back(cardSprite);
			this->addChild(cardSprite);
			i++;
			//������Լ�����������
			if (_perptr->id == _seatNum) {
				auto itper = _lstCards.begin();
				while (itper != _lstCards.end()) {
					if ((*itper)->getSeq() == it){
						this->removeChild(*itper);
						_lstCards.erase(itper);
						break;
					}
					++itper;
				}
				_recvtype = recv_review;
			}
		}
		_perptr = nullptr;
	}
	if (_clear){
		_clear = false;
		for (auto it : _mapPlayers) {
			for (auto iit : it.second->perCards)
				this->removeChild(iit);
		}
	}
	if (_over){
		_over = false;
		_vecPerCards.clear();
		_timenow = nullptr;
		_mustput = false;
		_labelWeScore->setString("0");
		_labelTheyScore->setString("0");
		_btReady->setVisible(true);
		for (auto &it : _lstCards)
			this->removeChild(it);
		for (auto &it : _mapPlayers) {
			it.second->_gone->setVisible(false);
			it.second->_surplus->setString("0");
		}
	}
}


void CDeskScene::playerSchedule(float dt)
{
	//����ʱ
	if (_timenow) {
		int t = atoi(_timenow->getString().c_str());
		t--;
		if (t >= 0) {
			ostringstream os;
			os << t;
			_timenow->setString(os.str());
			if (t == 0) { //������Լ���
				_timenow->setVisible(false);
				for (auto it : _mapPlayers) {
					if (it.second->_time == _timenow && it.first == _seatNum) {
						if (_mustput) {//ǿ�Ƴ���
							_btPut->setVisible(false);
							_btNoput->setVisible(false);
							ostringstream os;
							os << "cmd=desk;type=put;cards=" << _lstCards[0]->getSeq();
							messageQueue::instance()->sendMessage(os.str());
						}
						else {
							_btPut->setVisible(false);
							_btNoput->setVisible(false);
							messageQueue::instance()->sendMessage("cmd=desk;type=noput");
						}
					}
				}
			}
		}
	}
}


void CDeskScene::deskSchedule(float dt)
{
	auto ptr = messageQueue::instance()->getMessage();
	//change
	if (ptr && ptr->opt == "change") {
		
	}
	//��ʼ
	if (ptr && ptr->opt == "change") {
		
	}
	//����
	if (ptr && ptr->opt == "change") {
		
	}
	//����
	if (ptr && ptr->opt == "change") {
		
	}
}

void CDeskScene::ObserverPlaying(Ref* sendmsg)
{
	lock_guard<mutex> lg(_muxRecv);
	__String* p = (__String*)sendmsg;
	map<string, string> mapRst;
	stringToMap(p->_string, mapRst, ";");
	//��ȡ��Ϣ
	if (mapRst["type"] == "getmsg"){
		_recvType = "getmsg";
		_mapRecv = mapRst;
	}
	//��Ϸ
	if (mapRst["type"] == "play"){
		_recvType = "play";
		_mapRecv = mapRst;
	}
}

std::vector<int> CDeskScene::getSelectCardList()
{
	vector<int> vec;
	for_each(_lstCards.begin(), _lstCards.end(), [&vec](CardSprite* p) {
		if (p->getSelect())
			vec.push_back(p->getSeq());
	});
	sort(vec.begin(), vec.end());
	return vec;
}

void CDeskScene::playerLeave(playerPtr ptr)
{
	ptr->_nickName->setString("");
	ptr->_score->setString("");
	ptr->_total->setString("");
	ptr->_win->setString("");
	ptr->_time->setVisible(false);
	ptr->_ready->setVisible(false);
}

void CDeskScene::playerAdd(const string& content)
{
	vector<string> vectmp;
	stringToVector(content, vectmp, "<>");
	playerPtr ptr = _mapPlayers[atoi(vectmp[0].c_str())];
	ptr->_nickName->setString(vectmp[1]);
	ptr->_score->setString(vectmp[2]);
	ptr->_total->setString(vectmp[3]);
	ptr->_win->setString(vectmp[4]);
	if (vectmp[5] == "1")
		ptr->_ready->setVisible(true);
	else
		ptr->_ready->setVisible(false);
}

void CDeskScene::putPerCard(int id, const string& cards, int surplus, const string& go)
{
	//������һ�ҵ�״̬
	if (id == _seatNum){
		_btPut->setVisible(false);
		_btNoput->setVisible(false);
	}
	playerPtr perptr = _mapPlayers[id];
	ostringstream os;
	os << surplus;
	perptr->_surplus->setString(os.str());
	perptr->_time->setVisible(false);
	perptr->perCards.clear();
	_vecPerCards.clear();
	for (auto &it : perptr->perCards)
		this->removeChild(it);
	if (!cards.empty()) {
		vector<string> vec1;
		stringToVector(cards, vec1, ",");
		int i = 0;
		for (const auto &it : vec1) {
			int seq = atoi(it.c_str());
			_vecPerCards.push_back(seq);
			i++;
		}
		sort(_vecPerCards.begin(), _vecPerCards.end());
		_perptr = perptr;
	}
	//������
	if (surplus == 0 && !go.empty()){
		perptr->_gone->setString(go);
		perptr->_gone->setVisible(true);
	}
}

void CDeskScene::putCard(int id, bool isclear /*= false*/)
{
	_putId = id;
	if (id == _seatNum) {
		_btPut->setVisible(true);
		_btNoput->setVisible(true);
	}
	playerPtr ptr = _mapPlayers[id];
	ptr->_time->setString("30");
	ptr->_time->setVisible(true);
	_timenow = ptr->_time;
	if (isclear){
		_clear = true;
	}
}

void CDeskScene::updateScore(int id, int score)
{
	playerPtr ptr = _mapPlayers[id];
	int s = atoi(ptr->_score->getString().c_str());
	s += score;
	ostringstream os;
	os << s;
	ptr->_score->setString(os.str());
}

bool CDeskScene::onTouchBegan(Touch *touch, Event *event)
{
	Point p = touch->getLocation();
	for (int i = _lstCards.size(); i > 0; i--) {
		CardSprite* card = _lstCards[i - 1];
		Rect recv = card->getBoundingBox();
		//λ��̫���׵Ĳ�����
		if (p.y > recv.getMaxY() + 30)
			break;
		if (recv.containsPoint(p)) {
			_card_select_per = card;
			if (card->getSelect()) {
				card->setSelect(false);
				card->setPosition(Vec2(card->getPositionX(), 45));
			}
			else {
				card->setSelect(true);
				card->setPosition(Vec2(card->getPositionX(), 70));
			}
			break;
		}
	}
	return true;
}

void CDeskScene::onTouchMoved(Touch *touch, Event *event)
{
	Point p = touch->getLocation();
	Point per = touch->getPreviousLocation();

	for (size_t i = _lstCards.size(); i > 0; i--) {
		CardSprite* card = _lstCards[i - 1];
		Rect recv = card->getBoundingBox();
		if (p.y > recv.getMaxY() + 30)
			break;
		if (recv.containsPoint(p)) {
			//��ֹͬһ���Ʋ���
			if (card != _card_select_per) {
				_card_select_per = card;
				if (card->getSelect()) {
					card->setSelect(false);
					card->setPosition(Vec2(card->getPositionX(), 45));
				}
				else {
					card->setSelect(true);
					card->setPosition(Vec2(card->getPositionX(), 70));
				}
			}
			break;
		}
	}
}

void CDeskScene::gameOver()
{
	_isSatrting = false;
	_isExit = false;
	_btReady->setVisible(true);
}

void CDeskScene::clearDesk(int siteid)
{

}
