#include "Tape/Simulator/TapeHardwareSimulator.h"

#include "Config/Configuration.h"

#include <thread>

using YTape::ITapeSimulator;
using YTape::TapeHardwareSimulator;

TapeHardwareSimulator::TapeHardwareSimulator()
    : shiftDelay(getConfigParam("shiftDelay"))
    , rewindDelay(getConfigParam("rewindDelay"))
    , readDelay(getConfigParam("readDelay"))
    , writeDelay(getConfigParam("writeDelay"))
{}

ITapeSimulator& TapeHardwareSimulator::getInstance()
{
    static TapeHardwareSimulator tapeHardwareSimulator;
    return tapeHardwareSimulator;
}

TapeHardwareSimulator::TimeDuration TapeHardwareSimulator::getConfigParam(const std::string& param)
{
    const size_t ns = Configuration::get<TapeConfig>(param);
    return std::chrono::nanoseconds(ns);
}

void TapeHardwareSimulator::shift()
{
    std::this_thread::sleep_for(shiftDelay);
}

void TapeHardwareSimulator::rewind(size_t)
{
    std::this_thread::sleep_for(rewindDelay);
}

void TapeHardwareSimulator::read()
{
    std::this_thread::sleep_for(readDelay);
}

void TapeHardwareSimulator::write()
{
    std::this_thread::sleep_for(writeDelay);
}