#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/Interpreter.h"

namespace fs = std::filesystem;

std::string readFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr
            << "Error: Cannot open file '"
            << path << "'"
            << std::endl;

        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

bool runSource(
    const std::string& source,
    const std::string& label)
{
    if (source.empty())
    {
        return false;
    }

    try
    {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto program = parser.parseProgram();

        if (!program)
        {
            std::cerr
                << "[" << label
                << "] Parse error"
                << std::endl;
            return false;
        }

        Interpreter interpreter;
        interpreter.executeProgram(
            program.get());

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "[" << label
            << "] Runtime error: "
            << e.what()
            << std::endl;

        return false;
    }
}

void printUsage(const char* progName)
{
    std::cout
        << "MiniV8 — A Mini JavaScript Interpreter\n"
        << "\n"
        << "Usage:\n"
        << "  " << progName
        << " <file.js>          "
        << "Run a single JS file\n"
        << "  " << progName
        << " --test-dir <dir>   "
        << "Run all .js files in directory\n"
        << "  " << progName
        << "                    "
        << "Show this help\n"
        << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printUsage(argv[0]);
        return 0;
    }

    std::string arg1 = argv[1];

    // --test-dir mode: run all .js files
    if (arg1 == "--test-dir")
    {
        if (argc < 3)
        {
            std::cerr
                << "Error: --test-dir "
                << "requires a directory path"
                << std::endl;
            return 1;
        }

        std::string dirPath = argv[2];

        if (!fs::exists(dirPath) ||
            !fs::is_directory(dirPath))
        {
            std::cerr
                << "Error: '"
                << dirPath
                << "' is not a valid directory"
                << std::endl;
            return 1;
        }

        // Collect and sort .js files
        std::vector<fs::path> jsFiles;

        for (auto& entry :
            fs::directory_iterator(dirPath))
        {
            if (entry.path().extension()
                == ".js")
            {
                jsFiles.push_back(
                    entry.path());
            }
        }

        std::sort(
            jsFiles.begin(),
            jsFiles.end());

        int passed = 0;
        int failed = 0;

        for (size_t i = 0;
             i < jsFiles.size(); i++)
        {
            std::string filename =
                jsFiles[i].filename()
                    .string();

            std::cout
                << "\n===== "
                << filename
                << " ====="
                << std::endl;

            std::string source =
                readFile(
                    jsFiles[i].string());

            if (runSource(source, filename))
            {
                passed++;
            }
            else
            {
                failed++;
            }
        }

        std::cout
            << "\n===== Results ====="
            << std::endl;

        std::cout
            << "Passed: " << passed
            << " / "
            << (passed + failed)
            << std::endl;

        if (failed > 0)
        {
            std::cout
                << "Failed: " << failed
                << std::endl;
        }

        return failed > 0 ? 1 : 0;
    }

    // Single file mode
    if (!fs::exists(arg1))
    {
        std::cerr
            << "Error: File '"
            << arg1
            << "' not found"
            << std::endl;
        return 1;
    }

    std::string source = readFile(arg1);

    std::string filename =
        fs::path(arg1).filename().string();

    if (!runSource(source, filename))
    {
        return 1;
    }

    return 0;
}