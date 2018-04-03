#include "chess_movements.h"

ChessMovements::ChessMovements(PieceController* pPieceController, Chessboard* pBoardMain,
                               Chessboard* pBoardRemoved)
{
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;
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
        _pPieceController->movePieceWithManipulator(_pBoardRemoved, pSafeRemovedField);
    }
}


void ChessMovements::doMoveSequence(PosFromTo PosMove, SEQUENCE_TYPE Type)
{
    Field* pFieldFrom = _pBoardMain->getField(PosMove.from);
    Field* pFieldTo = _pBoardMain->getField(PosMove.to);

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
                 << sequenceTypeAsQstr(Type);
        break;
    }
}

void ChessMovements::regularMoveSequence(Field* pFrom, Field* pTo)
{
    _pPieceController->movePieceWithManipulator(_pBoardMain, pFrom, VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardMain, pTo, VM_PUT);
}

void ChessMovements::removeMoveSequence(Field* pFieldWithPieceToRemove)
{    
    //find and safe piece on board, before grabbing (i.e. till it stands on board)
    Piece* pPieceToRemove = pFieldWithPieceToRemove->getPieceOnField(SHOW_ERRORS);
    if (pPieceToRemove == nullptr) return;

    _pPieceController->movePieceWithManipulator(_pBoardMain, pFieldWithPieceToRemove, VM_GRAB);

    Field* pFieldForPieceToRemove =
            _pBoardRemoved->getField(pPieceToRemove->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
    _pPieceController->movePieceWithManipulator(_pBoardRemoved, pFieldForPieceToRemove, VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
}

void ChessMovements::restoreMoveSequence(Piece* pPieceToRestore)
{
    Field* pFieldOnBoardMain = _pBoardMain->getField(pPieceToRestore->getStartFieldPos());
    Field* pFieldOnBoardRemoved = _pBoardRemoved->getField(pPieceToRestore->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldOnBoardRemoved);
    _pPieceController->movePieceWithManipulator(_pBoardRemoved, pFieldOnBoardRemoved, VM_GRAB);
    this->goToSafeRemovedFieldIfNeeded(pFieldOnBoardRemoved);
    _pPieceController->movePieceWithManipulator(_pBoardMain, pFieldOnBoardMain, VM_PUT);
}

void ChessMovements::castlingMoveSequence(Field* pFrom, Field* pTo)
{
    _pPieceController->movePieceWithManipulator(_pBoardMain,
                                                this->findKingFieldInCastling(pFrom), VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardMain,
                                                this->findKingFieldInCastling(pTo), VM_PUT);
    _pPieceController->movePieceWithManipulator(_pBoardMain,
                                                this->findRookFieldInCastling(pFrom), VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardMain,
                                                this->findRookFieldInCastling(pTo), VM_PUT);
}

void ChessMovements::enpassantMoveSequence(Field* pFrom, Field* pTo)
{
    _pPieceController->movePieceWithManipulator(_pBoardMain, pFrom, VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardMain, pTo, VM_PUT);

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

    _pPieceController->movePieceWithManipulator(_pBoardMain, pFieldWithPieceToRemove, VM_GRAB);

    Field* pFieldForPieceToRemove =
            _pBoardRemoved->getField(pPieceToRemoveInEnpassant->getStartFieldPos());

    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
    _pPieceController->movePieceWithManipulator(_pBoardRemoved, pFieldForPieceToRemove, VM_PUT);
    this->goToSafeRemovedFieldIfNeeded(pFieldForPieceToRemove);
}

void ChessMovements::promoteMoveSequence(Field* pFrom, Field* pTo) //future
{
    this->regularMoveSequence(pFrom, pTo);
}

bool ChessMovements::resetPiecePositions()
{
    if (!this->isPieceSetOnStartFields())
    {
        Chessboard tempBoard(B_MAIN, IMAGINARY);

        do
        {
            memcpy(&tempBoard, _pBoardMain, sizeof(*_pBoardMain));

            for (short sField=1; sField<=64; ++sField)
            {
                Field* pExaminedField = _pBoardMain->getField(sField);
                Piece* pPieceOnExaminedField = pExaminedField->getPieceOnField();

                if (!_pPieceController->isPieceStayOnItsStartingField(pPieceOnExaminedField))
                {
                    if (pExaminedField->getPieceOnField() == nullptr)
                    { //if checking field is empty
                        Piece* pMissingPiece = _pPieceController->getPiece(pExaminedField->
                                                                 getStartPieceNrOnField());
                        Field* pMissingPieceActualFieldOnMainBoard =
                                _pPieceController->searchForPieceActualFieldOnBoard(_pBoardMain,
                                                                                    pMissingPiece);
                        if (pMissingPieceActualFieldOnMainBoard != nullptr) //if exists on mainB
                            this->regularMoveSequence(pMissingPieceActualFieldOnMainBoard,
                                                        pExaminedField);
                        else this->restoreMoveSequence(pMissingPiece);
                    }
                    else //checking field is occupied
                    {
                        Field* pFieldToPutAsidePiece =
                                _pBoardMain->getField(pPieceOnExaminedField->getStartFieldNr());
                        Piece* pPieceOnPutAsideField = pFieldToPutAsidePiece->getPieceOnField();
                        Field* pStartFieldOfPieceOnPutAsideField =
                                _pBoardMain->getField(pPieceOnPutAsideField->getStartFieldNr());

                        if (pPieceOnPutAsideField == nullptr)
                            this->regularMoveSequence(pExaminedField,
                                                             pFieldToPutAsidePiece);
                        else if (pExaminedField == pStartFieldOfPieceOnPutAsideField)
                            this->removeMoveSequence(pExaminedField);
                        //else: iterate through all fields one more time (pieces pos will change)
                    }
                }
            }

            if (this->isPieceSetOnBoardsIdentical(tempBoard, *_pBoardMain))
            {
                qDebug() << "ERROR: ChessMovements::resetPiecePositions(): boards are identical";
                break;
            }
        }
        while (!this->isPieceSetOnStartFields());
    }

     return true;
}


bool ChessMovements::isPieceSetOnStartFields()
{
    for (short sField=1; sField<=64; ++sField)
    {
        Field* pField = _pBoardMain->getField(sField);
        Piece* pStartingPieceOnField =
                _pPieceController->getPiece(pField->getStartPieceNrOnField());
        if (pStartingPieceOnField != pField->getPieceOnField())
            return false;
    }

    return true;
}

bool ChessMovements::isPieceSetOnBoardsIdentical(Chessboard& board1, Chessboard& board2)
{
    for (short sField=1; sField<=64; ++sField)
    {
        if (board1.getField(sField)->getPieceOnField() !=
                board2.getField(sField)->getPieceOnField())
            return false;
    }

    return true;
}
