#ifndef TAPE_STORAGE_FILE_TAPE_FORMATTER_H
#define TAPE_STORAGE_FILE_TAPE_FORMATTER_H

#include "Tape/FileTape.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>

namespace fs = std::filesystem;

namespace YTape
{


class FileTapeFormatter
{
public:

    static fs::path makeEmpty(const std::string& fileName, size_t size);
    static fs::path makeZero(const std::string& fileName, size_t size);
    static fs::path makeRandom(const std::string& fileName, size_t size, int a, int b);
    static fs::path makeFromFile(const std::string& tapeFileName, const std::string& srcFileName);

    template<std::forward_iterator Iter>
    static fs::path makeFromContainer(const std::string& fileName, Iter begin, Iter end)
    {
        fs::path path = makeFile(fileName);
        std::ofstream file(absolute(path));

        size_t size = std::distance(begin, end);
        file << size << "\n";

        file << std::setfill('_') << std::left;

        for (auto it = begin; it != end; ++it)
        {
            file << std::setw(FileTape::wordSize - 1) << *it << "\n";
        }

        file.close();

        return path;
    }

private:

    static fs::path makeFile(const std::string& fileName);
};

} // namespace YTape

#endif // TAPE_STORAGE_FILE_TAPE_FORMATTER_H
