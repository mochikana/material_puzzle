#ifndef __PUZZLESCENE_H__
#define __PUZZLESCENE_H__

#include "cocos2d.h"
#include "Config.h"


class PuzzleScene : public cocos2d::CCLayer
{
protected:
	// Z-Indexの管理
	enum kZOrder
    {
        kZOrderBackground,
		kZOrderField,
		kZOrderLabel,
        kZOrderDrop,
		kZOrderLine
    };
	// タグの管理
    enum kTag
    {
        kTagBackground = 1,
		kTagField,
		kTagRedDropBatch,
		kTagBlueDropBatch,
		kTagGreenDropBatch,
		kTagOrangeDropBatch,
		kTagPurpleDropBatch,
		kTagGrayDropBatch,
		kTagRedLabel,
		kTagBlueLabel,
		kTagGreenLabel,
		kTagOrangeLabel,
		kTagPurpleLabel,
		kTagGrayLabel,
		kTagScoreLabel,
		kTagChainLine,
        kTagBaseDrop = 10000,
    };
	struct PositionIndex
	{
		PositionIndex(int x1, int y1)
		{
			x = x1;
			y = y1;
		}
		int x;
		int y;
	};

	//----------------------------------
	//	メンバ変数
	//----------------------------------

	// レイヤスプライト
    cocos2d::CCSprite* m_background;
    cocos2d::CCSprite* m_field;
	
	// ドロップ
	std::vector<kDrop> dropTypes;
	std::map<kDrop, std::list<int> > m_dropTags;
	
	// スコア
	int m_removeCount;
	std::vector<int>m_removeDropCount;
	
	// ドロップの大きさ
	float m_dropSize;

	// 消去対象リスト
	int m_lastTargetTag;
	std::list<int> m_targetDropTags;

	
	//----------------------------------
	//	ユーティリティ関数
	//----------------------------------
	
	// オフセット取得
	cocos2d::CCPoint getOffset(int x, int y); // フィールドスプライトに持たせたい
	cocos2d::CCPoint getOffsetByTag(int tag); // ドロップスプライトに持たせたい
	
	// タグ取得
	int getTagByPosition(int posX, int posY);
	int getTagByPoint(cocos2d::CCPoint touchPoint);
	
	// 座標取得
	PositionIndex getPositionByTag(int tag);
	
	// 判定系
	bool hasSameDropList(std::list<int> dropTagList, int tag);
	bool isNextDrop(int tag1, int tag2);
	bool isSameColor(int tag1, int tag2);

	//----------------------------------
	//	メソッド
	//----------------------------------

	// レイヤスプライトのセット
	void showBackground();
	void showField();
	void setImageBatch();
	
	// スコアを更新
	void updateScore();

	// 変数の初期化
	void initForVariables();

	// ドロップをセット
	void fillDrop();
	std::map<int, bool> getExistDrop(); // ドロップが存在するか

	// 消去対象リストにセット
	void setTargetDropTag(int tag);
	void drawLine();
	void deleteLine();
	
	// ドロップを消去
	void removeDrop();
	
	// ドロップの移動
	void searchNewPosition();
	void setNewPosition(int tag, PositionIndex posIndex);	//ドロップスプライトに持たせたい
	void moveDrop();
	
	// 詰み判定
	bool hasRemovableDrop();
	std::list<int> getChainDropList(int tag);
	void swapingDrop();
	
	// ドロップのアニメーション
	void dropAnimation();

	
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(PuzzleScene);
	
	// タップを有効化
	virtual bool ccTouchBegan(cocos2d:: CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchMoved(cocos2d:: CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchEnded(cocos2d:: CCTouch* pTouch, cocos2d::CCEvent* pEvent);

private:
    void myCallback(CCObject* pSender);
};


#endif // __PUZZLESCENE_H__