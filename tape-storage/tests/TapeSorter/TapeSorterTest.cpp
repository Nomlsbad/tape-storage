
#include "TapeSorterTest.h"

#include "Sorter/TapeSorter.h"

using YTape::TapeSorter;

TEST_F(TapeSorterTest, ConstructorThrowsExceptionIfChunckLimitSmall)
{
    EXPECT_THROW(TapeSorter(tapes.begin(), tapes.end(), sizeof(int)-1), std::invalid_argument);
    EXPECT_NO_THROW(TapeSorter(tapes.begin(), tapes.end(), 4));
}

TEST_F(TapeSorterTest, ConstructorThrowsExceptionIfNotEnoughTmpTapes)
{
    EXPECT_THROW(TapeSorter(tapes.begin(), tapes.begin() + 3, 8), std::invalid_argument);
    EXPECT_NO_THROW(TapeSorter(tapes.begin(), tapes.begin() + 4, 8));
}

TEST_F(TapeSorterTest, SortingTest)
{
    FileTape in(inputPath, simulator);
    FileTape out(outputPath, simulator);

    TapeSorter sorter(tapes.begin(), tapes.end(), 8);
    sorter.sort(in, out);

    out.rewind();
    std::vector<int> result(sortedInput.size());
    for (auto& number: result)
    {
        out.read(number);
        out.next();
    }

    EXPECT_EQ(result, sortedInput);
}