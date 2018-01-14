#include "chess_status.h"

ChessStatus::ChessStatus(Chess *pChess)
{
    _pChess = pChess;
    _pBoardMain = _pChess->getBoardMainPointer();

    _WhoseTurn = NO_TURN;
}

QString **ChessStatus::FENToBoard(QString FENBoard)
{
    QString** QStrBoardArray = create2DArray();

    QStringList QStrFENBoardRows = FENBoard.split(QRegExp("/"));
    std::reverse(QStrFENBoardRows.begin(), QStrFENBoardRows.end());
    if (QStrFENBoardRows.size() == 8)
    {
        QRegExp rxEmpty("\\d");
        for (int nRow=0; nRow<=7; ++nRow)
        {
            int nColumn = 0;
            QString QStrFENBoardRow = QStrFENBoardRows.at(nRow);
            QStringList FENSigns = QStrFENBoardRow.split("");
            FENSigns.removeFirst();
            FENSigns.removeLast();

            for (int nFENSignPos=0; nFENSignPos<FENSigns.size(); ++nFENSignPos)
            {
                QString QStrFENSign = FENSigns.at(nFENSignPos);
                if (!rxEmpty.exactMatch(QStrFENSign)) //not digits
                {
                    QStrBoardArray[nColumn][nRow] = QStrFENSign;
                    if (nColumn>7) qDebug() << "ERROR: ChessStatus::FENToBoard: nColumn > 8 =" << nColumn;
                    ++nColumn;
                }
                else //digits
                {
                    for (int nEmptyFields=1; nEmptyFields<=QStrFENSign.toInt(); ++nEmptyFields)
                    {
                        QStrBoardArray[nColumn][nRow] = ".";
                        if (nColumn>7) qDebug() << "ERROR: ChessStatus::FENToBoard: nColumn > 8 =" << nColumn;
                        ++nColumn;
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "ERROR: ChessStatus::FENToBoard: boardRows.size() != 8. Size = " <<
                    QStrFENBoardRows.size();
        for (int i=0; i<=QStrFENBoardRows.size()-1; ++i)
            qDebug() << "QStrFENBoardRows at" << i << "=" << QStrFENBoardRows.at(i);
    }

    return QStrBoardArray;
}

bool ChessStatus::isMoveRemoving()
{
    PosFromTo MoveTo = _pChess->getMovementsPointer()->getMove().to;
    if (_pBoardMain->getField(MoveTo)->getPieceNrOnField() > 0)
        return true;
    else return false;
}

bool ChessStatus::isMovePromotion(QString QStrMoveToTest)
{
    if ((QStrMoveToTest.right(1) == "q" || QStrMoveToTest.right(1) == "r" ||
         QStrMoveToTest.right(1) == "b" || QStrMoveToTest.right(1) == "k")
            && QStrMoveToTest.length() == 5 )
        return true;
    else return false;
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
    PIECE_TYPE piece = Piece::Type(_pBoardMain->getField(MoveFrom)->getPieceNrOnField());
    if ((piece == P_PAWN && ((Piece::Color(piece) == PT_WHITE && _WhoseTurn == WHITE_TURN) ||
                            (Piece::Color(piece) == PT_BLACK && _WhoseTurn == BLACK_TURN))) &&
            QStrMoveToTest.right(2) == _QStrEnpassant)
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
        qDebug() << "FEN game state =" << QStrFENRecord.at(0);
        emit setBoardDataLabels(QStrFENRecord.at(0), BDL_GAME_STATUS);

        QString QStrFENBoard = QStrFENRecord.at(1);
        qDebug() << "QStrFENBoard =" << QStrFENBoard;
        this->FENToBoard(QStrFENBoard);
        emit showBoardInForm(Chessboard::arrayBoardToQStr(_QStrBoard));

        QString QStrWhoseTurn = QStrFENRecord.at(2);
        qDebug() << "QStrWhoseTurn =" << QStrWhoseTurn;
        this->setWhoseTurn(whoseTurn(QStrWhoseTurn));
        emit setBoardDataLabels(this->getStrWhoseTurn(), BDL_TURN);

        _QStrCastlings = QStrFENRecord.at(3);
        qDebug() << "QStrCastlings =" << _QStrCastlings;
        emit setBoardDataLabels(_QStrCastlings, BDL_CASTLINGS);

        _QStrEnpassant = QStrFENRecord.at(4);
        qDebug() << "QStrEnpassant =" << _QStrEnpassant;
        emit setBoardDataLabels(_QStrEnpassant, BDL_ENPASSANT);

        QString QStrHalfmoveClock = QStrFENRecord.at(5);
        QString QStrFullmoveNumber = QStrFENRecord.at(6);
        emit setBoardDataLabels(QStrHalfmoveClock + "/" + QStrFullmoveNumber , BDL_MOVES);
    }
    else
    {
        qDebug() << "ERROR: ChessStatus::saveStatusData: wrong QStrFENRecord size =" <<
                    QStrFENRecord.size();
        this->addTextToConsole("ERROR: ChessStatus::saveStatusData: wrong QStrFENRecord size = "
                               + QStrFENRecord.size());
    }
}

void ChessStatus::resetStatusData()
{
    this->setWhoseTurn(NO_TURN);
    this->clearLegalMoves();
    this->clearHistoryMoves();
    this->clearFormBoard();
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

END_TYPE ChessStatus::getFENGameStateAsEndType() const
{
    switch(_FENGameState)
    {
    case FGS_IN_PROGRESS: return ET_NONE; break;
    case FGS_WHITE_WON: return ET_WHIE_WON; break;
    case FGS_BLACK_WON: return ET_BLACK_WON; break;
    case FGS_DRAW: return ET_DRAW; break;
    default:
        qDebug() << "ERROR: ChessStatus::getFENGameStateAsEndType():"
                         "unknown _FENGameState val =" << _FENGameState;
        return ET_NONE;
    }
}

WHOSE_TURN ChessStatus::whoseTurn(QString QStrWhoseTurn)
{
    if (QStrWhoseTurn == "w") return WHITE_TURN;
    else if (QStrWhoseTurn == "b") return BLACK_TURN;
    else
    {
        return NO_TURN;
        qDebug () << "ERROR: ChessStatus::whoseTurn- unknown parameter:" << QStrWhoseTurn;
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
