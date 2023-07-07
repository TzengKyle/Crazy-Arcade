#include "Menu.h"
#include "ui_Menu.h"

#include "Game.h"
Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    //window的標題和圖示
    setWindowTitle(tr("Bomberman"));
    setWindowIcon(QPixmap(":img/img/bomb.png"));
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_ContinueAsButton_clicked()
{
    Game *game = new Game(1);
    game->show();
    this->hide();
}

void Menu::on_EndAsButton_clicked()
{
    //this->hide();
    QApplication::exit();
}
