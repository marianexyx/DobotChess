#include "tcpmsgs.h"

TCPMsgs::TCPMsgs()
{

}

//główna funkcja tcp, czyli rozmowa z socketem. wpada tutaj argument do sygnału connected() poprzez
//wyemitowanie go w websocketowej funckji (sygnale) processWebsocketMsg() (?na pewno? czy to jest stary opis?)
void TCPMsgs::doTcpConnect(QString QStrMsgForChenard)
{
    _QStrMsgForChenard = QStrMsgForChenard;
    socket = new QTcpSocket(this);

    //TODO: sprawdzić czy przenieść te connecty do konstruktora
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64))); //to mi raczej zbędne
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    emit addTextToTcpConsole("connecting...\n");

    // this is not blocking call
    socket->connectToHost("localhost", 22222); //docelowo emituje sygnał connected (tcp)

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        emit addTextToTcpConsole("Error12:" + socket->errorString() + "\n");
    }
}

void TCPMsgs::connected()
{
    emit addTextToTcpConsole("connected...\n");
    emit addTextToTcpConsole("msg from websocket: " + _QStrMsgForChenard + "\n");

    QByteArray QabMsgArrayed;
    QabMsgArrayed.append(_QStrMsgForChenard); //przetworzenie parametru dla funkcji write()
    // send msg to tcp from web. chenard rozumie koniec wiadomości poprzez "\n"
    socket->write(QabMsgArrayed + "\n"); //write wysyła wiadomość (w bajtach) na server przez tcp

    emit addTextToTcpConsole("wrote to TCP: " + QabMsgArrayed + "\n");
}

void TCPMsgs::disconnected()
{
    emit addTextToTcpConsole("disconnected...\n\n");
}

void TCPMsgs::bytesWritten(qint64 bytes) //mówi nam ile bajtów wysłaliśmy do tcp
{
    emit addTextToTcpConsole(QString::number(bytes) + " bytes written...\n");
}

void TCPMsgs::readyRead() //funckja odbierająca odpowiedź z tcp z wcześniej wysłanej wiadmoności
{
    emit addTextToTcpConsole("reading...\n");

    // read the data from the socket
    QString QStrMsgFromChenard = socket->readAll(); //w zmiennej zapisz odpowiedź z chenard
    emit addTextToTcpConsole("tcp answer: " + QStrMsgFromChenard); //pokaż ją w consoli tcp. \n dodaje się sam
    qDebug() << "tcp answer: " << QStrMsgFromChenard;
    _QStrMsgForChenard.clear();
    emit MsgFromChenard(QStrMsgFromChenard);
}
