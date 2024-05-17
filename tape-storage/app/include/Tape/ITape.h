#ifndef TAPE_STORAGE_ITAPE_H
#define TAPE_STORAGE_ITAPE_H

#include <cstddef>

namespace YTape
{


class ITape
{
public:

    virtual ~ITape() = default;

public:

    virtual bool next() = 0;
    virtual bool prev() = 0;
    virtual void rewind() = 0;

    virtual bool hasNext() const = 0;
    virtual bool hasPrev() const = 0;

    virtual bool read(int& value) = 0;
    virtual void write(int value) = 0;

    virtual size_t getSize() const = 0;
};


} // namespace YTape


#endif // TAPE_STORAGE_ITAPE_H
