#ifndef TAPE_STORAGE_FILETAPE_TEST_H
#define TAPE_STORAGE_FILETAPE_TEST_H

class FileTapeTest: public ::testing::Test
{
protected:

    inline static const std::string testsDir = TESTS_DIR;
    inline static const std::string testTapePath = testsDir + "/FileTape/testTape.txt";
    inline static const std::string writeTestTapePath = testsDir + "/FileTape/writeTestTape.txt";
};

#endif // TAPE_STORAGE_FILETAPE_TEST_H
