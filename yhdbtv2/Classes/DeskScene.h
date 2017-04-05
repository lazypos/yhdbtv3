#ifndef __CDeskScene_H__
#define __CDeskScene_H__

#include "cocos2d.h"
#include "UI/UIButton.h"
#include "CardSprite.h"
#include "MessageQueue.h"
#include <chrono>
USING_NS_CC;

typedef struct
{
	Label* _nickName;
	Label* _score;
	Label* _total;
	Label* _surplus;
	Sprite*_ready;
	LabelAtlas* _time;
	Label* _gone;
	Label* _buchu;
	int 	_x;
	int		_y;
	int		id;
	chrono::time_point<chrono::system_clock> timepoint;
	vector<CardSprite*> perCards;

}st_player_info;
typedef shared_ptr<st_player_info> playerPtr;

bool inline mycompair(CardSprite* c1, CardSprite* c2) {
	return c1->getSeq() > c2->getSeq();
}

class CDeskScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(int desk, int site, const string& name);

    virtual bool init();
   
    CREATE_FUNC(CDeskScene);

 	void onReady(Ref *pSender, ui::Widget::TouchEventType type);
	void onReturn(Ref *pSender, ui::Widget::TouchEventType type);
	void onNoput(Ref *pSender, ui::Widget::TouchEventType type);
	void onPut(Ref *pSender, ui::Widget::TouchEventType type);

	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchMoved(Touch *touch, Event *event);

	void	deskSchedule(float dt);
	void	timeSchedule(float dt);

	inline void	setDeskInfo(int desk, int site, const string& name) {
		_deskNum = desk; _seatNum = site; _playerName = name;
		ostringstream os;
		os << "Desk:" << desk;
		_labelDeskNum->setString(os.str());
		for (int i = 0; i < 4; i++)
			_mapPlayers[(_seatNum + i) % 4] = _vecPlayers[i];
	};

protected:

	void	gameOver();
	void	gameSatrt();
	void	clearDesk(int siteid); 
	void    perPutCards(msgptr ptr); //������һ��
	void	nowPutCards(msgptr ptr); //����ǰ��

	void	getSelectCardList(vector<int>& vec, vector<int>& pos);

	void	zhenglishoupai(vector<int>& vec);

private:
	ui::Button* _btReturn;
	ui::Button* _btReady;
	ui::Button* _btNoput; 
	ui::Button* _btPut;

	bool		_isSatrting = false;
	bool		_isExit = false;

	Label*	_labelDeskNum;
	Label*	_labelWeScore;
	Label*	_labelTheyScore;
	Label*  _labelDeskScore;
	Label*	_labelResult;

	int							_seatNum;	//�Լ�����λ��
	int							_deskNum;	//����
	string						_playerName;//�����
	int							_nowPut = -1; //��ǰ����
	Label*		_infoLabel;//��ǰ�����������
	vector<int>					_vecPerCards; //��һ�γ�����
	vector<CardSprite*>			_lstCards; //�Լ�������
	vector<playerPtr>			_vecPlayers; //��ʼ����ʱ��������Ȼ�����A��id���뵽map��
	map<int, playerPtr>			_mapPlayers;
	CardSprite*					_card_select_per = nullptr;
	Size						visibleSize;
};

#endif // __HELLOWORLD_SCENE_H__
