#include "websockets.h"

Websockets::Websockets(quint16 port, WebTable *pWebTable):
    m_pWebSocketServer(Q_NULLPTR), //przypisanie pustego wskaźnika
    m_clients()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);//nowy serwer websocket
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) //nasłuchuj na porcie
    {
        addTextToWebsocketConsole("Server listening on port " + QString::number(port) + "\n");
        //połącz slot nowego połączenia wbudowanej metody websocket z moją metodą onNewConnection
        //czyli jeżeli będzie nowe połączenie na websocketach, to odpali się funckja onNewConnection
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, onNewConnection);
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
    //!!! zacięcie czasami przy nowymn połączeniu !!! już nie?
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    //jeżeli socket dostanie wiadomość, to odpalamy metody przetwarzająca ją
    //sygnał textMessageReceived emituje QStringa do processWebsocketMsg
    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::processWebsocketMsg);

    //a jak disconnect, to disconnect
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);

    m_clients << pSocket;

    addTextToWebsocketConsole("New connection \n");
}


//TODO: podzielić funkcje poniższej metody na te które wymagają blokady zapytań tcp i te które tego
//nie robią. tu rozróżnić 2 opcje- jeżeli natrafmy na wiadomość dla tcp podczas gdy jedną wiadomość
//już przetwarzamy, to odrzucić tą wiadomość i zapisać error log w pliku oddzielnym do późniejszej
//analizy. jeżeli wiadomość jest dobra to powinna iść na tcp- docelowo poprzez kolejkę na bazie get-push.

//TODO: *wszystkie wiadomośći które przychodzą ze strony powinny być opatrzone identyfikatorem tego
//kto je wysyła: gracz biały, czarny, bądź niegracz.

//przetwarzanie wiadomośći otrzymanej przez websockety
void Websockets::processWebsocketMsg(QString QsWsMsgToProcess)
{
    QString QsWebsocketConsoleMsg; //wiadomość na konsolę
    //QWebSocket *pSender = qobject_cast<QWebSocket *>(sender()); // !! unused variable !! wykomentować?
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    // if (pClient) //!!! zrozumieć te mechanizmy
    //if (pClient != pSender) don't echo message back to sender

    //wiadomość pójdzie tylko do tego kto ją przysłał
    if (pClient && QsWsMsgToProcess == "keepConnected") pClient->sendTextMessage("connectionOnline"); //podtrzymanie połączenia
    else if (QsWsMsgToProcess == "new" || QsWsMsgToProcess.left(4) == "move" ||
             QsWsMsgToProcess.left(10) == "promote_to")
    {
        QsWebsocketConsoleMsg = "Send to 'chess' class: " + QsWsMsgToProcess; //TODO: naprawić text
        qDebug() << "Sending to 'chess' class: " << QsWsMsgToProcess;
        emit MsgFromWebsocketsToChess(QsWsMsgToProcess);
    }
    else if (QsWsMsgToProcess == "game_in_progress") //gra w toku
    {
        QsWebsocketConsoleMsg = "game_in_progress ";
        qDebug() << "Sending to site: game_in_progress";
        pClient->sendTextMessage("game_in_progress ");
    }
    else if (QsWsMsgToProcess.left(5) == "check")
    {
        if (QsWsMsgToProcess.mid(6) == "white_player")
        {
            QsWebsocketConsoleMsg = "checked_wp_is " + _pWebTable->getNameWhite();
            pClient->sendTextMessage("checked_wp_is " + _pWebTable->getNameWhite());
        }
        else if (QsWsMsgToProcess.mid(6) == "black_player")
        {
            QsWebsocketConsoleMsg = "checked_bp_is " + _pWebTable->getNameBlack();
            pClient->sendTextMessage("checked_bp_is " + _pWebTable->getNameBlack());
        }
        else if (QsWsMsgToProcess.mid(6) == "whose_turn")
        {
            QsWebsocketConsoleMsg = "checked_wt_is " + _pWebTable->getWhoseTurn();
            pClient->sendTextMessage("checked_wt_is " + _pWebTable->getWhoseTurn());
        }
        else
        {
            QsWebsocketConsoleMsg = "error: wrong check value.";
            qDebug() << "error: wrong check value.";
        }
    }
    else
    {
        Q_FOREACH (QWebSocket *pClient, m_clients) //dla każdego klienta wykonaj
        //TODO: gracze dostają komunikatów gracza przeciwnego. naprawić to (przy BAD_MOVE jest dłuższy komentarz)
        {
            if (QsWsMsgToProcess.left(17) == "white_player_name")
                //TODO: sprawdzić czy emit nie działa skrajnie asynchronicznie, tj. będzie próbowało...
                //... odczytać zmienną z get'era, podczas gry set'er w emit'ie nie uwstawił jeszcze nowej zmiennej.
            {
                emit MsgFromWebsocketsToWebtable(QsWsMsgToProcess); //zapamiętaj imię białego gracza
                QsWebsocketConsoleMsg = "new_white " + _pWebTable->getNameWhite();
                pClient->sendTextMessage("new_white " + _pWebTable->getNameWhite()); //wyślij do websocketowców nową nazwę białego
            }
            else if (QsWsMsgToProcess.left(17) == "black_player_name")
            {
                emit MsgFromWebsocketsToWebtable(QsWsMsgToProcess); //zapamiętaj imię czarnego gracza
                QsWebsocketConsoleMsg = "new_black " + _pWebTable->getNameBlack();
                pClient->sendTextMessage("new_black " + _pWebTable->getNameBlack()); //wyślij do websocketowców nową nazwę czarnego
            }
            else if (QsWsMsgToProcess.left(10) == "whose_turn")
            {
                emit MsgFromWebsocketsToWebtable(QsWsMsgToProcess); //zapamiętaj czyja jest tura
                QsWebsocketConsoleMsg = "whose_turn " + _pWebTable->getWhoseTurn();
                pClient->sendTextMessage("whose_turn " + _pWebTable->getWhoseTurn()); //wyślij do websocketowców info o turze
            }
            else if (QsWsMsgToProcess == "new_game") //udało się rozpocząć nową grę. wyślij info o tym na WWW
            {
                QsWebsocketConsoleMsg = "new_game";
                qDebug() << "New game started. Send to site: new_game";
                pClient->sendTextMessage("new_game");
            }
            else if (QsWsMsgToProcess == "white_won" || QsWsMsgToProcess == "black_won" ||
                     QsWsMsgToProcess == "draw") //koniec gry
            {
                QsWebsocketConsoleMsg = QsWsMsgToProcess;
                qDebug() << "End of game. Send to site: " << QsWsMsgToProcess;
                pClient->sendTextMessage(QsWsMsgToProcess);
            }
            else if (QsWsMsgToProcess == "promote_to_what") //test promocji ok. zapytaj się WWW na co promować pionka.
                //TODO: ten sam problem co w BAD_MOVE- nie wiem jak odpowiedzieć do danego gracza, więc...
                //...póki co odpowiadam wszsytkim, a oni będą filtrowali dane przychodzące.
            {
                QsWebsocketConsoleMsg = "promote_to_what";
                qDebug() << " Send to site: " << QsWsMsgToProcess;
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
                qDebug() << "Sending 'BAD_MOVE...' to site";
                QsWebsocketConsoleMsg = "BAD_MOVE " + QsBadMove.simplified();
                pClient->sendTextMessage("BAD_MOVE " + QsBadMove.simplified());
            }
            else //jeżeli chenard da inną odpowiedź (nie powinien)
            {
                QsWebsocketConsoleMsg = "Error11! Wrong msg from tcp: " + QsWsMsgToProcess;
                pClient->sendTextMessage("Error11! Wrong msg from tcp: " + QsWsMsgToProcess);
            }
            //TODO: rozgraniczyć które wiadomośći są "send to web" a które "to chess"
            this->addTextToWebsocketConsole("Send to web: " + QsWebsocketConsoleMsg + "\n");
            QsWebsocketConsoleMsg.clear();
        }

        //tempFunc4DobotVals2XYZ(message+"$"); //wyślij tą wiadmość na serial port

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
}


void Websockets::socketDisconnected() //rozłączanie websocketa
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
        addTextToWebsocketConsole("Disconnected\n");
    }
}
