#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QGraphicsPixmapItem>


#include "Setting.h" //該.h檔可以幫忙改一些遊戲設定包括血量等等

class Map : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    //explicit Map(QObject *parent = nullptr);
    Map(QObject *parent = nullptr);
    void updateMapAx(int x, int y, int value);
    void updatePlayerPos(int player, int x, int y);

signals:

private:
    int mapAx[12][12];
    int player1_pos[2]; //(x,y)
    int player2_pos[2]; //(x,y)
};

#endif // MAP_H
