#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include "define.h"

#include <QOpenGLWidget>
#include <QLabel>


class master: public QLabel
{
    Q_OBJECT
public:
    explicit master(QWidget *parent = nullptr);

    virtual ~master();

    void set_ship_to_master(int x, int y, int z, enum ship S_1, enum ship S_2); //playerのSHIP配置をマスタに読み込む関数

    int get_turn_count(void);       //ターン数を返す関数, どちらのplayerのターンか判断するのに使用

    void set_attack_from_1(int n);  //player1からの攻撃に対する応答

    void set_attack_from_2(int n);  //player2からの攻撃に対する応答

    enum ship get_which_ship(void); //攻撃結果を返す関数

    int count_unattacked_1(void);   //player2の勝利判定用

    int count_unattacked_2(void);   //player1の勝利判定用

private:
    enum ship board_player_1[BD_SIZE][BD_SIZE][BD_SIZE];    //player_1の配置

    enum ship board_player_2[BD_SIZE][BD_SIZE][BD_SIZE];    //player_2の配置

    int turn_count;                 //ターン数

    enum ship which_ship;           //攻撃結果を入れておく箱
};


#endif // GAMEMASTER_H
