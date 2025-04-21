#include "set_ship.h"
#include "ui_set_ship.h"


//コンストラクタ
set_ship::set_ship(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::set_ship)
{
    ui->setupUi(this);

    flag = 0;   //フラグは0に

    //配置初期化
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                ship[x][y][z] = 0;
            }
        }
    }

    //コンボボックスの中身
    ui->comboBox->addItem("shipA (1)");
    ui->comboBox->addItem("shipA (2)");
    ui->comboBox->addItem("shipB (1)");
    ui->comboBox->addItem("shipB (2)");
    ui->comboBox->addItem("shipB (3)");
    ui->comboBox->addItem("shipC (1)");
    ui->comboBox->addItem("shipC (2)");
    ui->comboBox->addItem("shipC (3)");
    ui->comboBox->addItem("shipC (4)");
}


//デコンストラクタ
set_ship::~set_ship()
{
    delete ui;
}


void set_ship::set_text_for_player1()
{
    ui->label->setText("Player 1");
}


void set_ship::set_text_for_player2()
{
    ui->label->setText("Player 2");
}


void set_ship::set_text_for_player()
{
    ui->label->setText("Player");
}


//indexに現在のコンボボックスの選択内容を格納
void set_ship::on_comboBox_currentIndexChanged(int index)
{
    ui->player->now_index = index;
    ui->player->reset_can_choose();
}


//表示リセットボタン
void set_ship::on_pushButton_released()
{
    ui->player->reset_button();
}


//deleteボタン
void set_ship::on_pushButton_2_released()
{
    ui->player->reset_ship();
}


//OKボタン
void set_ship::on_pushButton_3_released()
{
    //全てのSHIPが配置済みの時のみ反応
    if(ui->player->count_ship(player_set_ship::ASHIP1) == 3 && ui->player->count_ship(player_set_ship::ASHIP2) == 3 && ui->player->count_ship(player_set_ship::BSHIP1) == 2 && ui->player->count_ship(player_set_ship::BSHIP2) == 2 && ui->player->count_ship(player_set_ship::BSHIP3) == 2 && ui->player->count_ship(player_set_ship::CSHIP1) == 1 && ui->player->count_ship(player_set_ship::CSHIP2) == 1 && ui->player->count_ship(player_set_ship::CSHIP3) == 1 && ui->player->count_ship(player_set_ship::CSHIP4) == 1){

        flag = 1;

        //配置情報格納
        for(int x = 0; x < BD_SIZE; x++){
            for(int y = 0; y < BD_SIZE; y++){
                for(int z = 0; z < BD_SIZE; z++){
                    ship[x][y][z] = ui->player->get_ship(x, y, z);
                }
            }
        }
    }
    else{
        window_warning = new warning();
        window_warning->show();
    }
}


//close時にdeleteも行う
void set_ship::deleteSlot()
{
    static_cast<QWidget *>(sender())->deleteLater();
}
