#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

typedef enum {
	kDown = 0,
	kLeft = 1,
	kRight = 2,
	kUp = 3,
	kCount = 4
} HeroDirection;

class HelloWorld : public cocos2d::CCLayer
{
public:
	~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	cocos2d::CCAnimation *createAnimationByDirection(HeroDirection direction);
   
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

private:
	cocos2d::CCAnimation* walkAnimation[kCount];
	cocos2d::CCSprite *heroSprite;
	cocos2d::CCTMXTiledMap *map;

	void menuMoveCallBack(cocos2d::CCObject *pSender);
	void onWalkDone(cocos2d::CCNode *pTarget , void *data);
	cocos2d::CCPoint positionForTileCoord(cocos2d::CCPoint tileCoord);
	void setSceneScrollPosition(cocos2d::CCPoint postion);

	void updateScene(float dt);
};


#endif // __HELLOWORLD_SCENE_H__
