#include "start.h"
#include "ui_start.h"
#include "editor.h"

#include <QFileDialog>
#include <QDebug>

//コンスタラクタ
start::start(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::start)
{
    which_mode = 0;

    ui->setupUi(this);
    startTimer(300);

    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                //端にはROCKをセット
                if(x % (BD_SIZE - 1) == 0 && y % (BD_SIZE - 1) == 0 && z % (BD_SIZE - 1) == 0 ){
                    player1[x][y][z] = ROCK;
                    player2[x][y][z] = ROCK;
                }

                //それ以外はUNKNOWNに
                else{
                    player1[x][y][z] = NOSHIP;
                    player2[x][y][z] = NOSHIP;
                }
            }
        }
    }
}


//デコンストラクタ
start::~start()
{
    delete ui;
}


//定期的にflagをチェック(set_ship)
void start::timerEvent(QTimerEvent *){
    if(which_mode == 1){

        //player1のship配置が終了
        if(window_set_ship_1->flag == 1){
            for(int x = 0; x < BD_SIZE; x++){
                for(int y = 0; y < BD_SIZE; y++){
                    for(int z = 0; z < BD_SIZE; z++){

                        //player1に配置情報セット
                        if(x % (BD_SIZE - 1) != 0 || y % (BD_SIZE - 1) != 0 || z % (BD_SIZE - 1) != 0 ){

                            if(window_set_ship_1->ship[x][y][z] == 1){
                                player1[x][y][z] = ASHIP;
                            }
                            if(window_set_ship_1->ship[x][y][z] == 2){
                                player1[x][y][z] = BSHIP;
                            }
                            if(window_set_ship_1->ship[x][y][z] == 3){
                                player1[x][y][z] = CSHIP;
                            }
                        }
                    }
                }
            }

            //player2のship配置スタート
            window_set_ship_1->close();
            window_set_ship_2->show();
            window_set_ship_2->set_text_for_player2();
        }

        //player2のship配置が終了
        if(window_set_ship_2->flag == 1){
            for(int x = 0; x < BD_SIZE; x++){
                for(int y = 0; y < BD_SIZE; y++){
                    for(int z = 0; z < BD_SIZE; z++){

                        //player2に配置情報セット
                        if(x % (BD_SIZE - 1) != 0 || y % (BD_SIZE - 1) != 0 || z % (BD_SIZE - 1) != 0 ){
                            if(window_set_ship_2->ship[x][y][z] == 1){
                                player2[x][y][z] = ASHIP;
                            }
                            if(window_set_ship_2->ship[x][y][z] == 2){
                                player2[x][y][z] = BSHIP;
                            }
                            if(window_set_ship_2->ship[x][y][z] == 3){
                                player2[x][y][z] = CSHIP;
                            }
                        }
                    }
                }
            }

            window_set_ship_2->close();

            //結果ウィンドウ生成(表示はしない)
            window_result = new result();
            window_result->setAttribute(Qt::WA_DeleteOnClose); //close時にdeleteも

            //一旦MainWindowの中に配置情報を格納
            for(int x = 0; x < BD_SIZE; x++){
                for(int y = 0; y < BD_SIZE; y++){
                    for(int z = 0; z < BD_SIZE; z++){
                        window_result->window_mainwindow->set_ship(x, y, z, player1[x][y][z], player2[x][y][z]);
                    }
                }
            }

            //マスタに配置情報セット
            window_result->window_mainwindow->set();
        }
    }

    else if(which_mode == 2){

        //player1のship配置が終了
        if(window_set_ship_1->flag == 1){
            for(int x = 0; x < BD_SIZE; x++){
                for(int y = 0; y < BD_SIZE; y++){
                    for(int z = 0; z < BD_SIZE; z++){

                        //player1に配置情報セット
                        if(x % (BD_SIZE - 1) != 0 || y % (BD_SIZE - 1) != 0 || z % (BD_SIZE - 1) != 0 ){

                            if(window_set_ship_1->ship[x][y][z] == 1){
                                player1[x][y][z] = ASHIP;
                            }
                            if(window_set_ship_1->ship[x][y][z] == 2){
                                player1[x][y][z] = BSHIP;
                            }
                            if(window_set_ship_1->ship[x][y][z] == 3){
                                player1[x][y][z] = CSHIP;
                            }
                        }
                    }
                }
            }

            //player2のship配置スタート
            window_set_ship_1->close();


            //結果ウィンドウ生成(表示はしない)
            window_result_2 = new result_2();
            window_result_2->window_mainwindow_2->fileName = QFileDialog::getOpenFileName(this, tr(""), ".", tr("text(*.bshp)"));

            window_result_2->window_mainwindow_2->interpriter->ship_reader(window_result_2->window_mainwindow_2->fileName);
            window_result_2->setAttribute(Qt::WA_DeleteOnClose); //close時にdeleteも

            //一旦MainWindowの中に配置情報を格納
            for(int x = 0; x < BD_SIZE; x++){
                for(int y = 0; y < BD_SIZE; y++){
                    for(int z = 0; z < BD_SIZE; z++){
                        window_result_2->window_mainwindow_2->set_ship(x, y, z, player1[x][y][z], UNKNOWN);
                    }
                }
            }

            //マスタに配置情報セット
            window_result_2->window_mainwindow_2->set();
        }
    }
}


//Player vs Player
void start::on_start_button_2_released()
{
    which_mode = 1;

    //配置用ウィンドウ作成
    window_set_ship_1 = new set_ship();
    window_set_ship_2 = new set_ship();

    window_set_ship_1->setAttribute(Qt::WA_DeleteOnClose);
    window_set_ship_2->setAttribute(Qt::WA_DeleteOnClose);

    //player1配置ウィンドウ立ち上げ
    window_set_ship_1->show();
    window_set_ship_1->set_text_for_player1();

    close();
}


//Player vs CPU
void start::on_start_button_3_released()
{
    which_mode = 2;

    //配置用ウィンドウ作成
    window_set_ship_1 = new set_ship();
    //window_set_ship_2 = new set_ship();

    window_set_ship_1->setAttribute(Qt::WA_DeleteOnClose);
    //window_set_ship_2->setAttribute(Qt::WA_DeleteOnClose);

    //player1配置ウィンドウ立ち上げ
    window_set_ship_1->show();
    window_set_ship_1->set_text_for_player();

    close();
}


//CPU vs CPU
void start::on_start_button_4_released()
{
    window_result_3 = new result_3();
    window_result_3->window_mainwindow_3->fileName = QFileDialog::getOpenFileName(this, tr(""), ".", tr("text(*.bshp)"));
    window_result_3->window_mainwindow_3->interpriter->ship_reader(window_result_3->window_mainwindow_3->fileName);

    window_result_3->window_mainwindow_3->fileName2 = QFileDialog::getOpenFileName(this, tr(""), ".", tr("text(*.bshp)"));
    window_result_3->window_mainwindow_3->interpriter2->ship_reader(window_result_3->window_mainwindow_3->fileName2);

    window_result_3->setAttribute(Qt::WA_DeleteOnClose); //close時にdeleteも

    window_result_3->window_mainwindow_3->set();

    close();
}


void start::on_start_button_5_released()
{
    editor *window_editor = new editor(0);
    window_editor->show();
    if(window_editor->flag == 0){
        window_editor->close();
    }
}


void start::on_start_button_6_released()
{
    editor *window_editor = new editor(1);
    window_editor->show();
    if(window_editor->flag == 0){
        window_editor->close();
    }
}

