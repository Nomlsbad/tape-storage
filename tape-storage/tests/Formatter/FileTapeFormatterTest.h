#ifndef TAPE_STORAGE_FILETAPEFORMATTERTEST_H
#define TAPE_STORAGE_FILETAPEFORMATTERTEST_H

#include <gtest/gtest.h>

class FileTapeFormatterTest: public ::testing::Test
{
protected:

    inline static const std::string testsDir = TESTS_DIR;
    inline static const std::string testDir = testsDir + "/Formatter";
};

#endif // TAPE_STORAGE_FILETAPEFORMATTERTEST_H
