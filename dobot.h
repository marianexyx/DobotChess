#ifndef DOBOT_H
#define DOBOT_H

#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>
#include <QValidator>
#include <QList>
#include "DobotDll.h"
#include "DobotType.h"
#include "chessboard.h"
#include "arduinousb.h"
#include "vars/log.h"
#include "vars/basic_vars.h"
#include "vars/board_axis.h"
#include "vars/dobot_moves.h"
#include "vars/board_data_labels.h"

//todo: oddzielić wszystko co związane z szachami od klasy dobota

struct ArmPosForCurrentCmdQueuedIndex
{
    unsigned long long index;
    SEQUENCE_TYPE sequence;
    DOBOT_MOVE move;
    float x;
    float y;
    float z;
    float r;
};

struct PtpCmdActualVal
{
    float x;
    float y;
    float z;
    float r;
};

struct ServoArduino
{
    unsigned long long index;
    bool isGripperOpen;
};

class Dobot: public QObject
{
    Q_OBJECT

private:
    Chessboard *_pChessboard;
    ArduinoUsb *_pArduinoUsb;

    bool connectStatus;
    //typedef struct tagIOPWM {uint8_t address; float frequency; float dutyCycle;}IOPWM;
    IOPWM m_gripperServo;
    const float m_fGripOpened, m_fGripClosed;

    HOMEParams HomeChess;
    Pose pose;
    PtpCmdActualVal m_PtpCmdActualVal, retreatYPlus, retreatYMinus, middleAboveBoard;
    PTPCmd ptpCmd;
    WAITCmd gripperMoveDelay; //komenda mówiąca dobotowi, że ma nic nie robić przez chwilę...
    //...podczas gdy musi się do końca wykonać komenda zamykania chwytaka zanim ruszy dalej

    unsigned long long m_ullCoreQueuedCmdIndex; //aktualny index kolejki w pamięci systemu
    unsigned long long m_ullDobotQueuedCmdIndex; //aktualny id kolejki ruchu wykonywany przez dobota
    //Id nigdy się nie resetuje, bo raczej nie dobiję do 18 triliardów ruchów (unsigned long long)
    unsigned long long m_ullRetreatIndex; //id na którym ramie ma się odsunąć, by nie zasłaniać obrazu kamerki
    unsigned int m_uiQueuedCmdLeftSpace; //ile zostało miejsca w pamięci dobota
    ArmPosForCurrentCmdQueuedIndex m_posIdx; //dane ramienia przypisane do danego indexu dobota
    QList<ArmPosForCurrentCmdQueuedIndex> QueuedCmdIndexList; //kolejka (lista) zapytań do dobota
    ArmPosForCurrentCmdQueuedIndex firstPosId, lastPosId, takenPosId;

    QList<ServoArduino> arduinoGripperStateList;

    void checkPWM();

public:
    Dobot(Chessboard *pChessboard, ArduinoUsb *pArduinoUsb);

    void refreshBtn();
    void initDobot();
    void onPTPsendBtnClicked();
    void closeEvent(QCloseEvent *);

    void gripperAngle(float fDutyCycle);

    void pieceFromTo(DOBOT_MOVE partOfSequence, LETTER letter, DIGIT digit, SEQUENCE_TYPE Type); //todo: do chessboardu?
    void gripperState(DOBOT_MOVE state, SEQUENCE_TYPE Type); //todo: do chessboardu?
    void wait(int nMs, SEQUENCE_TYPE sequence);
    void addCmdToList(DOBOT_MOVE move, SEQUENCE_TYPE sequence = ST_NONE,
                      float x = ACTUAL_POS, float y = ACTUAL_POS,
                      float z = ACTUAL_POS, float r = ACTUAL_POS);
    void armUpDown(DOBOT_MOVE armDestination, DOBOT_MOVE partOfSequence, SEQUENCE_TYPE Type); //todo: do chessboardu?
    void removePiece(int nPieceRowPos, int nPieceColumnPos); //todo: do chessboardu?
    void writeMoveTypeInConsole(DOBOT_MOVE moveState, SEQUENCE_TYPE sequence = ST_NONE, QString QStrMoveFromOrTo = NULL);
    void QueuedIdList();

    //metody dostępowe do pól
    void setCoreQueuedCmdIndex(unsigned long long ullCoreQueuedCmdIndex)
    {m_ullCoreQueuedCmdIndex = ullCoreQueuedCmdIndex;}
    void setDobotQueuedCmdIndex(unsigned long long ullDobotQueuedCmdIndex)
    {m_ullDobotQueuedCmdIndex = ullDobotQueuedCmdIndex;}
    void setRetreatIndex(unsigned long long ullRetreatIndex)
    {m_ullRetreatIndex = ullRetreatIndex;}
    void setQueuedCmdLeftSpace(unsigned int uiQueuedCmdLeftSpace)
    {m_uiQueuedCmdLeftSpace = uiQueuedCmdLeftSpace;}

    unsigned long long getCoreQueuedCmdIndex() const    { return m_ullCoreQueuedCmdIndex; }
    unsigned long long  getDobotQueuedCmdIndex() const  { return m_ullDobotQueuedCmdIndex; }
    unsigned long long  getRetreatIndex() const         { return m_ullRetreatIndex; }
    unsigned int getQueuedCmdLeftSpace() const          { return m_uiQueuedCmdLeftSpace; }
    float getHomePos(char ch)                           { if (ch == 'x') return HomeChess.x;
                                                          else if (ch == 'y') return HomeChess.y;
                                                          else if (ch == 'z') return HomeChess.z;
                                                          else return HomeChess.r; }
    float getmiddleAboveBoardPos(char ch)               { if (ch == 'x') return middleAboveBoard.x;
                                                          else if (ch == 'y') return middleAboveBoard.y;
                                                          else if (ch == 'z') return middleAboveBoard.z;
                                                          else return middleAboveBoard.r; }

public slots:
    void onConnectDobot();
    void onPeriodicTaskTimer();
    void onGetPoseTimer();

signals:
    //sygnały do używania GUI MainWindow
    void addTextToConsole(QString, LOG);
    void JointLabelText(QString QSLabelText, short sJoint);
    void AxisLabelText(QString QSAxisLabelText, char chAxis);
    void RefreshDobotButtonsStates(bool bDobotButtonsStates);
    void deviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion);
    void DobotErrorMsgBox();
    void QueueLabels(int nSpace, int nDobotId, int nCoreMaxId, int nCoreIdLeft, int CoreNextId);
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);
    void showActualDobotQueuedCmdIndexList(QList<ArmPosForCurrentCmdQueuedIndex>);
    void showArduinoGripperStateList(QList<ServoArduino>);
};

#endif // DOBOT_H
