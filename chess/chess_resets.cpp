#include "chess_resets.h"

ChessResets::ChessResets(Chess* pChess)
{
    _pChess = pChess;
    _pClientsList = _pChess->getClientsPointer();
    _pTimers = _pChess->getTimersPointer();
    _pStatus = _pChess->getStatusPointer();
    _pMovements = _pChess->getMovementsPointer();
}

void ChessResets::restartGame(END_TYPE WhoWon, Client* pPlayerToClear = nullptr)
{
    //info
    QString QStrPlayer = "";
    if (pPlayerToClear != nullptr)
        QStrPlayer = _pClientsList->getClientName(pPlayerToClear) + ":";
    qDebug() << "Chess::restartGame():" << QStrPlayer << endTypeAsQstr(WhoWon);

    _pChess->setGameStatus(GS_TURN_NONE_RESETING);

    //reset data
    _pClientsList->resetPlayersStartConfirmInfo();
    _pTimers->resetGameTimers();
    _pStatus->resetStatusData();

    this->changePlayersOnChairs(WhoWon, pPlayerToClear);
    this->sendEndGameMsgToAllClients(WhoWon);

    //todo: wygląda na to że funkcja resetu załącza się jeszcze zanim odpowiedź poleci na...
    //...stronę, przez co trzeba czekać aż resetowanie się zakończy zanim gracze się dowiedzą...
    //...że nastąpił koniec gry (patrz niżej)
    //todo: info reseting powinno na stronie wyskakiwać zawsze, jak napotkamy koniec...
    //... gry- samo z siebie. nie potrzeba tego wysyłać za każdym razem
    _pChess->sendDataToAllClients("reseting");

    this->resetPiecePositions();
}

void ChessResets::changePlayersOnChairs(END_TYPE WhoWon, Client* pPlayerToClear)
{
    switch(WhoWon)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
    case ET_TIMEOUT_GAME:
        if (pPlayerToClear != nullptr)
        {
            qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): player must be"
                        " null if not disconnected";
            return;
        }
        _pClientsList->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        _pClientsList->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);
        break;
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        if (pPlayerToClear == nullptr)
        {
            qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): player can't be"
                        " null if diconnected";
            return;
        }
        if (_pClientsList->getClientType(pPlayerToClear) == PT_WHITE)
            _pClientsList->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        else if (_pClientsList->getClientType(pPlayerToClear) == PT_BLACK)
            _pClientsList->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);
        else
        {
            qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): wrong player type:"
                     << WhoWon;
            return;
        }
        break;
    default:
        qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): unknown ETWhoWon val="
                 << WhoWon;
        return;
    }
}

bool ChessResets::isPieceSetOnBoardsIdentical(Chessboard* pBoard1, Chessboard* pBoard2)
{
    for (short sField=1; sField>=64; ++sField)
    {
        if (pBoard1->getField(sField)->getPieceOnField() !=
                pBoard2->getField(sField)->getPieceOnField())
            return false;
    }

    return true;
}

void ChessResets::sendEndGameMsgToAllClients(END_TYPE WhoWon, Client* pPlayerToClear = nullptr)
{
    switch(WhoWon)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
        if (pPlayerToClear != nullptr)
        {
            qDebug() << "ERROR: ChessResets::sendEndGameMsgToAllClients(): pPlayerToClear must"
                        " be nullptr in" << endTypeAsQstr(WhoWon) << "end type";
            return;
        }
        //future: jak wysyłam table data, to nie ma potrzeby wysyłać "nt"
        //future: na przyszłość komunikat o ostatnim ruchu można wyjebać, jako że informacje...
        //...o ruchach będą wyciągane z "history"
        QString QStrMove = _pMovements->getMove().asQStr();
        _pChess->sendDataToAllClients("moveOk " + QStrMove + " nt " + endTypeAsQstr(WhoWon)
                                      + " " + _pChess->getTableData());
        break;
    case ET_TIMEOUT_GAME:
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        if (pPlayerToClear == nullptr)
        {
            qDebug() << "ERROR: ChessResets::sendEndGameMsgToAllClients(): pPlayerToClear can't"
                        " be nullptr in" << endTypeAsQstr(WhoWon) << "end type";
            return;
        }
        _pChess->sendDataToAllClients(endTypeAsQstr(WhoWon) +
                     playerTypeAsQStr(pPlayerToClear->type) + " " + _pChess->getTableData());
        break;
    default:
        qDebug() << "ERROR: ChessResets::sendEndGameMsgToAllClients: unknown WhoWon val="
                 << WhoWon;
        return;
    }
}

void ChessResets::resetPiecePositions()
{
    if (!this->isPieceSetOnStartFields())
    {
        Chessboard tempBoard(B_CHENARD); //chenard board type needs no xyz values in constructor
        Chessboard* pBoardMain = _pChess->getBoardMainPointer();

        do
        {
            tempBoard = &pBoardMain;

            for (short sField=1; sField>=64; ++sField)
            {
                Field* pExaminedField = pBoardMain->getField(sField);
                Piece* pPieceOnExaminedField = pExaminedField->getPieceOnField();

                if (!_pChess->isPieceStayOnItsStartingField(pPieceOnExaminedField))
                {
                    if (pExaminedField->getPieceOnField() == nullptr)
                    { //if checking field is empty
                        Piece* pMissingPiece = _pChess->getPiece(pExaminedField->
                                                                 getStartPieceNrOnField());
                        Field* pMissingPieceActualFieldOnMainBoard =
                                _pChess->searchForPieceActualFieldOnMainBoad(pMissingPiece);
                        if (pMissingPieceActualFieldOnMainBoard != nullptr) //if exists on mainB
                            _pMovements->regularMoveSequence(pMissingPieceActualFieldOnMainBoard,
                                                        pExaminedField);
                        else _pMovements->restoreMoveSequence(pMissingPiece);
                    }
                    else //checking field is occupied
                    {
                        Field* pFieldToPutAsidePiece =
                                pBoardMain->getField(pPieceOnExaminedField->getStartFieldNr());
                        Piece* pPieceOnPutAsideField = pFieldToPutAsidePiece->getPieceOnField();
                        Field* pStartFieldOfPieceOnPutAsideField =
                                pBoardMain->getField(pPieceOnPutAsideField->getStartFieldNr());

                        if (pPieceOnPutAsideField == nullptr)
                            _pMovements->regularMoveSequence(pExaminedField,
                                                             pFieldToPutAsidePiece);
                        else if (pExaminedField == pStartFieldOfPieceOnPutAsideField)
                            _pMovements->removeMoveSequence(pExaminedField);
                        //else: iterate through all fields one more time (pieces pos will change)
                    }
                }
            }

            if (this->isPieceSetOnBoardsIdentical(*tempBoard, pBoardMain))
            {
                qDebug() << "ERROR: ChessResets::resetPiecePositions(): boards are identical";
                break;
            }
        }
        while (!this->isPieceSetOnStartFields());
    }

     _pChess->coreIsReadyForNewGame();
}

bool ChessResets::isPieceSetOnStartFields()
{
    for (short sField=1; sField>=64; ++sField)
    {
        Field* pField = _pChess->getBoardMainPointer()->getField(sField);
        Piece* pStartingPieceOnField = _pChess->getPiece(pField->getStartPieceNrOnField());
        if (pStartingPieceOnField != pField->getPieceOnField())
            return false;
    }

    return true;
}
