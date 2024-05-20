#include "Formatter/FileTapeFormatter.h"

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <random>
#include <vector>

using YTape::FileTape;
using YTape::FileTapeFormatter;

FileTape FileTapeFormatter::makeEmpty(const std::string& path, size_t size)
{
    std::fstream file(path);

    std::string line(FileTape::wordSize, '_');
    line.back() = FileTape::delim;

    file << size << "\n";
    std::fill_n(std::ostream_iterator<std::string>(file), size, line);
    file.close();

    return FileTape(path);
}

FileTape FileTapeFormatter::makeZero(const std::string& path, size_t size)
{
    std::fstream file(path);

    std::string line(FileTape::wordSize, '_');
    line.front() = 0;
    line.back() = FileTape::delim;

    file << size << "\n";
    std::fill_n(std::ostream_iterator<std::string>(file), size, line);
    file.close();

    return FileTape(path);
}

FileTape FileTapeFormatter::makeRandom(const std::string& path, size_t size, int a, int b)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(a, b);

    std::fstream file(path);
    file << std::setfill('_') << std::left;

    for (size_t i = 0; i < size; ++i)
    {
        file << std::setw(FileTape::wordSize - 1) << dis(gen) << "\n";
    }
    file.close();

    return FileTape(path);
}

FileTape FileTapeFormatter::makeFromFile(const std::string& tapePath, const std::string& filePath)
{
    if (tapePath == filePath)
    {
        throw std::invalid_argument("tapePath and filePath must be different!");
    }

    std::fstream srcFile(filePath);
    std::fstream tapeFile(tapePath);

    std::stringstream stringstream;
    stringstream << std::setfill('_') << std::left;

    std::transform(std::istream_iterator<int>(srcFile), std::istream_iterator<int>(),
                   std::ostream_iterator<std::string>(tapeFile),
                   [&stringstream](int number)
                   {
                       stringstream << std::setw(FileTape::wordSize - 1) << number << "\n";
                       return std::move(stringstream).str();
                   });
    tapeFile.close();

    return FileTape(tapePath);
}