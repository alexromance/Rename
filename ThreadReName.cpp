//
// Created by alex on 2022/1/2.
//

#include "ThreadReName.h"

#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QCollator>

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
        if (mSrcFileInfoList.at(i).absolutePath() == mDstFileInfoList.at(i).absolutePath()){
            qDebug()<<"onThreadReNameStart "<<mSrcFileInfoList.at(i).fileName()<<mDstFileInfoList.at(i).fileName()<<i<<mDstFileInfoList;
            execProcess("ln", QStringList()<<"-s", mSrcFileInfoList.at(i).absolutePath(),
                        mSrcFileInfoList.at(i).fileName(), mDstFileInfoList.at(i).fileName());
        } else {
            execProcess("ln", QStringList()<<"-s"<<mSrcFileInfoList.at(i).absoluteFilePath()<<mDstFileInfoList.at(i).absoluteFilePath());
        }
    }
}

QFileInfoList ThreadReName::getFileList(const QString &path, const QStringList &filterList) {
    QDir dir(path);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folderList.size(); i++) {
        QString name = folderList.at(i).absoluteFilePath();
        qDebug()<<"folder is "<<name<<i;
        QFileInfoList childFileList = getFileList(name, filterList);
        fileList.append(childFileList);
    }

    return fileList;
}

void ThreadReName::onThreadGetFiles(const QString &fileDir) {
    qDebug()<<" onThreadGetFiles string path is "<<fileDir;
    auto dataList = getFileList(fileDir, QStringList());
    QFileInfoList sortedDataList;
    QStringList stringList;
    for (int i = 0; i < dataList.size(); ++i) {
        stringList.append(dataList.at(i).absoluteFilePath());
    }
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(stringList.begin(), stringList.end(), collator);
    qDebug()<<" srcPath file is "<<dataList;
    for (int i = 0; i < stringList.size(); ++i) {
        sortedDataList.append(QFileInfo(stringList.at(i)));
    }
    if (!sortedDataList.isEmpty()) {
        emit signalFileList(sortedDataList);
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

bool ThreadReName::execProcess(const QString &cmd, const QStringList &param, const QString &filePath,
                               const QString &srcFile, const QString &dstFile) {
    QProcess process;
    process.setWorkingDirectory(filePath);
    qDebug()<<"exec process"<<filePath<<srcFile<<dstFile;
    qDebug()<<"exec process string list"<<QStringList()<<param<<srcFile<<dstFile;
    process.start(cmd, QStringList()<<param<<srcFile<<dstFile);
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

