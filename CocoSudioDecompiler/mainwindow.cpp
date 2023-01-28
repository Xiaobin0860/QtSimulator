#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QtCore>
#include <QFileSystemModel>

#include "cocoswidget.h"
#include "rightwidget.h"

#include "cocos2d.h"

#define LOG_TAG "MainWindow"
#include "logger.h"


using namespace cocos2d;

#define DIC (cocostudio::DictionaryHelper::getInstance())

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<int> widgetSizes;
    widgetSizes << 150 << 960;
    ui->splitter->setSizes(widgetSizes);

    setAcceptDrops(true);

    connect(this, SIGNAL(acceptFile(QVariantMap)), ui->cocos, SLOT(onAcceptFile(QVariantMap)));
    connect(ui->treeDirs, SIGNAL(clicked(QModelIndex)), this, SLOT(onTreeItemClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

CocosWidget *MainWindow::getCocosWidget()
{
    return ui->cocos;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    LOGT("DragEnter");
    if (!event->mimeData()->hasUrls()) return;
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;

    _fileInfo.clear();
    foreach(const QUrl url, urls) {
        auto file = url.toLocalFile();
        LOGT(file);
        auto upper = file.toUpper();
        if (QFileInfo(file).isDir()) {
            _fileInfo["accetped"] = true;
            _fileInfo["type"] = "path";
            _fileInfo["path"] = file;
        }
        else if (upper.endsWith(".JSON")
                || upper.endsWith(".EXPORTJSON")
                || upper.endsWith(".PLIST"))
        {
            _fileInfo = checkFileFormat(file);
        }
        if (_fileInfo["accetped"].toBool())
        {
            event->setDropAction(Qt::CopyAction);
            event->accept();
            return;
        }
    }
    event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    LOGT("Drop");
    if (!event->mimeData()->hasUrls()) return;
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;

    auto file = urls[0].toLocalFile();
    LOGT(file);
    if (_fileInfo.contains("path") && _fileInfo["path"].toString() == file)
    {
        LOGT("accetped " << _fileInfo["type"].toString() << ": " << _fileInfo["path"].toString());
        if ("path" == _fileInfo["type"]) {
            FileUtils::getInstance()->addSearchPath(file.toStdString());
            auto model = new QFileSystemModel(ui->treeDirs);
            model->setNameFilterDisables(false);
            model->setNameFilters(QStringList() << "*.json" << "*.ExportJson" << "*.plist");
            auto root = _fileInfo["path"].toString();
            LOGT("root: " << root);
            model->setRootPath(root);
            ui->treeDirs->setModel(model);
            ui->treeDirs->setRootIndex(model->setRootPath(root));
        }
        else {
            emit acceptFile(_fileInfo);
        }
        event->accept();
        return;
    }
    event->ignore();
}

QVariantMap MainWindow::checkFileFormat(const QString& file)
{
    QVariantMap map;
    map["accetped"] = false;

    auto upper = file.toUpper();
    if (upper.endsWith(".JSON") || upper.endsWith(".EXPORTJSON"))
    {
        QFile f(file);
        if (!f.open(QIODevice::ReadOnly))
        {
            LOGE("open " << file << " failed!");
            return map;
        }

        auto data = f.readAll();
        auto doc = QJsonDocument::fromJson(data);
        if (doc.isObject())
        {
            auto obj = doc.object();
            if (obj.contains("components"))
            {
                map["accetped"] = true;
                map["type"] = "scene";
                map["path"] = file;
            }
            else if (obj.contains("widgetTree"))
            {
                map["accetped"] = true;
                map["type"] = "ui";
                map["path"] = file;
            }
            else if (obj.contains("animation_data"))
            {
                map["accetped"] = true;
                map["type"] = "animation";
                map["path"] = file;
                map["animations"] = obj["animation_data"].toArray();
            }
        }
        else
        {
            LOGW("file format error! data=" << data);
        }

        f.close();
    }
    else if (upper.endsWith(".PLIST"))
    {
        ValueMap plist = FileUtils::getInstance()->getValueMapFromFile(file.toStdString());
        if (plist.find("textureFileName") != plist.end())
        {
            map["accetped"] = true;
            map["type"] = "particle";
            map["path"] = file;
        }
        else if (plist.find("frames") != plist.end())
        {
            map["accetped"] = true;
            map["type"] = "frame";
            map["path"] = file;
        }
    }
    return map;
}

void MainWindow::onTreeItemClicked(QModelIndex idx)
{
    auto data = ((QFileSystemModel*)ui->treeDirs->model())->filePath(idx);
    LOGT("onTreeItemClicked: " << data);
    _fileInfo = checkFileFormat(data);
    if (_fileInfo["accetped"].toBool()) {
        emit acceptFile(_fileInfo);
    }
}
