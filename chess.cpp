#include "chess.h"

Chess::Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
             Websockets *pWebsockets, WebTable *pWebTable)
{
    _pTCPMsgs = pTCPMsgs;
    _pDobot = pDobot;
    _pChessboard = pChessboard;
    _pWebsockets = pWebsockets;
    _pWebTable = pWebTable;
}

void Chess::findBoardPos(QString QS_piecePositions)
{
    QS_piecieFromTo = QS_piecePositions.mid(5,4); // e2e4
    QS_pieceFrom = "[" + QS_piecePositions.mid(5,2) + "]f"; //[e2]f
    QS_pieceTo   = "[" + QS_piecePositions.mid(7,2) + "]t"; //[e4]t

    QS_literaPolaFrom = QS_piecePositions.mid(5,1);
    QS_literaPolaTo = QS_piecePositions.mid(7,1);

    if (QS_literaPolaFrom == "a" || QS_literaPolaFrom == "A") {n_literaPolaFrom = 0; QS_literaPolaFrom = "a";}
    else if (QS_literaPolaFrom == "b" || QS_literaPolaFrom == "B") {n_literaPolaFrom = 1; QS_literaPolaFrom = "b";}
    else if (QS_literaPolaFrom == "c" || QS_literaPolaFrom == "C") {n_literaPolaFrom = 2; QS_literaPolaFrom = "c";}
    else if (QS_literaPolaFrom == "d" || QS_literaPolaFrom == "D") {n_literaPolaFrom = 3; QS_literaPolaFrom = "d";}
    else if (QS_literaPolaFrom == "e" || QS_literaPolaFrom == "E") {n_literaPolaFrom = 4; QS_literaPolaFrom = "e";}
    else if (QS_literaPolaFrom == "f" || QS_literaPolaFrom == "F") {n_literaPolaFrom = 5; QS_literaPolaFrom = "f";}
    else if (QS_literaPolaFrom == "g" || QS_literaPolaFrom == "G") {n_literaPolaFrom = 6; QS_literaPolaFrom = "g";}
    else if (QS_literaPolaFrom == "h" || QS_literaPolaFrom == "H") {n_literaPolaFrom = 7; QS_literaPolaFrom = "h";}

    if (QS_literaPolaTo== "a" || QS_literaPolaTo== "A") {n_literaPolaTo = 0; QS_literaPolaTo= "a";}
    else if (QS_literaPolaTo== "b" || QS_literaPolaTo== "B") {n_literaPolaTo = 1; QS_literaPolaTo= "b";}
    else if (QS_literaPolaTo== "c" || QS_literaPolaTo== "C") {n_literaPolaTo = 2; QS_literaPolaTo= "c";}
    else if (QS_literaPolaTo== "d" || QS_literaPolaTo== "D") {n_literaPolaTo = 3; QS_literaPolaTo= "d";}
    else if (QS_literaPolaTo== "e" || QS_literaPolaTo== "E") {n_literaPolaTo = 4; QS_literaPolaTo= "e";}
    else if (QS_literaPolaTo== "f" || QS_literaPolaTo== "F") {n_literaPolaTo = 5; QS_literaPolaTo= "f";}
    else if (QS_literaPolaTo== "g" || QS_literaPolaTo== "G") {n_literaPolaTo = 6; QS_literaPolaTo= "g";}
    else if (QS_literaPolaTo== "h" || QS_literaPolaTo== "H") {n_literaPolaTo = 7; QS_literaPolaTo= "h";}

    n_cyfraPolaFrom = QS_piecePositions.mid(6,1).toInt() - 1;
    n_cyfraPolaTo = QS_piecePositions.mid(8,1).toInt() - 1;
}

void Chess::normalPieceMoving(QString QS_normalMove) //sekwencja normalnego przemieszczanie bierki
{
    //pierwszy ruch wykonywany z poziomu odpowiedzi z tcp
    if (QS_normalMove.left(11) == "n_movedFrom") // && QS_normalMove.mid(14,2) == QS_pieceFrom.mid(1,2) -ew. dodatkowe zabezpieczenie (może być błąd)
        _pChessboard->findDobotXYZVals("n_open1"); // odpowiedź na pierwszy ruch
    else if (QS_normalMove.left(9) == "n_opened1") _pChessboard->findDobotXYZVals("n_down1");
    else if (QS_normalMove.left(10)  == "n_armDown1") _pChessboard->findDobotXYZVals("n_close1");
    else if (QS_normalMove.left(9)  == "n_closed1") _pChessboard->findDobotXYZVals("n_up1");
    else if (QS_normalMove.left(8)  == "n_armUp1")
    {
        a_b_board[n_literaPolaFrom][n_cyfraPolaFrom] = false; //miejsce ruszanego pionka jest już puste
        _pChessboard->findDobotXYZVals("n_" + QS_pieceTo);
    }
    else if (QS_normalMove.left(9) == "n_movedTo") //  && QS_normalMove.mid(8,2) == QS_pieceTo.mid(1,2) -ew. dodatkowe zabezpieczenie (może być błąd)
        _pChessboard->findDobotXYZVals("n_down2");
    else if (QS_normalMove.left(10)  == "n_armDown2") _pChessboard->findDobotXYZVals("n_open2");
    else if (QS_normalMove.left(9)  == "n_opened2")
    {
        a_b_board[n_literaPolaTo][n_cyfraPolaTo] = true; //nowe miejsce ruszpnego pionka jest już teraz zajęte
        _pChessboard->findDobotXYZVals("n_up2");
    }
    else if (QS_normalMove.left(8)  == "n_armUp2")
    {
        QS_normalMove = ""; //czyścimy na wszelki wypadek. raczej to zbędne i inaczej wypadałoby to wykonać
        emit _pDobot->addTextToDobotConsole("-End of move sequence- \n");
        // !! ta zmienna nie była jeszcze deklarowana. chyba błędem jest próba użycia...
        //...sygnału który pochodzi z innej klasy

        _pWebsockets->processWebsocketMsg("OK 1\n"); // ! to jest bardzo mylący zapis
    }
    else emit _pDobot->addTextToDobotConsole("error01: ");

    QS_normalMove.clear();
}

bool Chess::removeStatements() // funkcje do sprawdzania czy bijemy bierkę
{
    if (a_b_board[n_literaPolaTo][n_cyfraPolaTo] == true) //sprawdzanie czy na pole, gdzie bierka idzie nie jest zajęte (nieprawdziwe dla enpassant)
    {
        QS_pieceToReject = "r_" + QS_pieceTo;
        return 1;
    }
    else return 0;
}

void Chess::removePieceSequence(QString QS_msgFromSerial) //sekwencja ruchów przy zbijaniu bierki
{
    //pierwszy ruch wykonywany z poziomu odpowiedzi z tcp
    if (QS_msgFromSerial.left(9) == "r_movedTo") _pChessboard->findDobotXYZVals("r_open1");
    else if (QS_msgFromSerial.left(9) == "r_opened1") _pChessboard->findDobotXYZVals("r_down");
    else if (QS_msgFromSerial.left(6) == "r_down") _pChessboard->findDobotXYZVals("r_close");
    else if (QS_msgFromSerial.left(8) == "r_closed") _pChessboard->findDobotXYZVals("r_up");
    else if (QS_msgFromSerial.left(7) == "r_armUp") _pChessboard->findDobotXYZVals("r_trash");
    else if (QS_msgFromSerial.left(9) == "r_trashed")
    {
        _pChessboard->findDobotXYZVals("r_open2");
        a_b_board[n_literaPolaTo][n_cyfraPolaTo] = false; //miejsce już nie jest zajęte
    }
    else if (QS_msgFromSerial.left(9) == "r_opened2") //zakończono usuwanie bierki...
    {
        _pChessboard->findDobotXYZVals("n_" + QS_pieceFrom); //...rozpocznij normalne przenoszenie
    }
    else emit _pDobot->addTextToDobotConsole("Error02!: Wrong command /n");
}

bool Chess::castlingStatements(QString QS_possibleCastlingVal) // sprawdzanie czy roszadę czas wykonać
{
    if (QS_possibleCastlingVal == "e1c1" || QS_possibleCastlingVal == "e1C1" || QS_possibleCastlingVal == "E1c1" || QS_possibleCastlingVal == "E1C1" ||
            QS_possibleCastlingVal == "e1g1" || QS_possibleCastlingVal == "e1G1" || QS_possibleCastlingVal == "E1g1" || QS_possibleCastlingVal == "E1G1" ||
            QS_possibleCastlingVal == "e8c8" || QS_possibleCastlingVal == "e8C8" || QS_possibleCastlingVal == "E8c8" || QS_possibleCastlingVal == "E8C8" ||
            QS_possibleCastlingVal == "e8g8" || QS_possibleCastlingVal == "e8G8" || QS_possibleCastlingVal == "E8g8" || QS_possibleCastlingVal == "E8G8" )
    {
        //ustawiane skąd-dokąd przenoszony będzie król podczas roszady
        QS_kingPosF = "c_" + QS_pieceFrom + "K"; //jest to info dla arduino, że mamy do czynienia z roszadą
        QS_kingPosT = "c_" + QS_pieceTo + "K";

        //ustawiane skąd-dokąd przenoszona będzie wieża podczas roszady
        if (QS_literaPolaTo == "c")
        {
            QS_rookPosF = "c_[a";
            QS_rookPosT = "c_[d";
        }
        else
        {
            QS_rookPosF = "c_[h";
            QS_rookPosT = "c_[f";
        }
        if(n_cyfraPolaTo == 1)
        {
            QS_rookPosF += "1]fR";
            QS_rookPosT += "1]tR";
        }
        else
        {
            QS_rookPosF += "8]fR";
            QS_rookPosT += "8]tR";
        }
        return 1;
    }
    else
        return 0;
}

void Chess::castlingMovingSequence(QString QS_msgFromSerial)
{
    if (QS_msgFromSerial.left(12) == "c_movedFromK") _pChessboard->findDobotXYZVals("c_open1");
    else if (QS_msgFromSerial.left(9) == "c_opened1") _pChessboard->findDobotXYZVals("c_down1");
    else if (QS_msgFromSerial.left(10) == "c_armDown1") _pChessboard->findDobotXYZVals("c_close1");
    else if (QS_msgFromSerial.left(9) == "c_closed1") _pChessboard->findDobotXYZVals("c_up1");
    else if (QS_msgFromSerial.left(8) == "c_armUp1") _pChessboard->findDobotXYZVals(QS_kingPosT);
    else if (QS_msgFromSerial.left(10) == "c_movedToK") _pChessboard->findDobotXYZVals("c_down2");
    else if (QS_msgFromSerial.left(10) == "c_armDown2") _pChessboard->findDobotXYZVals("c_open2");
    else if (QS_msgFromSerial.left(9) == "c_opened2") _pChessboard->findDobotXYZVals("c_up2");
    else if (QS_msgFromSerial.left(8) == "c_armUp2") _pChessboard->findDobotXYZVals(QS_rookPosF);
    else if (QS_msgFromSerial.left(12) == "c_movedFromR") _pChessboard->findDobotXYZVals("c_down3");
    else if (QS_msgFromSerial.left(10) == "c_armDown3") _pChessboard->findDobotXYZVals("c_close2");
    else if (QS_msgFromSerial.left(9) == "c_closed2") _pChessboard->findDobotXYZVals("c_up3");
    else if (QS_msgFromSerial.left(8) == "c_armUp3") _pChessboard->findDobotXYZVals(QS_rookPosT);
    else if (QS_msgFromSerial.left(10) == "c_movedToR") _pChessboard->findDobotXYZVals("c_down4");
    else if (QS_msgFromSerial.left(10) == "c_armDown4") _pChessboard->findDobotXYZVals("c_open3");
    else if (QS_msgFromSerial.left(9) == "c_opened3") _pChessboard->findDobotXYZVals("c_up4");
    else if (QS_msgFromSerial.left(8) == "c_armUp4")
    {
        // zmiana miejsc na tablicy pól zajętych
        a_b_board[n_literaPolaFrom][n_cyfraPolaFrom] = false;
        a_b_board[n_literaPolaTo][n_cyfraPolaTo] = true;

        //czyszczenie zmiennych, gdyby przez przypadek coś kiedyś chciało ich bez przypisywania używać
        ///!!! sprawdzić czy to tu tak ok
        QS_kingPosF.clear();
        QS_kingPosT.clear();
        QS_rookPosF.clear();
        QS_rookPosT.clear();

        emit _pDobot->addTextToDobotConsole("-End of move sequence- \n");
        _pWebsockets->processWebsocketMsg("OK 1\n");
    }
}

bool Chess::testEnpassant() //sprawdzanie możliwości wystąpienia enpassant
{
    //sprawdzmay czy zapytanie/ruch może być biciem w przelocie
    if (abs(n_literaPolaFrom - n_literaPolaTo) == 1 && //jeżeli pionek nie idzie po prostej (tj. po ukosie o 1 pole)...
            ((_pWebTable->getWhoseTurn() == "white_turn" && n_cyfraPolaFrom == 5 && n_cyfraPolaTo == 6) || //... i jeżeli bije biały...
             (_pWebTable->getWhoseTurn() == "black_turn" && n_cyfraPolaFrom == 4 && n_cyfraPolaTo == 3)) && //... lub czarny...
            a_b_board[n_literaPolaTo][n_cyfraPolaTo] == false) //... i pole na które idzie nie jest zajete (inaczej byłoby zwykłe bicie)
    {
        b_test_enpassant = true;
        _pTCPMsgs->doTcpConnect("test " + QS_piecieFromTo);
        return 1;
    }
    else return 0;
}

void Chess::enpassantMovingSequence()
{
    qDebug() << "Enpassant move sequence /n";
    int nEnpassantCyfraPos;  //cyfra pionka bitego

    if (_pWebTable->getWhoseTurn() == "white_turn")
        nEnpassantCyfraPos = n_cyfraPolaTo-1; //pozycja zbijanego czarnego pionka przez pionka białego w jego turze
    else if (_pWebTable->getWhoseTurn() == "black_turn")
        nEnpassantCyfraPos = n_cyfraPolaTo+1; //pozycja zbijanego białego pionka przez pionka czarnego w jego turze
    else
    {
        emit _pDobot->addTextToDobotConsole("Error03!: Wrong turn in enpassant statement /n");
        qDebug() << "Error03!: Wrong turn in enpassant statement /n";
        return;
    }
    //wyjątkowo zbijany będzie gdzie indziej niż lądujący (w enpassant zawsze występuje bicie)
    QS_enpassantToReject = "r_[" + QString::number(n_literaPolaTo) + QString::number(nEnpassantCyfraPos) + "]";
    b_test_enpassant = false; //wyłączenie, by nie zapętlać testów w odpowiedzi tcp
    _pChessboard->findDobotXYZVals(QS_enpassantToReject); //rozpocznij enpassant jeżeli test się powiódł
}

bool Chess::testPromotion() //sprawdzanie możliwości wystąpienia enpassant
{
    if (((_pWebTable->getWhoseTurn() == "white_turn" &&  n_cyfraPolaFrom == 7 && n_cyfraPolaTo == 8) ||  //jeżelii bierka chce isc z pola 2 na 1 w turze bialego...
         (_pWebTable->getWhoseTurn() == "black_turn" && n_cyfraPolaFrom == 2 && n_cyfraPolaTo == 1)) &&   //...lub z pola 7 na 8 w turze czarnego...
            abs(n_literaPolaFrom - n_literaPolaTo) <= 1)          //...i ruch odbywa się max o 1 pole odległości liter po ukosie
    {
        QS_futurePromote = QS_piecieFromTo; //póki nie wiadomo na co promujemy to zapamiętaj zapytanie o ten ruch
        _pTCPMsgs->doTcpConnect("test " + QS_piecieFromTo + "q"); //test awansu na któlową, który pójdzie na chenard
        return 1;
    }
    else return 0;
}

void Chess::checkMsgFromChenard(QString QStrMsgFromChenardTcp)
{
    if (QStrMsgFromChenardTcp == "OK 1\n") this->MoveOk();
    else if (QStrMsgFromChenardTcp == "OK\n") this->GameStarted();
    else if (QStrMsgFromChenardTcp.left(3) == "OK " && QStrMsgFromChenardTcp != "OK 1\n") this->TestOk(); //odp. na np. 'test e2e4' to 'OK e2e4'
    else if (QStrMsgFromChenardTcp.left(7) == "ILLEGAL") this->Illegal();
    else if (QStrMsgFromChenardTcp.left(8) == "BAD_MOVE") this->BadMove(QStrMsgFromChenardTcp);
    else if (QStrMsgFromChenardTcp.left(1) == "*") this->GameInProgress();
    else if (QStrMsgFromChenardTcp.left(3) == "1-0" || QStrMsgFromChenardTcp.left(3) == "0-1" ||
             QStrMsgFromChenardTcp.left(7) == "1/2-1/2") this->EndOfGame(QStrMsgFromChenardTcp);
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgFromChenard: " << QStrMsgFromChenardTcp;
}

void Chess::checkMsgFromWebsockets(QString QStrMsgFromWebsockets)
{
    if (QStrMsgFromWebsockets == "new") this->NewGame();
    else if (QStrMsgFromWebsockets.left(4) == "move") this->MovePiece(QStrMsgFromWebsockets);
    else if (QStrMsgFromWebsockets.left(10) == "promote_to") this->Promote(QStrMsgFromWebsockets); //odp. z WWW odnośnie tego na co promujemy
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgFromWebsockets: " << QStrMsgFromWebsockets;
}

void Chess::MoveOk() //ruch sie powiódł. wykonaj ruch i zapytaj się tcp o stan
{
    emit addTextToDobotConsole("-Begin move sequence-");

    if (b_test_enpassant) this->enpassantMovingSequence(); //jeżeli test na enpassant się powiódł, to rozpocznij ten ruch
    else if (castlingStatements(QS_piecieFromTo))
    {
        qDebug() << "Start castling move sequence \n";
        _pChessboard->findDobotXYZVals(QS_kingPosF); //rozpocznij roszadę jeżeli spełnione są jej warunki
    }
    else if (removeStatements())
    {
        qDebug() << "Start remove piece sequence \n";
        _pChessboard->findDobotXYZVals(QS_pieceToReject); //rozpocznij zbijanie jeżeli spełnione są jej warunki
    }
    else if (b_promotion_confirmed) //rozpocznij zbijanie podczas promocji jeżeli spełnione jej są warunki
    {
        qDebug() << "Start promotion move sequence \n";
        if (removeStatements()) _pChessboard->findDobotXYZVals(QS_pieceToReject);
        b_promotion_confirmed = false;
    }
    else
    {
        qDebug() << "Start standard move sequence \n";
        _pChessboard->findDobotXYZVals("n_" + QS_pieceFrom); //rozpocznij normalny ruch
    }

    //ruch niech się wykonuje, a w ten czas niech gra sprawdzi czy to nie jest koniec gry komendą 'status'
    _pWebsockets->addTextToWebsocketConsole("Sending 'status' command to tcp. \n");
    qDebug() << "Sending 'status' command to tcp. \n";
    _pTCPMsgs->doTcpConnect("status");
}

void Chess::NewGame() //przesyłanie prośby z WWW o nową grę na TCP
{
    if (_pTCPMsgs->getBlokadaZapytan() == false) //jeżeli chenard nie przetwarza żadnego zapytania
        //TODO: program gdzieś musi stackować zapytania których nie może jeszcze przetworzyć
    {
        _pTCPMsgs->setBlokadaZapytan(true);
        if (_pWebTable->getNameWhite() != "Biały" && _pWebTable->getNameBlack() != "Czarny" && //zabezpieczenie:
                !_pWebTable->getNameWhite().isEmpty() && !_pWebTable->getNameBlack().isEmpty()) //jeżeli obaj gracze siedzą przy stole
            //TODO: dodać więcej zabezpieczeń (inne nazwy, typy itd) i reagować na nie jakoś
        {
            _pWebsockets->addTextToWebsocketConsole("Sending 'new' game command to tcp. \n");
            qDebug() << "Sending 'new' game command to tcp. \n";
            _pTCPMsgs->doTcpConnect("new");
        }
        else _pWebsockets->addTextToWebsocketConsole("Error09! Wrong players names.\n");
    }
    else _pWebsockets->addTextToWebsocketConsole("Error10! Previous msg hasn't been processed.\n");
}

void Chess::GameStarted() //prześlij info na stronę o tym że gra wystartowała
{
    _pWebsockets->addTextToWebsocketConsole("new_game");
    qDebug() << "Sending 'new_game' to site";
    _pWebsockets->processWebsocketMsg("new_game");
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył wiadomość. uwolnienie blokady zapytań.
}

void Chess::MovePiece(QString QStrMsgFromWebsockets)
{
    if (_pTCPMsgs->getBlokadaZapytan() == false) //jeżeli chenard nie przetwarza żadnego zapytania
        //TODO: program gdzieś musi stackować zapytania których nie może jeszcze przetworzyć
    {
        this->findBoardPos(QStrMsgFromWebsockets); //oblicz wszystkie pozycje bierek, także dla ruchów specjalnych

        if(this->testPromotion()) return; //jeżeli możemy mieć doczynienia z promocją, to sprawdź tą opcję i przerwij
        else if (this->testEnpassant()) return; //jeżeli możemy mieć doczynienia z enpassant, to sprawdź tą opcję i przerwij
        else //a jeżeli było to każde inne rządzanie ruchu, to wykonuj przemieszczenie bierki...
            //...(do tych ruchów zaliczają się: zwykły ruch, bicie, roszada)
        {
            _pWebsockets->addTextToWebsocketConsole("Sending normal move to tcp: " + QStrMsgFromWebsockets + "\n");
            qDebug() << "Sending normal move to tcp: " << QStrMsgFromWebsockets << "\n";
            _pTCPMsgs->doTcpConnect(QStrMsgFromWebsockets);
        }
    }
    else
    {
        _pWebsockets->addTextToWebsocketConsole("Error08! Previous TCP msg hasn't been processed.\n");
        qDebug() << "Error08! Previous TCP msg hasn't been processed.\n";
    }
}

void Chess::Promote(QString QStrMsgFromWebsockets)
{
    b_promotion_confirmed = true; //w odpowiedzi na chenard ma się wykonać ruch typu promocja, by sprawdzić czy nie ma dodatkowo bicia
    _pTCPMsgs->doTcpConnect("move" + QS_futurePromote + QStrMsgFromWebsockets.mid(11,1)); //scal żądanie o ruch z typem promocji
    //dopóki fizycznie nie podmieniam pionków na nowe figury w promocji, to ruch może się odbywać jako normalne przemieszczenie
    qDebug() << "Send to TCP: move " << QS_futurePromote << QStrMsgFromWebsockets.mid(11,1);
}

void Chess::Illegal() //któryś z wykonywanych testów się nie powiódł
{
    //warunki ruchów specjalnych się znoszą, dlatego nie trzeba ich implementować.
    //jedynym wyjątkiem jest bicie podczas promocji zawarte w warunku poniżej
    if(b_promotion_confirmed && removeStatements()) //jeżeli dla testu promocji (który się nie powiódł) występuje inny special move (bicie bierki)..
        _pChessboard->findDobotXYZVals(QS_pieceToReject); //...to wykonaj normalne bicie, które potem samo przejdzie w normalny ruch
    else _pChessboard->findDobotXYZVals("n_" + QS_pieceFrom); //jako że innych przypadków nie trzeba rozpatrywać to każdy innych ruch jest zwykły
}

void Chess::TestOk()
{
    qDebug() << "Sending 'promote_to_what' to websockets";
    _pWebsockets->processWebsocketMsg("promote_to_what");
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył wiadomość. uwolnienie blokady zapytań.
}

void Chess::BadMove(QString QsMsgFromChenardTcp)
{
    QsMsgFromChenardTcp = QsMsgFromChenardTcp.mid(9);
    qDebug() << "Sending 'BAD_MOVE...' to websockets";
    _pWebsockets->processWebsocketMsg("BAD_MOVE " + QsMsgFromChenardTcp.simplified());
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył właśnie wiadomość. uwolnienie blokady zapytań
}

void Chess::GameInProgress() //gra w toku
{
    //podaj na stronę info o tym że ruch został wykonany
    //TODO: sprawdzić czy strona się nie wykrzaczy, bo usunąłem potwierdzenie na stronę...
    //...o tym jaki ruch został wykonany
    qDebug() << "Sending 'game_in_progress' to websockets";
    _pWebsockets->processWebsocketMsg("game_in_progress");
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył właśnie wiadomość. uwolnienie blokady zapytań
}
void Chess::EndOfGame(QString QStrMsgFromChenardTcp)
{
    QString QsWhoWon;
    if (QStrMsgFromChenardTcp.left(3) == "1-0") QsWhoWon = "white_won";
    else if (QStrMsgFromChenardTcp.left(3) == "0-1") QsWhoWon = "black_won";
    else if (QStrMsgFromChenardTcp.left(7) == "1/2-1/2") QsWhoWon = "draw";
    qDebug() << "Sending '" << QsWhoWon << "' to websockets";
    _pWebsockets->processWebsocketMsg("QsWhoWon");
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył właśnie wiadomość. uwolnienie blokady zapytań
    QsWhoWon.clear();
}
