#include "HelloWorldScene.h"
#include "AppMacros.h"
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
    if ( !CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    //CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    //
    //// position the label on the center of the screen
    //pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
    //                        origin.y + visibleSize.height - pLabel->getContentSize().height));

    //// add the label as a child to this layer
    //this->addChild(pLabel, 1);

    //// add "HelloWorld" splash screen"
    //CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    //// position the sprite on the center of the screen
    //pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //// add the sprite as a child to this layer
    //this->addChild(pSprite, 0);
    
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *player = CCSprite::create("player.png");
	player->setPosition(ccp(player->getContentSize().width/2, winSize.height/2));
	this->addChild(player);

	// call game Logic
	this->schedule( schedule_selector(HelloWorld::gameLogic), 1.0f);

	// touchEnable
	this->setTouchEnabled(true);

    return true;
}

void HelloWorld::addTarget()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *target = CCSprite::create("Target.png");
	CCSize size = target->getContentSize();
	
	int minY = size.height/2;
	int maxY = winSize.height - size.height/2;
	int rangeY = maxY - minY;
	int actualY = ( rand() % rangeY ) + minY;

	// Create the target slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated
	target->setPosition(
		ccp(winSize.width + (target->getContentSize().width/2), 
		actualY));
	this->addChild(target);

	// Determine speed of the target
	int minDuration = (int)2.0;
	int maxDuration = (int)4.0;
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = ( rand() % rangeDuration ) + minDuration;

	// Create the actions
	CCFiniteTimeAction* actionMove = CCMoveTo::create((float)actualDuration, ccp(0 - target->getContentSize().width/2, actualY) );
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this,callfuncN_selector(HelloWorld::spriteMoveFinished));

	target->runAction( CCSequence::create(actionMove, actionMoveDone, nullptr) );
}

void HelloWorld::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	this->removeChild(sprite, true);
}

void HelloWorld::gameLogic(float dt)
{
	this->addTarget();
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    // Choose one of the touches to work with
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    CCPoint location = touch->getLocation();
	
    // Set up initial location of projectile
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *projectile = CCSprite::create("Projectile.png");
    projectile->setPosition( ccp(20, winSize.height/2) );

    // Determinie offset of location to projectile
    int offX = location.x - projectile->getPosition().x;
    int offY = location.y - projectile->getPosition().y;

    // Bail out if we are shooting down or backwards
    if (offX <= 0) return;

    // Ok to add now - we've double checked position
    this->addChild(projectile);

    // Determine where we wish to shoot the projectile to
    int realX = winSize.width + (projectile->getContentSize().width/2);
    float ratio = (float)offY / (float)offX;
    int realY = (realX * ratio) + projectile->getPosition().y;
    CCPoint realDest = ccp(realX, realY);

    // Determine the length of how far we're shooting
    int offRealX = realX - projectile->getPosition().x;
    int offRealY = realY - projectile->getPosition().y;
    float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
    float velocity = 480/1; // 480pixels/1sec
    float realMoveDuration = length/velocity;

    // Move projectile to actual endpoint
	CCFiniteTimeAction * moved = CCMoveTo::create(realMoveDuration, realDest);
	CCFiniteTimeAction * finished = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));

    projectile->runAction( CCSequence::create(moved, finished, nullptr) );
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
