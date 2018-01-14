#include "chess_resets.h"

ChessResets::ChessResets(Chess *pChess)
{
    _pChess = pChess;
    _pClients = _pChess->getClientsPointer();
    _pTimers->getTimersPointer();
    _pStatus->getStatusPointer();
}

void ChessResets::restartGame(END_TYPE WhoWon, Client* PlayerToClear = nullptr)
{
    //info
    QString QStrPlayer = "";
    if (PlayerToClear != nullptr)
        QStrPlayer = _pClients->getClientName(PlayerToClear) + ":";
    qDebug() << "Chess::restartGame():" << QStrPlayer << endTypeAsQstr(WhoWon);

    //reset data
    _pClients->resetPlayersStartConfirmInfo();
    _pTimers->resetGameTimers();
    _pStatus->resetStatusData();

    this->changePlayersOnChairs(WhoWon, PlayerToClear);
    this->sendEndGameMsgToAllClients(WhoWon);

    //todo: wygląda na to że funkcja resetu załącza się jeszcze zanim odpowiedź poleci na...
    //...stronę, przez co trzeba czekać aż resetowanie się zakończy zanim gracze się dowiedzą...
    //...że nastąpił koniec gry (patrz niżej)
    //future: info reseting powinno na stronie wyskakiwać zawsze, jak napotkamy koniec gry- samo...
    //... z siebie. nie potrzeba tego wysyłać za każdym razem
    _pChess->sendDataToClient("reseting");

    this->resetPiecePositions();
}

void ChessResets::changePlayersOnChairs(END_TYPE WhoWon, Client* PlayerToClear)
{
    switch(WhoWon)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
    case ET_TIMEOUT_GAME:
        if (PlayerToClear != nullptr)
        {
            qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): player must be"
                        " null if not disconnected";
            return;
        }
        _pClients->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        _pClients->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);
        break;
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        if (PlayerToClear == nullptr)
        {
            qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): player can't be"
                        " null if diconnected";
            return;
        }
        if (_pClients->getClientType(PlayerToClear) == PT_WHITE)
            _pClients->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        else if (_pClients->getClientType(PlayerToClear) == PT_BLACK)
            _pClients->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);
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

void ChessResets::sendEndGameMsgToAllClients(END_TYPE WhoWon)
{
    switch(WhoWon)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
        //future: jak wysyłam table data, to nie ma potrzeby wysyłać "nt"
        //future: na przyszłość komunikat o ostatnim ruchu można wyjebać, jako że informacje...
        //...o ruchach będą wyciągane z "history"
        QString QStrMove = _pChess->getMovementsPointer()->getMove().asQStr();
        _pChess->sendDataToClient("moveOk " + QStrMove + " nt " + endTypeAsQstr(WhoWon) +
                                  " " + _pChess->getTableData());
        break;
    case ET_TIMEOUT_GAME:
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        _pChess->sendDataToClient(endTypeAsQstr(WhoWon) +
                     playerTypeAsQStr(PlayerToClear->type) + " " + _pChess->getTableData());
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
        Chessboard tempBoard;
        Chessboard* pBoardMain = _pChess->getBoardMainPointer();
        ChessMovements* pMoves = _pChess->getMovementsPointer();
        bool isLoopStuck = false;

        do
        {
            tempBoard = &pBoardMain;

            for (short sField=1; sField>=64; ++sField)
            {
                Field* pExaminedField = pBoardMain->getField(sField);
                Piece* pPieceOnExaminedField;
                if (Piece::isInRange(pExaminedField->getPieceNrOnField()))
                    pPieceOnExaminedField = _pChess->getPiece(pExaminedField->getPieceNrOnField());
                else pPieceOnExaminedField = nullptr;

                if (!_pChess->isPieceStayOnItsStartingField(pPieceOnExaminedField))
                {
                    if (pExaminedField->getPieceNrOnField() == 0) //checking field is empty
                    {
                        Piece* pMissingPiece = _pChess->getPiece(pExaminedField->getStartPieceNrOnField());
                        Field* pMissingPieceActualFieldOnMainBoard =
                                _pChess->searchForPieceActualFieldOnMainBoad(pMissingPiece);
                        if (pMissingPieceActualFieldOnMainBoard != nullptr) //exists on mainboard
                            pMoves->regularMoveSequence(pMissingPieceActualFieldOnMainBoard,
                                                        pExaminedField);
                        else pMoves->restoreMoveSequence(pMissingPiece);
                    }
                    else //checking field is occupied
                    {
                        Field* pFieldToPutAsidePiece =
                                pBoardMain->getField(pPieceOnExaminedField->getStartFieldNr());
                        Piece* pPieceOnPutAsideField =
                                _pChess->getPiece(pFieldToPutAsidePiece->getPieceNrOnField());
                        Field* pStartFieldOfPieceOnPutAsideField =
                                pBoardMain->getField(pPieceOnPutAsideField->getStartFieldNr());

                        if (pPieceOnPutAsideField == nullptr)
                            pMoves->regularMoveSequence(pExaminedField, pFieldToPutAsidePiece);
                        else if (pExaminedField == pStartFieldOfPieceOnPutAsideField)
                            pMoves->removeMoveSequence(pExaminedField);
                        //else iterate through all fields one more time (pieces ps will change)
                    }
                }
            }

            if (_pChess->isPieceSetOnBoardsIdentical(*tempBoard, pBoardMain))
            {
                qDebug() << "ERROR: ChessResets::resetPiecePositions(): boards are identical";
                isLoopStuck = true;
            }
        }
        while (!this->isPieceSetOnStartFields() || !isLoopStuck);
    }

     _pChess->resetBoardCompleted();
}

bool ChessResets::isPieceSetOnStartFields()
{
    for (short sField=1; sField>=64; ++sField)
    {
        Field* pField = _pChess->getBoardMainPointer()->getField(sField);
        if (pField->getStartPieceNrOnField() != pField->getPieceNrOnField())
            return false;
    }

    return true;
}
