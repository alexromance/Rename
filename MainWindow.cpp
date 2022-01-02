//
// Created by alex on 2022/1/2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <ThreadReName.h>

#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initData();
    initViews();
    initEvents();
}

MainWindow::~MainWindow() {
    mRenameThread.quit();
    mRenameThread.wait();
    delete ui;
    qDebug()<<"~~~~~ MainWindow";
}

void MainWindow::initRenameThread() {
    auto threadRename = new ThreadReName;
    threadRename->moveToThread(&mRenameThread);

    connect(&mRenameThread, &QThread::started, threadRename, &ThreadReName::onThreadReNameInit);
    connect(this, &MainWindow::signalRenameStart, threadRename, &ThreadReName::onThreadReNameStart);
    connect(this, &MainWindow::signalGetFiles, threadRename, &ThreadReName::onThreadGetFiles);
    connect(threadRename, &ThreadReName::signalFileList, this, [=] (const QFileInfoList &list) {
        mSrcFileInfoList = list;
        QStringList fileList;
        QList<QTreeWidgetItem*> treeList;
        for (int i = 0; i < list.size(); ++i) {
            fileList.append(list.at(i).absoluteFilePath());
            auto data = new QTreeWidgetItem(QStringList() << list.at(i).absoluteFilePath());
            treeList.append(data);

        }
        qDebug()<<" filelist is "<<fileList;

        ui->treeWidgetSrcDir->addTopLevelItems(treeList);
    });
    connect(&mRenameThread, &QThread::finished, threadRename, &ThreadReName::deleteLater);

    mRenameThread.start();
}

void MainWindow::onPushButton() {
    auto sends = sender();
    if (ui->pushButtonOpenSrcDir == sends) {
        mStringSrcPath.clear();
        const QString &path = QFileDialog::getExistingDirectory(this, "打开原始文件夹", ".");
        if (!path.isNull()) {
            mStringSrcPath = path;
            emit signalGetFiles(mStringSrcPath);
        }
        ui->lineEditSrcDir->setText(mStringSrcPath);
    } else if (ui->pushButtonOpenDstDir == sends) {
        mStringDstPath.clear();
        const QString &path = QFileDialog::getExistingDirectory(this, "打开目标文件夹", ".");
        if (!path.isNull()) {
            mStringDstPath = path;
        }
        ui->lineEditDstDir->setText(mStringDstPath);
    } else if (ui->pushButtonTransform == sends) {
        if (!mSrcFileInfoList.isEmpty() && !mDstFileInfoList.isEmpty()){
            emit signalRenameStart(mSrcFileInfoList, mDstFileInfoList);
        }
    } else if (ui->pushButtonReview == sends) {
        ui->treeWidgetDstDir->clear();
        if (!mStringSrcPath.isNull() && !mSrcFileInfoList.isEmpty()) {
            qDebug()<<" path is "<<mStringSrcPath<<mStringDstPath;
            ui->lineEditDstDir->setText(mStringDstPath.isNull() ? mStringSrcPath : mStringDstPath);
            QList<QTreeWidgetItem*> treeList;
//            QRect rect(ui->treeWidgetSrcDir->visualItemRect(ui->treeWidgetSrcDir))
            for (int i = 0; i < mSrcFileInfoList.size(); ++i) {
//                fileList.append(list.at(i).absoluteFilePath());
                qDebug() << " tree item is " << mSrcFileInfoList.at(i).suffix() << mSrcFileInfoList.at(i).absoluteDir().path();
                QString stringFile = QString("%1/S%2E%3.%4").
                        arg(mStringDstPath.isNull() ? mSrcFileInfoList.at(i).absolutePath() : mStringDstPath).
                        arg(ui->lineEditSeason->text().toInt(), 2, 10, QLatin1Char('0')).
                        arg(i + 1, 2, 10, QLatin1Char('0')).
                        arg(mSrcFileInfoList.at(i).suffix());
                auto data = new QTreeWidgetItem(QStringList() << stringFile);
                mDstFileInfoList.append(QFileInfo(stringFile));
                treeList.append(data);
            }
            ui->treeWidgetDstDir->addTopLevelItems(treeList);
        }
    }
}

void MainWindow::initData() {

}

void MainWindow::initViews() {

}

void MainWindow::initEvents() {
    initButton();
    initLineEdit();
    connect(ui->treeWidgetSrcDir, &QTreeWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenuRequested);
    initRenameThread();
}

void MainWindow::initButton() {
    auto lists = findChildren<QPushButton*>();
    qDebug()<<" buttons "<<lists;
    for (int i = 0; i < lists.size(); ++i) {
        connect(lists.at(i), &QPushButton::clicked, this, &MainWindow::onPushButton);
    }
}

void MainWindow::initLineEdit() {

}

void MainWindow::onLienEdit() {

}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos) {
    QMenu menu(this);
    menu.addAction("删除选中节点", this, &MainWindow::onDeleteItem);
    menu.exec(QCursor::pos());
}

void MainWindow::onDeleteItem() {
    auto item = ui->treeWidgetSrcDir->itemAt(QCursor::pos().x(), QCursor::pos().y());
    qDebug()<<"delete item"<<item<<ui->treeWidgetSrcDir->currentItem();
    auto deleteItem = ui->treeWidgetSrcDir->currentItem();
    auto index = ui->treeWidgetSrcDir->currentColumn();
    qDebug()<<" index is "<<index;
    removeItem(deleteItem, index);
}

void MainWindow::removeItem(QTreeWidgetItem *item, int index) {
//    ui->treeWidgetSrcDir->removeItemWidget(deleteItem, 0);
    if (!item) {
        qDebug()<<" return from null item";
        return;
    }
    qDebug()<<"delete item name is "<<item->text(DEFAULT_COLUMN);
    mSrcFileInfoList.removeOne(QFileInfo(item->text(DEFAULT_COLUMN)));
    delete item;
    qDebug()<<" after remove info list is "<<mSrcFileInfoList;
}