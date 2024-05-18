#ifndef TAPE_STORAGE_FILETAPE_H
#define TAPE_STORAGE_FILETAPE_H

#include "ITape.h"

#include <array>
#include <fstream>
#include <limits>

namespace YTape
{


class FileTape final: public ITape
{
public:

    explicit FileTape(const std::string& path);
    ~FileTape() override;

public:

    /**
     * ITape interfase implementation.
     */

    bool next() override;
    bool prev() override;
    void rewind() override;

    bool hasNext() const override;
    bool hasPrev() const override;

    bool read(int& value) override;
    void write(int value) override;

    size_t getSize() const noexcept override;
    size_t getPosition() const noexcept override;

private:

    using SizeType = std::fstream::off_type;

    std::fstream fstream_;
    SizeType size_ {};
    SizeType position_ {};
    SizeType begin {};

public:

    static constexpr SizeType bufferSize = 16;
    static constexpr SizeType wordSize = 12;
    static constexpr char delim = '\n';

private:

    void updateCache();

    void load();
    void store();

private:

    /**
     * Cashing variables.
     */

    using Buffer = std::array<int, bufferSize>;
    using BufferPointer = Buffer::iterator;

    Buffer buffer_ {};
    bool modified_ {false};
    SizeType bufferedPos_ {std::numeric_limits<SizeType>::max()};
};


} // namespace YTape

#endif // TAPE_STORAGE_FILETAPE_H
