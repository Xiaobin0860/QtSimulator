#ifndef COCOSWIDGET_H
#define COCOSWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVariantMap>

class CocosWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CocosWidget(QWidget *parent = 0);
    ~CocosWidget();

public slots:
    void onAcceptFile(QVariantMap map);

protected:
    virtual void resizeEvent(QResizeEvent *) override;
    virtual void moveEvent(QMoveEvent *) override;

private:
    QTimer _cocosTimer;

    void initCocos();

private slots:
    void cocosLoop();
};

#endif // COCOSWIDGET_H
