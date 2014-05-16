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
    
	map = new CCTMXTiledMap();
	map->initWithTMXFile("0.tmx");
	this->addChild(map);

	for(int i = 0 ; i < kCount ;i++){
		walkAnimation[i] = this->createAnimationByDirection((HeroDirection)i);
		walkAnimation[i]->retain();
	}
	
	CCAnimation *animation = walkAnimation[kUp];
	heroSprite = new CCSprite();

	CCTexture2D *heroTexture = CCTextureCache::sharedTextureCache()->addImage("hero.png");
	
	CCSpriteFrame *initialFrame = new CCSpriteFrame();		
	initialFrame->initWithTexture(heroTexture,cocos2d::CCRect(32*0,32*0,32,32));

	heroSprite->initWithSpriteFrame(initialFrame);
	heroSprite->setAnchorPoint(ccp(0,0));
	heroSprite->setPosition(this->positionForTileCoord(ccp(1,11)));
	this->addChild(heroSprite);

	CCNode *downLabel = CCLabelTTF::create("Down","Arial",30);
	CCNode *leftLabel = CCLabelTTF::create("Left","Arial",30);
	CCNode *rightLabel = CCLabelTTF::create("Right","Arial",30);
	CCNode *upLabel = CCLabelTTF::create("Up","Arial",30);
	
	CCMenuItemLabel *downMenuItem = CCMenuItemLabel::create(downLabel,this,menu_selector(HelloWorld::menuMoveCallBack));
	CCMenuItemLabel *leftMenuItem = CCMenuItemLabel::create(leftLabel,this,menu_selector(HelloWorld::menuMoveCallBack));
	CCMenuItemLabel *rightMenuItem = CCMenuItemLabel::create(rightLabel,this,menu_selector(HelloWorld::menuMoveCallBack));
	CCMenuItemLabel *upMenuItem = CCMenuItemLabel::create(upLabel,this,menu_selector(HelloWorld::menuMoveCallBack));
	downMenuItem->setTag(kDown);
	leftMenuItem->setTag(kLeft);
	rightMenuItem->setTag(kRight);
	upMenuItem->setTag(kUp);

	CCMenu *menu = CCMenu::create(downMenuItem,leftMenuItem,rightMenuItem,upMenuItem,NULL);
	menu->setVisible(true);
	menu->alignItemsHorizontallyWithPadding(50);
	this->addChild(menu);

	schedule(schedule_selector(HelloWorld::updateScene));

	
	CCArray *tileArray = map->getChildren();

	for(int i = 0 ; i < tileArray->count() ; i++)
	{
		CCSpriteBatchNode *child = (CCSpriteBatchNode *)tileArray->objectAtIndex(i);
		if(child == NULL) break;

		child->getTexture()->setAntiAliasTexParameters();
	}

	CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL(2);

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


void HelloWorld::menuMoveCallBack(CCObject *pSender)
{
	CCNode *node = (CCNode *)pSender;

	int direction = node->getTag();

	const int unitLen = 32;
	int dirV[4][2] = {{0,-1},{-1,0},{1,0},{0,1}};
	CCPoint moveV;
	moveV.x = unitLen*dirV[direction][0];
	moveV.y = unitLen*dirV[direction][1];


	CCAnimate *walkAnimate = CCAnimate::create(this->walkAnimation[direction]);
	CCAction *moveBy = CCMoveBy::create(0.28f,moveV);
	CCCallFuncND *callFuncND = CCCallFuncND::create(this,callfuncND_selector(HelloWorld::onWalkDone),(void*)direction);
	CCSpawn *actionsParal = CCSpawn::create(walkAnimate ,moveBy,NULL); 
	CCAction *action = CCSequence::create(actionsParal,callFuncND,NULL);
	this->heroSprite->runAction(action);
}

void HelloWorld::onWalkDone(cocos2d::CCNode *pTarget , void *data)
{
	
}

HelloWorld::~HelloWorld()
{
	for(int i = 0 ; i < kCount ; i++)
	{
		CC_SAFE_RELEASE(walkAnimation[i]);
	}

	this->heroSprite->release();
	this->map->release();
	this->unscheduleAllSelectors();
}

CCPoint HelloWorld::positionForTileCoord(CCPoint tileCoord)
{
	float x = tileCoord.x * map->getTileSize().width;
	float y = (map->getMapSize().height - tileCoord.y - 1) * map->getTileSize().height;
	return ccp(x,y);
}

void HelloWorld::setSceneScrollPosition(cocos2d::CCPoint position)
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	const CCSize &mapSize = map->getMapSize();
	const CCSize &tileSize = map->getTileSize();
	CCSize mapSizeInPixel = CCSize(mapSize.width * tileSize.width ,mapSize.height * tileSize.height);

	float x = MAX(position.x , screenSize.width / 2.0f);
	float y = MAX(position.y , screenSize.height / 2.0f);

	if(mapSizeInPixel.width > screenSize.width)
	{
		x = MIN(x , mapSizeInPixel.width - (screenSize.width / 2.0f));
	}

	if(mapSizeInPixel.height > screenSize.height)
	{
		y = MIN(y , mapSizeInPixel.height - (screenSize.height / 2.0f));
	}

	CCPoint heroPosition = ccp(x,y);
	CCPoint screenCenter = ccp(screenSize.width / 2.0f , screenSize.height / 2.0f);
	CCPoint scrollPosition = ccpSub(screenCenter , heroPosition);

	this->setPosition(scrollPosition);

	CCLog("%f,%f",scrollPosition.x,scrollPosition.y);
}

void HelloWorld::updateScene(float dt)
{
	this->setSceneScrollPosition(this->heroSprite->getPosition());
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
