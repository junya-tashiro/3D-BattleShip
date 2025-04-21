#include "player.h"

#include <GLUT/glut.h>
#include <math.h>
#include <QMouseEvent>

//コンストラクタ
player::player(QWidget *parent) : QOpenGLWidget (parent)
{
    pitch = INIT_PITCH;     //カメラのピッチ角初期値
    yaw = INIT_YAW;         //カメラのヨー角初期値
    startTimer(INTERVAL);   //再描画用のタイマースタート
    init_print_board();     //ボード状態の初期化
    player_counter = 0;
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                my_board[x][y][z] = NOSHIP;
            }
        }
    }
}


//デコンストラクタ
player::~player()
{
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                my_board[x][y][z] = NOSHIP;
            }
        }
    }

}


//リセットボタンが押された時の処理内容
void player::reset_button()
{
    pitch = INIT_PITCH;     //ピッチ角初期化
    yaw = INIT_YAW;         //ヨー角初期化
    clicked_num = NO_POINT; //clicked_numの初期化
}


//SHIPの配置を渡すための関数
enum ship player::get_ship(int x, int y, int z)
{
    return my_board[x][y][z];
}


enum ship player::get_print_board(int x, int y, int z){
    return print_board[x][y][z];
}


void player::set_ship(int x, int y, int z, enum ship S)
{
    my_board[x][y][z] = S;
}


//SHIPを表示用配列にセットするための関数
void player::set_ship_window(int x, int y, int z, enum ship S)
{
    print_board[x][y][z] = S;
}


//clicked_numを渡すための関数
int player::get_clicked_num()
{
    return clicked_num;
}


//clicked_numに値をセットするための関数
void player::set_clicked_num(int n)
{
    clicked_num = n;
}


//SHIP表示の初期化用関数
void player::init_print_board()
{
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                //端にはROCKをセット
                if(x % (BD_SIZE - 1) == 0 && y % (BD_SIZE - 1) == 0 && z % (BD_SIZE - 1) == 0 ){
                    print_board[x][y][z] = ROCK;
                }

                //それ以外はUNKNOWNに
                else{
                    print_board[x][y][z] = UNKNOWN;
                }
            }
        }
    }
}


//マウスのドラッグ時に呼ばれる関数
void player::mouseMoveEvent(QMouseEvent *event)
{
    int now_x, now_y, del_x, del_y;

    now_x = event->x();     //マウスのx座標
    now_y = event->y();     //マウスのy座標
    del_x = now_x - pre_x;  //x座標の変化量
    del_y = now_y - pre_y;  //y座標の変化量

    //変化量が大きすぎた場合は無効に
    if(del_x > 30 || del_y > 30 || del_x < -30 || del_y < -30){
        del_x = 0;
        del_y = 0;
    }

    //変化前座標の更新
    pre_x = now_x;
    pre_y = now_y;

    //ピッチ角, ヨー角の更新
    pitch -= del_y;
    yaw -= del_x;
}


//回転行列の計算
void player::calc_rot()
{
    float a = pitch * M_PI / (float)180;    //ピッチ角(ラジアン)
    float b = yaw * M_PI / (float)180;      //ヨー角(ラジアン)

    //回転行列に代入
    rot[0][0] =        cos(b);
    rot[0][1] = sin(a)*sin(b);
    rot[0][2] = cos(a)*sin(b);

    rot[1][0] =             0;
    rot[1][1] =        cos(a);
    rot[1][2] =       -sin(a);

    rot[2][0] =       -sin(b);
    rot[2][1] = sin(a)*cos(b);
    rot[2][2] = cos(a)*cos(b);
}


//クリック位置(3次元座標)の計算
void player::calc_pressed_coordinate(int x_win, int y_win, float t)
{
    //x_win,y_win:ウィンドウ上のマウス座標, t:直線用の媒介変数
    float x = t * (2 * ((((float)x_win / (float)width) - 0.5)) * tan(M_PI * (float)FOVY / (float)360));
    float y = t * (2 * ((((float)(height - y_win) / (float)height) - 0.5)) * tan(M_PI * (float)FOVY / (float)360));
    float z = DISTANCE - t;

    //クリックされた位置の座標計算(剛体固定座標系)
    pressed_coordinate[0] = rot[0][0] * x + rot[0][1] * y + rot[0][2] * z;
    pressed_coordinate[1] = rot[1][0] * x + rot[1][1] * y + rot[1][2] * z;
    pressed_coordinate[2] = rot[2][0] * x + rot[2][1] * y + rot[2][2] * z;
}


//マウスのクリック時に呼ばれる関数
void player::mousePressEvent(QMouseEvent *event)
{
    player_counter = 0;

    clicked_num = NO_POINT; //clicked_numの初期化

    calc_rot(); //回転行列計算

    float delta = 0.1;  //どの程度の座標のずれまで許容して選択するか

    //マウスの座標を代入
    int now_x = event->x();
    int now_y = event->y();

    for(float t = 1.5*DISTANCE; t > 0.5*DISTANCE; t -= 0.01){   //手前の点から選択されるようにtは奥から動かす
        calc_pressed_coordinate(now_x, now_y, t);   //剛体固定系での座標計算

        //近い位置に頂点があれば頂点の番号をclicked_numにセット
        for (int n = 0; n < BD_SIZE*BD_SIZE*BD_SIZE; n++) {
            if(
                (float)vertex[n][0] - pressed_coordinate[0] > -delta && (float)vertex[n][0] - pressed_coordinate[0] < delta &&  //x座標が十分近いか
                (float)vertex[n][1] - pressed_coordinate[1] > -delta && (float)vertex[n][1] - pressed_coordinate[1] < delta &&  //y座標が十分近いか
                (float)vertex[n][2] - pressed_coordinate[2] > -delta && (float)vertex[n][2] - pressed_coordinate[2] < delta     //z座標が十分近いか
                ){
                if(n != 0 && n != BD_SIZE-1 && n != BD_SIZE*(BD_SIZE-1) && n != BD_SIZE*BD_SIZE-1 && n != BD_SIZE*BD_SIZE*(BD_SIZE-1) && n != (BD_SIZE*BD_SIZE+1)*(BD_SIZE-1) && n != BD_SIZE*(BD_SIZE*BD_SIZE-1) && n != BD_SIZE*BD_SIZE*BD_SIZE-1){
                    clicked_num = n;
                }
            }
        }
    }
}


//再描画用関数
void player::timerEvent(QTimerEvent *)
{
    update();   //一定時間毎にpaintGL()を実行
    player_counter++;
}


//初期化用関数
void player::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
}


//ウィンドウサイズ変更時(起動時以外使用なし)
void player::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}


//描画用関数
void player::paintGL()
{
    glLoadIdentity();
    glColor3f(0, 0, 0);
    gluPerspective(FOVY, (double)width / (double)height, 1.0, 100.0);

    //カメラ視点セット
    gluLookAt(0.0, 0.0, DISTANCE, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //位置はz軸上, 原点向き, y軸正が上方向
    glRotatef(-pitch, 1.0, 0.0, 0.0 );  //x軸周りに剛体回転
    glRotatef(-yaw, 0.0, 1.0, 0.0 );    //y'軸周りに剛体回転

    //フレーム描画
    int i;
    glBegin(GL_LINES);
    for(i = 0; i < 3*BD_SIZE*BD_SIZE; i++){
        glVertex3dv(vertex[edge[i][0]]);
        glVertex3dv(vertex[edge[i][1]]);
    }
    glEnd();


    //頂点描画
    glPointSize((float)width/(float)27); //大きさ設定
    glBegin(GL_POINTS);
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(print_board[x][y][z] != ROCK){
                    int j = BD_SIZE * BD_SIZE * z + BD_SIZE * x + y;
                    if(j == clicked_num){
                        if(player_counter < 10){
                            glColor3f(1, 1, 1);     //選択点: 最初の10フレームは白色
                        }
                        else{
                            glColor3f(1, 0, 0);     //選択点: 赤色
                        }
                    }
                    else if(print_board[x][y][z] == NOSHIP){
                        glColor3f(0.8, 0.8, 0.8);   //NOSHIP: 灰色
                    }
                    else if(print_board[x][y][z] == ASHIP){
                        glColor3f(0.5, 1, 0.5);     //ASHIP: 緑色
                    }
                    else if(print_board[x][y][z] == BSHIP){
                        glColor3f(0.5, 0.3, 1);     //BSHIP: 紫色
                    }
                    else if(print_board[x][y][z] == CSHIP){
                        glColor3f(1, 0.8, 0);       //CSHIP: 黄色
                    }
                    else{
                        glColor3f(0, 0, 0);         //UNKNOWN: 黒色
                    }
                    glVertex3f(vertex[j][0], vertex[j][1], vertex[j][2]);
                }
            }
        }
    }

    glEnd();

    glFlush();
}
