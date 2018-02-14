#include "chess_movements.h"

ChessMovements::ChessMovements(Chess* pChess)
{
    _pChess = pChess; 
    _pBoardMain = _pChess->getBoardMainPointer();
    _pBoardRemoved = _pChess->getBoardRemovedPointer();
    _pStatus = _pChess->getStatusPointer();
}

Field* ChessMovements::findKingFieldInCastling(Field* pField)
{
    PosOnBoard KingFieldDest;
    KingFieldDest.Digit = pField->getPos().Digit;
    if (pField->getPos().Letter == L_C)
        KingFieldDest.Letter = L_A;
    else if (pField->getPos().Letter == L_G)
        KingFieldDest.Letter = L_H;
    else qDebug() << "ERROR: ChessMovements::findKingFieldInCastling(): "
                     "wrong FieldDest.Letter val =" << pField->getPos().Letter;
    return _pBoardMain->getField(KingFieldDest);
}

Field* ChessMovements::findRookFieldInCastling(Field* pField)
{
    PosOnBoard RookFieldDest;
    RookFieldDest.Digit = pField->getPos().Digit;
    if (pField->getPos().Letter == L_C)
        RookFieldDest.Letter = L_D;
    else if (pField->getPos().Letter == L_G)
        RookFieldDest.Letter = L_F;
    else qDebug() << "ERROR: ChessMovements::findKingFieldInCastling(): "
                     "wrong FieldDest.Letter val =" << pField->getPos().Letter;
    return _pBoardMain->getField(RookFieldDest);
}

void ChessMovements::doMoveSequence()
{
    Field* pFieldFrom = _pBoardMain->getField(_PosMove.from);
    Field* pFieldTo = _pBoardMain->getField(_PosMove.to);

    switch(_MoveType)
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
                 << sequenceTypeAsQstr(_MoveType);
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
    Piece* pPieceToRemove = pFieldWithPieceToRemove->getPieceOnField(SHOW_ERRORS);
    if (pPieceToRemove == nullptr) return;

    _pChess->movePieceWithManipulator(_pBoardMain, pFieldWithPieceToRemove, VM_GRAB);

    Field* pFieldForPieceToRemove =
            _pBoardRemoved->getField(pPieceToRemove->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
    _pChess->movePieceWithManipulator(_pBoardRemoved, pFieldForPieceToRemove, VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
}

void ChessMovements::restoreMoveSequence(Piece* pPieceToRestore)
{
    Field* pFieldOnBoardMain = _pBoardMain->getField(pPieceToRestore->getStartFieldPos());
    Field* pFieldOnBoardRemoved = _pBoardRemoved->getField(pPieceToRestore->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldOnBoardRemoved);
    _pChess->movePieceWithManipulator(_pBoardRemoved, pFieldOnBoardRemoved, VM_GRAB);
    this->goToSafeRemovedFieldIfNeeded(pFieldOnBoardRemoved);
    _pChess->movePieceWithManipulator(_pBoardMain, pFieldOnBoardMain, VM_PUT);
}

void ChessMovements::castlingMoveSequence(Field* pFrom, Field* pTo)
{
    _pChess->movePieceWithManipulator(_pBoardMain, this->findKingFieldInCastling(pFrom), VM_GRAB);
    _pChess->movePieceWithManipulator(_pBoardMain, this->findKingFieldInCastling(pTo), VM_PUT);
    _pChess->movePieceWithManipulator(_pBoardMain, this->findRookFieldInCastling(pFrom), VM_GRAB);
    _pChess->movePieceWithManipulator(_pBoardMain, this->findRookFieldInCastling(pTo), VM_PUT);
}

void ChessMovements::enpassantMoveSequence(Field* pFrom, Field* pTo)
{
    _pChess->movePieceWithManipulator(_pBoardMain, pFrom, VM_GRAB);
    _pChess->movePieceWithManipulator(_pBoardMain, pTo, VM_PUT);

    PosOnBoard PosOfPieceToRemoveInEnpassant = pTo->getPos();
    short nDigitOfPieceToRemove = static_cast<int>(PosOfPieceToRemoveInEnpassant.Digit);
    if (pFrom->getPos().Digit < pTo->getPos().Digit) //white move
        PosOfPieceToRemoveInEnpassant.Digit = static_cast<DIGIT>(nDigitOfPieceToRemove - 1);
    else if (pFrom->getPos().Digit > pTo->getPos().Digit) //black move
        PosOfPieceToRemoveInEnpassant.Digit = static_cast<DIGIT>(nDigitOfPieceToRemove + 1);
    else
    {
        qDebug() << "ERROR: ChessMovements::enpassantMoveSequence(): no move "
                     "detected in digits axis (digits abs(from-to)==0)";
        return;
    }

    Field* pFieldWithPieceToRemove = _pBoardMain->getField(PosOfPieceToRemoveInEnpassant);
    Piece* pPieceToRemoveInEnpassant = pFieldWithPieceToRemove->getPieceOnField(SHOW_ERRORS);

    _pChess->movePieceWithManipulator(_pBoardMain, pFieldWithPieceToRemove, VM_GRAB);

    Field* pFieldForPieceToRemove =
            _pBoardRemoved->getField(pPieceToRemoveInEnpassant->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
    _pChess->movePieceWithManipulator(_pBoardRemoved, pFieldForPieceToRemove, VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
}

void ChessMovements::promoteMoveSequence(Field* pFrom, Field* pTo) //future
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
        Field* pSafeRemovedField = _pBoardRemoved->getField(SafeRemovedFieldPos);
        _pChess->movePieceWithManipulator(_pBoardRemoved, pSafeRemovedField);
    }
}

SEQUENCE_TYPE ChessMovements::findMoveType(QString QStrMove)
{
    if (_pStatus->isMoveLegal(QStrMove))
    {
        if (_pStatus->isMoveLegal(QStrMove + "q")) return ST_PROMOTE_TO_WHAT; //promo autotest
        else if (_pStatus->isMoveEnpassant(QStrMove)) return ST_ENPASSANT;
        else if (_pStatus->isMoveCastling(QStrMove)) return ST_CASTLING;
        else if (_pStatus->isMoveRemoving()) return ST_REMOVING;
        else if (ChessStatus::isMovePromotion(QStrMove)) return ST_PROMOTION;
        else return ST_REGULAR;
    }
    else
    {
        qDebug() << "ERROR: ChessMovements::findMoveType(): move is illegal:" << QStrMove;
        return ST_NONE;
    }
}

bool ChessMovements::isMoveSet()
{
    if (_PosMove.from.Letter == L_X || _PosMove.from.Digit == D_X ||
            _PosMove.to.Letter == L_X || _PosMove.to.Digit == D_X)
        return false;
    else return true;
}

void ChessMovements::setMove(QString QStrMove)
{
    _MoveType = this->findMoveType(QStrMove);
    _PosMove = PosFromTo::fromQStr(QStrMove);
}
