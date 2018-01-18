#ifndef DOBOT_H
#define DOBOT_H

#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>
#include <QValidator>
#include <QList>
#include <limits>
#include "DobotDll.h"
#include "DobotType.h"
#include "arduinousb.h"
#include "vars/log.h"
#include "vars/basic_vars.h"
#include "vars/board_axis.h"
#include "vars/dobot_moves.h"
#include "vars/board_data_labels.h"

//todo: oddzielić wszystko co związane z szachami od klasy dobota

struct ArmPosForCurrentCmdQueuedIndex
{
    int64_t index;
    DOBOT_MOVE move;
    float x;
    float y;
    float z;
    float r;
};

struct PtpCmdActualVal //todo: mozna stosować point3d, a r dawać zawsze domyslnie jako 0
{
    float x;
    float y;
    float z;
    float r;
};

struct ServoArduino
{
    int64_t index;
    bool isGripperOpen;
};

class Dobot: public QObject
{
    Q_OBJECT

private:
    ArduinoUsb *_pArduinoUsb;

    short _sGrippersItemID;

    bool connectStatus;
    //typedef struct tagIOPWM {uint8_t address; float frequency; float dutyCycle;}IOPWM;
    IOPWM m_gripperServo;
    const float m_fGripOpened, m_fGripClosed;

    HOMEParams HomeChess;
    Pose pose;
    PtpCmdActualVal m_PtpCmdActualVal, retreatYPlus, retreatYMinus, middleAboveBoard;
    Point3D _lastGivenPoint, _realTimePoint;
    PTPCmd ptpCmd;
    WAITCmd gripperMoveDelay; //komenda mówiąca dobotowi, że ma nic nie robić przez chwilę...
    //...podczas gdy musi się do końca wykonać komenda zamykania chwytaka zanim ruszy dalej

    int64_t m_ullCoreQueuedCmdIndex; //aktualny index kolejki w pamięci systemu
    int64_t m_ullDobotQueuedCmdIndex; //aktualny id kolejki ruchu wykonywany przez dobota
    //Id nigdy się nie resetuje, bo raczej nie dobiję do 18 triliardów ruchów (int64_t)
    int64_t m_ullRetreatIndex; //id na którym ramie ma się odsunąć, by nie zasłaniać obrazu kamerki
    unsigned int m_uiQueuedCmdLeftSpace; //ile zostało miejsca w pamięci dobota
    ArmPosForCurrentCmdQueuedIndex m_posIdx; //dane ramienia przypisane do danego indexu dobota
    QList<ArmPosForCurrentCmdQueuedIndex> QueuedCmdIndexList; //kolejka (lista) zapytań do dobota
    ArmPosForCurrentCmdQueuedIndex firstPosId, lastPosId, takenPosId;

    QList<ServoArduino> arduinoGripperStateList;

    void checkPWM();

public:
    Dobot(ArduinoUsb* pArduinoUsb);

    void refreshBtn();
    void initDobot();
    void onPTPsendBtnClicked();
    void closeEvent(QCloseEvent *);

    void gripperAngle(float fDutyCycle);

    void doMoveSequence(Point3D dest3D, VERTICAL_MOVE VertMove = VM_NONE, double dJump);
    void gripperState(DOBOT_MOVE state);
    void wait(int nMs);
    void addCmdToList(DOBOT_MOVE Move, Point3D point = _lastGivenPoint);
    void armUpDown(DOBOT_MOVE ArmDestination, double dHeight);
    void writeMoveTypeInConsole(DOBOT_MOVE MoveState);
    void QueuedIdList();
    bool bIsMoveInAxisRange(Point3D point);

    //metody dostępowe do pól
    void setItemInGripper(short sGrippersItemID);
    void clearGripper();
    void setCoreQueuedCmdIndex(int64_t ullCoreQueuedCmdIndex)
    { m_ullCoreQueuedCmdIndex = ullCoreQueuedCmdIndex; }
    void setDobotQueuedCmdIndex(int64_t ullDobotQueuedCmdIndex)
    { m_ullDobotQueuedCmdIndex = ullDobotQueuedCmdIndex; }
    void setRetreatIndex(int64_t ullRetreatIndex)
    { m_ullRetreatIndex = ullRetreatIndex; }
    void setQueuedCmdLeftSpace(unsigned int uiQueuedCmdLeftSpace)
    { m_uiQueuedCmdLeftSpace = uiQueuedCmdLeftSpace; }

    bool isGripperEmpty() const { return _sGrippersItemID == 0 ? true : false; }
    short getItemInGripper() const { return _sGrippersItemID; }
    int64_t getCoreQueuedCmdIndex() const { return m_ullCoreQueuedCmdIndex; }
    int64_t getDobotQueuedCmdIndex() const { return m_ullDobotQueuedCmdIndex; }
    int64_t getRetreatIndex() const { return m_ullRetreatIndex; }
    unsigned int getQueuedCmdLeftSpace() const { return m_uiQueuedCmdLeftSpace; }
    float getHomePos(char ch);
    float getmiddleAboveBoardPos(char ch);

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
