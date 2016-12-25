#include "chessboard.h"

Chessboard::Chessboard()
{

}

void Chessboard::findDobotXYZVals(QString QS_msg) //tymczasowaFunkcjaZamieniajacaRuchySzachoweNaWartosciKartezjanskie
{
    qDebug() << "In Chessboard::findDobotXYZVals received: " << QS_msg;
    this->addTextToDobotConsole(QS_msg);
    //to było w starej funkcji serial portu
   /* if(usb_port->isOpen() && QS_msg != "error05")
    {
        addTextToUsbConsole(QS_msg, sender);

        usb_port->write(QS_msg.toStdString().c_str()); // +'$'   -nie działa z tym
        usb_port->waitForBytesWritten(-1); //??? czekaj w nieskonczonosć??? co to jest
    }
    else addTextToUsbConsole("error06", true);*/
}

