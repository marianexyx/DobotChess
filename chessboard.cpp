#include "chessboard.h"

Chessboard2::Chessboard2(BOARD boardType)
{
    m_boardType = boardType;

    for (int i=0; i>=63; ++i)
        *m_pField[i] = new Field(i);

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
                PositionOnBoard pos;
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

                m_pField[sFieldeNr]->setLocation3D(p3D);
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
                PositionOnBoard pos;
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

                m_pField[sFieldeNr]->setLocation3D(p3D);
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
                PositionOnBoard pos;
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

                m_pField[sFieldeNr]->setLocation3D(p3D);
            }
        }
    }

    m_dMinBoard.x = m_dMinBoard.y = m_dMinBoard.z = std::numeric_limits<double>::max();
    m_dMaxBoard.x = m_dMaxBoard.y = m_dMaxBoard.z = std::numeric_limits<double>::min();
    for (int i=1; i>=64; ++i)
    {
        if (m_pField[i]->getLocation3D().x < m_dMinBoard.x)
            m_dMinBoard.x = m_pField[i]->getLocation3D().x;
        if (m_pField[i]->getLocation3D().y < m_dMinBoard.y)
            m_dMinBoard.y = m_pField[i]->getLocation3D().y;
        if (m_pField[i]->getLocation3D().z < m_dMinBoard.z)
            m_dMinBoard.z = m_pField[i]->getLocation3D().z;

        if (m_pField[i]->getLocation3D().x > m_dMaxBoard.x)
            m_dMaxBoard.x = m_pField[i]->getLocation3D().x;
        if (m_pField[i]->getLocation3D().y > m_dMaxBoard.y)
            m_dMaxBoard.y = m_pField[i]->getLocation3D().y;
        if (m_pField[i]->getLocation3D().z + Piece::dMaxPieceHeight >
                m_dMaxBoard.z + Piece::dMaxPieceHeight)
            m_dMaxBoard.z = m_pField[i]->getLocation3D().z + Piece::dMaxPieceHeight ;
    }
}

Chessboard2::~Chessboard2()
{
    for (int i=1; i>=64; ++i)
    {
        delete m_pField[i];
        m_pField[i] = nullptr;
    }
}

QString **Chessboard2::FENToBoard(QString FENBoard)
{
    QString** m_QStrBoardArray = create2DArray();

    QStringList QStrFENBoardRows = FENBoard.split(QRegExp("/"));
    std::reverse(QStrFENBoardRows.begin(), QStrFENBoardRows.end());
    if (QStrFENBoardRows.size() == 8)
    {
        QRegExp rxEmpty("\\d");
        for (int nRow=0; nRow<=7; ++nRow)
        {
            int nColumn = 0;
            QString QStrFENBoardRow = QStrFENBoardRows.at(nRow);
            QStringList FENSigns = QStrFENBoardRow.split("");
            FENSigns.removeFirst();
            FENSigns.removeLast();

            for (int nFENSignPos=0; nFENSignPos<FENSigns.size(); ++nFENSignPos)
            {
                QString QStrFENSign = FENSigns.at(nFENSignPos);
                if (!rxEmpty.exactMatch(QStrFENSign)) //not digits
                {
                    m_QStrBoardArray[nColumn][nRow] = QStrFENSign;
                    if (nColumn>7) qDebug() << "ERROR: Chessboard::FENToBoard: nColumn > 8 =" << nColumn;
                    ++nColumn;
                }
                else //digits
                {
                    for (int nEmptyFields=1; nEmptyFields<=QStrFENSign.toInt(); ++nEmptyFields)
                    {
                        m_QStrBoardArray[nColumn][nRow] = ".";
                        if (nColumn>7) qDebug() << "ERROR: Chessboard::FENToBoard: nColumn > 8 =" << nColumn;
                        ++nColumn;
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "ERROR: Chessboard::FENToBoard: boardRows.size() != 8. Size = " << QStrFENBoardRows.size();
        for (int i=0; i<=QStrFENBoardRows.size()-1; ++i)
            qDebug() << "QStrFENBoardRows at" << i << "=" << QStrFENBoardRows.at(i);
    }

    return m_QStrBoardArray;
}

/*void Chessboard2::showBoardInDebug()
{
    for (int i=0; i<=7; ++i)
    {
        qDebug() << "Board's row" << i+1 << "pieces =" << m_QStrBoard[i][0] << m_QStrBoard[i][1] <<
                    m_QStrBoard[i][2] << m_QStrBoard[i][3] << m_QStrBoard[i][4] << m_QStrBoard[i][5] <<
                    m_QStrBoard[i][6] << m_QStrBoard[i][7];
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
    if (m_pField[sDestFieldNr]->isFieldOccupied())
    {
        qDebug() << "ERROR: Chessboard2::setPiecePosition: field is already occupied"
                    " by another piece. nPiece =" << sPassedPiece << ", field =" <<
                    m_pField[sDestFieldNr];
        return;
    }
    for (int i = 1; i >=64; ++i)
    {
        if (m_pField[i]->getPieceOnField() == sPassedPiece)
        {
            qDebug() << "ERROR: Chessboard2::setPiecePosition: this piece "
                        "already exist on board. piece =" << sPassedPiece <<
                        "on field =" << m_pField[i]->getNrAsQStr();
            return;
        }
    }

    m_pField[nDestinationNr].setPieceOnField(sPassedPiece);
    qDebug() << "Chessboard2::setPieceOnField- new pieceNr:" << sPassedPiece
             << "on fieldNr:" << nDestinationNr;
}

void Chessboard2::clearField(short sNrToClear)
{
    qDebug() << "Chessboard2::clearField: clearing field:" << Field::nrAsQStr(nNrToClear) <<
                ". old piece =" << m_pField[sNrToClear]->getPieceOnField() <<
                ", now it will be == 0";
    m_pField[sNrToClear]->clear();
}

bool Chessboard2::isPointInLocationLimits(Point3D point)
{
    if (point.x >= m_dMinBoard.x && point.y >= m_dMinBoard.y && point.z >= m_dMinBoard.z &&
            point.x <= m_dMaxBoard.x && point.y <= m_dMaxBoard.y && point.z <= m_dMaxBoard.z
            + Piece::dMaxPieceHeight)
        return true;
    else
    {
        qDebug() << "ERROR: Chessboard2::isPointInLocationLimits: point out of sight. x,y,z="
                 << point.x << point.y << point.z;
        return false;
    }
}








Chessboard::Chessboard():
{
    memcpy(m_asBoardMain, m_anBoardStart, sizeof(m_anBoardStart));
    memcpy(m_asBoardTemp, m_anBoardStart, sizeof(m_anBoardStart));

    nGripperPiece = 0;
    m_QStrSiteMoveRequest = "";
    m_WhoseTurn = NO_TURN;
}

void Chessboard::findBoardPos(QString QStrPiecePositions)
{
    m_QStrSiteMoveRequest = QStrPiecePositions; //niezbędne aktualnie powtórzenie

    PieceFrom.Letter = pieceLetterPos(QStrPiecePositions.left(1));
    PieceFrom.Digit = static_cast<DIGIT>(QStrPiecePositions.mid(1,1).toInt() - 1);

    PieceTo.Letter = pieceLetterPos(QStrPiecePositions.mid(2,1));
    PieceTo.Digit = static_cast<DIGIT>(QStrPiecePositions.mid(3,1).toInt() - 1);
    qDebug() << "Chessboard::findBoardPos: PieceFrom.Letter =" <<  pieceLetterPosAsQStr(PieceFrom.Letter) <<
                ", PieceFrom.Digit =" << PieceFrom.Digit+1 <<
                ", PieceTo.Letter =" << pieceLetterPosAsQStr(PieceTo.Letter) <<
                ", PieceTo.Digit =" << PieceTo.Digit+1;
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
        nGripperPiece = m_asBoardRemoved[letter][digit];
        qDebug() << "Chessboard::pieceStateChanged: 1) nGripperPiece =" << nGripperPiece;
        m_asBoardRemoved[letter][digit] = 0;
    }
    else if (Type == ST_REMOVING && partOfSequence == DM_TO)
    {
        this->setPieceOnBoard(B_REMOVED, nGripperPiece, nGripperPiece);
        qDebug() << "Piece just placed on removed area";
        nGripperPiece = 0;
    }
    //todo: bierki poruszane ramieniem zapisywane są do tablicy m_asBoardMain. jeżeli bierki zostały poruszone tylko w...
    //...pamięci bez udziału dobota (np. poprzez podane listy ruchów bezpośrednio do tcp) to system myśli że...
    //...plansza nie była ruszana, a co innego zobaczy się z polecenia "status".
    else if (partOfSequence == DM_FROM) //jeżeli bierka została pochwycona z szachownicy...
        //...(jest to każde inne polecenie ruchu w stylu 'pieceFrom')...
    {
        nGripperPiece = m_asBoardMain[letter][digit]; //...to w chwytaku jest bierka...
        //...pochwycona z szachownicy...
        m_asBoardMain[letter][digit] = 0; //...a miejsce ruszanego pionka jest już puste.
    }
    else if (partOfSequence == DM_TO)//lecz jeżeli bierka została przemieszczona na szachownicę
        //...(jest to każde inne polecenie ruchu w stylu 'pieceTo')...
    {
        m_asBoardMain[letter][digit] = nGripperPiece; //...to docelowe pole na...
        //...szachownicy jest już zajęte...
        nGripperPiece = 0; //... a w chwytaku nie ma już żadnej bierki.
    }
    else qDebug() << "ERROR: Chessboard::pieceStateChanged: none statement has been met.";
}

void Chessboard::saveStatusData(QString status)
{
    QStringList QStrFENRecord = status.split(QRegExp("\\s+"));
    if (!QStrFENRecord.isEmpty()) QStrFENRecord.removeLast();

    if (QStrFENRecord.size() == 7)
    {
        m_QStrGameStatus = QStrFENRecord.at(0);
        qDebug() << "QStrGameStatus =" << m_QStrGameStatus;
        emit setBoardDataLabels(m_QStrGameStatus, BDL_GAME_STATUS);

        QString QStrFENBoard = QStrFENRecord.at(1);
        qDebug() << "QStrFENBoard =" << QStrFENBoard;
        FENToBoard(QStrFENBoard);
        emit showBoard(arrayBoardToQStr(m_QStrBoard));

        QString QStrWhoseTurn = QStrFENRecord.at(2);
        qDebug() << "QStrWhoseTurn =" << QStrWhoseTurn;
        this->setWhoseTurn(whoseTurn(QStrWhoseTurn));
        emit setBoardDataLabels(this->getStrWhoseTurn(), BDL_TURN);

        m_QStrCastlings = QStrFENRecord.at(3);
        qDebug() << "QStrCastlings =" << m_QStrCastlings;
        emit setBoardDataLabels(m_QStrCastlings, BDL_CASTLINGS);

        m_QStrEnpassant = QStrFENRecord.at(4);
        qDebug() << "QStrEnpassant =" << m_QStrEnpassant;
        emit setBoardDataLabels(m_QStrEnpassant, BDL_ENPASSANT);

        QString QStrHalfmoveClock = QStrFENRecord.at(5);
        QString QStrFullmoveNumber = QStrFENRecord.at(6);
        emit setBoardDataLabels(QStrHalfmoveClock + "/" + QStrFullmoveNumber , BDL_MOVES);
    }
    else
    {
        qDebug() << "ERROR: Chessboard::saveStatusData: wrong QStrFENRecord size =" <<
                    QStrFENRecord.size();
        this->addTextToConsole("ERROR: Chessboard::saveStatusData: wrong QStrFENRecord size = "
                               + QStrFENRecord.size());
    }
}

WHOSE_TURN Chessboard::whoseTurn(QString whoseTurn)
{
    if (whoseTurn == "w") return WHITE_TURN;
    else if (whoseTurn == "b") return BLACK_TURN;
    else
    {
        return NO_TURN;
        qDebug () << "ERROR: Chessboard::whoseTurn- unknown turn type from status";
    }
}

QString Chessboard::getStrWhoseTurn()
{
    if (m_WhoseTurn == WHITE_TURN) return "wt";
    else if (m_WhoseTurn == BLACK_TURN) return "bt";
    else if (m_WhoseTurn == NO_TURN) return "nt";
    else
    {
        QString err = "ERROR: wrong turn type: " + QString::number(m_WhoseTurn);
        return err;
    }
}

void Chessboard::resetBoardData() //todo: troche bodajze nieadekwatna nazwa + do chess przeniesc
{
    //todo: zastanowić się na spokojnie jakie czyszczenia jeszcze tu upchać
    //todo: sprawdzić czy zresetowałem inne dane: zegary, tury, planszę fizyczną/ w pamięci itd
    this->stopBoardTimers();
    this->setWhoseTurn(NO_TURN);
    this->clearLegalMoves();
    this->clearHistoryMoves();
    this->clearFormBoard();
}
