#include "result_3.h"
#include "ui_result_3.h"


//コンストラクタ
result_3::result_3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::result_3)
{
    ui->setupUi(this);

    //メインウィンドウ表示
    window_mainwindow_3 = new MainWindow_3();
    window_mainwindow_3->setAttribute(Qt::WA_DeleteOnClose);  //close時にdeleteも

    window_mainwindow_3->show();

    //タイマースタート
    startTimer(300);
}


//デコンストラクタ
result_3::~result_3()
{
    delete ui;
    window_mainwindow_3->close(); //結果ウィンドウclose時にメインウィンドウも同時にclose
}


//タイマーイベント(定期的にwinner_flagをチェック)
void result_3::timerEvent(QTimerEvent *){

    //player1勝利時
    if(window_mainwindow_3->winner_flag == 1){
        ui->label->setText("Winner: CPU1");
        this->show();
    }

    //player2勝利時
    else if(window_mainwindow_3->winner_flag == 2){
        ui->label->setText("Winner: CPU2");
        this->show();
    }
}


//close時にdeleteも行う
void result_3::deleteSlot()
{
    static_cast<QWidget *>(sender())->deleteLater();
}
