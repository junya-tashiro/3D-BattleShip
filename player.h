#ifndef PLAYER_H
#define PLAYER_H

#include "define.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QColor>
#include <QOpenGLFunctions_1_0>
#include <GLUT/glut.h>


class player : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit player(QWidget *parent = nullptr);             //コンストラクタ

    virtual ~player();                                      //デコンストラクタ

    void reset_button(void);                                //リセットボタンが押された時の処理

    enum ship get_ship(int x, int y, int z);                //SHIPの配置を渡すための関数

    enum ship get_print_board(int x, int y, int z);

    void set_ship(int x, int y, int z, enum ship S);

    void set_ship_window(int x, int y, int z, enum ship S); //SHIPを表示用配列にセットするための関数

    int get_clicked_num(void);                              //clicked_numを渡すための関数

    void set_clicked_num(int n);                            //clicked_numに値をセットするための関数

    void init_print_board(void);                            //SHIP表示の初期化

protected:
    enum ship my_board[BD_SIZE][BD_SIZE][BD_SIZE];


    enum ship print_board[BD_SIZE][BD_SIZE][BD_SIZE];   //表示用SHIPの配置

    //フレーム構成用の頂点
    GLdouble vertex[BD_SIZE*BD_SIZE*BD_SIZE][3] = {
        {-3.0, -3.0, -3.0},     //00
        {-3.0, -1.0, -3.0},     //01
        {-3.0,  1.0, -3.0},     //02
        {-3.0,  3.0, -3.0},     //03
        {-1.0, -3.0, -3.0},     //04
        {-1.0, -1.0, -3.0},     //05
        {-1.0,  1.0, -3.0},     //06
        {-1.0,  3.0, -3.0},     //07
        { 1.0, -3.0, -3.0},     //08
        { 1.0, -1.0, -3.0},     //09
        { 1.0,  1.0, -3.0},     //10
        { 1.0,  3.0, -3.0},     //11
        { 3.0, -3.0, -3.0},     //12
        { 3.0, -1.0, -3.0},     //13
        { 3.0,  1.0, -3.0},     //14
        { 3.0,  3.0, -3.0},     //15

        {-3.0, -3.0, -1.0},     //16
        {-3.0, -1.0, -1.0},     //17
        {-3.0,  1.0, -1.0},     //18
        {-3.0,  3.0, -1.0},     //19
        {-1.0, -3.0, -1.0},     //20
        {-1.0, -1.0, -1.0},     //21
        {-1.0,  1.0, -1.0},     //22
        {-1.0,  3.0, -1.0},     //23
        { 1.0, -3.0, -1.0},     //24
        { 1.0, -1.0, -1.0},     //25
        { 1.0,  1.0, -1.0},     //26
        { 1.0,  3.0, -1.0},     //27
        { 3.0, -3.0, -1.0},     //28
        { 3.0, -1.0, -1.0},     //29
        { 3.0,  1.0, -1.0},     //30
        { 3.0,  3.0, -1.0},     //31

        {-3.0, -3.0,  1.0},     //32
        {-3.0, -1.0,  1.0},     //33
        {-3.0,  1.0,  1.0},     //34
        {-3.0,  3.0,  1.0},     //35
        {-1.0, -3.0,  1.0},     //36
        {-1.0, -1.0,  1.0},     //37
        {-1.0,  1.0,  1.0},     //38
        {-1.0,  3.0,  1.0},     //39
        { 1.0, -3.0,  1.0},     //40
        { 1.0, -1.0,  1.0},     //41
        { 1.0,  1.0,  1.0},     //42
        { 1.0,  3.0,  1.0},     //43
        { 3.0, -3.0,  1.0},     //44
        { 3.0, -1.0,  1.0},     //45
        { 3.0,  1.0,  1.0},     //46
        { 3.0,  3.0,  1.0},     //47

        {-3.0, -3.0,  3.0},     //48
        {-3.0, -1.0,  3.0},     //49
        {-3.0,  1.0,  3.0},     //50
        {-3.0,  3.0,  3.0},     //51
        {-1.0, -3.0,  3.0},     //52
        {-1.0, -1.0,  3.0},     //53
        {-1.0,  1.0,  3.0},     //54
        {-1.0,  3.0,  3.0},     //55
        { 1.0, -3.0,  3.0},     //56
        { 1.0, -1.0,  3.0},     //57
        { 1.0,  1.0,  3.0},     //58
        { 1.0,  3.0,  3.0},     //59
        { 3.0, -3.0,  3.0},     //60
        { 3.0, -1.0,  3.0},     //61
        { 3.0,  1.0,  3.0},     //62
        { 3.0,  3.0,  3.0},     //63
    };

    int edge[3*BD_SIZE*BD_SIZE][2] = {
        { 4,  8}, { 1, 13}, { 2, 14}, { 7, 11},
        { 1,  2}, { 4,  7}, { 8, 11}, {13, 14},

        {16, 28}, {17, 29}, {18, 30}, {19, 31},
        {16, 19}, {20, 23}, {24, 27}, {28, 31},

        {32, 44}, {33, 45}, {34, 46}, {35, 47},
        {32, 35}, {36, 39}, {40, 43}, {44, 47},

        {52, 56}, {49, 61}, {50, 62}, {55, 59},
        {49, 50}, {52, 55}, {56, 59}, {61, 62},

        {16, 32}, { 1, 49}, { 2, 50}, {19, 35},
        { 4, 52}, { 5, 53}, { 6, 54}, { 7, 55},

        { 8, 56}, { 9, 57}, {10, 58}, {11, 59},
        {28, 44}, {13, 61}, {14, 62}, {31, 47}
    };

    void mouseMoveEvent(QMouseEvent *event);    //マウスドラッグ時: ピッチ角, ヨー角を変更

    int pre_x = 0;              //マウスx座標(移動前)

    int pre_y = 0;              //マウスy座標(移動前)

    int width = WINDOW_SIZE;    //ウィンドウ幅

    int height = WINDOW_SIZE;   //ウィンドウ高さ

    float yaw, pitch;           //視点のピッチ角, ヨー角

    void mousePressEvent(QMouseEvent *event);   //マウスクリック時: calc_rot(), calc_pressed_coordinate()を使用してclicked_numにクリックされた点の番号をセット

    float rot[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };                                          //回転行列

    float pressed_coordinate[3] = {0, 0, 0};    //クリック点の剛体固定系座標

    void calc_rot();                            //クリックされた直線(ウィンドウ上では点)の回転行列を計算

    void calc_pressed_coordinate(int x_win, int y_win, float t);
                                                //クリックされた点の剛体固定系座標を計算（x_win, y_win: マウス座標, t:媒介変数)

    int clicked_num = NO_POINT;                 //クリック点の番号(初期値はNO_POINT)

    int player_counter;

    void timerEvent(QTimerEvent *);     //定期的に再描画

    void initializeGL();                //初期化

    void paintGL();                     //図形描画

    void resizeGL(int w, int h);        //ウィンドウサイズ変更
};


#endif // PLAYER_H
