#ifndef TAPE_STORAGE_FILE_TAPE_FORMATTER_H
#define TAPE_STORAGE_FILE_TAPE_FORMATTER_H

#include <filesystem>

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

private:

    static fs::path makeFile(const std::string& fileName);
};

} // namespace YTape

#endif // TAPE_STORAGE_FILE_TAPE_FORMATTER_H
