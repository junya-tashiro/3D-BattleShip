#include "mainwindow_3.h"
#include "ui_mainwindow_3.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>


//コンストラクタ
MainWindow_3::MainWindow_3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_3)
{
    ui->setupUi(this);

    interpriter = new bsl_interpriter();
    interpriter2 = new bsl_interpriter();

    //winnerフラグを0に
    winner_flag = 0;
}


//デコンストラクタ
MainWindow_3::~MainWindow_3()
{
    delete ui;
}


//SHIPセット
void MainWindow_3::set()
{
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){

                enum ship S1 = UNKNOWN;
                enum ship S2 = UNKNOWN;
                if(interpriter->ship[x][y][z] == 0){
                    S1 = ROCK;
                }
                if(interpriter->ship[x][y][z] == 1){
                    S1 = NOSHIP;
                }
                if(interpriter->ship[x][y][z] == 2){
                    S1 = ASHIP;
                }
                if(interpriter->ship[x][y][z] == 3){
                    S1 = BSHIP;
                }
                if(interpriter->ship[x][y][z] == 4){
                    S1 = CSHIP;
                }
                if(interpriter2->ship[x][y][z] == 0){
                    S2 = ROCK;
                }
                if(interpriter2->ship[x][y][z] == 1){
                    S2 = NOSHIP;
                }
                if(interpriter2->ship[x][y][z] == 2){
                    S2 = ASHIP;
                }
                if(interpriter2->ship[x][y][z] == 3){
                    S2 = BSHIP;
                }
                if(interpriter2->ship[x][y][z] == 4){
                    S2 = CSHIP;
                }

                //マスタにセット
                ui->gamemaster->set_ship_to_master(x, y, z, S1, S2);
            }
        }
    }
}


//SHIPセット
void MainWindow_3::set_ship(int x, int y, int z, enum ship S1, enum ship S2)
{
    ui->player_1->set_ship(x, y, z, S1);
    ui->player_2->set_ship(x, y, z, S2);
}


//close時にdeleteも行う
void MainWindow_3::deleteSlot()
{
    static_cast<QWidget *>(sender())->deleteLater();
}



//player_1リセットボタン
void MainWindow_3::on_reset_attack_from_player1_released()
{
    ui->player_2->reset_button();
}


//player_2リセットボタン
void MainWindow_3::on_reset_attack_from_player2_released()
{
    ui->player_1->reset_button();
}


void MainWindow_3::timerEvent(QTimerEvent *)
{
    if(winner_flag == 0){
        if(turn % 2 == 0){
            int n = interpriter->interpriter(fileName);
            //マスタに攻撃番号をセット
            ui->gamemaster->set_attack_from_1(n);

            //3変数化
            int x = (n % (BD_SIZE * BD_SIZE)) / BD_SIZE;
            int y = n % BD_SIZE;
            int z = n / (BD_SIZE * BD_SIZE);

            //攻撃済みでなかった場合のみ表示に結果を反映
            if(ui->gamemaster->get_which_ship() != ATTACKED){
                ui->player_2->set_ship_window(x, y, z, ui->gamemaster->get_which_ship());

                //interpriterにボード状態を反映
                interpriter->set_enemy_board(z, x, y, ui->player_2->get_print_board(x, y, z));
            }

            //攻撃番号の初期化
            ui->player_2->set_clicked_num(NO_POINT);

            //player2の船が全て攻撃済みになった場合
            if(ui->gamemaster->count_unattacked_2() == 0){

                //winnerフラグ1に(これで結果windowが立ち上がる)
                winner_flag = 1;
            }
            turn++;
        }
        else{
            int n = interpriter2->interpriter(fileName2);
            //マスタに攻撃番号をセット
            ui->gamemaster->set_attack_from_2(n);

            //3変数化
            int x = (n % (BD_SIZE * BD_SIZE)) / BD_SIZE;
            int y = n % BD_SIZE;
            int z = n / (BD_SIZE * BD_SIZE);

            //攻撃済みでなかった場合のみ表示に結果を反映
            if(ui->gamemaster->get_which_ship() != ATTACKED){
                ui->player_1->set_ship_window(x, y, z, ui->gamemaster->get_which_ship());

                //interpriterにボード状態を反映
                interpriter->set_enemy_board(z, x, y, ui->player_1->get_print_board(x, y, z));
            }

            //攻撃番号の初期化
            ui->player_1->set_clicked_num(NO_POINT);

            //player1の船が全て攻撃済みになった場合
            if(ui->gamemaster->count_unattacked_1() == 0){

                //winnerフラグ2に(これで結果windowが立ち上がる)
                winner_flag = 2;
            }
            turn++;
        }
    }
}


void MainWindow_3::on_pushButton_released()
{
    turn = 0;
    startTimer(30);
}

