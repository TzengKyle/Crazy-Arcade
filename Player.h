#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QAudioOutput> //注意這裡QMediaPlayer
#include <QMediaPlayer>

#include "Setting.h"
#include "ItemOnMap.h"
//#include "widget.h"

enum direction {
    Left,
    Right,
    Down,
    Up
};

class Player : public QObject, public QGraphicsPixmapItem
{

    Q_OBJECT

public:
    Player(int x, int y, int player = 1, QObject *parent = nullptr); //Player建構子 分別是x,y座標和是建構哪一個player
    void move(direction dir,int player, ItemOnMap* ptrMap[sizes::Rows][sizes::Columns]); //std::vector<std::vector<Field *>>& fields
    void plantBomb(ItemOnMap* ptrMap[sizes::Rows][sizes::Columns]);
    void hurt();
    void addRange();
    void addMaxBombs();
    void getSuper();
    void makeOppoDizzy();
    void rememberOppo(Player*player){oppositePlayer = player;}

    int getX(){return x;}
    int getY(){return y;}
    int getHealth(){return health;}
    int getMaxBombs(){return maxBombs;}
    int getRange(){return range;}
    int getPlantedBomb(){return plantedBomb;}
    int getDizzyState(){return dizzyState;}

    int getScore(){return score;}

    int x;
    int y;

    int health;
    int maxBombs;
    int range;

    int score = 0;

public slots:
    void plantedBomb_explode();
    void lossSuper();
    void wakeFromDizzy();
    void player_effect();

    void addScoreDestroy(){score++;}
    void addScoreHurt(){score+=8;}

signals:
    void explode();
    void player_dead();

private:
    int plantedBomb;

    int superState;
    int dizzyState;
    int player_effect_timer_index = 0;

    Player*oppositePlayer;

    //聲音
    QAudioOutput*move_sound;
    QMediaPlayer*move_sound_Player;
    QAudioOutput*plant_bomb_down_sound;
    QMediaPlayer*plant_bomb_down_sound_Player;
    QAudioOutput*explode_sound;
    QMediaPlayer*explode_sound_Player;
    QAudioOutput*pick_up_sound;
    QMediaPlayer*pick_up_sound_Player;
    QAudioOutput*star_power_sound;
    QMediaPlayer*star_power_sound_Player;

    QTimer*superState_timer;
    QTimer*dizzyState_timer;
    QTimer*player_effect_timer;

};

#endif // PLAYER_H
