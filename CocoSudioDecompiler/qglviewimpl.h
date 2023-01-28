#ifndef QGLVIEWIMPL_H
#define QGLVIEWIMPL_H
#include "platform/CCGLView.h"
#include "cocos2d.h"

#include <QWidget>

using cocos2d::Rect;

class QGLViewImpl : public QWidget, public cocos2d::GLView
{
    Q_OBJECT
public:
    explicit QGLViewImpl(QWidget *parent = 0);
    ~QGLViewImpl();

    static QGLViewImpl* create(const std::string& viewName);

    virtual void end() override;
    virtual bool isOpenGLReady() override { return _isInited; }
    virtual void swapBuffers() override;
    virtual void setIMEKeyboardState(bool bOpen) override;
    virtual void setFrameSize(float width, float height) override;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    HWND getWin32Window() { return nullptr; }
#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) */

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    id getCocoaWindow() { return nullptr; }
#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

protected:
    bool initWithRect(const std::string& viewName, Rect rect, float frameZoomFactor);
    bool initGlew();


//    virtual void initializeGL() override;

signals:

public slots:

private:
    bool _isInited;
};

#endif // QGLVIEWIMPL_H
