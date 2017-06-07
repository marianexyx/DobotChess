#include "webtable.h"

WebTable::WebTable():
    m_bGameInProgress(false),
    m_QStrNameWhite("Biały"),
    m_QStrNameBlack("Czarny")
{

}

void WebTable::checkWebsocketMsg(QString QstrWebsocketMsg)
{
    if (QstrWebsocketMsg.left(8) == "newWhite")
        m_QStrNameWhite = QstrWebsocketMsg.mid(9);
    else if (QstrWebsocketMsg.left(8) == "newBlack")
        m_QStrNameBlack = QstrWebsocketMsg.mid(9);
    else qDebug() << "ERROR: WebTable::checkWebsocketMsg: unknown parameter";
}
