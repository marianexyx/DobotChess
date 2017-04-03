#include "tcpmsgs.h"
#include <QDataStream>


//TODO: jakaś normalizacja i odpowiednie miejsce na constanty
const int WEBSITE = 1;
const int ARDUINO = 2;

TCPMsgs::TCPMsgs()
{
    m_bWaitingForReadyRead = false;
    m_ullID = 0;
    m_blockSize = 0;
}

void TCPMsgs::queueMsgs(int nSender, QString msg)
{
    qDebug() << "TCPMsgs::queueMsgs received msg: " << msg << ", sender = " << nSender;

    TcpMsgMetadata QStrReceivedData;
    QStrReceivedData.ullTcpID = ++m_ullID;
    QStrReceivedData.nSender = nSender;
    QStrReceivedData.QStrMsgForTcp = msg;
    TCPMsgsList << QStrReceivedData; //wrzuć do kontenera wiadomość
    if (!TCPMsgsList.isEmpty()) //jeżeli kontener nie jest pusty
    {
        if (!m_bWaitingForReadyRead) //zaczekaj z kolejkowaniem kolejnego zapytania do TCP jeżeli...
            //...czekamy aktualnie na przetworzenie jakiejś odpowiedzi z TCP
            this->doTcpConnect(); //to wykonaj najstarszą wiadomość z kontenera.
        //TODO: To działa w założeniu, że kolejny ruch nie wykona się nigdy dopóki nie dostaniemy ...
        //...informacji o tym jaki jest status gry (tzn. ruch się w pełni wykonał). To czy inne...
        //...znikome wyjątki zamiany kolejności zapytań tutaj wystąpią może być niezwykle rzadkie i...
        //...wymaga głębszej analizy "nie na teraz" o tym czy to wystapi i w jakich warunkach.
    }
}

void TCPMsgs::TcpQueueMsg(int nSender, QString msg)
{
    this->queueMsgs(nSender, msg);
}

//rozmowa z tcp. każde 1 polecenie tworzy 1 instancję rozmowy z tcp.
void TCPMsgs::doTcpConnect()
{
    m_blockSize = 0;
    m_bWaitingForReadyRead = true;

    socket = new QTcpSocket(this);

    //TODO: przesunąć sygnały do np. konstruktora? (tam chyba giną po zakończeniu jego inicjalizacji)
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64))); //to mi raczej zbędne
    //connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket, &QIODevice::readyRead, this, &TCPMsgs::readyRead);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, displayError);

    //emit addTextToConsole("connecting...\n", 't');
    qDebug() << "TCPMsgs: connecting...";

    socket->abort(); //pozwoli to zakończyć stare połączenie jeżeli jeszcze nie zostało zerwane...
    //...by można było stworzyć nowe

    // this is not blocking call
    socket->connectToHost("localhost", 22222); //docelowo emituje sygnał connected (tcp)

    if(!socket->waitForConnected(5000)) //czkemy na połączenie 5 sekund
    {
        emit addTextToConsole("Error12:" + socket->errorString() + "\n", 't');
    }
}

void TCPMsgs::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break; //server protocol ends with the server closing the connection
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "ERROR: TCPMsgs: The host was not found. Please check the"
                    "host name and port settings." ;
        emit addTextToConsole("ERROR: TCPMsgs: The host was not found. Please check the"
                              "host name and port settings.\n", 't');
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "ERROR: TCPMsgs: The connection was refused by the peer. Make sure the server is "
                    "running, and check that the host name and port settings are correct.";
        emit addTextToConsole("ERROR: TCPMsgs: The connection was refused by the peer. Make sure the server is "
                              "running, and check that the host name and port settings are correct.", 't');
        break;
    default:
        qDebug() << "ERROR: TCPMsgs: The following error occurred:" << socket->errorString();
        emit addTextToConsole("ERROR: TCPMsgs: The following error occurred: " + socket->errorString(), 't');
    }
}

void TCPMsgs::connected() //udało się nawiązać połączenie z tcp
{
    TcpMsgMetadata QStrData;
    if (!TCPMsgsList.isEmpty()) QStrData = TCPMsgsList.last();
    else
    {
        qDebug() << "ERROR: TCPMsgs::connected(): TCPMsgsList is empty";
        return;
    }

    //emit addTextToConsole("connected...\n", 't');
    //emit addTextToConsole("msg from websocket: " + QStrData.QStrMsgForTcp + "\n", 't');
    qDebug() << "TCPMsgs: connected...";
    qDebug() << "TCPMsgs: parsing msg to chenard:" << QStrData.QStrMsgForTcp;

    QByteArray QabMsgArrayed;
    QabMsgArrayed.append(QStrData.QStrMsgForTcp); //przetworzenie parametru dla funkcji write()
    // send msg to tcp from sender. chenard rozumie koniec wiadomości poprzez "\n"
    socket->write(QabMsgArrayed + "\n"); //write wysyła wiadomość (w bajtach) na server przez tcp

    emit addTextToConsole("wrote to TCP: " + QabMsgArrayed + "\n", 't');
}

void TCPMsgs::disconnected()
{
    //TODO: Gdy dwa polecenie na tcp są wykonywane jedne po drugim, to tcp nie zdąży disconnectować...
    //...pierwszej podczas gdy zaczyna od razu wykonywać drugą. Nie widzę na tą chwilę by to w czymś...
    //...przeszkadzało, aczkolwiek wygląda to średnio.
    //emit addTextToConsole("disconnected...\n", 't');
    qDebug() << "disconnected...";
    //TODO2: Odsyłać wiadomości z Tcp dopiero po disconnectowaniu? tj tutaj wrzucić doTcpConnect z kolejki?
}

void TCPMsgs::bytesWritten(qint64 bytes) //mówi nam ile bajtów wysłaliśmy do tcp
{
    //emit addTextToConsole(QString::number(bytes) + " bytes written...\n", 't');
    qDebug() << "TCPMsgs: " << QString::number(bytes) << " bytes written...";
}

void TCPMsgs::readyRead() //funckja odbierająca odpowiedź z tcp z wcześniej wysłanej wiadmoności
{
    //emit addTextToConsole("reading...\n", 't');
    qDebug() << "TCPMsgs: reading...";
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    qDebug() << "reached1";
    if (m_blockSize == 0)
    {
        /*if (socket->bytesAvailable() < (int)sizeof(quint16))
        {
            qDebug() << "reached2";
            m_bWaitingForReadyRead = false;
            return;
        }
        qDebug() << "reached3";*/
        do
        {
            socket->waitForReadyRead(50);
        }
        while(socket->bytesAvailable() < (int)sizeof(quint64));
        in >> m_blockSize;
    }
    qDebug() << "reached4";
    /*if (socket->bytesAvailable() < m_blockSize) //to trzeba niby sprawdzać, ale z tym nie działa i o co tu chodzi??
    {
        qDebug() << "reached5";
        qDebug() << "socket->bytesAvailable()=" << socket->bytesAvailable() <<
                    ", m_blockSize=" << m_blockSize;
        m_bWaitingForReadyRead = false;
        return;
    }*/
    do
    {
        socket->waitForReadyRead(50);
    }
    while(socket->bytesAvailable() < m_blockSize);
    qDebug() << "reached6";
    QString QStrMsgFromTcp;
    in >> QStrMsgFromTcp;
    qDebug() << "reached7";
    /*QString QStrMsgFromTcp = socket->readAll(); //w zmiennej zapisz odpowiedź z chenard
    do
    {
        QStrMsgFromTcp = socket->readAll(); //ominięcie problemu pustych wiadomości, którego nie mogę rozwiązać
    }
    while (!QStrMsgFromTcp.isEmpty() && QStrMsgFromTcp != "\n");*/

    //TODO: dlaczego czasem dostaję 2 rozklejone wiadomości "OK 1" i "\n" zamiast 1 poprawnej "OK 1\n"?
    if (!QStrMsgFromTcp.isEmpty() && QStrMsgFromTcp != "\n") //jeżeli nie jest to końcówka/syf
    {
        emit addTextToConsole("tcp answer: " + QStrMsgFromTcp, 't');

        TcpMsgMetadata QStrData;
        if (!TCPMsgsList.isEmpty())
            QStrData = TCPMsgsList.takeLast();
        else
        {
            qDebug() << "ERROR: TCPMsgs::readyRead(): TCPMsgsList is empty";
            return;
        }

        if (QStrData.nSender == WEBSITE) //inna klasa powinna o tym decydowac?
        {
            qDebug() << "ID =" << QStrData.ullTcpID << ", sender =" << QStrData.nSender <<
                        ", msgForTcp =" << QStrData.QStrMsgForTcp << ", msgFromTcp =" << QStrMsgFromTcp;
            //core niech odbierze tą wiadomość i zdecyduje do dalej
            emit this->msgFromTcpToWeb(QStrData.QStrMsgForTcp, QStrMsgFromTcp);
            //m_bWaitingForReadyRead = false;
        }
        else if (QStrData.nSender == ARDUINO)
        {
            qDebug() << "ID =" << QStrData.ullTcpID << ", sender =" << QStrData.nSender <<
                        ", msgForTcp =" << QStrData.QStrMsgForTcp << ", msgFromTcp =" << QStrMsgFromTcp;
            emit this->msgFromTcpToArd(QStrData.QStrMsgForTcp, QStrMsgFromTcp);
            //m_bWaitingForReadyRead = false;
        }
        else qDebug() << "ERROR: TCPMsgs::readyRead(): unknown QStrData.nSender value =" <<
                         QStrData.nSender;
    }
    else if (QStrMsgFromTcp.isEmpty())
        qDebug() << "ERROR: TCPMsgs::readyRead() received empty msg.";
    else if (QStrMsgFromTcp == "\n")
        qDebug() << "ERROR: TCPMsgs::readyRead() received '\\n' msg.";
    else qDebug() << "ERROR: TCPMsgs::readyRead() received:" << QStrMsgFromTcp;

    m_bWaitingForReadyRead = false;

    if (!TCPMsgsList.isEmpty()) //jeżeli pozostały jeszcze jakieś zapytania do tcp do przetworzenia
    {
        this->doTcpConnect(); //to je wykonaj
        //TODO: Nie wiem czy to tu nie przyczyni się kiedyś do jakiegoś błędu. Mółgbym ten warunek...
        //...przesunąć do funkcji w core odpowiadającej wykonywaniu odpowiedzi na "status" z tcp...
        //..., ale to też wymaga analizy.
    }
}
