#include "Tape/Utils.h"

#include <Tape/ITape.h>

using YTape::Utility;

bool Utility::copy(ITape& src, ITape& dst)
{
    if (src.getSize() != dst.getSize()) return false;
    if (&src == &dst) return true;

    const size_t startPosition = src.getPosition();

    dst.rewind();
    while (dst.getPosition() != startPosition)
    {
        dst.next();
    }

    do
    {
        int value = 0;
        src.read(value);
        dst.write(value);

        if (src.hasNext())
        {
            src.next();
            dst.next();
        }
        else
        {
            src.rewind();
            dst.rewind();
        }

    } while (src.getPosition() != startPosition);

    return true;
}
