#ifndef BSL_INTERPRITER_H
#define BSL_INTERPRITER_H

#include "define.h"

#include <QLabel>

class bsl_interpriter
{
public:
    explicit bsl_interpriter();
    virtual ~bsl_interpriter();

    int interpriter(QString fileName);

    void ship_reader(QString fileName);

    int ship[BD_SIZE][BD_SIZE][BD_SIZE];

    enum ship enemy_board[BD_SIZE][BD_SIZE][BD_SIZE];

    void set_enemy_board(int x, int y, int z, enum ship S);

    QString text;

private:
    int variable_box[30];

    int list_box[26][64];

    const char variable_name[30] = {
        '0', '1', '2', '3', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'n', 'm', 'o', 'p', 'q', 'r', 's' , 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    const char list_name[26] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'n', 'm', 'o', 'p', 'q', 'r', 's' , 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    int read_var(int n, int m);

    int read_a_line(int n, int m);

    int init_flag;

    QList<QString>lines;
};


#endif // BSL_INTERPRITER_H
