#include "player_set_ship.h"

#include <GLUT/glut.h>
#include <math.h>
#include <QMouseEvent>


//コンストラクタ
player_set_ship::player_set_ship(QWidget *parent) : player(parent)
{
    init_print_board();
    now_index = 0;
    clicked_num = NO_POINT;
    counter = 0;
}


//デコンストラクタ
player_set_ship::~player_set_ship()
{
    init_print_board();
}


//SHIP表示の初期化用関数
void player_set_ship::init_print_board()
{
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                //端にはROCKをセット
                if(x % (BD_SIZE - 1) == 0 && y % (BD_SIZE - 1) == 0 && z % (BD_SIZE - 1) == 0 ){
                    print_board[x][y][z] = ROCK;
                }

                //それ以外はNOSIHPに
                else{
                    print_board[x][y][z] = NOSHIP;
                }
            }
        }
    }
}


//配置情報取得(ASHIPは1, BSHIPは2, CSHIPは3に)
int player_set_ship::get_ship(int x, int y, int z){
    if(print_board[x][y][z] == ASHIP1 || print_board[x][y][z] == ASHIP2){
        return 1;
    }
    if(print_board[x][y][z] == BSHIP1 || print_board[x][y][z] == BSHIP2 || print_board[x][y][z] == BSHIP3){
        return 2;
    }
    if(print_board[x][y][z] == CSHIP1 || print_board[x][y][z] == CSHIP2 || print_board[x][y][z] == CSHIP3 || print_board[x][y][z] == CSHIP4){
        return 3;
    }
    else{
        return 0;
    }
}


//CANCHOOSEのマスを元に戻す
void player_set_ship::reset_can_choose()
{
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(print_board[x][y][z] == CANCHOOSE){
                    print_board[x][y][z] = NOSHIP;
                }
            }
        }
    }
}


//コンボボックスで選択されたSHIPを消去
void player_set_ship::reset_ship()
{
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(print_board[x][y][z] == ship[now_index]){
                    print_board[x][y][z] = NOSHIP;
                }
            }
        }
    }
}


//マウスのクリック時に呼ばれる関数
void player_set_ship::mousePressEvent(QMouseEvent *event)
{
    counter = 0;    //クリックモーション用

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

    //選択された点にSHIPを配置可能だった場合
    if(clicked_num != NO_POINT){
        int x1 = (clicked_num % (BD_SIZE * BD_SIZE)) / BD_SIZE;
        int y1 = clicked_num % BD_SIZE;
        int z1 = clicked_num / (BD_SIZE*BD_SIZE);


        if(print_board[x1][y1][z1] == CANCHOOSE){
            print_board[x1][y1][z1] = ship[now_index];
        }

        reset_can_choose();
    }
}


//定期的に再描画
void player_set_ship::timerEvent(QTimerEvent *)
{
    update();
    counter++;
}


//描画用関数
void player_set_ship::paintGL()
{

    set_canchoose_to_board();   //CANCHOOSEのセット

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


    //頂点描画(非選択SHIP)
    glPointSize((float)width/(float)27); //大きさ設定
    glBegin(GL_POINTS);
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(print_board[x][y][z] != ROCK){
                    if(print_board[x][y][z] != ship[now_index]){
                        int j = BD_SIZE * BD_SIZE * z + BD_SIZE * x + y;

                        if(j == clicked_num && counter < 10){
                            glColor3f(1, 1, 1);         //選択点: 最初の10フレームは白色
                        }
                        else if(print_board[x][y][z] == NOSHIP){
                            glColor3f(0.8, 0.8, 0.8);   //NOSHIP: 灰色
                        }
                        else if(print_board[x][y][z] == ASHIP1 || print_board[x][y][z] == ASHIP2){
                            glColor3f(0.5, 1, 0.5);     //ASHIP: 緑色
                        }
                        else if(print_board[x][y][z] == BSHIP1 || print_board[x][y][z] == BSHIP2 || print_board[x][y][z] == BSHIP3){
                            glColor3f(0.5, 0.3, 1);     //BSHIP: 紫色
                        }
                        else if(print_board[x][y][z] == CSHIP1 || print_board[x][y][z] == CSHIP2 || print_board[x][y][z] == CSHIP3 || print_board[x][y][z] == CSHIP4){
                            glColor3f(1, 0.8, 0);       //CSHIP: 黄色
                        }
                        else if(print_board[x][y][z] == CANCHOOSE){
                            glColor3f(0, 0, 0);         //CANCHOOSE: 黒色
                        }

                        glVertex3f(vertex[j][0], vertex[j][1], vertex[j][2]);
                    }
                }
            }
        }
    }

    glEnd();

    //頂点描画(選択SHIP)
    glPointSize((float)width/(float)21); //大きさ設定
    glBegin(GL_POINTS);
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(print_board[x][y][z] != ROCK){
                    if(print_board[x][y][z] == ship[now_index]){
                        int j = BD_SIZE * BD_SIZE * z + BD_SIZE * x + y;

                        if(j == clicked_num && counter < 10){
                            glColor3f(1, 1, 1);         //選択点: 最初の10フレームは白色
                        }
                        else if(print_board[x][y][z] == NOSHIP){
                            glColor3f(0.8, 0.8, 0.8);   //NOSHIP: 灰色
                        }
                        else if(print_board[x][y][z] == ASHIP1 || print_board[x][y][z] == ASHIP2){
                            glColor3f(0.5, 1, 0.5);     //ASHIP: 緑色
                        }
                        else if(print_board[x][y][z] == BSHIP1 || print_board[x][y][z] == BSHIP2 || print_board[x][y][z] == BSHIP3){
                            glColor3f(0.5, 0.3, 1);     //BSHIP: 紫色
                        }
                        else if(print_board[x][y][z] == CSHIP1 || print_board[x][y][z] == CSHIP2 || print_board[x][y][z] == CSHIP3 || print_board[x][y][z] == CSHIP4){
                            glColor3f(1, 0.8, 0);       //CSHIP: 黄色
                        }
                        else if(print_board[x][y][z] == CANCHOOSE){
                            glColor3f(0, 0, 0);         //CANCHOOSE: 黒色
                        }

                        glVertex3f(vertex[j][0], vertex[j][1], vertex[j][2]);
                    }
                }
            }
        }
    }

    glEnd();

    glFlush();
}


//close時にdeleteも行う
void player_set_ship::deleteSlot()
{
    static_cast<QWidget *>(sender())->deleteLater();
}


//SHIPカウント
int player_set_ship::count_ship(enum ship S){
    int count = 0;
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(print_board[x][y][z] == S){
                    count++;
                }
            }
        }
    }
    return count;
}


//S,NOSHIP,ROCK,CANCHOOSEかボード外の点なら1を, それ以外なら0を
int player_set_ship::is_not_other_ship(int x, int y, int z, enum ship S)
{
    if(x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE  && z < BD_SIZE){
        if(print_board[x][y][z] == NOSHIP || print_board[x][y][z] == CANCHOOSE || print_board[x][y][z] == ROCK || print_board[x][y][z] == S){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 1;
    }
}


//(x, y, z)にSがいるか
int player_set_ship::is_aship(int x, int y, int z, enum ship S)
{
    if(x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE  && z < BD_SIZE && print_board[x][y][z] != ROCK){
        if(print_board[x][y][z] == S){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}


//周りに他の船がいないか, Sは例外として周囲にいてもOKなSHIP
int player_set_ship::can_put_ship(int x, int y, int z, enum ship S)
{
    if(x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE  && z < BD_SIZE && print_board[x][y][z] != ROCK){
        if(is_not_other_ship(x-1, y, z, S) && is_not_other_ship(x+1, y, z, S) && is_not_other_ship(x, y-1, z, S) && is_not_other_ship(x, y+1, z, S) && is_not_other_ship(x, y, z-1, S) && is_not_other_ship(x, y, z+1, S)){
            return  1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}


//CANCHOOSEのセッター
void player_set_ship::set_canchoose(int x, int y, int z){
    if(print_board[x][y][z] == NOSHIP || print_board[x][y][z] == CANCHOOSE_TEMP){
        print_board[x][y][z] = CANCHOOSE;
    }
}


//CANCHOOSE_TEMPのセッター
void player_set_ship::set_canchoose_temp(int x, int y, int z){
    if(print_board[x][y][z] == NOSHIP || print_board[x][y][z] == CANCHOOSE){
        print_board[x][y][z] = CANCHOOSE_TEMP;
    }
}


//CANCHOOSEのセット
void player_set_ship::set_canchoose_to_board()
{
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){

                //ASHIP1からCSHIP4
                for(int index = 0; index < 9; index++){

                    //ASHIP
                    if(index <= 1){
                        if(now_index == index){

                            //同種のAが2つある場合
                            if(count_ship(ship[index]) == 2){
                                if(is_aship(x, y, z, ship[index])){

                                    //(x, y, z), (x-2, y, z)がA, (x-1, y, z)に配置可能
                                    if(can_put_ship(x-1, y, z, ship[index]) && is_aship(x-2, y, z, ship[index])){
                                        set_canchoose(x-1, y, z);
                                    }

                                    //(x, y, z), (x＋2, y, z)がA, (x＋1, y, z)に配置可能
                                    if(can_put_ship(x+1, y, z, ship[index]) && is_aship(x+2, y, z, ship[index])){
                                        set_canchoose(x+1, y, z);
                                    }

                                    //(x, y, z), (x, y−2, z)がA, (x, y-1, z)に配置可能
                                    if(can_put_ship(x, y-1, z, ship[index]) && is_aship(x, y-2, z, ship[index])){
                                        set_canchoose(x, y-1, z);
                                    }

                                    //(x, y, z), (x, y+2, z)がA, (x, y+1, z)に配置可能
                                    if(can_put_ship(x, y+1, z, ship[index]) && is_aship(x, y+2, z, ship[index])){
                                        set_canchoose(x, y+1, z);
                                    }

                                    //(x, y, z), (x, y, z-2)がA, (x, y, z-1)に配置可能
                                    if(can_put_ship(x, y, z-1, ship[index]) && is_aship(x, y, z-2, ship[index])){
                                        set_canchoose(x, y, z-1);
                                    }

                                    //(x, y, z), (x, y, z+2)がA, (x, y, z+1)に配置可能
                                    if(can_put_ship(x, y, z+1, ship[index]) && is_aship(x, y, z+2, ship[index])){
                                        set_canchoose(x, y, z+1);
                                    }

                                    //(x, y, z), (x-1, y, z)がA, (x-2, y, z)に配置可能
                                    if(can_put_ship(x-2, y, z, ship[index]) && is_aship(x-1, y, z, ship[index])){
                                        set_canchoose(x-2, y, z);
                                    }

                                    //(x, y, z), (x+1, y, z)がA, (x+2, y, z)に配置可能
                                    if(can_put_ship(x+2, y, z, ship[index]) && is_aship(x+1, y, z, ship[index])){
                                        set_canchoose(x+2, y, z);
                                    }

                                    //(x, y, z), (x, y−1, z)がA, (x, y-2, z)に配置可能
                                    if(can_put_ship(x, y-2, z, ship[index]) && is_aship(x, y-1, z, ship[index])){
                                        set_canchoose(x, y-2, z);
                                    }

                                    //(x, y, z), (x, y+1, z)がA, (x, y+2, z)に配置可能
                                    if(can_put_ship(x, y+2, z, ship[index]) && is_aship(x, y+1, z, ship[index])){
                                        set_canchoose(x, y+2, z);
                                    }

                                    //(x, y, z), (x, y, z-1)がA, (x, y, z-2)に配置可能
                                    if(can_put_ship(x, y, z-2, ship[index]) && is_aship(x, y, z-1, ship[index])){
                                        set_canchoose(x, y, z-2);
                                    }

                                    //(x, y, z), (x, y, z+1)がA, (x, y, z+2)に配置可能
                                    if(can_put_ship(x, y, z+2, ship[index]) && is_aship(x, y, z+1, ship[index])){
                                        set_canchoose(x, y, z+2);
                                    }
                                }
                            }

                            //同種のAが1つの場合
                            else if(count_ship(ship[index]) == 1){

                                //(x, y, z)がA
                                if(is_aship(x, y, z, ship[index])){

                                    //(x-1, y, z), (x-2, y, z)に配置可能
                                    if(can_put_ship(x-1, y, z, ship[index]) && can_put_ship(x-2, y, z, ship[index])){
                                        set_canchoose(x-1, y, z);
                                        set_canchoose(x-2, y, z);
                                    }

                                    //(x+1, y, z), (x, y+2, z)に配置可能
                                    if(can_put_ship(x+1, y, z, ship[index]) && can_put_ship(x+2, y, z, ship[index])){
                                        set_canchoose(x+1, y, z);
                                        set_canchoose(x+2, y, z);
                                    }

                                    //(x, y-1, z), (x, y-2, z)に配置可能
                                    if(can_put_ship(x, y-1, z, ship[index]) && can_put_ship(x, y-2, z, ship[index])){
                                        set_canchoose(x, y-1, z);
                                        set_canchoose(x, y-2, z);
                                    }

                                    //(x, y+1, z), (x, y+2, z)に配置可能
                                    if(can_put_ship(x, y+1, z, ship[index]) && can_put_ship(x, y+2, z, ship[index])){
                                        set_canchoose(x, y+1, z);
                                        set_canchoose(x, y+2, z);
                                    }

                                    //(x, y, z-1), (x, y, z-2)に配置可能
                                    if(can_put_ship(x, y, z-1, ship[index]) && can_put_ship(x, y, z-2, ship[index])){
                                        set_canchoose(x, y, z-1);
                                        set_canchoose(x, y, z-2);
                                    }

                                    //(x, y, z+1), (x, y, z+2)に配置可能
                                    if(can_put_ship(x, y, z+1, ship[index]) && can_put_ship(x, y, z+2, ship[index])){
                                        set_canchoose(x, y, z+1);
                                        set_canchoose(x, y, z+2);
                                    }

                                    //(x-1, y, z), (x+1, y, z)に配置可能
                                    if(can_put_ship(x-1, y, z, ship[index]) && can_put_ship(x+1, y, z, ship[index])){
                                        set_canchoose(x-1, y, z);
                                        set_canchoose(x+1, y, z);
                                    }

                                    //(x, y-1, z), (x, y+1, z)に配置可能
                                    if(can_put_ship(x, y-1, z, ship[index]) && can_put_ship(x, y+1, z, ship[index])){
                                        set_canchoose(x, y-1, z);
                                        set_canchoose(x, y+1, z);
                                    }

                                    //(x, y, z-1), (x, y, z+1)に配置可能
                                    if(can_put_ship(x, y, z-1, ship[index]) && can_put_ship(x, y, z+1, ship[index])){
                                        set_canchoose(x, y, z-1);
                                        set_canchoose(x, y, z+1);
                                    }
                                }
                            }

                            //同種のAがない場合
                            else if(count_ship(ship[index]) == 0){
                                if(print_board[x][y][z] == NOSHIP || print_board[x][y][z] == CANCHOOSE){
                                    if(can_put_ship(x, y, z, CANCHOOSE)){

                                        //(x, y, z)を仮にASHIPとする
                                        set_canchoose_temp(x, y, z);

                                        //(x-1, y, z), (x-2, y, z)に配置可能
                                        if(can_put_ship(x-1, y, z, CANCHOOSE_TEMP) && can_put_ship(x-2, y, z, CANCHOOSE_TEMP)){
                                            set_canchoose(x, y, z);     //(x, y, z)をCANCHOOSEに
                                            set_canchoose(x-1, y, z);   //(x-1, y, z)をCANCHOOSEに
                                            set_canchoose(x-2, y, z);   //(x-2, y, z)をCANCHOOSEに
                                        }

                                        //(x＋1, y, z), (x＋2, y, z)に配置可能
                                        if(can_put_ship(x+1, y, z, CANCHOOSE_TEMP) && can_put_ship(x+2, y, z, CANCHOOSE_TEMP)){
                                            set_canchoose(x, y, z);     //(x, y, z)をCANCHOOSEに
                                            set_canchoose(x+1, y, z);   //(x+1, y, z)をCANCHOOSEに
                                            set_canchoose(x+2, y, z);   //(x+2, y, z)をCANCHOOSEに
                                        }

                                        //(x, y-1, z), (x, y-2, z)に配置可能
                                        if(can_put_ship(x, y-1, z, CANCHOOSE_TEMP) && can_put_ship(x, y-2, z, CANCHOOSE_TEMP)){
                                            set_canchoose(x, y, z);     //(x, y, z)をCANCHOOSEに
                                            set_canchoose(x, y-1, z);   //(x, y-1, z)をCANCHOOSEに
                                            set_canchoose(x, y-2, z);   //(x, y-2, z)をCANCHOOSEに
                                        }

                                        //(x, y+1, z), (x, y+2, z)に配置可能
                                        if(can_put_ship(x, y+1, z, CANCHOOSE_TEMP) && can_put_ship(x, y+2, z, CANCHOOSE_TEMP)){
                                            set_canchoose(x, y, z);     //(x, y, z)をCANCHOOSEに
                                            set_canchoose(x, y+1, z);   //(x, y+1, z)をCANCHOOSEに
                                            set_canchoose(x, y+2, z);   //(x, y+2, z)をCANCHOOSEに
                                        }

                                        //(x, y, z-1), (x, y, z-2)に配置可能
                                        if(can_put_ship(x, y, z-1, CANCHOOSE_TEMP) && can_put_ship(x, y, z-2, CANCHOOSE_TEMP)){
                                            set_canchoose(x, y, z);     //(x, y, z)をCANCHOOSEに
                                            set_canchoose(x, y, z-1);   //(x, y, z-1)をCANCHOOSEに
                                            set_canchoose(x, y, z-2);   //(x, y, z-2)をCANCHOOSEに
                                        }

                                        //(x, y, z+1), (x, y, z+2)に配置可能
                                        if(can_put_ship(x, y, z+1, CANCHOOSE_TEMP) && can_put_ship(x, y, z+2, CANCHOOSE_TEMP)){
                                            set_canchoose(x, y, z);     //(x, y, z)をCANCHOOSEに
                                            set_canchoose(x, y, z+1);   //(x, y, z+1)をCANCHOOSEに
                                            set_canchoose(x, y, z+2);   //(x, y, z+2)をCANCHOOSEに
                                        }

                                        //(x-1, y, z), (x+1, y, z)に配置可能
                                        if(can_put_ship(x-1, y, z, CANCHOOSE_TEMP) && can_put_ship(x+1, y, z, CANCHOOSE_TEMP)){
                                            set_canchoose(x, y, z);     //(x, y, z)をCANCHOOSEに
                                            set_canchoose(x-1, y, z);   //(x-1, y, z)をCANCHOOSEに
                                            set_canchoose(x+1, y, z);   //(x+1, y, z)をCANCHOOSEに
                                        }

                                        //(x, y-1, z), (x, y+1, z)に配置可能
                                        if(can_put_ship(x, y-1, z, CANCHOOSE_TEMP) && can_put_ship(x, y+1, z, CANCHOOSE_TEMP)){
                                            set_canchoose(x, y, z);     //(x, y, z)をCANCHOOSEに
                                            set_canchoose(x, y-1, z);   //(x, y-1, z)をCANCHOOSEに
                                            set_canchoose(x, y+1, z);   //(x, y+1, z)をCANCHOOSEに
                                        }

                                        //(x, y, z-1), (x, y, z+1)に配置可能
                                        if(can_put_ship(x, y, z-1, CANCHOOSE_TEMP) && can_put_ship(x, y, z+1, CANCHOOSE_TEMP)){
                                            set_canchoose(x, y, z);     //(x, y, z)をCANCHOOSEに
                                            set_canchoose(x, y, z-1);   //(x, y, z-1)をCANCHOOSEに
                                            set_canchoose(x, y, z+1);   //(x, y, z+1)をCANCHOOSEに
                                        }
                                    }
                                }
                            }
                        }
                    }

                    //BSHIP
                    if(index >= 2 && index <= 4){
                        if(now_index == index){

                            //同種のBが1つある場合
                            if(count_ship(ship[index]) == 1){

                                //(x, y, z)がA
                                if(print_board[x][y][z] == ship[index]){

                                    //(x-1, y, z)に配置可能
                                    if(can_put_ship(x-1, y, z, ship[index])){
                                        set_canchoose(x-1, y, z);
                                    }

                                    //(x＋1, y, z)に配置可能
                                    if(can_put_ship(x+1, y, z, ship[index])){
                                        set_canchoose(x+1, y, z);
                                    }

                                    //(x, y-1, z)に配置可能
                                    if(can_put_ship(x, y-1, z, ship[index])){
                                        set_canchoose(x, y-1, z);
                                    }

                                    //(x, y+1, z)に配置可能
                                    if(can_put_ship(x, y+1, z, ship[index])){
                                        set_canchoose(x, y+1, z);
                                    }

                                    //(x, y, z-1)に配置可能
                                    if(can_put_ship(x, y, z-1, ship[index])){
                                        set_canchoose(x, y, z-1);
                                    }

                                    //(x, y, z+1)に配置可能
                                    if(can_put_ship(x, y, z+1, ship[index])){
                                        set_canchoose(x, y, z+1);
                                    }
                                }
                            }

                            //同種のBがない場合
                            else if(count_ship(ship[index]) == 0){
                                if(print_board[x][y][z] == NOSHIP || print_board[x][y][z] == CANCHOOSE){
                                    if(can_put_ship(x, y, z, CANCHOOSE)){

                                        //(x, y, z)を仮にBSHIPとする
                                        set_canchoose_temp(x, y, z);

                                        //(x-1, y, z), (x+1, y, z), (x, y-1, z), (x, y+1, z), (x, y, z-1), (x, y, z+1)のいずれかに配置可能
                                        if(can_put_ship(x-1, y, z, CANCHOOSE_TEMP) || can_put_ship(x+1, y, z, CANCHOOSE_TEMP) || can_put_ship(x, y-1, z, CANCHOOSE_TEMP) || can_put_ship(x, y+1, z, CANCHOOSE_TEMP) || can_put_ship(x, y, z-1, CANCHOOSE_TEMP) || can_put_ship(x, y, z+1, CANCHOOSE_TEMP)){
                                           set_canchoose(x, y, z);  //(x, y, z)をCANCHOOSEに
                                        }
                                    }
                                }
                            }
                        }
                    }

                    //CSHIP
                    if(index >= 5){
                        if(now_index == index && count_ship(ship[index]) == 0){
                            for(int x = 0; x < BD_SIZE; x++){
                                for(int y = 0; y < BD_SIZE; y++){
                                    for(int z = 0; z < BD_SIZE; z++){

                                        //(x, y, z)に配置可能
                                        if(print_board[x][y][z] == NOSHIP && can_put_ship(x, y, z, NOSHIP)){
                                            set_canchoose(x, y, z); //(x, y, z)をCANCHOOSEに
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //CANCHOOSE_TEMPのままのマスを元に戻す
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(print_board[x][y][z] == CANCHOOSE_TEMP){
                    print_board[x][y][z] = NOSHIP;
                }
            }
        }
    }
}
