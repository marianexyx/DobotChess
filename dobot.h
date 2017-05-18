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
#include "vars/log.h"

//todo: oddzielić wszystko co związane z szachami od klasy dobota

const int ACTUAL_POS = 1000;
enum DOBOT_MOVE { TO_POINT, HOME, WAIT, OPEN_GRIP, CLOSE_GRIP, UP, DOWN };
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

class Dobot: public QObject
{
    Q_OBJECT

private:
    Chessboard *_pChessboard;

    bool connectStatus;
    //typedef struct tagIOPWM {uint8_t address; float frequency; float dutyCycle;}IOPWM;
    IOPWM m_gripperServo;
    const float m_fGripOpened, m_fGripClosed;
    const int m_nMaxPieceHeight; //na jakiej wysokości ma latać ramię by nie przewracać bierek
    const int m_nMaxRemPieceH; //na jaką dodatkową wysokość może się podnieść...
    //...ramię nad bierkami zbitymi
    const int m_nActualPos; //działa jak warunek do sprawdzania czy pozycja ma się zmienić

    float m_fPtpCmd_xActualVal;
    float m_fPtpCmd_yActualVal;
    float m_fPtpCmd_zActualVal;
    float m_fPtpCmd_rActualVal;

    PTPCmd ptpCmd;
    WAITCmd gripperMoveDelay; //komenda mówiąca dobotowi, że ma nic nie robić przez chwilę...
    //...podczas gdy musi się do końca wykonać komenda zamykania chwytaka zanim ruszy dalej

    unsigned long long m_ullCoreQueuedCmdIndex; //aktualny index kolejki w pamięci systemu
    unsigned long long m_ullDobotQueuedCmdIndex; //aktualny id kolejki ruchu wykonywany przez dobota
    //Id nigdy się nie resetuje, bo raczej nie dobiję do 18 triliardów ruchów (unsigned long long)
    unsigned int m_uiQueuedCmdLeftSpace; //ile zostało miejsca w pamięci dobota
    ArmPosForCurrentCmdQueuedIndex m_posIdx; //dane ramienia przypisane do danego indexu dobota
    QList<ArmPosForCurrentCmdQueuedIndex> QueuedCmdIndexList; //kolejka (lista) zapytań do dobota
    ArmPosForCurrentCmdQueuedIndex firstPosId, lastPosId, takenPosId; //TODO: sprawdzić to

    void checkPWM();

public:
    Dobot(Chessboard *pChessboard);

    const int nHome_x = 140;
    const int nHome_y = 0;
    const int nHome_z = 10; //niżej by waliło w szachownicę
    const int nHome_r = 0;

    void refreshBtn();
    void initDobot();
    void onPTPsendBtnClicked();
    void closeEvent(QCloseEvent *);

    void gripperAngle(float fDutyCycle);

    void pieceFromTo(bool bIsPieceMovingTo, int nLetter, int nDigit, SEQUENCE_TYPE Type);
    void gripperOpennedState(bool gripperOpened, SEQUENCE_TYPE Type);
    void wait(int nMs, SEQUENCE_TYPE sequence);
    void addCmdToList(DOBOT_MOVE move, SEQUENCE_TYPE sequence = NONE,
                      float x = ACTUAL_POS, float y = ACTUAL_POS,
                      float z = ACTUAL_POS, float r = ACTUAL_POS);
    void armUpDown(bool isArmGoingUp, bool bIsArmAboveFromSquare, SEQUENCE_TYPE Type);
    void removePiece(int nPieceRowPos, int nPieceColumnPos);
    void writeMoveTypeInConsole(DOBOT_MOVE moveState, SEQUENCE_TYPE sequence = NONE);
    void QueuedIdList();

    //metody dostępowe do pól
    int getMaxPieceHeight() const {return m_nMaxPieceHeight;}
    unsigned long long getCoreQueuedCmdIndex() const {return m_ullCoreQueuedCmdIndex;}
    unsigned long long  getDobotQueuedCmdIndex() const {return m_ullDobotQueuedCmdIndex;}
    unsigned int getQueuedCmdLeftSpace() const {return m_uiQueuedCmdLeftSpace;}

    void setCoreQueuedCmdIndex(unsigned long long ullCoreQueuedCmdIndex)
    {m_ullCoreQueuedCmdIndex = ullCoreQueuedCmdIndex;}
    void setDobotQueuedCmdIndex(unsigned long long ullDobotQueuedCmdIndex)
    {m_ullDobotQueuedCmdIndex = ullDobotQueuedCmdIndex;}
    void setQueuedCmdLeftSpace(unsigned int uiQueuedCmdLeftSpace)
    {m_uiQueuedCmdLeftSpace = uiQueuedCmdLeftSpace;}

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

};

#endif // DOBOT_H
