#include "chessboard.h"

Chessboard::Chessboard(BOARD BoardType, bool bBoardIsReal /*= true*/,
                       BoardsRealVars realVars /*= BoardsRealVars()*/):
    dMaxPieceHeight(realVars.fPieceHeight)
{
    m_boardType = BoardType;
    m_bBoardIsReal = bBoardIsReal;

    for (int i=0; i<=63; ++i)
        m_pField[i] = new Field(i+1);

    if (this->isBoardReal())
    {
        if (BoardType == B_MAIN)
        {
            m_a1 = realVars.A1;
            m_a8 = realVars.A8;
            m_h1 = realVars.H1;
            m_h8 = realVars.H8;

            this->calculateFieldsPointsOnMainBoard(m_a1, m_a8, m_h1, m_h8);
        }
        else if (BoardType == B_REMOVED)
        {
            m_remWhiteCloserOuter = realVars.remWhiteCloserOuter;
            m_remWhiteCloserInner = realVars.remWhiteCloserInner;
            m_remWhiteFurtherOuter = realVars.remWhiteFurtherOuter;
            m_remWhiteFurtherInner = realVars.remWhiteFurtherInner;
            m_remBlackCloserOuter = realVars.remBlackCloserOuter;
            m_remBlackCloserInner = realVars.remBlackCloserInner;
            m_remBlackFurtherOuter = realVars.remBlackFurtherOuter;
            m_remBlackFurtherInner = realVars.remBlackFurtherInner;

            this->calculateFieldsPointsOnRemovedBoard(m_remWhiteCloserOuter,
                  m_remWhiteCloserInner, m_remWhiteFurtherOuter, m_remWhiteFurtherInner,
                  m_remBlackCloserOuter, m_remBlackCloserInner, m_remBlackFurtherOuter,
                  m_remBlackFurtherInner);
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

double Chessboard::calculateFieldCenter(Point3D bottomLeft, Point3D bottomRight,
                                        Point3D topLeft, Point3D topRight,
                                        int nHorizontalFields, int nVerticalFields,
                                        int nHorizontalFieldNr, int nVerticalFieldNr, AXIS axis)
{
    int nHorizontalAmountOfDistancesBetweenFieldsCenters = nHorizontalFields - 1;
    int nVerticalAmountOfDistancesBetweenFieldsCenters = nVerticalFields - 1;

    //shorter names:
    double hort = nHorizontalAmountOfDistancesBetweenFieldsCenters;
    double vert = nVerticalAmountOfDistancesBetweenFieldsCenters;
    int hNr = nHorizontalFieldNr;
    int vNr = nVerticalFieldNr;
    double bl, br, tl, tr;

    switch(axis)
    {
    case AXIS_X:
        bl = bottomLeft.x;
        br = bottomRight.x;
        tl = topLeft.x;
        tr = topRight.x;
        break;
    case AXIS_Y:
        bl = bottomLeft.y;
        br = bottomRight.y;
        tl = topLeft.y;
        tr = topRight.y;
        break;
    case AXIS_Z:
        bl = bottomLeft.z;
        br = bottomRight.z;
        tl = topLeft.z;
        tr = topRight.z;
        break;
    default: qCritical() << "unknown axis type:" << QString::number(axis);
    }

    //equation explanation: draw 2 non-crossing line segments between 2 diffrent points (each...
    //...point can be used to draw 1 line). divide line segments to number of fields in that...
    //...lines, minus one. now you have all field centers, that are under 2 line segments.
    //...now draw next line segment between calculated specified centers field number. finally...
    //...divide this new line segment to number of fields in this line row, minus one:
    return bl + hNr*((br-bl)/hort) + vNr*((tl-bl)/vert) +
            ((hNr*vNr*(tr-tl-br+bl))/(hort*vert));
}

//test.x = this->calculateFieldCenter(wco, wci, wfo, wfi, 2, 8, col, row, AXIS_X);

void Chessboard::calculateFieldsPointsOnMainBoard(Point3D A1, Point3D A8, Point3D H1, Point3D H8)
{
    for (int digit=0; digit<8; ++digit)
    {
        for (int letter=0; letter<8; ++letter)
        {
            PosOnBoard pos;
            pos.Letter = static_cast<LETTER>(letter+1);
            pos.Digit = static_cast<DIGIT>(digit+1);

            Point3D p3D;
            p3D.x = this->calculateFieldCenter(A1, H1, A8, H8, 8, 8, letter, digit, AXIS_X);
            p3D.y = this->calculateFieldCenter(A1, H1, A8, H8, 8, 8, letter, digit, AXIS_Y);
            p3D.z = this->calculateFieldCenter(A1, H1, A8, H8, 8, 8, letter, digit, AXIS_Z);

            m_pField[Field::nr(pos)-1]->set3DLocation(p3D);
        }
    }
}

void Chessboard::calculateFieldsPointsOnRemovedBoard(Point3D whiteCloserOuter,
         Point3D whiteCloserInner, Point3D whiteFutherOuter, Point3D whiteFutherInner,
         Point3D blackCloserOuter, Point3D blackCloserInner, Point3D blackFutherOuter,
         Point3D blackFutherInner)
{    
    Point3D wco = whiteCloserOuter;
    Point3D wci = whiteCloserInner;
    Point3D wfo = whiteFutherOuter;
    Point3D wfi = whiteFutherInner;

    for (int col=0; col<2; col++)
    {
        for (int row=0; row<8; row++)
        {
            PosOnBoard pos;
            pos.Letter = static_cast<LETTER>(row+1);
            pos.Digit = static_cast<DIGIT>(col+1);

            Point3D p3D;
            p3D.x = this->calculateFieldCenter(wco, wci, wfo, wfi, 2, 8, col, row, AXIS_X);
            p3D.y = this->calculateFieldCenter(wco, wci, wfo, wfi, 2, 8, col, row, AXIS_Y);
            p3D.z = this->calculateFieldCenter(wco, wci, wfo, wfi, 2, 8, col, row, AXIS_Z);

            m_pField[Piece::nr(pos)-1]->set3DLocation(p3D);
        }
    }

    Point3D bco = blackCloserOuter;
    Point3D bci = blackCloserInner;
    Point3D bfo = blackFutherOuter;
    Point3D bfi = blackFutherInner;

    for (int col=2; col<4; col++)
    {
        for (int row=0; row<8; row++)
        {
            PosOnBoard pos;
            pos.Letter = static_cast<LETTER>(row+1);
            pos.Digit = static_cast<DIGIT>(col+1);

            Point3D p3D;
            p3D.x = this->calculateFieldCenter(bco, bci, bfo, bfi, 2, 8, col-2, row, AXIS_X);
            p3D.y = this->calculateFieldCenter(bco, bci, bfo, bfi, 2, 8, col-2, row, AXIS_Y);
            p3D.z = this->calculateFieldCenter(bco, bci, bfo, bfi, 2, 8, col-2, row, AXIS_Z);

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
    short sFieldsAmount = m_boardType == B_MAIN ? 64 : 32;
    for (int i=0; i<=sFieldsAmount-1; ++i)
        QStrData += m_pField[i]->dumpAllData() + "\n";

    return QStrData;
}
