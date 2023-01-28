#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define SM (getSceneMgr())

class SceneManager
{
public:
    bool Init();
    void Deinit();
    
    void runWithNode(cocos2d::Node *node);

    SceneManager();
    ~SceneManager();

private:
    cocos2d::Scene *_runningScene;
    cocos2d::Layer *_content;
    cocos2d::ui::ListView *_logView;
    cocos2d::Label *_lblInfo;
    cocos2d::ui::Text *_txtInfo;
};

SceneManager& getSceneMgr();

#endif // SCENEMANAGER_H
