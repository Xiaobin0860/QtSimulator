#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariantMap>
#include <QModelIndex>

namespace Ui {
    class MainWindow;
}

#define MAIN_WND (MainWindow::getMainWindow())

class CocosWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow* getMainWindow() {
        extern MainWindow* g_mainWindow;
        return g_mainWindow;
    }

    CocosWidget *getCocosWidget();

signals:
    void acceptFile(QVariantMap map);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *) override;
    virtual void dropEvent(QDropEvent *) override;

private slots:
    void onTreeItemClicked(QModelIndex);

private:
    Ui::MainWindow *ui;
    QVariantMap _fileInfo;
    QVariantMap checkFileFormat(const QString& file);
};

#endif // MAINWINDOW_H
