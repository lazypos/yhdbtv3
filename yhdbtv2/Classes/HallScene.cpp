#include "HallScene.h"
#include "SimpleAudioEngine.h"
#include "CardSprite.h"
#include "ResourceManager.h"
#include "ui/UIButton.h"
#include "DeskScene.h"
#include "MessageQueue.h"
#include "CommonFunction.h"
#include <SimpleAudioEngine.h>

using namespace CocosDenshion;

USING_NS_CC;

Scene* CHallScene::createScene()
{
    auto _scene = Scene::create();
    auto layer = CHallScene::create();
	_scene->addChild(layer);
    return _scene;
}

bool CHallScene::init()
{
    if (!Layer::init())
        return false;

	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/bj.mp3", true);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* bj = Sprite::create();
	bj->initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hbeijing"));
	bj->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bj,-1);

	//关闭按钮
	_btClose = ui::Button::create("close.png", "close.png", "close.png");
	_btClose->setPosition(Vec2(visibleSize.width - _btClose->getContentSize().width / 2,
		visibleSize.height - _btClose->getContentSize().height / 2));
	_btClose->addTouchEventListener(CC_CALLBACK_2(CHallScene::OnClose, this));
	_btClose->setEnabled(true);
	this->addChild(_btClose);

	//加入按钮 
	_btFastAdd = ui::Button::create("start.png", "start.png", "start.png");
	_btFastAdd->setPosition(Vec2(visibleSize.width/2, visibleSize.height / 2));
	_btFastAdd->addTouchEventListener(CC_CALLBACK_2(CHallScene::OnFastAddDesk, this));
	_btFastAdd->setEnabled(true);
#ifndef _WIN32
	_btFastAdd->setScale(2.0);
#endif
	this->addChild(_btFastAdd);

	//当前在线人数
	_infoLabel = Label::createWithTTF("Online:0", "fonts/arial.ttf", 25);
	_infoLabel->setColor(Color3B::WHITE);
	_infoLabel->setPosition(Vec2(visibleSize.width / 2,
		visibleSize.height/2 - 80));
	this->addChild(_infoLabel, 2);

	this->schedule(CC_SCHEDULE_SELECTOR(CHallScene::onlineSchedule), float(0.1));
	messageQueue::instance()->sendMessage("{\"opt\":\"query\"}");
    return true;
}

void CHallScene::OnFastAddDesk(Ref *pSender, ui::Widget::TouchEventType type)
{	
	_btFastAdd->setEnabled(false);
	if (type == ui::Widget::TouchEventType::ENDED) {
		_btFastAdd->setEnabled(false);
		messageQueue::instance()->sendMessage("{\"opt\":\"add\"}");
	}
	_btFastAdd->setEnabled(true);
}


void CHallScene::OnClose(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		exit(-1);
	}
}

void CHallScene::onlineSchedule(float delta)
{
	int static times = 1;
	if (times % 100 == 0)
		messageQueue::instance()->sendMessage("{\"opt\":\"query\"}");
	times++;
	
	auto ptr = messageQueue::instance()->getMessage();
	if (ptr && ptr->opt == "query") 
		_infoLabel->setString("Online:" + ptr->online);
	//加入桌子
	if (ptr && ptr->opt == "add") {
		Scene *pScene = CDeskScene::createScene(ptr->desk, ptr->site, ptr->name);
		Director::getInstance()->replaceScene(pScene);
	}
}
