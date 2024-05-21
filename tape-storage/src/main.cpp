#include "Config/Configuration.h"
#include "Sorter/TapeSorter.h"
#include "Tape/FileTape.h"
#include "Formatter/FileTapeFormatter.h"

#include <iostream>

using namespace YTape;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Invalid input.\n" << "Usage: ./tape_sorter <input-tape-path> <output-tape-path>\n";
        return EXIT_FAILURE;
    }

    fs::path inputPath = fs::absolute(argv[1]);
    fs::path outputPath = fs::absolute(argv[2]);
    if (!fs::exists(inputPath))
    {
        std::cerr << "file " << inputPath << " doesn't exist\n";
        return EXIT_FAILURE;
    }
    if (!fs::exists(outputPath))
    {
        std::cerr << "file " << outputPath << " doesn't exist\n";
        return EXIT_FAILURE;
    }

    std::string projectPath = PROJECT_DIR;
    Configuration::initialize(projectPath + "/config.json");

    FileTape in(inputPath);
    FileTape out(outputPath);

    std::string resourcesPath = RESOURCES_DIR;
    const size_t tmpTapes = Configuration::get<SystemConfig>("tmpTapes");
    std::vector<ITape*> tapes(tmpTapes);
    for (size_t i = 0; i < tmpTapes; ++i)
    {
        std::stringstream stringstream;
        stringstream << resourcesPath << "/tmp/tape-" << i << ".txt";
        auto path = FileTapeFormatter::makeEmpty(std::move(stringstream).str(), in.getSize());
        tapes[i] = new FileTape(path);
    }

    try
    {
        size_t chunkLimit = Configuration::get<SystemConfig>("memory");
        TapeSorter sorter(tapes.begin(), tapes.end(), chunkLimit);
        sorter.sort(in, out);

    } catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    for (auto& tape: tapes)
    {
        delete tape;
    }

    std::cout << "finally, tapes are sorted!";

    return 1;
}