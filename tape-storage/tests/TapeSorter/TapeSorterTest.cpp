
#include "TapeSorterTest.h"

#include "Sorter/TapeSorter.h"

using YTape::TapeSorter;

TEST_F(TapeSorterTest, ConstructorThrowsExceptionIfChunckLimitSmall)
{
    EXPECT_THROW(TapeSorter(tapes.begin(), tapes.end(), sizeof(int) - 1), std::invalid_argument);
    EXPECT_NO_THROW(TapeSorter(tapes.begin(), tapes.end(), 4));
}

TEST_F(TapeSorterTest, ConstructorThrowsExceptionIfNotEnoughTmpTapes)
{
    EXPECT_THROW(TapeSorter(tapes.begin(), tapes.begin() + 3, 8), std::invalid_argument);
    EXPECT_NO_THROW(TapeSorter(tapes.begin(), tapes.begin() + 4, 8));
}

TEST_F(TapeSorterTest, SortThrowsExceptionIfInSizeGreaterThanOutSize)
{
    auto inPath = FileTapeFormatter::makeEmpty(tapesDir +"/5.txt", 5);
    auto outPath = FileTapeFormatter::makeEmpty(tapesDir +"/3.txt", 3);
    FileTape in(inPath, simulator);
    FileTape outL(outPath, simulator);
    FileTape outG(outputPath, simulator);

    TapeSorter sorter(tapes.begin(), tapes.end(), 8);

    EXPECT_LT(outL.getSize(), in.getSize());
    EXPECT_THROW(sorter.sort(in, outL), std::invalid_argument);
}

TEST_F(TapeSorterTest, SortThrowsExceptionIfInOrOutSizesGreaterThanMinTmpSize)
{
    FileTape in(inputPath, simulator);
    FileTape out(outputPath, simulator);

    std::vector<FileTape> tmpTapes;
    tmpTapes.emplace_back(FileTapeFormatter::makeEmpty(tapesDir + "/empty-tape-1.txt", 5), simulator);
    tmpTapes.emplace_back(FileTapeFormatter::makeEmpty(tapesDir + "/empty-tape-2.txt", 5), simulator);
    tmpTapes.emplace_back(FileTapeFormatter::makeEmpty(tapesDir + "/empty-tape-3.txt", 5), simulator);
    tmpTapes.emplace_back(FileTapeFormatter::makeEmpty(tapesDir + "/empty-tape-4.txt", 5), simulator);

    TapeSorter sorter(tmpTapes.begin(), tmpTapes.end(), 8);

    EXPECT_GT(in.getSize(), 5);
    EXPECT_GT(out.getSize(), 5);
    EXPECT_THROW(sorter.sort(in, out), std::invalid_argument);
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