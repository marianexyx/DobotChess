#include "dobot.h"

Dobot::Dobot()
{
    connectStatus = false;
}

//periodic task Timer
void Dobot::setPeriodicTaskTimer()
{
    QTimer *periodicTaskTimer = new QTimer(this);
    periodicTaskTimer->setObjectName("periodicTaskTimer");
    connect(periodicTaskTimer, SIGNAL(timeout()), this, SLOT(onPeriodicTaskTimer()));
    periodicTaskTimer->setSingleShot(true);
    periodicTaskTimer->start(5);
}

//getPose Timer
void Dobot::getPoseTimer()
{
    QTimer *getPoseTimer = new QTimer(this);
    getPoseTimer->setObjectName("getPoseTimer");
    connect(getPoseTimer, SIGNAL(timeout()), this, SLOT(onGetPoseTimer()));
    getPoseTimer->setSingleShot(true);
    getPoseTimer->start(200);
}

void Dobot::onPeriodicTaskTimer()
{
    PeriodicTask();
    QTimer *periodicTaskTimer = findChild<QTimer *>("periodicTaskTimer");
    periodicTaskTimer->start();
}

void Dobot::onGetPoseTimer()
{
    QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");

    emit JointLabelText(QString::number(thread.pose->jointAngle[0]), 1);
    emit JointLabelText(QString::number(thread.pose->jointAngle[1]), 2);
    emit JointLabelText(QString::number(thread.pose->jointAngle[2]), 3);
    emit JointLabelText(QString::number(thread.pose->jointAngle[3]), 4);
    /*ui->joint1Label->setText(QString::number((thread.pose->jointAngle[0])));
    ui->joint2Label->setText(QString::number((thread.pose->jointAngle[1])));
    ui->joint3Label->setText(QString::number((thread.pose->jointAngle[2])));
    ui->joint4Label->setText(QString::number((thread.pose->jointAngle[3])));*/

    emit AxisLabelText(QString::number(thread.pose->x), 'x');
    emit AxisLabelText(QString::number(thread.pose->y), 'y');
    emit AxisLabelText(QString::number(thread.pose->z), 'z');
    emit AxisLabelText(QString::number(thread.pose->r), 'r');
    /*ui->xLabel->setText(QString::number(thread.pose->x));
    ui->yLabel->setText(QString::number(thread.pose->y));
    ui->zLabel->setText(QString::number(thread.pose->z));
    ui->rLabel->setText(QString::number(thread.pose->r));*/

    getPoseTimer->start();
}

void Dobot::onConnectDobot()
{
    //connect dobot
    if (!connectStatus) {
        if (ConnectDobot(0, 115200) != DobotConnect_NoError) emit DobotErrorMsgBox();
        connectStatus = true;
        refreshBtn();
        initDobot();

        //start subThread to getpose
        thread.start();

        qDebug() << "connect success!!!";
    }else {
        connectStatus = false;
        refreshBtn();
        DisconnectDobot();
    }
}

void Dobot::refreshBtn()
{
    if(connectStatus) emit ConnectButton(false);
    /*{
        ui->connectBtn->setText(tr("Disconnect"));

        ui->sendBtn->setEnabled(true);
        ui->xPTPEdit->setEnabled(true);
        ui->yPTPEdit->setEnabled(true);
        ui->zPTPEdit->setEnabled(true);
        ui->rPTPEdit->setEnabled(true);
    }*/
    else emit ConnectButton(true);
    /*{
        ui->connectBtn->setText(tr("Connect"));

        ui->sendBtn->setEnabled(false);
        ui->xPTPEdit->setEnabled(false);
        ui->yPTPEdit->setEnabled(false);
        ui->zPTPEdit->setEnabled(false);
        ui->rPTPEdit->setEnabled(false);
    }*/
}

void Dobot::initDobot()
{
        //Command timeout
        SetCmdTimeout(3000);
        //clear old commands and set the queued command running
        SetQueuedCmdClear();
        SetQueuedCmdStartExec();

        char deviceSN[64];
        GetDeviceSN(deviceSN, sizeof(deviceSN));
        //ui->deviceSNLabel->setText(deviceSN);

        char deviceName[64];
        GetDeviceName(deviceName, sizeof(deviceName));
        //ui->DeviceNameLabel->setText(deviceName);

        uint8_t majorVersion, minorVersion, revision;
        GetDeviceVersion(&majorVersion, &minorVersion, &revision);
        /*ui->DeviceInfoLabel->setText(QString::number(majorVersion) + "." +
                                     QString::number(minorVersion) + "." +
                                     QString::number(revision));*/

        emit deviceLabels(deviceSN, deviceName, QString::number(majorVersion) + "." +
                          QString::number(minorVersion) + "." +
                          QString::number(revision));

        //set the end effector parameters
        EndEffectorParams endEffectorParams;
        memset(&endEffectorParams, 0, sizeof(endEffectorParams));
        endEffectorParams.xBias = 59.7f;
        SetEndEffectorParams(&endEffectorParams, false, NULL);

        JOGJointParams jogJointParams;
        for (int i = 0; i < 4; i++)
        {
            jogJointParams.velocity[i] = 100;
            jogJointParams.acceleration[i] = 100;
        }
        SetJOGJointParams(&jogJointParams, false, NULL);

        JOGCoordinateParams jogCoordinateParams;
        for (int i = 0; i < 4; i++)
        {
            jogCoordinateParams.velocity[i] = 100;
            jogCoordinateParams.acceleration[i] = 100;
        }
        SetJOGCoordinateParams(&jogCoordinateParams, false, NULL);

        JOGCommonParams jogCommonParams;
        jogCommonParams.velocityRatio = 50;
        jogCommonParams.accelerationRatio = 50;
        SetJOGCommonParams(&jogCommonParams, false, NULL);

        PTPJointParams ptpJointParams;
        for (int i = 0; i < 4; i++)
        {
            ptpJointParams.velocity[i] = 100;
            ptpJointParams.acceleration[i] = 100;
        }
        SetPTPJointParams(&ptpJointParams, false, NULL);

        PTPCoordinateParams ptpCoordinateParams;
        ptpCoordinateParams.xyzVelocity = 100;
        ptpCoordinateParams.xyzAcceleration = 100;
        ptpCoordinateParams.rVelocity = 100;
        ptpCoordinateParams.rAcceleration = 100;
        SetPTPCoordinateParams(&ptpCoordinateParams, false, NULL);

        PTPJumpParams ptpJumpParams;
        ptpJumpParams.jumpHeight = 20;
        ptpJumpParams.zLimit = 150;
        SetPTPJumpParams(&ptpJumpParams, false, NULL);
}

void Dobot::PTPvalues(int nPtpCmd_xVal, int nPtpCmd_yVal, int nPtpCmd_zVal, int nPtpCmd_rVal)
{
    PTPCmd ptpCmd;
    ptpCmd.ptpMode = PTPMOVLXYZMode;
    ptpCmd.x = nPtpCmd_xVal;
    ptpCmd.y = nPtpCmd_yVal;
    ptpCmd.z = nPtpCmd_zVal;
    ptpCmd.r = nPtpCmd_rVal;

    SetPTPCmd(&ptpCmd, false, NULL);
}

void Dobot::closeEvent(QCloseEvent *)
{
    thread.quit();
}

void moveToPosition(QString QsMoveToPosition)
{
    if (QsMoveToPosition == ""){}; //TODO: rozpatrywać to co wpada
    //TODO: wyciaganie pozycji z chessboard
    //TODO: wrzucanie pozycji XYZ na dobota
}
