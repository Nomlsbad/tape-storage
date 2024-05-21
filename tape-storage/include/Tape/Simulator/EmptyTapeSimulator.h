#ifndef TAPE_STORAGE_EMPTY_TAPE_SIMULATOR_H
#define TAPE_STORAGE_EMPTY_TAPE_SIMULATOR_H

#include "ITapeSimulator.h"

namespace YTape
{

class EmptyTapeSimulator: public ITapeSimulator
{
public:

    void shift() override {};
    void rewind(size_t) override {};
    void read() override {};
    void write() override {};
};

}

#endif // TAPE_STORAGE_EMPTY_TAPE_SIMULATOR_H
