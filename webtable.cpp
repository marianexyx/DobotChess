#include "webtable.h"

WebTable::WebTable():
    _bGameInProgress(false),
    _QSNameWhite("Biały"),
    _QSNameBlack("Czarny"),
    _QSWhoseTurn("no_turn")
{

}

void WebTable::checkWebsocketMsg(QString QstrWebsocketMsg)
{
    if (QstrWebsocketMsg.left(17) == "white_player_name")
        _QSNameWhite = QstrWebsocketMsg.mid(18);
    else if (QstrWebsocketMsg.left(17) == "black_player_name")
        _QSNameBlack = QstrWebsocketMsg.mid(18);
    else if (QstrWebsocketMsg.left(10) == "whose_turn")
        _QSWhoseTurn = QstrWebsocketMsg.mid(11);
    else qDebug() << "ERROR: wrong unknown parameter in WebTable::checkWebsocketMsg";
}
