#include "chess_conditions.h"

ChessConditions::ChessConditions(Clients* pClientsList, ChessStatus* pStatus)
{
    m_pClientsList = pClientsList;
    m_pStatus = pStatus;
}

bool ChessConditions::isClientRequestCanBeAccepted(QString QStrMsg, Client* pSender,
                                                   GAME_STATUS GS, REJECTED_REQUEST_REACTION& RRR)
{
    clientRequest request;

    if (requestTypeFromQStr(QStrMsg, SHOW_ERRORS) == RT_NONE) return false;

    request.type = requestTypeFromQStr(QStrMsg, SHOW_ERRORS);
    request.param = this->extractParamIfTypeIsInProperFormat(request.type, QStrMsg, RRR);

    if (!this->isRequestAppropriateToGameStatus(request.type, GS)) return false;
    if (!this->isSenderAppropriate(pSender, request.type)) return false;
    if (!this->isThereAnySpecialConditionBeenMet(pSender, request, RRR)) return false;

    return true;
}

QString ChessConditions::extractParamIfTypeIsInProperFormat(REQUEST_TYPE Type, QString QStrMsg,
                                                            REJECTED_REQUEST_REACTION& RRR)
{
    if (this->isRequestAParameterType(Type))
    {
        QString QStrParam = QStrMsg.mid(requestTypeAsQStr(Type).length() + 1);

        clientRequest request;
        request.type = Type;
        request.param = QStrParam;
        if (this->isRequestParameterInProperFormat(request))
            return QStrParam;
        else
        {
            RRR = RRR_REMOVE_AND_REFRESH_CLIENT;
            return "";
        }
    }
    else return "";
}

bool ChessConditions::isRequestAParameterType(REQUEST_TYPE Type, bool bErrorLog /*= false*/)
{
    switch(Type)
    {
    case RT_MOVE:
    case RT_SIT_ON:
    case RT_IM:
    case RT_PROMOTE_TO:
        return true;
    default:
        if (bErrorLog)
            qCritical() << "unknown REQUEST_TYPE:" << requestTypeAsQStr(Type);

        return false;
    }
}

bool ChessConditions::isRequestParameterInProperFormat(clientRequest request)
{
    bool bReturn = false;

    switch(request.type)
    {
    case RT_MOVE:
        if (ChessStatus::isMoveInProperFormat(request.param, SHOW_ERRORS))
            bReturn = true;
        else bReturn = false;
        break;
    case RT_SIT_ON:
        if (playerTypeFromQStr(request.param) != PT_WHITE &&
                playerTypeFromQStr(request.param) != PT_BLACK)
            bReturn = false;
        else bReturn = true;
        break;
    case RT_IM:
    {
        QRegExp reg("\\d+"); //a digit (\d), one or more times (+)
        QString QStrSqlId = request.param.left(request.param.indexOf("&"));
        QString QStrHash = request.param.mid(request.param.indexOf("&")+1);
        if (!request.param.isEmpty() && request.param.contains("&")
                && reg.exactMatch(QStrSqlId) && QStrHash.length() == 20)
            bReturn = true;
        else bReturn = false;
        break;
    }
    case RT_PROMOTE_TO:
        if (ChessStatus::isSignProperPromotionType(request.param))
            bReturn = true;
        else bReturn = false;
        break;
    default:
        qCritical() << "unknown request.type:" << QString::number(request.type);
    }

    if (!bReturn)
        qWarning() << "requirements not met (== false) in:" << requestTypeAsQStr(request.type);

    return bReturn;
}


bool ChessConditions::isRequestAppropriateToGameStatus(REQUEST_TYPE Type, GAME_STATUS Status)
{
    switch(Type)
    {
    case RT_NONE:
        qCritical() << "Type == RT_NONE";
        return false;
    case RT_NEW_GAME: return Status == GS_TURN_NONE_WAITING_FOR_START_CONFIRMS ? true : false;
    case RT_MOVE: return whoseTurnFromGameStatus(Status) != NO_TURN ? true : false;
    case RT_SIT_ON: return Status == GS_TURN_NONE_WAITING_FOR_PLAYERS ? true : false;
    case RT_PROMOTE_TO:
        if (Status == GS_TURN_WHITE_PROMOTE || Status == GS_TURN_BLACK_PROMOTE) return true;
        else return false;
    case RT_QUEUE_ME:
        if (whoseTurnFromGameStatus(Status) != NO_TURN ||
                Status == GS_TURN_NONE_WAITING_FOR_START_CONFIRMS) return true;
        else return false;
    default: return true;
    }
}

bool ChessConditions::isSenderAppropriate(Client* pSender, REQUEST_TYPE Type)
{
    if (!m_pClientsList->isClientInList(*pSender, SHOW_ERRORS)) return false;

    bool bLogged = pSender->sqlID() > 0 ? true : false;
    bool bSittingOnChair = m_pClientsList->isClientAPlayer(*pSender);
    bool bInQueue = m_pClientsList->isClientInQueue(*pSender);

    bool bSuccess = false;
    switch(Type)
    {
    case RT_NONE:
        qCritical() << "Type == RT_NONE";
        bSuccess = false;
        break;
    case RT_NEW_GAME:
    case RT_MOVE:
    case RT_STAND_UP:
    case RT_PROMOTE_TO:
        if (bSittingOnChair && !bInQueue) bSuccess = true;
        else bSuccess = false;
        break;
    case RT_SIT_ON:
    case RT_QUEUE_ME:
        if (bLogged && !bSittingOnChair && !bInQueue) bSuccess = true;
        else bSuccess = false;
        break;
    case RT_LEAVE_QUEUE:
        if (bLogged && !bSittingOnChair && bInQueue) bSuccess = true;
        else bSuccess = false;
        break;
    case RT_GET_TABLE_DATA:
    case RT_IM:
    case RT_CLIENT_LEFT:
        bSuccess = true;
        break;
    default:
        qCritical() << "unknown REQUEST_TYPE =" << QString::number(Type);
        bSuccess = false;
    }

    if (!bSuccess)
        qWarning() << "Type = " << requestTypeAsQStr(Type) << ". bLogged ="
                    << QString::number(bLogged) << ", bSittingOnChair ="
                    << QString::number(bSittingOnChair) << ", bInQueue ="
                    << QString::number(bInQueue);

    return bSuccess;
}

bool ChessConditions::isThereAnySpecialConditionBeenMet(Client* pSender, clientRequest request,
                                                        REJECTED_REQUEST_REACTION& RRR)
{
    bool bSuccess = false;

    switch(request.type)
    {
    case RT_NONE:
        qCritical() << "Type = RT_NONE";
        bSuccess = false;
        break;
    case RT_MOVE:
    case RT_PROMOTE_TO:
        if ((pSender->type() == PT_WHITE && m_pStatus->getWhoseTurn() == WHITE_TURN) ||
                (pSender->type() == PT_BLACK && m_pStatus->getWhoseTurn() == BLACK_TURN))
            bSuccess = true;
        else bSuccess = false;
        break;
    case RT_SIT_ON:
    {
        PLAYER_TYPE PlayerChair = playerTypeFromQStr(request.param);
        if (m_pClientsList->isPlayerChairEmpty(PlayerChair) &&
                !m_pClientsList->isClientAPlayer(*pSender))
            bSuccess = true;
        else bSuccess = false;
        break;
    }
    case RT_IM:
    {
        if (Sql::isClientHashOk(request.param))
        {
            uint unSqlId = request.param.left(request.param.indexOf("&")).toInt();
            if (pSender->sqlID() == 0 || pSender->sqlID() == unSqlId)
                bSuccess = true;
            else
            {
                RRR = RRR_REMOVE_AND_REFRESH_CLIENT;
                bSuccess = false;
            }
        }
        else bSuccess = false;
        break;
    }
    case RT_QUEUE_ME:
        if (m_pClientsList->isWholeGameTableOccupied())
            bSuccess = true;
        else bSuccess = false;
        break;
    case RT_LEAVE_QUEUE:
        if (m_pClientsList->isClientInQueue(*pSender))
            bSuccess = true;
        else bSuccess = false;
        break;
    case RT_CLIENT_LEFT:
        if (m_pClientsList->isClientInList(*pSender, SHOW_ERRORS))
            bSuccess = true;
        else bSuccess = false;
        break;
    default: bSuccess = true;
    }

    if (!bSuccess)
        qWarning() << "bSuccess = false";

    return bSuccess;
}
