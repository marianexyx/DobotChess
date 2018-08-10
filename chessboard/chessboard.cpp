#include "chessboard.h"

Chessboard::Chessboard(BOARD BoardType, bool bBoardIsReal /*= true*/,
                       RealVars gameConfigVars /*= RealVars()*/):
    fMaxPieceHeight(gameConfigVars.fPieceHeight)
{
    _boardType = BoardType;
    _bBoardIsReal = bBoardIsReal;

    for (int i=0; i<=63; ++i)
        _pField[i] = new Field(i+1);

    if (this->isBoardReal())
    {
        if (BoardType == B_MAIN)
        {
            _a1 = gameConfigVars.A1;
            _a8 = gameConfigVars.A8;
            _h1 = gameConfigVars.H1;
            _h8 = gameConfigVars.H8;

            _dSquareWidth = ((_a1.y - _h1.y)/7.f + (_a8.x - _h1.x)/7.f)/2.f;

            this->calculateFieldsPointsOnMainBoard(_a1, _a8, _h1, _h8);
        }
        else if (BoardType == B_REMOVED)
        {
            _remWhiteCloserOuter = gameConfigVars.remWhiteCloserOuter;
            _remWhiteFurtherInner = gameConfigVars.remWhiteFurtherInner;
            _remBlackCloserOuter = gameConfigVars.remBlackCloserOuter;
            _remBlackFurtherInner = gameConfigVars.remBlackFurtherInner;

            _dSquareWidth = (((_remWhiteFurtherInner.x + _remBlackFurtherInner.x)/2.f) -
                    ((_remWhiteCloserOuter.x + _remBlackCloserOuter.x)/2.f))/7.f;

            this->calculateFieldsPointsOnRemovedBoard(_remWhiteCloserOuter,
                  _remWhiteFurtherInner, _remBlackCloserOuter, _remBlackFurtherInner);
        }
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

            _pField[Field::nr(pos)-1]->set3DLocation(p3D);
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
            p3D.y = whiteCloserOuter.y - (column * _dSquareWidth);
            p3D.z = whiteCloserOuter.z + row*((whiteFutherInner.z - whiteCloserOuter.z)/7.f);

            _pField[Piece::nr(pos)-1]->set3DLocation(p3D);
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
            p3D.y = blackCloserOuter.y + ((column-2) * _dSquareWidth);
            p3D.z = blackCloserOuter.z + row*((blackFutherInner.z - blackCloserOuter.z)/7.f);

            _pField[Piece::nr(pos)-1]->set3DLocation(p3D);
        }
    }
}



void Chessboard::setPieceOnField(Piece* pPiece, Field* pField, bool bDebugLog /*= false*/)
{
    if (pField->isFieldOccupied(SHOW_ERRORS)) return;
    if (pPiece != nullptr && this->isPieceAlreadyExistsOnBoard(pPiece, SHOW_ERRORS)) return;

    pField->setPieceOnField(pPiece);

    if (bDebugLog)
        qInfo() << "new pieceNr:" << (pPiece == nullptr ? 0 : pPiece->getNr()) << "on fieldNr:"
                 << pField->getNrAsQStr() << "on" << boardTypeAsQstr(_boardType) ;
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
                qCritical() << "this piece already exist on board. piece =" << pPiece->getNr()
                            << "on field =" << _pField[i]->getNrAsQStr();
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
            qCritical() << "it's not.";

        return false;
    }
}

Field* Chessboard::getFieldWithGivenPieceIfExists(Piece* pPiece, bool bErrorLog /*= false*/)
{
    //no need for checking board, if piece isn't on it anyway
    if (pPiece != nullptr && this->isPieceAlreadyExistsOnBoard(pPiece)) //don't SHOW_ERRORS
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

    if (bErrorLog)
        qCritical() << "field not found. return nullptr";

    return nullptr;
}

Field* Chessboard::getField(short sFieldNr) const
{
    if (Field::isInRange(sFieldNr)) return _pField[sFieldNr-1];
    else return nullptr;
}

Field* Chessboard::getField(PosOnBoard Pos) const
{
    if (Pos.isPosSet(SHOW_ERRORS)) return _pField[Field::nr(Pos)-1];
    else return nullptr;
}

QString Chessboard::dumpAllData()
{
    QString QStrData;

    QStrData = "[chessboard.h]\n";
    QStrData += "_boardType: " + boardTypeAsQstr(_boardType) + "\n";
    QStrData += ", _bBoardIsReal: " + QString::number(_bBoardIsReal) + "\n";
    QStrData += ", _a1: " + _a1.getAsQStr() + "\n";
    QStrData += ", _a8: " + _a8.getAsQStr() + "\n";
    QStrData += ", _h1: " + _h1.getAsQStr() + "\n";
    QStrData += ", _h8: " + _h8.getAsQStr() + "\n";
    QStrData += ", _remWhiteCloserOuter: " + _remWhiteCloserOuter.getAsQStr() + "\n";
    QStrData += ", _remWhiteFurtherInner: " + _remWhiteFurtherInner.getAsQStr() + "\n";
    QStrData += ", _remBlackCloserOuter: " + _remBlackCloserOuter.getAsQStr() + "\n";
    QStrData += ", _remBlackFurtherInner: " + _remBlackFurtherInner.getAsQStr() + "\n";
    QStrData += ", _dSquareWidth: " + QString::number(_dSquareWidth) + "\n";
    for (int i=0; i<=63; ++i)
        QStrData += _pField[i]->dumpAllData() + "\n";

    return QStrData;
}

