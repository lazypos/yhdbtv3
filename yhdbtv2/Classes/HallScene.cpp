#include "HallScene.h"
#include "SimpleAudioEngine.h"
#include "CardSprite.h"
#include "ResourceManager.h"
#include "ui/UIButton.h"
#include "DeskScene.h"
#include "MessageQueue.h"
#include "CommonFunction.h"

USING_NS_CC;
bool		changeScene = false;
bool		bsend = false;
string 						_hallrecvType;
map<string, string>			_hallmapRecv;
mutex						_hallmuxRecv;

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

	auto visibleSize = Director::getInstance()->getVisibleSize();

	//加入按钮 
	_btFastAdd = ui::Button::create("add_desk_fast.png", "add_desk_fast_press.png", "add_desk_fast_press.png");
	_btFastAdd->setAnchorPoint(Vec2(0,0));
	_btFastAdd->setPosition(Vec2(visibleSize.width/2, visibleSize.height / 2));
	_btFastAdd->addTouchEventListener(CC_CALLBACK_2(CHallScene::OnFastAddDesk, this));
	this->addChild(_btFastAdd);

	//当前在线人数
	_infoLabel = Label::createWithTTF("online: 0", "fonts/arial.ttf", 15);
	_infoLabel->setColor(Color3B::WHITE);
	_infoLabel->setAnchorPoint(Vec2(0, 0));
	_infoLabel->setPosition(Vec2(visibleSize.width / 2,
		visibleSize.height/2 - 20));
	this->addChild(_infoLabel, 2);

	this->schedule(CC_SCHEDULE_SELECTOR(CHallScene::onlineSchedule), 1.0);
    return true;
}

void CHallScene::OnFastAddDesk(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
		messageQueue::instance()->sendMessage("{\"opt\":\"add\"}");
}


void CHallScene::onlineSchedule(float delta)
{
	int static times = 0;
	if (times % 10 == 0)
		messageQueue::instance()->sendMessage("{\"opt\":\"query\"}");
	
	auto ptr = messageQueue::instance()->getMessage();
	if (ptr && ptr->opt == "query") 
		_infoLabel->setString("online:" + ptr->online);
	//加入桌子
	if (ptr && ptr->opt == "add") {
		Scene *pScene = CDeskScene::createScene(ptr->deskNum);
		Director::getInstance()->replaceScene(pScene);
	}
	
	times++;
}
