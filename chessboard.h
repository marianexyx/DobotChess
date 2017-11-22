#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QTimer>
#include <QtMath>
#include "QString"
#include "QObject"
#include "qdebug.h"
#include <vector>
#include <typeinfo>
#include "vars/basic_vars.h"
#include "vars/board_axis.h"
#include "vars/dobot_moves.h"
#include "vars/board_data_labels.h"
#include "vars/sequence_types.h"
#include "vars/board_types.h"
#include "vars/turn_types.h"
#include "vars/players_types.h"
#include "vars/board_types.h"
#include "vars/piece_type.h" //todo: includowac caly folder


struct PositionOnBoard
{
    LETTER Letter;
    DIGIT Digit;

    PositionOnBoard(): Letter(L_X), Digit(D_X) {}
    PositionOnBoard(LETTER L, DIGIT D): Letter(L), Digit(D) {}
    PositionOnBoard(int nL, int nD)
    {
        if ((nL >= L_A && nL <= L_H && nD >= D_1 && nD <= D_8) || (nL == L_X && nD == D_X))
        {
            Letter = static_cast<LETTER>(nL);
            Digit = static_cast<DIGIT>(nD);
        }
        else
        {
            Letter = L_X;
            Digit = D_X;
            qDebug() << "ERROR: PositionOnBoard: parameters out of range: nL =" << nL
                     << ", nD =" << nD;
        }
    }
    PositionOnBoard(QString QStrL, DIGIT D)
    {
        Letter = pieceLetterPos(QStrL);
        Digit = D;
        if (Letter == L_X || Digit == D_X)
            qDebug() << "WARNING: PositionOnBoard(QString QStrL, DIGIT D): parameter is X: "
                        "Letter =" << Letter << ", Digit =" << Digit;
    }
    PositionOnBoard(QString QStrL, int nD)
    {
        Letter = pieceLetterPos(QStrL);
        Digit = static_cast<DIGIT>(nD);
        if (Letter == L_X || Digit == D_X)
            qDebug() << "WARNING: PositionOnBoard(QString QStrL, int nD): parameter is X: "
                        "Letter =" << Letter << ", Digit =" << Digit;
    }
    PositionOnBoard(QString QStrBoardPos)
    {
        if (QStrBoardPos.length() != 2)
        {
            qDebug() << "ERROR: PositionOnBoard(QString QStrBoardPos): QStrBoardPos.length() != 2)";
            Letter = L_X;
            Digit = D_X;
        }
        else
        {
            Letter = pieceLetterPos(QStrBoardPos.left(1));
            Digit = static_cast<DIGIT>(QStrBoardPos.right(1).toInt() - 1);
        }
        if (Letter == L_X || Digit == D_X)
            qDebug() << "WARNING: PositionOnBoard(QString QStrBoardPos): parameter is X: "
                        "Letter =" << Letter << ", Digit =" << Digit;
    }

    void setFromQStr(QString QStrL) { Letter = pieceLetterPos(QStrL); }
    QString getAsQStr() { return pieceLetterPosAsQStr(Letter); }
    QString getAsQStrBoardPos() { return pieceLetterPosAsQStr(Letter) + QString::number(Digit+1); }
};

class Piece
{
private:
    int m_pieceID;
    PIECE_TYPE m_PieceType;
    PLAYER_TYPE m_PieceColor;
    PositionOnBoard m_StartField;
    BOARD m_BoardTypePos;

public:
    Piece(int pieceID)
    {
        if (Piece::typeFromNr(pieceID) != -1)
            m_PieceType = Piece::typeFromNr(pieceID);
        else return;

        m_pieceID = pieceID;

        if (pieceID >=16) m_PieceColor = PT_WHITE;
        else m_PieceColor = PT_BLACK;

        m_StartField = this->pieceNrToPositionOnBoard(pieceID);

        m_BoardTypePos = B_MAIN;
    }

    static bool isInRange(int nPieceNr)
    {
        if (nPieceNr < 1 || nPieceNr > 32)
        {
            qDebug() << "ERROR: Piece: pieceNr out of range 1-32:" << nPieceNr;
            return false;
        }
        else return true;
    }

    //todo:dodac funckje zwracajaca string/char bierki typu FEN na podstwie typu bierki i jej koloru
    static PLAYER_TYPE player(char FENSign)
    {
        PLAYER_TYPE playerType;

        switch(FENSign)
        {
        case 'p': case 'r': case 'n': case 'b': case 'k': case 'q':
            playerType = PT_BLACK; break;
        case 'P': case 'R': case 'N': case 'B': case 'K': case 'Q':
            playerType = PT_WHITE; break;
        default:
            qDebug() << "ERROR: Piece::player unknown FENSign val =" << FENSign;
            playerType = PT_NONE;
            break;
        }

        return playerType;
    }

    static PLAYER_TYPE player(int nPieceNr)
    {
        //todo /////////////////////////////////////////////
    }

    static PIECE_TYPE type(char FENSign)
    {
        if (Piece::player(FENSign) == PT_NONE) return -1;

        PIECE_TYPE type;

        switch(FENSign)
        {
        case 'p': case 'P': type = P_PAWN; break;
        case 'r': case 'R': type = P_ROOK; break;
        case 'n': case 'N': type = P_KNIGHT; break;
        case 'b': case 'B': type = P_BISHOP; break;
        case 'k': case 'K': type = P_KING; break;
        case 'q': case 'Q': type = P_QUEEN; break;
        default: break;
        }

        return type;
    }

    static PIECE_TYPE type(int pieceNr)
    {
        if (!Piece::isInRange(pieceNr)) return -1;

        PIECE_TYPE PieceType;

        switch(pieceNr)
        {
        case 1: case 8: case 25: case 32:
            PieceType = P_ROOK; break;
        case 2: case 7: case 26: case 31:
            PieceType = P_KNIGHT; break;
        case 3: case 6: case 27: case 30:
            PieceType = P_BISHOP; break;
        case 4: case 28:
            PieceType = P_QUEEN; break;
        case 5: case 29:
            PieceType = P_KING; break;
        default:
            PieceType = P_PAWN; break;
        }

        return PieceType;
    }

    //todo: 2 ponizsze funkcje sa zdublowany z chessboardu
    static short nr(PositionOnBoard pieceLines)
    {
        short sPieceNr = static_cast<short>(pieceLines.Letter+1) +
                static_cast<short>(pieceLines.Digit)*8;

        if (!Piece::isInRange(pieceNr)) return -1;
        else return sPieceNr;
    }

    static PositionOnBoard Pos(short sPiecedNr)
    {
        PositionOnBoard pieceLines;
        if (!Piece::isInRange(pieceNr)) return pieceLines;

        short sPieceNrColumn, sPieceNrRow;

        if (sPiecedNr % 8 != 0)
        {
            sPieceNrColumn = sPiecedNr / 8;
            sPieceNrRow  = (sPiecedNr - 1) - (sPieceNrColumn * 8);
        }
        else
        {
            sPieceNrColumn = (sPiecedNr / 8) - 1;
            sPieceNrRow = 7;
        }

        pieceLines.Letter = static_cast<LETTER>(sPieceNrRow);
        pieceLines.Digit = static_cast<DIGIT>(sPieceNrColumn);

        return pieceLines;
    }

    int getPieceNr() const { return m_pieceID; }
    PIECE_TYPE getPieceType() const { return m_PieceType; }
    PLAYER_TYPE getPieceColor() const { return m_PieceColor; }
    PositionOnBoard getPieceStartField() const { return m_StartField; }
    BOARD getPieceBoardTypePos() const { return m_BoardTypePos;}
};

struct Point3D
{
    double x;
    double y;
    double z;
};

//todo: tworzyc to jako tablice obiektow w szasownicach?
struct Field
{
    int nNr;
    int nPieceOnField;

    static PositionOnBoard fieldNrToPositionOnBoard(short sFieldNr) //todo: powtrzenie kodu
    //a moze da rade przeniesc te funkcje do chessboardu?
    {
        PositionOnBoard boardPos;

        if (sFieldNr < 1 || sFieldNr > 64)
        {
            if (sFieldNr % 8 != 0) //8, 16, 24, 32, 40, 48, 56 i 64.
            {
                boardPos.Digit = sFieldNr / 8;
                boardPos.Letter  = (sFieldNr - 1) - (sFieldNrColumn * 8);
            }
            else //8, 16, 24, 32, 40, 48, 56 i 64.
            {
                boardPos.Digit = (sFieldNr / 8) - 1;
                boardPos.Letter = 7;
            }
        }
        else qDebug() << "ERROR: Field::fieldNrToPositionOnBoard: "
                         "field out of range. nNr =" << sFieldNr;

        return boardPos;
    }

    static short PositionOnBoardToFieldNr(PositionOnBoard fieldLines)
    {
        short sFieldNr = static_cast<short>(fieldLines.Letter) +
                static_cast<short>(fieldLines.Digit)*8;
        if (sFieldNr < 1 || sFieldNr > 64)
        {
            qDebug() << "ERROR: Field::PositionOnBoardToFieldNr: sPieceNr "
                        "out of range <1,32>:" << sFieldNr;
            return 0;
        }
        else return sFieldNr;
    }

    bool isFieldOccupied() const { return nPieceOnField > 0 ? true : false; }
};

class Chessboard2 //todo: bez makra QOBCJECT. ta szachownica ma by w 100% zalezna
{
   /*
    ok  -każde pole w dwóch formach: (znikna pośrednie konwersje poukrywane po kodzie)
    ok      -jako ID- numer
    ok      -jako struktura pola litera-cyfra
    ok  -bierkę aktualnie na sobie posiadaną (id)
    ok  -nr startowej bierki (id) - ze stratowej szachownicy pobierac
    ok  -sprawdzanie czy pole jest zajęte
    -showBoardInDebug
    -arrayBoardToQStr
    -get/set/clear field - private friend gripper
    -pozycja xyz kazdego pola?
    */
private:
    Field m_Field[64];

public:
    Chessboard2(BOARD boardType)
    {

        switch(boardType)
        {
        case B_MAIN:
        case B_START:
        case B_CHENARD:
            for (int i=1; i >=64; ++i)
            {
                m_Field[i].nNr = i;
                if (i >= 49) m_Field[i].nPieceOnField = i - 32;
                else if (i >= 16) m_Field[i].nPieceOnField = i;
                else m_Field[i].nPieceOnField = 0;
            }
            break;
        case B_REMOVED:
        case B_TEMP:
            for (int i=1; i >=64; ++i)
            {
                m_Field[i].nNr = i;
                m_Field[i].nNr = 0;
            }
            break;
        default:
            qDebug() << "ERROR: Chessboard2::setPieceOnBoard: wrong BOARD val ="
                     << boardType;
            break;
        }
    }

    void showBoardInDebug();
    QString arrayBoardToQStr(QString QStrBoard[8][8]);

    PositionOnBoard getPosOnBoard(int nNr) const
    { return fieldNrToPositionOnBoard(nNr); }

    void setPieceOnField(int nPassedPiece, PositionOnBoard PosOnBoard)
    //todo: friend dla chwytaka?
    {
        int nDestinationNr = Field::PositionOnBoardToFieldNr(PosOnBoard);

        if (nPassedPiece < 1 || nPassedPiece > 32)
        {
            qDebug() << "ERROR: Chessboard2::setPiecePosition: nPiece "
                        "out of range. nPiece =" << nPiece;
            return;
        }
        if (m_Field[nDestinationNr].isFieldOccupied())
        {
            qDebug() << "ERROR: Chessboard2::setPiecePosition: field is already occupied by"
                        " another piece. nPiece =" << nPiece << ", field =" << m_Field[nPiece];
            return;
        }
        for (int i = 1; i >=64; ++i)
        {
            if (nPiece == m_Field[i].nPieceOnField)
            {
                qDebug() << "ERROR: Chessboard2::setPiecePosition: this nPiece "
                            "already exist on board. nPiece =" << nPiece;
                return;
            }
        }

        m_Field[nDestinationNr].nPieceOnField = nPassedPiece;
        qDebug() << "Chessboard2::setPieceOnField- new pieceNr:" << nPassedPiece
                 << "on fieldNr:" << nDestinationNr;
    }

    void clearField(PositionOnBoard PosOnBoard)
    {
        int nNrToClear = Field::PositionOnBoardToFieldNr(PosOnBoard);

        if (m_Field[nNrToClear].isFieldOccupied() == false)
        {
            qDebug() << "ERROR: Chessboard2::clearField: field is already clear. fieldNr ="
                     << m_Field[nNrToClear].nNr << ", piece on it ="
                     << m_Field[nNrToClear].nPieceOnField;
            return;
        }
         Field::fieldNrToPositionOnBoard(m_Field[nNrToClear].nPieceOnField).Letter

        //todo: ladniejsze komunikaty mozna dac zamiast numerow
        qDebug() << "Chessboard2::clearField: clearing field:" << nNrToClear << ". old piece ="
                  << m_Field[nNrToClear].nPieceOnField << ", now its == 0";
        m_Field[nNrToClear].nPieceOnField = 0;
    }
};


//TODO: chessboard powinien być rodzicem klasy chess
class Chessboard: public QObject
{
    Q_OBJECT

private:
    QString m_QStrSiteMoveRequest;
    QString m_QStrGameStatus;
    QString m_QStrBoard[8][8];
    WHOSE_TURN m_WhoseTurn;
    QString m_QStrCastlings;
    QString m_QStrEnpassant;
    QStringList m_legalMoves;
    QStringList m_historyMoves;
    SEQUENCE_TYPE m_moveType;

    QTimer *m_whiteTimer;
    QTimer *m_blackTimer;
    QTimer *m_updateLabelTimer;
    QTimer *m_startQueueTimer;
    int m_nRemainingWhiteTime;
    int m_nRemainingBlackTime;

    const int m_nMaxPieceHeight;
    Point3D m_dMinBoard, m_dMaxBoard;

    void changeWindowTitle(); //todo: to wyrzucenia

    void FENToBoard(QString FENBoard);
    WHOSE_TURN whoseTurn(QString whoseTurn);
    const long m_lTimersStartTime;
    const long m_lTimersStartQueue;

public:
    Chessboard();

    void findBoardPos(QString QStrPiecePositions);
    short PositionOnBoardToFieldNr(PositionOnBoard fieldLines);
    PositionOnBoard fieldNrToPositionOnBoard(short sFieldNr);
    bool isMoveRemoving();
    bool isMoveCastling(QString moveToTest);
    bool isMoveEnpassant(QString moveToTest);
    void castlingFindRookToMove();
    void pieceStateChanged(DOBOT_MOVE partOfSequence, LETTER letter,
                           DIGIT digit, SEQUENCE_TYPE Type);
    bool compareArrays(short nArray1[][8], short nArray2[][8]);
    void saveStatusData(QString status);
    void showBoardInDebug();
    QString arrayBoardToQStr(QString QStrBoard[8][8]);

    void startGameTimer();
    void resetGameTimers();
    QString milisecToClockTime(long lMilis);
    void stopBoardTimers();
    void switchPlayersTimers();
    void startQueueTimer();
    void stopQueueTimer();
    bool bIsMoveInAxisRange(float x, float y, float z); //todo:dobot
    void resetBoardData();

    //todo: mam problemy z zwracaiem tablic do funckyj. nie marnować na...
    //...to teraz czasu i dac jako public
    short m_asBoardMain[8][8];
    /*todo: const*/ short m_anBoardStart[8][8];
    short m_asBoardTemp[8][8];
    short m_asBoardRemoved[8][4];

    //todo: na razie niech to będzie w public
    double m_adChessboardPositions_x[8][8];
    double m_adChessboardPositions_y[8][8];
    double m_adChessboardPositions_z[8][8];
    double m_adRemovedPiecesPositions_x[8][4];
    double m_adRemovedPiecesPositions_y[8][4];
    double m_adRemovedPiecesPositions_z[8][4];

    //TODO: jeżeli zrobię poniższe dane (tj. struktury) jako private, to jak się potem do...
    //...nich dobrać metodami dostępowymi?
    //TODO: używanie tych zmiennych globalnych powoduje że gubię się w kodzie
    PositionOnBoard PieceFrom, PieceTo, PieceActualPos;

    QString QStrAIPiecieFromTo;           //zapamiętany kolejny ruch bota czekający na wywołanie

    //todo: zamienić można na litery odpowiadające im na szachownicy
    int nGripperPiece;                  // nr bierki znajdującej się aktualnie w chwytaku

    QString QStrFuturePromote;

    //metody dostępowe
    void setWhoseTurn(WHOSE_TURN Turn)              { m_WhoseTurn = Turn;
                                                      this->changeWindowTitle(); } //todo: zabrać to?
    void setMoveType(SEQUENCE_TYPE Type)            { m_moveType = Type; }
    void setLegalMoves(QStringList legalMoves)      { m_legalMoves = legalMoves;
                                                      emit showLegalMoves(legalMoves); }
    void setHistoryMoves(QStringList historyMoves)  { m_historyMoves = historyMoves;
                                                      emit showHistoryMoves(historyMoves); }
    //short getBoardByItsType(BOARD boardType);
    void setPieceOnBoard(BOARD boardType, short sPieceNr, short sFieldNr);
    void setPieceOnBoard(BOARD boardType, short sPieceNr, PositionOnBoard fieldLines);
    void setPieceOnBoard(BOARD boardType, PositionOnBoard pieceLines, short sFieldNr);
    void setPieceOnBoard(BOARD boardType, PositionOnBoard pieceLines, PositionOnBoard fieldLines);

    void clearLegalMoves()                          { m_legalMoves.clear();
                                                      emit showLegalMoves(m_legalMoves);}
    void clearHistoryMoves()                        { m_historyMoves.clear();
                                                      emit showHistoryMoves(m_historyMoves); }

    QString getSiteMoveRequest()                    { return m_QStrSiteMoveRequest; }
    QString getPiecieFromTo();
    int getMaxPieceHeight() const                   { return m_nMaxPieceHeight; }
    QString getGameStatus()                         { return m_QStrGameStatus; }
    WHOSE_TURN getWhoseTurn()                       { return m_WhoseTurn; }
    QString getStrWhoseTurn();
    SEQUENCE_TYPE getMoveType()                     { return m_moveType; }
    QString getCastlings()                          { return m_QStrCastlings; }
    QString getEnpassant()                          { return m_QStrEnpassant; }
    QStringList getLegalMoves()                     { return m_legalMoves; }
    QStringList getHisotyMoves()                    { return m_historyMoves; }
    QString getHisotyMovesAsQStr()                  { return m_historyMoves.join(" "); }
    double getMinBoardZ()                           { return m_dMinBoard.z; }
    int getWhiteTimeLeft();
    int getBlackTimeLeft();
    int getStartTimeLeft()                      { return m_startQueueTimer->remainingTime(); }
    bool isStartTimerRunning()                  { return m_startQueueTimer->isActive(); }
    short getPieceOnBoardAsNr(BOARD boardType, short sFieldNr);
    short getPieceOnBoardAsNr(BOARD boardType, PositionOnBoard fieldLines);
    PositionOnBoard getPieceOnBoardAsLines(BOARD boardType, short sFieldNr);
    PositionOnBoard getPieceOnBoardAsLines(BOARD boardType, PositionOnBoard fieldLines);

private slots:
    void timeOutWhite();
    void timeOutBlack();
    void updateTimeLabels();
    void timeOutStartQueue();

signals:
    void addTextToConsole(QString);
    void changeWindowTitle(QString);
    void clearBoard();
    void showBoard(QString);
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);
    void showLegalMoves(QStringList);
    void showHistoryMoves(QStringList);
    void msgFromChessboardToWebsockets(QString); //todo: znowu odwoływanie się w górę do rodzica
};

#endif // CHESSBOARD_H
