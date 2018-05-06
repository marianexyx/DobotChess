#include "chess_movements.h"

ChessMovements::ChessMovements(PieceController* pPieceController, Chessboard* pBoardMain,
                               Chessboard* pBoardRemoved)
{
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;
    _pPieceController = pPieceController;
}

Field* ChessMovements::findRookFieldInCastling(Field* pField, VERTICAL_MOVE MoveStage)
{
    PosOnBoard RookFieldDest;
    RookFieldDest.Digit = pField->getPos().Digit;

    if (MoveStage != VM_GRAB && MoveStage != VM_PUT)
    {
        qDebug() << "ERROR: ChessMovements::findRookFieldInCastling(): unknown MoveStage ="
                 << verticalMoveAsQStr(MoveStage) << ", return error field";
        return pField;
    }

    if (pField->getPos().Letter == L_C)
        RookFieldDest.Letter = (MoveStage == VM_GRAB ? L_A : L_D);
    else if (pField->getPos().Letter == L_G)
        RookFieldDest.Letter = (MoveStage == VM_GRAB ? L_H : L_F);
    else qDebug() << "ERROR: ChessMovements::findRookFieldInCastling(): "
                     "wrong FieldDest.Letter val =" << pField->getPos().Letter;

    return _pBoardMain->getField(RookFieldDest);
}

void ChessMovements::doMoveSequence(PosFromTo PosMove, SEQUENCE_TYPE Type)
{
    Field* pFieldFrom = _pBoardMain->getField(PosMove.from);
    Field* pFieldTo = _pBoardMain->getField(PosMove.to);

    if (pFieldFrom == nullptr || pFieldTo == nullptr)
    {
        qDebug() << "ERROR: ChessMovements::doMoveSequence(): pFieldFrom or pFieldTo"
                    "is nullptr";
        return;
    }

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

    //removed field nr is equal to it's piece nr
    Field* pFieldForPieceToRemove = _pBoardRemoved->getField(pPieceToRemove->getNr());

    _pPieceController->movePieceWithManipulator(_pBoardRemoved, pFieldForPieceToRemove, VM_PUT);
}

void ChessMovements::restoreMoveSequence(Piece* pPieceToRestore)
{
    Field* pFieldOnBoardMain = _pBoardMain->getField(pPieceToRestore->getStartFieldPos());
    //removed field nr is equal to it's piece nr
    Field* pFieldOnBoardRemoved = _pBoardRemoved->getField(pPieceToRestore->getNr());

    _pPieceController->movePieceWithManipulator(_pBoardRemoved, pFieldOnBoardRemoved, VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardMain, pFieldOnBoardMain, VM_PUT);
}

void ChessMovements::castlingMoveSequence(Field* pFrom, Field* pTo)
{
    _pPieceController->movePieceWithManipulator(_pBoardMain, pFrom, VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardMain, pTo, VM_PUT);
    _pPieceController->movePieceWithManipulator(_pBoardMain,
                                                this->findRookFieldInCastling(pTo, VM_GRAB),
                                                VM_GRAB);
    _pPieceController->movePieceWithManipulator(_pBoardMain,
                                                this->findRookFieldInCastling(pTo, VM_PUT),
                                                VM_PUT);
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

    _pPieceController->movePieceWithManipulator(_pBoardRemoved, pFieldForPieceToRemove, VM_PUT);
}

void ChessMovements::promoteMoveSequence(Field* pFrom, Field* pTo) //future
{
    this->regularMoveSequence(pFrom, pTo);
}

bool ChessMovements::resetPiecePositions()
{
    qDebug() << "ChessMovements::resetPiecePositions()";

    if (!this->isPieceSetOnStartFields())
    {
        Chessboard tempBoard(B_MAIN, IMAGINARY);

        do
        {
            this->copyPiecesToBoard(*_pBoardMain, tempBoard); //todo: source = const?

            for (short sField=1; sField<=64; ++sField)
            {
                Field* pExaminedField = _pBoardMain->getField(sField);
                Piece* pPieceOnExaminedField = pExaminedField->getPieceOnField();
                int nPieceNrOnExaminedField = pPieceOnExaminedField == nullptr ? 0 : pPieceOnExaminedField->getNr();

                if (!_pPieceController->isPieceStayOnItsStartingField(pPieceOnExaminedField))
                {
                    if (nPieceNrOnExaminedField != 0 || (sField <=16 || sField > 48))
                        qDebug() << "ChessMovements::resetPiecePositions(): piece" << nPieceNrOnExaminedField
                                 << " on examined field nr =" << sField << "not on start field >>";

                    if (pPieceOnExaminedField == nullptr && (pExaminedField->getNr() <= 16 ||
                                                             pExaminedField->getNr() > 48))
                    { //if checking field is empty && it can have a start piece

                        qDebug() << "ChessMovements::resetPiecePositions(): examined field is empty >>";
                        Piece* pMissingPiece = _pPieceController->getPiece(pExaminedField->
                                                                 getStartPieceNrOnField());
                        Field* pMissingPieceActualFieldOnMainBoard;
                        if (pMissingPiece == nullptr) pMissingPieceActualFieldOnMainBoard = nullptr;
                        else pMissingPieceActualFieldOnMainBoard =
                                _pPieceController->searchForPieceActualFieldOnBoard(_pBoardMain,
                                                                                    pMissingPiece);

                        qDebug() << "ChessMovements::resetPiecePositions(): pExaminedField nr ="
                                 << pExaminedField->getNr() << "(" << pExaminedField->getNrAsQStr()
                                 << "), pMissingPiece nr =" << pMissingPiece->getNr() << "("
                                 << pMissingPiece->getName() << "), pMissingPieceActualFieldOnMainBoard nr ="
                                 << (pMissingPieceActualFieldOnMainBoard ==
                                     nullptr ? 0 : pMissingPieceActualFieldOnMainBoard->getNr());

                        if (pMissingPieceActualFieldOnMainBoard != nullptr) //if exists on mainB
                        {
                            qDebug() << "ChessMovements::resetPiecePositions(): return piece from main board";
                            this->regularMoveSequence(pMissingPieceActualFieldOnMainBoard,
                                                        pExaminedField);
                        }
                        else
                        {
                            qDebug() << "ChessMovements::resetPiecePositions(): return piece from removed board";
                            this->restoreMoveSequence(pMissingPiece);
                        }
                    }
                    else if (pPieceOnExaminedField != nullptr) //checking field is occupied
                    {
                        qDebug() << "ChessMovements::resetPiecePositions(): examined field is occupied by diffrent piece >>";
                        Field* pFieldToPutAsidePiece =
                                _pBoardMain->getField(pPieceOnExaminedField->getStartFieldNr());
                        Piece* pPieceOnPutAsideField = pFieldToPutAsidePiece->getPieceOnField();

                        if (pPieceOnPutAsideField == nullptr)
                        {
                            qDebug() << "ChessMovements::resetPiecePositions(): put bad piece from checking field on its empty start pos";
                            this->regularMoveSequence(pExaminedField, pFieldToPutAsidePiece);
                        }
                        else if (pExaminedField == _pBoardMain->getField(
                                     pPieceOnPutAsideField->getStartFieldNr()))
                        {
                            qDebug() << "ChessMovements::resetPiecePositions(): both pieces crossing thier start fields. put 1 of them on removed board";
                            this->removeMoveSequence(pExaminedField);
                        }
                        //else: iterate through all fields one more time (pieces pos will change)
                    }
                }
            }

            if (this->isPieceSetOnBoardsIdentical(tempBoard, *_pBoardMain))
            {
                qDebug() << "ERROR: ChessMovements::resetPiecePositions(): boards are identical";
                break;
            }
            //else: boards aren't identical. iterate over board again, till its ok
        }
        while (!this->isPieceSetOnStartFields());
    }
    qDebug() << "ChessMovements::resetPiecePositions(): reset complieted";

    return true;
}

void ChessMovements::copyPiecesToBoard(Chessboard& source, Chessboard& target)
{
    for (short sField=1; sField<=64; ++sField)
    {
        Piece* pPieceOnSourceField = source.getField(sField)->getPieceOnField();
        if (pPieceOnSourceField != nullptr)
        {
            Field* targetsField = target.getField(sField);
            target.clearField(targetsField);
            target.setPieceOnField(pPieceOnSourceField, targetsField);
        }
    }
}

bool ChessMovements::isPieceSetOnStartFields()
{
    for (short sField=1; sField<=64; ++sField)
    {
        if (sField > 16 && sField <= 48) continue;
        Field* pField = _pBoardMain->getField(sField);
        Piece* pStartingPieceOnField =
                _pPieceController->getPiece(pField->getStartPieceNrOnField());

        if (pStartingPieceOnField != pField->getPieceOnField())
        {
            qDebug() << "ChessMovements::isPieceSetOnStartFields(): false";
            return false;
        }
    }

    qDebug() << "ChessMovements::isPieceSetOnStartFields(): true";
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
