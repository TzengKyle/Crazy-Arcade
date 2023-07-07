#include "ItemOnMap.h"
#include "player.h"

ItemOnMap::ItemOnMap(int x, int y, QObject *parent) : QObject(parent) //, Player*player1, Player*player2
{
    this->x = x;
    this->y = y;
    setPos(sizes::Strat_pixel_x + x * sizes::FieldSize,sizes::Strat_pixel_y + y * sizes::FieldSize);
    type = Empty;

    player1 = nullptr;
    player2 = nullptr;

    prop_timer = new QTimer(this);
    connect(prop_timer, SIGNAL(timeout()), this, SLOT(prop_picked_monitor()));
    prop_timer->start();

    flash_bomb_timer = new QTimer(this);
    connect(flash_bomb_timer, SIGNAL(timeout()), this, SLOT(floatProp()));
    flash_bomb_timer->start(100);

    prop_float_timer = new QTimer(this);
    connect(prop_float_timer, SIGNAL(timeout()), this, SLOT(flashBomb()));
    prop_float_timer->start(100);
}

void ItemOnMap::setPlayerOnMap(int player,Player*player_ptr){
    if(player == 1){
        this->player1 = player_ptr;
        qDebug() << "player1 on" << this->x << "," << this->y;
    }else{
        this->player2 = player_ptr;
        qDebug() << "player2 on" << this->x << "," << this->y;
    }
}

void ItemOnMap::setPlayerOffMap(int player){
    if(player == 1){
        this->player1 = nullptr;
        qDebug() << "player1 leave" << this->x << "," << this->y;
    }else{
        this->player2 = nullptr;
        qDebug() << "player2 leave" << this->x << "," << this->y;
    }
}

void ItemOnMap::explosion(){
    qDebug() << "(" << x << "," << y << ") Explode.";

    bool canGenerProp = false;

    if(this->type == Destroyableblock){
        canGenerProp = true;
    }

    setFire();

    if(player1 != nullptr){
        player1->hurt();
    }

    if(player2 != nullptr){
        player2->hurt();
    }

    if(canGenerProp == true){
        int randnum = rand()%100;

        qDebug() << "Destroyableblock" << x << y << ": randnum" << randnum;

        if(randnum < sizes::no_prop){
            QTimer::singleShot(100, this, SLOT(setEmpty()));
        }else if(randnum < sizes::Increaserange_prop){
            QTimer::singleShot(100, this, SLOT(setIncreaserangeprop()));
        }else if(randnum < sizes::Increasemaxbombs_prop){
            QTimer::singleShot(100, this, SLOT(setIncreasemaxbombsprop()));
        }else if(randnum < sizes::Superstar_prop){
            QTimer::singleShot(100, this, SLOT(setSuperstarprop()));
        }else{ //== else if(randnum < sizes::Dizzy_prop)
            QTimer::singleShot(100, this, SLOT(setDizzyprop()));
        }

    }else{
        QTimer::singleShot(100, this, SLOT(setEmpty()));
    }
}

void ItemOnMap::setEmpty(){
    setPixmap(QPixmap("")); //設為沒有
    type = Empty;
}

void ItemOnMap::setUndestroyableblock(){
    setPixmap(QPixmap(":/img/img/unDestroyableBlock.png"));
    type = Undestroyableblock;
}

void ItemOnMap::setDestroyableblock(){
    int randnum = rand();
    if(randnum%2==0)
        setPixmap(QPixmap(":/img/img/tree.png"));
    else
        setPixmap(QPixmap(":/img/img/destroyableBlock.png"));
    type = Destroyableblock;
}

void ItemOnMap::setBomb(){
    setPixmap(QPixmap(":/img/img/bomb.png"));
    type = Bomb;
}

void ItemOnMap::setFire(){
    setPixmap(QPixmap(":/img/img/fire.png"));
    type = Fire;
}

void ItemOnMap::setIncreaserangeprop(){
    setPixmap(QPixmap(":/img/img/range.png"));
    type = Increaserangeprop;
}

void ItemOnMap::setIncreasemaxbombsprop(){
    setPixmap(QPixmap(":/img/img/Number.png"));
    type = Increasemaxbombsprop;
}

void ItemOnMap::setSuperstarprop(){
    setPixmap(QPixmap(":/img/img/SuperStar.png"));
    type = Superstarprop;
}

void ItemOnMap::setDizzyprop(){
    setPixmap(QPixmap(":/img/img/dizzy.png"));
    type = Dizzyprop;
}

void ItemOnMap::increaserangeprop_picked_func(){

    if(player1 != nullptr){
        player1->addRange();
    }

    if(player2 != nullptr){
        player2->addRange();
    }

    setEmpty();
}

void ItemOnMap::increasemaxbombsprop_picked_func(){

    if(player1 != nullptr){
        player1->addMaxBombs();
    }

    if(player2 != nullptr){
        player2->addMaxBombs();
    }

    setEmpty();
}

void ItemOnMap::superstarprop_picked_func(){

    if(player1 != nullptr){
        player1->getSuper();
    }

    if(player2 != nullptr){
        player2->getSuper();
    }

    setEmpty();
}

void ItemOnMap::dizzyprop_picked_func(){

    if(player1 != nullptr){
        player1->makeOppoDizzy();
    }

    if(player2 != nullptr){
        player2->makeOppoDizzy();
    }

    setEmpty();
}

void ItemOnMap::prop_picked_monitor(){
    if(player1 != nullptr || player2 != nullptr){
        if(type == Increaserangeprop){
            QTimer::singleShot(0, this, SLOT(increaserangeprop_picked_func()));
        }
    }

    if(player1 != nullptr || player2 != nullptr){
        if(type == Increasemaxbombsprop){
            QTimer::singleShot(0, this, SLOT(increasemaxbombsprop_picked_func()));
        }
    }

    if(player1 != nullptr || player2 != nullptr){
        if(type == Superstarprop){
            QTimer::singleShot(0, this, SLOT(superstarprop_picked_func()));
        }
    }

    if(player1 != nullptr || player2 != nullptr){
        if(type == Dizzyprop){
            QTimer::singleShot(0, this, SLOT(dizzyprop_picked_func()));
        }
    }
}

void ItemOnMap::flashBomb(){
    if(type == Bomb){
        flash_bomb_timer_index++;
        if(flash_bomb_timer_index/5%2 == 0){
            setPixmap(QPixmap(":/img/img/bomb.png"));
        }
        else{
            setPixmap(QPixmap(":/img/img/flashed_bomb.png"));
        }
    }
}

void ItemOnMap::floatProp(){
    if(type == Superstarprop || type == Increasemaxbombsprop || type == Increaserangeprop || type == Dizzyprop){
        prop_float_timer_index++;
        if(prop_float_timer_index/3%4 == 0){
            setPos(sizes::Strat_pixel_x +x*sizes::FieldSize,sizes::Strat_pixel_y + y*sizes::FieldSize-1);
        }else if(prop_float_timer_index/3%4 == 1){
            setPos(sizes::Strat_pixel_x +x*sizes::FieldSize,sizes::Strat_pixel_y +y*sizes::FieldSize-5);
        }else if(prop_float_timer_index/3%3 == 2){
            setPos(sizes::Strat_pixel_x +x*sizes::FieldSize,sizes::Strat_pixel_y +y*sizes::FieldSize-1);
        }else{
            setPos(sizes::Strat_pixel_x +x*sizes::FieldSize,sizes::Strat_pixel_y +y*sizes::FieldSize+3);
        }
    }
}




