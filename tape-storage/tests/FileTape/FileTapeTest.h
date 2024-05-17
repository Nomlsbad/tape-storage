#ifndef TAPE_STORAGE_FILETAPE_TEST_H
#define TAPE_STORAGE_FILETAPE_TEST_H

class FileTapeTest: public ::testing::Test
{
protected:

    static constexpr std::string testsDir = TESTS_DIR;
    static constexpr std::string testTapePath = testsDir + "/FileTape/testTape.txt";
    static constexpr std::string writeTestTapePath = testsDir + "/FileTape/writeTestTape.txt";
};

#endif // TAPE_STORAGE_FILETAPE_TEST_H
