#ifndef TAPE_STORAGE_FILETAPE_TEST_H
#define TAPE_STORAGE_FILETAPE_TEST_H

class FileTapeTest: public ::testing::Test
{
protected:

    static void SetUpTestCase()
    {
        testTapePath = "/home/ilya/programming/test-tasks/tape-storage/tape-storage/tests/FileTape/testTape.txt";
        writeTestTapePath =
            "/home/ilya/programming/test-tasks/tape-storage/tape-storage/tests/FileTape/writeTestTape.txt";
    }

    static constexpr size_t size = 5;
    inline static std::string testTapePath;
    inline static std::string writeTestTapePath;
};

#endif // TAPE_STORAGE_FILETAPE_TEST_H
