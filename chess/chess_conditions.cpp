#include "chess_conditions.h"

ChessConditions::ChessConditions(Clients* pClientsList, ChessStatus* pStatus)
{
    m_pClientsList = pClientsList;
    m_pStatus = pStatus;
}

/*static*/ bool ChessConditions::isRequestMsgInProperFormat(QString QStrMsg)
{
    clientRequest request;
    request.type = requestTypeFromQStr(QStrMsg, SHOW_ERRORS);
    if (request.type == RT_NONE)
        return false;
    else
    {
        if (ChessConditions::isRequestAParameterType(request.type))
        {
            request.param = QStrMsg.mid(requestTypeAsQStr(request.type).length() + 1);
            if (ChessConditions::isRequestParameterInProperFormat(request))
                return true;
            else return true;
        }
        else return true;
    }
}

/*static*/ bool ChessConditions::isRequestAParameterType(REQUEST_TYPE Type,
                                                         bool bErrorLog /*= false*/)
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

/*static*/ bool ChessConditions::isRequestParameterInProperFormat(clientRequest request)
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

/*static*/ QString ChessConditions::extractParamIfExists(QString QStrMsg)
{
    clientRequest request;
    request.type = requestTypeFromQStr(QStrMsg, SHOW_ERRORS);
    if (request.type == RT_NONE)
    {
        qCritical() << "Type == RT_NONE";
        return "";
    }

    if (ChessConditions::isRequestAParameterType(request.type))
    {
        request.param = QStrMsg.mid(requestTypeAsQStr(request.type).length() + 1);
        if (ChessConditions::isRequestParameterInProperFormat(request))
            return request.param;
        else return ""; //returning REJECTED_REQUEST_REACTION isn't needed here
    }
    else return "";
}

REJECTED_REQUEST_REACTION ChessConditions::isClientRequestCanBeExecuted(clientRequest request,
                                                                        GAME_STATUS GS)
{
    //this method assumes that request param is already in proper format

    //future: im passing locally created objects, not pointers. app works only because...
    //...i'm using passed ID everywhere. 2 options: use only ID everywhere, or only pointers
    Client sender = m_pClientsList->getClient(request.clientID);
    if (!this->isRequestAppropriateToGameStatus(request.type, GS)) return RRR_RESEND_TABLE_DATA;
    if (!this->isSenderAppropriate(&sender, request.type)) return RRR_RESEND_TABLE_DATA;
    if (this->isThereAnySpecialConditionBeenMet(&sender, request) != RRR_NONE)
        return this->isThereAnySpecialConditionBeenMet(&sender, request);

    return RRR_NONE;
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
    case RT_GET_TABLE_DATA: //redundant code- let it be here for safety
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

REJECTED_REQUEST_REACTION ChessConditions::isThereAnySpecialConditionBeenMet(Client* pSender,
                                                                             clientRequest request)
{
    REJECTED_REQUEST_REACTION RRR = RRR_NONE;

    switch(request.type)
    {
    case RT_NONE:
        qCritical() << "Type == RT_NONE";
        RRR = RRR_REMOVE_AND_REFRESH_CLIENT;
        break;
    case RT_MOVE:
    case RT_PROMOTE_TO:
        if (!(pSender->type() == PT_WHITE && m_pStatus->getWhoseTurn() == WHITE_TURN) &&
                !(pSender->type() == PT_BLACK && m_pStatus->getWhoseTurn() == BLACK_TURN))
            RRR = RRR_RESEND_TABLE_DATA;
        break;
    case RT_SIT_ON:
    {
        PLAYER_TYPE PlayerChair = playerTypeFromQStr(request.param);
        if (!m_pClientsList->isPlayerChairEmpty(PlayerChair) ||
                m_pClientsList->isClientAPlayer(*pSender))
            RRR = RRR_RESEND_TABLE_DATA;
        break;
    }
    case RT_IM:
    {
        if (Sql::isClientHashOk(request.param))
        {
            //double login is proper request. it's served in request execusion
            uint unSqlId = request.param.left(request.param.indexOf("&")).toInt();
            if (pSender->sqlID() != 0 && pSender->sqlID() != unSqlId)
                RRR = RRR_REMOVE_AND_REFRESH_CLIENT;
        }
        else RRR = RRR_REMOVE_AND_REFRESH_CLIENT;
        break;
    }
    case RT_QUEUE_ME:
        if (!m_pClientsList->isWholeGameTableOccupied())
            RRR = RRR_RESEND_TABLE_DATA;
        break;
    case RT_LEAVE_QUEUE:
        if (!m_pClientsList->isClientInQueue(*pSender))
            RRR = RRR_RESEND_TABLE_DATA;
        break;
    case RT_CLIENT_LEFT:
        if (!m_pClientsList->isClientInList(*pSender, SHOW_ERRORS))
            RRR = RRR_RESEND_TABLE_DATA;
        break;
    default: RRR = RRR_NONE;
    }

    if (RRR != RRR_NONE)
        qWarning() << "RRR != RRR_NONE. it's =" << rejectedRequestReactionAsQStr(RRR);

    return RRR;
}
