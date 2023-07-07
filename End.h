#ifndef END_H
#define END_H

#include <QWidget>
#include <QKeyEvent>
#include <random>
#include <QGraphicsPixmapItem>
#include <QObject>

#include <QGraphicsView>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "Scene.h"

namespace Ui {
class End;
}

class End : public QWidget
{
    Q_OBJECT

public:
    explicit End(int win=1,QWidget *parent = nullptr);
    ~End();

private:
    Ui::End *ui;
    Scene *scene;

    QAudioOutput*end_music;
    QMediaPlayer*end_music_Player;
};

#endif // END_H
