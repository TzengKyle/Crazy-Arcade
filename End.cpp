#include "End.h"
#include "ui_End.h"

#include <QObject>
#include <QMediaPlayer>



End::End(int a, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::End)
{
    ui->setupUi(this);
    this->scene=new Scene;

    if(a==1){
        QImage image(QString(":/img/img/one_wins.png"));
        ui->label->setPixmap(QPixmap::fromImage(image));
        ui->label->resize(image.width(),image.height());

    }else{
        QImage image(QString(":/img/img/two_wins.png"));
        ui->label->setPixmap(QPixmap::fromImage(image));
        ui->label->resize(image.width(),image.height());
    }

    //window的標題和圖示
    setWindowTitle(tr("Result"));
    setWindowIcon(QPixmap(":img/img/bomb.png"));

    end_music = new QAudioOutput();
    end_music->setVolume(50);
    end_music_Player = new QMediaPlayer();
    end_music_Player->setAudioOutput(end_music);
    end_music_Player->setSource(QUrl("qrc:/sound/sound/end_music.mp3"));
    end_music_Player->play();
}

End::~End()
{
    delete ui;
}
