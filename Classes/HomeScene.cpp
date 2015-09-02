#include "HomeScene.h"
#include "PuzzleScene.h"
#include "PrefsScene.h"

using namespace cocos2d;
using namespace std;

CCScene* HomeScene::scene()
{
    CCScene *scene = CCScene::create();
    HomeScene *layer = HomeScene::create();
    scene->addChild(layer);

    return scene;
}

bool HomeScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
    // 背景表示
    showBackground();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// Puzzle画面へ飛ぶボタン
    CCMenuItemImage *btnItem1 = CCMenuItemImage::create("menu/btnFarm.png", "menu/btnFarm.png", this, menu_selector(HomeScene::startPuzzle));
    CCMenu* btn1 = CCMenu::create(btnItem1, NULL);
    btn1->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    m_background->addChild(btn1);

	
	// 設定画面へ飛ぶボタン
	CCMenuItemLabel *btnItem2 = CCMenuItemFont::create("Preferences", this, menu_selector(HomeScene::switchPrefsTab));
	CCMenu* btn2 = CCMenu::create(btnItem2, NULL);
	btn2->setPosition(ccp(winSize.width / 2, winSize.height * 0.0387));
	m_background->addChild(btn2);
    
	return true;
}

// 背景表示
void HomeScene::showBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //背景を生成
    m_background = CCSprite::create(PNG_HOME_BACKGROUND);
    m_background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    addChild(m_background, kZorderBackground, kTagBackground);
}


// PuzzleSceneへ
void HomeScene::startPuzzle(CCObject* pSender)
{
    CCScene* nextScene = PuzzleScene::scene();
    float duration = 1.0f;

    //フェードイン＆フェードアウト
    CCScene* scene = CCTransitionFade::create(duration, nextScene);
    if(scene){
        CCDirector::sharedDirector()->replaceScene(scene);
    }
}

// Prefsへ
void HomeScene::switchPrefsTab(CCObject* pSender)
{
    CCScene* nextScene = PrefsScene::scene();
    float duration = 1.0f;
	
    //フェードイン＆フェードアウト
    CCScene* scene = CCTransitionFade::create(duration, nextScene);
    if(scene){
        CCDirector::sharedDirector()->replaceScene(scene);
    }
}


