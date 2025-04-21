#include "bsl_interpriter.h"

#include <QFile>
#include <QTextStream>
#include <stdlib.h>


//コンストラクタ
bsl_interpriter::bsl_interpriter()
{

    //敵船記録用配列の初期化
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){

                //端はROCKに
                if(x % (BD_SIZE - 1) == 0 && y % (BD_SIZE - 1) == 0 && z % (BD_SIZE - 1) == 0 ){
                    enemy_board[x][y][z] = ROCK;
                }

                //それ以外
                else{
                    enemy_board[x][y][z] = UNKNOWN;
                }
            }
        }
    }

    //変数を入れておくための箱の初期化
    for(int n = 0; n < 30; n++){

        //定数0~3は変更不可の変数として扱う(名称が0~3の箱に数値0~3を格納しておく)
        if(n == 0 || n == 1 || n == 2 || n == 3){
            variable_box[n] = n;
        }

        //それ以外(a~z)は初期化
        else{
            variable_box[n] = NO_POINT;
        }
    }

    //リストを格納しておくための箱の初期化
    for(int m = 0; m < 26; m++){
        for (int l = 0; l < 64; l++){

            //リスト内の初期化
            list_box[m][l] = NO_POINT;
        }
    }

    //def{}実行判定フラグの初期化
    init_flag = 0;
}


//デコンストラクタ
bsl_interpriter::~bsl_interpriter()
{
    //変数の初期化
    for(int n = 0; n < 30; n++){
        if(n == 0 || n == 1 || n == 2 || n == 3){
            variable_box[n] = n;
        }
        else{
            variable_box[n] = NO_POINT;
        }
    }

    //リストの初期化
    for(int m = 0; m < 26; m++){
        for (int l = 0; l < 64; l++){
            list_box[m][l] = NO_POINT;
        }
    }
    for(int l = 0; l < 64; l++){
        list_box[0][l] = 0;
    }

    //def{}実行判定フラグの初期化
    init_flag = 0;
}


//敵船の攻撃結果をenemy_boardに格納しておくための関数
void bsl_interpriter::set_enemy_board(int x, int y, int z, enum ship S)
{
    enemy_board[x][y][z] = S;
}


//変数解読関数([]内を読み取って整数で返す), 2文字の加減法のみ定義
int bsl_interpriter::read_var(int n, int m) //nは行数, mは[の列数
{
//ここから下コメント未記入==========================================================
    if(lines[n][m+2] == "]"){
        for(int t0 = 0; t0 < 30; t0++){
            if(lines[n][m+1] == variable_name[t0]){
                return variable_box[t0];
            }
        }
    }
    else{
        int a, b;
        for(int t0 = 0; t0 < 30; t0++){
            if(lines[n][m+1] == variable_name[t0]){
                a =  variable_box[t0];
            }
            if(lines[n][m+3] == variable_name[t0]){
                b =  variable_box[t0];
            }
        }
        if(lines[n][m+2] == "+"){
            return a + b;
        }
        else if(lines[n][m+2] == "-"){
            return a - b;
        }
    }
    return NO_POINT;
}


int bsl_interpriter::read_a_line(int n, int m){
    for(int k = n; k < 10000; k++){

        //終了
        if(lines[k][m-4] == "}"){
            return NO_POINT;
        }

        //変数宣言 var●=[●+1]のような形で, 1文字のみ, 代入元は0~3とa~z
        else if(lines[k][m] == "v" && lines[k][m+1] == "a" && lines[k][m+2] == "r" && lines[k][m+5] == "="){
            int alted_num = NO_POINT;
            for(int t = 0; t < 30; t++){
                if(lines[k][m+4] == variable_name[t]){
                    alted_num = t;
                }
            }
            variable_box[alted_num] = read_var(k, m+6);
            text.append(">> ").append(QString::number(variable_box[alted_num])).append(" is subsutituted to ").append(variable_name[alted_num]).append("\n");
        }

        //リスト宣言 list●のような形で, 1文字のみ
        else if(lines[k][m] == "l" && lines[k][m+1] == "i" && lines[k][m+2] == "s" && lines[k][m+3] == "t"){
            int declared_num = NO_POINT;
            for(int t = 0; t < 26; t++){
                if(lines[k][m+5] == list_name[t]){
                    declared_num = t;
                    break;
                }
            }
            if(declared_num >= 0 && declared_num < 26) {
                for(int a = 0; a < 64; a ++){
                   list_box[declared_num][a] = 0;
                }
                text.append(">> list ").append(list_name[declared_num]).append(" is declared\n");
            }
        }

        //リストに追加
        else if(lines[k][m] == "a" && lines[k][m+1] == "d" && lines[k][m+2] == "d"){
            int num, x, y, z = NO_POINT;
            for(int t = 0; t < 26; t++){
                if(lines[k][m+4] == list_name[t]){
                    num = t;
                }
            }
            int flag = 0;
            for(int t = 0; t < 20; t++){
                if(lines[k][m+6+t] == "["){
                    if(flag == 0){
                        flag = 1;
                        x = read_var(k, m+6+t);
                    }
                    else if(flag == 1){
                        flag = 2;
                        y = read_var(k, m+6+t);
                    }
                    else if(flag == 2){
                        z = read_var(k, m+6+t);
                        break;
                    }
                }
            }
            if(x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE && z < BD_SIZE){
                if(list_box[num][BD_SIZE * BD_SIZE * x + BD_SIZE * y + z] == 0){
                    list_box[num][BD_SIZE * BD_SIZE * x + BD_SIZE * y + z] = 1;
                    text.append(">> [").append(QString::number(x)).append("][").append(QString::number(y)).append("][").append(QString::number(z)).append("] is added to list ").append(list_name[num]).append("\n");
                }
            }
        }

        //リストから削除
        else if(lines[k][m] == "r" && lines[k][m+1] == "e" && lines[k][m+2] == "m" && lines[k][m+3] == "o" && lines[k][m+4] == "v" && lines[k][m+5] == "e"){
            int num, x, y, z = NO_POINT;
            for(int tt = 0; tt < 26; tt++){
                if(lines[k][m+7] == list_name[tt]){
                    num = tt;
                }
            }
            int flag = 0;
            for(int t = 0; t < 20; t++){
                if(lines[k][m+9+t] == "["){
                    if(flag == 0){
                        flag = 1;
                        x = read_var(k, m+9+t);
                    }
                    else if(flag == 1){
                        flag = 2;
                        y = read_var(k, m+9+t);
                    }
                    else if(flag == 2){
                        z = read_var(k, m+9+t);
                        break;
                    }
                }
            }
            if(x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE && z < BD_SIZE){
                if(list_box[num][BD_SIZE * BD_SIZE * x + BD_SIZE * y + z] == 1){
                    list_box[num][BD_SIZE * BD_SIZE * x + BD_SIZE * y + z] = 0;
                    text.append(">> [").append(QString::number(x)).append("][").append(QString::number(y)).append("][").append(QString::number(z)).append("] is removed from list ").append(list_name[num]).append("\n");
                }
            }
        }

        //繰り返し
        else if(lines[k][m] == "f" && lines[k][m+1] == "o" && lines[k][m+2] == "r"){
            int num_x, num_y, num_z;
            for(int t = 0; t < 30; t++){
                if(lines[k][m+5] == variable_name[t]){
                    num_x = t;
                }
                if(lines[k][m+8] == variable_name[t]){
                    num_y = t;
                }
                if(lines[k][m+11] == variable_name[t]){
                    num_z = t;
                }
            }
            for(int x = 0; x < BD_SIZE; x++){
                for(int y = 0; y < BD_SIZE; y++){
                    for(int z = 0; z < BD_SIZE; z++){
                        variable_box[num_x] = x;
                        variable_box[num_y] = y;
                        variable_box[num_z] = z;

                        int a = read_a_line(k+1, m+4);
                        if(a != NO_POINT){
                            return a;
                        }
                    }
                }
            }
        }

        //条件文
        else if(lines[k][m] == "i" && lines[k][m+1] == "f"){
            if(lines[k][m+3] == "i" && lines[k][m+4] == "n"){
                int x, y, z = NO_POINT;
                int flag = 0;
                int num = NO_POINT;
                for(int t = 0; t < 30; t++){
                    if(lines[k][m+5+t] == "{"){
                        break;
                    }
                    else if(lines[k][m+5+t] == "["){
                        if(flag == 0){
                            flag = 1;
                            x = read_var(k, m+5+t);
                        }
                        else if(flag == 1){
                            flag = 2;
                            y = read_var(k, m+5+t);
                        }
                        else if(flag == 2){
                            z = read_var(k, m+5+t);
                        }
                    }
                    else if(lines[k][m+5+t] == ","){
                        if(lines[k][m+6+t] == "!"){
                            for(int t1 = 0; t1 < 26; t1++){
                                if(lines[k][m+7+t] == list_name[t1]){
                                    num = t1;
                                    break;
                                }
                            }
                            if(num >= 0 && num < 26 && x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE && z < BD_SIZE){
                                if(list_box[num][BD_SIZE * BD_SIZE * x + BD_SIZE * y + z] == 0){
                                    int a = read_a_line(k+1, m+4);
                                    if(a != NO_POINT){
                                        return a;
                                    }
                                }
                            }
                        }
                        else{
                            for(int t1 = 0; t1 < 26; t1++){
                                if(lines[k][m+6+t] == list_name[t1]){
                                    num = t1;
                                    break;
                                }
                            }
                            if(num >= 0 && num < 26 && x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE && z < BD_SIZE){
                                if(list_box[num][BD_SIZE * BD_SIZE * x + BD_SIZE * y + z] == 1){
                                    int a = read_a_line(k+1, m+4);
                                    if(a != NO_POINT){
                                        return a;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(lines[k][m+3] == "i" && lines[k][m+4] == "s"){
                int x, y, z = NO_POINT;
                int flag = 0;
                enum ship S = UNKNOWN;
                for(int t = 0; t < 30; t++){
                    if(lines[k][m+5+t] == "{"){
                        break;
                    }
                    else if(lines[k][m+5+t] == "["){
                        if(flag == 0){
                            flag = 1;
                            x = read_var(k, m+5+t);
                        }
                        else if(flag == 1){
                            flag = 2;
                            y = read_var(k, m+5+t);
                        }
                        else if(flag == 2){
                            flag = 3;
                            z = read_var(k, m+5+t);
                        }
                    }
                    else if(lines[k][m+5+t] == ","){
                        if(lines[k][m+6+t] == "!"){
                            if(lines[k][m+7+t] == "U"){
                                S = UNKNOWN;
                            }
                            else if(lines[k][m+7+t] == "N"){
                                S = NOSHIP;
                            }
                            else if(lines[k][m+7+t] == "R"){
                                S = ROCK;
                            }
                            else if(lines[k][m+7+t] == "A"){
                                S = ASHIP;
                            }
                            else if(lines[k][m+7+t] == "B"){
                                S = BSHIP;
                            }
                            else if(lines[k][m+7+t] == "C"){
                                S = CSHIP;
                            }
                            if(x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE && z < BD_SIZE){
                                if(enemy_board[x][y][z] != S){
                                    int a = read_a_line(k+1, m+4);
                                    if(a != NO_POINT){
                                        return a;
                                    }
                                }
                            }
                        }
                        else{
                            if(lines[k][m+6+t] == "U"){
                                S = UNKNOWN;
                            }
                            else if(lines[k][m+6+t] == "N"){
                                S = NOSHIP;
                            }
                            else if(lines[k][m+6+t] == "R"){
                                S = ROCK;
                            }
                            else if(lines[k][m+6+t] == "A"){
                                S = ASHIP;
                            }
                            else if(lines[k][m+6+t] == "B"){
                                S = BSHIP;
                            }
                            else if(lines[k][m+6+t] == "C"){
                                S = CSHIP;
                            }
                            if(x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE && z < BD_SIZE){
                                if(enemy_board[x][y][z] == S){
                                    int a = read_a_line(k+1, m+4);
                                    if(a != NO_POINT){
                                        return a;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        //攻撃
        else if(lines[k][m] == "a" && lines[k][m+1] == "t" && lines[k][m+2] == "t" && lines[k][m+3] == "a" && lines[k][m+4] == "c" && lines[k][m+5] == "k"){
            int x, y, z;
            int flag = 0;
            for(int t = 0; t < 20; t++){
                if(lines[k][m+7+t] == "["){
                    if(flag == 0){
                        flag = 1;
                        x = read_var(k, m+7+t);
                    }
                    else if(flag == 1){
                        flag = 2;
                        y = read_var(k, m+7+t);
                    }
                    else if(flag == 2){
                        z = read_var(k, m+7+t);
                        break;
                    }
                }
            }
            if(x >= 0 && y >= 0 && z >= 0 && x < BD_SIZE && y < BD_SIZE && z < BD_SIZE){
                text.append(">> player attacks to [").append(QString::number(x)).append("][").append(QString::number(y)).append("][").append(QString::number(z)).append("]");
                return BD_SIZE * BD_SIZE * x + BD_SIZE * y + z;
            }
        }
    }

    return NO_POINT;
}


int bsl_interpriter::interpriter(QString fileName)
{
    QFile file(fileName);

    if (! file.open(QIODevice::ReadOnly)) {
        return NO_POINT;
    }

    QTextStream in(&file);
    QString inStr = in.readAll();
    file.close();

    lines = inStr.split("\n");

    text = "";

    for(int n = 0; n < 10000; n++){
        if(init_flag == 0 && lines[n][0] == "d" && lines[n][1] == "e" && lines[n][2] == "f" && lines[n][3] == "{"){
            init_flag = 1;
            read_a_line(n+1, 4);
        }
        if(lines[n][0] == "m" && lines[n][1] == "a" && lines[n][2] == "i" && lines[n][3] == "n" && lines[n][4] == "{"){
            return read_a_line(n+1,4);
        }
    }
    return NO_POINT;
}

void bsl_interpriter::ship_reader(QString fileName)
{
    QFile file(fileName);

    if (! file.open(QIODevice::ReadOnly)) {
        return;
    }

    QTextStream in(&file);
    QString inStr = in.readAll();
    file.close();

    lines = inStr.split("\n");

    for(int n = 0; n < 10000; n++){
        if(lines[n][0] == "s" && lines[n][1] == "h" && lines[n][2] == "i" && lines[n][3] == "p" && lines[n][4] == "{"){
            for(int x = 0; x < BD_SIZE; x++){
                for(int y = 0; y < BD_SIZE; y++){
                    for(int z = 0; z < BD_SIZE; z++){
                        if(lines[n+z+1][x+5*y+4] == "a"){
                            ship[x][y][z] = 0;
                        }
                        else if(lines[n+z+1][x+5*y+4] == "b"){
                            ship[x][y][z] = 1;
                        }
                        else if(lines[n+z+1][x+5*y+4] == "c" || lines[n+z+1][x+5*y+4] == "d"){
                            ship[x][y][z] = 2;
                        }
                        else if(lines[n+z+1][x+5*y+4] == "e" || lines[n+z+1][x+5*y+4] == "f" || lines[n+z+1][x+5*y+4] == "g"){
                            ship[x][y][z] = 3;
                        }
                        else if(lines[n+z+1][x+5*y+4] == "h" || lines[n+z+1][x+5*y+4] == "i" || lines[n+z+1][x+5*y+4] == "j" || lines[n+z+1][x+5*y+4] == "k"){
                            ship[x][y][z] = 4;
                        }
                    }
                }
            }
        }
        return;
    }
}
