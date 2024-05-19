#include "Tape/FileTape.h"

#include <cassert>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <limits>

using YTape::FileTape;

FileTape::FileTape(const std::string& path)
    : fstream_(path)
    , bufferedPos_(-bufferSize)
{
    if (fstream_.fail())
    {
        throw std::invalid_argument("Can't open file: " + path);
    }

    fstream_ >> size_;
    begin = fstream_.tellp();
    begin += 1;
    fstream_ << std::setfill('_') << std::left;
}

FileTape::~FileTape()
{
    store();
}

bool FileTape::next()
{
    if (!hasNext()) return false;

    ++position_;
    return true;
}

bool FileTape::prev()
{
    if (!hasPrev()) return false;

    --position_;
    return true;
}

void FileTape::rewind()
{
    position_ = 0;
}

bool FileTape::hasNext() const
{
    return position_ != size_ - 1;
}

bool FileTape::hasPrev() const
{
    return position_ != 0;
}

bool FileTape::read(int& value)
{
    updateCache();
    assert(position_ - bufferedPos_ >= 0);

    const auto i = static_cast<size_t>(position_ - bufferedPos_);
    value = buffer_[i];

    return true;
}

void FileTape::write(int value)
{
    updateCache();
    assert(position_ - bufferedPos_ >= 0);

    const auto i = static_cast<size_t>(position_ - bufferedPos_);
    buffer_[i] = value;

    modified_ = true;
}

size_t FileTape::getSize() const noexcept
{
    return static_cast<size_t>(size_);
}

size_t FileTape::getPosition() const noexcept
{
    return static_cast<size_t>(position_);
}

void FileTape::updateCache()
{
    const auto diff = position_ - bufferedPos_;
    if (diff < 0 || diff >= cashed)
    {
        store();
        load(diff);
    }
}

void FileTape::load(SizeType diff)
{
    SizeType pos;

    if (diff < 0)
    {
        pos = std::max<SizeType>(0, position_ - bufferSize + 1);
        cashed = std::min(bufferSize, position_ - pos + 1);
    }
    else
    {
        pos = position_;
        cashed = std::min(bufferSize, size_ - pos);
    }

    fstream_.seekp(begin + pos * wordSize);
    bufferedPos_ = pos;

    for (size_t i = 0; i < cashed; ++i)
    {
        char number[wordSize];
        fstream_.getline(number, wordSize, delim);

        std::from_chars(number, number + wordSize, buffer_[i]);
    }
}

void FileTape::store()
{
    if (!modified_) return;

    fstream_.seekp(begin + bufferedPos_ * wordSize);

    for (size_t i = 0; i < cashed; ++i)
    {
        fstream_ << std::setw(wordSize - 1) << buffer_[i] << "\n";
    }

    modified_ = false;
}
