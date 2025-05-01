#include "result.h"
#include "ui_result.h"
#include "ui_result_2.h"


//コンストラクタ
result::result(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::result_2)
{
    ui->setupUi(this);

    //メインウィンドウ表示
    window_mainwindow = new MainWindow();
    window_mainwindow->setAttribute(Qt::WA_DeleteOnClose);  //close時にdeleteも
    window_mainwindow->show();

    //タイマースタート
    startTimer(300);
}


//デコンストラクタ
result::~result()
{
    delete ui;
    window_mainwindow->close(); //結果ウィンドウclose時にメインウィンドウも同時にclose
}


//タイマーイベント(定期的にwinner_flagをチェック)
void result::timerEvent(QTimerEvent *){

    //player1勝利時
    if(window_mainwindow->winner_flag == 1){
        ui->label->setText("Winner: player 1");
        this->show();
    }

    //player2勝利時
    else if(window_mainwindow->winner_flag == 2){
        ui->label->setText("Winner: player 2");
        this->show();
    }
}


//close時にdeleteも行う
void result::deleteSlot()
{
    static_cast<QWidget *>(sender())->deleteLater();
}
