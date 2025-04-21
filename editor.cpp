#include "editor.h"
#include "ui_editor.h"

#include <QFile>
#include <QTextStream>
#include <QTextBrowser>
#include <QFont>
#include <QFileDialog>
#include <QDebug>

editor::editor(int a, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editor)
{
    ui->setupUi(this);

    if(a == 0){
        fileName = QFileDialog::getSaveFileName(this, tr(""), ".", tr("text(*.bshp)"));

        QString text = "";
        text.append("ship{\n");
        text.append("    abha bcbb bbgb abba\n");
        text.append("    kbbb bcbb bbgb bjbb\n");
        text.append("    bbeb bcbd ibbd bbbd\n");
        text.append("    abea bbbb bbbb affa\n");
        text.append("}\n");
        text.append("def{\n");
        text.append("}\n");
        text.append("main{\n");
        text.append("}\n");
        text.append("#END\n");

        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << text;
        file.close();
    }
    else{
        fileName = QFileDialog::getOpenFileName(this, tr(""), ".", tr("text(*.bshp)"));
    }

    flag = 1;
    if(fileName == NULL){
        flag = 0;
        return;
    }

    QString title = fileName;
    title.append(" - BSL Editor");

    this->setWindowTitle(title);

    QFile file(fileName);

    if (! file.open(QIODevice::ReadOnly)){
        return;
    }

    QTextStream in(&file);
    inStr = in.readAll();
    file.close();

    ui->textBrowser->setFont(QFont("Courier", 11));
    ui->textBrowser_2->setFont(QFont("Courier", 11));
    ui->label_4->setFont(QFont("Courier", 11));
    ui->label_5->setFont(QFont("Courier", 11));
    ui->label_6->setFont(QFont("Courier", 11));
    ui->label_12->setFont(QFont("Courier", 11));
    ui->label_13->setFont(QFont("Courier", 11));

    ui->treeWidget->setColumnCount(1);
    QStringList list;
    list << "process tree";
    ui->treeWidget->setHeaderLabels(list);
    ui->treeWidget->header()->resizeSection(0, 400);

    ui->stackedWidget->setCurrentIndex(0);

    row = NO_POINT;
    column = NO_POINT;

    rebuild(0);

    connect(this->ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(widget_set()));

    //配置初期化
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(lines[z+1][x+5*y+4] == "a"){
                    ship[x][y][z] = player_set_ship::ROCK;
                    ui->player->print_board[x][y][z] = player_set_ship::ROCK;
                }
                else if(lines[z+1][x+5*y+4] == "b"){
                    ship[x][y][z] = player_set_ship::NOSHIP;
                    ui->player->print_board[x][y][z] = player_set_ship::NOSHIP;
                }
                else if(lines[z+1][x+5*y+4] == "c"){
                    ship[x][y][z] = player_set_ship::ASHIP1;
                    ui->player->print_board[x][y][z] = player_set_ship::ASHIP1;
                }
                else if(lines[z+1][x+5*y+4] == "d"){
                    ship[x][y][z] = player_set_ship::ASHIP2;
                    ui->player->print_board[x][y][z] = player_set_ship::ASHIP2;
                }
                else if(lines[z+1][x+5*y+4] == "e"){
                    ship[x][y][z] = player_set_ship::BSHIP1;
                    ui->player->print_board[x][y][z] = player_set_ship::BSHIP1;
                }
                else if(lines[z+1][x+5*y+4] == "f"){
                    ship[x][y][z] = player_set_ship::BSHIP2;
                    ui->player->print_board[x][y][z] = player_set_ship::BSHIP2;
                }
                else if(lines[z+1][x+5*y+4] == "g"){
                    ship[x][y][z] = player_set_ship::BSHIP3;
                    ui->player->print_board[x][y][z] = player_set_ship::BSHIP3;
                }
                else if(lines[z+1][x+5*y+4] == "h"){
                    ship[x][y][z] = player_set_ship::CSHIP1;
                    ui->player->print_board[x][y][z] = player_set_ship::CSHIP1;
                }
                else if(lines[z+1][x+5*y+4] == "i"){
                    ship[x][y][z] = player_set_ship::CSHIP2;
                    ui->player->print_board[x][y][z] = player_set_ship::CSHIP2;
                }
                else if(lines[z+1][x+5*y+4] == "j"){
                    ship[x][y][z] = player_set_ship::CSHIP3;
                    ui->player->print_board[x][y][z] = player_set_ship::CSHIP3;
                }
                else if(lines[z+1][x+5*y+4] == "k"){
                    ship[x][y][z] = player_set_ship::CSHIP4;
                    ui->player->print_board[x][y][z] = player_set_ship::CSHIP4;
                }
            }
        }
    }

    //コンボボックスの中身
    ui->comboBox_4->addItem("shipA (1)");
    ui->comboBox_4->addItem("shipA (2)");
    ui->comboBox_4->addItem("shipB (1)");
    ui->comboBox_4->addItem("shipB (2)");
    ui->comboBox_4->addItem("shipB (3)");
    ui->comboBox_4->addItem("shipC (1)");
    ui->comboBox_4->addItem("shipC (2)");
    ui->comboBox_4->addItem("shipC (3)");
    ui->comboBox_4->addItem("shipC (4)");

    startTimer(50);

    QString init = "## BSL Editor for: ";
    init.append(fileName);
    ui->textBrowser_2->append(init);

    ui->textBrowser_2->append("|");
    ui->textBrowser_2->append("+-- change command");
    ui->textBrowser_2->append("|         => process tree");
    ui->textBrowser_2->append("|         => command property");
    ui->textBrowser_2->append("+-- change ships");
    ui->textBrowser_2->append("|         => ship placement");
    ui->textBrowser_2->append("+-- check code and save");
    ui->textBrowser_2->append("|         => code");
    ui->textBrowser_2->append("+-- test your command");
    ui->textBrowser_2->append("          => command test");

    reset_debug();
}

editor::~editor()
{
    delete ui;
}


void editor::rebuild(int x)
{
    if(x == 1){
        inStr = "";
        for(int n = 0; n < 10000; n++){
            if(lines[n] == "#END"){
                inStr.append(lines[n]);
                break;
            }
            else if(lines[n] != "#BLANK"){
                inStr.append(lines[n]);
                inStr.append("\n");
            }
        }
    }

    set_newcommand();

    ui->textBrowser->setText(inStr);

    ui->treeWidget->clear();

    number = 0;

    for(int n = 0; n < 1000; n++){
        line_number[n] = 0;
        depth[n] = 0;
    }

    for(int l = 0; l < 10000; l++){
        if(lines[l][0] == "#" && lines[l][1] == "E" && lines[l][2] == "N"){
            break;
        }
        if(lines[l][0] == "d" && lines[l][1] == "e" && lines[l][2] == "f"){
            item[number] = new QTreeWidgetItem;
            item[number]->setText(0, "define");
            ui->treeWidget->addTopLevelItem(item[number]);
            line_number[number] = l;
            depth[number] = 0;
            number++;

            read(l+1, 4, item[number-1]);
        }
        if(lines[l][0] == "m" && lines[l][1] == "a" && lines[l][2] == "i"){
            item[number] = new QTreeWidgetItem;
            item[number]->setText(0, "main");
            ui->treeWidget->addTopLevelItem(item[number]);
            number++;
            line_number[number] = l;
            depth[number] = 0;

            read(l+1, 4, item[number-1]);
        }
    }

    ui->treeWidget->expandAll();
    row = NO_POINT;
    column = NO_POINT;
}

void editor::set_newcommand(void)
{
    lines = inStr.split("\n");
    inStr = "";

    for(int k = 0; k < 10000; k++){
        if(lines[k][0] == "#" && lines[k][1] == "E" && lines[k][2] == "N"){
            for(int n = 0; n <= k; n++){
                inStr.append(lines[n]);
                inStr.append("\n");
            }
            lines = inStr.split("\n");
            return;
        }
        else{
            QString text = lines[k];
            if(text.trimmed()[0] == "}"){
                QString text2 = lines[k-1];
                if(text2.trimmed().midRef(0, 2) == "de" || text2.trimmed().midRef(0, 2) == "ma" || text2.trimmed().midRef(0, 2) == "if" || text2.trimmed().midRef(0, 2) == "fo"){
                    lines[k-1].append("\n");
                    for(int n = 0; n < 1000; n++){
                        if(lines[k][n] == " "){
                            lines[k-1].append(" ");
                        }
                        else{
                            lines[k-1].append("    #NEW COMMAND");
                            break;
                        }
                    }
                }
            }
        }
    }
}

void editor::read(int n, int m, QTreeWidgetItem *process)
{
    for(int k = n; k < 10000; k++){
        if(lines[k][m-4] == "}"){
            return;
        }
        else if(lines[k][m] == "v" && lines[k][m+1] == "a" && lines[k][m+2] == "r"){
            item[number] = new QTreeWidgetItem;

            QString text = "new variable : ";
            text.append(lines[k][m+4]);
            text.append(" = ");
            int i = lines[k].indexOf("[");
            int j = lines[k].indexOf("]");
            text.append(lines[k].midRef(i+1, j-i-1));
            item[number]->setText(0, text);
            process->addChild(item[number]);
            line_number[number] = k;
            depth[number] = m;
            number++;
        }
        else if(lines[k][m] == "l" && lines[k][m+1] == "i" && lines[k][m+2] == "s"){
            item[number] = new QTreeWidgetItem;
            QString text = "new list : ";
            text.append(lines[k][m+5]);
            item[number]->setText(0, text);
            process->addChild(item[number]);
            line_number[number] = k;
            depth[number] = m;
            number++;
        }
        else if(lines[k][m] == "a" && lines[k][m+1] == "d" && lines[k][m+2] == "d"){
            item[number] = new QTreeWidgetItem;
            QString text = "add ";
            text.append(lines[k].midRef(m+6));
            text.append(" to list ");
            text.append(lines[k][m+4]);
            item[number]->setText(0, text);
            process->addChild(item[number]);
            line_number[number] = k;
            depth[number] = m;
            number++;
        }
        else if(lines[k][m] == "r" && lines[k][m+1] == "e" && lines[k][m+2] == "m"){
            item[number] = new QTreeWidgetItem;
            QString text = "remove ";
            text.append(lines[k].midRef(m+9));
            text.append(" from list ");
            text.append(lines[k][m+7]);
            item[number]->setText(0, text);
            process->addChild(item[number]);
            line_number[number] = k;
            depth[number] = m;
            number++;
        }
        else if(lines[k][m] == "f" && lines[k][m+1] == "o" && lines[k][m+2] == "r"){
            item[number] = new QTreeWidgetItem;
            QString text = "while moving ";
            text.append(lines[k].midRef(m+4, 9));
            text.append(" from [0][0][0] to [3][3][3]");
            item[number]->setText(0, text);
            process->addChild(item[number]);
            line_number[number] = k;
            depth[number] = m;
            number++;

            read(k+1, m+4, item[number-1]);
        }
        else if(lines[k][m] == "i" && lines[k][m+1] == "f"){
            item[number] = new QTreeWidgetItem;

            int i = lines[k].indexOf("(");
            int j = lines[k].indexOf(",");

            QString text = "if ";
            text.append(lines[k].midRef(i+1, j-i-1));
            text.append(" is ");

            if(lines[k][m+3] == "i" && lines[k][m+4] == "s"){
                if(lines[k][j+1] == "!"){
                    text.append("not ");
                    if(lines[k][j+2] == "U"){
                        text.append("UNKNOWN");
                    }
                    else if(lines[k][j+2] == "N"){
                        text.append("NOSHIP");
                    }
                    else if(lines[k][j+2] == "R"){
                        text.append("ROCK");
                    }
                    else if(lines[k][j+2] == "A"){
                        text.append("ASHIP");
                    }
                    else if(lines[k][j+2] == "B"){
                        text.append("BSHIP");
                    }
                    else if(lines[k][j+2] == "C"){
                        text.append("CSHIP");
                    }
                }
                else{
                    if(lines[k][j+1] == "U"){
                        text.append("UNKNOWN");
                    }
                    else if(lines[k][j+1] == "N"){
                        text.append("NOSHIP");
                    }
                    else if(lines[k][j+1] == "R"){
                        text.append("ROCK");
                    }
                    else if(lines[k][j+1] == "A"){
                        text.append("ASHIP");
                    }
                    else if(lines[k][j+1] == "B"){
                        text.append("BSHIP");
                    }
                    else if(lines[k][j+1] == "C"){
                        text.append("CSHIP");
                    }
                }
            }
            else if(lines[k][m+3] == "i" && lines[k][m+4] == "n"){
                if(lines[k][j+1] == "!"){
                    text.append("not included in list ");
                    text.append(lines[k][j+2]);
                }
                else{
                    text.append("included in list ");
                    text.append(lines[k][j+1]);
                }
            }
            item[number]->setText(0, text);
            process->addChild(item[number]);
            line_number[number] = k;
            depth[number] = m;
            number++;

            read(k+1, m+4, item[number-1]);
        }
        else if(lines[k][m] == "a" && lines[k][m+1] == "t" && lines[k][m+2] == "t"){
            item[number] = new QTreeWidgetItem;
            QString text = "attack to ";
            text.append(lines[k].midRef(m+7));
            item[number]->setText(0, text);
            process->addChild(item[number]);
            line_number[number] = k;
            depth[number] = m;
            number++;
        }

        else if(lines[k][m] == "#" && lines[k][m+1] == "N" && lines[k][m+2] == "E"){
            item[number] = new QTreeWidgetItem;
            QString text = "New Command";
            item[number]->setText(0, text);
            process->addChild(item[number]);
            line_number[number] = k;
            depth[number] = m;
            number++;
        }
    }
}


void editor::on_pushButton_released()
{
    if(column != 0 && column != NO_POINT){
        if(lines[row][column] == "i" || lines[row][column] == "f"){
            for(int n = 0; n < 1000; n++){
                if(lines[row+n][column] == "}"){
                    lines[row+n] = "#BLANK";
                    break;
                }
                else{
                    lines[row+n] = "#BLANK";
                }
            }
        }
        else{
            lines[row] = "#BLANK";
        }
        rebuild(1);
    }
}


void editor::on_pushButton_2_released()
{
    if(column != 0 && column != NO_POINT){
        int num = row;
        if(lines[row][column] == "i" || lines[row][column] == "f"){
            for(int n = row; n < 1000; n++){
                if(lines[n][column] == "}"){
                    num = n;
                    break;
                }
            }
        }
        lines[num].append("\n");
        for(int n = 0; n < column; n++){
            lines[num].append((" "));
        }
        lines[num].append("#NEW COMMAND");

        rebuild(1);
    }
}


void editor::widget_set(void)
{
    QTreeWidgetItem *chosen;
    chosen = ui->treeWidget->currentItem();
    row = NO_POINT;
    column = NO_POINT;
    for(int n = 0; n < 1000; n++){
        if(chosen == item[n]){
            row = line_number[n];
            column = depth[n];
            break;
        }
    }
    if(column != NO_POINT){
        if(lines[row][column] == "#" && lines[row][column+1] == "N"){
            ui->stackedWidget->setCurrentIndex(9);
        }
        else{
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
}

void editor::on_comboBox_56_currentIndexChanged(int index)
{
    if(index != 0){
        ui->stackedWidget->setCurrentIndex(index);
    }
    ui->comboBox_56->setCurrentIndex(0);
}


void editor::on_pushButton_3_released(){ui->stackedWidget->setCurrentIndex(9);}
void editor::on_pushButton_6_released(){ui->stackedWidget->setCurrentIndex(9);}
void editor::on_pushButton_8_released(){ui->stackedWidget->setCurrentIndex(9);}
void editor::on_pushButton_10_released(){ui->stackedWidget->setCurrentIndex(9);}
void editor::on_pushButton_12_released(){ui->stackedWidget->setCurrentIndex(9);}
void editor::on_pushButton_14_released(){ui->stackedWidget->setCurrentIndex(9);}
void editor::on_pushButton_16_released(){ui->stackedWidget->setCurrentIndex(9);}
void editor::on_pushButton_18_released(){ui->stackedWidget->setCurrentIndex(9);}


void editor::on_pushButton_4_released()
{
    QString text = "";

    for(int n = 0; n < column; n++){
        text.append(" ");
    }

    text.append("var ");
    text.append(ui->comboBox->currentText());
    text.append("=[");
    text.append(ui->comboBox_2->currentText());
    text.append(ui->comboBox_3->currentText());
    text.append("]");

    lines[row] = text;

    rebuild(1);
}


void editor::on_pushButton_5_released()
{
    QString text = "";

    for(int n = 0; n < column; n++){
        text.append(" ");
    }

    text.append("list ");
    text.append(ui->comboBox_10->currentText());

    lines[row] = text;

    rebuild(1);
}


void editor::on_pushButton_7_released()
{
    QString text = "";

    for(int n = 0; n < column; n++){
        text.append(" ");
    }

    text.append("add ");
    text.append(ui->comboBox_11->currentText());
    text.append(" [");
    text.append(ui->comboBox_12->currentText());
    text.append(ui->comboBox_15->currentText());
    text.append("][");
    text.append(ui->comboBox_13->currentText());
    text.append(ui->comboBox_16->currentText());
    text.append("][");
    text.append(ui->comboBox_14->currentText());
    text.append(ui->comboBox_17->currentText());
    text.append("]");

    lines[row] = text;

    rebuild(1);
}


void editor::on_pushButton_9_released()
{
    QString text = "";

    for(int n = 0; n < column; n++){
        text.append(" ");
    }

    text.append("remove ");
    text.append(ui->comboBox_35->currentText());
    text.append(" [");
    text.append(ui->comboBox_33->currentText());
    text.append(ui->comboBox_29->currentText());
    text.append("][");
    text.append(ui->comboBox_30->currentText());
    text.append(ui->comboBox_31->currentText());
    text.append("][");
    text.append(ui->comboBox_34->currentText());
    text.append(ui->comboBox_32->currentText());
    text.append("]");

    lines[row] = text;

    rebuild(1);
}


void editor::on_pushButton_11_released()
{
    QString text = "";

    for(int n = 0; n < column; n++){
        text.append(" ");
    }

    text.append("for [");
    text.append(ui->comboBox_37->currentText());
    text.append("][");
    text.append(ui->comboBox_36->currentText());
    text.append("][");
    text.append(ui->comboBox_41->currentText());
    text.append("]{\n");

    for(int n = 0; n < column; n++){
        text.append(" ");
    }
    text.append("}");

    lines[row] = text;

    rebuild(1);
}


void editor::on_pushButton_13_released()
{
    QString text = "";

    for(int n = 0; n < column; n++){
        text.append(" ");
    }

    text.append("if in([");
    text.append(ui->comboBox_39->currentText());
    text.append(ui->comboBox_42->currentText());
    text.append("][");
    text.append(ui->comboBox_38->currentText());
    text.append(ui->comboBox_43->currentText());
    text.append("][");
    text.append(ui->comboBox_44->currentText());
    text.append(ui->comboBox_40->currentText());
    text.append("],");

    if(ui->checkBox->isChecked()){
        text.append("!");
    }

    text.append(ui->comboBox_45->currentText());
    text.append("){\n");

    for(int n = 0; n < column; n++){
        text.append(" ");
    }
    text.append("}");

    lines[row] = text;

    rebuild(1);
}


void editor::on_pushButton_15_released()
{
    QString text = "";

    for(int n = 0; n < column; n++){
        text.append(" ");
    }

    text.append("if is([");
    text.append(ui->comboBox_47->currentText());
    text.append(ui->comboBox_49->currentText());
    text.append("][");
    text.append(ui->comboBox_46->currentText());
    text.append(ui->comboBox_50->currentText());
    text.append("][");
    text.append(ui->comboBox_51->currentText());
    text.append(ui->comboBox_48->currentText());
    text.append("],");

    if(ui->checkBox_2->isChecked()){
        text.append("!");
    }

    text.append(ui->comboBox_52->currentText().midRef(0, 1));
    text.append("){\n");

    for(int n = 0; n < column; n++){
        text.append(" ");
    }
    text.append("}");

    lines[row] = text;

    rebuild(1);
}


void editor::on_pushButton_17_released()
{
    QString text = "";

    for(int n = 0; n < column; n++){
        text.append(" ");
    }

    text.append("attack [");
    text.append(ui->comboBox_54->currentText());
    text.append("][");
    text.append(ui->comboBox_53->currentText());
    text.append("][");
    text.append(ui->comboBox_55->currentText());
    text.append("]");

    lines[row] = text;

    rebuild(1);
}


void editor::on_pushButton_21_released()
{
    ui->player->reset_button();
}


void editor::on_comboBox_4_currentIndexChanged(int index)
{
    ui->player->now_index = index;
    ui->player->reset_can_choose();
}


void editor::on_pushButton_19_released()
{
    ui->player->reset_ship();
}


void editor::on_pushButton_20_released()
{
    //全てのSHIPが配置済みの時のみ反応
    if(ui->player->count_ship(player_set_ship::ASHIP1) == 3 && ui->player->count_ship(player_set_ship::ASHIP2) == 3 && ui->player->count_ship(player_set_ship::BSHIP1) == 2 && ui->player->count_ship(player_set_ship::BSHIP2) == 2 && ui->player->count_ship(player_set_ship::BSHIP3) == 2 && ui->player->count_ship(player_set_ship::CSHIP1) == 1 && ui->player->count_ship(player_set_ship::CSHIP2) == 1 && ui->player->count_ship(player_set_ship::CSHIP3) == 1 && ui->player->count_ship(player_set_ship::CSHIP4) == 1){

        //配置情報格納
        for(int x = 0; x < BD_SIZE; x++){
            for(int y = 0; y < BD_SIZE; y++){
                for(int z = 0; z < BD_SIZE; z++){
                    ship[x][y][z] = ui->player->print_board[x][y][z];

                    if(x % (BD_SIZE - 1) == 0 && y % (BD_SIZE - 1) == 0 && z % (BD_SIZE - 1) == 0 ){
                        lines[z+1].replace(x+5*y+4, 1, "a");
                    }
                    else if(ship[x][y][z] == player_set_ship::NOSHIP){
                        lines[z+1].replace(x+5*y+4, 1, "b");
                    }
                    else if(ship[x][y][z] == player_set_ship::ASHIP1){
                        lines[z+1].replace(x+5*y+4, 1, "c");
                    }
                    else if(ship[x][y][z] == player_set_ship::ASHIP2){
                        lines[z+1].replace(x+5*y+4, 1, "d");
                    }
                    else if(ship[x][y][z] == player_set_ship::BSHIP1){
                        lines[z+1].replace(x+5*y+4, 1, "e");
                    }
                    else if(ship[x][y][z] == player_set_ship::BSHIP2){
                        lines[z+1].replace(x+5*y+4, 1, "f");
                    }
                    else if(ship[x][y][z] == player_set_ship::BSHIP3){
                        lines[z+1].replace(x+5*y+4, 1, "g");
                    }
                    else if(ship[x][y][z] == player_set_ship::CSHIP1){
                        lines[z+1].replace(x+5*y+4, 1, "h");
                    }
                    else if(ship[x][y][z] == player_set_ship::CSHIP2){
                        lines[z+1].replace(x+5*y+4, 1, "i");
                    }
                    else if(ship[x][y][z] == player_set_ship::CSHIP3){
                        lines[z+1].replace(x+5*y+4, 1, "j");
                    }
                    else if(ship[x][y][z] == player_set_ship::CSHIP4){
                        lines[z+1].replace(x+5*y+4, 1, "k");
                    }
                }
            }
        }

        rebuild(1);
        ui->label_3->setText("Placement changed");
    }
    else{
        ui->label_3->setText("Invalid placement");
    }
}


void editor::on_pushButton_22_released()
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << inStr;
    file.close();

    QString save = "## file saved: ";
    save.append(fileName);
    ui->textBrowser_2->append(save);
}


void editor::on_pushButton_23_released()
{
    ui->enemy->reset_button();
}


void editor::reset_debug()
{
    int n = ui->comboBox_5->currentIndex();
    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                ui->enemy->set_ship(x, y, z, enemy1[n][x][y][z]);
                enemy_state[x][y][z] = enemy1[n][x][y][z];
            }
        }
    }

    ui->enemy->init_print_board();

    interpriter_for_debug = new bsl_interpriter();

    attack_counter = 100;
    run_flag = 0;
}


int editor::count_unattacked()
{
    int count = 0;  //攻撃を受けていない船の数

    for(int x = 0; x < BD_SIZE; x++){
        for(int y = 0; y < BD_SIZE; y++){
            for(int z = 0; z < BD_SIZE; z++){
                if(enemy_state[x][y][z] == ASHIP || enemy_state[x][y][z] == BSHIP || enemy_state[x][y][z] == CSHIP){
                    count++;
                }
            }
        }
    }
    return count;
}


void editor::timerEvent(QTimerEvent *)
{
    if(run_flag == 1 && attack_counter < 100){
        QString turn = "turn: ";
        turn.append(QString::number(attack_counter+1));
        ui->textBrowser_2->append(turn);
        int n = interpriter_for_debug->interpriter(fileName);

        if(n >= 0 && n < BD_SIZE * BD_SIZE * BD_SIZE){
            int x = n / (BD_SIZE*BD_SIZE);
            int y = (n % (BD_SIZE * BD_SIZE)) / BD_SIZE;
            int z = n % BD_SIZE;

            ui->enemy->set_ship_window(x, y, z, ui->enemy->get_ship(x, y, z));

            interpriter_for_debug->set_enemy_board(x, y, z, ui->enemy->get_ship(x, y, z));

            QString result;
            if(enemy_state[x][y][z] == ATTACKED){
                result = "==========>> result: ATTACKED";
            }
            else if(enemy_state[x][y][z] == ROCK){
                result = "==========>> result: ROCK";
            }
            else if(enemy_state[x][y][z] == ASHIP){
                result = "==========>> result: ASHIP";
            }
            else if(enemy_state[x][y][z] == BSHIP){
                result = "==========>> result: BSHIP";
            }
            else if(enemy_state[x][y][z] == CSHIP){
                result = "==========>> result: CSHIP";
            }
            else{
                result = "==========>> result: NOSHIP";
            }

            enemy_state[x][y][z] = ATTACKED;

            ui->textBrowser_2->append(interpriter_for_debug->text);

            ui->textBrowser_2->append(result);

            if(count_unattacked() == 0){
                ui->textBrowser_2->append("## all ships attacked");
                attack_counter = 100;
                run_flag = 0;
                return;
            }
        }
        else{
            ui->textBrowser_2->append(">> out of range");
        }

        attack_counter++;
        if(attack_counter == 100){
            ui->textBrowser_2->append("## too many turns");
        }
    }
}


void editor::on_pushButton_24_released()
{
    if(attack_counter == 100){
        reset_debug();
        attack_counter = 0;
        run_flag = 1;
    }
    else if(run_flag == 0){
        run_flag = 1;
    }
    else{
        run_flag = 0;
    }
}


void editor::on_pushButton_26_released()
{
    ui->textBrowser_2->append("## reset");
    reset_debug();
}


void editor::on_comboBox_5_currentIndexChanged(int index)
{
    if(index == 0){
        ui->textBrowser_2->append("## enemy changed : enemy1");
    }
    if(index == 1){
        ui->textBrowser_2->append("## enemy changed : enemy2");
    }
    if(index == 2){
        ui->textBrowser_2->append("## enemy changed : enemy3");
    }
    reset_debug();
}
