#ifndef TAPE_STORAGE_FILETAPE_TEST_H
#define TAPE_STORAGE_FILETAPE_TEST_H

#include "Formatter/FileTapeFormatter.h"

class FileTapeTest: public ::testing::Test
{
protected:

    inline static void SetUpTestCase()
    {
        YTape::FileTapeFormatter::makeEmpty(testTapePath, testTapeSize);
        YTape::FileTapeFormatter::makeEmpty(writeTestTapePath, writeTestTapeSize);
    }

    inline static const std::string testsDir = TESTS_DIR;
    inline static const std::string testDir = testsDir + "/FileTape";

    inline static const std::string testTapePath = testDir + "/tapes/testTape.txt";
    static constexpr size_t testTapeSize = 5;

    inline static const std::string writeTestTapePath = testDir + "/tapes/writeTestTape.txt";
    static constexpr size_t writeTestTapeSize = 5;
};

#endif // TAPE_STORAGE_FILETAPE_TEST_H
