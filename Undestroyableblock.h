#ifndef UNDESTROYABLEBLOCK_H
#define UNDESTROYABLEBLOCK_H

#include <QObject>
#include <QGraphicsPixmapItem>

class UnDestroyableBlock : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    UnDestroyableBlock(QObject *parent = nullptr);

signals:

};

#endif // UNDESTROYABLEBLOCK_H
