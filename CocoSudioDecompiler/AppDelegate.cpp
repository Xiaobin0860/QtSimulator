#include "AppDelegate.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <string>

#include "platform/desktop/CCGLViewImpl-desktop.h"
#include "scenemanager.h"

#define LOG_TAG "AppDelegate"
#include "logger.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    FileUtils::getInstance()->addSearchPath("");

    // initialize director
    auto director = Director::getInstance();
	
    // turn on display FPS
    director->setDisplayStats(true);

    // run
    SM.Init();

    SM.runWithNode(Node::create());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
