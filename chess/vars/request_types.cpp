#include "request_types.h"

/*static*/ uint64_t clientRequest::counter = 0;

clientRequest::clientRequest():
    clientID(0), type(RT_NONE), param(""), service(false)
{
    ++counter;
}

clientRequest::clientRequest(uint64_t i, REQUEST_TYPE R, QString p, bool s /*= false*/):
    clientID(i), type(R), param(p), service(s)
{
    ++counter;
}

void clientRequest::clear()
{
    type = RT_NONE; param.clear();
}

QString clientRequest::dumpAllData()
{
    return "[request_types.h] (clientRequest) counter: " + QString::number(counter)
            + ", clientID: " + QString::number(clientID)
            + ", type: " + requestTypeAsQStr(type)
            + ", param: " + param
            + ", service: " + (service ? "true" : "false");
}
