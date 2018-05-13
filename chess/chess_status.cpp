#include "chess_status.h"

ChessStatus::ChessStatus(PieceController* pPieceController, Chessboard* pBoardMain,
                         Clients* pClientsList)
{
    _pBoardMain = pBoardMain;
    _pClientsList = pClientsList;
    _pPieceController = pPieceController;

    _WhoseTurn = NO_TURN;
}

/*static*/ bool ChessStatus::isSignProperPromotionType(QString QStrSign,
                                                      bool bErrorLog /*= false*/)
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

/*static*/ bool ChessStatus::isMovePromotion(QString QStrMove, bool bErrorLog /*= false*/)
{
    if (ChessStatus::isSignProperPromotionType(QStrMove.right(1)) && QStrMove.length() == 5)
        return true;
    else
    {
        if (bErrorLog)
            qDebug() << "ERROR:: ChessStatus::isMovePromotion(): it's not. Move =" << QStrMove;
        return false;
    }
}

/*static*/ bool ChessStatus::isMoveInProperFormat(QString QStrMove, bool bErrorLog /*= false*/)
{
    if (PosFromTo::isMoveInProperFormat(QStrMove.left(4))
            || ChessStatus::isMovePromotion(QStrMove))
        return true;
    else
    {
        if (bErrorLog)
            qDebug() << "ERROR: ChessStatus::isMoveInProperFormat(): it's not. it =" << QStrMove;
        return false;
    }
}

SEQUENCE_TYPE ChessStatus::findMoveType(QString QStrMove)
{
    if (this->isMoveLegal(QStrMove + "q")) return ST_PROMOTE_TO_WHAT; //promo autotest
    else if (this->isMoveLegal(QStrMove))
    {
        if (this->isMoveEnpassant(QStrMove)) return ST_ENPASSANT;
        else if (this->isMoveCastling(QStrMove)) return ST_CASTLING;
        else if (this->isMoveRemoving(QStrMove)) return ST_REMOVING;
        else if (this->isMovePromotion(QStrMove)) return ST_PROMOTION;
        else return ST_REGULAR;
    }
    else
    {
        qDebug() << "ChessStatus::findMoveType(): move is illegal (bad move):"
                 << QStrMove;
        return ST_NONE;
    }
}

bool ChessStatus::isMoveRemoving(QString QStrMoveToTest)
{
    //todo: a co jeśli zbijanie będzie się odbywać razem z promocją?
    if (QStrMoveToTest.length() != 4) return false;

    PosFromTo move = PosFromTo::fromQStr(QStrMoveToTest);
    if (_pBoardMain->getField(move.to)->getPieceOnField() != nullptr)
        return true;
    else return false;
}

bool ChessStatus::isMoveCastling(QString QStrMoveToTest)
{
    if (QStrMoveToTest.length() != 4) return false;

    PosOnBoard WhiteKingStartFieldPos(L_E, D_1);
    PosOnBoard BlackKingStartFieldPos(L_E, D_8);

    Piece* pWhiteKing =
            _pPieceController->getPiece(Field::startPieceNrOnField(WhiteKingStartFieldPos));
    Piece* pBlackKing =
            _pPieceController->getPiece(Field::startPieceNrOnField(BlackKingStartFieldPos));

    if ((_pPieceController->isPieceStayOnItsStartingField(pWhiteKing) &&
         ((QStrMoveToTest == "e1c1" && _QStrCastlings.contains("Q")) ||
            (QStrMoveToTest == "e1g1" && _QStrCastlings.contains("K"))))
            ||
            (_pPieceController->isPieceStayOnItsStartingField(pBlackKing) &&
            ((QStrMoveToTest == "e8c8" && _QStrCastlings.contains("q")) ||
            (QStrMoveToTest == "e8g8" && _QStrCastlings.contains("k")))))
        return true;
    else return false;
}

bool ChessStatus::isMoveEnpassant(QString QStrMoveToTest)
{
    if (QStrMoveToTest.length() != 4) return false;

    PosFromTo move = PosFromTo::fromQStr(QStrMoveToTest);
    Piece* piece = _pBoardMain->getField(move.from)->getPieceOnField();

    if (piece == nullptr) return false;

    if (QStrMoveToTest.right(2) == _QStrEnpassant && (piece->getType() == P_PAWN &&
         ((Piece::Color(piece->getNr()) == PT_WHITE && _WhoseTurn == WHITE_TURN)
            || (Piece::Color(piece->getNr()) == PT_BLACK && _WhoseTurn == BLACK_TURN))))
        return true;
    else
        return false;
}

void ChessStatus::saveStatusData(QString QStrStatus)
{
    QStringList QStrFENRecord = QStrStatus.split(QRegExp("\\s+"));
    if (!QStrFENRecord.isEmpty()) QStrFENRecord.removeLast();

    if (QStrFENRecord.size() == 7)
    {
        _FENGameState = FENGameState(QStrFENRecord.at(0));
        qDebug() << "ChessStatus::saveStatusData(): FEN game state =" << QStrFENRecord.at(0);
        emit this->setBoardDataLabel(QStrFENRecord.at(0), BDL_GAME_STATUS);

        QString QStrFENBoard = QStrFENRecord.at(1);
        qDebug() << "ChessStatus::saveStatusData(): QStrFENBoard =" << QStrFENBoard;
        emit _pBoardMain->showBoardInUI(QStrFENBoard, _pBoardMain->getBoardType());

        QString QStrWhoseTurn = QStrFENRecord.at(2);
        qDebug() << "ChessStatus::saveStatusData(): QStrWhoseTurn =" << QStrWhoseTurn;
        _WhoseTurn = this->whoseTurn(QStrWhoseTurn);
        emit this->setBoardDataLabel(turnTypeAsQstr(_WhoseTurn), BDL_TURN);

        _QStrCastlings = QStrFENRecord.at(3);
        qDebug() << "ChessStatus::saveStatusData(): QStrCastlings =" << _QStrCastlings;
        emit this->setBoardDataLabel(_QStrCastlings, BDL_CASTLINGS);

        _QStrEnpassant = QStrFENRecord.at(4);
        qDebug() << "ChessStatus::saveStatusData(): QStrEnpassant =" << _QStrEnpassant;
        emit this->setBoardDataLabel(_QStrEnpassant, BDL_ENPASSANT);

        QString QStrHalfMoveClock = QStrFENRecord.at(5); //future:
        QString QStrFullMoveNr = QStrFENRecord.at(6); //future:
        emit this->setBoardDataLabel(QStrHalfMoveClock + "/" + QStrFullMoveNr, BDL_MOVES);
    }
    else
        qDebug() << "ERROR: ChessStatus::saveStatusData(): wrong QStrFENRecord size ="
                 << QStrFENRecord.size();
}

void ChessStatus::resetStatusData()
{
    this->setWhoseTurn(NO_TURN);
    this->clearLegalMoves();
    this->clearHistoryMoves();
    emit _pBoardMain->clearBoardInUI();
}

void ChessStatus::setMove(QString QStrMove)
{
    _MoveType = this->findMoveType(QStrMove);
    qDebug() << "ChessStatus::setMove(): found move type ="
             << sequenceTypeAsQstr(_MoveType);
    if (_MoveType == ST_PROMOTION) QStrMove = QStrMove.left(4);
    _PosMove = PosFromTo::fromQStr(QStrMove);
}

void ChessStatus::setLegalMoves(QString QStrMsg)
{
    QStringList legalMoves = QStrMsg.split(QRegExp("\\s"));
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

void ChessStatus::setLegalMoves(QStringList moves)
{
    _legalMoves = moves;
    emit this->showLegalMoves(_legalMoves);
}

void ChessStatus::setHistoryMoves(QString QStrMsg)
{
    QStringList historyMoves = QStrMsg.split(QRegExp("\\s"));
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

void ChessStatus::setHistoryMoves(QStringList moves)
{
    _historyMoves = moves;
    emit this->showHistoryMoves(_legalMoves);
}

WHOSE_TURN ChessStatus::whoseTurn(QString QStrWhoseTurn)
{
    if (QStrWhoseTurn == "w") return WHITE_TURN;
    else if (QStrWhoseTurn == "b") return BLACK_TURN;
    else
    {
        return NO_TURN;
        qDebug() << "ERROR: ChessStatus::whoseTurn(); unknown parameter:" << QStrWhoseTurn;
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

void ChessStatus::clearLegalMoves()
{
    _legalMoves.clear();
    emit this->showLegalMoves(_legalMoves);
}

void ChessStatus::clearHistoryMoves()
{
    _historyMoves.clear();
    emit this->showHistoryMoves(_legalMoves);
}
