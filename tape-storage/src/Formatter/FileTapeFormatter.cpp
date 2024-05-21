#include "Formatter/FileTapeFormatter.h"

#include "Tape/FileTape.h"

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <random>
#include <vector>

using YTape::FileTape;
using YTape::FileTapeFormatter;

fs::path FileTapeFormatter::makeEmpty(const std::string& fileName, size_t size)
{
    fs::path path = makeFile(fileName);
    std::ofstream file(absolute(path));

    std::string line(FileTape::wordSize, '_');
    line.back() = FileTape::delim;

    file << size << "\n";
    std::fill_n(std::ostream_iterator<std::string>(file), size, line);
    file.close();

    return path;
}

fs::path FileTapeFormatter::makeZero(const std::string& fileName, size_t size)
{
    fs::path path = makeFile(fileName);
    std::ofstream file(absolute(path));

    std::string line(FileTape::wordSize, '_');
    line.front() = 0;
    line.back() = FileTape::delim;

    file << size << "\n";
    std::fill_n(std::ostream_iterator<std::string>(file), size, line);
    file.close();

    return fileName;
}

fs::path FileTapeFormatter::makeRandom(const std::string& fileName, size_t size, int a, int b)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(a, b);

    fs::path path = makeFile(fileName);
    std::ofstream file(absolute(path));

    file << size << "\n";
    file << std::setfill('_') << std::left;

    for (size_t i = 0; i < size; ++i)
    {
        file << std::setw(FileTape::wordSize - 1) << dis(gen) << "\n";
    }
    file.close();

    return path;
}

fs::path FileTapeFormatter::makeFromFile(const std::string& tapeFileName, const std::string& srcFileName)
{
    if (tapeFileName == srcFileName)
    {
        throw std::invalid_argument("tapePath and filePath must be different!");
    }
    if (!fs::exists(srcFileName))
    {
        throw std::invalid_argument("file doesn't exist!");
    }

    fs::path path = makeFile(tapeFileName);

    std::ifstream srcFile(absolute(fs::path(srcFileName)));
    std::ofstream tapeFile(absolute(path));

    const size_t maxDigitsInSize = 20;
    tapeFile << std::string(maxDigitsInSize, '_') << "\n";

    size_t size = 0;
    std::stringstream stringstream;
    stringstream << std::setfill('_') << std::left;

    std::transform(std::istream_iterator<int>(srcFile), std::istream_iterator<int>(),
                   std::ostream_iterator<std::string>(tapeFile),
                   [&stringstream, &size](int number)
                   {
                       ++size;
                       stringstream << std::setw(FileTape::wordSize - 1) << number << "\n";
                       return std::move(stringstream).str();
                   });

    tapeFile.seekp(0);
    tapeFile << std::setfill('_') << std::left << std::setw(maxDigitsInSize) << size << "\n";
    tapeFile.close();

    return path;
}

fs::path FileTapeFormatter::makeFile(const std::string& path)
{
    fs::path fPath(path);
    if (!fs::exists(fPath))
    {
        fs::path directory = fPath.parent_path();
        if (!directory.empty() && !fs::exists(directory))
        {
            fs::create_directories(directory);
        }
    }
    return fPath;
}