//
// Created by alex on 2022/1/2.
//

#ifndef RENAME_MAINWINDOW_H
#define RENAME_MAINWINDOW_H


#include <QMainWindow>
#include <QThread>
#include <QFileInfoList>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    constexpr static const int DEFAULT_COLUMN = 0;
    Ui::MainWindow *ui;
    QThread mRenameThread;
    QString mStringSrcPath;
    QString mStringDstPath;
    QFileInfoList mSrcFileInfoList;
    QFileInfoList mDstFileInfoList;

    void initData();
    void initViews();
    void initEvents();
    void initButton();
    void initLineEdit();
    void initRenameThread();
    void removeItem(QTreeWidgetItem *item, int index);

signals:
    void signalGetFiles(const QString &fileDir, const QStringList &filter);
    void signalRenameStart(const QFileInfoList &mSrcFileInfoList, const QFileInfoList &mDstFileInfoList);

private slots:
    void onPushButton();
    void onLienEdit();
    void onCustomContextMenuRequested(const QPoint &pos);
    void onDeleteItem();
};


#endif //RENAME_MAINWINDOW_H
