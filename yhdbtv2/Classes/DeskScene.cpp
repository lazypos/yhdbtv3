#include "DeskScene.h"
#include "SimpleAudioEngine.h"
#include "CardSprite.h"
#include "ResourceManager.h"
#include "ui/UIButton.h"
#include "HallScene.h"
#include "DBTRule.h"
#include "CommonFunction.h"
#include <SimpleAudioEngine.h>
using namespace CocosDenshion;

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

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	_vecPlayers.clear();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* bj = Sprite::create();
	bj->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("beijing"));
	bj->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bj, -1);

	//返回按钮
	_btReturn = ui::Button::create("return.png", "return_press.png", "return_press.png");
	_btReturn->setPosition(Vec2(visibleSize.width - _btReturn->getContentSize().width / 2, 
		visibleSize.height - _btReturn->getContentSize().height / 2));
	_btReturn->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onReturn, this));
	this->addChild(_btReturn);
	//出牌按钮
	_btPut = ui::Button::create("put.png", "put_press.png", "put_press.png");
	_btPut->setPosition(Vec2(visibleSize.width / 2 - 100, 170));
	_btPut->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onPut, this));
	_btPut->setScale(float(0.8));
	_btPut->setVisible(false);
	_btPut->setEnabled(true);
	this->addChild(_btPut,2);
	_btNoput = ui::Button::create("noput.png", "noput_press.png", "noput_press.png");
	_btNoput->setPosition(Vec2(visibleSize.width / 2 + 100, 170));
	_btNoput->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onNoput, this));
	_btNoput->setScale(float(0.8));
	_btNoput->setVisible(false);
	_btNoput->setEnabled(true);
	this->addChild(_btNoput,2);
	_btReady = ui::Button::create("ready.png", "ready_press.png", "ready_press.png");
	_btReady->setPosition(Vec2(visibleSize.width / 2, 170));
	_btReady->addTouchEventListener(CC_CALLBACK_2(CDeskScene::onReady, this));
	_btReady->setScale(float(0.8));
	_btReady->setVisible(true);
	this->addChild(_btReady, 2);

	//加载四个人的信息
	Sprite* userInfo0 = Sprite::create();
	userInfo0->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nickname"));
	userInfo0->setPosition(Vec2(visibleSize.width / 2, userInfo0->getContentSize().height / 2));
	//this->addChild(userInfo0);
	Sprite* userInfo1 = Sprite::create();
	userInfo1->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nickname"));
	userInfo1->setPosition(Vec2(visibleSize.width - userInfo1->getContentSize().width / 2, 
		600 + userInfo1->getContentSize().height));
	//this->addChild(userInfo1);
	Sprite* userInfo2 = Sprite::create();
	userInfo2->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nickname"));
	userInfo2->setPosition(Vec2(userInfo2->getContentSize().width / 2,
		600 + userInfo2->getContentSize().height));
	//this->addChild(userInfo2);
	Sprite* userInfo3 = Sprite::create();
	userInfo3->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nickname"));
	userInfo3->setPosition(Vec2(visibleSize.width / 2,
		visibleSize.height - userInfo3->getContentSize().height / 2));
	//this->addChild(userInfo3);

	//桌号
	//得分
	Sprite* score = Sprite::create();
	score->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("score"));
	score->setPosition(Vec2(score->getContentSize().width / 2, visibleSize.height - score->getContentSize().height / 2));
	this->addChild(score);
	_labelDeskNum = Label::createWithTTF("desk:"+to_string(_deskNum), "fonts/arial.ttf", 20);
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
	_labelDeskScore = Label::createWithTTF("0", "fonts/arial.ttf", 40);
	_labelDeskScore->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2+30));
	this->addChild(_labelDeskScore);

	//四个人->自己
	playerPtr ptr = make_shared<st_player_info>();
	ptr->_nickName = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr->_nickName->setPosition(Vec2(visibleSize.width / 2 -35,
		userInfo0->getContentSize().height / 2));
	ptr->_nickName->setColor(Color3B::RED);
	this->addChild(ptr->_nickName,30);
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
	ptr->_gone = Label::createWithTTF("over", "fonts/arial.ttf", 15);
	ptr->_gone->setColor(Color3B::RED);
	ptr->_gone->setPosition(Vec2(visibleSize.width / 2,
		(visibleSize.height / 2) - 200));
	ptr->_gone->setVisible(false);
	this->addChild(ptr->_gone);
	ptr->_buchu = Label::createWithTTF("pass", "fonts/arial.ttf", 15);
	ptr->_buchu->setColor(Color3B::RED);
	ptr->_buchu->setPosition(Vec2(visibleSize.width / 2,
		(visibleSize.height / 2) - 200));
	ptr->_buchu->setVisible(false);
	this->addChild(ptr->_buchu);
	ptr->_x = visibleSize.width / 2 - 200;
	ptr->_y = 250;
	_vecPlayers.emplace_back(ptr);
	//->下家
	playerPtr ptr1 = make_shared<st_player_info>();
	ptr1->_nickName = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr1->_nickName->setPosition(Vec2(visibleSize.width - 135,
		userInfo1->getPosition().y));
	ptr1->_nickName->setColor(Color3B::RED);
	this->addChild(ptr1->_nickName,3);
	ptr1->_surplus = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr1->_surplus->setPosition(Vec2(visibleSize.width - 135,
		userInfo1->getPosition().y-15));
	ptr1->_surplus->setColor(Color3B::WHITE);
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
	ptr1->_gone = Label::createWithTTF("over", "fonts/arial.ttf", 15);
	ptr1->_gone->setColor(Color3B::RED);
	ptr1->_gone->setPosition(Vec2(visibleSize.width / 2 + 350,
		(visibleSize.height / 2)));
	ptr1->_gone->setVisible(false);
	this->addChild(ptr1->_gone);
	ptr1->_buchu = Label::createWithTTF("pass", "fonts/arial.ttf", 15);
	ptr1->_buchu->setColor(Color3B::RED);
	ptr1->_buchu->setPosition(Vec2(visibleSize.width / 2 + 350,
		(visibleSize.height / 2)));
	ptr1->_buchu->setVisible(false);
	this->addChild(ptr1->_buchu);
	ptr1->_x = visibleSize.width / 2;
	ptr1->_y = visibleSize.height / 2;
	_vecPlayers.emplace_back(ptr1);
	//对家
	playerPtr ptr2 = make_shared<st_player_info>();
	ptr2->_nickName = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr2->_nickName->setPosition(Vec2(visibleSize.width / 2 - 35,
		userInfo3->getPosition().y));
	ptr2->_nickName->setColor(Color3B::RED);
	this->addChild(ptr2->_nickName,3);
	ptr2->_surplus = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr2->_surplus->setPosition(Vec2(visibleSize.width /2 - 35,
		userInfo3->getPosition().y - 15));
	ptr2->_surplus->setColor(Color3B::WHITE);
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
	ptr2->_gone = Label::createWithTTF("over", "fonts/arial.ttf", 15);
	ptr2->_gone->setColor(Color3B::RED);
	ptr2->_gone->setPosition((Vec2(visibleSize.width / 2,
		(visibleSize.height / 2) + 200)));
	ptr2->_gone->setVisible(false);
	this->addChild(ptr2->_gone);
	ptr2->_buchu = Label::createWithTTF("pass", "fonts/arial.ttf", 15);
	ptr2->_buchu->setColor(Color3B::RED);
	ptr2->_buchu->setPosition((Vec2(visibleSize.width / 2,
		(visibleSize.height / 2) + 200)));
	ptr2->_buchu->setVisible(false);
	this->addChild(ptr2->_buchu);
	ptr2->_x = visibleSize.width / 2 - 200;
	ptr2->_y = visibleSize.height / 2 + 200;
	_vecPlayers.emplace_back(ptr2);
	//上家
	playerPtr ptr3 = make_shared<st_player_info>();
	ptr3->_nickName = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr3->_nickName->setPosition(Vec2(70,
		userInfo1->getPosition().y));
	ptr3->_nickName->setColor(Color3B::RED);
	this->addChild(ptr3->_nickName,3);
	ptr3->_surplus = Label::createWithTTF("", "fonts/arial.ttf", 15);
	ptr3->_surplus->setPosition(Vec2(70,
		userInfo1->getPosition().y - 15));
	ptr3->_surplus->setColor(Color3B::WHITE);
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
	ptr3->_gone = Label::createWithTTF("over", "fonts/arial.ttf", 15);
	ptr3->_gone->setColor(Color3B::RED);
	ptr3->_gone->setPosition(Vec2(visibleSize.width / 2 - 280, 600));
	ptr3->_gone->setVisible(false);
	this->addChild(ptr3->_gone);
	ptr3->_buchu = Label::createWithTTF("pass", "fonts/arial.ttf", 15);
	ptr3->_buchu->setColor(Color3B::RED);
	ptr3->_buchu->setPosition(Vec2(visibleSize.width / 2 - 350,
		visibleSize.height / 2));
	ptr3->_buchu->setVisible(false);
	this->addChild(ptr3->_buchu);
	ptr3->_x = 0;
	ptr3->_y = visibleSize.height / 2;
	_vecPlayers.emplace_back(ptr3);

	this->schedule(CC_SCHEDULE_SELECTOR(CDeskScene::deskSchedule), float(0.1));
	this->schedule(CC_SCHEDULE_SELECTOR(CDeskScene::timeSchedule), 1.0);
	//注册触摸事件
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
		_btReady->setEnabled(false);
		_mapPlayers[_seatNum]->_ready->setVisible(true);
		ostringstream os;
		os << "{\"opt\":\"change\",\"type\":\"ready\",\"desk\":" << _deskNum << ",\"site\":" << _seatNum << "}";
		messageQueue::instance()->sendMessage(os.str());
		//清桌面
		clearDesk(-1);
	}
}

void CDeskScene::onReturn(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		if (_isSatrting && !_isExit){
			MessageBox("游戏还未结束，真的要退出吗？再次点击返回按钮将强制退出回到大厅！","提醒");
			_isExit = true;
			return;
		}
		ostringstream os;
		os << "{\"opt\":\"change\",\"type\":\"leave\",\"desk\":"<< _deskNum <<",\"site\":"<< _seatNum <<"}";
		messageQueue::instance()->sendMessage(os.str());
		Scene *hScene = CHallScene::createScene();
		Director::getInstance()->replaceScene(hScene);
	}
}

void CDeskScene::onNoput(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		//一定要出
		if (_vecPerCards.empty())
			return;
		_btNoput->setEnabled(false);
		_btPut->setEnabled(false);
		_btPut->setVisible(false);
		_btNoput->setVisible(false);
		ostringstream os;
		os << "{\"opt\":\"game\",\"desk\":" << _deskNum << ",\"site\":"<<_seatNum<<",\"cards\":\"\"}";
		messageQueue::instance()->sendMessage(os.str());
	}
}

void CDeskScene::onPut(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		vector<int> nowCards;
		vector<int> posCards;
		getSelectCardList(nowCards, posCards);
		if (nowCards.empty())
			return;
		auto r = CDBTRule::isBigger(_vecPerCards, nowCards);
		if (r.first){
			_btPut->setVisible(false);
			_btNoput->setVisible(false);
			_btNoput->setEnabled(false);
			_btPut->setEnabled(false);
			_vecPerCards = nowCards;
			//整理手牌
			for (const auto&it : nowCards){
				auto itper = _lstCards.begin();
				while (itper != _lstCards.end()) {
					if ((*itper)->getSeq() == it) {
						this->removeChild(*itper);
						_lstCards.erase(itper);
						break;
					}
					++itper;
				}
			}
			auto visibleSize = Director::getInstance()->getVisibleSize();
			int i = 0;
			for (auto &it : _lstCards) {
				it->setPosition(Vec2(visibleSize.width / 2 - (_lstCards.size() - 1) * DEF_cardsep / 2 + i * DEF_cardsep + it->getContentSize().width / 4, 45));
				i++;
			}
			//发送信息
			ostringstream os;
			os << "{\"opt\":\"game\",\"desk\":"<<_deskNum<<",\"site\":"<<_seatNum<<",\"cards\":\"";
			for (const auto &it : nowCards)
				os << it << ",";
			os << "\"}";
			messageQueue::instance()->sendMessage(os.str());
			SimpleAudioEngine::getInstance()->playEffect("sound/cp.wav");
			if (r.second == CDBTRule::type_atom && nowCards.size() == 4)
				SimpleAudioEngine::getInstance()->playEffect("sound/3.mp3");
			return;
		}
		MessageBox("出牌不符合规则.","提示");
	}
}

void CDeskScene::deskSchedule(float dt)
{
	auto ptr = messageQueue::instance()->getMessage();
	//变更
	if (ptr && ptr->opt == "change") {
		for (int i = 0; i < 4; i++){
			_mapPlayers[i]->_nickName->setString(ptr->arrPlayInfo[i].name);
			if (ptr->arrPlayInfo[i].name.empty())
				clearDesk(i);
			if (ptr->arrPlayInfo[i].ready == 1)
				//if (!_mapPlayers[i]->_ready->isVisible())
				_mapPlayers[i]->_ready->setVisible(true);
			else
				//if (_mapPlayers[i]->_ready->isVisible())
				_mapPlayers[i]->_ready->setVisible(false);
		}
	}
	//开始
	if (ptr && ptr->opt == "start") {
		gameSatrt();
		SimpleAudioEngine::getInstance()->playEffect("sound/bg.wav");
		vector<string> ctmp;
		stringToVector(ptr->cards, ctmp, ",");
		vector<int> _vecStartCards;
		for (auto it : ctmp)
			_vecStartCards.emplace_back(atoi(it.c_str()));
		sort(_vecStartCards.begin(), _vecStartCards.end());
		auto visibleSize = Director::getInstance()->getVisibleSize();
		for (size_t i = 0; i < _vecStartCards.size(); i++) {
			auto cardSprite = CardSprite::createCardSprite(_vecStartCards[i]);
			cardSprite->setPosition(Vec2(visibleSize.width / 2 - (_vecStartCards.size() - 1) * DEF_cardsep / 2 + i * DEF_cardsep + cardSprite->getContentSize().width / 4, 45));
			this->addChild(cardSprite, 1);
			_lstCards.emplace_back(cardSprite);
		}
	}
	//逃跑
	if (ptr && ptr->opt == "run") {
		ostringstream os;
		os << ptr->site << "->" << ptr->name << ":-10\r\n";
		MessageBox(os.str().c_str(), "玩家逃跑，游戏结束");
		gameOver();
	}
	//结束
	if (ptr && ptr->opt == "over") {
		ostringstream os;
		for (int i = 0; i < 4; i++)
			os << i << ":" << ptr->arrPlayInfo[i].result << "\r\n";
		if (ptr->arrPlayInfo[_seatNum].result > 0) {
			SimpleAudioEngine::getInstance()->playEffect("sound/win.mp3");
			MessageBox(string("恭喜，赢" + to_string(ptr->arrPlayInfo[_seatNum].result) + "局！").c_str(), "游戏结束，你赢了！");
		}
		if (ptr->arrPlayInfo[_seatNum].result < 0) {
			SimpleAudioEngine::getInstance()->playEffect("sound/lose.mp3");
			MessageBox(string("很遗憾，输" + to_string(ptr->arrPlayInfo[_seatNum].result) + "局！").c_str(), "游戏结束，你输了！");
		}
		if (ptr->arrPlayInfo[_seatNum].result == 0)
			MessageBox(string("平局，再接再厉！").c_str(), "游戏结束，平局！");
		gameOver();
	}
	//主流程
	if (ptr && ptr->opt == "game") {
		_labelDeskScore->setString(to_string(ptr->score));
		if (ptr->per != -1)
			perPutCards(ptr);
		if (ptr->now != -1)
			nowPutCards(ptr);
	}
	//得分
	if (ptr && ptr->opt == "score") {
		if (_seatNum % 2 == 0){
			_labelWeScore->setString(ptr->p0score);
			_labelTheyScore->setString(ptr->p1score);
		}else {
			_labelWeScore->setString(ptr->p1score);
			_labelTheyScore->setString(ptr->p0score);
		}
	}
	if (ptr && ptr->opt == "timeout") {
		Scene *hScene = CHallScene::createScene();
		Director::getInstance()->replaceScene(hScene);
	}
}

void CDeskScene::timeSchedule(float dt)
{
	if (_isSatrting && _nowPut != -1){
		auto ptr = _mapPlayers[_nowPut];
		int t = atoi(ptr->_time->getString().c_str());
		t--;
		ptr->_time->setString(to_string(t));
		if (_nowPut == _seatNum && t < 10 && t%2==0)
			SimpleAudioEngine::getInstance()->playEffect("sound/jg.mp3");
		
		//自己超时，强制出牌
		if (t < 0 && _nowPut == _seatNum) {
			ptr->_time->setVisible(false);
			_btPut->setVisible(false);
			_btNoput->setVisible(false);
			ostringstream os;
			//强制出牌
			if (_vecPerCards.empty()) {
				os << "{\"opt\":\"game\",\"desk\":" << _deskNum << ",\"site\":" << _seatNum << ",\"cards\":\"" << _lstCards[0]->getSeq() << "\"}";
				//_vecPerCards.emplace_back(_lstCards[0]->getSeq());
				this->removeChild(_lstCards[0]);
				_lstCards.erase(_lstCards.begin());
				auto visibleSize = Director::getInstance()->getVisibleSize();
				int i = 0;
				for (auto &it : _lstCards) {
					it->setPosition(Vec2(visibleSize.width / 2 - (_lstCards.size() - 1) * DEF_cardsep / 2 + i * DEF_cardsep + it->getContentSize().width / 4, 45));
					i++;
				}
			}
			//no put
			else
				os << "{\"opt\":\"game\",\"desk\":" << _deskNum << ",\"site\":" << _seatNum << ",\"cards\":\"\"}";
			messageQueue::instance()->sendMessage(os.str());
			_nowPut = -1;
		}
	}
}

void CDeskScene::getSelectCardList(vector<int>& vec, vector<int>& pos)
{
	vec.clear();
	int i = 0;
	for (const auto &it : _lstCards) {
		if (it->getPosition().y == 70) {
			pos.emplace_back(i);
			vec.emplace_back(it->getSeq());
		}
		i++;
	}
}

bool CDeskScene::onTouchBegan(Touch *touch, Event *event)
{
	Point p = touch->getLocation();
	for (int i = _lstCards.size(); i > 0; i--) {
		CardSprite* card = _lstCards[i - 1];
		Rect recv = card->getBoundingBox();
		//位置太离谱的不处理
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
			//防止同一张牌颤抖
			if (card != _card_select_per) {
				_card_select_per = card;
				if (card->getPosition().y == 70) {
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
	clearDesk(_seatNum);
	_nowPut = -1;
	_btReady->setVisible(true);
	_btReady->setEnabled(true);
}

void CDeskScene::gameSatrt()
{
	_isSatrting = true;
	for (const auto& it : _mapPlayers) {
		it.second->_ready->setVisible(false);
	}
}

void CDeskScene::clearDesk(int siteid)
{
	_labelWeScore->setString("0");
	_labelTheyScore->setString("0");
	_labelDeskScore->setString("0");
	if (siteid == -1){
		for (size_t i = 0; i < _lstCards.size(); i++)
			this->removeChild(_lstCards[i]);
		_lstCards.clear();
		_btNoput->setVisible(false);
		_btPut->setVisible(false);
		for (int i = 0; i < 4; i++) {
			playerPtr ptr = _mapPlayers[i];
			ptr->_time->setVisible(false);
			ptr->_ready->setVisible(false);
			ptr->_gone->setVisible(false);
			ptr->_buchu->setVisible(false);
			for (size_t i = 0; i < ptr->perCards.size(); i++)
				this->removeChild(ptr->perCards[i]);
			ptr->perCards.clear();
		}
	}
	else {
		playerPtr ptr = _mapPlayers[siteid];
		ptr->_time->setVisible(false);
		ptr->_ready->setVisible(false);
		ptr->_gone->setVisible(false);
		ptr->_buchu->setVisible(false);
	}
}

void CDeskScene::perPutCards(msgptr ptr)
{
	playerPtr perptr = _mapPlayers[ptr->per];
	perptr->_surplus->setString(ptr->surplus);
	//完了
	if (ptr->surplus == "0") {
		perptr->_gone->setVisible(true);
	}
	perptr->_time->setVisible(false);
	if (ptr->cards.empty()) {
		SimpleAudioEngine::getInstance()->playEffect("sound/by.mp3");
		perptr->_buchu->setVisible(true);
	}
	else {
		SimpleAudioEngine::getInstance()->playEffect("sound/cp.wav");
		vector<string> vec;
		stringToVector(ptr->cards, vec, ",");
		int i = 0;
		_vecPerCards.clear();
		for (const auto &it : vec) {
			int seq = atoi(it.c_str());
			auto cardSprite = CardSprite::createCardSprite(seq);
			_vecPerCards.emplace_back(seq);
			perptr->perCards.emplace_back(cardSprite);
			cardSprite->setPosition(
				Vec2(perptr->_x + cardSprite->getContentSize().width + i*DEF_cardsep, perptr->_y));
			this->addChild(cardSprite);
			i++;
		}
		auto ty = CDBTRule::getType(_vecPerCards);
		switch (ty.first)
		{
		case  CDBTRule::type_boom:
			SimpleAudioEngine::getInstance()->playEffect("sound/boom.mp3");
			break;
		case  CDBTRule::type_atom:
			SimpleAudioEngine::getInstance()->playEffect("sound/at.wav");
			break;
		}
	}
}

void CDeskScene::nowPutCards(msgptr ptr)
{
	playerPtr nowptr = _mapPlayers[ptr->now];
	nowptr->_buchu->setVisible(false);
	for (const auto &it : nowptr->perCards)
		this->removeChild(it);
	nowptr->perCards.clear();
	_nowPut = ptr->now;
	nowptr->_time->setString("30");
	nowptr->_time->setVisible(true);

	if (ptr->must) {
		for (int i = 0; i < 4; i++) {
			playerPtr ptr = _mapPlayers[i];
			ptr->_buchu->setVisible(false);
			for (size_t i = 0; i < ptr->perCards.size(); i++)
				this->removeChild(ptr->perCards[i]);
			ptr->perCards.clear();
		}
	}
	//自己
	if (_nowPut == _seatNum){
		if (ptr->must) {
			_vecPerCards.clear();
		}
		else {
			_btNoput->setVisible(true);
			_btNoput->setEnabled(true);
		}
		_btPut->setVisible(true);
		_btPut->setEnabled(true);
	}
}
