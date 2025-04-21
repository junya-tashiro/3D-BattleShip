#ifndef MAINWINDOW_2_H
#define MAINWINDOW_2_H

#include "define.h"
#include "bsl_interpriter.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow_3; }
QT_END_NAMESPACE

class MainWindow_2 : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow_2(QWidget *parent = nullptr);  //コンストラクタ

    ~MainWindow_2();                          //デコンストラクタ

    int winner_flag;                        //結果表示用

    void set();

    void set_ship(int x, int y, int z, enum ship S1, enum ship S2);

    bsl_interpriter *interpriter;

    QString fileName;

private slots:
    void deleteSlot();                              //windowのclose時に自動でdeleteも行う

    void on_reset_attack_from_player1_released();   //player_1リセットボタン

    void on_reset_attack_from_player2_released();   //player_2リセットボタン

    void on_attack_from_player1_released();         //player_1アタックボタン

private:
    Ui::MainWindow_3 *ui;
} ;


#endif // MAINWINDOW_2_H
