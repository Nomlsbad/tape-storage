#ifndef TAPE_STORAGE_UTILS_H
#define TAPE_STORAGE_UTILS_H

#include "ITape.h"

namespace YTape
{


class Utility
{
public:

    static bool copy(ITape& src, ITape& dst);
};


} // namespace YTape

#endif // TAPE_STORAGE_UTILS_H
