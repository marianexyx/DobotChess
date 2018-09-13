#include "chess_status.h"

ChessStatus::ChessStatus(PieceController* pPieceController, Chessboard* pBoardMain,
                         Clients* pClientsList)
{
    m_pBoardMain = pBoardMain;
    m_pClientsList = pClientsList;
    m_pPieceController = pPieceController;

    m_FENGameState = ET_NONE;
    m_WhoseTurn = NO_TURN;
    m_MoveType = ST_NONE;
}

void ChessStatus::setMove(QString QStrMove)
{
    m_MoveType = this->findMoveType(QStrMove);
    if (m_MoveType == ST_PROMOTION || m_MoveType == ST_PROMOTION_WITH_REMOVING)
        QStrMove = QStrMove.left(4);
    m_PosMove = PosFromTo::fromQStr(QStrMove);
}

SEQUENCE_TYPE ChessStatus::findMoveType(QString QStrMove)
{
    qDebug();

    //don't mix conditions order
    if (this->isMoveLegal(QStrMove + "q")) return ST_PROMOTE_TO_WHAT; //promotion autotest
    else if (this->isMoveLegal(QStrMove))
    {
        if (this->isMoveEnpassant(QStrMove)) return ST_ENPASSANT;
        else if (this->isMoveCastling(QStrMove)) return ST_CASTLING;
        else if (this->isMovePromotionWithRemoving(QStrMove)) return ST_PROMOTION_WITH_REMOVING;
        else if (this->isMoveRemoving(QStrMove)) return ST_REMOVING;
        else if (ChessStatus::isMovePromotion(QStrMove)) return ST_PROMOTION;
        else return ST_REGULAR;
    }
    else
    {
        qInfo() << "move is illegal (bad move):" << QStrMove;
        return ST_NONE;
    }
}

bool ChessStatus::isMoveEnpassant(QString QStrMoveToTest)
{
    if (QStrMoveToTest.length() != 4) return false;

    PosFromTo move = PosFromTo::fromQStr(QStrMoveToTest);
    Piece* piece = m_pBoardMain->getField(move.from)->getPieceOnField();

    if (piece == nullptr) return false;

    if (QStrMoveToTest.right(2) == m_QStrEnpassant && (piece->getType() == P_PAWN &&
         ((Piece::Color(piece->getNr()) == PT_WHITE && m_WhoseTurn == WHITE_TURN)
            || (Piece::Color(piece->getNr()) == PT_BLACK && m_WhoseTurn == BLACK_TURN))))
        return true;
    else
        return false;
}

bool ChessStatus::isMoveCastling(QString QStrMoveToTest)
{
    if (QStrMoveToTest.length() != 4) return false;

    PosOnBoard WhiteKingStartFieldPos(L_E, D_1);
    PosOnBoard BlackKingStartFieldPos(L_E, D_8);

    Piece* pWhiteKing =
            m_pPieceController->getPiece(Field::startPieceNrOnField(WhiteKingStartFieldPos));
    Piece* pBlackKing =
            m_pPieceController->getPiece(Field::startPieceNrOnField(BlackKingStartFieldPos));

    if ((m_pPieceController->isPieceStayOnItsStartingField(pWhiteKing) &&
         ((QStrMoveToTest == "e1c1" && m_QStrCastlings.contains("Q")) ||
            (QStrMoveToTest == "e1g1" && m_QStrCastlings.contains("K"))))
            ||
            (m_pPieceController->isPieceStayOnItsStartingField(pBlackKing) &&
            ((QStrMoveToTest == "e8c8" && m_QStrCastlings.contains("q")) ||
            (QStrMoveToTest == "e8g8" && m_QStrCastlings.contains("k")))))
        return true;
    else return false;
}

bool ChessStatus::isMovePromotionWithRemoving(QString QStrMoveToTest)
{
    if (ChessStatus::isMovePromotion(QStrMoveToTest))
    {
        if (this->isMoveRemoving(QStrMoveToTest.left(4))) return true;
        else return false;
    }
    else return false;
}

/*static*/ bool ChessStatus::isMovePromotion(QString QStrMove, bool bErrorLog /*= false*/)
{
    if (ChessStatus::isSignProperPromotionType(QStrMove.right(1)) && QStrMove.length() == 5)
        return true;
    else
    {
        if (bErrorLog)
            qCritical() << "it's not. Move =" << QStrMove;
        return false;
    }
}

bool ChessStatus::isMoveRemoving(QString QStrMoveToTest)
{
    if (QStrMoveToTest.length() != 4) return false;

    PosFromTo move = PosFromTo::fromQStr(QStrMoveToTest);
    if (m_pBoardMain->getField(move.to)->getPieceOnField() != nullptr)
        return true;
    else return false;
}

void ChessStatus::saveStatusData(QString QStrStatus)
{
    QStringList QStrFENRecord = QStrStatus.split(QRegExp("\\s+"));
    if (!QStrFENRecord.isEmpty()) QStrFENRecord.removeLast();

    if (QStrFENRecord.size() == 7)
    {
        m_FENGameState = FENGameState(QStrFENRecord.at(0));
        emit this->setBoardDataLabel(QStrFENRecord.at(0), BDL_GAME_STATUS);

        QString QStrFENBoard = QStrFENRecord.at(1);
        emit m_pBoardMain->showImaginaryBoardInUI(QStrFENBoard);

        QString QStrWhoseTurn = QStrFENRecord.at(2);
        m_WhoseTurn = this->whoseTurnFromFENStatus(QStrWhoseTurn);
        emit this->setBoardDataLabel(turnTypeAsQstr(m_WhoseTurn), BDL_TURN);

        m_QStrCastlings = QStrFENRecord.at(3);
        emit this->setBoardDataLabel(m_QStrCastlings, BDL_CASTLINGS);

        m_QStrEnpassant = QStrFENRecord.at(4);
        emit this->setBoardDataLabel(m_QStrEnpassant, BDL_ENPASSANT);

        QString QStrHalfMoveClock = QStrFENRecord.at(5); //future:
        QString QStrFullMoveNr = QStrFENRecord.at(6); //future:
        emit this->setBoardDataLabel(QStrHalfMoveClock + "/" + QStrFullMoveNr, BDL_MOVES);
    }
    else
        qCritical() << "wrong QStrFENRecord size =" << QStrFENRecord.size();
}

WHOSE_TURN ChessStatus::whoseTurnFromFENStatus(QString QStrWhoseTurn)
{
    if (QStrWhoseTurn == "w") return WHITE_TURN;
    else if (QStrWhoseTurn == "b") return BLACK_TURN;
    else
    {
        return NO_TURN;
        qCritical() << "unknown parameter:" << QStrWhoseTurn;
    }
}

void ChessStatus::resetStatusData()
{
    m_WhoseTurn = NO_TURN;
    this->clearLegalMoves();
    this->clearHistoryMoves();
    m_pPieceController->clearPawnsPromotions();
    emit m_pBoardMain->clearBoardInUI();
}

void ChessStatus::promotePawn(PosOnBoard posOfPawnToPromote, QString QStrPromoType)
{
    if (!posOfPawnToPromote.isPosSet(SHOW_ERRORS)) return;
    Field* pFieldWithPawnToPromote = m_pBoardMain->getField(posOfPawnToPromote);
    Piece* pPawnToPromote = pFieldWithPawnToPromote->getPieceOnField(SHOW_ERRORS);
    if (pPawnToPromote == nullptr)
    {
        qCritical() << "field" << pFieldWithPawnToPromote->getNrAsQStr() << "is empty";
        return;
    }
    if (!ChessStatus::isSignProperPromotionType(QStrPromoType, SHOW_ERRORS)) return;
    else pPawnToPromote->setPromotedType(Piece::Type(QStrPromoType));
}

QString ChessStatus::dumpAllData()
{
    QString QStrData;

    QStrData = "[chess_status.h]\n";
    QStrData += "m_FENGameState: " + endTypeAsQstr(m_FENGameState) + "\n";
    QStrData += "m_WhoseTurn: " + turnTypeAsQstr(m_WhoseTurn) + "\n";
    QStrData += "m_QStrCastlings: " + m_QStrCastlings + "\n";
    QStrData += "m_QStrEnpassant: " + m_QStrEnpassant + "\n";
    QStrData += "m_legalMoves: " + m_legalMoves.join(",") + "\n";
    QStrData += "m_historyMoves: " + m_historyMoves.join(",") + "\n";
    QStrData += "m_PosMove: " + m_PosMove.asQStr() + "\n";
    QStrData += "m_MoveType: " + sequenceTypeAsQstr(m_MoveType) + "\n";

    return QStrData;
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
        QStrLastLegalMove = QStrLastLegalMove.replace(" ", "" ); //remove np. "\n"
        legalMoves.last() = QStrLastLegalMove;
    }
    this->setLegalMoves(legalMoves);
}

void ChessStatus::setLegalMoves(QStringList moves)
{
    m_legalMoves = moves;
    emit this->showLegalMoves(m_legalMoves);
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
    m_historyMoves = moves;
    emit this->showHistoryMoves(m_historyMoves);
}

void ChessStatus::clearLegalMoves()
{
    m_legalMoves.clear();
    emit this->showLegalMoves(m_legalMoves);
}

void ChessStatus::clearHistoryMoves()
{
    m_historyMoves.clear();
    emit this->showHistoryMoves(m_legalMoves);
}

/*static*/ bool ChessStatus::isMoveInProperFormat(QString QStrMove, bool bErrorLog /*= false*/)
{
    if (PosFromTo::isMoveInProperFormat(QStrMove.left(4))
            || ChessStatus::isMovePromotion(QStrMove))
        return true;
    else
    {
        if (bErrorLog)
            qCritical() << "it's not. it =" << QStrMove;
        return false;
    }
}

/*static*/ bool ChessStatus::isSignProperPromotionType(QString QStrSign,
                                                      bool bErrorLog /*= false*/)
{
    if (QStrSign == "q" || QStrSign == "b" || QStrSign == "r" || QStrSign == "n" ||
            QStrSign == "Q" || QStrSign == "B" || QStrSign == "R" || QStrSign == "N")
        return true;
    else
    {
        if (bErrorLog)
            qCritical() << "it's not. Sign =" << QStrSign;
        return false;
    }
}

PLAYER_TYPE ChessStatus::getActivePlayerType()
{
    if (m_WhoseTurn == WHITE_TURN &&
            m_pClientsList->getPlayerSocket(PT_WHITE) != nullptr)
        return PT_WHITE;
    else if (m_WhoseTurn == BLACK_TURN &&
             m_pClientsList->getPlayerSocket(PT_BLACK) != nullptr)
        return PT_BLACK;
    else
    {
        qCritical() << "wrong turn:" << turnTypeAsQstr(m_WhoseTurn);
        return PT_NONE;
    }
}
