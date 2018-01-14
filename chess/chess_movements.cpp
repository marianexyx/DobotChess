#include "chess_movements.h"

ChessMovements::ChessMovements(Chess *pChess)
{
    _pChess = pChess; 
    _pBoardMain = _pChess->getBoardMainPointer();
    _pBoardRemoved = _pChess->getBoardRemovedPointer();

}

Field* ChessMovements::findKingFieldInCastling(Field* pField)
{
    PosFromTo KingFieldDest;
    KingFieldDest.Digit = pField->getPos().Digit;
    if (pField->getPos().Letter == L_C) KingFieldDest.Letter = L_A;
    else if (pField->getPos().Letter == L_G) KingFieldDest.Letter = L_H;
    else qDebug() << "ERROR: ChessMovements::findKingFieldInCastling: "
                     "wrong FieldDest.Letter val =" << pField->getPos().Letter;
    return _pChess->getBoardMainPointer()->getField(KingFieldDest);
}

Field* ChessMovements::findRookFieldInCastling(Field* pField)
{
    PosFromTo RookFieldDest;
    RookFieldDest.Digit = pField->getPos().Digit;
    if (pField->getPos().Letter == L_C) RookFieldDest.Letter = L_D;
    else if (pField->getPos().Letter == L_G) RookFieldDest.Letter = L_F;
    else qDebug() << "ERROR: ChessMovements::findKingFieldInCastling: "
                     "wrong FieldDest.Letter val =" << pField->getPos().Letter;
    return _pChess->getBoardMainPointer()->getField(RookFieldDest);
}

void ChessMovements::doMoveSequence(SEQUENCE_TYPE MoveType, PosFromTo Move)
{
    Field* pFieldFrom = _pChess->getBoardMainPointer()->getField(Move.from);
    Field* pFieldTo = _pChess->getBoardMainPointer()->getField(Move.to);

    switch(MoveType)
    {
    case ST_REGULAR: this->regularMoveSequence(pFieldFrom, pFieldTo); break;
    case ST_REMOVING:
        this->removeMoveSequence(pFieldTo);
        this->regularMoveSequence(pFieldFrom, pFieldTo);
        break;
    case ST_ENPASSANT: this->enpassantMoveSequence(pFieldFrom, pFieldTo); break;
    case ST_CASTLING: this->castlingMoveSequence(pFieldFrom, pFieldTo); break;
    case ST_PROMOTION: this->promoteMoveSequence(pFieldFrom, pFieldTo); break;
    case ST_NONE:
    default:
        qDebug() << "ERROR: ChessMovements::doMoveSequence(): wrong MoveType:"
                 << sequenceTypeAsQstr(MoveType);
        break;
    }
}

void ChessMovements::regularMoveSequence(Field* pFrom, Field* pTo)
{
    _pChess->movePieceWithManipulator(_pBoardMain, pFrom, VM_GRAB);
    _pChess->movePieceWithManipulator(_pBoardMain, pTo, VM_PUT);
}

void ChessMovements::removeMoveSequence(Field* pFieldWithPieceToRemove)
{    
    //find and safe piece on board, before grabbing (i.e. till it stands on board)
    short sPieceNrToRemove = pFieldWithPieceToRemove->getPieceNrOnField();
    if (!Piece::isInRange(sPieceNrToRemove)) return;
    Piece* pPieceToRemove = _pChess->getPiece(sPieceNrToRemove);

    _pChess->movePieceWithManipulator(_pBoardMain, pFieldWithPieceToRemove, VM_GRAB);

    Field* pFieldForPieceToRemove =
            _pChess->getBoardRemovedPointer()->getField(pPieceToRemove->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
    _pChess->movePieceWithManipulator(_pBoardRemoved, pFieldForPieceToRemove, VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
}

void ChessMovements::restoreMoveSequence(Piece *pPieceToRestore)
{
    Field* pFieldOnBoardMain =
            _pChess->getBoardMainPointer()->getField(pPieceToRestore->getStartFieldPos());
    Field* pFieldOnBoardRemoved =
            _pChess->getBoardRemovedPointer()->getField(pPieceToRestore->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldOnBoardRemoved);
    _pChess->movePieceWithManipulator(_pBoardRemoved, pFieldOnBoardRemoved, VM_GRAB);
    this->goToSafeRemovedFieldIfNeeded(pFieldOnBoardRemoved);
    _pChess->movePieceWithManipulator(_pBoardMain, pFieldOnBoardMain, VM_PUT);
}

void ChessMovements::castlingMoveSequence(Field *pFrom, Field *pTo)
{
    _pChess->movePieceWithManipulator(_pBoardMain, this->findKingFieldInCastling(pFrom), VM_GRAB);
    _pChess->movePieceWithManipulator(_pBoardMain, this->findKingFieldInCastling(pTo), VM_PUT);
    _pChess->movePieceWithManipulator(_pBoardMain, this->findRookFieldInCastling(pFrom), VM_GRAB);
    _pChess->movePieceWithManipulator(_pBoardMain, this->findRookFieldInCastling(pTo), VM_PUT);
}

void ChessMovements::enpassantMoveSequence(Field *pFrom, Field *pTo)
{
    _pChess->movePieceWithManipulator(_pBoardMain, pFrom, VM_GRAB);
    _pChess->movePieceWithManipulator(_pBoardMain, pTo, VM_PUT);

    PosOnBoard PosOfPieceToRemoveInEnpassant = pTo->getPos();
    if (pFrom->getPos().Digit < pTo->getPos().Digit) //white move
        PosOfPieceToRemoveInEnpassant.Digit -= 1;
    else if (pFrom->getPos().Digit > pTo->getPos().Digit) //black move
        PosOfPieceToRemoveInEnpassant.Digit += 1;
    else
    {
        qDebug() << "ERROR: ChessMovements::enpassantMoveSequence: no move "
                     "detected is digits axis (digits abs(from-to)==0)";
        return;
    }

    Field* pFieldWithPieceToRemove = _pBoardMain->getField(PosOfPieceToRemoveInEnpassant);
    Piece* pPieceToRemoveInEnpassant =
            _pChess->getPiece(pFieldWithPieceToRemove->getPieceNrOnField());

    _pChess->movePieceWithManipulator(_pBoardMain, pFieldWithPieceToRemove, VM_GRAB);

    Field* pFieldForPieceToRemove =
            _pChess->getBoardRemovedPointer()->getField(
                pPieceToRemoveInEnpassant->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
    _pChess->movePieceWithManipulator(_pBoardRemoved, pFieldForPieceToRemove, VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
}

void ChessMovements::promoteMoveSequence(Field* pFrom, Field* pTo) //metoda przyda się na przyszłość
{
    this->regularMoveSequence(pFrom, pTo);
}

void ChessMovements::goToSafeRemovedFieldIfNeeded(Field* pFieldDest)
{
    //todo: mozliwe ze cale przjscia typu goToSafeRemovedField beda zbedne jezeli...
    //...kaze dobotowi isc ruchami kolistymi na przegubach
    if (pFieldDest->getPos().Letter >= L_D)
    {
        return; //no need for safe move
    }
    else
    {
        DIGIT SafePosToDigit = D_X;
        if (pFieldDest->getPos().Digit <= D_2) SafePosToDigit = D_2;
        else if (pFieldDest->getPos().Digit >= D_7) SafePosToDigit = D_7;
        else
        {
            qDebug() << "ERROR: ChessMovements::goToSafeRemovedField(): unknown"
                        " digit value =" << pFieldDest->getPos().Digit;
            return;
        }

        PosOnBoard SafeRemovedFieldPos(L_D, SafePosToDigit);
        Field* pSafeRemovedField = _pChess->getBoardRemovedPointer()->getField(SafeRemovedFieldPos);
        _pChess->movePieceWithManipulator(_pBoardRemoved, pSafeRemovedField);
    }
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
