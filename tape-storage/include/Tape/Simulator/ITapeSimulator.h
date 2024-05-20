#ifndef TAPE_STORAGE_ITAPE_SIMULATOR_H
#define TAPE_STORAGE_ITAPE_SIMULATOR_H

#include <cstddef>

namespace YTape
{

class ITapeSimulator
{
public:

    virtual void shift() = 0;
    virtual void rewind(size_t n) = 0;
    virtual void read() = 0;
    virtual void write() = 0;

    virtual ~ITapeSimulator() = default;
};

} // namespace YTape

#endif // TAPE_STORAGE_ITAPE_SIMULATOR_H
