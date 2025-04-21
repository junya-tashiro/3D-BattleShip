#ifndef RESULT_2_H
#define RESULT_2_H

#include "mainwindow_2.h"

#include <QMainWindow>


namespace Ui {
class result_2;
}

class result_2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit result_2(QWidget *parent = nullptr); //コンストラクタ

    ~result_2();                                  //デコンストラクタ

    MainWindow_2 *window_mainwindow_2;              //メインウィンドウ

    void timerEvent(QTimerEvent *);             //タイマーイベント

protected slots:
    void deleteSlot();                          //windowのclose時に自動でdeleteも行う

protected:
    Ui::result_2 *ui;
};


#endif // RESULT_2_H
