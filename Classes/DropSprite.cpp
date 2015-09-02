#include "DropSprite.h"

DropSprite::DropSprite()
{
	initNextPos();
}


DropSprite::~DropSprite()
{
}


DropSprite* DropSprite::createWithDropType(kDrop dropType)
{
    DropSprite *pRet = new DropSprite();
    if (pRet && pRet->initWithDropType(dropType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}


bool DropSprite::initWithDropType(kDrop dropType)
{
    if(!CCSprite::initWithFile(getDropImageFileName(dropType)))
    {
        return false;
    }

    m_dropType = dropType;

    return true;
}


// ドロップのファイル名を返却する関数
const char* DropSprite::getDropImageFileName(kDrop dropType)
{
    switch (dropType) {
        case kDropRed:
            return "drop/red.png";
            
        case kDropBlue:
            return "drop/blue.png";
            
        case kDropOrange:
            return "drop/orange.png";
            
        case kDropGreen:
            return "drop/green.png";
            
        case kDropPurple:
            return "drop/purple.png";

        case kDropGray:
            return "drop/gray.png";

        default:
            CCAssert(false, "invalid dropType");
            return "";
    }
}


// 移動先の初期化
void DropSprite::initNextPos()
{
	m_nextPosX = -1;
	m_nextPosY = -1;
}

// 移動先インデックスをセット
void DropSprite::setNextPos(int nextPosX, int nextPosY)
{
	m_nextPosX = nextPosX;
	m_nextPosY = nextPosY;
}