#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QKeyEvent>
#include <random>
#include <QGraphicsPixmapItem>
#include <QObject>


#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include "Scene.h"
#include "Player.h"
#include "Setting.h"
#include "ItemOnMap.h"
#include "Menu.h"
#include "End.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Game; }
QT_END_NAMESPACE

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(int isBuildNewGame=1, QWidget *parent = nullptr);
    ~Game();

public slots:
    void game_timer_update();
    void ui_timer_update();
    void dizzy_circle_timer_update();
    void game_ui_background_timer_update();

    void player1_dead_func();
    void player2_dead_func();

    void QApplication_exit();
    void nothing();

signals:
    void timesUp_end();

private slots:
    void on_SaveAsButton_clicked();

    void on_PauseAsButton_clicked();

private:
    Ui::Game *ui;
    Scene *game_scene;

    void keyPressEvent(QKeyEvent *event);

    Player *player1;
    Player *player2;
    ItemOnMap* ptrMap[sizes::Rows][sizes::Columns];

    QTimer*game_timer;
    QTimer*ui_timer;
    QTimer*dizzy_circle_timer;
    QTimer*game_ui_background_timer;

    int game_timer_index = 0;
    int dizzy_circle_timer_index = 0;
    int game_ui_background_timer_index = 0;

    void writeFile(const QString &fileName);
    void readFile();

    QGraphicsPixmapItem*player1_dizzy_circle;
    QGraphicsPixmapItem*player2_dizzy_circle;

    QGraphicsPixmapItem*player1_blood;
    QGraphicsPixmapItem*player2_blood;

    QGraphicsPixmapItem*minute_label;
    QGraphicsPixmapItem*tenSec_label;
    QGraphicsPixmapItem*sec_label;

    QGraphicsPixmapItem*game_ui_background;

    QAudioOutput*game_music;
    QMediaPlayer*game_music_Player;

};
#endif // GAME_H
