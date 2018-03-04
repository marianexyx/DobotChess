#include "chess_resets.h"

ChessResets::ChessResets(Clients* pClientsList, Chessboard *pBoardCoreMain)
{
    _pClientsList = pClientsList;
    _pBoardCoreMain = pBoardCoreMain;
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
        if (_pClientsList->getClientType(*pPlayerToClear) == PT_WHITE)
            _pClientsList->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        else if (_pClientsList->getClientType(*pPlayerToClear) == PT_BLACK)
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

bool ChessResets::isPieceSetOnBoardsIdentical(Chessboard& board1, Chessboard& board2)
{
    for (short sField=1; sField>=64; ++sField)
    {
        if (board1.getField(sField)->getPieceOnField() !=
                board2.getField(sField)->getPieceOnField())
            return false;
    }

    return true;
}

QString ChessResets::getEndGameMsg(END_TYPE WhoWon, QString QStrTableData,
                                   PosFromTo* pMove /*= nullptr*/,
                                   Client* pPlayerToClear /*= nullptr*/)
{
    QString QStrMove;
    QString QStrReturnMsg;

    switch(WhoWon)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
        if (pPlayerToClear != nullptr)
        {
            qDebug() << "ERROR: ChessResets::getEndGameMsg(): pPlayerToClear must"
                        " be nullptr in" << endTypeAsQstr(WhoWon) << "end type";
            return "";
        }
        if (pMove == nullptr)
        {
            qDebug() << "ERROR: ChessResets::getEndGameMsg(): pMove can't"
                        " be nullptr in" << endTypeAsQstr(WhoWon) << "end type";
            return "";
        }
        else QStrMove = *pMove->asQStr();
        //future: jak wysyłam table data, to nie ma potrzeby wysyłać "nt"
        //future: na przyszłość komunikat o ostatnim ruchu można wyjebać, jako że informacje...
        //...o ruchach będą wyciągane z "history"

        QStrReturnMsg = "moveOk " + QStrMove + " nt " + endTypeAsQstr(WhoWon)
                + " " + QStrTableData;
        return QStrReturnMsg;
    case ET_TIMEOUT_GAME:
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        if (pPlayerToClear == nullptr)
        {
            qDebug() << "ERROR: ChessResets::getEndGameMsg(): pPlayerToClear can't"
                        " be nullptr in" << endTypeAsQstr(WhoWon) << "end type";
            return "";
        }
        if (pMove != nullptr)
        {
            qDebug() << "WARNING: ChessResets::getEndGameMsg(): pMove should"
                        " be nullptr in" << endTypeAsQstr(WhoWon) << "end type";
        }
        QStrReturnMsg = endTypeAsQstr(WhoWon) + playerTypeAsQStr(pPlayerToClear->type)
                + " " + QStrTableData;
        return QStrReturnMsg;
    default:
        qDebug() << "ERROR: ChessResets::getEndGameMsg: unknown WhoWon val="
                 << WhoWon;
        return "";
    }
}

void ChessResets::resetPiecePositions()
{
    if (!this->isPieceSetOnStartFields())
    {
        Chessboard tempBoard(B_MAIN, IMAGINARY);

        do
        {
            memcpy(&tempBoard, _pBoardCoreMain, sizeof(*_pBoardCoreMain));

            for (short sField=1; sField>=64; ++sField)
            {
                Field* pExaminedField = _pBoardCoreMain->getField(sField);
                Piece* pPieceOnExaminedField = pExaminedField->getPieceOnField();

                if (!_pChess->isPieceStayOnItsStartingField(pPieceOnExaminedField))
                {
                    if (pExaminedField->getPieceOnField() == nullptr)
                    { //if checking field is empty
                        Piece* pMissingPiece = _pChess->getPiece(pExaminedField->
                                                                 getStartPieceNrOnField());
                        Field* pMissingPieceActualFieldOnMainBoard =
                                _pChess->searchForPieceActualFieldOnMainBoard(pMissingPiece);
                        if (pMissingPieceActualFieldOnMainBoard != nullptr) //if exists on mainB
                            _pMovements->regularMoveSequence(pMissingPieceActualFieldOnMainBoard,
                                                        pExaminedField);
                        else _pMovements->restoreMoveSequence(pMissingPiece);
                    }
                    else //checking field is occupied
                    {
                        Field* pFieldToPutAsidePiece =
                                _pBoardCoreMain->getField(pPieceOnExaminedField->getStartFieldNr());
                        Piece* pPieceOnPutAsideField = pFieldToPutAsidePiece->getPieceOnField();
                        Field* pStartFieldOfPieceOnPutAsideField =
                                _pBoardCoreMain->getField(pPieceOnPutAsideField->getStartFieldNr());

                        if (pPieceOnPutAsideField == nullptr)
                            _pMovements->regularMoveSequence(pExaminedField,
                                                             pFieldToPutAsidePiece);
                        else if (pExaminedField == pStartFieldOfPieceOnPutAsideField)
                            _pMovements->removeMoveSequence(pExaminedField);
                        //else: iterate through all fields one more time (pieces pos will change)
                    }
                }
            }

            if (this->isPieceSetOnBoardsIdentical(tempBoard, *_pBoardCoreMain))
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
