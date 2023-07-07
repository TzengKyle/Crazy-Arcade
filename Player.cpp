#include "Player.h"
#include <iostream>
#include <QTimer>

//Player 建構子
Player::Player(int x, int y, int player, QObject *parent)  : QObject (parent) {
    srand(time(NULL));

    this->x = x;
    this->y = y;
    this->health = sizes::Health;
    this->maxBombs = sizes::IniBombsNum;
    this->range = sizes::IniExplosionRange;
    this->plantedBomb = 0;
    this->superState = 0;
    this->dizzyState = 0;


    setPos(sizes::Strat_pixel_x + x * sizes::FieldSize,sizes::Strat_pixel_y + y * sizes::FieldSize);
    if(player == 1){
        setPixmap(QPixmap(":/img/img/charac_1_down.png"));
    }else{
        setPixmap(QPixmap(":/img/img/charac_2_down.png"));
    }

    //讀取音樂
        //移動的音效
    move_sound = new QAudioOutput();
    move_sound->setVolume(50);
    move_sound_Player = new QMediaPlayer();
    move_sound_Player->setAudioOutput(move_sound);
    move_sound_Player->setSource(QUrl("qrc:/sound/sound/move_sound.mp3"));
        //放水球音效
    plant_bomb_down_sound = new QAudioOutput();
    plant_bomb_down_sound->setVolume(50);
    plant_bomb_down_sound_Player = new QMediaPlayer();
    plant_bomb_down_sound_Player->setAudioOutput(plant_bomb_down_sound);
    plant_bomb_down_sound_Player->setSource(QUrl("qrc:/sound/sound/plant_bomb_down_sound.mp3"));
        //爆炸的音效
    explode_sound = new QAudioOutput();
    explode_sound->setVolume(50);
    explode_sound_Player = new QMediaPlayer();
    explode_sound_Player->setAudioOutput(explode_sound);
    explode_sound_Player->setSource(QUrl("qrc:/sound/sound/explode_sound.mp3"));
        //拾取的音效
    pick_up_sound = new QAudioOutput();
    pick_up_sound->setVolume(50);
    pick_up_sound_Player = new QMediaPlayer();
    pick_up_sound_Player->setAudioOutput(pick_up_sound);
    pick_up_sound_Player->setSource(QUrl("qrc:/sound/sound/pick_up_sound.mp3"));
        //無敵狀態的音效
    star_power_sound = new QAudioOutput();
    star_power_sound->setVolume(25);
    star_power_sound_Player = new QMediaPlayer();
    star_power_sound_Player->setAudioOutput(star_power_sound);
    star_power_sound_Player->setSource(QUrl("qrc:/sound/sound/star_power_sound.mp3"));

    //Timer
    superState_timer = new QTimer(this); //
    connect(superState_timer, SIGNAL(timeout()), this, SLOT(lossSuper()));
    superState_timer->start(1000);

    dizzyState_timer = new QTimer(this); //
    connect(dizzyState_timer, SIGNAL(timeout()), this, SLOT(wakeFromDizzy()));
    dizzyState_timer->start(1000);

    player_effect_timer = new QTimer(this); //
    connect(player_effect_timer, SIGNAL(timeout()), this, SLOT(player_effect()));
    player_effect_timer->start(100);
}

//Player 移動 注意player2被炸彈檔住的部分
void Player::move(direction dir, int player, ItemOnMap*ptrMap[sizes::Rows][sizes::Columns]){ //int player只是為了在移動更改圖片時會有差
    if(player == 1){
        if(dir == Left) {
            int ori_x = this->x, ori_y = this->y;
            x--;
            if(!((x >= 0 && x < sizes::Columns) && (y >= 0 && y < sizes::Rows))){
                x++;
                qDebug() << "player1_touch_border";
            }else if(ptrMap[y][x]->type == Undestroyableblock){
                qDebug() << "player1_touch_UnDestroyableBlock :" << x << "," << y;
                x++;
            }else if(ptrMap[y][x]->type == Destroyableblock){
                qDebug() << "player1_touch_DestroyableBlock :" << x << "," << y;
                x++;
            }else if(ptrMap[y][x]->type == Bomb){
                qDebug() << "player1_touch_Bomb :" << x << "," << y;
                x++;
            }else if(ptrMap[y][x]->player2 != nullptr){
                x++;
            }
            setPos(sizes::Strat_pixel_x + this->x * sizes::FieldSize,sizes::Strat_pixel_y + this->y * sizes::FieldSize);
            setPixmap(QPixmap(":/img/img/charac_1_left.png"));

            //改座標
            ptrMap[ori_y][ori_x]->setPlayerOffMap(1);
            ptrMap[y][x]->setPlayerOnMap(1, this);

        }
        else if (dir == Right) {
            int ori_x = this->x, ori_y = this->y;
            x++;
            if(!((x >= 0 && x < sizes::Columns) && (y >= 0 && y < sizes::Rows))){
                x--;
                qDebug() << "player1_touch_border";
            }else if(ptrMap[y][x]->type == Undestroyableblock){
                qDebug() << "player1_touch_UnDestroyableBlock :" << x << "," << y;
                x--;
            }else if(ptrMap[y][x]->type == Destroyableblock){
                qDebug() << "player1_touch_DestroyableBlock :" << x << "," << y;
                x--;
            }else if(ptrMap[y][x]->type == Bomb){
                qDebug() << "player1_touch_Bomb :" << x << "," << y;
                x--;
            }else if(ptrMap[y][x]->player2 != nullptr){
                x--;
            }
            setPos(sizes::Strat_pixel_x + this->x * sizes::FieldSize,sizes::Strat_pixel_y + this->y * sizes::FieldSize);
            setPixmap(QPixmap(":/img/img/charac_1_right.png"));

            //改座
            ptrMap[ori_y][ori_x]->setPlayerOffMap(1);
            ptrMap[y][x]->setPlayerOnMap(1, this);
        }
        else if (dir == Down) {
            int ori_x = this->x, ori_y = this->y;
            y++;
            if(!((x >= 0 && x < sizes::Columns) && (y >= 0 && y < sizes::Rows))){
                y--;
                qDebug() << "player1_touch_border";
            }else if(ptrMap[y][x]->type == Undestroyableblock){
                qDebug() << "player1_touch_UnDestroyableBlock :" << x << "," << y;
                y--;
            }else if(ptrMap[y][x]->type == Destroyableblock){
                qDebug() << "player1_touch_DestroyableBlock :" << x << "," << y;
                y--;
            }else if(ptrMap[y][x]->type == Bomb){
                qDebug() << "player1_touch_Bomb :" << x << "," << y;
                y--;
            }else if(ptrMap[y][x]->player2 != nullptr){
                y--;
            }
            setPos(sizes::Strat_pixel_x + this->x * sizes::FieldSize,sizes::Strat_pixel_y + this->y * sizes::FieldSize);
            setPixmap(QPixmap(":/img/img/charac_1_down.png"));

            //改座標
            ptrMap[ori_y][ori_x]->setPlayerOffMap(1);
            ptrMap[y][x]->setPlayerOnMap(1, this);
        }
        else if (dir == Up) {
            int ori_x = this->x, ori_y = this->y;
            y--;
            if(!((x >= 0 && x < sizes::Columns) && (y >= 0 && y < sizes::Rows))){
                y++;
                qDebug() << "player1_touch_border";
            }else if(ptrMap[y][x]->type == Undestroyableblock){
                qDebug() << "player1_touch_UnDestroyableBlock :" << x << "," << y;
                y++;
            }else if(ptrMap[y][x]->type == Destroyableblock){
                qDebug() << "player1_touch_DestroyableBlock :" << x << "," << y;
                y++;
            }else if(ptrMap[y][x]->type == Bomb){
                qDebug() << "player1_touch_Bomb :" << x << "," << y;
                y++;
            }else if(ptrMap[y][x]->player2 != nullptr){
                y++;
            }
            setPos(sizes::Strat_pixel_x + this->x * sizes::FieldSize,sizes::Strat_pixel_y + this->y * sizes::FieldSize);
            setPixmap(QPixmap(":/img/img/charac_1_up.png"));

            //改座標
            ptrMap[ori_y][ori_x]->setPlayerOffMap(1);
            ptrMap[y][x]->setPlayerOnMap(1, this);

        }
        move_sound_Player->play();
    }else{
        if(dir == Left) {
            int ori_x = this->x, ori_y = this->y;
            x--;
            if(!((x >= 0 && x < sizes::Columns) && (y >= 0 && y < sizes::Rows))){
                x++;
                qDebug() << "player1_touch_border";
            }else if(ptrMap[y][x]->type == Undestroyableblock){
                qDebug() << "player2_touch_UnDestroyableBlock :" << x << "," << y;
                x++;
            }else if(ptrMap[y][x]->type == Destroyableblock){
                qDebug() << "player2_touch_DestroyableBlock :" << x << "," << y;
                x++;
            }else if(ptrMap[y][x]->type == Bomb){
                qDebug() << "player2_touch_Bomb :" << x << "," << y;
                x++;
            }else if(ptrMap[y][x]->player1 != nullptr){
                x++;
            }
            setPos(sizes::Strat_pixel_x + this->x * sizes::FieldSize,sizes::Strat_pixel_y + this->y * sizes::FieldSize);
            setPixmap(QPixmap(":/img/img/charac_2_left.png"));

            //改座標
            ptrMap[ori_y][ori_x]->setPlayerOffMap(2);
            ptrMap[y][x]->setPlayerOnMap(2, this);
        }
        else if (dir == Right) {
            int ori_x = this->x, ori_y = this->y;
            x++;
            if(!((x >= 0 && x < sizes::Columns) && (y >= 0 && y < sizes::Rows))){
                x--;
                qDebug() << "player1_touch_border";
            }else if(ptrMap[y][x]->type == Undestroyableblock){
                qDebug() << "player2_touch_UnDestroyableBlock :" << x << "," << y;
                x--;
            }else if(ptrMap[y][x]->type == Destroyableblock){
                qDebug() << "player2_touch_DestroyableBlock :" << x << "," << y;
                x--;
            }else if(ptrMap[y][x]->type == Bomb){
                qDebug() << "player2_touch_Bomb :" << x << "," << y;
                x--;
            }else if(ptrMap[y][x]->player1 != nullptr){
                x--;
            }
            setPos(sizes::Strat_pixel_x + this->x * sizes::FieldSize,sizes::Strat_pixel_y + this->y * sizes::FieldSize);
            setPixmap(QPixmap(":/img/img/charac_2_right.png"));

            //改座標
            ptrMap[ori_y][ori_x]->setPlayerOffMap(2);
            ptrMap[y][x]->setPlayerOnMap(2, this);
        }
        else if (dir == Down) {
            int ori_x = this->x, ori_y = this->y;
            y++;
            if(!((x >= 0 && x < sizes::Columns) && (y >= 0 && y < sizes::Rows))){
                y--;
                qDebug() << "player1_touch_border";
            }else if(ptrMap[y][x]->type == Undestroyableblock){
                qDebug() << "player2_touch_UnDestroyableBlock :" << x << "," << y;
                y--;
            }else if(ptrMap[y][x]->type == Destroyableblock){
                qDebug() << "player2_touch_DestroyableBlock :" << x << "," << y;
                y--;
            }else if(ptrMap[y][x]->type == Bomb){
                qDebug() << "player2_touch_Bomb :" << x << "," << y;
                y--;
            }else if(ptrMap[y][x]->player1 != nullptr){
                y--;
            }
            setPos(sizes::Strat_pixel_x + this->x * sizes::FieldSize,sizes::Strat_pixel_y + this->y * sizes::FieldSize);
            setPixmap(QPixmap(":/img/img/charac_2_down.png"));

            //改座標
            ptrMap[ori_y][ori_x]->setPlayerOffMap(2);
            ptrMap[y][x]->setPlayerOnMap(2, this);
        }
        else if (dir == Up) {
            int ori_x = this->x, ori_y = this->y;
            y--;
            if(!((x >= 0 && x < sizes::Columns) && (y >= 0 && y < sizes::Rows))){
                y++;
                qDebug() << "player1_touch_border";
            }else if(ptrMap[y][x]->type == Undestroyableblock){
                qDebug() << "player2_touch_UnDestroyableBlock :" << x << "," << y;
                y++;
            }else if(ptrMap[y][x]->type == Destroyableblock){
                qDebug() << "player2_touch_DestroyableBlock :" << x << "," << y;
                y++;
            }else if(ptrMap[y][x]->type == Bomb){
                qDebug() << "player2_touch_Bomb :" << x << "," << y;
                y++;
            }else if(ptrMap[y][x]->player1 != nullptr){
                y++;
            }
            setPos(sizes::Strat_pixel_x + this->x * sizes::FieldSize,sizes::Strat_pixel_y + this->y * sizes::FieldSize);
            setPixmap(QPixmap(":/img/img/charac_2_up.png"));

            //改座標
            ptrMap[ori_y][ori_x]->setPlayerOffMap(2);
            ptrMap[y][x]->setPlayerOnMap(2, this);
        }
        move_sound_Player->play();
    }
}

void Player::plantBomb(ItemOnMap* ptrMap[sizes::Rows][sizes::Columns]){
    if(ptrMap[this->y][this->x]->type == Empty){
        ptrMap[this->y][this->x]->setBomb();
        plant_bomb_down_sound_Player->play();

        QTimer::singleShot(2000, ptrMap[this->y][this->x], SLOT(explosion()));

        bool flagLeft = true, flagRight = true, flagUp = true, flagDown = true;
        for(int i = 1; i <= this->range ;i++){ //=
            if(this->x+i < sizes::Columns && ptrMap[this->y][this->x+i]->type != Undestroyableblock && flagRight == true){
                if(ptrMap[this->y][this->x+i]->type != Empty && ptrMap[this->y][this->x+i]->type != Bomb && ptrMap[this->y][this->x+i]->type != Fire){
                    flagRight = false;
                    if(ptrMap[this->y][this->x+i]->type == Destroyableblock){
                        QTimer::singleShot(2000, this, SLOT(addScoreDestroy()));

                    }
                }
                QTimer::singleShot(2000, ptrMap[this->y][this->x+i], SLOT(explosion()));
            }

            if(this->x-i >= 0 && ptrMap[this->y][this->x-i]->type != Undestroyableblock && flagLeft == true){
                if(ptrMap[this->y][this->x-i]->type != Empty && ptrMap[this->y][this->x-i]->type != Bomb && ptrMap[this->y][this->x-i]->type != Fire){
                    flagLeft = false;
                    if(ptrMap[this->y][this->x-i]->type == Destroyableblock){
                        QTimer::singleShot(2000, this, SLOT(addScoreDestroy()));
                    }
                }
                QTimer::singleShot(2000, ptrMap[this->y][this->x-i], SLOT(explosion()));
            }

            if(this->y+i < sizes::Rows && ptrMap[this->y+i][this->x]->type != Undestroyableblock && flagDown == true){
                if(ptrMap[this->y+i][this->x]->type != Empty && ptrMap[this->y+i][this->x]->type != Bomb && ptrMap[this->y+i][this->x]->type != Fire){
                    flagDown = false;
                    if(ptrMap[this->y+i][this->x]->type == Destroyableblock){
                        QTimer::singleShot(2000, this, SLOT(addScoreDestroy()));
                    }
                }
                QTimer::singleShot(2000, ptrMap[this->y+i][this->x], SLOT(explosion()));
            }

            if(this->y-i >= 0 && ptrMap[this->y-i][this->x]->type != Undestroyableblock && flagUp == true){
                if(ptrMap[this->y-i][this->x]->type != Empty && ptrMap[this->y-i][this->x]->type != Bomb && ptrMap[this->y-i][this->x]->type != Fire){
                    flagUp = false;
                    if(ptrMap[this->y-i][this->x]->type == Destroyableblock){
                        QTimer::singleShot(2000, this, SLOT(addScoreDestroy()));
                    }
                }
                QTimer::singleShot(2000, ptrMap[this->y-i][this->x], SLOT(explosion()));
            }


            if(this->x+i < sizes::Columns && ptrMap[this->y][this->x+i]->type == Undestroyableblock){
                flagRight = false;
            }
            if(this->x-i >= 0 && ptrMap[this->y][this->x-i]->type == Undestroyableblock){
                flagLeft = false;
            }
            if(this->y+i < sizes::Rows && ptrMap[this->y+i][this->x]->type == Undestroyableblock){
                flagDown = false;
            }
            if(this->y-i >= 0 && ptrMap[this->y-i][this->x]->type == Undestroyableblock){
                flagUp = false;
            }

        }
        this->plantedBomb++;

        QTimer::singleShot(2000, this, SLOT(plantedBomb_explode()));//
    }
}

void Player::plantedBomb_explode(){
    explode_sound_Player->play();
    plantedBomb--;
}

void Player::hurt(){
    if(superState == 0){
        health--;
        oppositePlayer->addScoreHurt();
    }

    if(health <= 0){
        emit player_dead();
    }
}

void Player::addRange(){
    if(range < sizes::MaximumExplosionRange){
        range++;
    }
    pick_up_sound_Player->play();
}

void Player::addMaxBombs(){
    if(maxBombs < sizes::MaximumBombsNum){
        maxBombs++;
    }
    pick_up_sound_Player->play();
}

void Player::getSuper(){
    superState += sizes::Superstar_time;
    qDebug() << "get super";
}

void Player::lossSuper(){
    if(superState > 0){
        superState--;
        qDebug() << "super still have" << superState;
    }
}

void Player::makeOppoDizzy(){
    oppositePlayer->dizzyState += sizes::Dizzy_time;
    qDebug() << "make dizzy";
}

void Player::wakeFromDizzy(){
    if(dizzyState > 0){
        dizzyState--;
        qDebug() << "dizzy still have" << dizzyState;
    }
}

void Player::player_effect(){
    if(superState > 0){
        star_power_sound_Player->play();
        player_effect_timer_index++;
        if(player_effect_timer_index/2%2 == 0){
            setOpacity(1);
        }
        else{
            setOpacity(0.2);
        }
    }else{
        setOpacity(1);
        star_power_sound_Player->pause();
    }

}
