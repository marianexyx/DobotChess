#include "webtable.h"

WebTable::WebTable():
    m_bGameInProgress(false),
    m_QStrNameWhite("Biały"),
    m_QStrNameBlack("Czarny")
{

}

void WebTable::checkWebsocketMsg(QString QstrWebsocketMsg)
{
    if (QstrWebsocketMsg.left(17) == "white_player_name")
        m_QStrNameWhite = QstrWebsocketMsg.mid(18);
    else if (QstrWebsocketMsg.left(17) == "black_player_name")
        m_QStrNameBlack = QstrWebsocketMsg.mid(18);
    else qDebug() << "ERROR: unknown parameter in WebTable::checkWebsocketMsg";
}
