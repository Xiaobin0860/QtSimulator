#include "scenemanager.h"
#include "cocostudio/CocoStudio.h"

#include <QString>
#include <QStringList>

#define LOG_TAG "SceneManager"
#include "logger.h"

using namespace cocos2d;
using namespace cocos2d::ui;

SceneManager::SceneManager()
: _runningScene(nullptr)
, _content(nullptr)
, _logView(nullptr)
, _lblInfo(nullptr)
, _txtInfo(nullptr)
{
}

SceneManager::~SceneManager()
{
    Deinit();
}

bool SceneManager::Init()
{
    _runningScene = Scene::create();
    if (!_runningScene) return false;
    _runningScene->retain();

    _content = LayerColor::create(Color4B(0, 0, 0, 255));
    if (!_content) return false;
    _runningScene->addChild(_content);

//    _logView = ListView::create();
//    _logView->setBackGroundColorType(Layout::BackGroundColorType::NONE);
//    _logView->setDirection(ScrollView::Direction::VERTICAL);
//    _logView->setItemModel(Text::create("", "", 15));
//    _logView->setSize(_content->getContentSize());
//    _logView->setSize(Size(400, 300));
//    _logView->setPosition(Vec2::ZERO);
//    _runningScene->addChild(_logView);
//    auto test = LayerColor::create(Color4B(255, 0, 0, 128));
//    test->addChild(_logView);
//    _runningScene->addChild(test);

    auto size = _runningScene->getContentSize();
    auto infoBg = LayerColor::create(Color4B(0, 0, 0, 200));
    infoBg->setPosition(Vec2(0, size.height-40));
    infoBg->setTag(123456);
    _runningScene->addChild(infoBg);

    _lblInfo = Label::createWithSystemFont("", "", 40);
    _lblInfo->setTextColor(Color4B(255, 0, 0, 255));
    _lblInfo->setAnchorPoint(Vec2(0, 1));
    _lblInfo->setPosition(Vec2(0, size.height));
    _runningScene->addChild(_lblInfo);

    Director::getInstance()->runWithScene(_runningScene);

    return true;
}

void SceneManager::Deinit()
{
    CC_SAFE_RELEASE_NULL(_runningScene);
}

void SceneManager::runWithNode(Node *node)
{
    auto cs = node->getContentSize();
    auto ps = _content->getContentSize();
    auto anchor = node->getAnchorPoint();
    auto pos = Vec2(ps.width/2, ps.height/2) + Vec2((anchor.x-0.5)*cs.width, (anchor.y-0.5)*cs.height);
    node->setPosition(pos);
    _content->removeAllChildrenWithCleanup(true);
    _content->addChild(node);

//    _runningScene->runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([this]{
        cocostudio::ArmatureDataManager::destroyInstance();
        cocostudio::SpriteFrameCacheHelper::getInstance();
        cocostudio::DataReaderHelper::getInstance();
        cocostudio::ArmatureDataManager::getInstance();

        auto dr = Director::getInstance();
        dr->purgeCachedData();
        QString qs(dr->getTextureCache()->getCachedTextureInfo().c_str());
        QStringList sl = qs.split("TextureCache dumpDebugInfo:", QString::SkipEmptyParts);
        if (sl.size() == 2)
        {
            _lblInfo->setString(sl.at(1).toStdString());
        }
        else
        {
            _lblInfo->setString("");
        }
        auto infoSize = _lblInfo->getContentSize();
        _runningScene->getChildByTag(123456)->setContentSize(infoSize);
//    }), nullptr));
}

SceneManager& getSceneMgr()
{
    static SceneManager mgr;
    return mgr;
}
