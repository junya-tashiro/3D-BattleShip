#ifndef CPU_H
#define CPU_H

#include "player.h"

class cpu : public player
{
    Q_OBJECT
public:
    explicit cpu(QWidget *parent = nullptr);
    virtual ~cpu();

    void mousePressEvent(QMouseEvent *event);

private:
    void paintGL();
};

#endif // CPU_H
