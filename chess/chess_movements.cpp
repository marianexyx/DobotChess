#include "chess_movements.h"

ChessMovements::ChessMovements(PieceController* pPieceController, Chessboard* pBoardCoreMain,
                               Chessboard* pBoardCoreRemoved)
{
    _pBoardCoreMain = pBoardCoreMain;
    _pBoardCoreRemoved = pBoardCoreRemoved;
    _pPieceController = pPieceController;
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
    return _pBoardCoreMain->getField(KingFieldDest);
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
    return _pBoardCoreMain->getField(RookFieldDest);
}

void ChessMovements::doMoveSequence(PosFromTo PosMove, SEQUENCE_TYPE Type)
{
    Field* pFieldFrom = _pBoardCoreMain->getField(PosMove.from);
    Field* pFieldTo = _pBoardCoreMain->getField(PosMove.to);

    switch(Type)
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
    _pPieceController->movePieceWithManipulator(_pBoardCoreMain, pFrom, VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardCoreMain, pTo, VM_PUT);
}

void ChessMovements::removeMoveSequence(Field* pFieldWithPieceToRemove)
{    
    //find and safe piece on board, before grabbing (i.e. till it stands on board)
    Piece* pPieceToRemove = pFieldWithPieceToRemove->getPieceOnField(SHOW_ERRORS);
    if (pPieceToRemove == nullptr) return;

    _pPieceController->movePieceWithManipulator(_pBoardCoreMain, pFieldWithPieceToRemove, VM_GRAB);

    Field* pFieldForPieceToRemove =
            _pBoardCoreRemoved->getField(pPieceToRemove->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
    _pPieceController->movePieceWithManipulator(_pBoardCoreRemoved, pFieldForPieceToRemove, VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
}

void ChessMovements::restoreMoveSequence(Piece* pPieceToRestore)
{
    Field* pFieldOnBoardMain = _pBoardCoreMain->getField(pPieceToRestore->getStartFieldPos());
    Field* pFieldOnBoardRemoved = _pBoardCoreRemoved->getField(pPieceToRestore->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldOnBoardRemoved);
    _pPieceController->movePieceWithManipulator(_pBoardCoreRemoved, pFieldOnBoardRemoved, VM_GRAB);
    this->goToSafeRemovedFieldIfNeeded(pFieldOnBoardRemoved);
    _pPieceController->movePieceWithManipulator(_pBoardCoreMain, pFieldOnBoardMain, VM_PUT);
}

void ChessMovements::castlingMoveSequence(Field* pFrom, Field* pTo)
{
    _pPieceController->movePieceWithManipulator(_pBoardCoreMain,
                                                this->findKingFieldInCastling(pFrom), VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardCoreMain,
                                                this->findKingFieldInCastling(pTo), VM_PUT);
    _pPieceController->movePieceWithManipulator(_pBoardCoreMain,
                                                this->findRookFieldInCastling(pFrom), VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardCoreMain,
                                                this->findRookFieldInCastling(pTo), VM_PUT);
}

void ChessMovements::enpassantMoveSequence(Field* pFrom, Field* pTo)
{
    _pPieceController->movePieceWithManipulator(_pBoardCoreMain, pFrom, VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardCoreMain, pTo, VM_PUT);

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

    Field* pFieldWithPieceToRemove = _pBoardCoreMain->getField(PosOfPieceToRemoveInEnpassant);
    Piece* pPieceToRemoveInEnpassant = pFieldWithPieceToRemove->getPieceOnField(SHOW_ERRORS);

    _pPieceController->movePieceWithManipulator(_pBoardCoreMain, pFieldWithPieceToRemove, VM_GRAB);

    Field* pFieldForPieceToRemove =
            _pBoardCoreRemoved->getField(pPieceToRemoveInEnpassant->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
    _pPieceController->movePieceWithManipulator(_pBoardCoreRemoved, pFieldForPieceToRemove, VM_PUT);
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
        Field* pSafeRemovedField = _pBoardCoreRemoved->getField(SafeRemovedFieldPos);
        _pPieceController->movePieceWithManipulator(_pBoardCoreRemoved, pSafeRemovedField);
    }
}

