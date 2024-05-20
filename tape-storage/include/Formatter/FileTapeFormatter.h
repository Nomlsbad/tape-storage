#ifndef TAPE_STORAGE_FILE_TAPE_FORMATTER_H
#define TAPE_STORAGE_FILE_TAPE_FORMATTER_H

#include "Tape/FileTape.h"

namespace YTape
{

class FileTapeFormatter
{
public:
    static FileTape makeEmpty(const std::string& path, size_t size);
    static FileTape makeZero(const std::string& path, size_t size);
    static FileTape makeRandom(const std::string& path, size_t size, int a, int b);
    static FileTape makeFromFile(const std::string& tapePath, const std::string& filePath);
};

} // namespace YTape

#endif // TAPE_STORAGE_FILE_TAPE_FORMATTER_H
