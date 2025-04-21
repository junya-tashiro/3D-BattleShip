#include "result_2.h"
#include "ui_result_2.h"


//コンストラクタ
result_2::result_2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::result_2)
{
    ui->setupUi(this);

    //メインウィンドウ表示
    window_mainwindow_2 = new MainWindow_2();
    window_mainwindow_2->setAttribute(Qt::WA_DeleteOnClose);  //close時にdeleteも

    window_mainwindow_2->show();

    //タイマースタート
    startTimer(300);
}


//デコンストラクタ
result_2::~result_2()
{
    delete ui;
    window_mainwindow_2->close(); //結果ウィンドウclose時にメインウィンドウも同時にclose
}


//タイマーイベント(定期的にwinner_flagをチェック)
void result_2::timerEvent(QTimerEvent *){

    //player1勝利時
    if(window_mainwindow_2->winner_flag == 1){
        ui->label->setText("Winner: Player");
        this->show();
    }

    //player2勝利時
    else if(window_mainwindow_2->winner_flag == 2){
        ui->label->setText("Winner: CPU");
        this->show();
    }
}


//close時にdeleteも行う
void result_2::deleteSlot()
{
    static_cast<QWidget *>(sender())->deleteLater();
}
