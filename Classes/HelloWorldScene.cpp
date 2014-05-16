#include "HelloWorldScene.h"
#include <iostream>

//#include "tilemap_parallax_nodes\CCTMXTiledMap.h"
//#include "cocoa\CCArray.h"
//#include "sprite_nodes\CCAnimation.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	CCTMXTiledMap *map = new CCTMXTiledMap();
	map->initWithTMXFile("0.tmx");
	this->addChild(map);

	CCAnimation* walkAnimation[kCount] ;

	for(int i = 0 ; i < kCount ;i++){
		walkAnimation[i] = this->createAnimationByDirection((HeroDirection)i);
	}
	
	CCAnimation *animation = walkAnimation[kUp];

	CCSprite *heroSprite = new CCSprite();

	CCTexture2D *heroTexture = CCTextureCache::sharedTextureCache()->addImage("hero.png");
	CCSpriteFrame *initialFrame = new CCSpriteFrame();		
	initialFrame->initWithTexture(heroTexture,cocos2d::CCRect(32*0,32*0,32,32));

	heroSprite->initWithSpriteFrame(initialFrame);
	heroSprite->setPosition(ccp(100,100));
	this->addChild(heroSprite);
	
	CCAnimate *animate = new CCAnimate();
	animate->initWithAnimation(animation);
	CCRepeatForever *action = new CCRepeatForever();
	action->initWithAction(animate);
	heroSprite->runAction(action);


	animate->release();
	action->release();
	heroSprite->release();
	map->release();
	initialFrame->release();

    return true;
}

CCAnimation* HelloWorld::createAnimationByDirection(HeroDirection direction){

	CCTexture2D *heroTexture = CCTextureCache::sharedTextureCache()->addImage("hero.png");

	CCSpriteFrame *frame0 = new CCSpriteFrame() 
				, *frame1 = new CCSpriteFrame() 
				, *frame2 = new CCSpriteFrame();

	frame0->initWithTexture(heroTexture,cocos2d::CCRect(32*0,32*direction,32,32));
	frame1->initWithTexture(heroTexture,cocos2d::CCRect(32*1,32*direction,32,32));
	frame2->initWithTexture(heroTexture,cocos2d::CCRect(32*2,32*direction,32,32));
	
	CCArray *animFrames = new CCArray();
	animFrames->addObject(frame0);
	animFrames->addObject(frame1);
	animFrames->addObject(frame2);
	CCAnimation *animation = new CCAnimation();
	animation->initWithSpriteFrames(animFrames,0.2f);
	
	frame0->release();
	frame1->release();
	frame2->release();
	animFrames->release();

	animation->autorelease();

	return animation;
	
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
