#include "chessboard.h"

Chessboard::Chessboard(BOARD BoardType, bool bBoardIsReal /*= true*/,
                       RealVars gameConfigVars /*= RealVars()*/):
    fMaxPieceHeight(gameConfigVars.fPieceHeight)
{
    _BoardType = BoardType;
    _bBoardIsReal = bBoardIsReal;

    for (int i=0; i<=63; ++i)
        _pField[i] = new Field(i+1);

    if (this->isBoardReal())
    {
        if (BoardType == B_MAIN)
        {
            _A1 = gameConfigVars.A1;
            _A8 = gameConfigVars.A8;
            _H1 = gameConfigVars.H1;
            _H8 = gameConfigVars.H8;

            _dSquareWidth = ((_A1.y - _H1.y)/7 + (_A8.x - _H1.x)/7)/2;

            this->calculateFields3DLocationsOnMainBoard(_A1, _A8, _H1, _H8);
        }
        else if (BoardType == B_REMOVED)
        {
            _remWhiteCloserOuter = gameConfigVars.remWhiteCloserOuter;
            _remWhiteFurtherInner = gameConfigVars.remWhiteFurtherInner;
            _remBlackCloserOuter = gameConfigVars.remBlackCloserOuter;
            _remBlackFurtherInner = gameConfigVars.remBlackFurtherInner;

            _dSquareWidth = (((_remWhiteFurtherInner.x + _remBlackFurtherInner.x)/2.f) -
                    ((_remWhiteCloserOuter.x + _remBlackCloserOuter.x)/2.f))/7.f;

            this->calculateFields3DLocationsOnRemovedBoard(_remWhiteCloserOuter,
                  _remWhiteFurtherInner, _remBlackCloserOuter, _remBlackFurtherInner);
        }

        this->calculateMarginal3DValues();
        this->calculateMiddleAbovePoint();
        this->calculateRetreatPoints();
    }
}

Chessboard::~Chessboard()
{
    qDebug() << "destroying board =" << boardTypeAsQstr(_BoardType)
             << ", _bBoardIsReal =" << _bBoardIsReal;

    for (int i=0; i<=63; ++i)
    {
        delete _pField[i];
        _pField[i] = nullptr; //good c++ practise
    }
}

void Chessboard::calculateFields3DLocationsOnMainBoard(Point3D A1, Point3D A8,
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
            pos.Letter = static_cast<LETTER>(row+1);
            pos.Digit = static_cast<DIGIT>(column+1);

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
            pos.Letter = static_cast<LETTER>(row+1);
            pos.Digit = static_cast<DIGIT>(column+1);

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
    _MaxBoard.x = _MaxBoard.y = _MaxBoard.z = -1000; //big int to start comparing

    for (int i=0; i<=63; ++i)
    {
        if (_BoardType == B_REMOVED && i>=32)
            break;

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
        if (_pField[i]->getLocation3D().z > _MaxBoard.z) //fMaxPieceHeight się skraca
            _MaxBoard.z = _pField[i]->getLocation3D().z;
    }
    _MaxBoard.z += (double)fMaxPieceHeight;
}

void Chessboard::calculateMiddleAbovePoint()
{
    _middleAbove.x = (_MinBoard.x + _MaxBoard.x)/2;
    _middleAbove.y = (_MinBoard.y + _MaxBoard.y)/2;
    _middleAbove.z = _MaxBoard.z;

    qDebug() << "Chessboard::calculateMiddleAbovePoint(): board ="
             << boardTypeAsQstr(_BoardType) << ", _middleAbove ="
             << _middleAbove.getAsQStr();
}

void Chessboard::calculateRetreatPoints()
{
    _retreatLeft.x = _retreatRight.x = _middleAbove.x;
    _retreatLeft.y = _pField[Field::nr(L_A, D_4)-1]->getLocation3D().y;
    _retreatRight.y = _pField[Field::nr(L_H, D_4)-1]->getLocation3D().y;
    _retreatLeft.z = _retreatRight.z = _MaxBoard.z;
}

void Chessboard::setPieceOnField(Piece* pPiece, Field* pField, bool bDebugLog /*= false*/)
{

    if (pField->isFieldOccupied(SHOW_ERRORS)) return;
    if (pPiece != nullptr && this->isPieceAlreadyExistsOnBoard(pPiece, SHOW_ERRORS)) return;

    pField->setPieceOnField(pPiece);

    //if (bDebugLog)
        qDebug() << "Chessboard::setPieceOnField(): new pieceNr ="
                 << (pPiece == nullptr ? 0 : pPiece->getNr())
                 << "on fieldNr =" << pField->getNrAsQStr();
}

void Chessboard::clearField(Field* pField, bool bErrorLog /*= false*/)
{
    qDebug() << "inside Chessboard::clearField()";
    short sPieceNr = (pField->getPieceOnField() == nullptr ?
                          0 : pField->getPieceOnField()->getNr());
    qDebug() << "Chessboard::clearField(): clearing field nr ="
             << pField->getNrAsQStr() << ". Old piece nr ="
             << (sPieceNr == 0 ? "0" : Piece::name(sPieceNr))
             << "(nr =" << (sPieceNr == 0 ? "0" : QString::number(sPieceNr)) << ")";
    pField->clearField(bErrorLog);
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
            + (double)fMaxPieceHeight)
        return true;
    else
    {
        qDebug() << "ERROR: Chessboard::isPointInLocationLimits(): point out of sight. x,y,z="
                 << point.x << point.y << point.z;
        return false;
    }
}

bool Chessboard::isPieceAlreadyExistsOnBoard(Piece* pPiece, bool bErrorLog /*= false*/)
{
    for (int i=0; i<=63; ++i)
    {
        short sPieceNr = (_pField[i]->getPieceOnField() == nullptr ?
                              0 : _pField[i]->getPieceOnField()->getNr());

        if (pPiece != nullptr && pPiece->getNr() == sPieceNr)
        {
            if (bErrorLog)
            {
                qDebug() << "ERROR: Chessboard::isPieceAlreadyExistsOnBoard(): this piece "
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

Field* Chessboard::getFieldWithGivenPieceIfExists(Piece* pPiece, bool bErrorLog /*= false*/)
{
   /* qDebug() << "Chessboard::getFieldWithGivenPieceIfExists(): pieceNr ="
             << (pPiece == nullptr ? 0 : pPiece->getNr());*/

    //no need for checking board, if piece isn't on it anyway
    if (pPiece != nullptr &&this->isPieceAlreadyExistsOnBoard(pPiece)) //don't SHOW_ERRORS
    {
        for (int i=0; i<=63; ++i)
        {
            if (_pField[i]->getPieceOnField() != nullptr)
            {
                short sPieceNr = (_pField[i]->getPieceOnField() == nullptr ?
                                      0 : _pField[i]->getPieceOnField()->getNr());

                if (sPieceNr == pPiece->getNr())
                    return _pField[i];
            }
        }
    }
    else qDebug() << "Chessboard::getFieldWithGivenPieceIfExists(): piece don't exists"
                     "on board";

    if (bErrorLog)
        qDebug() << "ERROR: Chessboard::getFieldWithGivenPieceIfExists(): "
                    "field not found. return nullptr";

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
