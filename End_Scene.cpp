#include "End_Scene.h"
#include "ui_end.h"

End_Scene::End_Scene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::End_Scene)
{
    ui->setupUi(this);
}

End_Scene::~End_Scene()
{
    delete ui;
}
