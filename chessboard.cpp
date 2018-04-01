#include "chessboard.h"

Chessboard::Chessboard(BOARD BoardType, bool bBoardIsReal /*= true*/):
    _A1(157.4, 76.3, -22.9),
    _A8(306.6, 75.0, -19.1),
    _H1(157.4, -81.9, -23.1),
    _H8(305.6, -79.2, -19.3),
    _remWhiteCloserOuter(108.9, 176.0, -21.8),
    _remWhiteFurtherInner(259.0, 169.3, -19.5), //y is unused
    _remBlackCloserOuter(115.5, -148.4, -23.2),
    _remBlackFurtherInner(267.2, 0, -19.4) //y is unused
{
    _BoardType = BoardType;
    _bBoardIsReal = bBoardIsReal;

    for (int i=0; i<=63; ++i)
        _pField[i] = new Field(i+1);

    if (this->isBoardReal())
    {
        if (BoardType == B_MAIN)
        {
            this->calculateFields3DLocationsOnMainBoard(_A1, _A8, _H1, _H8);
            _dSquareWidth = ((_A8.x - _A1.x)/7 + (_A8.y - _A1.y)/7)/2;
        }
        else if (BoardType == B_REMOVED)
        {
            this->calculateFields3DLocationsOnRemovedBoard(_remWhiteCloserOuter,
                  _remWhiteFurtherInner, _remBlackCloserOuter, _remBlackFurtherInner);
            _dSquareWidth = qFabs(_remWhiteCloserOuter.y - _remWhiteFurtherInner.y);
        }

        this->calculateMarginal3DValues();
        this->calculateMiddleAbovePoint();
        this->calculateRetreatPoints();
    }
}

Chessboard::~Chessboard()
{
    for (int i=0; i<=63; ++i)
    {
        delete _pField[i];
        _pField[i] = nullptr; //good c++ practise
    }
}

void Chessboard::calculateFields3DLocationsOnMainBoard(Point3D A1, Point3D A8,
                                                       Point3D H1, Point3D H8)
{
    for (int digit = 0; digit <= 7; digit++)
    {
        for (int letter = 0; letter <= 7; letter++)
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

            _pField[Field::nr(pos)-1]->setField3DLocation(p3D);
        }
    }
}

void Chessboard::calculateFields3DLocationsOnRemovedBoard(Point3D whiteCloserOuter,
      Point3D whiteFutherInner, Point3D blackCloserOuter, Point3D blackFutherInner)
{
    for (int column=0; column<=1; column++)
    {
        for (int row=0; row<=7; row++)
        {
            PosOnBoard pos;
            pos.Letter = static_cast<LETTER>(column+1);
            pos.Digit = static_cast<DIGIT>(row+1);

            Point3D p3D;
            p3D.x = whiteCloserOuter.x + row*((whiteFutherInner.x - whiteCloserOuter.x)/7.f);
            p3D.y = whiteCloserOuter.y - column * _dSquareWidth;
            p3D.z = whiteCloserOuter.z + row*((whiteFutherInner.z - whiteCloserOuter.z)/7.f);

            _pField[Piece::nr(pos)-1]->setField3DLocation(p3D);
        }
    }

    for (int column=2; column<=3; column++)
    {
        for (int row=0; row<=7; row++)
        {
            PosOnBoard pos;
            pos.Letter = static_cast<LETTER>(column+1);
            pos.Digit = static_cast<DIGIT>(row+1);

            Point3D p3D;
            p3D.x = blackCloserOuter.x + row*((blackFutherInner.x - blackCloserOuter.x)/7.f);
            p3D.y = blackCloserOuter.y + ((column-2)*(-_dSquareWidth));
            p3D.z = blackCloserOuter.z + row*((blackFutherInner.z - blackCloserOuter.z)/7.f);

            _pField[Piece::nr(pos)-1]->setField3DLocation(p3D);
        }
    }
}

void Chessboard::calculateMarginal3DValues()
{
    _MinBoard.x = _MinBoard.y = _MinBoard.z = std::numeric_limits<double>::max();
    _MaxBoard.x = _MaxBoard.y = _MaxBoard.z = std::numeric_limits<double>::min();

    for (int i=0; i<=63; ++i)
    {
        if (_pField[i]->getLocation3D().x < _MinBoard.x)
            _MinBoard.x = _pField[i]->getLocation3D().x;
        if (_pField[i]->getLocation3D().y < _MinBoard.y)
            _MinBoard.y = _pField[i]->getLocation3D().y;
        if (_pField[i]->getLocation3D().z < _MinBoard.z)
            _MinBoard.z = _pField[i]->getLocation3D().z;

        if (_pField[i]->getLocation3D().x > _MaxBoard.x)
            _MaxBoard.x = _pField[i]->getLocation3D().x;
        if (_pField[i]->getLocation3D().y > _MaxBoard.y)
            _MaxBoard.y = _pField[i]->getLocation3D().y;
        if (_pField[i]->getLocation3D().z > _MaxBoard.z) //Piece::dMaxPieceHeight się skraca
            _MaxBoard.z = _pField[i]->getLocation3D().z + Piece::dMaxPieceHeight ;
    }
}

void Chessboard::calculateMiddleAbovePoint()
{
    _middleAbove.x =  (_MinBoard.x = _MaxBoard.x)/2;
    _middleAbove.y =  (_MinBoard.y = _MaxBoard.y)/2;
    _middleAbove.z =  _MaxBoard.z;
}

void Chessboard::calculateRetreatPoints()
{
    _retreatLeft.x = _retreatRight.x = _middleAbove.x;
    _retreatLeft.y = _pField[Field::nr(L_D, D_1)-1]->getLocation3D().y;
    _retreatRight.y = _pField[Field::nr(L_D, D_8)-1]->getLocation3D().y;
    _retreatLeft.z = _retreatRight.z = _MaxBoard.z;
}

void Chessboard::setPieceOnField(Piece* pPiece, Field* pField)
{
    qDebug() << "inside Chessboard::setPieceOnField()";
    if (pField->isFieldOccupied(SHOW_ERRORS)) return;
    if (this->isPieceExistsOnBoard(pPiece, SHOW_ERRORS)) return;

    pField->setPieceOnField(pPiece);
    qDebug() << "Chessboard::setPieceOnField(): new pieceNr:" << pPiece->getNr()
             << "on fieldNr:" << pField->getNrAsQStr();
}

void Chessboard::clearField(Field *pField)
{
    qDebug() << "Chessboard::clearField(): clearing field:" << pField->getNrAsQStr() <<
                ". old piece =" << pField->getPieceOnField()->getNr() << ", now it will be == 0";
    pField->clearField();
}

bool Chessboard::isPointInLocationLimits(Point3D point)
{
    if (!_bBoardIsReal)
    {
        qDebug() << "ERROR: Chessboard::isPointInLocationLimits(): board isn't real";
        return false;
    }

    if (point.x >= _MinBoard.x && point.y >= _MinBoard.y && point.z >= _MinBoard.z &&
            point.x <= _MaxBoard.x && point.y <= _MaxBoard.y && point.z <= _MaxBoard.z
            + Piece::dMaxPieceHeight)
        return true;
    else
    {
        qDebug() << "ERROR: Chessboard::isPointInLocationLimits(): point out of sight. x,y,z="
                 << point.x << point.y << point.z;
        return false;
    }
}

bool Chessboard::isPieceExistsOnBoard(Piece* pPiece, bool bErrorLog /*= false*/)
{
    if (pPiece != nullptr)
    {
        qDebug() << "ERROR: Chessboard::isPieceExistsOnBoard(): piece can't be nullptr";
        return false;
    }

    for (int i=0; i<=63; ++i)
    {
        if (_pField[i]->getPieceOnField() == pPiece)
        {
            if (bErrorLog)
            {
                qDebug() << "ERROR: Chessboard::isPieceExistsOnBoard(): this piece "
                            "already exist on board. piece =" << pPiece->getNr() <<
                            "on field =" << _pField[i]->getNrAsQStr();
            }
            return true;
        }
    }
    return false;
}

bool Chessboard::isBoardReal(bool bErrorLog /*= false*/)
{
    if (_bBoardIsReal) return true;
    else
    {
        if (bErrorLog)
            qDebug() << "ERROR: Chessboard::isBoardReal(): it's not.";

        return false;
    }
}

Field* Chessboard::getFieldWithGivenPieceIfExists(Piece* pPiece)
{
    qDebug() << "inside Chessboard::getFieldWithGivenPieceIfExists(). piece nr ="
             << pPiece->getNr();

    if (this->isPieceExistsOnBoard(pPiece, SHOW_ERRORS))
    {
        for (int i=0; i<=63; ++i)
        {
            if (_pField[i]->getPieceOnField()->getNr() == pPiece->getNr())
                return _pField[i];
        }
    }

    qDebug() << "Chessboard::getFieldWithGivenPieceIfExists(): piece of nr"
             << pPiece->getNr() << "is not on" << boardTypeAsQstr(_BoardType);
    return nullptr;
}

Point3D Chessboard::getBoardPoint3D(BOARD_POINTS BP) const
{
    switch(BP)
    {
    case BP_MIN: return _MinBoard; break;
    case BP_MAX: return _MaxBoard; break;
    case BP_MIDDLE: return _middleAbove; break;
    case BP_RETREAT_LEFT: return _retreatLeft; break;
    case BP_RETREAT_RIGHT: return _retreatRight; break;
    default: qDebug() << "ERROR: Chessboard::getBoardPoint3D(): unknown bp:" << BP;
        return _middleAbove;
    }
}
