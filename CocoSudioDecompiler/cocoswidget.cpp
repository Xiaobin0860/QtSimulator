#include "cocoswidget.h"

#include <QApplication>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QFileInfo>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QtXmlPatterns/QXmlQuery>

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocosbuilder/CocosBuilder.h"
#include "glfw3.h"
#include "AppDelegate.h"
#include "scenemanager.h"

#define LOG_TAG "CocosWidget"
#include "logger.h"

USING_NS_CC;
using namespace cocostudio;

static GLViewImpl* s_glview = nullptr;

static AppDelegate& getApp()
{
    static AppDelegate app;
    return app;
}

static const float DEFAULT_FPS = 1/60.f;

CocosWidget::CocosWidget(QWidget *parent)
: QWidget(parent)
{
    initCocos();
}

CocosWidget::~CocosWidget()
{
    LOGT("~CocosWidget()");
}

void CocosWidget::initCocos() {
    auto w = 1136.f;
    auto h = 640.f;
    QString json;

    auto args = qApp->arguments();
    for (auto i=1; i<args.size(); ++i) {
        auto cmd = args[i];
        if (cmd == "-XMLPATH") {
            if (i+1 < args.size()) {
                auto xmlPath = args[i+1].trimmed();
                LOGT("xml: " << xmlPath);
                QFile f(xmlPath);
                if (f.open(QIODevice::ReadOnly)) {
                    QXmlQuery query;
                    query.setFocus(&f);

                    query.setQuery("SceneProject/Resources");
                    QString resources;
                    query.evaluateTo(&resources);
                    resources.replace(QRegExp("<.[^>]*>"), "").replace(QRegExp("<.[^>]*/>"), "");
                    resources = resources.trimmed();
                    LOGT("Resources: " << resources);
                    FileUtils::getInstance()->addSearchPath(resources.toStdString());

                    query.setQuery("SceneProject/CanvasSize/Width");
                    QString width;
                    query.evaluateTo(&width);
                    width.replace(QRegExp("<.[^>]*>"), "").replace(QRegExp("<.[^>]*/>"), "");
                    LOGT("Width: " << width);
                    width = width.trimmed();
                    w = width.toFloat();
                    query.setQuery("SceneProject/CanvasSize/Height");
                    QString height;
                    query.evaluateTo(&height);
                    height.replace(QRegExp("<.[^>]*>"), "").replace(QRegExp("<.[^>]*/>"), "");
                    height = height.trimmed();
                    LOGT("Height: " << height);
                    h = height.toFloat();

                    query.setQuery("SceneProject/ResRelativePath");
                    QString relative;
                    query.evaluateTo(&relative);
                    relative.replace(QRegExp("<.[^>]*>"), "").replace(QRegExp("<.[^>]*/>"), "");
                    relative = relative.trimmed();
                    LOGT("ResRelativePath: " << relative);

                    query.setQuery("SceneProject/ProjectDir");
                    QString projDir;
                    query.evaluateTo(&projDir);
                    projDir.replace(QRegExp("<.[^>]*>"), "").replace(QRegExp("<.[^>]*/>"), "");
                    projDir = projDir.trimmed();
                    LOGT("ProjectDir: " << projDir);

                    query.setQuery("SceneProject/JsonFolder");
                    QString jsonDir;
                    query.evaluateTo(&jsonDir);
                    jsonDir.replace(QRegExp("<.[^>]*>"), "").replace(QRegExp("<.[^>]*/>"), "");
                    jsonDir = jsonDir.trimmed();
                    LOGT("JsonFolder: " << jsonDir);
                    query.setQuery("SceneProject/JsonFileName");
                    QString jsonName;
                    query.evaluateTo(&jsonName);
                    jsonName.replace(QRegExp("<.[^>]*>"), "").replace(QRegExp("<.[^>]*/>"), "");
                    jsonName = jsonName.trimmed();
                    LOGT("JsonFileName: " << jsonName);

                    json = jsonDir + "/" + jsonName;
#ifdef WIN32
                    json.replace("/", "\\");
                    relative.replace("/", "\\");
                    relative.replace("\\\\", "\\");
#endif
                    LOGT("add search: " << (projDir + relative));
                    FileUtils::getInstance()->addSearchPath((projDir + relative).toStdString());

                    if (xmlPath.contains("ccsprojs")) {
                        auto search = xmlPath.split("ccsprojs")[0] + "assets";
                        LOGT("add search: " << search);
                        FileUtils::getInstance()->addSearchPath(search.toStdString());
                    }


                    f.close();
                }
            }
        }
        else if (cmd == "-DLLPATH") {
            if (i+1 < args.size()) {
                auto dllPath = args[i+1];
                auto dlls = dllPath.split('|', QString::SkipEmptyParts);
                foreach (auto dll, dlls) {
                    LOGT("dll: " << dll);
                    if (QFile(dll).exists()) {
                        LOGT("LoadLibrary: " << dll);
                        ::LoadLibraryA(dll.toStdString().c_str());
                    }
                }
            }
        }
    }

    // create gl window
    auto director = Director::getInstance();
    s_glview = (GLViewImpl*)director->getOpenGLView();
    if(!s_glview) {
        s_glview = GLViewImpl::create("");
        director->setOpenGLView(s_glview);
        s_glview->setFrameSize(w, h);
    }
    s_glview->setDesignResolutionSize(w, h, ResolutionPolicy::EXACT_FIT);

    // run default secene
    getApp().applicationDidFinishLaunching();

    if (json.length()) {
        LOGT("create node from " << json);
        auto node = SceneReader::getInstance()->createNodeWithSceneFile(json.toStdString());
        if (node) {
            auto scene = Scene::create();
            scene->addChild(node);
            Director::getInstance()->replaceScene(scene);
        }
    }

    // set gl wnd as child
#ifdef WIN32
    HWND glWnd = s_glview->getWin32Window();
    HWND widgetWnd = (HWND)this->winId();
    DWORD style = ::GetWindowLong(glWnd, GWL_STYLE);
    style &= ~(WS_POPUP | WS_CAPTION | WS_BORDER);
    style |= WS_CHILD;
    SetWindowLong(glWnd, GWL_STYLE, style);
    ::SetParent(glWnd, widgetWnd);
#else
    Q_UNUSED(glview);
#endif

    // start cocos loop
    connect(&_cocosTimer, SIGNAL(timeout()),
            this, SLOT(cocosLoop()));
    _cocosTimer.start(DEFAULT_FPS * 1000);
}

void CocosWidget::cocosLoop()
{
    //    LOGT("cocosLoop ...");
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) return;
    glview->retain();

    if(!glview->windowShouldClose()) {
        director->mainLoop();
        glview->pollEvents();
    }
    else {
        LOGT("cocosLoop: windowShouldClose");
        _cocosTimer.stop();
        // Director should still do a cleanup if the window was closed manually.
        if (glview->isOpenGLReady())
        {
            director->end();
            director->mainLoop();
        }
        glview->release();
        qApp->quit();
        return;
    }
    glview->release();
}

void CocosWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QSize size = event->size();
    LOGT("resizeEvent: {" << size.width() << ", " << size.height() << "}");
    QRect rect = this->geometry();
    LOGT("resizeEvent cocos rect: {(" << rect.x() << ", " << rect.y() << "), (" << rect.width() << ", " << rect.height() << ")}");

#ifdef WIN32
    if (!s_glview) return;
    HWND glwnd = s_glview->getWin32Window();
    SetWindowPos(glwnd, HWND_TOP,
                 0, 0, size.width(), size.height(),
                 SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
#else
    Q_UNUSED(glview);
#endif
}

void CocosWidget::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);
    QPoint pt = event->pos();
    LOGT("moveEvent: {" << pt.x() << ", " << pt.y() << "}");
    QRect rect = this->geometry();
    LOGT("moveEvent cocos rect: {(" << rect.x() << ", " << rect.y() << "), (" << rect.width() << ", " << rect.height() << ")}");

#ifdef WIN32
    if (!s_glview) return;
    HWND glwnd = s_glview->getWin32Window();
    SetWindowPos(glwnd, NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
#else
    Q_UNUSED(glview);
#endif
}

void CocosWidget::onAcceptFile(QVariantMap map)
{
    auto type = map["type"].toString();
    auto file = map["path"].toString();
    LOGT("onAcceptFile: type=" << type << ", file=" << file);
//    auto director = Director::getInstance();
    Node *node = nullptr;
    if ("scene" == type)
    {
        LOGT("create node from " << file);
        node = SceneReader::getInstance()->createNodeWithSceneFile(file.toStdString());
        LOGT("create node ok.");
    }
    else if ("ui" == type)
    {
        LOGT("create widget from " << file);
        node = GUIReader::getInstance()->widgetFromJsonFile(file.toStdString().c_str());
//        auto size = node->getContentSize();
//        auto winSize = director->getWinSize();
//        node->setPosition((winSize.width-size.width)/2, (winSize.height-size.height)/2);
        LOGT("create widget ok.");
    }
    else if ("animation" == type)
    {
        auto animations = map["animations"].toJsonArray();
        if (animations.size()) {
            ArmatureDataManager::getInstance()->addArmatureFileInfo(file.toStdString());
            auto obj = animations[0].toObject();
            auto actions = obj["mov_data"].toArray();
            node = Armature::create(obj["name"].toString().toStdString());
            if (actions.size()) {
                ((Armature*)node)->getAnimation()->playWithIndex(0, -1, 1);
            }
//            auto winSize = director->getWinSize();
//            node->setPosition(winSize.width/2, winSize.height/2);
        }
    }
    else if ("particle" == type)
    {
        node = ParticleSystemQuad::create(file.toStdString());
//        auto winSize = director->getWinSize();
//        node->setPosition(winSize.width/2, winSize.height/2);
    }
    else if ("frame" == type)
    {

    }
    if (node) {
//        Scene *scene = Scene::create();
//        scene->addChild(node);
//        director->replaceScene(scene);
        SM.runWithNode(node);
    }
}
