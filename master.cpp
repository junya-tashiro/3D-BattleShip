#include "master.h"


master::master(QWidget *parent) : QLabel (parent)
{
    turn_count = 0;

    //player1, player2の配置読み込み用配列を初期化
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                board_player_1[x][y][z] = UNKNOWN;
                board_player_2[x][y][z] = UNKNOWN;
            }
        }
    }
}


master::~master()
{

}


//playerのSHIP配置をマスタに読み込む関数
void master::set_ship_to_master(int x, int y, int z, enum ship S_1, enum ship S_2){
    //S_1の値をboard_player_1[x][y][z]に, S_2の値をboard_player_2[x][y][z]に代入
    board_player_1[x][y][z] = S_1;
    board_player_2[x][y][z] = S_2;
}


//ターン数を返す関数
int master::get_turn_count()
{
    return turn_count;
}


//player1からの攻撃に応答する関数
void master::set_attack_from_1(int n){
    //1変数の攻撃点情報(BD_SIZE進法でzxy)を3変数に
    int x = (n % (BD_SIZE * BD_SIZE)) / BD_SIZE;
    int y = n % BD_SIZE;
    int z = n / (BD_SIZE*BD_SIZE);

    which_ship = board_player_2[x][y][z];   //攻撃結果のセット
    board_player_2[x][y][z] = ATTACKED;     //マスのステータスを攻撃済みに変更
    turn_count++;                           //ターン数+1
}


//player2からの攻撃に応答する関数
void master::set_attack_from_2(int n){
    //1変数の攻撃点情報(BD_SIZE進法でzxy)を3変数に
    int x = (n % (BD_SIZE * BD_SIZE)) / BD_SIZE;
    int y = n % BD_SIZE;
    int z = n / (BD_SIZE*BD_SIZE);

    which_ship = board_player_1[x][y][z];   //攻撃結果のセット
    board_player_1[x][y][z] = ATTACKED;     //マスのステータスを攻撃済みに変更
    turn_count++;                           //ターン数+1
}


//攻撃結果を返す関数
enum ship master::get_which_ship()
{
    return which_ship;
}


//player2の勝利判定用
int master::count_unattacked_1()
{
    int count = 0;  //攻撃を受けていない船の数

    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(board_player_1[x][y][z] == ASHIP || board_player_1[x][y][z] == BSHIP || board_player_1[x][y][z] == CSHIP){
                    count++;
                }
            }
        }
    }
    return count;
}


//player1の勝利判定用
int master::count_unattacked_2()
{
    int count = 0;  //攻撃を受けていない船の数

    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(board_player_2[x][y][z] == ASHIP || board_player_2[x][y][z] == BSHIP || board_player_2[x][y][z] == CSHIP){
                    count++;
                }
            }
        }
    }
    return count;
}
