#include "websockets.h"

Websockets::Websockets(WebTable *pWebTable, quint16 port, QObject *parent):
    QObject(parent),
    m_clients()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this); //nowy serwer websocket
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) //niech ws serwer nasłuchuje na tym porcie
    {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        qDebug() << "WebSocket server listening on port" << port;
        emit addTextToWebsocketConsole("WebSocket server listening on port" + port);
    }

    _pWebTable = pWebTable;
}

Websockets::~Websockets()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void Websockets::onNewConnection() //nowe połączenia
{
    qDebug() << "Websockets::onNewConnection() \n";
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection(); //gniazdo do komunikacji...
    //...z danym klientem (obiekty QWebSocket są właśnie gniazdami)

    //jeżeli socket dostanie wiadomość, to odpalamy metody przetwarzająca ją...
    //...sygnał textMessageReceived emituje QStringa do processWebsocketMsg
    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::processWebsocketMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected); //a jak disconnect, to disconnect
    m_clients << pSocket;
    emit addTextToWebsocketConsole("New connection \n");
}

//TODO: *wszystkie wiadomośći które przychodzą ze strony powinny być opatrzone identyfikatorem tego
//kto je wysyła: gracz biały, czarny, bądź niegracz.

//przetwarzanie wiadomośći otrzymanej przez websockety
void Websockets::processWebsocketMsg(QString QsWsMsgToProcess)
{
    qDebug() << "Websockets::processWebsocketMsg received: " << QsWsMsgToProcess << "/n";
    QString QsWebsocketConsoleMsg; //wiadomość na konsolę
    //QWebSocket *pSender = qobject_cast<QWebSocket *>(sender()); //bodajże (bardzo w skrócie): zestaw...
    //...(przypisz) aktualnie otrzymaną wiadomość z tcp do tego gniazda (stwórz gniazdo tymczasowe/temp)
    //for (QWebSocket *pClient : qAsConst(m_clients)) //np. następnie przeleć przez wszystkich...
    //...aktualnych klientów, czyli wszystkie otwarte stałe gniazda przynależne do użytkownika
    //if (pClient == pSender) //i wykonaj funkcje tylko pdo warunkiem, gdzie aktualna tymczasowa...
    //...wiadomość otrzymana websocketami z WWW (gniazdo) jest takie same jak obiekt gniazda...
    //...stałego; otwartego przynależnego do danego użytkownika strony
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());


    //wiadomość pójdzie tylko do tego kto ją przysłał
    if (pClient && QsWsMsgToProcess == "keepConnected")
        pClient->sendTextMessage("connectionOnline"); //podtrzymanie połączenia
    else if (QsWsMsgToProcess == "new" || QsWsMsgToProcess.left(4) == "move" ||
             QsWsMsgToProcess.left(10) == "promote_to" || QsWsMsgToProcess == "reset")
    {
        this->sendToChess(QsWsMsgToProcess);
    }
    else if (QsWsMsgToProcess.left(5) == "check")
    {
        if (QsWsMsgToProcess.mid(6) == "white_player")
        {
            qDebug() << "Sending to website: checked_wp_is " << _pWebTable->getNameWhite();
            QsWebsocketConsoleMsg = "Sending to website: checked_wp_is " + _pWebTable->getNameWhite();
            pClient->sendTextMessage("checked_wp_is " + _pWebTable->getNameWhite());
        }
        else if (QsWsMsgToProcess.mid(6) == "black_player")
        {
            qDebug() << "Sending to website: checked_bp_is " << _pWebTable->getNameBlack();
            QsWebsocketConsoleMsg = "Sending to website: checked_bp_is " + _pWebTable->getNameBlack();
            pClient->sendTextMessage("checked_bp_is " + _pWebTable->getNameBlack());
        }
        else if (QsWsMsgToProcess.mid(6) == "whose_turn")
        {
            qDebug() << "Sending to website: checked_wt_is " << _pWebTable->getWhoseTurn();
            QsWebsocketConsoleMsg = "Sending to website: checked_wt_is " + _pWebTable->getWhoseTurn();
            pClient->sendTextMessage("checked_wt_is " + _pWebTable->getWhoseTurn());
        }
        else
        {
            qDebug() << "Error: wrong check value.";
            QsWebsocketConsoleMsg = "Error: wrong check value.";
        }
    }
    else
    {
        //Q_FOREACH (QWebSocket *pClient, m_clients) //to jest zapis który wkrótce podobno będzie deprecated
        for (QWebSocket *pClient : qAsConst(m_clients)) //dla każdego klienta kontenera m_clients wykonaj
            //TODO: gracze dostają komunikatów gracza przeciwnego. naprawić to (przy BAD_MOVE jest dłuższy komentarz)
        {
            if (QsWsMsgToProcess.left(17) == "white_player_name")
                //TODO: sprawdzić czy emit nie działa skrajnie asynchronicznie, tj. będzie próbowało...
                //... odczytać zmienną z get'era, podczas gry set'er w emit'ie nie uwstawił jeszcze nowej zmiennej...
                //... - zadać to pytanie na forum QT
            {
                emit MsgFromWebsocketsToWebtable(QsWsMsgToProcess); //zapamiętaj imię białego gracza
                qDebug() << "Echo back to website: new_white " << _pWebTable->getNameWhite();
                QsWebsocketConsoleMsg = "Echo back to website: new_white " + _pWebTable->getNameWhite();
                pClient->sendTextMessage("new_white " + _pWebTable->getNameWhite()); //wyślij do WS nową nazwę białego
            }
            else if (QsWsMsgToProcess.left(17) == "black_player_name")
            {
                emit MsgFromWebsocketsToWebtable(QsWsMsgToProcess); //zapamiętaj imię czarnego gracza
                qDebug() << "Echo back to website: new_black " << _pWebTable->getNameBlack();
                QsWebsocketConsoleMsg = "Echo back to website: new_black " + _pWebTable->getNameBlack();
                pClient->sendTextMessage("new_black " + _pWebTable->getNameBlack()); //wyślij do WS nową nazwę czarnego
            }
            else if (QsWsMsgToProcess.left(10) == "whose_turn") //?TODO: wiadomość wyskakuje 2 razy w qdebug
            {
                emit MsgFromWebsocketsToWebtable(QsWsMsgToProcess); //zapamiętaj czyja jest tura
                qDebug() << "Echo back to website: whose_turn " << _pWebTable->getWhoseTurn();
                QsWebsocketConsoleMsg = "Echo back to website: whose_turn " + _pWebTable->getWhoseTurn();
                pClient->sendTextMessage("whose_turn " + _pWebTable->getWhoseTurn()); //wyślij do websocketowców info o turze
            }
            else if (QsWsMsgToProcess == "new_game") //udało się rozpocząć nową grę. wyślij info o tym na WWW
            {
                qDebug() << "New game started. Sending to website: new_game";
                QsWebsocketConsoleMsg = "Sending to website: new_game";
                pClient->sendTextMessage("new_game");
            }
            else if (QsWsMsgToProcess == "white_won" || QsWsMsgToProcess == "black_won" ||
                     QsWsMsgToProcess == "draw") //koniec gry
            {
                qDebug() << "End of game. Sending to website: " << QsWsMsgToProcess;
                QsWebsocketConsoleMsg = "Sending to website: " + QsWsMsgToProcess;
                pClient->sendTextMessage(QsWsMsgToProcess);
            }
            else if (QsWsMsgToProcess == "promote_to_what") //test promocji ok. zapytaj się WWW na co promować pionka.
                //TODO: ten sam problem co w BAD_MOVE- nie wiem jak odpowiedzieć do danego gracza, więc...
                //...póki co odpowiadam wszsytkim, a oni będą filtrowali dane przychodzące.
            {
                qDebug() << " Sending to website: " << QsWsMsgToProcess;
                QsWebsocketConsoleMsg = "Sending to website: promote_to_what";
                pClient->sendTextMessage("promote_to_what");
            }
            else if (QsWsMsgToProcess.left(8) == "BAD_MOVE") //wiadomosc przychodzi tutaj przez klase 'chess'
                /* TODO: Muszę odpowiedzieć pytającemu graczowi że spierdolił zapytanie. By to zrobić muszę go odczytać
             * z systemu (znaleźć który to gracz w 'pClient'ach, bo tą wiadomość dostaję z tcp, a nie od gracza który
             * zrobił błąd. Najgorszą tymczasową opcją jest odpowiadanie wszsytkim użytkownikom websocketów,
             * a (TODO2:) na stronie przyjmowanie tej wiadomości tylko wtedy gdy jest tura gracza wykonującego ruch.
             * Trudniejszą i normalną opcją jest powiązanie websocketowego clienta z graczem białym lub czarnym, lub
             * czymś podobnym. Zostanę na razie przy półśrodku, ale trzeba to zmienić.*/
            {
                QString QsBadMove = QsWsMsgToProcess.mid(9);
                qDebug() << "Sending to website: BAD_MOVE...";
                QsWebsocketConsoleMsg = "Sending to website: BAD_MOVE " + QsBadMove.simplified();
                pClient->sendTextMessage("BAD_MOVE " + QsBadMove.simplified());
            }
            //TODO: powinien był odpowiadać tylko graczom, a nie wszystkim
            else if (QsWsMsgToProcess.left(16) == "game_in_progress") //gra w toku
            {
                qDebug() << "Sending to website: " << QsWsMsgToProcess;
                QsWebsocketConsoleMsg = "Sending to website: " + QsWsMsgToProcess;
                pClient->sendTextMessage(QsWsMsgToProcess);
            }
            else //jeżeli chenard da inną odpowiedź (nie powinien)
            {
                qDebug() << "Error11! Wrong msg from tcp: " << QsWsMsgToProcess;
                QsWebsocketConsoleMsg = "Error11! Wrong msg from tcp: " + QsWsMsgToProcess;
                pClient->sendTextMessage("Error11! Wrong msg from tcp: " + QsWsMsgToProcess);
            }
        }
        /* if (pClient == pSender) //jeżeli wiadomośc wpadła od klienta (tj.z sieci)
            {
            addTextToWebsocketConsole("pClient == pSender\n");
            addTextToWebsocketConsole("Received from web: " + QsWsMsgToProcess + "\n");

            if(QsWsMsgToProcess == "new" || QsWsMsgToProcess.left(4) == "move") //jeżeli są to wiadomości dla tcp
            {
            QS_chenardQuestion = QsWsMsgToProcess; //string do tcp będzie przekazana przez globalną zmienną
            doTcpConnect(); //łączy i rozłącza z tcp za każdym razem gdy jest wiadomość do przesłania
            }
            else //wiadomości o stanie stołu
            {
            if (QsWsMsgToProcess.left(17) == "white_player_name")
            {
            QS_nameWhite = QsWsMsgToProcess.mid(18);
            pClient->sendTextMessage("new_white " + QS_nameWhite); //wyślij do websocketowców nową nazwę białego
            addTextToWebsocketConsole("Send to web: new_white " + QS_nameWhite + "\n");
            }
            else if (QsWsMsgToProcess.left(17) == "black_player_name")
            {
            QS_nameBlack = QsWsMsgToProcess.mid(18);
            pClient->sendTextMessage("new_black " +QS_nameBlack); //wyślij do websocketowców nową nazwę czarnego
            addTextToWebsocketConsole("Send to web: new_black " + QS_nameBlack + "\n");
            }
            else addTextToWebsocketConsole("ERROR UNKNOW MESSAGE!\n");
            }
            }

            if (pClient != pSender) //jeżeli wiadomość jest wygenerowana przez serwer i leci na stronę
            {
            addTextToWebsocketConsole("pClient != pSender\n");
            if (QsWsMsgToProcess == "OK\n") //jeżeli chenard zaczął nową grę
            {
            pClient->sendTextMessage("new_game");// wyślij websocketem odpowiedź z tcp na stronę
            addTextToWebsocketConsole("Send to web: new_game\n");
            }
            else if (QsWsMsgToProcess == "OK 1\n") //jeżeli chenard wykonał ruch prawidłowo
            {
            QS_chenardQuestion = "status"; //zapytaj się tcp o stan gry
            addTextToWebsocketConsole("Send to tcp: status\n");
            doTcpConnect(); //wykonaj drugie zapytanie tcp
            }
            else
            {
            if (QsWsMsgToProcess.left(1) == "*") //gra w toku
            {
            pClient->sendTextMessage("game_in_progress");
            addTextToWebsocketConsole("Send to web: game_in_progress\n\n");
            }
            else if (QsWsMsgToProcess.left(3) == "1-0") //biały wygrał
            {
            pClient->sendTextMessage("white_won");
            addTextToWebsocketConsole("Send to web: white_won\n\n");
            }
            else if (QsWsMsgToProcess.left(3) == "0-1") //czarny wygrał
            {
            pClient->sendTextMessage("black_won");
            addTextToWebsocketConsole("Send to web: black_won\n\n");
            }
            else if (QsWsMsgToProcess.left(7) == "1/2-1/2") //remis
            {
            pClient->sendTextMessage("draw");
            addTextToWebsocketConsole("Send to web: draw\n\n");
            }
            else //jeżeli chenard da inną odpowiedź
            {
            pClient->sendTextMessage("error");
            addTextToWebsocketConsole("Send to web: error\n\n");
            }// wszystko to powyżej da się ładnie zapakować w 2 funkcje
            }
            }
        }*/
    }
    if (!QsWebsocketConsoleMsg.isEmpty())
        emit addTextToWebsocketConsole(QsWebsocketConsoleMsg + "\n");
    QsWebsocketConsoleMsg.clear();
}

void Websockets::sendToChess(QString QsMsgForChessClass)
{
    qDebug() << "Sending to 'chess' class: " << QsMsgForChessClass;
    QsWebsocketConsoleMsg = "Sending to 'chess' class: " + QsMsgForChessClass;
    emit MsgFromWebsocketsToChess(QsMsgForChessClass);
}


void Websockets::socketDisconnected() //rozłączanie websocketa
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
        emit addTextToWebsocketConsole("Disconnected\n");
    }
}
