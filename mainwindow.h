#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "define.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);  //コンストラクタ

    ~MainWindow();                          //デコンストラクタ

    int winner_flag;                        //結果表示用

    void set();

    void set_ship(int x, int y, int z, enum ship S1, enum ship S2);

private slots:
    void deleteSlot();                              //windowのclose時に自動でdeleteも行う

    void on_reset_attack_from_player1_released();   //player_1リセットボタン

    void on_reset_attack_from_player2_released();   //player_2リセットボタン

    void on_attack_from_player1_released();         //player_1アタックボタン

    void on_attack_from_player2_released();         //player_2アタックボタン

private:
    Ui::MainWindow *ui;
} ;


#endif // MAINWINDOW_H
