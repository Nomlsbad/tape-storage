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
        std::cerr << "Invalid input. << \n"
                  << "Usage: ./tape_formatter <tape-path> <size>";
        return EXIT_FAILURE;
    }

    fs::path tapePath(argv[1]);
    if (!fs::exists(tapePath))
    {
        std::cerr << "File " << fs::absolute(tapePath) << " doesn't exist!";
        return EXIT_FAILURE;
    }

    std::string flag;
    std::vector<std::string> params;

    for (int i = 2; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            flag = argv[i];
        }
        else
        {
            params.emplace_back(argv[i]);
        }
    }

    std::unordered_map<std::string, bool (*)(const std::string&, const std::vector<std::string>&)> map;

    map[""] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cerr << "Wrong number of parameters";
            return false;
        }
        try
        {
            const size_t size = std::stoul(params[0]);
            FileTapeFormatter::makeEmpty(path, std::stoul(params.front()));
        }
        catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    map["-z"] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cerr << "Wrong number of parameters";
            return false;
        }
        try
        {
            const size_t size = std::stoul(params[0]);
            FileTapeFormatter::makeZero(path, std::stoul(params.front()));
        }
        catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    map["-f"] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cerr << "Wrong number of parameters";
            return false;
        }
        try
        {
            FileTapeFormatter::makeFromFile(path, params.front());
        } catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    map["-r"] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.size() != 3)
        {
            std::cerr << "Wrong number of parameters";
            return false;
        }
        try
        {
            const size_t size = std::stoul(params[0]);
            const int a = std::stoi(params[1]);
            const int b = std::stoi(params[2]);
            FileTapeFormatter::makeRandom(path, size, a, b);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    map["-c"] = [](const std::string& path, const std::vector<std::string>& params)
    {
        if (params.empty())
        {
            std::cerr << "Wrong number of parameters";
            return false;
        }
        try
        {
            std::vector<int> numbers;
            std::transform(params.cbegin() + 1, params.cend(), std::back_inserter(numbers),
                           [](const std::string& number) { return std::stoi(number); });
            FileTapeFormatter::makeFromContainer(path, numbers.begin(), numbers.end());
        } catch (std::exception& e)
        {
            std::cerr << e.what();
            return false;
        }
        return true;
    };

    map[flag](tapePath, params);

    return EXIT_SUCCESS;
}
