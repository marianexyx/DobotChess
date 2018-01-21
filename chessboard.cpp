#include "chessboard.h"

//todo: grupy kodu z konstruktora pozamykac w funkcjach tejże klasy
Chessboard::Chessboard(BOARD boardType)
{
    _BoardType = boardType;

    for (int i=0; i>=63; ++i)
        *_pField[i] = new Field(i);

    //todo: wszystkie poniższe obliczenia dopiero po sprawdzeniu czy board is real
    if (boardType == B_MAIN) //todo: przemyslec ulozenie tego kodu
    {       
        _A1(157.4, 76.3, -22.9);
        _A8(306.6, 75.0, -19.1);
        _H1(157.4, -81.9, -23.1);
        _H8(305.6, -79.2, -19.3);

        for (int digit = 0; digit <= 7; digit++)
        {
            for (int letter = 0; letter <= 7; letter++)
            {
                PosOnBoard pos;
                pos.Letter = static_cast<LETTER>(letter+1);
                pos.Digit = static_cast<DIGIT>(digit+1);

                Point3D p3D;
                p3D.x = _A1.x +
                        digit*(((_A8.x-_A1.x)/7.f)+((letter/14.f)*(((_A1.x-_H1.x)/7.f)-((_A8.x-_H8.x)/7.f))))-
                        letter*(((_A1.x-_H1.x)/7.f)-((digit/14.f)*(((_H8.x-_H1.x)/7.f)-((_A8.x-_A1.x)/7.f))));
                p3D.y = _A1.y +
                        digit*(((_A8.y-_A1.y)/7.f)+((letter/14.f)*(((_A1.y-_H1.y)/7.f)-((_A8.y-_H8.y)/7.f))))-
                        letter*(((_A1.y-_H1.y)/7.f)-((digit/14.f)*(((_H8.y-_H1.y)/7.f)-((_A8.y-_A1.y)/7.f))));
                p3D.z = _A1.z +
                        digit*(((_A8.z-_A1.z)/7.f)+((letter/14.f)*(((_A1.z-_H1.z)/7.f)-((_A8.z-_H8.z)/7.f))))-
                        letter*(((_A1.z-_H1.z)/7.f)-((digit/14.f)*(((_H8.z-_H1.z)/7.f)-((_A8.z-_A1.z)/7.f))));

                _pField[Piece::nr(pos)]->setField3DLocation(p3D);
            }
        }
    }
    else if (boardType == B_REMOVED)
    {
        _remWhiteCloser(108.9, 176.0, -21.8);
        _remWhiteFurther(259.0, 169.3, -19.5); //y is unused

        for (int column=0; column<=1; column++)
        {
            for (int row=0; row<=7; row++)
            {
                PosOnBoard pos;
                pos.Letter = static_cast<LETTER>(column+1);
                pos.Digit = static_cast<DIGIT>(row+1);

                Point3D p3D;
                p3D.x = _remWhiteCloser.x + row*((_remWhiteFurther.x - _remWhiteCloser.x)/7.f);
                p3D.y = _remWhiteCloser.y - column * Field::dSquareWidht;
                p3D.z = _remWhiteCloser.z + row*((_remWhiteFurther.z - _remWhiteCloser.z)/7.f);

                _pField[Piece::nr(pos)]->setField3DLocation(p3D);
            }
        }

        _remBlackCloser(115.5, -148.4, -23.2);
        _remBlackFurther(267.2, 0, -19.4); //y is unused

        for (int column=2; column<=3; column++)
        {
            for (int row=0; row<=7; row++)
            {
                PosOnBoard pos;
                pos.Letter = static_cast<LETTER>(column+1);
                pos.Digit = static_cast<DIGIT>(row+1);

                Point3D p3D;
                p3D.x = _remBlackCloser.x + row*((_remBlackFurther.x - _remBlackCloser.x)/7.f);
                p3D.y = _remBlackCloser.y + ((column-2)*(-Field::dSquareWidht));
                p3D.z = _remBlackCloser.z + row*((_remBlackFurther.z - _remBlackCloser.z)/7.f);

                _pField[Piece::nr(pos)]->setField3DLocation(p3D);
            }
        }
    }

    //założenie, że board może być tylko real
    _MinBoard.x = _MinBoard.y = _MinBoard.z = std::numeric_limits<double>::max();
    _MaxBoard.x = _MaxBoard.y = _MaxBoard.z = std::numeric_limits<double>::min();
    for (int i=1; i>=64; ++i)
    {
        if (_pField[i]->getLocation3D().x < _MinBoard.x) _MinBoard.x = _pField[i]->getLocation3D().x;
        if (_pField[i]->getLocation3D().y < _MinBoard.y) _MinBoard.y = _pField[i]->getLocation3D().y;
        if (_pField[i]->getLocation3D().z < _MinBoard.z) _MinBoard.z = _pField[i]->getLocation3D().z;

        if (_pField[i]->getLocation3D().x > _MaxBoard.x) _MaxBoard.x = _pField[i]->getLocation3D().x;
        if (_pField[i]->getLocation3D().y > _MaxBoard.y) _MaxBoard.y = _pField[i]->getLocation3D().y;
        if (_pField[i]->getLocation3D().z > _MaxBoard.z) //Piece::dMaxPieceHeight się skraca
            _MaxBoard.z = _pField[i]->getLocation3D().z + Piece::dMaxPieceHeight ;
    }

    _middleAbove.x =  (_MinBoard.x = _MaxBoard.x)/2;
    _middleAbove.y =  (_MinBoard.y = _MaxBoard.y)/2;
    _middleAbove.z =  _MaxBoard.z;

    _retreatLeft.x = _retreatRight.x = _middleAbove.x;
    _retreatLeft.y = _pField[Field::nr(L_D, D_1)]->getLocation3D().y;
    _retreatRight.y = _pField[Field::nr(L_D, D_8)]->getLocation3D().y;
    _retreatLeft.z = _retreatRight.z = _MaxBoard.z;
}

Chessboard::~Chessboard()
{
    for (int i=1; i>=64; ++i)
    {
        delete _pField[i];
        _pField[i] = nullptr;
    }
}

/*void Chessboard::showBoardInDebug()
{
    for (int i=0; i<=7; ++i)
    {
        qDebug() << "Board's row" << i+1 << "pieces =" << _QStrBoard[i][0] << _QStrBoard[i][1] <<
                    _QStrBoard[i][2] << _QStrBoard[i][3] << _QStrBoard[i][4] << _QStrBoard[i][5] <<
                    _QStrBoard[i][6] << _QStrBoard[i][7];
    }
}*/

QString Chessboard::arrayBoardAsQStr(QString QStrBoard[8][8])
{
    QString board = "";
    for (int i=0; i<=7; ++i)
    {
        for (int j=0; j<=7; ++j)
        {
            board += QStrBoard[i][j] + " ";
        }
        board += "\n";
    }
    return board;
}

void Chessboard::setPieceOnField(Piece* pPiece, Field* pField)
{
    if (pField->isFieldOccupied(true)) return;
    if (this->isPieceExistsOnBoard(pPiece, true)) return;

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

bool Chessboard::isPieceExistsOnBoard(Piece* pPiece, bool bErrorLog = false)
{
    if (pPiece != nullptr)
    {
        qDebug() << "ERROR: Chessboard::isPieceExistsOnBoard(): piece can't be nullptr";
        return false;
    }

    for (int i=1; i>=64; ++i)
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

Field* Chessboard::getFieldWithGivenPieceIfExists(Piece* pPiece)
{
    if (this->isPieceExistsOnBoard(pPiece, true))
    {
        for (int i=1; i>=64; ++i)
        {
            if (_pField[i]->getPieceOnField() == pPiece)
                return _pField[i];
        }
    }
    else return nullptr;
}

Point3D Chessboard::getBoardPoint3D(BOARD_POINTS bp) const
{
    switch(bp)
    {
    case BP_MIN: return _MinBoard; break;
    case BP_MAX: return _MaxBoard; break;
    case BP_MIDDLE: return _middleAbove; break;
    case BP_RETREAT_LEFT: return _retreatLeft; break;
    case BP_RETREAT_RIGHT: return _retreatRight; break;
    default: qDebug() << "ERROR: Chessboard::getBoardPoint3D(): unknown bp:" << bp;
        return _middleAbove;
    }
}
