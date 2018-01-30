#include "chess_status.h"

ChessStatus::ChessStatus(Chess *pChess)
{
    _pChess = pChess;
    _pBoardMain = _pChess->getBoardMainPointer();
    _pClientsList = _pChess->getClientsPointer();

    _WhoseTurn = NO_TURN;
}

bool ChessStatus::isMoveRemoving()
{
    PosFromTo MoveTo = _pChess->getMovementsPointer()->getMove().to;
    if (_pBoardMain->getField(MoveTo)->getPieceOnField() != nullptr)
        return true;
    else return false;
}

static bool ChessStatus::isSignProperPromotionType(QString QStrSign, bool bErrorLog = false)
{
    if (QStrSign == "q" || QStrSign == "b" || QStrSign == "r" || QStrSign == "k" ||
            QStrSign == "Q" || QStrSign == "B" || QStrSign == "R" || QStrSign == "K")
        return true;
    else
    {
        if (bErrorLog)
            qDebug() << "ERROR:: ChessStatus::isSignProperPromotionType(): it's not. "
                        "Sign =" << QStrSign;
        return false;
    }
}

static bool ChessStatus::isMovePromotion(QString QStrMove, bool bErrorLog = false)
{
    if (this->isSignProperPromotionType(QStrMove.right(1), SHOW_ERRORS)
            && QStrMove.length() == 5)
        return true;
    else
    {
        if (bErrorLog)
            qDebug() << "ERROR:: ChessStatus::isMovePromotion(): it's not. Move =" << QStrMove;
        return false;
    }
}

bool ChessStatus::isMoveCastling(QString QStrMoveToTest)
{
    PosOnBoard WhiteKingStartFieldPos(L_E, D_1);
    PosOnBoard BlackKingStartFieldPos(L_E, D_8);

    Piece* WhiteKing = _pChess->getPiece(Field::startPieceNrOnField(WhiteKingStartFieldPos));
    Piece* BlackKing = _pChess->getPiece(Field::startPieceNrOnField(BlackKingStartFieldPos));

    if (_pChess->isPieceStayOnItsStartingField(WhiteKing) &&
         ((QStrMoveToTest == "e1c1" && _QStrCastlings.contains("Q")) ||
            (QStrMoveToTest == "e1g1" && _QStrCastlings.contains("K"))) ||
            _pChess->isPieceStayOnItsStartingField(BlackKing) &&
            (QStrMoveToTest == "e8c8" && _QStrCastlings.contains("q")) ||
            (QStrMoveToTest == "e8g8" && _QStrCastlings.contains("k")))
        return true;
    else return false;
}

bool ChessStatus::isMoveEnpassant(QString QStrMoveToTest)
{
    PosFromTo MoveFrom = _pChess->getMovementsPointer()->getMove().from;
    PIECE_TYPE PieceType = _pBoardMain->getField(MoveFrom)->getPieceOnField()->getType();
    if (QStrMoveToTest.right(2) == _QStrEnpassant && (PieceType == P_PAWN &&
         ((Piece::Color(PieceType) == PT_WHITE && _WhoseTurn == WHITE_TURN)
            || (Piece::Color(PieceType) == PT_BLACK && _WhoseTurn == BLACK_TURN))))
        return true;
    else return false;
}

void ChessStatus::saveStatusData(QString status)
{
    QStringList QStrFENRecord = status.split(QRegExp("\\s+"));
    if (!QStrFENRecord.isEmpty()) QStrFENRecord.removeLast();

    if (QStrFENRecord.size() == 7)
    {
        _FENGameState = FENGameState(QStrFENRecord.at(0));
        qDebug() << "ChessStatus::saveStatusData(): FEN game state =" << QStrFENRecord.at(0);
        emit _pChess->setBoardDataLabel(QStrFENRecord.at(0), BDL_GAME_STATUS);

        QString QStrFENBoard = QStrFENRecord.at(1);
        qDebug() << "ChessStatus::saveStatusData(): QStrFENBoard =" << QStrFENBoard;
        emit _pBoardMain->showBoardInUI(QStrFENBoard, _pBoardMain->getBoardType());

        QString QStrWhoseTurn = QStrFENRecord.at(2);
        qDebug() << "ChessStatus::saveStatusData(): QStrWhoseTurn =" << QStrWhoseTurn;
        _WhoseTurn = this->whoseTurn(QStrWhoseTurn);
        emit _pChess->setBoardDataLabel(this->getStrWhoseTurn(), BDL_TURN);

        _QStrCastlings = QStrFENRecord.at(3);
        qDebug() << "ChessStatus::saveStatusData(): QStrCastlings =" << _QStrCastlings;
        emit _pChess->setBoardDataLabel(_QStrCastlings, BDL_CASTLINGS);

        _QStrEnpassant = QStrFENRecord.at(4);
        qDebug() << "ChessStatus::saveStatusData(): QStrEnpassant =" << _QStrEnpassant;
        emit _pChess->setBoardDataLabel(_QStrEnpassant, BDL_ENPASSANT);

        QString QStrHalfMoveClock = QStrFENRecord.at(5);
        QString QStrFullMoveNr = QStrFENRecord.at(6);
        emit _pChess->setBoardDataLabel(QStrHalfMoveClock + "/" + QStrFullMoveNr, BDL_MOVES);
    }
    else
        qDebug() << "ERROR: ChessStatus::saveStatusData(): wrong QStrFENRecord size ="
                 << QStrFENRecord.size();
}

void ChessStatus::resetStatusData()
{
    _pChess->getMovementsPointer()->clearMove();
    this->setWhoseTurn(NO_TURN);
    this->clearLegalMoves();
    this->clearHistoryMoves();
    emit _pBoardMain->clearBoardInUI();
}

void ChessStatus::setLegalMoves(QString msg)
{
    QStringList legalMoves = msg.split(QRegExp("\\s"));
    if (!legalMoves.isEmpty()) legalMoves.removeFirst(); //remove "ok"
    if (!legalMoves.isEmpty()) legalMoves.removeFirst(); //remove np. "20"

    if (!legalMoves.isEmpty())
    {
        QString QStrLastLegalMove = legalMoves.last();
        QStrLastLegalMove = QStrLastLegalMove.simplified();
        QStrLastLegalMove = QStrLastLegalMove.replace( " ", "" ); //remove np. "\n"
        legalMoves.last() = QStrLastLegalMove;
    }
    this->setLegalMoves(legalMoves);
}

void ChessStatus::setHistoryMoves(QString msg)
{
    QStringList historyMoves = msg.split(QRegExp("\\s"));
    if (!historyMoves.isEmpty()) historyMoves.removeFirst(); //remove "ok"
    if (!historyMoves.isEmpty()) historyMoves.removeFirst(); //remove f.e. "20"

    if (!historyMoves.isEmpty()) //whipe CR, LF, spacebars etc
    {
        QString QStrLastHistoryMove = historyMoves.last();
        QStrLastHistoryMove = QStrLastHistoryMove.simplified();
        QStrLastHistoryMove = QStrLastHistoryMove.replace( " ", "" ); //remove f.e. "\n"
        historyMoves.last() = QStrLastHistoryMove;
    }

    this->setHistoryMoves(historyMoves);
}

WHOSE_TURN ChessStatus::whoseTurn(QString QStrWhoseTurn)
{
    if (QStrWhoseTurn == "w") return WHITE_TURN;
    else if (QStrWhoseTurn == "b") return BLACK_TURN;
    else
    {
        return NO_TURN;
        qDebug () << "ERROR: ChessStatus::whoseTurn(); unknown parameter:" << QStrWhoseTurn;
    }
}

QString ChessStatus::getStrWhoseTurn()
{
    if (_WhoseTurn == WHITE_TURN) return "wt";
    else if (_WhoseTurn == BLACK_TURN) return "bt";
    else if (_WhoseTurn == NO_TURN) return "nt";
    else
    {
        QString err = "ERROR: ChessStatus::getStrWhoseTurn(): wrong turn type: "
                + QString::number(_WhoseTurn);
        return err;
    }
}

PLAYER_TYPE ChessStatus::getActivePlayerType()
{
    if (_WhoseTurn == WHITE_TURN &&
            _pClientsList->getPlayerSocket(PT_WHITE) != nullptr)
        return PT_WHITE;
    else if (_WhoseTurn == BLACK_TURN &&
             _pClientsList->getPlayerSocket(PT_BLACK) != nullptr)
        return PT_BLACK;
    else
    {
        qDebug() << "ERROR: ChessStatus::getActivePlayerType(): wrong turn:"
                 << turnTypeAsQstr(_WhoseTurn);
        return PT_NONE;
    }
}
