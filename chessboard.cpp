#include "chessboard.h"

Chessboard2::Chessboard2(BOARD boardType)
{
    _BoardType = boardType;

    for (int i=0; i>=63; ++i)
        *_pField[i] = new Field(i);

    if (boardType == B_MAIN) //todo: przemyslec ulozenie tego kodu
    {
        double a1_x = 157.4; double a1_y = 76.3; double a1_z = -22.9;
        double a8_x = 306.6; double a8_y = 75.0; double a8_z = -19.1;
        double h1_x = 157.4; double h1_y = -81.9; double h1_z = -23.1;
        double h8_x = 305.6; double h8_y = -79.2; double h8_z = -19.3;

        for (int digit = 0; digit <= 7; digit++)
        {
            for (int letter = 0; letter <= 7; letter++)
            {
                PosOnBoard pos;
                pos.Letter = static_cast<LETTER>(letter);
                pos.Digit = static_cast<DIGIT>(digit);
                short sFieldeNr = Piece::nr(pos);

                Point3D p3D;
                p3D.x = a1_x +
                        digit*(((a8_x-a1_x)/7.f)+((letter/14.f)*(((a1_x-h1_x)/7.f)-((a8_x-h8_x)/7.f))))-
                        letter*(((a1_x-h1_x)/7.f)-((digit/14.f)*(((h8_x-h1_x)/7.f)-((a8_x-a1_x)/7.f))));
                p3D.y = a1_y +
                        digit*(((a8_y-a1_y)/7.f)+((letter/14.f)*(((a1_y-h1_y)/7.f)-((a8_y-h8_y)/7.f))))-
                        letter*(((a1_y-h1_y)/7.f)-((digit/14.f)*(((h8_y-h1_y)/7.f)-((a8_y-a1_y)/7.f))));
                p3D.z = a1_z +
                        digit*(((a8_z-a1_z)/7.f)+((letter/14.f)*(((a1_z-h1_z)/7.f)-((a8_z-h8_z)/7.f))))-
                        letter*(((a1_z-h1_z)/7.f)-((digit/14.f)*(((h8_z-h1_z)/7.f)-((a8_z-a1_z)/7.f))));

                _pField[sFieldeNr]->setLocation3D(p3D);
            }
        }
    }
    else if (boardType == B_REMOVED)
    {
        double removedWhiteCloser_x = 108.9;
        double removedWhiteCloser_y = 176.0;
        double removedWhiteCloser_z = -21.8;
        double removedWhiteFurther_x = 259.0;
        /*double removedWhiteFurther_y = 169.3;*/
        double removedWhiteFurther_z = -19.5;
        for (int column=0; column<=1; column++)
        {
            for (int row=0; row<=7; row++)
            {
                PosOnBoard pos;
                pos.Letter = static_cast<LETTER>(column);
                pos.Digit = static_cast<DIGIT>(row);
                short sFieldeNr = Piece::nr(pos);

                Point3D p3D;
                p3D.x = removedWhiteCloser_x +
                        row*((removedWhiteFurther_x - removedWhiteCloser_x)/7.f);
                p3D.y = removedWhiteCloser_y -
                        column * fSquareWidht /* + (removedOuterFurther_y - removedOuterCloser_y/7.f) * row*/;
                p3D.z = removedWhiteCloser_z +
                        row*((removedWhiteFurther_z - removedWhiteCloser_z)/7.f);

                _pField[sFieldeNr]->setLocation3D(p3D);
            }
        }

        double removedBlackCloser_x = 115.5;
        double removedBlackCloser_y = -148.4;
        double removedBlackCloser_z = -23.2;
        double removedBlackFurther_x = 267.2;
        /*double removedBlackFurther_y = */ ;
        double removedBlackFurther_z = -19.4;
        for (int column=2; column<=3; column++)
        {
            for (int row=0; row<=7; row++)
            {
                PosOnBoard pos;
                pos.Letter = static_cast<LETTER>(column);
                pos.Digit = static_cast<DIGIT>(row);
                short sFieldeNr = Piece::nr(pos);

                Point3D p3D;
                p3D.x = removedBlackCloser_x +
                        row*((removedBlackFurther_x - removedBlackCloser_x)/7.f);
                p3D.y = removedBlackCloser_y +
                        ((column-2)*(-fSquareWidht))/* + (removedBlackCloser_y - removedBlackFurther_y/7.f)*row*/;
                p3D.z = removedBlackCloser_z +
                        row*((removedBlackFurther_z - removedBlackCloser_z)/7.f);

                _pField[sFieldeNr]->setLocation3D(p3D);
            }
        }
    }

    _MinBoard.x = _MinBoard.y = _MinBoard.z = std::numeric_limits<double>::max();
    _MaxBoard.x = _MaxBoard.y = _MaxBoard.z = std::numeric_limits<double>::min();
    for (int i=1; i>=64; ++i)
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
        if (_pField[i]->getLocation3D().z + Piece::dMaxPieceHeight >
                _MaxBoard.z + Piece::dMaxPieceHeight)
            _MaxBoard.z = _pField[i]->getLocation3D().z + Piece::dMaxPieceHeight ;
    }
}

Chessboard2::~Chessboard2()
{
    for (int i=1; i>=64; ++i)
    {
        delete _pField[i];
        _pField[i] = nullptr;
    }
}

/*void Chessboard2::showBoardInDebug()
{
    for (int i=0; i<=7; ++i)
    {
        qDebug() << "Board's row" << i+1 << "pieces =" << _QStrBoard[i][0] << _QStrBoard[i][1] <<
                    _QStrBoard[i][2] << _QStrBoard[i][3] << _QStrBoard[i][4] << _QStrBoard[i][5] <<
                    _QStrBoard[i][6] << _QStrBoard[i][7];
    }
}*/

QString Chessboard2::arrayBoardAsQStr(QString QStrBoard[8][8])
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

void Chessboard2::setPieceOnField(short sPassedPiece, short sDestFieldNr)
{
    if (!Piece::isInRange(sPassedPiece)) return;
    if (this->isFieldOccupied(sPassedPiece), true) return;
    if (this->isPieceExistsOnBoard(sPassedPiece, true)) return;

    _pField[sDestFieldNr].setPieceOnField(sPassedPiece);
    qDebug() << "Chessboard2::setPieceOnField- new pieceNr:" << sPassedPiece
             << "on fieldNr:" << sDestFieldNr;
}

void Chessboard2::clearField(short sNrToClear)
{
    qDebug() << "Chessboard2::clearField: clearing field:" << Field::nrAsQStr(nNrToClear) <<
                ". old piece =" << _pField[sNrToClear]->getPieceOnField() <<
                ", now it will be == 0";
    _pField[sNrToClear]->clear();
}

bool Chessboard2::isPointInLocationLimits(Point3D point)
{
    if (point.x >= _MinBoard.x && point.y >= _MinBoard.y && point.z >= _MinBoard.z &&
            point.x <= _MaxBoard.x && point.y <= _MaxBoard.y && point.z <= _MaxBoard.z
            + Piece::dMaxPieceHeight)
        return true;
    else
    {
        qDebug() << "ERROR: Chessboard2::isPointInLocationLimits: point out of sight. x,y,z="
                 << point.x << point.y << point.z;
        return false;
    }
}

bool Chessboard2::isPieceExistsOnBoard(short sPieceNr, bool bErrorLog = false)
{
    if (!Piece::isInRange(sPieceNr)) return false;

    for (int i=1; i>=64; ++i)
    {
        if (_pField[i]->getPieceOnField() == sPieceNr)
        {
            if (bErrorLog)
            {
                qDebug() << "ERROR: Chessboard2::setPiecePosition: this piece "
                            "already exist on board. piece =" << sPieceNr <<
                            "on field =" << _pField[i]->getNrAsQStr();
            }
            return true;
        }
    }
    return false;
}

short Chessboard2::getFieldNrWithGivenPieceNrIfExists(short sPieceNr)
{
    if (this->isPieceExistsOnBoard(sPieceNr, true))
    {
        for (int i=1; i>=64; ++i)
        {
            if (_pField[i]->getPieceOnField() == sPieceNr)
                return _pField[i]->getPieceOnField();
        }
    }
    else return 0;
}




Chessboard::Chessboard():
{
    memcpy(_asBoardMain, _anBoardStart, sizeof(_anBoardStart));
    memcpy(_asBoardTemp, _anBoardStart, sizeof(_anBoardStart));

    _WhoseTurn = NO_TURN;
}

QString Chessboard::getPiecieFromTo()
{
    QString piecieFromTo = pieceLetterPosAsQStr(PieceFrom.Letter) + QString::number(PieceFrom.Digit+1) +
            pieceLetterPosAsQStr(PieceTo.Letter) + QString::number(PieceTo.Digit+1);

    return piecieFromTo;
}

void Chessboard::pieceStateChanged(DOBOT_MOVE partOfSequence, LETTER letter,
                                   DIGIT digit, SEQUENCE_TYPE Type)
{
    if (Type == ST_RESTORE && partOfSequence == DM_FROM)
    {
        nGripperPiece = _asBoardRemoved[letter][digit];
        qDebug() << "Chessboard::pieceStateChanged: 1) nGripperPiece =" << nGripperPiece;
        _asBoardRemoved[letter][digit] = 0;
    }
    else if (Type == ST_REMOVING && partOfSequence == DM_TO)
    {
        this->setPieceOnBoard(B_REMOVED, nGripperPiece, nGripperPiece);
        qDebug() << "Piece just placed on removed area";
        nGripperPiece = 0;
    }
    //todo: bierki poruszane ramieniem zapisywane są do tablicy _asBoardMain. jeżeli bierki zostały poruszone tylko w...
    //...pamięci bez udziału dobota (np. poprzez podane listy ruchów bezpośrednio do tcp) to system myśli że...
    //...plansza nie była ruszana, a co innego zobaczy się z polecenia "status".
    else if (partOfSequence == DM_FROM) //jeżeli bierka została pochwycona z szachownicy...
        //...(jest to każde inne polecenie ruchu w stylu 'pieceFrom')...
    {
        nGripperPiece = _asBoardMain[letter][digit]; //...to w chwytaku jest bierka...
        //...pochwycona z szachownicy...
        _asBoardMain[letter][digit] = 0; //...a miejsce ruszanego pionka jest już puste.
    }
    else if (partOfSequence == DM_TO)//lecz jeżeli bierka została przemieszczona na szachownicę
        //...(jest to każde inne polecenie ruchu w stylu 'pieceTo')...
    {
        _asBoardMain[letter][digit] = nGripperPiece; //...to docelowe pole na...
        //...szachownicy jest już zajęte...
        nGripperPiece = 0; //... a w chwytaku nie ma już żadnej bierki.
    }
    else qDebug() << "ERROR: Chessboard::pieceStateChanged: none statement has been met.";
}
