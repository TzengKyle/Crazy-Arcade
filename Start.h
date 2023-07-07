#ifndef START_H
#define START_H

#include <QMainWindow>
#include <QSplashScreen>
#include <QMovie>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>


#include "Game.h"
#include "Scene.h"
namespace Ui {
class Start;
}

class Start : public QMainWindow
{
    Q_OBJECT

public:
    Start(QWidget *parent = nullptr);
    ~Start();

private slots:
    void on_NewAsButton_clicked();

    void readFile(const QString &fileName);
    void on_RecordAsButton_clicked();

private:
    Ui::Start *ui;
    Scene *start_scene;

    QMovie *move;

    QAudioOutput*start_music;
    QMediaPlayer*start_music_Player;

    QGraphicsPixmapItem*game_ui_background;
};

#endif // START_H
