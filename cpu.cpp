#include "cpu.h"

#include <QMouseEvent>

cpu::cpu(QWidget *parent) : player(parent)
{

}


cpu::~cpu()
{

}


void cpu::mousePressEvent(QMouseEvent *event)
{
    event->x();
    event->y();
}


//描画用関数
void cpu::paintGL()
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
                    if(print_board[x][y][z] == NOSHIP){
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
