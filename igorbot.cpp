#include "igorbot.h"

#define ROW 1
#define COLUMN 0

#define OPEN 1
#define CLOSE 0

#define FROM 1
#define TO 0

#define UP 1
#define DOWN 0

#define WEBSITE 1
#define ARDUINO 2

IgorBot::IgorBot(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
                 WebTable *pWebTable, ArduinoUsb *pArduinoUsb)
{
    _pDobot = pDobot;
    _pChessboard = pChessboard;
    _pTCPMsgs = pTCPMsgs;
    _pWebTable = pWebTable;
    _pArduinoUsb = pArduinoUsb;

    m_bAI = false;
    m_bUndo = false;
}

//-------------------------------------------------------------------------------//
//----------------------------KOMUNIKACJA Z ARDUINO------------------------------//
//-------------------------------------------------------------------------------//

void IgorBot::GameStarted() //zareaguj na to że gra wystartowała
{
    emit this->addTextToConsole("new_game\n", 'a');
    qDebug() << "Sending to USB: new_game";
    _pArduinoUsb->sendDataToUsb("started"); //na arduino daj możliwość już wciśnięcua start
}

void IgorBot::BadMove(QString msg)
{
    qDebug() << "Sending to USB:" << msg;
    emit this->addTextToConsole("Sending to USB: " + msg + "\n", 'c');

    _pArduinoUsb->sendDataToUsb(msg.simplified()); //np. "BAD_MOVE e2e4"
}

void IgorBot::GameInProgress() //gra w toku
{
    //podaj na stronę info o tym że ruch został wykonany
    qDebug() << "Chess::GameInProgress(): Sending to Websockets: game_in_progress "
             << _pChessboard->QsPiecieFromTo;
    emit this->addTextToConsole("game_in_progress " + _pChessboard->QsPiecieFromTo, 'c');

    if (!m_bUndo) //jeżeli po wykonaniu ruchu gracza gra jest dalej w toku...
        this->Think5000(); //...to wymyśl kolejny ruch bota białego Igora...
    else  //...a jeżeli po wykonaniu ruchu Igora gra jest dalej w toku...
    {
        m_bUndo = false;
        _pArduinoUsb->sendDataToUsb("IgorHasEndedMove"); //...to niech gracz wykonuje swój kolejny ruch.
    }
}

void IgorBot::EndOfGame(QString msg)
{
    QString QsWhoWon;
    if (msg.left(3) == "1-0") QsWhoWon = "white_won";
    else if (msg.left(3) == "0-1") QsWhoWon = "black_won";
    else if (msg.left(7) == "1/2-1/2") QsWhoWon = "draw";

    qDebug() << "Sending to usb: " << QsWhoWon;
    _pArduinoUsb->sendDataToUsb("GameOver: " + QsWhoWon);

    this->resetPiecePositions(); //przywróć wszystkie bierki do stanu początkowego
    //TODO: zablokować możliwość robienia czegokolwiek na stronie/arduino aż to się nie zakończy
}

void IgorBot::PromoteToWhat() //inicjalizowane w TestOk()
{
    qDebug() << "Sending to arduino: promote";
    _pArduinoUsb->sendDataToUsb("promote"); //zapytaj się arduino na co ma być ta promocja
}

//-------------------------------------------------------------------------------//
//----------------------------KOMUNIKACJA Z CHENARD------------------------------//
//-------------------------------------------------------------------------------//

void IgorBot::EnemyStart()
{
    this->checkAI();

    this->resetPiecePositions(); //przywróć bierki na pierwotne pozycje
    this->NewGame(); //zacznij w pamięci chenardu nową grę
}

void IgorBot::checkMsgFromChenard(QString tcpMsgType, QString tcpRespond)
{
    this->checkAI();

    if (tcpMsgType == "new")
    {
        if (tcpRespond == "OK\n" || tcpRespond == "OK") this->GameStarted();
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType.left(4) == "move")
    {
        //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego 2 warunki
        if (tcpRespond == "OK 1\n" || tcpRespond == "OK 1") this->TcpMoveOk(ARDUINO);
        else if (tcpRespond.left(8) == "BAD_MOVE") this->BadMove(tcpRespond);
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType.left(4) == "test") //dla zapytania: 'test e2e4' dostaniemy: 'OK e2e4 (...)'.
    {
        if (tcpRespond.left(3) == "OK " && tcpRespond.left(4) != "OK 1") this->TestOk();
        else if (tcpRespond == "ILLEGAL") this->MoveTcpPiece(_pChessboard->QsPiecieFromTo);
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType == "status")
    {
        if (tcpRespond.left(1) == "*") this ->GameInProgress();
        else  if (tcpRespond.left(3) == "1-0" || tcpRespond.left(3) == "0-1" || tcpRespond.left(7) == "1/2-1/2")
            this->EndOfGame(tcpRespond);
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType == "undo 1")
    {
        if (tcpRespond == "OK\n") this->UndoOk();
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType == "think 5000") //jeżeli mamy doczynienia z botem, który wymyślił ruch ...
    {
        if (tcpRespond.left(3) == "OK " && tcpRespond.left(4) != "OK 1") this->ThinkOk(tcpRespond); //"OK d1h5 Qh5#"
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
}

void IgorBot::checkMsgForChenard(QString msg)
{
    qDebug() << "IgorBot::checkMsgFromWebsockets: received: " << msg;
    if (msg == "new") this->NewGame();
    else if (msg.left(4) == "move") this->TestMove(ARDUINO, msg); //sprawdź najpierw czy nie występują ruchy specjalne
    else if (msg.left(10) == "promote_to") this->Promote(msg); //odp. z WWW odnośnie tego na co promujemy
    else if (msg.left(5) == "reset") this->resetPiecePositions(); //przywróć bierki na szachownicę do stanu startowego
    else qDebug() << "ERROR: received not recognized msg in IgorBot::checkMsgForChenard: " << msg;
}

void IgorBot::NewGame()
{
    qDebug() << "Sending to tcp: new";
    emit this->addTextToConsole("Sending to tcp: new\n", 'c');
    _pTCPMsgs->queueMsgs(ARDUINO, "new");
}

void IgorBot::MoveTcpPiece(QString msg) // żądanie ruchu- przemieszczenie bierki.
{ //TODO: mylne jest wrażenie że ta funckja już wykonuje ruch bierką
    //do tych ruchów zaliczają się: zwykły ruch, bicie, roszada.
    qDebug() << "IgorBot::MoveTcpPiece: Sending move to tcp: " << msg;
    emit this->addTextToConsole("Sending move to tcp: " + msg + "\n", 'c');
    _pTCPMsgs->queueMsgs(ARDUINO, msg); //zapytaj się tcp o poprawność prośby o ruch
}

void IgorBot::Status(int sender)
{
    this->addTextToConsole("Sending to tcp: status\n", 'w');
    qDebug() << "Sending to tcp: status";
    _pTCPMsgs->queueMsgs(sender, "status");
}

void IgorBot::Promote(QString msg)
{
    _pChessboard->bPromotionConfirmed = true; //w odpowiedzi na chenard ma się wykonać ruch typu...
    //...promocja, by sprawdzić czy nie ma dodatkowo bicia
    _pTCPMsgs->queueMsgs(ARDUINO, "move " + _pChessboard->QsFuturePromote +
                         msg.mid(11,1)); //scal żądanie o ruch z żądanym typem promocji
    //dopóki fizycznie nie podmieniam pionków na nowe figury w promocji, to...
    //...ruch może się odbywać jako normalne przemieszczenie
    qDebug() << "Sent to TCP: move " << _pChessboard->QsFuturePromote << msg.mid(11,1);
}

void IgorBot::Think5000()
{
    _pTCPMsgs->queueMsgs(ARDUINO, "think 5000");
}

void IgorBot::UndoOk()
{
    m_bUndo = true; //zapamiętaj że cofnięcie ruchu miało miejsce
    //niech się wykona cały ruch Igora, łącznie ze sprawdzeniem wszystkich...
    //...dziwnych ruchów tak aby też wykonał się dobrze mechanicznie
    this->checkMsgForChenard("move " + _pChessboard->QsAIPiecieFromTo);
}

void IgorBot::ThinkOk(QString msg)
{
    emit this->addTextToConsole("AI is ready to start move\n", 'c');
    qDebug() << "AI is ready to start move";

    _pChessboard->QsAIPiecieFromTo = msg.mid(3,4); //zapisz w pamięci ruch wymyślony przez bota
    _pTCPMsgs->queueMsgs(ARDUINO, "undo 1"); //...i wróć do stanu sprzed ruchu Igora, by zaraz przeciągnąć go...
    //...przez cały kod sprawdzający ruchy, by wiedzieć jak ramie ma się poruszać w szczególnych przypadkach.
}

void IgorBot::wrongTcpAnswer(QString msgType, QString respond)
{
    this->checkAI();

    //TODO: zapanować jakoś nad tymi sygnałami konsoli
     emit this->addTextToConsole("ERROR: IgorBot::wrongTcpAnswer(): unknown tcpRespond = " +
                                  respond + "for tcpMsgType = " + msgType + "\n", 'c');
    qDebug() << "ERROR: IgorBot::checkMsgFromChenard(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void IgorBot::checkAI()
{
    if (!m_bAI) //wywal błąd jeżeli próbuje wywołać metodę w tej klasie bez włączonej SI
    {
        emit this->addTextToConsole("ERROR: initiated IgorBot method with AI turned off\n", '0');
        qDebug() << "ERROR: initiated IgorBot method with AI turned off";
    }
}
