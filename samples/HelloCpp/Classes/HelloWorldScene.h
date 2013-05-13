#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	HelloWorld();
	virtual 

    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	// ȭ�鿡 Target �� �߰��Ѵ�.
	void addTarget();
	// Target�� ȭ�� ������ �̵��ϸ�, �ش� object ó�� ��
	void spriteMoveFinished(CCNode *sender);
	// �ֱ������� ȣ��Ǵ� ����
	void gameLogic(float dt);

	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
};

#endif // __HELLOWORLD_SCENE_H__
