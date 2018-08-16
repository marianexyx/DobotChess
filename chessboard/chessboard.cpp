#include "chessboard.h"

Chessboard::Chessboard(BOARD BoardType, bool bBoardIsReal /*= true*/,
                       RealVars gameConfigVars /*= RealVars()*/):
    dMaxPieceHeight(gameConfigVars.fPieceHeight)
{
    m_boardType = BoardType;
    m_bBoardIsReal = bBoardIsReal;

    for (int i=0; i<=63; ++i)
        m_pField[i] = new Field(i+1);

    if (this->isBoardReal())
    {
        if (BoardType == B_MAIN)
        {
            m_a1 = gameConfigVars.A1;
            m_a8 = gameConfigVars.A8;
            m_h1 = gameConfigVars.H1;
            m_h8 = gameConfigVars.H8;

            m_dSquareWidth = ((m_a1.y - m_h1.y)/7.f + (m_a8.x - m_h1.x)/7.f)/2.f;

            this->calculateFieldsPointsOnMainBoard(m_a1, m_a8, m_h1, m_h8);
        }
        else if (BoardType == B_REMOVED)
        {
            m_remWhiteCloserOuter = gameConfigVars.remWhiteCloserOuter;
            m_remWhiteFurtherInner = gameConfigVars.remWhiteFurtherInner;
            m_remBlackCloserOuter = gameConfigVars.remBlackCloserOuter;
            m_remBlackFurtherInner = gameConfigVars.remBlackFurtherInner;

            m_dSquareWidth = (((m_remWhiteFurtherInner.x + m_remBlackFurtherInner.x)/2.f) -
                    ((m_remWhiteCloserOuter.x + m_remBlackCloserOuter.x)/2.f))/7.f;

            this->calculateFieldsPointsOnRemovedBoard(m_remWhiteCloserOuter,
                  m_remWhiteFurtherInner, m_remBlackCloserOuter, m_remBlackFurtherInner);
        }
    }
}

Chessboard::~Chessboard()
{
    for (int i=0; i<=63; ++i)
    {
        delete m_pField[i];
        m_pField[i] = nullptr; //good c++ practise
    }
}

void Chessboard::calculateFieldsPointsOnMainBoard(Point3D A1, Point3D A8,
                                                       Point3D H1, Point3D H8)
{
    for (int digit=0; digit<=7; ++digit)
    {
        for (int letter=0; letter<=7; ++letter)
        {
            PosOnBoard pos;
            pos.Letter = static_cast<LETTER>(letter+1);
            pos.Digit = static_cast<DIGIT>(digit+1);

            Point3D p3D;
            p3D.x = A1.x +
                    digit*(((A8.x-A1.x)/7.f)+
                           ((letter/14.f)*(((A1.x-H1.x)/7.f)-((A8.x-H8.x)/7.f))))-
                    letter*(((A1.x-H1.x)/7.f)-
                            ((digit/14.f)*(((H8.x-H1.x)/7.f)-((A8.x-A1.x)/7.f))));
            p3D.y = A1.y +
                    digit*(((A8.y-A1.y)/7.f)+
                           ((letter/14.f)*(((A1.y-H1.y)/7.f)-((A8.y-H8.y)/7.f))))-
                    letter*(((A1.y-H1.y)/7.f)-
                            ((digit/14.f)*(((H8.y-H1.y)/7.f)-((A8.y-A1.y)/7.f))));
            p3D.z = A1.z +
                    digit*(((A8.z-A1.z)/7.f)+
                           ((letter/14.f)*(((A1.z-H1.z)/7.f)-((A8.z-H8.z)/7.f))))-
                    letter*(((A1.z-H1.z)/7.f)-
                            ((digit/14.f)*(((H8.z-H1.z)/7.f)-((A8.z-A1.z)/7.f))));

            m_pField[Field::nr(pos)-1]->set3DLocation(p3D);
        }
    }
}

void Chessboard::calculateFieldsPointsOnRemovedBoard(Point3D whiteCloserOuter,
      Point3D whiteFutherInner, Point3D blackCloserOuter, Point3D blackFutherInner)
{
    for (int column=0; column<=1; column++)
    {
        for (int row=0; row<=7; row++)
        {
            PosOnBoard pos;
            pos.Letter = static_cast<LETTER>(row+1);
            pos.Digit = static_cast<DIGIT>(column+1);

            Point3D p3D;
            p3D.x = whiteCloserOuter.x + row*((whiteFutherInner.x - whiteCloserOuter.x)/7.f);
            p3D.y = whiteCloserOuter.y - (column * m_dSquareWidth);
            p3D.z = whiteCloserOuter.z + row*((whiteFutherInner.z - whiteCloserOuter.z)/7.f);

            m_pField[Piece::nr(pos)-1]->set3DLocation(p3D);
        }
    }

    for (int column=2; column<=3; column++)
    {
        for (int row=0; row<=7; row++)
        {
            PosOnBoard pos;
            pos.Letter = static_cast<LETTER>(row+1);
            pos.Digit = static_cast<DIGIT>(column+1);

            Point3D p3D;
            p3D.x = blackCloserOuter.x + row*((blackFutherInner.x - blackCloserOuter.x)/7.f);
            p3D.y = blackCloserOuter.y + ((column-2) * m_dSquareWidth);
            p3D.z = blackCloserOuter.z + row*((blackFutherInner.z - blackCloserOuter.z)/7.f);

            m_pField[Piece::nr(pos)-1]->set3DLocation(p3D);
        }
    }
}

void Chessboard::setPieceOnField(Piece* pPiece, Field* pField, bool bDebugLog /*= false*/)
{
    if (pField->isFieldOccupied(SHOW_ERRORS)) return;
    if (pPiece != nullptr && this->isPieceAlreadyExistsOnBoard(pPiece, SHOW_ERRORS)) return;

    pField->setPieceOnField(pPiece);

    if (bDebugLog)
        qInfo() << "new pieceNr:" << (pPiece == nullptr ? "0" : QString::number(pPiece->getNr()))
                << "on fieldNr:" << pField->getNrAsQStr() << "on" << boardTypeAsQstr(m_boardType);
}

bool Chessboard::isBoardReal(bool bErrorLog /*= false*/)
{
    if (m_bBoardIsReal) return true;
    else
    {
        if (bErrorLog)
            qCritical() << "it's not.";

        return false;
    }
}

bool Chessboard::isPieceAlreadyExistsOnBoard(Piece* pPiece, bool bErrorLog /*= false*/)
{
    for (int i=0; i<=63; ++i)
    {
        short sPieceNr = (m_pField[i]->getPieceOnField() == nullptr ?
                              0 : m_pField[i]->getPieceOnField()->getNr());

        if (pPiece != nullptr && pPiece->getNr() == sPieceNr)
        {
            if (bErrorLog)
                qCritical() << "this piece already exist on board. piece ="
                            << QString::number(pPiece->getNr()) << "on field ="
                            << m_pField[i]->getNrAsQStr();
            return true;
        }
    }
    return false;
}

Field* Chessboard::getField(short sFieldNr) const
{
    if (Field::isInRange(sFieldNr)) return m_pField[sFieldNr-1];
    else return nullptr;
}

Field* Chessboard::getField(PosOnBoard Pos) const
{
    if (Pos.isPosSet(SHOW_ERRORS)) return m_pField[Field::nr(Pos)-1];
    else return nullptr;
}

Field* Chessboard::getFieldWithGivenPieceIfExists(Piece* pPiece, bool bErrorLog /*= false*/)
{
    //no need for checking board, if piece isn't on it anyway
    if (pPiece != nullptr && this->isPieceAlreadyExistsOnBoard(pPiece)) //don't SHOW_ERRORS
    {
        for (int i=0; i<=63; ++i)
        {
            if (m_pField[i]->getPieceOnField() != nullptr)
            {
                short sPieceNr = (m_pField[i]->getPieceOnField() == nullptr ?
                                      0 : m_pField[i]->getPieceOnField()->getNr());

                if (sPieceNr == pPiece->getNr())
                    return m_pField[i];
            }
        }
    }

    if (bErrorLog)
        qCritical() << "field not found. return nullptr";

    return nullptr;
}

QString Chessboard::dumpAllData()
{
    QString QStrData;

    QStrData = "[chessboard.h]\n";
    QStrData += "m_boardType: " + boardTypeAsQstr(m_boardType) + "\n";
    QStrData += "m_bBoardIsReal: " + QString::number(m_bBoardIsReal) + "\n";
    QStrData += "m_a1: " + m_a1.getAsQStr() + "\n";
    QStrData += "m_a8: " + m_a8.getAsQStr() + "\n";
    QStrData += "m_h1: " + m_h1.getAsQStr() + "\n";
    QStrData += "m_h8: " + m_h8.getAsQStr() + "\n";
    QStrData += "m_remWhiteCloserOuter: " + m_remWhiteCloserOuter.getAsQStr() + "\n";
    QStrData += "m_remWhiteFurtherInner: " + m_remWhiteFurtherInner.getAsQStr() + "\n";
    QStrData += "m_remBlackCloserOuter: " + m_remBlackCloserOuter.getAsQStr() + "\n";
    QStrData += "m_remBlackFurtherInner: " + m_remBlackFurtherInner.getAsQStr() + "\n";
    QStrData += "m_dSquareWidth: " + QString::number(m_dSquareWidth) + "\n";
    short sFieldsAmount = m_boardType == B_MAIN ? 64 : 32;
    for (int i=0; i<=sFieldsAmount-1; ++i)
        QStrData += m_pField[i]->dumpAllData() + "\n";

    return QStrData;
}
