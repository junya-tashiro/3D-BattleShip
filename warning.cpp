#include "warning.h"
#include "ui_warning.h"


//コンストラクタ
warning::warning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::warning)
{
    ui->setupUi(this);
}


//デコンストラクタ
warning::~warning()
{
    delete ui;
}


//OKボタン
void warning::on_pushButton_released()
{
    this->close();
}

