#include "webtable.h"

WebTable::WebTable():
    m_QStrNameWhite("WHITE"),
    m_QStrNameBlack("BLACK")
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
