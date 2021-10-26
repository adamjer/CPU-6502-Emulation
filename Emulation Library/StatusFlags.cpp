#include "StatusFlags.h"

void StatusFlags::Reset()
{
    C = Z = I = D = B = V = N = 0;
}

void StatusFlags::Set()
{
}

bool StatusFlags::operator== (const StatusFlags& other) const
{
    if (this->C == other.C && this->Z == other.Z
        && this->I == other.I && this->D == other.D
        && this->B == other.B && this->B == other.V
        && this->N == other.N)
        return true;
    return false;
}
