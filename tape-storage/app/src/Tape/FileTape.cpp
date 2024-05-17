#include "Tape/FileTape.h"

#include <cassert>
#include <charconv>
#include <iomanip>
#include <iostream>

using YTape::FileTape;

FileTape::FileTape(const std::string& path)
    : fstream_(path)
{
    if (fstream_.fail())
    {
        std::cerr << "Can't open file: " << path << "\n";
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

bool FileTape::hasNext() const
{
    return position_ != size_ - 1;
}

bool FileTape::hasPrev() const
{
    return position_ != 0;
}

size_t FileTape::getSize() const noexcept
{
    return static_cast<size_t>(size_);
}

void FileTape::updateCache()
{
    const auto diff = position_ - bufferedPos_;
    if (diff < 0 || diff > wordSize)
    {
        store();
        bufferedPos_ = position_;
        load();
    }
}

void FileTape::load()
{
    fstream_.seekp(begin + position_ * wordSize);

    const auto steps = static_cast<size_t>(std::min(bufferSize, size_ - position_));
    for (size_t i = 0; i < steps; ++i)
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

    const auto steps = static_cast<size_t>(std::min(bufferSize, size_ - bufferedPos_));
    for (size_t i = 0; i < steps; ++i)
    {
        fstream_ << std::setw(wordSize - 1) << buffer_[i] << "\n";
    }

    modified_ = false;
}
