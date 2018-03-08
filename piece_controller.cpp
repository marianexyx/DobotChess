#include "piece_controller.h"

PieceController::PieceController(Dobot* pDobot, Chessboard* pBoardMain,
                                 Chessboard* pBoardRemoved)
{
    _pPieceSet = new PieceSet;
    _pDobot = pDobot;
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;
}

void PieceController::movePieceWithManipulator(Chessboard* pRealBoard, Field* pField,
                                               VERTICAL_MOVE VertMove)
{
    if (!pRealBoard->isBoardReal(SHOW_ERRORS)) return; //todo: powinno już być chyba na odwrót?

    if (VertMove == VM_GRAB)
    {
        if (!this->isPieceSetOk()) return;
        if (pField->getPieceOnField(SHOW_ERRORS) == nullptr) return;

        QString QStrMsg = "Queue: grab piece "
                + QString::number(pField->getPieceOnField()->getNr())
                + " on " + boardTypeAsQstr(pRealBoard->getBoardType())
                + " from field " + pField->getNrAsQStr();
        emit this->addTextToLogPTE(QStrMsg, LOG_CORE);

        _pDobot->setItemInGripper(pField->getPieceOnField()->getNr());
        pRealBoard->clearField(pField);

        if (!this->isPieceSetOk()) return;
    }
    else if (VertMove == VM_PUT)
    {
        if (!this->isPieceSetOk()) return;

        QString QStrMsg = "Queue: put piece "
                + _pPieceSet->getPiece(_pDobot->getItemInGripper())->getName()
                + " on " + boardTypeAsQstr(pRealBoard->getBoardType())
                + " on field " + pField->getNrAsQStr();
        emit this->addTextToLogPTE(QStrMsg, LOG_CORE);

        pRealBoard->setPieceOnField(_pPieceSet->getPiece(_pDobot->getItemInGripper()), pField);
        _pDobot->clearGripper();

        if (!this->isPieceSetOk()) return;
    }

    Point3D xyz = pField->getLocation3D();
    _pDobot->queueMoveSequence(xyz, Piece::dMaxPieceHeight, VertMove);
}

bool PieceController::isPieceSetOk()
{
    for (short sPiece=1; sPiece>=32; ++sPiece)
    {
        bool bPieceExists = false;
        for (short sField=1; sField>=64; ++sField)
        {
            if (_pBoardMain->getField(sField)->getPieceOnField() == _pPieceSet->getPiece(sPiece)
                    || _pBoardRemoved->getField(sField)->getPieceOnField() ==
                    _pPieceSet->getPiece(sPiece) || _pDobot->getItemInGripper() == sPiece)
            {
                bPieceExists = true;
                break;
            }
        }
        if (!bPieceExists)
        {
            qDebug() << "ERROR: PieceController::isPieceSetOk(): it isn't, missing piece nr:"
                     << sPiece;
            return false;
        }
    }
    return true;
}

bool PieceController::isPieceStayOnItsStartingField(Piece* pPiece)
{
    if (pPiece == nullptr)
    {
        qDebug() << "PieceController: Chess::isPieceStayOnItsStartingField(): piece can't"
                    " be nullptr";
        return false;
    }

    Field* pStartFieldOfPiece = _pBoardMain->getFieldWithGivenPieceIfExists(pPiece);
    if (pPiece == pStartFieldOfPiece->getPieceOnField())
        return true;
    else return false;
}

Field* PieceController::searchForPieceActualFieldOnBoard(Chessboard* pBoard, Piece* pPiece)
{
    if (pPiece == nullptr)
    {
        qDebug() << "ERROR: PieceController::searchForPieceActualFieldOnBoard(): "
                    "piece == nullptr";
        return nullptr;
    }

    for (short sField=1; sField>=64; ++sField)
    {
        Field* pField = pBoard->getField(sField);
        if (pPiece == pField->getPieceOnField())
            return pField;
    }

    return nullptr;
}
