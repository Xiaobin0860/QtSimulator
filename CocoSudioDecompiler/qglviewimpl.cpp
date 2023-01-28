#include "qglviewimpl.h"
#include "cocos2d.h"

using namespace cocos2d;

QGLViewImpl::QGLViewImpl(QWidget *parent) : QWidget(parent), _isInited(false)
{

}

QGLViewImpl::~QGLViewImpl()
{

}

QGLViewImpl *QGLViewImpl::create(const std::string &viewName) {
    auto view = new QGLViewImpl;
    if (view) {
        if (!view->initWithRect(viewName, Rect(0, 0, 960, 640), 1.f)) {
            view->release();
            return nullptr;
        }
        view->autorelease();
    }
    return nullptr;
}

void QGLViewImpl::end() {

}

void QGLViewImpl::swapBuffers() {
    this->update();
}

void QGLViewImpl::setIMEKeyboardState(bool bOpen) {

}

void QGLViewImpl::setFrameSize(float width, float height) {

}

bool QGLViewImpl::initWithRect(const std::string &viewName, Rect rect, float frameZoomFactor) {
    setViewName(viewName);

    if (!initGlew()) return false;

    return true;
}

bool QGLViewImpl::initGlew() {
    if (_isInited) return true;

//    EGLNativeWindowType nativeWindow = (EGLNativeWindowType)this->winId();
//    EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;

    _isInited = true;
    return true;
}

//void QGLViewImpl::initializeGL() {
//    QOpenGLWidget::initializeGL();
//    _isInited = true;
//}
