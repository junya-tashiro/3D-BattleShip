#ifndef WARNING_H
#define WARNING_H

#include <QWidget>

namespace Ui {
class warning;
}

class warning : public QWidget
{
    Q_OBJECT

public:
    explicit warning(QWidget *parent = nullptr);    //コンストラクタ
    ~warning();                                     //デコンストラクタ

private slots:
    void on_pushButton_released();                  //OKボタン

private:
    Ui::warning *ui;
};

#endif // WARNING_H
