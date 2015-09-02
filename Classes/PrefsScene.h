#ifndef __PREFSSCENE_H__
#define __PREFSSCENE_H__

#include "cocos2d.h"
#include "Config.h"

class PrefsScene : public cocos2d::CCLayer
{
protected:
	// Z-Indexの管理
    enum kZOrder
    {
        kZOrderBackground,
		kZOrderLabel,
		kZOrderButton
    };
	// タグの管理
	enum kTag
    {
        kTagBackground = 1,
		kTagDropXLabel,
		kTagDropYLabel,
		kTagDropColorLabel,
		kTagSwitchHomeBtn,
		kTagChangeDropXBtn,
		kTagChangeDropYBtn,
		kTagChangeDropColorBtn
    };
    
	//----------------------------------
	//	メンバ変数
	//----------------------------------
	
	// レイヤスプライト
	cocos2d::CCSprite* m_background;

	//----------------------------------
	//	メソッド
	//----------------------------------

	// レイヤスプライトのセット
	void showBackground();

	// 各種ボタンの配置
	void showButtons();	
	
	// 設定値の表示(更新)
	void updatePrefs();
		
public:
	virtual bool init();
	static cocos2d::CCScene* scene();
	CREATE_FUNC(PrefsScene);

private:
	// ホームに戻る
	void switchHomeTab();

	// ドロップの数(横)を変更
	void increaseDropX();
	void decreaseDropX();
	
	// ドロップの数(縦)を変更
	void increaseDropY();
	void decreaseDropY();
	
	// ドロップの種類を変更
	void increaseDropColor();
	void decreaseDropColor();
};

#endif //__PREFSSCENE_H__
