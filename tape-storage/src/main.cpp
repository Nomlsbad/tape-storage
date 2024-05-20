#include "Sorter/TapeSorter.h"
#include "Tape/FileTape.h"

#include <iostream>

using namespace YTape;

int main()
{
    std::string resourcesPath = RESOURCES_DIR;
    std::vector<ITape*> tapes = {
        new FileTape(resourcesPath + "/tmp/tmp-tape-1.txt"),
        new FileTape(resourcesPath + "/tmp/tmp-tape-2.txt"),
        new FileTape(resourcesPath + "/tmp/tmp-tape-3.txt"),
        new FileTape(resourcesPath + "/tmp/tmp-tape-4.txt"),
    };
    FileTape in(resourcesPath + "/input-tape.txt");
    FileTape out(resourcesPath + "/output-tape.txt");

    try
    {
        TapeSorter<std::greater<>> sorter(tapes.begin(), tapes.end(), 8);
        sorter.sort(in, out);
    } catch (const std::invalid_argument& e)
    {
        std::cerr << e.what() << "\n";
    }

    for (auto& tape: tapes)
    {
        delete tape;
    }

    return 1;
}