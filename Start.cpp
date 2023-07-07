#include "Start.h"
#include "ui_Start.h"

#include "Game.h"


Start::Start(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Start)
{
    this->start_scene=new Scene;
    ui->setupUi(this);

    start_scene->setSceneRect(0,0,1161,794); //550,650
    ui->graphicsView->setScene(start_scene);
    ui->graphicsView->setFixedSize(1161,794);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //window的標題和圖示
    setWindowTitle(tr("Bomberman"));
    setWindowIcon(QPixmap(":img/img/bomb.png"));

    QMovie *movie = new QMovie(":/gif/gif/Yo.gif");
    ui->label->setMovie(movie);
    movie->start();

    QGraphicsPixmapItem*game_ui_background = new QGraphicsPixmapItem;
    game_ui_background->setPixmap(QPixmap(":/img/img/game_ui_background_1.png"));
    start_scene->addItem(game_ui_background);
    game_ui_background->setZValue(-1);

    start_music = new QAudioOutput();
    start_music->setVolume(50);
    start_music_Player = new QMediaPlayer();
    start_music_Player->setAudioOutput(start_music);
    start_music_Player->setSource(QUrl("qrc:/sound/sound/start_music.mp3"));
    start_music_Player->play();
}

Start::~Start()
{
    delete ui;
}

void Start::on_NewAsButton_clicked()
{
    start_music_Player->pause();
    Game *game = new Game(0);
    game->show();
    this->hide();
    //delete this;
}

void Start::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file for reading:" << qPrintable(file.errorString());
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QStringList fileLine = in.readLine().split(" ");
        qDebug() << fileLine;
        qDebug() << fileLine[0];
    }
    file.close();
}


void Start::on_RecordAsButton_clicked()
{
    start_music_Player->pause();
    Game *game = new Game(1);;
    game->show();
    this->hide();

}

