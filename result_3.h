#ifndef RESULT_3_H
#define RESULT_3_H

#include "mainwindow_3.h"

#include <QMainWindow>


namespace Ui {
class result_3;
}

class result_3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit result_3(QWidget *parent = nullptr); //コンストラクタ

    ~result_3();                                  //デコンストラクタ

    MainWindow_3 *window_mainwindow_3;              //メインウィンドウ

    void timerEvent(QTimerEvent *);             //タイマーイベント

protected slots:
    void deleteSlot();                          //windowのclose時に自動でdeleteも行う

protected:
    Ui::result_3 *ui;
};


#endif // RESULT_3_H
