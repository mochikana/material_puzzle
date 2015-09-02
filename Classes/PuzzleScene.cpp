#include "PuzzleScene.h"
#include "HomeScene.h"
#include "DropSprite.h"

using namespace cocos2d;
using namespace std;

CCScene* PuzzleScene::scene()
{
    CCScene *scene = CCScene::create();
    PuzzleScene *layer = PuzzleScene::create();
    scene->addChild(layer);
	
    return scene;
}

bool PuzzleScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

    // タップイベントを有効化
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	// 変数初期化
	initForVariables();

	// 背景表示
	showBackground();
	showField();

	// スコアを更新
	updateScore();
	
	// ドロップをセット
	fillDrop();	

	// ホームに戻るボタン
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCMenuItemImage *btnItem = CCMenuItemImage::create("menu/btnList.png", "menu/btnList.png", this, menu_selector(PuzzleScene::myCallback));
    CCMenu* btn = CCMenu::create(btnItem, NULL);
    btn->setPosition(ccp(80 , winSize.height - 44));
    this->addChild(btn);
	
	return true;
}



/*------------------------------------------------------------------
	 変数の初期化
	 void initForVariables();
 ------------------------------------------------------------------*/

void PuzzleScene::initForVariables()
{
	// 乱数の初期化
	srand((unsigned)time(NULL));
	
	// ドロップのスケール
	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	int dropFieldX = user->getIntegerForKey("dropFieldX", MAX_DROP_X);
	int dropFieldY = user->getIntegerForKey("dropFieldY", MAX_DROP_Y);
	float scale = 1;
	
	if ( MAX_DROP_X < dropFieldX || MAX_DROP_Y < dropFieldY)
	{
		if(dropFieldX > dropFieldY)
		{
			scale = (float) MAX_DROP_Y / dropFieldX;
		}
		else
		{
			scale = (float) MAX_DROP_Y / dropFieldY;
		}
	}

	
	// ドロップの大きさ
	DropSprite* pDrop = DropSprite::createWithDropType(kDropRed);
	m_dropSize = pDrop->getContentSize().width * scale;

	// ドロップの種類
	dropTypes.push_back(kDropRed);
	dropTypes.push_back(kDropBlue);
	dropTypes.push_back(kDropGreen);
	dropTypes.push_back(kDropOrange);
	dropTypes.push_back(kDropPurple);
	dropTypes.push_back(kDropGray);

	// スコア用
	m_removeCount = 0;
	m_removeDropCount.resize(kDropCount);
	
	// 消去リスト
	m_targetDropTags.clear();
	m_lastTargetTag = 0;
}



/*------------------------------------------------------------------
	レイヤスプライトのセット
	void showBackground();
	cocos2d::CCSprite* m_background;

	void showField();
	cocos2d::CCSprite* m_field;
 
	void setImageBatch();
 ------------------------------------------------------------------*/

//- 背景スプライトをセット
void PuzzleScene::showBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //背景を生成
    m_background = CCSprite::create(PNG_PUZZLE_BACKGROUND);
    m_background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    addChild(m_background, kZOrderBackground, kTagBackground);
}

//- フィールドスプライトをセット
void PuzzleScene::showField()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //背景を生成
    m_field = CCSprite::create(PNG_PUZZLE_FIELD);
    m_field->setPosition(ccp(winSize.width / 2, winSize.height * 0.6004));

	addChild(m_field, kZOrderField, kTagField);
}



/*------------------------------------------------------------------
	スコアのセット
	void updateScore();
 ------------------------------------------------------------------*/

// スコアラベルをセット
void PuzzleScene::updateScore()
{
	CCSize bgSize = m_background->getContentSize();
	
	// 消した色の数
	int tagsForLabel[]={kTagRedLabel, kTagBlueLabel, kTagGreenLabel, kTagOrangeLabel, kTagPurpleLabel, kTagGrayLabel};
	float heightRate[] = {0.2086, 0.1576, 0.1065, 0.2086, 0.1576, 0.1065};
	float widthRate[] = {0.2344, 0.2344, 0.2344, 0.7297, 0.7297, 0.7297};
	
	vector<kDrop>::iterator it = dropTypes.begin();
	while (it != dropTypes.end())
	{
		int count = m_removeDropCount[*it];
		const char* countStr = ccsf("%d", count);
		
		CCLabelBMFont* label = (CCLabelBMFont*)m_background->getChildByTag(tagsForLabel[*it]);
		if (!label)
		{
			label = CCLabelBMFont::create(countStr, FONT_NUMBER);
			label->setPosition(ccp(bgSize.width * widthRate[*it], bgSize.height * heightRate[*it]));
			m_background->addChild(label, kZOrderLabel, tagsForLabel[*it]);
		}
		else
		{
			label->setString(countStr);
		}
		
		it++;
	}
	
	// 消した回数
	const char* scoreStr = ccsf("%d", m_removeCount);
	
	CCLabelBMFont* scoreLabel = (CCLabelBMFont*)m_background->getChildByTag(kTagScoreLabel);
	if (!scoreLabel)
	{
		scoreLabel = CCLabelBMFont::create(scoreStr, FONT_NUMBER);
		scoreLabel->setPosition(ccp(bgSize.width * 0.2938, bgSize.height * 0.8644));
		m_background->addChild(scoreLabel, kZOrderLabel, kTagScoreLabel);
	}
	else
	{
		scoreLabel->setString(scoreStr);
	}
}



/*------------------------------------------------------------------
	タップイベント
	virtual bool ccTouchBegan(cocos2d:: CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	std::list<int> m_targetDropTags;

	virtual void ccTouchMoved(cocos2d:: CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchEnded(cocos2d:: CCTouch* pTouch, cocos2d::CCEvent* pEvent);
 ------------------------------------------------------------------*/

bool PuzzleScene::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	// タップした座標ポイントを取得
	CCPoint touchPoint = m_field->convertTouchToNodeSpace(pTouch);

	// タップしたドロップのタグを消去リストに追加
	int tag = getTagByPoint(touchPoint);
	if (tag != 0)
	{
		setTargetDropTag(tag);
	}

	return true;
}

void PuzzleScene::ccTouchMoved(cocos2d:: CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	// タップした座標ポイントを取得
	CCPoint touchPoint = m_field->convertTouchToNodeSpace(pTouch);

	// ドロップをタップしていれば消去リストに追加
	int tag = getTagByPoint(touchPoint);
	if (tag != 0 && m_lastTargetTag != tag)
	{
		setTargetDropTag(tag);
	}
	m_lastTargetTag = tag;

}

void PuzzleScene::ccTouchEnded(cocos2d:: CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	// 消去対象リストに3つ以上入っていれば
	if (m_targetDropTags.size() > 2)
	{
		//ドロップを消去
		removeDrop();
		// ドロップをアニメーション;
		dropAnimation();
		// ドロップを埋める
		fillDrop();
		
		// スコアを更新
		updateScore();
	}
	m_targetDropTags.clear();
	deleteLine();
}




/*------------------------------------------------------------------
	ドロップをセット
	void fillDrop();
	bool getExistDrop(int tag);
 ------------------------------------------------------------------*/

void PuzzleScene::fillDrop()
{
	// ドロップの存在マップをロード
	map<int, bool> xDrop = getExistDrop();

	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	
	int dropFieldX = user->getIntegerForKey("dropFieldX", MAX_DROP_X);
	int dropFieldY = user->getIntegerForKey("dropFieldY", MAX_DROP_Y);
	float scale = 1;
	
	if ( MAX_DROP_X < dropFieldX || MAX_DROP_Y < dropFieldY)
	{
		if(dropFieldX > dropFieldY)
		{
			scale = (float) MAX_DROP_Y / dropFieldX;
		}
		else
		{
			scale = (float) MAX_DROP_Y / dropFieldY;
		}
	}
	
	for (int x = 0; x < dropFieldX; x++)
	{
		for (int y = 0; y < dropFieldY; y++)
		{
			int tag = getTagByPosition(x, y);
			
			// ドロップが存在しなければ新しいドロップで埋める
			if(!xDrop[tag])
			{
				// ドロップの抽選
				int dropColor = user->getIntegerForKey("dropColor", kDropCount);
				kDrop dropType = (kDrop)(rand() % dropColor);
				
				m_dropTags[dropType].push_back(tag);
				
				DropSprite* pDrop = DropSprite::createWithDropType(dropType);
				pDrop->setPosition(getOffset(x, y));
				pDrop->setScale(0);
				m_field->addChild(pDrop, kZOrderDrop, tag);
				
				CCActionInterval* action = CCScaleTo::create(TIME_DROP_CREATE, scale);
				pDrop->runAction(action);
			}
		}
	}
}

// ドロップの存在マップを生成
map<int, bool> PuzzleScene::getExistDrop()
{
	// 検索配列の初期化
	map<int, bool> xDrop;

	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	
	int dropFieldX = user->getIntegerForKey("dropFieldX", MAX_DROP_X);
	int dropFieldY = user->getIntegerForKey("dropFieldY", MAX_DROP_Y);
	
	for (int x = 0; x < dropFieldX; x++)
	{
		for (int y = 0; y < dropFieldY; y++)
		{
			int tag= getTagByPosition(x, y);
			xDrop[tag] = false;
		}
	}
	
	// ドロップの検索
	vector<kDrop>::iterator it1 = dropTypes.begin();
	while (it1 != dropTypes.end())
	{
		list<int>::iterator it2 = m_dropTags[*it1].begin();
		while (it2 != m_dropTags[*it1].end())
		{
			xDrop[*it2] = true;
			it2++;
		}
		it1++;
	}
	
	return xDrop;
}



/*------------------------------------------------------------------
	消去リストにドロップをセット
	void setTargetDropTag(int tag);
	void drawLine();
	void deleteLine();
 ------------------------------------------------------------------*/

void PuzzleScene::setTargetDropTag(int tag){
	// 最初のタグならセット
	if (m_targetDropTags.empty())
	{
		m_targetDropTags.push_back(tag);
	}

	// 消去リストに存在していれば消去リストを巻き戻す
	if(hasSameDropList(m_targetDropTags, tag))
	{
		list<int>::reverse_iterator rit = m_targetDropTags.rbegin();
		while (*rit != tag)
		{
			m_targetDropTags.remove(*rit);
			deleteLine();
			drawLine();
		}
	}
	// 最後のタグと隣接かつ同色で既に消去リストに存在しなければ追加
	else if(isNextDrop(m_targetDropTags.back(), tag) && isSameColor(m_targetDropTags.back(), tag))
	{
		m_targetDropTags.push_back(tag);
		drawLine();
	}
}

void PuzzleScene::drawLine(){
	CCDrawNode* draw = (CCDrawNode*)m_field->getChildByTag(kTagChainLine);
	if(!draw)
	{
		draw = CCDrawNode::create();
		draw->setPosition(ccp(0,0));
		m_field->addChild(draw, kZOrderLine, kTagChainLine);
	}

	// チェインラインを描画
	if (m_targetDropTags.size() > 1)
	{
		cocos2d::CCPoint start = getOffsetByTag(m_targetDropTags.front());

		list<int>::iterator it = m_targetDropTags.begin();
		while (it != m_targetDropTags.end())
		{
			if (it != m_targetDropTags.begin())
			{
				cocos2d::CCPoint end = getOffsetByTag(*it);
				draw->drawSegment(start, end, 5, ccc4FFromccc3B(ccRED));
				start = end;
			}
			it++;
		}
	}
}

void PuzzleScene::deleteLine()
{
	m_field->removeChildByTag(kTagChainLine);
}


/*------------------------------------------------------------------
	ドロップを消去
	void removeDrop(std::list<int> removeDropTags);
 ------------------------------------------------------------------*/

void PuzzleScene::removeDrop()
{
	// 消した回数と数をスコアにセット
	// 消した回数
	m_removeCount++;
	
	// 消したドロップの数
	int tag = m_targetDropTags.front();
	DropSprite* dropSprite = (DropSprite*)m_field->getChildByTag(tag);
	kDrop dropType = dropSprite->getdropType();
	m_removeDropCount[dropType] += m_targetDropTags.size();
	
	// ドロップ配列から削除
	list<int>::iterator it1 = m_targetDropTags.begin();
	while (it1 != m_targetDropTags.end())
	{
		vector<kDrop>::iterator it2 = dropTypes.begin();
		while (it2 != dropTypes.end())
		{
			m_dropTags[*it2].remove(*it1);
			CCNode* drop = m_field->getChildByTag(*it1);
			if (drop)
			{
				drop->removeFromParentAndCleanup(true);
			}
			it2++;
		}
		it1++;
	}
}


/*------------------------------------------------------------------
	ドロップを移動
	void searchNewPosition();
	void setNewPosition(int tag, PositionIndex posIndex);	//ドロップスプライトに持たせたい
	void moveDrop();
 ------------------------------------------------------------------*/

// 移動するドロップを探す
void PuzzleScene::searchNewPosition()
{
	list<int>::iterator it1 = m_targetDropTags.begin();
	while (it1 != m_targetDropTags.end())
	{
		PositionIndex removeDropPos = getPositionByTag(*it1);
		vector<kDrop>::iterator it2 = dropTypes.begin();
		while (it2 != dropTypes.end())
		{
			list<int>::iterator it3 = m_dropTags[*it2].begin();
			while(it3 != m_dropTags[*it2].end())
			{
				PositionIndex DropPos = getPositionByTag(*it3);
				
				// ドロップの位置が消えるドロップの上なら
				if ( DropPos.x == removeDropPos.x && DropPos.y > removeDropPos.y)
				{
					setNewPosition(*it3, DropPos);
				}
				it3++;
			}
			it2++;
		}
		it1++;
	}
}

// 移動するドロップに移動先を設定
void PuzzleScene::setNewPosition(int tag, PuzzleScene::PositionIndex posIndex)
{
	DropSprite* dropSprite = (DropSprite*)m_field->getChildByTag(tag);
	int nextY = dropSprite->getNextPosY();
	if (nextY == -1)
	{
		nextY = posIndex.y;
	}
	
	dropSprite->setNextPos(posIndex.x, --nextY);
}

// ドロップを移動
void PuzzleScene::moveDrop()
{
	vector<kDrop>::iterator it1 = dropTypes.begin();
	while (it1 != dropTypes.end()){
		list<int>::iterator it2 = m_dropTags[*it1].begin();
		while (it2 != m_dropTags[*it1].end())
		{
			DropSprite* dropSprite = (DropSprite*)m_field->getChildByTag(*it2);
			int nextX = dropSprite->getNextPosX();
			int nextY = dropSprite->getNextPosY();
			
			if (nextX != -1 || nextY != -1)
			{
				// 新しいタグをセット
				int newTag = getTagByPosition(nextX, nextY);
				dropSprite->initNextPos();
				dropSprite->setTag(newTag);
				
				// タグ一覧の値も新しいタグにセット
				*it2 = newTag;
				
				// アニメーションをセット
				CCMoveTo* move = CCMoveTo::create(TIME_DROP_MOVE, getOffset(nextX, nextY));
				dropSprite->runAction(move);
			}
			it2++;
		}
		it1++;
	}
}



/*------------------------------------------------------------------
	ドロップのアニメーション
	void dropAnimation();
 ------------------------------------------------------------------*/

void PuzzleScene::dropAnimation()
{
	searchNewPosition();
	moveDrop();
}



/*------------------------------------------------------------------
	詰み判定
	bool hasRemovableDrop();
	std::list<int> getChainDropList(int tag);
	void swapingDrop();
 ------------------------------------------------------------------*/

bool PuzzleScene::hasRemovableDrop(){
	vector<kDrop>::iterator it1 = dropTypes.begin();
	while (it1 != dropTypes.end())
	{
		list<int>::iterator it2 = m_dropTags[*it1].begin();
		while (it2 != m_dropTags[*it1].end())
		{
			if(getChainDropList(*it2).size() > 2)
			{
				return true;
			}
			it2++;
		}
		it1++;
	}
	
	return false;
}

// 同色で繋がっているドロップのリストを取得
std::list<int> PuzzleScene::getChainDropList(int tag)
{
	std::list<int> chainDropList;
	chainDropList.push_back(tag);
	DropSprite* dropSprite = (DropSprite*)m_field->getChildByTag(tag);
	kDrop dropType = dropSprite->getdropType();
	
	list<int>::iterator it = chainDropList.begin();
	while(it != chainDropList.end())
	{
		PositionIndex dropPos = getPositionByTag(*it);
		
		// 奇数段はずれているので判定条件が変わる
		int tags[6];
		tags[0] = *it + 100;
		tags[1] = *it - 100;
		tags[2] = *it + 1;
		tags[3] = *it - 1;
		
		if(dropPos.y % 2 != 0)
		{
			tags[4] = *it + 100 + 1;
			tags[5] = *it + 100 - 1;
		}
		else
		{
			tags[4] = *it - 100 + 1;
			tags[5] = *it - 100 - 1;
		}
		
		for (int i = 0; i < sizeof(tags) / sizeof(tags[0]); i++)
		{
			// 既にリストにないか
			if (!hasSameDropList(chainDropList, tags[i]))
			{
				// 色ごとのドロップリストにあるか
				if(hasSameDropList(m_dropTags[dropType], tags[i]))
				{
					chainDropList.push_back(tags[i]);
				}
			}
		}
		
		it++;
	}
	
	return chainDropList;
}

// ドロップを交換して詰み解消
void PuzzleScene::swapingDrop()
{
	while (hasRemovableDrop())
	{
		// 一番上のドロップのひとつを抽選しドロップを再抽選
		int x = (int)(rand() % MAX_DROP_X);
		int tag = getTagByPosition(x, MAX_DROP_Y);
		kDrop dropType = (kDrop)(rand() % kDropCount);

		// ドロップ削除
		vector<kDrop>::iterator it = dropTypes.begin();
		while (it != dropTypes.end()){
			m_dropTags[*it].remove(tag);
			CCNode* drop = m_field->getChildByTag(tag);
			if (drop)
			{
				drop->removeFromParentAndCleanup(true);
			}
		}
		// ドロップ再追加
		DropSprite* pDrop = DropSprite:: createWithDropType(dropType);
		pDrop->setPosition(getOffset(x, MAX_DROP_Y));
		m_field->addChild(pDrop, kZOrderDrop, tag);
	}
}




/*------------------------------------------------------------------
	ユーティリティ関数
	cocos2d::CCPoint getOffset(int x, int y);

	int getTagByPosition(int posX, int posY);
	int getTagByPoint(cocos2d::CCPoint touchPoint);

	PositionIndex getPositionByTag(int tag);

	bool hasSameDropList(std::list<int> dropTagList, int tag);

	bool isNextDrop(int tag1, int tag2);
	bool isSameColor(int tag1, int tag2);
 ------------------------------------------------------------------*/

// ドロップのオフセットを取得
cocos2d::CCPoint PuzzleScene::getOffset(int x, int y)
{
	float offsetX = m_field->getContentSize().width * 0.0547;
	float offsetY = m_field->getContentSize().height * 0;
	
	// yが奇数の時は高さをずらす
	if (y % 2 != 0)
	{
		offsetX += m_dropSize / 2;
	}
	
	return CCPoint((x + 0.5) * m_dropSize + offsetX, (y + 0.5) * m_dropSize + offsetY);
}

// タグからオフセットを取得
cocos2d::CCPoint PuzzleScene::getOffsetByTag(int tag)
{
	PositionIndex pos = getPositionByTag(tag);
	return getOffset(pos.x, pos.y);
}

// 座標インデックスからタグ取得
int PuzzleScene::getTagByPosition(int posX, int posY)
{
	return kTagBaseDrop + posX * 100 + posY;
}

// 座標ポイントからタグ取得
int PuzzleScene::getTagByPoint(cocos2d::CCPoint touchPoint)
{
	cocos2d::CCUserDefault* user = cocos2d::CCUserDefault::sharedUserDefault();
	
	int dropFieldX = user->getIntegerForKey("dropFieldX", MAX_DROP_X);
	int dropFieldY = user->getIntegerForKey("dropFieldY", MAX_DROP_Y);

	for (int x = 0; x < dropFieldX; x++)
	{
		for (int y = 0; y < dropFieldY; y++)
		{
			int currentTag = getTagByPosition(x, y);
			CCNode* node = m_field->getChildByTag(currentTag);
			if (node && node->boundingBox().containsPoint(touchPoint))
			{
				return currentTag;
			}
		}
	}
	return 0;
}

// タグから座標インデックスを取得
PuzzleScene::PositionIndex PuzzleScene::getPositionByTag(int tag)
{
	int x = (tag - kTagBaseDrop) / 100;
	int y = (tag - kTagBaseDrop) % 100;
	
	return PositionIndex(x, y);
}

// リストにドロップがあるか
bool PuzzleScene::hasSameDropList(std::list<int> dropTagList, int tag)
{
	list<int>::iterator it = dropTagList.begin();
	while (it != dropTagList.end())
	{
		if (*it == tag)
		{
			return true;
		}
		it++;
	}
	return false;
}

// 隣接しているか
bool PuzzleScene::isNextDrop(int tag1, int tag2)
{
	PositionIndex dropPos = getPositionByTag(tag1);
	int tags[6];
	tags[0] = tag1 + 100;
	tags[1] = tag1 - 100;
	tags[2] = tag1 + 1;
	tags[3] = tag1 - 1;
	
	if(dropPos.y % 2 != 0)
	{
		tags[4] = tag1 + 100 + 1;
		tags[5] = tag1 + 100 - 1;
	}
	else
	{
		tags[4] = tag1 - 100 + 1;
		tags[5] = tag1 - 100 - 1;
	}
	
	for (int i = 0; i < sizeof(tags) / sizeof(tags[0]); i++)
	{
		if(tags[i] == tag2)
		{
			return true;
		}
	}
	
	return false;
}

bool PuzzleScene::isSameColor(int tag1, int tag2)
{
	DropSprite* dropSprite = (DropSprite*)m_field->getChildByTag(tag1);
	kDrop dropType = dropSprite->getdropType();

	if (hasSameDropList(m_dropTags[dropType], tag2))
	{
		return true;
	}
	return false;
}





/*------------------------------------------------------------------
	ホーム画面に戻るボタン
	void myCallback(CCObject* pSender);
 ------------------------------------------------------------------*/
void PuzzleScene::myCallback(CCObject* pSender)
{
    CCScene* nextScene = HomeScene::scene();
    float duration = 0.5f;
	
    //フェードイン＆フェードアウト
    CCScene* scene = CCTransitionFade::create(duration, nextScene);
    if(scene){
        CCDirector::sharedDirector()->replaceScene(scene);
    }
}
