#include "Tape/FileTape.h"

#include <cassert>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <limits>

using YTape::FileTape;

FileTape::FileTape(const std::string& path)
    : fstream_(path)
    , bufferedPos_(std::numeric_limits<SizeType>::min())
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
    if (diff < 0 || diff >= wordSize)
    {
        store();
        load(diff);
    }
}

void FileTape::load(SizeType diff)
{
    SizeType pos;
    SizeType steps;

    if (diff < 0)
    {
        pos = std::min<SizeType>(0, position_ - bufferSize + 1);
        steps = std::min(bufferSize, position_ - pos);
    }
    else
    {
        pos = position_;
        steps = std::min(bufferSize, size_ - pos);
    }

    fstream_.seekp(begin + pos * wordSize);
    bufferedPos_ = pos;

    for (size_t i = 0; i < steps; ++i)
    {
        char number[wordSize];
        fstream_.getline(number, wordSize, delim);

        auto result = std::from_chars(number, number + wordSize, buffer_[i]);
        if (result.ec != std::errc())
        {
            buffer_[i] = 0;
        }
    }
}

void FileTape::store()
{
    if (!modified_) return;

    fstream_.seekp(begin + bufferedPos_ * wordSize);

    const auto steps = static_cast<size_t>(std::min(bufferSize, size_ - bufferedPos_));
    for (size_t i = 0; i < steps; ++i)
    {
        fstream_ << std::setw(wordSize - 1) << buffer_[i] << "\n";
    }

    modified_ = false;
}
