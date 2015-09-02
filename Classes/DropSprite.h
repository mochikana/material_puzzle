#ifndef __DROPSPRITE_H__
#define __DROPSPRITE_H__

#include "cocos2d.h"
#include "Config.h"


class DropSprite : public cocos2d::CCSprite
{
protected:
    const char* getDropImageFileName(kDrop dropType);
    
public:
    CC_SYNTHESIZE_READONLY(kDrop, m_dropType, dropType);
    CC_SYNTHESIZE_READONLY(int, m_nextPosX, NextPosX);
    CC_SYNTHESIZE_READONLY(int, m_nextPosY, NextPosY);

	void initNextPos();
	void setNextPos(int nextPosX, int nextPosY);
	
    DropSprite();
    virtual ~DropSprite();
    virtual bool initWithDropType(kDrop dropType);
    static DropSprite* createWithDropType(kDrop dropType);
};


#endif //__DROPSPRITE_H__