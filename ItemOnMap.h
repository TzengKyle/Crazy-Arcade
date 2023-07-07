#ifndef ITEMONMAP_H
#define ITEMONMAP_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QTimer>

#include <random>
#include <time.h>

#include "Setting.h"

class Player;

enum itemType { //只有開頭大寫 Class是字首都大寫
    Empty,
    Undestroyableblock,
    Destroyableblock,
    Bomb,
    Fire,
    Increaserangeprop,
    Increasemaxbombsprop,
    Superstarprop,
    Dizzyprop
};

class ItemOnMap : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    ItemOnMap(int x = 0, int y = 0, QObject *parent = nullptr); //explicit , Player*player1 = nullptr, Player*player2 = nullptr
    itemType type;
    Player*player1;
    Player*player2;



public slots:
    void setPlayerOnMap(int player,Player*player_ptr);
    void setPlayerOffMap(int player);

    void setEmpty();
    void setUndestroyableblock();
    void setDestroyableblock();
    void setBomb();
    void setFire();
    void setIncreaserangeprop();
    void setIncreasemaxbombsprop();
    void setSuperstarprop();
    void setDizzyprop();

    void flashBomb();
    void floatProp();

    void explosion();
    void prop_picked_monitor();

    void increaserangeprop_picked_func();
    void increasemaxbombsprop_picked_func();
    void superstarprop_picked_func();
    void dizzyprop_picked_func();

signals:

private:
    int x;
    int y;

    int prop_float_timer_index = 0;
    int flash_bomb_timer_index = 0;

    QTimer*prop_timer;
    QTimer*flash_bomb_timer;
    QTimer*prop_float_timer;
};

#endif // ITEMONMAP_H
