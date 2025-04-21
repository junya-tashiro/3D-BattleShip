#ifndef START_H
#define START_H

#include "define.h"

#include "result.h"
#include "result_2.h"
#include "result_3.h"
#include "set_ship.h"

#include <QMainWindow>


namespace Ui {
class start;
}


class start : public QMainWindow
{
    Q_OBJECT

public:
    explicit start(QWidget *parent = nullptr);  //コンストラクタ

    ~start();                                   //デコンストラクタ

    result *window_result;                      //結果ウィンドウ
    result_2 *window_result_2;
    result_3 *window_result_3;

    set_ship * window_set_ship_1;               //player1配置設定用ウィンドウ

    set_ship * window_set_ship_2;               //player2配置設定用ウィンドウ

    enum ship player1[BD_SIZE][BD_SIZE][BD_SIZE];   //player1配置

    enum ship player2[BD_SIZE][BD_SIZE][BD_SIZE];   //player2配置

    void timerEvent(QTimerEvent *);             //タイマーイベント

private slots:    
    void on_start_button_2_released();          //スタートボタン

    void on_start_button_3_released();

    void on_start_button_4_released();

    void on_start_button_6_released();

    void on_start_button_5_released();

private:
    int which_mode;

    Ui::start *ui;
};


#endif // START_H
