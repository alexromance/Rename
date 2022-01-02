//
// Created by alex on 2022/1/2.
//

#include "ThreadReName.h"

#include <QDebug>
#include <QDir>
#include <QProcess>

ThreadReName::ThreadReName(QObject *parent) : QObject(parent) {

}

ThreadReName::~ThreadReName() {
    qDebug()<<"~~~~~ ThreadReName";
}

void ThreadReName::onThreadReNameInit() {

}

void ThreadReName::onThreadReNameStart(const QFileInfoList &mSrcFileInfoList, const QFileInfoList &mDstFileInfoList) {
    qDebug()<<" string path is "<<mSrcFileInfoList<<mDstFileInfoList;
    for (int i = 0; i < mSrcFileInfoList.size(); ++i) {
//        execProcess("bash", QStringList()<<"-c"<<"sudo"<<"ln"<<"<<-s"<<mSrcFileInfoList.at(i).absoluteFilePath()<<mDstFileInfoList.at(i).absoluteFilePath());
//        execProcess("sudo", QStringList()<<"-S"<<"ls");
//        execProcess("bash", QStringList()<<"-c"<<"sudo -S mkdir /home/alex/test111");
//        execProcess("bash", QStringList()<<"-c"<<"sudo -S ls");
//        execProcess("ls", QStringList());
        execProcess("ln", QStringList()<<"-s"<<mSrcFileInfoList.at(i).absoluteFilePath()<<mDstFileInfoList.at(i).absoluteFilePath());
    }
}

QFileInfoList ThreadReName::getFileList(const QString &path, const QStringList &filterList) {
    QDir dir(path);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folderList.size(); i++) {
        QString name = folderList.at(i).absoluteFilePath();
        qDebug()<<"folder is "<<name<<i;
        QFileInfoList child_file_list = getFileList(name, filterList);
        fileList.append(child_file_list);
    }

    return fileList;
}

void ThreadReName::onThreadGetFiles(const QString &fileDir) {
    qDebug()<<" onThreadGetFiles string path is "<<fileDir;
    auto dataList = getFileList(fileDir, QStringList());
    qDebug()<<" srcPath file is "<<dataList;
    if (!dataList.isEmpty()) {
        emit signalFileList(dataList);
    }
}

bool ThreadReName::execProcess(const QString &cmd, const QStringList &param) {
    QProcess process;
    process.start(cmd, param);
//    process.start("sudo", QStringList()<<"ls");
//    process.start("sudo -S ls");
    // 等待进程启动
    if (!process.waitForStarted()) {
        qDebug() <<  "process start failed\n";
        return false;
    }
    process.closeWriteChannel();

    // 用于保存进程的控制台输出
    QByteArray procOutput;
    while (!process.waitForFinished(300)) {
        qDebug() <<  "process finished failed\n";
        return false;
    }
    procOutput = process.readAll();
    qDebug()<<" proc out put is "<<procOutput;
    return true;

}

