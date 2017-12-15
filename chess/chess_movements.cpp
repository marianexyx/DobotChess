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
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPosFrom(), VM_GRAB);
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPosTo(), VM_PUT);

    pChess->SendMsgToTcp("move " + move); //todo: wbijam sie w private
    //todo2: SendMsgToTcp robic w warstwie wyzej by nie powtarzac tego tutaj za kazdym razem
    //todo3: nie znam "move"
}

void ChessMovements::removeMoveSequence(Chess* pChess)
{
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPosTo(), VM_GRAB);
    this->goToSafeRemovedFieldIfNeeded(pChess);
    pChess->movePieceWithManipulator(_pChessboard2Removed,
                                     pChess->getPiece(pChess->getPosTo())->getStartField(),
                                     VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(pChess);
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPosFrom(), VM_GRAB);
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPosTo(), VM_PUT);

    pChess->SendMsgToTcp("move " + move);
}

void ChessMovements::restoreMoveSequence(Chess* pChess, short sPieceToRestore)
{
    this->goToSafeRemovedFieldIfNeeded(pChess);
    pChess->movePieceWithManipulator(_pChessboard2Removed, Field::Pos(sPieceToRestore), VM_GRAB);
    this->goToSafeRemovedFieldIfNeeded(pChess);
    pChess->movePieceWithManipulator(_pChessboard2Main,
                                     pChess->getPiece(sPieceToRestore)->getStartField(),
                                     VM_PUT);
}

void ChessMovements::castlingMoveSequence(Chess* pChess)
{
    pChess->movePieceWithManipulator(_pChessboard2Main,
                                     this->findKingToMoveInCastling(pChess->getPosFrom()),
                                     VM_GRAB);
    pChess->movePieceWithManipulator(_pChessboard2Main,
                                     this->findKingToMoveInCastling(pChess->getPosTo()),
                                     VM_PUT);
    pChess->movePieceWithManipulator(_pChessboard2Main,
                                     this->findRookToMoveInCastling(pChess->getPosFrom()),
                                     VM_GRAB);
    pChess->movePieceWithManipulator(_pChessboard2Main,
                                     this->findRookToMoveInCastling(pChess->getPosTo()),
                                     VM_PUT);

    this->SendMsgToTcp("move " + move);
}

void ChessMovements::enpassantMoveSequence(Chess* pChess)
{
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPosFrom(), VM_GRAB);
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPosTo(), VM_PUT);
    if (pChess->getPosFrom().Digit < pChess->getPosTo().Digit) //white move
    {
        PositionOnBoard pieceToRemoveInEnpassant(pChess->getPosTo().Letter,
                                                 pChess->getPosTo().Digit - 1);
        pChess->movePieceWithManipulator(_pChessboard2Main,
                                         pieceToRemoveInEnpassant,
                                         VM_GRAB);
    }
    else if (pChess->getPosFrom().Digit > pChess->getPosTo().Digit) //black move
    {
        PositionOnBoard pieceToRemoveInEnpassant(pChess->getPosTo().Letter,
                                                 pChess->getPosTo().Digit + 1);
        pChess->movePieceWithManipulator(_pChessboard2Main,
                                         pieceToRemoveInEnpassant,
                                         VM_GRAB);
    }
    else
    {
        qDebug() << "ERROR: ChessMovements::enpassantMoveSequence: no move "
                     "detected is digits axis (digits abs(from-to)==0)";
        return;
    }
    this->goToSafeRemovedFieldIfNeeded(pChess);
    pChess->movePieceWithManipulator(_pChessboard2Removed,
                                     pChess->getPiece(pChess->getPosTo())->getStartField(),
                                     VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(pChess);

    this->SendMsgToTcp("move " + move);
}
//todo: po wszystkich ruchach czyscic zmienne globalne from/to jezeli jeszcze beda istniec

//todo: niby sie rozni tylko przypisaniem innej bierki
void ChessMovements::promoteMoveSequence(Chess* pChess)
{
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPosFrom(), VM_GRAB);
    pChess->movePieceWithManipulator(_pChessboard2Main, pChess->getPosTo(), VM_PUT);
}

void ChessMovements::goToSafeRemovedFieldIfNeeded(Chess *pChess)
{
    DIGIT digitTo = pChess->getPosTo().Digit;
    if (digitTo == D_1) digitTo = D_2;
    else if (digitTo == D_4) digitTo = D_3;
    else
    {
        qDebug() << "ERROR: Chess::goToSafeRemovedField: unknown digitTo value =" << digitTo;
        return;
    }

    PositionOnBoard safeRemovedField(L_D, digitTo);
    pChess->movePieceWithManipulator(_pChessboard2Removed, safeRemovedField;
}
