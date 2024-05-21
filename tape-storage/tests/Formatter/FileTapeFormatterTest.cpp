#include "FileTapeFormatterTest.h"

#include "Formatter/FileTapeFormatter.h"
#include "Tape/FileTape.h"

#include <charconv>
#include <fstream>
#include <unordered_set>

using YTape::FileTape;
using YTape::FileTapeFormatter;

TEST_F(FileTapeFormatterTest, FormatterCreatesFileIfItDoesntExist)
{
    auto path1 = FileTapeFormatter::makeEmpty(testDir + "/tapes/tmp/1.txt", 0);
    auto path2 = FileTapeFormatter::makeZero(testDir + "/tapes/tmp/2.txt", 0);
    auto path3 = FileTapeFormatter::makeRandom(testDir + "/tapes/tmp/3.txt", 0, 0, 1);

    EXPECT_TRUE(fs::exists(path1));
    EXPECT_TRUE(fs::exists(path2));
    EXPECT_TRUE(fs::exists(path3));
}

TEST_F(FileTapeFormatterTest, MakeEmptyTest)
{
    auto path = FileTapeFormatter::makeEmpty(testDir + "/tapes/emptyTape.txt", 5);
    bool allLinesAreEmpty = true;

    std::fstream file(path);
    const std::string emptyLine(11, '_');

    size_t size = 0;
    file >> size;

    std::string line;
    std::getline(file, line, FileTape::delim);
    for (size_t i = 0; i < size; ++i)
    {
        std::getline(file, line, FileTape::delim);
        allLinesAreEmpty &= line == emptyLine;
    }
    std::getline(file, line, FileTape::delim);

    EXPECT_EQ(size, 5);
    EXPECT_TRUE(allLinesAreEmpty);
    EXPECT_TRUE(file.eof());
}

TEST_F(FileTapeFormatterTest, MakeZeroTest)
{
    auto path = FileTapeFormatter::makeZero(testDir + "/tapes/zeroTape.txt", 8);
    bool allLinesAreZero = true;

    std::fstream file(path);
    std::string zeroLine(11, '_');
    zeroLine.front() = '0';

    size_t size = 0;
    file >> size;

    std::string line;
    std::getline(file, line, FileTape::delim);
    for (size_t i = 0; i < size; ++i)
    {
        std::getline(file, line, FileTape::delim);
        allLinesAreZero &= line == zeroLine;
    }
    std::getline(file, line, FileTape::delim);

    EXPECT_EQ(size, 8);
    EXPECT_TRUE(allLinesAreZero);
    EXPECT_TRUE(file.eof());
}

TEST_F(FileTapeFormatterTest, MakeRandomTest)
{
    int a = 1;
    int b = 5;
    auto path = FileTapeFormatter::makeRandom(testDir + "/tapes/randomTape.txt", 10, a, b);
    bool allLinesAreAllowed = true;

    std::fstream file(path);

    std::stringstream stringstream;
    stringstream << std::setfill('_') << std::left;

    std::unordered_set<std::string> allowedLines;
    for (int i = a; i <= b; ++i)
    {
        stringstream << std::setw(FileTape::wordSize - 1) << i;
        allowedLines.insert(std::move(stringstream).str());
    }

    size_t size = 0;
    file >> size;

    std::string line;
    std::getline(file, line, FileTape::delim);
    for (size_t i = 0; i < size; ++i)
    {
        std::getline(file, line, FileTape::delim);
        allLinesAreAllowed &= allowedLines.contains(line);
    }
    std::getline(file, line, FileTape::delim);

    EXPECT_EQ(size, 10);
    EXPECT_TRUE(allLinesAreAllowed);
    EXPECT_TRUE(file.eof());
}

TEST_F(FileTapeFormatterTest, MakeFromFileTest)
{
    auto path = FileTapeFormatter::makeFromFile(testDir + "/tapes/fromFileTape.txt", testDir + "/tape-template.txt");
    bool allLinesAreCorrect = true;

    std::fstream tape(path);

    size_t size = 0;
    char number[21];
    tape.getline(number, 21, FileTape::delim);
    std::from_chars(number, number + 21, size);

    std::string line;
    std::fstream file(testDir + "/tape-template.txt");
    for (size_t i = 0; i < size; ++i)
    {
        tape.getline(number, FileTape::wordSize, FileTape::delim);

        int tapeValue;
        auto result = std::from_chars(number, number + FileTape::wordSize, tapeValue);

        int value;
        file >> value;

        allLinesAreCorrect &= result.ec == std::errc() && tapeValue == value;
    }
    std::getline(tape, line, FileTape::delim);

    EXPECT_EQ(size, 7);
    EXPECT_TRUE(allLinesAreCorrect);
    EXPECT_TRUE(tape.eof());
}

TEST_F(FileTapeFormatterTest, MakeFromContainerTest)
{
    std::vector numbers = {3, 1, -5, 22, 31, 14, -32, 85, 7, 6};
    auto path =
        FileTapeFormatter::makeFromContainer(testDir + "/tapes/fromContainerTape.txt", numbers.begin(), numbers.end());
    bool allLinesAreCorrect = true;

    std::fstream file(path);
    std::string zeroLine(11, '_');
    zeroLine.front() = '0';

    size_t size = 0;
    file >> size;

    std::string line;
    std::getline(file, line, FileTape::delim);
    for (size_t i = 0; i < size; ++i)
    {
        int value = 0;
        std::getline(file, line, FileTape::delim);
        auto result = std::from_chars(line.data(), line.data() + FileTape::wordSize, value);
        allLinesAreCorrect &= result.ec == std::errc() && value == numbers[i];
    }
    std::getline(file, line, FileTape::delim);

    EXPECT_EQ(size, numbers.size());
    EXPECT_TRUE(allLinesAreCorrect);
    EXPECT_TRUE(file.eof());
}