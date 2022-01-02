//
// Created by alex on 2022/1/2.
//

#ifndef RENAME_THREADRENAME_H
#define RENAME_THREADRENAME_H

#include <QObject>
#include <QFileInfoList>

class ThreadReName : public QObject {
    Q_OBJECT
public:
    explicit ThreadReName(QObject *parent = nullptr);
    ~ThreadReName() override;

private:
    QFileInfoList getFileList(const QString &path, const QStringList &filterList);
    bool execProcess(const QString &cmd, const QStringList &param);
    bool execProcess(const QString &cmd, const QStringList &param, const QString &filePath, const QString &srcFile, const QString &dstFile);
signals:
    void signalStatus(bool value);
    void signalFileList(const QFileInfoList &list);

public slots:
    void onThreadReNameInit();
    void onThreadReNameStart(const QFileInfoList &mSrcFileInfoList, const QFileInfoList &mDstFileInfoList);
    void onThreadGetFiles(const QString &fileDir);
};


#endif //RENAME_THREADRENAME_H
