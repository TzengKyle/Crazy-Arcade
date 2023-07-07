#include "Game.h"
#include "ui_Game.h"
#include "Setting.h"

#include <string>
#include <QAudioOutput> //注意這裡QMediaPlayer
#include <QMediaPlayer>
#include <QTimer>
#include <QDebug>
#include <QRandomGenerator>
#include <QMovie>

Game::Game(int isBuildNewGame, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Game), game_scene(new Scene)
{
    //一些前置作業 先忽略
    ui->setupUi(this);
    game_scene->setSceneRect(0,0,1161,794); //550,650
    ui->graphicsView->setScene(game_scene);
    ui->graphicsView->setFixedSize(1161,794);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //window的標題和圖示
    setWindowTitle(tr("Bomberman"));
    setWindowIcon(QPixmap(":img/img/bomb.png"));

    int get_record_counter=0;

    if (isBuildNewGame == 0){//創新遊戲
        //遊戲時間
        game_timer_index = sizes::GameTime;
        //會出現在遊戲畫面的所有東西在這裡construct
        player1 = new Player(0, 0, 1);
        player2 = new Player(sizes::Columns - 1, sizes::Rows - 1, 2); //Player(sizes::Columns - 1, sizes::Rows - 1, 2)
        player1->rememberOppo(player2);
        player2->rememberOppo(player1);
        /*
        ui->player1_Range->setText(QString::number(player1->getRange()));
        ui->player2_Range->setText(QString::number(player2->getRange()));
        ui->player1_Bombs->setText(QString::number(player1->getMaxBombs()));
        ui->player2_Bombs->setText(QString::number(player2->getMaxBombs()));

        ui->player1_Range->setStyleSheet("QLabel {color : white; }");
        ui->player2_Range->setStyleSheet("QLabel {color : white; }");
        ui->player1_Bombs->setStyleSheet("QLabel {color : white; }");
        ui->player2_Bombs->setStyleSheet("QLabel {color : white; }");


        ui->game_timer_Label->setStyleSheet("QLabel { background-color : white; color : black; }");
        */

        //把上面建構的player加入進scene
        game_scene->addItem(player1);
        game_scene->addItem(player2);

        for (int i = 0; i < sizes::Rows; i++) {
            for(int j = 0; j < sizes::Columns; j++) {
                this->ptrMap[i][j] = new ItemOnMap(j , i);

                //generating UndestroyableBlocks
                if (i % 2 && j % 2) {
                    ptrMap[i][j]->setUndestroyableblock();
                }
                //generating DestoryableBlocks
                else if ((i > 1 || j > 1) && (i < sizes::Rows - 2 || j < sizes::Columns - 2)) {
                    if(QRandomGenerator::global()->bounded(sizes::Frequency)) { //put bigger number to generate blocks more often
                        ptrMap[i][j]->setDestroyableblock();
                    }
                }
                game_scene->addItem(ptrMap[i][j]);
            }
        }
        for(int i = 0; i < sizes::Rows; i++){
            for(int j = 0; j < sizes::Columns; j++){
                //qDebug() << "(" << j << "," << i << ")" << this->ptrMap[i][j]->type;
            }
        }

        //設定哪個ItemOnMap可以有player;
        ptrMap[this->player1->getY()][this->player1->getX()]->setPlayerOnMap(1, this->player1);
        ptrMap[this->player2->getY()][this->player2->getX()]->setPlayerOnMap(2, this->player2);

    }else{//讀檔
        QFile file("record.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Cannot open file for reading:" << qPrintable(file.errorString());
            return;
        }

        QTextStream record_file(&file);
        int i = 0;
        int j = 0;
        while (!record_file.atEnd()){
            QStringList fileLine = record_file.readLine().split(" ");;
            if (get_record_counter==0){
                //遊戲時間
                game_timer_index = fileLine[0].toInt();
                qDebug()<<game_timer_index;
                get_record_counter++;
            }
            else if(get_record_counter==1){
                int x1 = fileLine[0].toInt();
                int y1 = fileLine[1].toInt();
                int h1 = fileLine[2].toInt();
                int m1 = fileLine[3].toInt();
                int r1 = fileLine[4].toInt();
                player1 = new Player(x1, y1, 1);
                player1->health=h1;
                player1->maxBombs=m1;
                player1->range=r1;
                get_record_counter++;
            }else if(get_record_counter==2){
                int x1 = fileLine[0].toInt();
                int y1 = fileLine[1].toInt();
                int h1 = fileLine[2].toInt();
                int m1 = fileLine[3].toInt();
                int r1 = fileLine[4].toInt();
                player2 = new Player(x1, y1, 2);
                player2 = new Player(x1, y1, 2);
                player2->health=h1;
                player2->maxBombs=m1;
                player2->range=r1;
                get_record_counter++;

                player1->rememberOppo(player2);
                player2->rememberOppo(player1);
                /*
                ui->player1_Range->setText(QString::number(player1->getRange()));
                ui->player2_Range->setText(QString::number(player2->getRange()));
                ui->player1_Bombs->setText(QString::number(player1->getMaxBombs()));
                ui->player2_Bombs->setText(QString::number(player2->getMaxBombs()));
                */



                //ui->game_timer_Label->setStyleSheet("QLabel { background-color : white; color : black; }");



                //把上面建構的都加入進scene
                game_scene->addItem(player1);
                game_scene->addItem(player2);


            }
            else{
                //建造場景
                if(i < sizes::Rows){
                    if (j < sizes::Columns){
                        this->ptrMap[i][j] = new ItemOnMap(j , i);
                        if (fileLine[0].toInt()==1){
                            this->ptrMap[i][j]->setEmpty();
                            game_scene->addItem(ptrMap[i][j]);
                            j++;
                        }else if (fileLine[1].toInt()==1){
                            //this->ptrMap[i][j]->type = Undestroyableblock;
                            this->ptrMap[i][j]->setUndestroyableblock();
                            game_scene->addItem(ptrMap[i][j]);
                            j++;
                        }else if (fileLine[2].toInt()==1) {
                            //this->ptrMap[i][j]->type = Destroyableblock;
                            this->ptrMap[i][j]->setDestroyableblock();
                            game_scene->addItem(ptrMap[i][j]);
                            j++;
                        }else if (fileLine[3].toInt()==1){
                            //this->ptrMap[i][j]->type = Bomb;
                            this->ptrMap[i][j]->setBomb();
                            game_scene->addItem(ptrMap[i][j]);
                            j++;
                        }else if (fileLine[4].toInt()==1){
                            //this->ptrMap[i][j]->type = Increaserangeprop;
                            this->ptrMap[i][j]->setIncreaserangeprop();
                            game_scene->addItem(ptrMap[i][j]);
                            j++;
                        }else if (fileLine[5].toInt()==1){
                            //this->ptrMap[i][j]->type = Increasemaxbombsprop;
                            this->ptrMap[i][j]->setIncreasemaxbombsprop();
                            game_scene->addItem(ptrMap[i][j]);
                            j++;
                        }else if (fileLine[6].toInt()==1) {
                            //this->ptrMap[i][j]->type = Superstarprop;
                            this->ptrMap[i][j]->setSuperstarprop();
                            game_scene->addItem(ptrMap[i][j]);
                            j++;
                        }else {
                            //this->ptrMap[i][j]->type = Dizzyprop;
                            this->ptrMap[i][j]->setDizzyprop();
                            game_scene->addItem(ptrMap[i][j]);
                            j++;
                        }
                        if (j==sizes::Columns){
                            j=0;
                            i++;
                        }
                    }
                }
                get_record_counter++;
            }
        }
    }

    //建造場景
    game_ui_background = new QGraphicsPixmapItem();
    game_ui_background->setPixmap(QPixmap(":/img/img/game_ui_background_1.png"));
    //game_ui_background->setZValue(-2);
    game_scene->addItem(game_ui_background);

    QGraphicsPixmapItem*backland = new QGraphicsPixmapItem();
    backland->setPixmap(QPixmap(":/img/img/backland_1.png"));
    backland->setOffset(sizes::Strat_pixel_x-10, sizes::Strat_pixel_y-10);
    backland->setZValue(-1);
    game_scene->addItem(backland);

    //暈眩頭環
    player1_dizzy_circle = new QGraphicsPixmapItem();
    player1_dizzy_circle->setPos(sizes::Strat_pixel_x + player1->getX() * sizes::FieldSize, sizes::Strat_pixel_y + player1->getY() * sizes::FieldSize);
    player1_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_1.png"));
    game_scene->addItem(player1_dizzy_circle);
    player2_dizzy_circle = new QGraphicsPixmapItem();
    player2_dizzy_circle->setPos(sizes::Strat_pixel_x + player2->getX() * sizes::FieldSize, sizes::Strat_pixel_y + player2->getY() * sizes::FieldSize);
    player2_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_1.png"));
    game_scene->addItem(player2_dizzy_circle);

    //玩家數據
    player1_blood = new QGraphicsPixmapItem();
    player2_blood = new QGraphicsPixmapItem();
    player1_blood->setPos(975,116);
    player2_blood->setPos(975,381);
    player1_blood->setPixmap(QPixmap(":/img/img/blood_3.png"));
    player2_blood->setPixmap(QPixmap(":/img/img/blood_3.png"));
    game_scene->addItem(player1_blood);
    game_scene->addItem(player2_blood);

    minute_label = new QGraphicsPixmapItem();
    minute_label->setPos(915,630);
    minute_label->setPixmap(QPixmap(":/img/img/3.png"));
    //minute_label->setZValue(3);
    tenSec_label = new QGraphicsPixmapItem();
    tenSec_label->setPos(950,630);
    tenSec_label->setPixmap(QPixmap(":/img/img/0.png"));
    sec_label = new QGraphicsPixmapItem();
    sec_label->setPos(980,630);
    sec_label->setPixmap(QPixmap(":/img/img/0.png"));
    game_scene->addItem(minute_label);
    game_scene->addItem(tenSec_label);
    game_scene->addItem(sec_label);



    ui->player1_Range->setText(QString::number(player1->getRange()));
    ui->player2_Range->setText(QString::number(player2->getRange()));
    ui->player1_Bombs->setText(QString::number(player1->getMaxBombs()));
    ui->player2_Bombs->setText(QString::number(player2->getMaxBombs()));
    ui->player1_Score->setText(QString::number(player1->getScore()));
    ui->player2_Score->setText(QString::number(player2->getScore()));


    ui->player1_Range->setStyleSheet("QLabel {color : white; }");
    ui->player2_Range->setStyleSheet("QLabel {color : white; }");
    ui->player1_Bombs->setStyleSheet("QLabel {color : white; }");
    ui->player2_Bombs->setStyleSheet("QLabel {color : white; }");
    ui->player1_Score->setStyleSheet("QLabel {color : white; }");
    ui->player2_Score->setStyleSheet("QLabel {color : white; }");

    //game_Timer
    game_timer = new QTimer(this); //
    connect(game_timer, SIGNAL(timeout()), this, SLOT(game_timer_update()));
    game_timer->start(1000);

    //UI Timer
    ui_timer = new QTimer(this); // 時間到會結束遊戲也是這個
    connect(ui_timer, SIGNAL(timeout()), this, SLOT(ui_timer_update()));
    ui_timer->start();

    //dizzy_circle_timer
    dizzy_circle_timer = new QTimer(this); // 時間到會結束遊戲也是這個
    connect(dizzy_circle_timer, SIGNAL(timeout()), this, SLOT(dizzy_circle_timer_update()));
    dizzy_circle_timer->start(100);

    //game_ui_background_timer
    game_ui_background_timer = new QTimer(this); //
    connect(game_ui_background_timer, SIGNAL(timeout()), this, SLOT(game_ui_background_timer_update()));
    game_ui_background_timer->start(100);

    //連接玩家死亡
    connect(player1, SIGNAL(player_dead()), this, SLOT(player1_dead_func()));
    connect(player2, SIGNAL(player_dead()), this, SLOT(player2_dead_func()));

    game_music = new QAudioOutput();
    game_music->setVolume(50);
    game_music_Player = new QMediaPlayer();
    game_music_Player->setAudioOutput(game_music);
    game_music_Player->setSource(QUrl("qrc:/sound/sound/game_music.mp3"));
    game_music_Player->play();
}



Game::~Game()
{
    delete ui;
    delete game_scene;
}

void Game::game_timer_update()
{
    game_timer_index--;
    minute_label->setPixmap(QPixmap(":/img/img/"+QString::number(game_timer_index/60)+".png"));
    tenSec_label->setPixmap(QPixmap(":/img/img/"+QString::number(game_timer_index%60/10)+".png"));
    sec_label->setPixmap(QPixmap(":/img/img/"+QString::number(game_timer_index%60%10)+".png"));

    QString time_str;
    if(game_timer_index%60 >= 10){
        time_str = QString::number(game_timer_index/60) + " : " + QString::number(game_timer_index%60);
    }else{
        time_str = QString::number(game_timer_index/60) + " : 0" + QString::number(game_timer_index%60);
    }

    //ui->game_timer_Label->setText(time_str);

    if(game_timer_index == 0){
        qDebug() << "End Game for time is up.";
        //QTimer::singleShot(2000, this, SLOT(QApplication_exit()));//
        End *e;
        if(player1->getScore() >= player2->getScore())
            e = new End(1);
        else
            e = new End(2);
        e->show();
        this->hide();

    }
//    if(this->player1->health==0 || this->player1->health==0){
//        qDebug() << "End Game for one player winning.";
//        QTimer::singleShot(2000, this, SLOT(QApplication_exit()));//
//    }
}

void Game::ui_timer_update()
{

    //ui->player1_HP->setText("HP : " + QString::number(player1->getHealth()));
    //ui->player2_HP->setText("HP : " + QString::number(player2->getHealth()));

    ui->player1_Range->setText(QString::number(player1->getRange()));
    ui->player2_Range->setText(QString::number(player2->getRange()));
    ui->player1_Bombs->setText(QString::number(player1->getMaxBombs()));
    ui->player2_Bombs->setText(QString::number(player2->getMaxBombs()));
    ui->player1_Score->setText(QString::number(player1->getScore()));
    ui->player2_Score->setText(QString::number(player2->getScore()));

    player1_dizzy_circle->setPos(sizes::Strat_pixel_x + player1->getX() * sizes::FieldSize, sizes::Strat_pixel_y + player1->getY() * sizes::FieldSize - 10);
    player2_dizzy_circle->setPos(sizes::Strat_pixel_x + player2->getX() * sizes::FieldSize, sizes::Strat_pixel_y + player2->getY() * sizes::FieldSize - 10);

    if(player1->getHealth() == 3){
        player1_blood->setPixmap(QPixmap(":/img/img/blood_3.png"));
    }else if(player1->getHealth() == 2){
        player1_blood->setPixmap(QPixmap(":/img/img/blood_2.png"));
    }else if(player1->getHealth() == 1){
        player1_blood->setPixmap(QPixmap(":/img/img/blood_1.png"));
    }else{
        player1_blood->setPixmap(QPixmap(""));
    }

    if(player2->getHealth() == 3){
        player2_blood->setPixmap(QPixmap(":/img/img/blood_3.png"));
    }else if(player2->getHealth() == 2){
        player2_blood->setPixmap(QPixmap(":/img/img/blood_2.png"));
    }else if(player2->getHealth() == 1){
        player2_blood->setPixmap(QPixmap(":/img/img/blood_1.png"));
    }else{
        player2_blood->setPixmap(QPixmap(""));
    }


}

void Game::dizzy_circle_timer_update()
{
    dizzy_circle_timer_index++;
    if(player1->getDizzyState() > 0){
        if(dizzy_circle_timer_index%4 == 0){
            player1_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_1.png"));
        }
        else if(dizzy_circle_timer_index%4 == 1){
            player1_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_2.png"));
        }
        else if(dizzy_circle_timer_index%4 == 2){
            player1_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_3.png"));
        }
        else{
            player1_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_4.png"));
        }
    }else{
        player1_dizzy_circle->setPixmap(QPixmap(""));
    }

    if(player2->getDizzyState() > 0){
        if(dizzy_circle_timer_index%4 == 0){
            player2_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_1.png"));
        }
        else if(dizzy_circle_timer_index%4 == 1){
            player2_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_2.png"));
        }
        else if(dizzy_circle_timer_index%4 == 2){
            player2_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_3.png"));
        }
        else{
            player2_dizzy_circle->setPixmap(QPixmap(":/img/img/dizzy_circle_4.png"));
        }
    }else{
        player2_dizzy_circle->setPixmap(QPixmap(""));
    }
}

void Game::game_ui_background_timer_update(){
    game_ui_background_timer_index++;
    game_ui_background->setPixmap(QPixmap(":/img/img/game_ui_background_" + QString::number(game_ui_background_timer_index%18 + 1) + ".png"));
}

void Game::player1_dead_func(){
    qDebug() << "player1 dead, player2 win!!";
    //QTimer::singleShot(5000, this, SLOT(nothing()));//
    End *e = new End(2);
    e->show();
    this->hide();
}

void Game::player2_dead_func(){
    qDebug() << "player2 dead, player1 win!!";
    //QTimer::singleShot(5000, this, SLOT(nothing()));//
    End *e = new End(1);
    e->show();
    this->hide();
}

void Game::QApplication_exit(){
    QApplication::exit();
}

void Game::nothing()
{
  return;
}

//有BUG 要先按到上下左右見以外的案件才能生效 推估跟移動視窗有關
void Game::keyPressEvent(QKeyEvent *event) {
    setFocus();
    //player1 control
    switch (event->key()) {
        case Qt::Key_A:
            if(player1->getDizzyState() == 0)
                player1->move(Left, 1, this->ptrMap);
            else
                player1->move(Right, 1, this->ptrMap);
            break;
        case Qt::Key_D:
            if(player1->getDizzyState() == 0)
                player1->move(Right, 1, this->ptrMap);
            else
                player1->move(Left, 1, this->ptrMap);
            break;
        case Qt::Key_S:
            if(player1->getDizzyState() == 0)
                player1->move(Down, 1, this->ptrMap);
            else
                player1->move(Up, 1, this->ptrMap);
            break;
        case Qt::Key_W:
            if(player1->getDizzyState() == 0)
                player1->move(Up, 1, this->ptrMap);
            else
                player1->move(Down, 1, this->ptrMap);
            break;
        case Qt::Key_Space:
            if(player1->getPlantedBomb() < player1->getMaxBombs()){
                player1->plantBomb(this->ptrMap); //////////////////
            }
            break;
    }
    //player2 control
    switch (event->key()) {
        case Qt::Key_J:
            if(player2->getDizzyState() == 0)
                player2->move(Left, 2, this->ptrMap);
            else
                player2->move(Right, 2, this->ptrMap);
            break;
        case Qt::Key_L:
            if(player2->getDizzyState() == 0)
                player2->move(Right, 2, this->ptrMap);
            else
                player2->move(Left, 2, this->ptrMap);
            break;
        case Qt::Key_K:
            if(player2->getDizzyState() == 0)
                player2->move(Down, 2, this->ptrMap);
            else
                player2->move(Up, 2, this->ptrMap);
            break;
        case Qt::Key_I:
            if(player2->getDizzyState() == 0)
                player2->move(Up, 2, this->ptrMap);
            else
                player2->move(Down, 2, this->ptrMap);
            break;
        case Qt::Key_P:
            if(player2->getPlantedBomb() < player2->getMaxBombs()){
                player2->plantBomb(this->ptrMap); //////////////////
            }
            qDebug() << player2->getPlantedBomb() << player2->getMaxBombs();
            break;
    }
}

void Game::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file for writing:" << qPrintable(file.errorString());
        return;
    }

    QTextStream out(&file);
    out << game_timer_index<< "\n";
    qDebug()<<game_timer_index;
    out << player1->x << " " << player1->y << " " << player1->health << " " << player1->maxBombs << " " << player1->range << "\n";
    out << player2->x << " " << player2->y << " " << player2->health << " " << player2->maxBombs << " " << player2->range << "\n";
    for (int i = 0; i < sizes::Rows; i++) {
        for(int j = 0; j < sizes::Columns; j++) {
            if (this->ptrMap[i][j]->type == Empty) {
                out << QObject::tr("1") << " ";
            }else{
                out << QObject::tr("0") << " ";
            }
            if (this->ptrMap[i][j]->type == Undestroyableblock) {
                out << QObject::tr("1") << " ";
            }else{
                out << QObject::tr("0") << " ";
            }
            if (this->ptrMap[i][j]->type == Destroyableblock) {
                out << QObject::tr("1") << " ";
            }else{
                out << QObject::tr("0") << " ";
            }
            if (this->ptrMap[i][j]->type == Bomb) {
                out << QObject::tr("1") << " ";
            }else{
                out << QObject::tr("0") << " ";
            }
            if (this->ptrMap[i][j]->type == Increaserangeprop) {
                out << QObject::tr("1") << " ";
            }else{
                out << QObject::tr("0") << " ";
            }
            if (this->ptrMap[i][j]->type == Increasemaxbombsprop) {
                out << QObject::tr("1") << " ";
            }else{
                out << QObject::tr("0") << " ";
            }
            if (this->ptrMap[i][j]->type == Superstarprop) {
                out << QObject::tr("1") << " ";
            }else{
                out << QObject::tr("0") << " ";
            }
            if (this->ptrMap[i][j]->type == Dizzyprop) {
                out << QObject::tr("1");
            }else{
                out << QObject::tr("0");
            }
            out << QObject::tr("\n");
        }
    }
    file.close();
}

void Game::on_SaveAsButton_clicked()
{
    this->writeFile("record.txt");
}

void Game::on_PauseAsButton_clicked()
{
    this->writeFile("record.txt");
    Menu *menu = new Menu();
    menu->show();
    this->hide();
    delete this;
}

