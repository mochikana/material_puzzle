#ifndef __HOMESCENE_H__
#define __HOMESCENE_H__

#include "cocos2d.h"
#include "Config.h"

class HomeScene : public cocos2d::CCLayer
{
protected:
    enum kTag
    {
        kTagBackground = 1,
    };
    
    enum kZOrder
    {
        kZorderBackground,
    };

    cocos2d::CCSprite* m_background;
    void showBackground();
    
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(HomeScene);

private:
	void switchPrefsTab(CCObject* pSender);
    void startPuzzle(CCObject* pSender);
};

#endif // __HOMESCENE_H__