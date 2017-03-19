#include "tcpmsgs.h"


//TODO: odpowiedzi na zapytania do TCP są niejednoznaczne. niech tcp oprócz zwracanej odpowiedzi zwraca...
//...też informację o tym jakie zapytanie było wrzucane do TCP, jako że TCP działa tylko na zasadzie...
//... zapytanie-odpowiedź.
TCPMsgs::TCPMsgs()
{
    bUndo = false; //TODO: do usunięcia na przyszłość po zmianie działania TCP
}

void TCPMsgs::queueMsgs(QString msg)
{
    qDebug() << "TCPMsgs::queueMsgs received msg: " << msg;
    TCPMsgsList << msg; //wrzuć do kontenera wiadomość
    if (!TCPMsgsList.isEmpty()) //jeżeli kontener nie jest pusty
    {
        this->doTcpConnect(TCPMsgsList.takeLast()); //to wykonaj najstarszą wiadomość z kontenera.
        //TODO: To działa w założeniu, że kolejny ruch nie wykona się nigdy dopóki nie dostaniemy ...
        //...informacji o tym jaki jest status gry (tzn. ruch się w pełni wykonał). To czy inne...
        //...znikome wyjątki zamiany kolejności zapytań tutaj wystąpią może być niezwykle rzadkie i...
        //...wymaga głębszej analizy "nie na teraz" o tym czy to wystapi i w jakich warunkach.
    }
}

void TCPMsgs::TcpQueueMsg(QString msg)
{
    this->queueMsgs(msg);
}

//rozmowa z tcp. każde 1 polecenie tworzy 1 instancję rozmowy z tcp.
void TCPMsgs::doTcpConnect(QString QStrMsgForChenard)
{
    //TODO: o co chodziło z tym, że mam 2 praktycznie takie same zmienne?
    _QStrMsgForChenard = QStrMsgForChenard;
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64))); //to mi raczej zbędne
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    //emit addTextToTcpConsole("connecting...\n", 't');
    qDebug() << "TCPMsgs: connecting...";

    // this is not blocking call
    socket->connectToHost("localhost", 22222); //docelowo emituje sygnał connected (tcp)

    if(!socket->waitForConnected(5000)) //czkemy na połączenie 5 sekund
    {
        emit addTextToTcpConsole("Error12:" + socket->errorString() + "\n", 't');
    }
}

void TCPMsgs::connected()
{
    //emit addTextToTcpConsole("connected...\n", 't');
    //emit addTextToTcpConsole("msg from websocket: " + _QStrMsgForChenard + "\n", 't');
    qDebug() << "TCPMsgs: connected...";
    qDebug() << "TCPMsgs: msg from websocket" << _QStrMsgForChenard;

    QByteArray QabMsgArrayed;
    QabMsgArrayed.append(_QStrMsgForChenard); //przetworzenie parametru dla funkcji write()
    // send msg to tcp from web. chenard rozumie koniec wiadomości poprzez "\n"
    socket->write(QabMsgArrayed + "\n"); //write wysyła wiadomość (w bajtach) na server przez tcp

    emit addTextToTcpConsole("wrote to TCP: " + QabMsgArrayed + "\n", 't');
}

void TCPMsgs::disconnected()
{
    //TODO: Gdy dwa polecenie na tcp są wykonywane jedne po drugim, to tcp nie zdąży disconnectować...
    //...pierwszej podczas gdy zaczyna od razu wykonywać drugą. Nie widzę na tą chwilę by to w czymś...
    //...przeszkadzało, aczkolwiek wygląda to średnio.
    //emit addTextToTcpConsole("disconnected...\n", 't');
}

void TCPMsgs::bytesWritten(qint64 bytes) //mówi nam ile bajtów wysłaliśmy do tcp
{
    //emit addTextToTcpConsole(QString::number(bytes) + " bytes written...\n", 't');
    qDebug() << "TCPMsgs: " << QString::number(bytes) << " bytes written...";
}

void TCPMsgs::readyRead() //funckja odbierająca odpowiedź z tcp z wcześniej wysłanej wiadmoności
{
    //emit addTextToTcpConsole("reading...\n", 't');
    qDebug() << "TCPMsgs: reading...";

    // read the data from the socket
    QString QStrMsgFromChenard = socket->readAll(); //w zmiennej zapisz odpowiedź z chenard
    emit addTextToTcpConsole("tcp answer: "
                             + QStrMsgFromChenard, 't'); //pokaż ją w consoli tcp. \n dodaje się sam
    qDebug() << "tcp answer: " << QStrMsgFromChenard;
    _QStrMsgForChenard.clear();

    //TODO: dlaczego czasem dostaję 2 rozklejone wiadomości "OK 1" i "\n" zamiast 1 poprawnej "OK 1\n"?
    if (!QStrMsgFromChenard.isEmpty() && QStrMsgFromChenard != "\n") //jeżeli nei jest to końcówka/syf
        emit MsgFromChenard(QStrMsgFromChenard); //core niech odbierze tą wiadomość i zdecyduje do dalej

    if (!TCPMsgsList.isEmpty()) //jeżeli pozostały jeszcze jakieś zapytania do tcp do przetworzenia
    {
        this->doTcpConnect(TCPMsgsList.takeLast()); //to je wykonaj
        //TODO: Nie wiem czy to tu nie przyczyni się kiedyś do jakiegoś błędu. Mółgbym ten warunek...
        //...przesunąć do funkcji w core odpowiadającej wykonywaniu odpowiedzi na "status" z tcp...
        //..., ale to też wymaga analizy.
    }
}
