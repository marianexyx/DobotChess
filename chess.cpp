#include "chess.h"

Chess::Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
             Websockets *pWebsockets, WebTable *pWebTable): _bServiceTests(false)
{
    _pTCPMsgs = pTCPMsgs;
    _pDobot = pDobot;
    _pChessboard = pChessboard;
    _pWebsockets = pWebsockets;
    _pWebTable = pWebTable;
}
void Chess::normalPieceMoving() //sekwencja normalnego przemieszczanie bierki
{
    _pDobot->PTPvalues(_pChessboard->PieceFrom.x, _pChessboard->PieceFrom.y,
                       _pChessboard->PieceFrom.z, _pChessboard->PieceFrom.r);
    emit _pDobot->addTextToDobotConsole("normalPieceMoving: PieceFrom: " + _pChessboard->QsPieceFrom + "\n");

    _pDobot->gripperOpennedState(false);
    emit _pDobot->addTextToDobotConsole("normalPieceMoving: GripperOpenned\n");

    _pDobot->PTPvalues(_pChessboard->ArmDown.x, _pChessboard->ArmDown.y,
                       _pChessboard->ArmDown.z, _pChessboard->ArmDown.r);
    emit _pDobot->addTextToDobotConsole("normalPieceMoving: ArmDown\n");

    _pDobot->gripperOpennedState(true);
    emit _pDobot->addTextToDobotConsole("normalPieceMoving: GripperClosed\n");

    _pDobot->PTPvalues(_pChessboard->ArmUp.x, _pChessboard->ArmUp.y,
                       _pChessboard->ArmUp.z, _pChessboard->ArmUp.r);
    emit _pDobot->addTextToDobotConsole("normalPieceMoving: ArmUp\n");
    _pChessboard->abBoard[_pChessboard->nLiteraPolaFrom][_pChessboard->nCyfraPolaFrom] = false; //miejsce ruszanego pionka jest już puste

    _pDobot->PTPvalues(_pChessboard->PieceTo.x, _pChessboard->PieceTo.y,
                       _pChessboard->PieceTo.z, _pChessboard->PieceTo.r);
    emit _pDobot->addTextToDobotConsole("normalPieceMoving: PieceTo: " + _pChessboard->QsPieceTo + "\n");

    _pDobot->PTPvalues(_pChessboard->ArmDown.x, _pChessboard->ArmDown.y,
                       _pChessboard->ArmDown.z, _pChessboard->ArmDown.r);
    emit _pDobot->addTextToDobotConsole("normalPieceMoving: ArmDown\n");

    _pDobot->gripperOpennedState(false);
    emit _pDobot->addTextToDobotConsole("normalPieceMoving: GripperOpenned\n");
    _pChessboard->abBoard[_pChessboard->nLiteraPolaTo][_pChessboard->nCyfraPolaTo] = true; //nowe miejsce ruszanego pionka jest już teraz zajęte

    _pDobot->PTPvalues(_pChessboard->ArmUp.x, _pChessboard->ArmUp.y,
                       _pChessboard->ArmUp.z, _pChessboard->ArmUp.r);
    emit _pDobot->addTextToDobotConsole("normalPieceMoving: ArmUp\n-End of move sequence-\n");
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
        _pChessboard->abBoard[_pChessboard->nLiteraPolaTo][_pChessboard->nCyfraPolaTo] = true; //miejsce już nie jest zajęte
    }
    else if (QS_msgFromSerial.left(9) == "r_opened2") //zakończono usuwanie bierki...
    {
        //_pChessboard->findDobotXYZVals("n_" + QS_pieceFrom); //...rozpocznij normalne przenoszenie
    }
    else emit _pDobot->addTextToDobotConsole("Error02!: Wrong command /n");
}

void Chess::castlingMovingSequence(QString QS_msgFromSerial)
{
    if (QS_msgFromSerial.left(12) == "c_movedFromK") _pChessboard->findDobotXYZVals("c_open1");
    else if (QS_msgFromSerial.left(9) == "c_opened1") _pChessboard->findDobotXYZVals("c_down1");
    else if (QS_msgFromSerial.left(10) == "c_armDown1") _pChessboard->findDobotXYZVals("c_close1");
    else if (QS_msgFromSerial.left(9) == "c_closed1") _pChessboard->findDobotXYZVals("c_up1");
    //else if (QS_msgFromSerial.left(8) == "c_armUp1") _pChessboard->findDobotXYZVals(QS_kingPosT); //wykomentowane by uniknąć błędów podczas testów
    else if (QS_msgFromSerial.left(10) == "c_movedToK") _pChessboard->findDobotXYZVals("c_down2");
    else if (QS_msgFromSerial.left(10) == "c_armDown2") _pChessboard->findDobotXYZVals("c_open2");
    else if (QS_msgFromSerial.left(9) == "c_opened2") _pChessboard->findDobotXYZVals("c_up2");
    //else if (QS_msgFromSerial.left(8) == "c_armUp2") _pChessboard->findDobotXYZVals(QS_rookPosF); //wykomentowane by uniknąć błędów podczas testów
    else if (QS_msgFromSerial.left(12) == "c_movedFromR") _pChessboard->findDobotXYZVals("c_down3");
    else if (QS_msgFromSerial.left(10) == "c_armDown3") _pChessboard->findDobotXYZVals("c_close2");
    else if (QS_msgFromSerial.left(9) == "c_closed2") _pChessboard->findDobotXYZVals("c_up3");
    //else if (QS_msgFromSerial.left(8) == "c_armUp3") _pChessboard->findDobotXYZVals(QS_rookPosT); //wykomentowane by uniknąć błędów podczas testów
    else if (QS_msgFromSerial.left(10) == "c_movedToR") _pChessboard->findDobotXYZVals("c_down4");
    else if (QS_msgFromSerial.left(10) == "c_armDown4") _pChessboard->findDobotXYZVals("c_open3");
    else if (QS_msgFromSerial.left(9) == "c_opened3") _pChessboard->findDobotXYZVals("c_up4");
    else if (QS_msgFromSerial.left(8) == "c_armUp4")
    {
        // zmiana miejsc na tablicy pól zajętych
        _pChessboard->abBoard[_pChessboard->nLiteraPolaFrom][_pChessboard->nCyfraPolaFrom] = false;
        _pChessboard->abBoard[_pChessboard->nLiteraPolaTo][_pChessboard->nCyfraPolaTo] = true;

        emit _pDobot->addTextToDobotConsole("-End of move sequence- \n");
        _pWebsockets->processWebsocketMsg("OK 1\n");
    }
}

bool Chess::testEnpassant() //sprawdzanie możliwości wystąpienia enpassant
{
    //sprawdzmay czy zapytanie/ruch może być biciem w przelocie
    if (abs(_pChessboard->nLiteraPolaFrom - _pChessboard->nLiteraPolaTo) == 1 && //jeżeli pionek nie idzie po prostej (tj. po ukosie o 1 pole)...
            ((_pWebTable->getWhoseTurn() == "white_turn" && _pChessboard->nCyfraPolaFrom == 5 &&
              _pChessboard->nCyfraPolaTo == 6) || //... i jeżeli bije biały...
             (_pWebTable->getWhoseTurn() == "black_turn" && _pChessboard->nCyfraPolaFrom == 4 &&
              _pChessboard->nCyfraPolaTo == 3)) && //... lub czarny...
            //... i pole na które idzie nie jest zajete (inaczej byłoby zwykłe bicie)
            _pChessboard->abBoard[_pChessboard->nLiteraPolaTo][_pChessboard->nCyfraPolaTo] == false)
    {
        b_test_enpassant = true;
        _pTCPMsgs->doTcpConnect("test " /*+ QS_piecieFromTo*/);  //wykomentowane by uniknąć błędów podczas testów
        return 1;
    }
    else return 0;
}

void Chess::enpassantMovingSequence()
{
    qDebug() << "Enpassant move sequence /n";
    int nEnpassantCyfraPos;  //cyfra pionka bitego

    if (_pWebTable->getWhoseTurn() == "white_turn")
        nEnpassantCyfraPos = _pChessboard->nCyfraPolaTo-1; //pozycja zbijanego czarnego pionka przez pionka białego w jego turze
    else if (_pWebTable->getWhoseTurn() == "black_turn")
        nEnpassantCyfraPos = _pChessboard->nCyfraPolaTo+1; //pozycja zbijanego białego pionka przez pionka czarnego w jego turze
    else
    {
        emit _pDobot->addTextToDobotConsole("Error03!: Wrong turn in enpassant statement /n");
        qDebug() << "Error03!: Wrong turn in enpassant statement /n";
        return;
    }
    //wyjątkowo zbijany będzie gdzie indziej niż lądujący (w enpassant zawsze występuje bicie)
    QS_enpassantToReject = "r_[" + QString::number(_pChessboard->nLiteraPolaTo) + QString::number(nEnpassantCyfraPos) + "]";
    b_test_enpassant = false; //wyłączenie, by nie zapętlać testów w odpowiedzi tcp
    _pChessboard->findDobotXYZVals(QS_enpassantToReject); //rozpocznij enpassant jeżeli test się powiódł
}

bool Chess::testPromotion() //sprawdzanie możliwości wystąpienia enpassant
{
    if (((_pWebTable->getWhoseTurn() == "white_turn" &&  _pChessboard->nCyfraPolaFrom == 7 &&
          _pChessboard->nCyfraPolaTo == 8) ||  //jeżelii bierka chce isc z pola 2 na 1 w turze bialego...
         (_pWebTable->getWhoseTurn() == "black_turn" && _pChessboard->nCyfraPolaFrom == 2 &&
          _pChessboard->nCyfraPolaTo == 1)) &&   //...lub z pola 7 na 8 w turze czarnego...
            abs(_pChessboard->nLiteraPolaFrom - _pChessboard->nLiteraPolaTo) <= 1) //... i ruch odbywa się max o 1 pole odległości liter po ukosie
    {
        QS_futurePromote = _pChessboard->QsPiecieFromTo; //póki nie wiadomo na co promujemy to zapamiętaj zapytanie o ten ruch
        _pTCPMsgs->doTcpConnect("test " + _pChessboard->QsPiecieFromTo + "q"); //test awansu na któlową, który pójdzie na chenard
        return 1;
    }
    else return 0;
}

void Chess::checkMsgFromChenard(QString QStrMsgFromChenardTcp)
{
    qDebug() << "Chess::checkMsgFromChenard: " << QStrMsgFromChenardTcp;
    if (QStrMsgFromChenardTcp == "OK 1\n") this->MoveOk();
    else if (QStrMsgFromChenardTcp == "OK\n") this->GameStarted();
    else if (QStrMsgFromChenardTcp.left(3) == "OK " && QStrMsgFromChenardTcp.left(4) != "OK 1")
        this->TestOk(); //odp. na np. 'test e2e4' to 'OK e2e4'
    else if (QStrMsgFromChenardTcp.left(7) == "ILLEGAL") this->Illegal();
    else if (QStrMsgFromChenardTcp.left(8) == "BAD_MOVE") this->BadMove(QStrMsgFromChenardTcp);
    else if (QStrMsgFromChenardTcp.left(1) == "*") this->GameInProgress();
    else if (QStrMsgFromChenardTcp.left(3) == "1-0" || QStrMsgFromChenardTcp.left(3) == "0-1" ||
             QStrMsgFromChenardTcp.left(7) == "1/2-1/2") this->EndOfGame(QStrMsgFromChenardTcp);
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgFromChenard: " << QStrMsgFromChenardTcp;
}

void Chess::checkMsgFromWebsockets(QString QStrMsgFromWebsockets)
{
    qDebug() << "Chess::checkMsgFromWebsockets: received: " << QStrMsgFromWebsockets;
    if (QStrMsgFromWebsockets == "new") this->NewGame();
    else if (QStrMsgFromWebsockets.left(4) == "move") this->MovePiece(QStrMsgFromWebsockets);
    else if (QStrMsgFromWebsockets.left(10) == "promote_to") this->Promote(QStrMsgFromWebsockets); //odp. z WWW odnośnie tego na co promujemy
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgFromWebsockets: " << QStrMsgFromWebsockets;
}

void Chess::MoveOk() //ruch sie powiódł. wykonaj ruch i zapytaj się tcp o stan
{
    qDebug() << "-Begin move sequence-";
    emit addTextToDobotConsole("-Begin move sequence-\n");

    if (b_test_enpassant) this->enpassantMovingSequence(); //jeżeli test na enpassant się powiódł, to rozpocznij ten ruch
    else if (_pChessboard->castlingStatements(_pChessboard->QsPiecieFromTo))
    {
        qDebug() << "Start castling move sequence";
        _pChessboard->findDobotXYZVals(_pChessboard->QsKingPosF); //rozpocznij roszadę jeżeli spełnione są jej warunki
    }
    else if (_pChessboard->removeStatements()) //jeżeli podczas ruchu następuje bicie
    {
        qDebug() << "Start remove piece sequence";
        _pChessboard->findDobotXYZVals(_pChessboard->QsPieceToReject); //rozpocznij zbijanie jeżeli spełnione są jej warunki
    }
    else if (bPromotionConfirmed) //rozpocznij zbijanie podczas promocji jeżeli spełnione jej są warunki
    {
        qDebug() << "Start promotion move sequence";
        if (_pChessboard->removeStatements()) //jeżeli przy promocji następuje bicie
            _pChessboard->findDobotXYZVals(_pChessboard->QsPieceToReject); //to najpierw zbijamy
        bPromotionConfirmed = false;
    }
    else
    {
        qDebug() << "Start standard move sequence";
        this->normalPieceMoving(); //rozpocznij normalny ruch
    }

    //ruch niech się wykonuje, a w ten czas niech gra sprawdzi czy to nie jest koniec gry komendą 'status'
    _pWebsockets->addTextToWebsocketConsole("Sending 'status' command to tcp. \n");
    qDebug() << "Sending 'status' command to tcp";
    _pTCPMsgs->doTcpConnect("status");
}

void Chess::NewGame() //przesyłanie prośby z WWW o nową grę na TCP
{
    if (_pTCPMsgs->getBlokadaZapytan() == false) //jeżeli chenard nie przetwarza żadnego zapytania
        //TODO: program gdzieś musi stackować zapytania których nie może jeszcze przetworzyć
    {
        _pTCPMsgs->setBlokadaZapytan(true);
        if ((_pWebTable->getNameWhite() != "Biały" && _pWebTable->getNameBlack() != "Czarny" && //zabezpieczenie:
                !_pWebTable->getNameWhite().isEmpty() && !_pWebTable->getNameBlack().isEmpty()) || //jeżeli obaj gracze siedzą przy stole
                _bServiceTests) //albo jeżeli mamy do czynienia z zapytaniem serwisowym
            //TODO: dodać więcej zabezpieczeń (inne nazwy, typy itd) i reagować na nie jakoś
        {
            _pWebsockets->addTextToWebsocketConsole("Sending 'new' game command to tcp. \n");
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
    if (!_bServiceTests)_pWebsockets->processWebsocketMsg("new_game");
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył wiadomość. uwolnienie blokady zapytań.
}

void Chess::MovePiece(QString QStrMsgFromWebsockets)
{
    if (_pTCPMsgs->getBlokadaZapytan() == false) //jeżeli chenard nie przetwarza żadnego zapytania
        //TODO: program gdzieś musi stackować zapytania których nie może jeszcze przetworzyć
    {
        _pChessboard->findBoardPos(QStrMsgFromWebsockets); //oblicz wszystkie pozycje bierek, także dla ruchów specjalnych

        if(this->testPromotion()) return; //jeżeli możemy mieć doczynienia z promocją, to sprawdź tą opcję i przerwij
        else if (this->testEnpassant()) return; //jeżeli możemy mieć doczynienia z enpassant, to sprawdź tą opcję i przerwij
        else //a jeżeli było to każde inne rządzanie ruchu, to wykonuj przemieszczenie bierki...
            //...(do tych ruchów zaliczają się: zwykły ruch, bicie, roszada)
        {
            _pWebsockets->addTextToWebsocketConsole("Sending normal move to tcp: " + QStrMsgFromWebsockets + "\n");
            qDebug() << "Sending normal move to tcp: " << QStrMsgFromWebsockets;
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
    bPromotionConfirmed = true; //w odpowiedzi na chenard ma się wykonać ruch typu promocja, by sprawdzić czy nie ma dodatkowo bicia
    _pTCPMsgs->doTcpConnect("move" + QS_futurePromote + QStrMsgFromWebsockets.mid(11,1)); //scal żądanie o ruch z typem promocji
    //dopóki fizycznie nie podmieniam pionków na nowe figury w promocji, to ruch może się odbywać jako normalne przemieszczenie
    qDebug() << "Send to TCP: move " << QS_futurePromote << QStrMsgFromWebsockets.mid(11,1);
}

void Chess::Illegal() //któryś z wykonywanych testów się nie powiódł
{
    //warunki ruchów specjalnych się znoszą, dlatego nie trzeba ich implementować.
    //jedynym wyjątkiem jest bicie podczas promocji zawarte w warunku poniżej
    if(bPromotionConfirmed && _pChessboard->removeStatements()) //jeżeli dla testu promocji (który się nie powiódł) występuje inny special move (bicie bierki)..
        _pChessboard->findDobotXYZVals(_pChessboard->QsPieceToReject); //...to wykonaj normalne bicie, które potem samo przejdzie w normalny ruch
    else _pChessboard->findDobotXYZVals("n_" + _pChessboard->QsPieceFrom); //jako że innych przypadków nie trzeba rozpatrywać to każdy innych ruch jest zwykły
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
    qDebug() << "Chess::GameInProgress(): Sending to Websockets: game_in_progress " << _pChessboard->QsPiecieFromTo;
    _pWebsockets->processWebsocketMsg("game_in_progress " + _pChessboard->QsPiecieFromTo);
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
