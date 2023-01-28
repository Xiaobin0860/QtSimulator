#ifndef RIGHTWIDGET_H
#define RIGHTWIDGET_H

#include <QWidget>

class RightWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightWidget(QWidget *parent = 0);
    ~RightWidget();

protected:
    virtual void resizeEvent(QResizeEvent *) override;

signals:

public slots:
};

#endif // RIGHTWIDGET_H
