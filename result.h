#ifndef RESULT_H
#define RESULT_H

#include "mainwindow.h"

#include <QMainWindow>


namespace Ui {
class result_2;
}

class result : public QMainWindow
{
    Q_OBJECT

public:
    explicit result(QWidget *parent = nullptr); //コンストラクタ

    ~result();                                  //デコンストラクタ

    MainWindow *window_mainwindow;              //メインウィンドウ

    void timerEvent(QTimerEvent *);             //タイマーイベント

protected slots:
    void deleteSlot();                          //windowのclose時に自動でdeleteも行う

protected:
    Ui::result_2 *ui;
};


#endif // RESULT_H
