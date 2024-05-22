#include "Formatter/FileTapeFormatter.h"

#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>

using YTape::FileTapeFormatter;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Invalid input.\n"
                  << "Usage: ./tape-formatter <tape-path> <size>\n";
        return EXIT_FAILURE;
    }

    fs::path tapePath(argv[1]);

    std::string flag;
    std::vector<std::string> params;

    std::unordered_map<std::string, bool (*)(const std::string&, const std::vector<std::string>&)> formatterScripts;
    formatterScripts[""] = nullptr;
    formatterScripts["-z"] = nullptr;
    formatterScripts["-r"] = nullptr;
    formatterScripts["-f"] = nullptr;
    formatterScripts["-c"] = nullptr;

    for (int i = 2; i < argc; ++i)
    {
        if (formatterScripts.contains(argv[i]))
        {
            flag = argv[i];
        }
        else
        {
            params.emplace_back(argv[i]);
        }
    }

    formatterScripts[""] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cerr << "Wrong number of parameters\n"
                      << "Usage: ./tape-formatter <tape-path> <size>\n";
            return false;
        }
        try
        {
            const size_t size = std::stoul(params[0]);
            FileTapeFormatter::makeEmpty(path, size);
        } catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    formatterScripts["-z"] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cerr << "Wrong number of parameters\n"
                      << "Usage: ./tape-formatter <tape-path> -z <size>\n";
            return false;
        }
        try
        {
            const size_t size = std::stoul(params[0]);
            FileTapeFormatter::makeZero(path, size);
        } catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    formatterScripts["-f"] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cerr << "Wrong number of parameters\n"
                      << "Usage: ./tape-formatter <tape-path> -f <file-path>\n";
            return false;
        }
        try
        {
            FileTapeFormatter::makeFromFile(path, params[0]);
        } catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    formatterScripts["-r"] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.size() != 3)
        {
            std::cerr << "Wrong number of parameters\n"
                      << "Usage: ./tape-formatter <tape-path> -r <size> <min> <max>\n";
            return false;
        }
        try
        {
            const size_t size = std::stoul(params[0]);
            const int a = std::stoi(params[1]);
            const int b = std::stoi(params[2]);
            FileTapeFormatter::makeRandom(path, size, a, b);
        } catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    formatterScripts["-c"] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.empty())
        {
            std::cerr << "Wrong number of parameters\n"
                      << "Usage: ./tape-formatter <tape-path> -c <numbers...>\n";
            return false;
        }
        try
        {
            std::vector<int> numbers;
            std::transform(params.cbegin(), params.cend(), std::back_inserter(numbers),
                           [](const std::string& number) { return std::stoi(number); });
            FileTapeFormatter::makeFromContainer(path, numbers.begin(), numbers.end());
        } catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    return formatterScripts[flag](tapePath, params) ? EXIT_SUCCESS : EXIT_FAILURE;
}
