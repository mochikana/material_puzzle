#include "PrefsScene.h"
#include "HomeScene.h"

using namespace cocos2d;
using namespace std;

CCScene* PrefsScene::scene()
{
    CCScene *scene = CCScene::create();
    PrefsScene *layer = PrefsScene::create();
    scene->addChild(layer);
	
    return scene;
}

bool PrefsScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
    // 背景表示
    showBackground();
	
	// ボタンの表示
	showButtons();
	
	// 設定値の表示(更新)
	updatePrefs();
	
	return true;
}



/*------------------------------------------------------------------
	レイヤスプライトのセット
	void showBackground();
	cocos2d::CCSprite* m_background;
 ------------------------------------------------------------------*/

void PrefsScene::showBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //背景を生成
    m_background = CCSprite::create(PNG_PREFS_BACKGROUND);
    m_background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    addChild(m_background, kZOrderBackground, kTagBackground);
}



/*------------------------------------------------------------------
	各種ボタンの配置
	void showButtons();
 ------------------------------------------------------------------*/

void PrefsScene::showButtons()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// ホーム画面に戻るボタン
	CCMenuItemLabel *btnItem2 = CCMenuItemFont::create("Home", this, menu_selector(PrefsScene::switchHomeTab));
	CCMenu* btn2 = CCMenu::create(btnItem2, NULL);
	btn2->setPosition(ccp(winSize.width / 2, winSize.height * 0.0387));
	m_background->addChild(btn2, kZOrderLabel, kTagSwitchHomeBtn);


	// ドロップの数(横)を変更するボタン
	CCMenuItemImage *btnPlusDropX = CCMenuItemImage::create(PNG_PLUS_BTN, PNG_PLUS_HL_BTN, this, menu_selector(PrefsScene::increaseDropX));
	CCMenuItemImage *btnMinusDropX = CCMenuItemImage::create(PNG_MINUS_BTN, PNG_MINUS_HL_BTN, this, menu_selector(PrefsScene::decreaseDropX));
	CCMenu* menuChangeDropX = CCMenu::create(btnPlusDropX, btnMinusDropX, NULL);
	menuChangeDropX->setPosition(ccp(winSize.width * 0.7969, winSize.height * 0.6452));
	menuChangeDropX->alignItemsHorizontallyWithPadding(29.0f);
	m_background->addChild(menuChangeDropX, kZOrderButton, kTagChangeDropXBtn);

	
	// ドロップの数(縦)を変更するボタン
	CCMenuItemImage *btnPlusDropY = CCMenuItemImage::create(PNG_PLUS_BTN, PNG_PLUS_HL_BTN, this, menu_selector(PrefsScene::increaseDropY));
	CCMenuItemImage *btnMinusDropY = CCMenuItemImage::create(PNG_MINUS_BTN, PNG_MINUS_HL_BTN, this, menu_selector(PrefsScene::decreaseDropY));
	CCMenu* menuChangeDropY = CCMenu::create(btnPlusDropY, btnMinusDropY, NULL);
	menuChangeDropY->setPosition(ccp(winSize.width * 0.7969, winSize.height * 0.4771));
	menuChangeDropY->alignItemsHorizontallyWithPadding(29.0f);
	m_background->addChild(menuChangeDropY, kZOrderButton, kTagChangeDropYBtn);
	
	
	// ドロップの種類を変更するボタン
	CCMenuItemImage *btnPlusDropColor = CCMenuItemImage::create(PNG_PLUS_BTN, PNG_PLUS_HL_BTN, this, menu_selector(PrefsScene::increaseDropColor));
	CCMenuItemImage *btnMinusDropColor = CCMenuItemImage::create(PNG_MINUS_BTN, PNG_MINUS_HL_BTN, this, menu_selector(PrefsScene::decreaseDropColor));
	CCMenu* menuChangeDropColor = CCMenu::create(btnPlusDropColor, btnMinusDropColor, NULL);
	menuChangeDropColor->setPosition(ccp(winSize.width * 0.7969, winSize.height * 0.2623));
	menuChangeDropColor->alignItemsHorizontallyWithPadding(29.0f);
	m_background->addChild(menuChangeDropColor, kZOrderButton, kTagChangeDropColorBtn);
}



/*------------------------------------------------------------------
	設定値の表示(更新)
	void updatePrefs();
 ------------------------------------------------------------------*/
void PrefsScene::updatePrefs()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	
	// ドロップの数(横)
	const char* dropFieldX = ccsf("%d", user->getIntegerForKey("dropFieldX", MAX_DROP_X));
	
	CCLabelBMFont* dropXLabel = (CCLabelBMFont*)m_background->getChildByTag(kTagDropXLabel);
	if(!dropXLabel)
	{
		dropXLabel = CCLabelBMFont::create(dropFieldX, FONT_NUMBER);
		dropXLabel->setPosition(ccp(winSize.width * 0.3375, winSize.height * 0.6452));
		m_background->addChild(dropXLabel, kZOrderLabel, kTagDropXLabel);
	}
	else
	{
		dropXLabel->setString(dropFieldX);
	}
	
	
	// ドロップの数(縦)
	const char* dropFieldY = ccsf("%d", user->getIntegerForKey("dropFieldY", MAX_DROP_Y));
	
	CCLabelBMFont* dropYLabel = (CCLabelBMFont*)m_background->getChildByTag(kTagDropYLabel);
	if(!dropYLabel)
	{
		dropYLabel = CCLabelBMFont::create(dropFieldY, FONT_NUMBER);
		dropYLabel->setPosition(ccp(winSize.width * 0.3375, winSize.height * 0.4771));
		m_background->addChild(dropYLabel, kZOrderLabel, kTagDropYLabel);
	}
	else
	{
		dropYLabel->setString(dropFieldY);
	}

	
	// ドロップの種類
	const char* dropColor = ccsf("%d", user->getIntegerForKey("dropColor", kDropCount));
	
	CCLabelBMFont* dropColorLabel = (CCLabelBMFont*)m_background->getChildByTag(kTagDropColorLabel);
	if(!dropColorLabel)
	{
		dropColorLabel = CCLabelBMFont::create(dropColor, FONT_NUMBER);
		dropColorLabel->setPosition(ccp(winSize.width * 0.3375, winSize.height * 0.2623));
		m_background->addChild(dropColorLabel, kZOrderLabel, kTagDropColorLabel);
	}
	else
	{
		dropColorLabel->setString(dropColor);
	}
}



/*------------------------------------------------------------------
	ドロップの数(横)を変更
	void increaseDropX();
	void decreaseDropX();
 ------------------------------------------------------------------*/

void PrefsScene::increaseDropX()
{
	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	int dropX = user->getIntegerForKey("dropFieldX", MAX_DROP_X);

	dropX++;

	// バリデーション
	if (dropX < 4)
	{
		dropX = 4;
	}
	else if (10 < dropX)
	{
		dropX = 10;
	}

	user->setIntegerForKey("dropFieldX", dropX);
	user->flush();
	
	updatePrefs();
}

void PrefsScene::decreaseDropX()
{
	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	int dropX = user->getIntegerForKey("dropFieldX", MAX_DROP_X);

	dropX--;

	// バリデーション
	if (dropX < 4)
	{
		dropX = 4;
	}
	else if (10 < dropX)
	{
		dropX = 10;
	}
	
	user->setIntegerForKey("dropFieldX", dropX);
	user->flush();
	
	updatePrefs();
}



/*------------------------------------------------------------------
	ドロップの数(縦)を変更
	void increaseDropY();
	void decreaseDropY();
 ------------------------------------------------------------------*/

void PrefsScene::increaseDropY()
{
	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	int dropY = user->getIntegerForKey("dropFieldY", MAX_DROP_Y);
	
	dropY++;

	// バリデーション
	if (dropY < 4)
	{
		dropY = 4;
	}
	else if (10 < dropY)
	{
		dropY = 10;
	}
	
	user->setIntegerForKey("dropFieldY", dropY);
	user->flush();
	
	updatePrefs();
}

void PrefsScene::decreaseDropY()
{
	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	int dropY = user->getIntegerForKey("dropFieldY", MAX_DROP_Y);
	
	dropY--;

	// バリデーション
	if (dropY < 4)
	{
		dropY = 4;
	}
	else if (10 < dropY)
	{
		dropY = 10;
	}
		
	user->setIntegerForKey("dropFieldY", dropY);
	user->flush();
	
	updatePrefs();
}



/*------------------------------------------------------------------
 ドロップの種類を変更
 void increaseDropColor();
 void decreaseDropColor();
 ------------------------------------------------------------------*/

void PrefsScene::increaseDropColor()
{
	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	int dropColor = user->getIntegerForKey("dropColor", kDropCount);
	
	dropColor++;

	// バリデーション
	if (dropColor < 3)
	{
		dropColor = 3;
	}
	else if (6 < dropColor)
	{
		dropColor = 6;
	}
	
	user->setIntegerForKey("dropColor", dropColor);
	user->flush();
	
	updatePrefs();
}

void PrefsScene::decreaseDropColor()
{
	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	int dropColor = user->getIntegerForKey("dropColor", kDropCount);

	dropColor--;

	// バリデーション
	if (dropColor < 3)
	{
		dropColor = 3;
	}
	else if (6 < dropColor)
	{
		dropColor = 6;
	}

	user->setIntegerForKey("dropColor", dropColor);
	user->flush();
	
	updatePrefs();
}



/*------------------------------------------------------------------
	ホームに戻る
	void switchHomeTabe();
 ------------------------------------------------------------------*/

void PrefsScene::switchHomeTab()
{
    CCScene* nextScene = HomeScene::scene();
    float duration = 1.0f;
	
    //フェードイン＆フェードアウト
    CCScene* scene = CCTransitionFade::create(duration, nextScene);
    if(scene){
        CCDirector::sharedDirector()->replaceScene(scene);
    }
}


