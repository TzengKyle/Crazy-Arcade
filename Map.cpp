#include "Map.h"

Map::Map(QObject *parent): QObject(parent)
{
    for(int i = 0; i < sizes::Rows; i++){
        for(int j = 0; j < sizes::Columns; j++){
            this->mapAx[i][j] = 0;
        }
    }
    player1_pos[0] = 0;
    player1_pos[1] = 0;
    player2_pos[0] = sizes::Columns - 1;
    player2_pos[1] = sizes::Rows - 1;


    setPos(0,0);
    setPixmap(QPixmap(":/img/img/background.png"));
}
//還沒寫
void Map::updateMapAx(int x, int y, int value){
    //可能要在想遺下
    //int oriVal = mapAx[y][x];
    mapAx[y][x] = value;
    //qDebug() << "mapAx: (" << x << "," << y << ") turn to" << value << "from" << oriVal << ".";
}

void Map::updatePlayerPos(int player, int x, int y){
    if(player == 1){
        //int ori_x = this->player1_pos[0], ori_y = this->player1_pos[1];
        player1_pos[0] = x;
        player1_pos[1] = y;
        //qDebug() << "player1_pos: turn to (" << x << "," << y << ") from" << "(" << ori_x << "," << ori_y << ").";
    }else{
        //int ori_x = this->player2_pos[0], ori_y = this->player2_pos[1];
        player2_pos[0] = x;
        player2_pos[1] = y;
        //qDebug() << "player2_pos: turn to (" << x << "," << y << ") from" << "(" << ori_x << "," << ori_y << ").";
    }

}
