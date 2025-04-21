#ifndef SET_SHIP_H
#define SET_SHIP_H

#include "define.h"
#include "warning.h"

#include <QMainWindow>

namespace Ui {
class set_ship;
}

class set_ship : public QMainWindow
{
    Q_OBJECT

public:
    explicit set_ship(QWidget *parent = nullptr);       //コンストラクタ
    ~set_ship();                                        //デコンストラクタ

    int flag;                                           //終了判定用フラグ

    int ship[BD_SIZE][BD_SIZE][BD_SIZE];                //配置格納用

    void set_text_for_player1();

    void set_text_for_player2();

    void set_text_for_player();

private slots:
    void on_comboBox_currentIndexChanged(int index);    //コンボボックス

    void on_pushButton_released();                      //表示リセットボタン

    void on_pushButton_2_released();                    //deleteボタン

    void on_pushButton_3_released();                    //OKボタン

    void deleteSlot();

private:
    Ui::set_ship *ui;

    warning *window_warning;
};

#endif // SET_SHIP_H
