#include "chess_movements.h"

ChessMovements::ChessMovements(Chess *pChess, Websockets *pWebsockets,
                               Chessboard* pBoardMain, Chessboard* pBoardRemoved)
{
    _pChess = pChess;
    _pWebsockets = pWebsockets;
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;    
}

PosOnBoard ChessMovements::findKingPosInCastling(PosOnBoard FieldDest)
{
    PosOnBoard KingFieldDest;
    KingFieldDest.Digit = FieldDest.Digit;
    if (FieldDest.Letter == L_C) KingFieldDest.Letter = L_A;
    else if (FieldDest.Letter == L_G) KingFieldDest.Letter = L_H;
    else qDebug() << "ERROR: ChessMovements::findKingPosInCastling: "
                     "wrong FieldDest.Letter val =" << FieldDest.Letter;
    return KingFieldDest;
}

PosOnBoard ChessMovements::findRookPosInCastling(PosOnBoard FieldDest)
{
    PosOnBoard RookFieldDest;
    RookFieldDest.Digit = RookFieldDest.Digit;
    if (FieldDest.Letter == L_C) RookFieldDest.Letter = L_D;
    else if (FieldDest.Letter == L_G) RookFieldDest.Letter = L_F;
    else qDebug() << "ERROR: ChessMovements::findKingPosInCastling: "
                     "wrong FieldDest.Letter val =" << FieldDest.Letter;
    return RookFieldDest;
}

void ChessMovements::regularMoveSequence(Chess* pChess)
{
    pChess->movePieceWithManipulator(_pBoardMain, _PosMove.from, VM_GRAB);
    pChess->movePieceWithManipulator(_pBoardMain, _PosMove.to, VM_PUT);

    pChess->sendMsgToTcp("move " + move); //todo: wbijam sie w private
    //todo2: sendMsgToTcp robic w warstwie wyzej by nie powtarzac tego tutaj za kazdym razem
    //todo3: nie znam "move"
}

void ChessMovements::removeMoveSequence(Chess* pChess)
{
    short sPieceNrToRemove = _pBoardMain->getField(_PosMove.to)->getPieceNrOnField();
    Piece* PieceToRemove = pChess->getPiece(sPieceNrToRemove);

    pChess->movePieceWithManipulator(_pBoardMain, _PosMove.to, VM_GRAB);
    this->goToSafeRemovedFieldIfNeeded(*pChess);
    pChess->movePieceWithManipulator(_pBoardRemoved,
                                     PieceToRemove->getStartFieldPos(), VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(*pChess);

    this->regularMoveSequence(pChess);

    pChess->sendMsgToTcp("move " + move);
}

void ChessMovements::restoreMoveSequence(Chess* pChess, short sPieceToRestore)
{
    Piece* PieceToRestore = pChess->getPiece(sPieceToRestore);

    this->goToSafeRemovedFieldIfNeeded(*pChess);
    pChess->movePieceWithManipulator(_pBoardRemoved,
                                     PieceToRestore->getStartFieldPos(), VM_GRAB);
    this->goToSafeRemovedFieldIfNeeded(*pChess);
    pChess->movePieceWithManipulator(_pBoardMain,
                                     PieceToRestore->getStartFieldPos(), VM_PUT);
}

void ChessMovements::castlingMoveSequence(Chess* pChess)
{
    pChess->movePieceWithManipulator(_pBoardMain,
                                     this->findKingPosInCastling(_PosMove.from), VM_GRAB);
    pChess->movePieceWithManipulator(_pBoardMain,
                                     this->findKingPosInCastling(_PosMove.to), VM_PUT);
    pChess->movePieceWithManipulator(_pBoardMain,
                                     this->findRookPosInCastling(_PosMove.from), VM_GRAB);
    pChess->movePieceWithManipulator(_pBoardMain,
                                     this->findRookPosInCastling(_PosMove.to), VM_PUT);

    this->sendMsgToTcp("move " + move);
}

void ChessMovements::enpassantMoveSequence(Chess* pChess)
{
    pChess->movePieceWithManipulator(_pBoardMain, _PosMove.from, VM_GRAB);
    pChess->movePieceWithManipulator(_pBoardMain, _PosMove.to, VM_PUT);

    PosOnBoard PosOfPieceToRemoveInEnpassant = _PosMove.to;
    if (_PosMove.from.Digit < _PosMove.to.Digit) //white move
        PosOfPieceToRemoveInEnpassant.Digit -= 1;
    else if (_PosMove.from.Digit > _PosMove.to.Digit) //black move
        PosOfPieceToRemoveInEnpassant.Digit += 1;
    else
    {
        qDebug() << "ERROR: ChessMovements::enpassantMoveSequence: no move "
                     "detected is digits axis (digits abs(from-to)==0)";
        return;
    }

    Field* FieldWithPieceToRemove = _pBoardMain->getField(PosOfPieceToRemoveInEnpassant);
    short sPieceNrToRemoveInEnpassant = FieldWithPieceToRemove->getPieceNrOnField();
    Piece* PieceToRemoveInEnpassant = pChess->getPiece(sPieceNrToRemoveInEnpassant);

    pChess->movePieceWithManipulator(_pBoardMain,
                                     PosOfPieceToRemoveInEnpassant, VM_GRAB);
    this->goToSafeRemovedFieldIfNeeded(*pChess);
    pChess->movePieceWithManipulator(_pBoardRemoved,
                                     PieceToRemoveInEnpassant->getStartFieldPos(), VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(*pChess);

    this->sendMsgToTcp("move " + move);
}
//todo: po wszystkich ruchach czyscic zmienne globalne from/to jezeli jeszcze beda istniec

void ChessMovements::promoteMoveSequence(Chess* pChess) //metoda przyda się na przyszłość
{
    this->regularMoveSequence(pChess);
}

void ChessMovements::goToSafeRemovedFieldIfNeeded(Chess *pChess)
{
    //todo: mozliwe ze cale przjscia typu goToSafeRemovedField beda zbedne jezeli...
    //...kaze dobotowi isc ruchami kolistymi na przegubach
    DIGIT SafePosToDigit = D_X;
    if (_PosMove.to.Digit == D_1) SafePosToDigit = D_2;
    else if (_PosMove.to.Digit == D_8) SafePosToDigit = D_7;
    else
    {
        qDebug() << "ERROR: ChessMovements::goToSafeRemovedField: unknown"
                    " _PosMove.to.Digit value =" << _PosMove.to.Digit;
        return;
    }

    PosOnBoard safeRemovedField(L_D, SafePosToDigit);
    pChess->movePieceWithManipulator(_pBoardRemoved, safeRemovedField);
}

SEQUENCE_TYPE ChessMovements::findMoveType(QString QStrMove)
{
     if (_pChess->getStatusPointer()->isMoveLegal(QStrMove))
    {
        if (_pChess->getStatusPointer()->isMoveLegal(QStrMove + "q")) //q == autotest for quenn promo
            return ST_PROMOTE_TO_WHAT;
        else if (_pChess->getStatusPointer()->isMoveEnpassant(QStrMove)) return ST_ENPASSANT;
        else if (_pChess->getStatusPointer()->isMoveCastling(QStrMove)) return ST_CASTLING;
        else if (_pChess->getStatusPointer()->isMoveRemoving()) return ST_REMOVING;
        else if (_pChess->getStatusPointer()->isMovePromotion(QStrMove)) return ST_PROMOTION;
        else return ST_REGULAR;
    }
     else
     {
         qDebug() << "ERROR: ChessMovements::findMoveType: move is illegal:" << QStrMove;
         return ST_NONE;
     }
}
