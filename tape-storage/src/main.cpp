#include "Sorter/TapeSorter.h"
#include "Tape/FileTape.h"

#include <iostream>

using namespace YTape;

int main()
{
    std::string path = "/home/ilya/programming/test-tasks/tape-storage/tape-storage/resources/";
    std::vector<ITape*> tapes = {
        new FileTape(path + "/tmp/tmp-tape-1.txt"),
        new FileTape(path + "/tmp/tmp-tape-2.txt"),
        new FileTape(path + "/tmp/tmp-tape-3.txt"),
        new FileTape(path + "/tmp/tmp-tape-4.txt"),
    };
    FileTape in(path + "input-tape.txt");
    FileTape out(path + "output-tape.txt");

    try
    {
        TapeSorter sorter(tapes.begin(), tapes.end(), 8);
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