#include "rightwidget.h"

#include <QResizeEvent>

#include "cocos2d.h"
#include "mainwindow.h"
#include "cocoswidget.h"

#define LOG_TAG "RightWidget"
#include "logger.h"


RightWidget::RightWidget(QWidget *parent) : QWidget(parent)
{
}

RightWidget::~RightWidget()
{
}

void RightWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // parent
    float pw = event->size().width();
    float ph = event->size().height();

    // child
    auto director = cocos2d::Director::getInstance();
    cocos2d::Size childSize = director->getWinSize();   // design
    auto cw = childSize.width;
    auto ch = childSize.height;

    auto f1 = cw / ch;
    auto f2 = pw / ph;
    // new child size
    auto nw = cw;
    auto nh = ch;
    if (f1 > f2) {
        nw = pw;
        nh = nw / f1;
    }
    else {
        nh = ph;
        nw = nh * f1;
    }

    auto cocos = MAIN_WND->getCocosWidget();
    cocos->setGeometry((pw-nw)/2, (ph-nh)/2, nw, nh);
    LOGT("cocos rect: " << (pw-nw)/2 << "," << (ph-nh)/2 << "|" << nw << "," << nh);
}
