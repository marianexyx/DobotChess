#ifndef VERTICAL_MOVE_H
#define VERTICAL_MOVE_H

#include <QString>

enum VERTICAL_MOVE
{
    VM_NONE,
    VM_GRAB,
    VM_PUT
};

inline QString verticalMoveAsQStr(VERTICAL_MOVE VM)
{
    switch(VM)
    {
    case VM_NONE: return "none"; break;
    case VM_GRAB: return "grab"; break;
    case VM_PUT: return "put"; break;
    default:
        qCritical() << "wrong arg =" << VM;
        return "none";
    }
}

#endif // VERTICAL_MOVE_H
