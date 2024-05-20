#include <gtest/gtest.h>
#include <array>

#include "Tape/FileTape.h"
#include "Tape/Simulator/EmptyTapeSimulator.h"
#include "FileTapeTest.h"

using YTape::FileTape;
using YTape::EmptyTapeSimulator;

TEST_F(FileTapeTest, PrevReturnsFalseIfTapeDoesntHave)
{
    EmptyTapeSimulator simulator;
    FileTape tape(testTapePath, simulator);

    const bool hasPrev = tape.hasPrev();
    const bool shiftCompleted = tape.prev();

    EXPECT_FALSE(hasPrev);
    EXPECT_FALSE(shiftCompleted);
}

TEST_F(FileTapeTest, PrevAndNextReturnTrueIfTapeHas)
{
    EmptyTapeSimulator simulator;
    FileTape tape(testTapePath, simulator);

    const bool hasNext = tape.hasNext();
    const bool shiftNextCompleted = tape.next();

    const bool hasPrev = tape.hasPrev();
    const bool shiftPrevCompleted = tape.prev();

    EXPECT_TRUE(hasNext);
    EXPECT_TRUE(shiftNextCompleted);
    EXPECT_TRUE(hasPrev);
    EXPECT_TRUE(shiftPrevCompleted);
}

TEST_F(FileTapeTest, AfterSizeMinusOneStepsCantGoNext)
{
    EmptyTapeSimulator simulator;
    FileTape tape(testTapePath, simulator);
    size_t steps = 0;

    while (tape.next()) { ++steps; }
    const bool hasNext = tape.hasNext();

    EXPECT_EQ(steps, tape.getSize() - 1);
    EXPECT_FALSE(hasNext);
}

TEST_F(FileTapeTest, RewindReturnsToTheBegining)
{
    EmptyTapeSimulator simulator;
    FileTape tape(testTapePath, simulator);
    constexpr std::array<size_t, 3> stepsAmounts = {1, 2, 4};
    bool rewindsWasCompleted = false;

    for (auto steps: stepsAmounts)
    {
        for (size_t i = 0; i < steps - 1; ++i)
        {
            tape.next();
        }
        tape.rewind();
        rewindsWasCompleted |= tape.hasPrev();
    }

    EXPECT_FALSE(rewindsWasCompleted);
}

TEST_F(FileTapeTest, WiriteInTape)
{
    EmptyTapeSimulator simulator;
    FileTape tape(writeTestTapePath, simulator);
    int32_t value = 0;

    constexpr std::array answer = {1, 2, 3};
    std::array result = {0, 0, 0};

    // Set third number to 2
    tape.next();
    tape.next();
    tape.write(2);

    // Set second number to 1
    tape.prev();
    tape.write(1);

    // Set fourth number to 3
    tape.next();
    tape.next();
    tape.write(3);

    tape.rewind();
    tape.next();
    for (size_t i = 0; i < 3; ++i)
    {
        tape.read(value);
        result[i] = value;

        tape.next();
    }

    EXPECT_EQ(result, answer);
}