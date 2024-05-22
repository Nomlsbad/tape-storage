#ifndef TAPE_STORAGE_TAPE_SORTER_TEST_H
#define TAPE_STORAGE_TAPE_SORTER_TEST_H

#include "Formatter/FileTapeFormatter.h"
#include "Tape/FileTape.h"
#include "Tape/Simulator/EmptyTapeSimulator.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <iterator>
#include <random>

using YTape::EmptyTapeSimulator;
using YTape::FileTape;
using YTape::FileTapeFormatter;
using YTape::ITape;

class TapeSorterTest: public ::testing::Test
{
protected:

    inline static void SetUpTestCase()
    {
        for (size_t i = 1; i <= maxTmpTapes; ++i)
        {
            std::stringstream stringstream;
            stringstream << testDir << "/tapes/tmp" << i << ".txt";

            auto path = FileTapeFormatter::makeEmpty(std::move(stringstream).str(), maxTapeSize);
            fileTapes.emplace_back(path, simulator);
        }
    }

    void SetUp() override
    {
        std::transform(fileTapes.begin(), fileTapes.end(), std::back_inserter(tapes),
                       [](FileTape& tape) mutable { return &tape; });

        std::random_device rd;
        std::mt19937 g(rd());
        std::vector input = sortedInput;
        std::shuffle(input.begin(), input.end(), g);

        inputPath = FileTapeFormatter::makeFromContainer(testDir + "/tapes/input.txt", input.begin(), input.end());
        outputPath = FileTapeFormatter::makeEmpty(testDir + "/tapes/output.txt", input.size());
    }

protected:

    inline static const size_t maxTapeSize = 16;
    inline static const size_t maxTmpTapes = 4;

    inline static std::vector<FileTape> fileTapes;
    inline static EmptyTapeSimulator simulator;

    std::vector<ITape*> tapes;

protected:

    inline static const std::string testsDir = TESTS_DIR;
    inline static const std::string testDir = testsDir + "/TapeSorter";
    inline static const std::string tapesDir = testDir + "/tapes";

    inline static std::string inputPath;
    inline static std::string outputPath;

    inline static std::vector<int32_t> sortedInput {-6, -1, 0, 1, 1, 1, 2, 2, 3, 6, 7, 7, 8, 12, 23, 43};
};


#endif // TAPE_STORAGE_TAPE_SORTER_TEST_H
