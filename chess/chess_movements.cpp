#include "chess_movements.h"

ChessMovements::ChessMovements(Chessboard2* pChessboard2Main, Chessboard2* pChessboard2Removed)
{
    _pChessboard2Main = pChessboard2Main;
    _pChessboard2Removed = pChessboard2Removed;
}

bool ChessMovements::isMoveRemoving()
{
    if (m_QStrBoard[PieceTo.Letter][PieceTo.Digit] != ".")
    {
        qDebug() << "m_QStrBoard[PieceTo.Letter:" << pieceLetterPosAsQStr(PieceTo.Letter) << "][PieceTo.Digit:" <<
                    PieceTo.Digit+1 << "] =" << m_QStrBoard[PieceTo.Letter][PieceTo.Digit];
        return true;
    }
    else return false;
}

bool ChessMovements::isMoveCastling(QString moveToTest)
{
    if ((moveToTest == "e1c1" && m_QStrCastlings.contains("Q") && m_QStrBoard[4][0] == "K") ||
            (moveToTest == "e1g1" && m_QStrCastlings.contains("K") && m_QStrBoard[4][0] == "K") ||
            (moveToTest == "e8c8" && m_QStrCastlings.contains("q") && m_QStrBoard[4][7] == "k") ||
            (moveToTest == "e8g8" && m_QStrCastlings.contains("k") && m_QStrBoard[4][7] == "k"))
        return true;
    else return false;
}

bool ChessMovements::isMoveEnpassant(QString moveToTest)
{
    if (((m_QStrBoard[PieceFrom.Letter][PieceFrom.Digit] == "P" && m_WhoseTurn == WHITE_TURN) ||
         (m_QStrBoard[PieceFrom.Letter][PieceFrom.Digit] == "p" && m_WhoseTurn == BLACK_TURN)) &&
            moveToTest.right(2) == m_QStrEnpassant)
        return true;
    else return false;
}

PositionOnBoard ChessMovements::findKingToMoveInCastling(PositionOnBoard FieldDest)
{
    PositionOnBoard KingFieldDest;
    KingFieldDest.Digit = FieldDest.Digit;
    if (FieldDest.Letter == L_C) KingFieldDest.Letter = L_A;
    else if (FieldDest.Letter == L_G) KingFieldDest.Letter = L_H;
    else qDebug() << "ERRORChessMovements::findKingToMoveInCastling: "
                     "wrong FieldDest.Letter val =" << FieldDest.Letter;
    return KingFieldDest;
}

PositionOnBoard ChessMovements::findRookToMoveInCastling(PositionOnBoard FieldDest)
{
    PositionOnBoard RookFieldDest;
    RookFieldDest.Digit = RookFieldDest.Digit;
    if (FieldDest.Letter == L_C) RookFieldDest.Letter = L_D;
    else if (FieldDest.Letter == L_G) RookFieldDest.Letter = L_F;
    else qDebug() << "ERRORChessMovements::findKingToMoveInCastling: "
                     "wrong FieldDest.Letter val =" << FieldDest.Letter;
    return RookFieldDest;
}

void ChessMovements::regularMoveSequence(Chess* pChess)
{
    this->listMovesForDobot(ST_REGULAR);
    //TODO: to wygląda jakby to robił dobot wszystko, ten ruch tj.
    this->MoveTcpPiece("move " + move);

    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPieceFrom(), VM_GRAB);
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPieceTo(), VM_PUT);

    pChess->MoveTcpPiece("move " + move); //todo: wbijam sie w private
}






