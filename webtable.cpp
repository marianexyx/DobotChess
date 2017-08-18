#include "webtable.h"

WebTable::WebTable():
    m_QStrNameWhite("WHITE"),
    m_QStrNameBlack("BLACK")
{
    m_bIsWhiteStartClicked = false;
    m_bIsBlackStartClicked = false;
}

void WebTable::checkWebsocketMsg(QString QstrWebsocketMsg)
{
    if (QstrWebsocketMsg.left(8) == "newWhite")
        m_QStrNameWhite = QstrWebsocketMsg.mid(9);
    else if (QstrWebsocketMsg.left(8) == "newBlack")
        m_QStrNameBlack = QstrWebsocketMsg.mid(9);
    else qDebug() << "ERROR: WebTable::checkWebsocketMsg: unknown parameter";
}

void  WebTable::addPlayerToQueue(queuedPlayer playerToQueue)
{ //todo: zabronić w kolejce takich samych gniazd i nazw graczy
    if (m_QStrNameWhite != "WHITE" && m_QStrNameBlack != "BLACK")
        m_playersQueue << playerToQueue;
    else qDebug() << "ERROR: WebTable::addPlayerToQueue: table isn't full";
}

void WebTable::removePlayerFromQueue(QString playerToRemove)
{
    if (!m_playersQueue.isEmpty())
    {
        int nStructIter = 0; //TODO: kiepsko i naookoło omijam problem nieogarniania przeciążania
        //...operatorów przy removeOne - http://doc.qt.io/qt-5/qlist.html#removeOne
        foreach (const queuedPlayer &playerKey, m_playersQueue)
        {
            nStructIter++;
            if (playerToRemove == playerKey.playerName)
            {
                if (nStructIter >= 0 && nStructIter < m_playersQueue.size())
                {
                    m_playersQueue.removeAt(nStructIter);
                    break;
                }
                else qDebug() << "ERROR: wrong m_playersQueue.size():" << m_playersQueue.size();
            }
        }
    }
}

QString WebTable::getQueuedPlayers()
{
    QString queueListAsQString = "";

    if (!m_playersQueue.isEmpty())
    {
        queuedPlayer playerKey;
        foreach (playerKey, m_playersQueue)
        {
            queueListAsQString += playerKey.playerName + ",";
            queueListAsQString.remove(queueListAsQString.length() - 1); //usuń ostatni przecinek
        }
    }
    else queueListAsQString = "janusz,bogdan,stefan"; //"queueEmpty";

    return queueListAsQString;
}
