#ifndef DOBOT_H
#define DOBOT_H

#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>
#include <QValidator>
#include "DobotDll.h"
#include "DobotType.h"
#include "chessboard.h"

class Dobot: public QObject
{
    Q_OBJECT

private:
    Chessboard *_pChessboard;

    bool connectStatus;
    //typedef struct tagIOPWM {uint8_t address; float frequency; float dutyCycle;}IOPWM;
    IOPWM m_gripperServo1;
    IOPWM m_gripperServo2;
    int m_nMaxPieceHeight; //na jakiej wysokości ma latać ramię by nie przewracać bierek
    int m_nMaxRemovedPieceHeight; //na jaką dodatkową wysokość może się podnieść...
    //...ramię nad bierkami zbitymi

public:
    Dobot(Chessboard *pChessboard);

    void refreshBtn();
    void initDobot();
    void onPTPsendBtnClicked();
    void closeEvent(QCloseEvent *);

    void PTPvalues(float fPtpCmd_xVal, float fPtpCmd_yVal, float fPtpCmd_zVal, float fPtpCmd_rVal);
    void gripperAngle(float fDutyCycle1, float fDutyCycle2);

    void pieceFromTo(bool bIsPieceMovingTo, int nLetter, int nDigit, char chMoveType);
    void gripperOpennedState(bool gripperOpened, char chMovementType);
    void armUpDown(bool isArmGoingUp, bool bIsArmAboveFromSquare, char chMovementType);
    void removePiece(int nPieceRowPos, int nPieceColumnPos);

public slots:
    void onConnectDobot();
    void onPeriodicTaskTimer();
    void onGetPoseTimer();

signals:
    //sygnały do używania GUI MainWindow
    void addTextToDobotConsole(QString QS_msg); //dodawanie komunikatu do konsoli dobota
    void JointLabelText(QString QSLabelText, short sJoint);
    void AxisLabelText(QString QSAxisLabelText, char chAxis);
    void RefreshDobotButtonsStates(bool bDobotButtonsStates);
    void deviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion);
    void DobotErrorMsgBox();
};

#endif // DOBOT_H
