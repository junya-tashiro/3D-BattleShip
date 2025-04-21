#ifndef EDITOR_H
#define EDITOR_H

#include "define.h"
#include "bsl_interpriter.h"

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class editor;
}

class editor : public QWidget
{
    Q_OBJECT

public:
    explicit editor(int a, QWidget *parent = nullptr);
    ~editor();

    QString fileName;
    int flag;

protected slots:
    void on_pushButton_released();

    void on_pushButton_2_released();

    void widget_set(void);

    void on_comboBox_56_currentIndexChanged(int index);

    void on_pushButton_3_released();

    void on_pushButton_6_released();

    void on_pushButton_8_released();

    void on_pushButton_10_released();

    void on_pushButton_12_released();

    void on_pushButton_14_released();

    void on_pushButton_16_released();

    void on_pushButton_18_released();

    void on_pushButton_4_released();

    void on_pushButton_5_released();

    void on_pushButton_7_released();

    void on_pushButton_9_released();

    void on_pushButton_11_released();

    void on_pushButton_13_released();

    void on_pushButton_15_released();

    void on_pushButton_17_released();

    void on_pushButton_21_released();

    void on_comboBox_4_currentIndexChanged(int index);

    void on_pushButton_19_released();

    void on_pushButton_20_released();

    void on_pushButton_22_released();

    void on_pushButton_23_released();

    void on_pushButton_24_released();

    void on_pushButton_26_released();

    void on_comboBox_5_currentIndexChanged(int index);

protected:
    Ui::editor *ui;

    void rebuild(int x);

    void set_newcommand();

    void read(int n, int m, QTreeWidgetItem *process);

    int row, column;

    QString inStr;
    QList<QString>lines;

    int number;
    QTreeWidgetItem *item[1000];
    int line_number[1000];
    int depth[1000];

    int ship[BD_SIZE][BD_SIZE][BD_SIZE];

    bsl_interpriter *interpriter_for_debug;

    enum ship enemy1[3][BD_SIZE][BD_SIZE][BD_SIZE] = {
        {
            {{  ROCK,  ASHIP, NOSHIP,   ROCK}, {NOSHIP,  ASHIP, NOSHIP,  BSHIP}, {NOSHIP,  ASHIP, NOSHIP,  BSHIP}, {  ROCK, NOSHIP, NOSHIP,   ROCK}},
            {{ BSHIP, NOSHIP, NOSHIP,  CSHIP}, { BSHIP, NOSHIP,  ASHIP, NOSHIP}, {NOSHIP, NOSHIP,  ASHIP, NOSHIP}, {NOSHIP, NOSHIP,  ASHIP, NOSHIP}},
            {{NOSHIP,  BSHIP,  BSHIP, NOSHIP}, {NOSHIP, NOSHIP, NOSHIP,  CSHIP}, {NOSHIP,  CSHIP, NOSHIP, NOSHIP}, {NOSHIP, NOSHIP, NOSHIP,  CSHIP}},
            {{  ROCK, NOSHIP, NOSHIP,   ROCK}, {NOSHIP, NOSHIP, NOSHIP, NOSHIP}, {NOSHIP, NOSHIP, NOSHIP, NOSHIP}, {  ROCK, NOSHIP, NOSHIP,   ROCK}}
        },{
            {{  ROCK, NOSHIP, NOSHIP,   ROCK}, {NOSHIP, NOSHIP,  ASHIP, NOSHIP}, {NOSHIP, NOSHIP, NOSHIP, NOSHIP}, {  ROCK,  CSHIP, NOSHIP,   ROCK}},
            {{ ASHIP, NOSHIP, NOSHIP,  CSHIP}, { ASHIP, NOSHIP,  ASHIP, NOSHIP}, { ASHIP, NOSHIP, NOSHIP,  BSHIP}, {NOSHIP, NOSHIP, NOSHIP,  BSHIP}},
            {{NOSHIP, NOSHIP, NOSHIP, NOSHIP}, {NOSHIP, NOSHIP,  ASHIP, NOSHIP}, {NOSHIP,  BSHIP, NOSHIP, NOSHIP}, {NOSHIP, NOSHIP,  CSHIP, NOSHIP}},
            {{  ROCK,  BSHIP,  BSHIP,   ROCK}, { CSHIP, NOSHIP, NOSHIP, NOSHIP}, {NOSHIP,  BSHIP, NOSHIP, NOSHIP}, {  ROCK, NOSHIP, NOSHIP,   ROCK}}
        },{
            {{  ROCK, NOSHIP, NOSHIP,   ROCK}, { ASHIP,  ASHIP,  ASHIP, NOSHIP}, {NOSHIP, NOSHIP, NOSHIP, NOSHIP}, {  ROCK,  CSHIP, NOSHIP,   ROCK}},
            {{ BSHIP, NOSHIP,  CSHIP, NOSHIP}, {NOSHIP, NOSHIP, NOSHIP, NOSHIP}, {NOSHIP, NOSHIP, NOSHIP, NOSHIP}, {NOSHIP, NOSHIP, NOSHIP, NOSHIP}},
            {{ BSHIP, NOSHIP, NOSHIP,  ASHIP}, {NOSHIP, NOSHIP, NOSHIP,  ASHIP}, { CSHIP, NOSHIP, NOSHIP,  ASHIP}, {NOSHIP, NOSHIP, NOSHIP, NOSHIP}},
            {{  ROCK, NOSHIP,  CSHIP,   ROCK}, { BSHIP,  BSHIP, NOSHIP, NOSHIP}, {NOSHIP, NOSHIP,  BSHIP, NOSHIP}, {  ROCK, NOSHIP,  BSHIP,   ROCK}}
        }
    };

    enum ship enemy_state[BD_SIZE][BD_SIZE][BD_SIZE];

    void reset_debug();

    void timerEvent(QTimerEvent *);

    int attack_counter;
    int now_counter;
    int run_flag;

    int count_unattacked();
};

#endif // EDITOR_H
