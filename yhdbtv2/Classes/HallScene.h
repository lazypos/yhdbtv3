#ifndef __CHallScene_H__
#define __CHallScene_H__

#include "cocos2d.h"
#include "UI/UIButton.h"
USING_NS_CC;

class CHallScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
   
    CREATE_FUNC(CHallScene);

	void OnFastAddDesk(Ref *pSender, ui::Widget::TouchEventType type);

	void onlineSchedule(float dt);

private:
	
	ui::Button* _btFastAdd;
	Label*		_infoLabel;
	bool		_changeScene = false;
};

#endif // __HELLOWORLD_SCENE_H__
