#include "mainwindow.h"
#include "ui_mainwindow.h"


//コンストラクタ
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //メッセージ表示
    ui->message_for_player1->setText("Your turn!");
    ui->message_for_player2->setText("Not your turn!");

    //winnerフラグを0に
    winner_flag = 0;
}


//デコンストラクタ
MainWindow::~MainWindow()
{
    delete ui;
}


//SHIPセット
void MainWindow::set()
{
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){

                //マスタにセット
                ui->gamemaster->set_ship_to_master(x, y, z, ui->player_1->get_ship(x, y, z), ui->player_2->get_ship(x, y, z));
            }
        }
    }
}


//SHIPセット
void MainWindow::set_ship(int x, int y, int z, enum ship S1, enum ship S2)
{
    ui->player_1->set_ship(x, y, z, S1);
    ui->player_2->set_ship(x, y, z, S2);
}


//close時にdeleteも行う
void MainWindow::deleteSlot()
{
    static_cast<QWidget *>(sender())->deleteLater();
}



//player_1リセットボタン
void MainWindow::on_reset_attack_from_player1_released()
{
    ui->player_2->reset_button();
}


//player_2リセットボタン
void MainWindow::on_reset_attack_from_player2_released()
{
    ui->player_1->reset_button();
}


//player_1アタックボタン
void MainWindow::on_attack_from_player1_released()
{

    //player1の攻撃ターンの時
    if(ui->gamemaster->get_turn_count() % 2 == 0 && ui->gamemaster->count_unattacked_1() != 0 && ui->gamemaster->count_unattacked_2() != 0){

        //取得した攻撃点の番号
        int n = ui->player_2->get_clicked_num();

        //番号が適切にセットできている時のみ攻撃実施
        if(n != NO_POINT){

            //マスタに攻撃番号をセット
            ui->gamemaster->set_attack_from_1(n);

            //3変数化
            int x = (n % (BD_SIZE * BD_SIZE)) / BD_SIZE;
            int y = n % BD_SIZE;
            int z = n / (BD_SIZE * BD_SIZE);

            //攻撃済みでなかった場合のみ表示に結果を反映
            if(ui->gamemaster->get_which_ship() != ATTACKED){
                ui->player_2->set_ship_window(x, y, z, ui->gamemaster->get_which_ship());
            }

            //攻撃番号の初期化
            ui->player_2->set_clicked_num(NO_POINT);

            //メッセージ表示
            ui->message_for_player1->setText("Not your turn!");
            ui->message_for_player2->setText("Your turn!");

            //player2の船が全て攻撃済みになった場合
            if(ui->gamemaster->count_unattacked_2() == 0){

                //メッセージ表示
                ui->message_for_player1->setText("You win!");
                ui->message_for_player2->setText("You lose...");

                //winnerフラグ1に(これで結果windowが立ち上がる)
                winner_flag = 1;
            }
        }

        else
        {
            //メッセージ表示
            ui->message_for_player1->setText("Choose a point!");
        }
    }
}


//player_2アタックボタン
void MainWindow::on_attack_from_player2_released()
{
    //player2の攻撃ターンの時
    if(ui->gamemaster->get_turn_count() % 2 == 1 && ui->gamemaster->count_unattacked_1() != 0 && ui->gamemaster->count_unattacked_2() != 0){
        //取得した攻撃点の番号
        int n = ui->player_1->get_clicked_num();

        //番号が適切にセットできている時のみ攻撃実施
        if(n != NO_POINT){

            //マスタに攻撃番号をセット
            //master.set_attack_from_2(n);
            ui->gamemaster->set_attack_from_2(n);

            //3変数化
            int x = (n % (BD_SIZE * BD_SIZE)) / BD_SIZE;
            int y = n % BD_SIZE;
            int z = n / (BD_SIZE * BD_SIZE);

            //攻撃済みでなかった場合のみ表示に結果を反映
            if(ui->gamemaster->get_which_ship() != ATTACKED){
                ui->player_1->set_ship_window(x, y, z, ui->gamemaster->get_which_ship());
            }

            //攻撃番号の初期化
            ui->player_1->set_clicked_num(NO_POINT);

            //メッセージ表示
            ui->message_for_player1->setText("Your turn!");
            ui->message_for_player2->setText("Not your turn!");

            //player1の船が全て攻撃済みになった場合
            if(ui->gamemaster->count_unattacked_1() == 0){

                //メッセージ表示
                ui->message_for_player1->setText("You lose...");
                ui->message_for_player2->setText("You win!");

                //winnerフラグ2に(これで結果windowが立ち上がる)
                winner_flag = 2;
            }
        }

        else
        {
            //メッセージ表示
            ui->message_for_player2->setText("Choose a point!");
        }
    }
}
