#ifndef END_SCENE_H
#define END_SCENE_H

#include <QWidget>

namespace Ui {
class End_Scene;
}

class End_Scene : public QWidget
{
    Q_OBJECT

public:
    explicit End_Scene(QWidget *parent = nullptr);
    ~End_Scene();

private:
    Ui::End_Scene *ui;
};

#endif // END_SCENE_H
