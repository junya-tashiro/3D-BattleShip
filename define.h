#ifndef DEFINE_H
#define DEFINE_H

#include <GL/glut.h>

#define BD_SIZE 4           //マスの1辺のサイズ
#define INIT_PITCH -30.0    //ピッチ角初期値
#define INIT_YAW 30.0       //ヨー角初期値
#define NO_POINT 100        //クリック位置に点がない場合のclicked_numの値, 0~(BD_SIZE^3)-1の整数でなければなんでもOK
#define WINDOW_SIZE 400     //ウィンドウサイズ, 400に固定
#define INTERVAL 10         //再描画までの時間[ms]
#define FOVY 30.0           //視野角度[°]
#define DISTANCE 20.0       //カメラと原点の距離

enum ship {
    UNKNOWN,    //黒色
    ROCK,       //表示なし
    NOSHIP,     //灰色
    ASHIP,      //緑色, 長さ3
    BSHIP,      //紫色, 長さ2
    CSHIP,      //黄色, 長さ1
    ATTACKED    //攻撃済み, 表示上は反映なし
};


#endif // DEFINE_H
