#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QTimer>
#include <QtMath>
#include "QString"
#include "QObject"
#include "qdebug.h"
#include <vector>
#include "vars/basic_vars.h"
#include "vars/board_axis.h"
#include "vars/dobot_moves.h"
#include "vars/board_data_labels.h"
#include "vars/sequence_types.h"
#include "vars/board_types.h"
#include "vars/turn_types.h"

struct FieldLinesPos
{
    LETTER Letter = L_X;
    DIGIT Digit = D_X;
};

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
    QStringList m_historyMoves;
    SEQUENCE_TYPE m_moveType;

    QTimer *m_whiteTimer;
    QTimer *m_blackTimer;
    QTimer *m_updateLabelTimer;
    QTimer *m_startQueueTimer;
    int m_nRemainingWhiteTime;
    int m_nRemainingBlackTime;

    const int m_nMaxPieceHeight;
    const int m_nMaxRemovedPieceH; //todo: powinna to być jedna wartość z powyższą?
    int m_nMaxBoardZ;

    void changeWindowTitle();

    void FENToBoard(QString FENBoard);
    WHOSE_TURN whoseTurn(QString whoseTurn);
    const long m_lTimersStartTime;
    const long m_lTimersStartQueue;

public:
    Chessboard();

    void findBoardPos(QString QStrPiecePositions);
    short fieldLinesPosToFieldNr(FieldLinesPos fieldLines);
    FieldLinesPos fieldNrToFieldLinesPos(short sFieldNr);
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
    FieldLinesPos PieceFrom, PieceTo, PieceActualPos;

    QString QsPiecieFromTo;             // f.e. "e2e4"
    QString QsAIPiecieFromTo;           //zapamiętany kolejny ruch bota czekający na wywołanie

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
    void setPieceOnBoard(BOARD boardType, short sPieceNr, FieldLinesPos fieldLines);
    void setPieceOnBoard(BOARD boardType, FieldLinesPos pieceLines, short sFieldNr);
    void setPieceOnBoard(BOARD boardType, FieldLinesPos pieceLines, FieldLinesPos fieldLines);

    void clearLegalMoves()                          { m_legalMoves.clear();
                                                      emit showLegalMoves(m_legalMoves);}
    void clearHistoryMoves()                        { m_historyMoves.clear();
                                                      emit showHistoryMoves(m_historyMoves); }

    QString getSiteMoveRequest()                    { return m_QStrSiteMoveRequest; }
    QString getPiecieFromTo();
    int getMaxPieceHeight() const                   { return m_nMaxPieceHeight; }
    int getMaxRemovedPieceHeight() const            { return m_nMaxRemovedPieceH; }
    QString getGameStatus()                         { return m_QStrGameStatus; }
    WHOSE_TURN getWhoseTurn()                       { return m_WhoseTurn; }
    QString getStrWhoseTurn();
    SEQUENCE_TYPE getMoveType()                     { return m_moveType; }
    QString getCastlings()                          { return m_QStrCastlings; }
    QString getEnpassant()                          { return m_QStrEnpassant; }
    QStringList getLegalMoves()                     { return m_legalMoves; }
    QStringList getHisotyMoves()                    { return m_historyMoves; }
    QString getHisotyMovesAsQStr()                  { return m_historyMoves.join(" "); }
    double getMaxBoardZ()                           { return m_nMaxBoardZ; }
    int getWhiteTimeLeft();
    int getBlackTimeLeft();
    int getStartTimeLeft()                      { return m_startQueueTimer->remainingTime(); }
    bool isStartTimerRunning()                  { return m_startQueueTimer->isActive(); }
    short getPieceOnBoardAsNr(BOARD boardType, short sFieldNr);
    short getPieceOnBoardAsNr(BOARD boardType, FieldLinesPos fieldLines);
    FieldLinesPos getPieceOnBoardAsLines(BOARD boardType, short sFieldNr);
    FieldLinesPos getPieceOnBoardAsLines(BOARD boardType, FieldLinesPos fieldLines);

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
