#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include "option_parser.h"
#include "../../mbc/include/mbc/h5.h"

static int help()
{
    std::cerr
        << "Moves an instrument within a h5 file." << std::endl
        << "Arguments: file.h5:/path/oldName /path/newName" << std::endl;
    return -1;
}

static void action(const std::string& path)
{
    const bool status = mbc::h5::File::enumerateDatasets([&path](const mbc::h5::DataInfo& dataInfo) noexcept
    {
        std::cout << path << ":" << dataInfo.path() << std::endl;

        if (!dataInfo.isValidName())
            std::cerr << "invalid name: " << dataInfo.name() << std::endl;

    }, path.c_str(), mbc::h5::FileDriver::Sec2, true);
}

int main(const int argc, const char* argv[])  // NOLINT(bugprone-exception-escape)
{
    const option_parser parser{ argc, argv };
    if (parser.is_help(2U))
        return help();

    // The first argument is a file with an instrument path.
    const std::string& name = parser.get_option(0U);


/*
            string[] splitted = args[0].Split(new[] { ".h5:/" }, StringSplitOptions.None);
            string file = splitted[0] + ".h5";
            string pathOld = "/" + splitted[1];

            try
            {
                using (Repository repository = Repository.OpenReadWrite(file, false))
                {
                    if (null == repository)
                    {
                        string msg = string.Format(CultureInfo.InvariantCulture, "[{0}] failed to open file", file);
                        Trace.TraceError(msg);
                        return;
                    }
                    repository.MoveInstrument(pathOld, args[1]);
                }
            }
            catch (Exception ex)
            {
                Trace.TraceError("Exception: {0}", ex);
            }
*/






    if (!std::filesystem::exists(name)) {
        std::cerr << name << " does not exist" << std::endl;
        return -1;
    }

    if (std::filesystem::is_regular_file(name)) {
        action(name);
        return 0;
    }

    if (!std::filesystem::is_directory(name)) {
        std::cerr << name << " is not a regular file or directory" << std::endl;
        return -1;
    }

    // The optional second argument is a regex pattern.
    const std::shared_ptr<std::regex> pattern = parser.count() > 1U ?
        std::make_shared<std::regex>(parser.get_option(1U)) :
        std::make_shared<std::regex>(".*\\.h5");

    for (auto entry = std::filesystem::recursive_directory_iterator(name); entry != std::filesystem::recursive_directory_iterator(); ++entry)
    {
        const auto path = entry->path().string();
        if (!std::filesystem::is_regular_file(path))
            continue;

        const bool match = pattern ? std::regex_match(entry->path().filename().string(), *pattern) : true;
        if (match)
            action(path);
    }
}
