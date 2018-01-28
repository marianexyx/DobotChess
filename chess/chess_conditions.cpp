#include "chess_conditions.h"

ChessConditions::ChessConditions(Chess* pChess)
{
    _pChess = pChess;
}

bool ChessConditions::isClientRequestCanBeAccepted(QString QStrMsg)
{
    if (requestType(QStrMsg, SHOW_ERRORS) == RT_NONE) return false;
    _request.type = requestType(QStrMsg, SHOW_ERRORS);

    _request.parameter = extractParameter(QStrMsg);

    if (!this->isRequestAppropriateToGameStatus(QStrMsg, SHOW_ERRORS)) return false;
    if (!this->isSenderAppropriate(QStrMsg, SHOW_ERRORS)) return false;
    if (!this->isThereAnySpecialConditionBeenMet(QStrMsg, SHOW_ERRORS)) return false;

    return true;
}

bool ChessConditions::isRequestAppropriateToGameStatus(QString QStrMsg, bool bErrorLog = false)
{


    return true; //todo
}

bool ChessConditions::isSenderAppropriate(QString QStrMsg, bool bErrorLog = false)
{

}

bool ChessConditions::isThereAnySpecialConditionBeenMet(QString QStrMsg, bool bErrorLog = false)
{

}

bool ChessConditions::isRequestParameterInProperFormat()
{
    ChessMovements* pMovements = _pChess->getMovementsPointer();

    switch(_request.type)
    {
    case RT_MOVE:
        if (pMovements->findMoveType(_request.parameter) == ST_NONE ||
                _pMovements->findMoveType(_request.parameter) == ST_PROMOTE_TO_WHAT)
            return false;
        else return true;
    case RT_SIT_ON:
        if (playerTypeFromQStr(_request.parameter) != PT_WHITE &&
                playerTypeFromQStr(_request.parameter) != PT_WHITE &&)
            return false;
        else return true;
    case RT_IM:
        if (_request.parameter.isEmpty())
            return false;
        else return true;
    case RT_PROMOTE_TO:
        ///todo: kontynuj sobie tutej
    default:
    }
}

QString ChessConditions::extractParameter(QString QStrRequest)
{
    if (_request.type != RT_MOVE && _request.type != RT_SIT_ON &&
            _request.type != RT_IM && _request.type != RT_PROMOTE_TO)
    {
        qDebug() << "ERROR: ChessConditions::extractParameter(): this kind of _request.type"
                    " can't contain a parameter:" << requestTypeAsQStr(_request.type);
        return "";
    }

    QString QStrParameter = QStrRequest.mid(requestTypeAsQStr(_request.type).length() + 1);
    return QStrParameter;
}
