#include "piece_controller.h"

PieceController::PieceController(Dobot* pDobot, Chessboard* pBoardMain,
                                 Chessboard* pBoardRemoved)
{
    m_pPieceSet = new PieceSet();
    m_pDobot = pDobot;
    m_pBoardMain = pBoardMain;
    m_pBoardRemoved = pBoardRemoved;

    for (int i=0; i<=63; ++i) //fill board with pieces on their start positions
    {
        short sStartPieceNr = m_pBoardMain->getField(i+1)->getStartPieceNrOnField();
        if (sStartPieceNr > 0)
            m_pBoardMain->getField(i+1)->setPieceOnField(this->getPiece(sStartPieceNr));
    }
}

bool PieceController::isPieceSetOk()
{
    for (short sPiece=1; sPiece<=32; ++sPiece)
    {
        bool bPieceExists = false;
        for (short sField=1; sField<=64; ++sField)
        {
            if (m_pBoardMain->getField(sField)->getPieceOnField() == m_pPieceSet->getPiece(sPiece)
                    || m_pBoardRemoved->getField(sField)->getPieceOnField() ==
                    m_pPieceSet->getPiece(sPiece) || m_pDobot->getItemInGripper() == sPiece)
            {
                bPieceExists = true;
                break;
            }
        }
        if (!bPieceExists)
        {
            qCritical() << "it isn't, missing piece nr:" << QString::number(sPiece)
                        << ". item (piece) nr in gripper ="
                        << QString::number(m_pDobot->getItemInGripper());
            return false;
        }
    }
    return true;
}

bool PieceController::isAnyPawnPromoted()
{
    for (short sPiece=1; sPiece<=32; ++sPiece)
    {
        Piece* pPiece = this->getPiece(sPiece);
        Field* pField = m_pBoardMain->getFieldWithGivenPieceIfExists(pPiece);
        if (pPiece->getType() == P_PAWN && pPiece->getPromotedType() != P_ERROR &&
                 pField != nullptr)
            return true;
    }
    return false;
}

void PieceController::movePieceWithManipulator(Chessboard* pRealBoard, Field* pField,
                                               VERTICAL_MOVE VertMove /*= VM_NONE*/,
                                               bool bRetreat /*= false*/)
{
    if (!pRealBoard->isBoardReal(SHOW_ERRORS)) return;

    if (VertMove == VM_GRAB)
    {
        if (!this->isPieceSetOk()) return;
        if (pField->getPieceOnField(SHOW_ERRORS) == nullptr) return;

        QString QStrMsg = "Queue: grab piece '" + pField->getPieceOnField()->getName()
                + "' (nr =" + QString::number(pField->getPieceOnField()->getNr()) + ") on "
                + boardTypeAsQstr(pRealBoard->getBoardType()) + " from field nr " +
                QString::number(pField->getNr()) + "(" + pField->getNrAsQStr() + ")\n";
        emit this->addTextToLogPTE(QStrMsg, LOG_CORE);

        m_pDobot->setItemInGripper(pField->getPieceOnField()->getNr());
        pRealBoard->clearField(pField, SHOW_ERRORS);

        if (!this->isPieceSetOk()) return;
    }
    else if (VertMove == VM_PUT)
    {
        if (!this->isPieceSetOk()) return;

        QString QStrMsg = "Queue: put piece "
                + m_pPieceSet->getPiece(m_pDobot->getItemInGripper())->getName()
                + " on " + boardTypeAsQstr(pRealBoard->getBoardType())
                + " on field " + pField->getNrAsQStr() + "\n";
        emit this->addTextToLogPTE(QStrMsg, LOG_CORE);

        pRealBoard->setPieceOnField(m_pPieceSet->getPiece(m_pDobot->getItemInGripper()),
                                    pField, PRINT_DEBUG);
        m_pDobot->clearGripper();

        if (!this->isPieceSetOk()) return;
    }

    m_lastBoardType = pRealBoard->getBoardType();
    m_lastPos = pField->getPos();
    Point3D xyz = pField->getLocation3D();
    m_pDobot->queueMoveSequence(xyz, m_pBoardMain->dMaxPieceHeight, VertMove, bRetreat);
    emit this->showRealBoardInUI();
}

Field* PieceController::searchForPieceActualFieldOnBoard(Chessboard* pBoard, Piece* pPiece)
{
    if (pPiece == nullptr)
    {
        qCritical() << "piece == nullptr";
        return nullptr;
    }

    for (short sField=1; sField<=64; ++sField)
    {
        Field* pField = pBoard->getField(sField);
        if (pPiece == pField->getPieceOnField())
            return pField;
    }

    return nullptr;
}

bool PieceController::isPieceStayOnItsStartingField(Piece* pPiece, bool bErrorLog /*= false*/)
{
    if (pPiece == nullptr)
    {
        if (bErrorLog)
            qCritical() << "piece can't be nullptr";
        return false;
    }

    Field* pActualFieldOfGIvenPiece = m_pBoardMain->getFieldWithGivenPieceIfExists(pPiece);
    if (pActualFieldOfGIvenPiece == nullptr) return false;
    if (pPiece->getStartFieldNr() == pActualFieldOfGIvenPiece->getNr()) return true;
    else return false;
}

bool PieceController::isMoveSet()
{
    if (m_lastPos.Letter == L_X || m_lastPos.Digit == D_X)
        return false;
    else return true;
}

void PieceController::clearPawnsPromotions()
{
    for (short sPiece=1; sPiece<=32; ++sPiece)
        this->getPiece(sPiece)->clearPromotedType();
}

QString PieceController::getPromotedPawnsPositions()
{
    QString QStrPromotedPawnsPositions;
    for (short sPiece=1; sPiece<=32; ++sPiece)
    {
        Piece* pPiece = this->getPiece(sPiece);
        Field* pField = m_pBoardMain->getFieldWithGivenPieceIfExists(pPiece);
        if (pPiece->getType() == P_PAWN && pPiece->getPromotedType() != P_ERROR &&
                 pField != nullptr)
        {
            QString QStrNext = pField->getNrAsQStr() + "_" + pPiece->getAsFENSign();
            QStrPromotedPawnsPositions += QStrNext + " ";
        }
    }

    if (!QStrPromotedPawnsPositions.isEmpty())
    {
        QStrPromotedPawnsPositions = QStrPromotedPawnsPositions.left(
                    QStrPromotedPawnsPositions.length() - 1); //delete last spacebar
    }
    else QStrPromotedPawnsPositions = "0";

    return QStrPromotedPawnsPositions; //f.e.: "b2_Q c7_q g5_N e1_R d4_b"
}

QString PieceController::dumpAllData()
{
    QString QStrData;

    QStrData = "[piece_controller.h]\n";
    QStrData += "m_lastPos: " + m_lastPos.getAsQStr() + "\n";
    QStrData += "m_lastBoardType: " + boardTypeAsQstr(m_lastBoardType) + "\n";

    return QStrData;
}
