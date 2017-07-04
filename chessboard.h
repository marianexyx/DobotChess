#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "QString"
#include "QObject"
#include "qdebug.h"
#include <vector>
#include "vars/basic_vars.h"
#include "vars/board_axis.h"
#include "vars/dobot_moves.h"

struct ArmPosition
{
    LETTER Letter = L_X;
    DIGIT Digit = D_X;
};

enum WHOSE_TURN { NO_TURN, WHITE_TURN, BLACK_TURN }; //todo: do zewnętrznych plików dać to też
enum SEQUENCE_TYPE { NONE, BADMOVE, REGULAR, PROMOTE_TO_WHAT, PROMOTION, ENPASSANT, CASTLING,
                  CASTLING_KING, CASTLING_ROOK, REMOVING, RESTORE, SERVICE };

//TODO: chessboard powinien być obiektem klasy chess
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
    SEQUENCE_TYPE m_moveType;

    void changeWindowTitle();

    void FENToBoard(QString FENBoard);
    WHOSE_TURN whoseTurn(QString whoseTurn); //TODO: to samo co w _pWebTable

public:
    Chessboard();

    void findBoardPos(QString QStrPiecePositions);
    LETTER findPieceLetterPos(QString QsLetter);
    QString findPieceLetterPos(LETTER letter);
    int fieldNrToFieldPos(int nfieldNr, bool bRow);
    bool isMoveRemoving();
    bool isMoveCastling(QString moveToTest);
    bool isMoveEnpassant(QString moveToTest);
    void castlingFindRookToMove();
    void pieceStateChanged(DOBOT_MOVE partOfSequence, LETTER letter,
                           DIGIT digit, SEQUENCE_TYPE Type);
    bool compareArrays(int nArray1[][8], int nArray2[][8]);
    void saveStatusData(QString status);
    void showBoardInDebug();

    //TODO: jeżeli zrobię poniższe dane (tj. struktury) jako private, to jak się potem do...
    //...nich dobrać metodami dostępowymi?
    ArmPosition PieceFrom, PieceTo, PieceActualPos;

    QString QsPiecieFromTo;             // f.e. "e2e4"
    QString QsAIPiecieFromTo;           //zapamiętany kolejny ruch bota czekający na wywołanie

    //todo: zamienić można na litery odpowiadające im na szachownicy
    int nGripperPiece;                  // nr bierki znajdującej się aktualnie w chwytaku

    QString QStrFuturePromote;

    /*const*/ int anStartBoard[8][8];       //do sprawdzania przy odkładaniu bierek na miejsce czy ...
                                        //...szachownica osiągła swoje startowe ułożenie bierek.
    int anBoard[8][8];                  //plansza jako tablica. liczby to zajęte pola.
                                        //każda liczba to inna bierka.
    int anTempBoard[8][8];              //pomocnicza tablica do sprawdzania czy udało się przywrócić...
                                        //...bierki na szachownicy do oryginalnego/pierwotnego stanu.
    int anRemoved[8][4];               //tablica usuniętych

    double adChessboardPositions_x[8][8];
    double adChessboardPositions_y[8][8];
    double adChessboardPositions_z[8][8];

    double adRemovedPiecesPositions_x[8][4];
    double adRemovedPiecesPositions_y[8][4];
    double adRemovedPiecesPositions_z[8][4];

    const int m_nMaxPieceHeight;
    const int m_nMaxRemovedPieceH; //todo: powinna to być jedna wartość z powyższą?
    int m_nMaxBoardZ;

    //metody dostępowe
    void setWhoseTurn (WHOSE_TURN Turn)         { m_WhoseTurn = Turn; this->changeWindowTitle(); } //todo: zabrać to?
    void setMoveType (SEQUENCE_TYPE Type)       { m_moveType = Type; }
    void setLegalMoves(QStringList legalMoves)  { m_legalMoves = legalMoves; }

    void clearLegalMoves()                      { m_legalMoves.clear(); }

    QString getSiteMoveRequest()                { return m_QStrSiteMoveRequest; }
    QString getPiecieFromTo();
    int getMaxPieceHeight() const               { return m_nMaxPieceHeight; }
    int getMaxRemovedPieceHeight() const        { return m_nMaxRemovedPieceH; }
    QString getGameStatus()                     { return m_QStrGameStatus; }
    WHOSE_TURN getWhoseTurn()                   { return m_WhoseTurn; }
    QString getStrWhoseTurn();
    SEQUENCE_TYPE getMoveType()                 { return m_moveType; }
    QString getCastlings()                      { return m_QStrCastlings; }
    QString getEnpassant()                      { return m_QStrEnpassant; }
    QStringList getLegalMoves()                 { return m_legalMoves; }
    double getMaxBoardZ()                       { return m_nMaxBoardZ; }

signals:
    void addTextToConsole(QString);
    void changeWindowTitle(QString);
};

#endif // CHESSBOARD_H
