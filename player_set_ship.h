#ifndef PLAYER_SET_SHIP_H
#define PLAYER_SET_SHIP_H

#include "player.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QColor>
#include <QOpenGLFunctions_1_0>
#include <GLUT/glut.h>


class player_set_ship : public player
{
    Q_OBJECT
public:
    explicit player_set_ship(QWidget *parent = nullptr);    //コンストラクタ

    virtual ~player_set_ship();                             //デコンストラクタ

    void init_print_board();                                //print_boardの初期化

    enum ship {
        CANCHOOSE,
        CANCHOOSE_TEMP,
        ROCK,
        NOSHIP,
        ASHIP1,
        ASHIP2,
        BSHIP1,
        BSHIP2,
        BSHIP3,
        CSHIP1,
        CSHIP2,
        CSHIP3,
        CSHIP4
    };

    int get_ship(int x, int y, int z);          //配置情報取得(ASHIPは1, BSHIPは2, CSHIPは3に)


    int now_index;                              //コンボボックスの選択状態

    void reset_can_choose();                    //CANCHOOSEのマスを元に戻す

    void reset_ship();                          //コンボボックスで選択されたSHIPを消去

    int count_ship(enum ship S);                //SHIPカウント

    void mousePressEvent(QMouseEvent *event);   //マウスのクリック時に呼ばれる関数

    void paintGL();                             //描画用関数

    enum ship print_board[BD_SIZE][BD_SIZE][BD_SIZE];   //SHIP配置

private slots:
    void deleteSlot();

private:
    enum ship ship[9] = {
        ASHIP1,
        ASHIP2,
        BSHIP1,
        BSHIP2,
        BSHIP3,
        CSHIP1,
        CSHIP2,
        CSHIP3,
        CSHIP4
    };

    int counter;                                                //クリックモーション用

    void timerEvent(QTimerEvent *);                             //定期的に再描画

    int is_not_other_ship(int x, int y, int z, enum ship S);    //S,NOSHIP,ROCK,CANCHOOSEかボード外の点なら1を, それ以外なら0を

    int is_aship(int x, int y, int z, enum ship S);             //(x, y, z)にSがいるか

    int can_put_ship(int x, int y, int z, enum ship S);         //周りに他の船がいないか, Sは例外として周囲にいてもOKなSHIP

    void set_canchoose(int x, int y, int z);                    //CANCHOOSEのセッター

    void set_canchoose_temp(int x, int y, int z);               //CANCHOOSE_TEMPのセッター

    void set_canchoose_to_board();                              //CANCHOOSEのセット
};

#endif // PLAYER_SET_SHIP_H
