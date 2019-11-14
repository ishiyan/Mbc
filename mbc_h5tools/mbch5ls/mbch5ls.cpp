#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include "option_parser.h"
#include "../../mbc/include/mbc/h5.h"

static int help()
{
    std::cerr
        << "Lists datasets in h5 file(s)." << std::endl
        << "Arguments: file_or_directory_name {-r|--regex pattern} {-s|--sort} {-h|--help}" << std::endl
        << R"(-r,--regex    filename regular expression pattern, default value is .*\.[hH]5)" << std::endl
        << R"(-s|--sort     sort entries within a h5 file)" << std::endl
        << std::endl
        << "Examples:" << std::endl
        << R"(.             matches default regex pattern recursively)" << std::endl
        << R"(. -r .*\.H5   matches all .H5 files recursively)" << std::endl
        << R"(foo.h5        matches ./foo.h5 file)" << std::endl
        << std::endl
        << "Regex (single-line) crash course:" << std::endl
        << R"(\      marks the next character as either a special character or escapes a literal)" << std::endl
        << R"(       "\d" matches a digit, "\\" matches "\" and "\(" matches "(".)" << std::endl
        << R"(^      matches the first character in the string)" << std::endl
        << R"($      matches the last character in the string)" << std::endl
        << R"(*      matches the preceding character zero or more times)" << std::endl
        << R"(       "zo*" matches either "z" or "zoo")" << std::endl
        << R"(+      matches the preceding character one or more times)" << std::endl
        << R"(       "zo+" matches "zoo" but not "z")" << std::endl
        << R"(?      matches the preceding character zero or one time)" << std::endl
        << R"(       "a?ve?" matches the "ve" in "never")" << std::endl
        << R"(.      matches any single character except a newline character)" << std::endl
        << R"(x|y    matches either x or y)" << std::endl
        << R"(       "z|wood" matches "z" or "wood", "(z|w)oo" matches "zoo" or "wood")" << std::endl
        << R"({n}    matches exactly n times, n >= 0)" << std::endl
        << R"(       "o{2}" does not match the "o" in "Bob," but matches the first two o's in "foooood")" << std::endl
        << R"({n,}   matches at least n times, n >= 0)" << std::endl
        << R"(       "o{2,}" does not match the "o" in "Bob" and matches all the o's in "foooood")" << std::endl
        << R"(       "o{1,}" is equivalent to "o+", "o{0,}" is equivalent to "o*")" << std::endl
        << R"({n,m}  matches at least n and at most m times, n,m >= 0)" << std::endl
        << R"(       "o{1,3}" matches the first three o's in "foooood", "o{0,1}" is equivalent to "o?")" << std::endl
        << R"([xyz]  a character set, matches any one of the enclosed characters)" << std::endl
        << R"(       "[abc]" matches the "a" in "plain")" << std::endl
        << R"([^xyz] a negative character set, matches any character not enclosed)" << std::endl
        << R"(       "[^abc]" matches the "p" in "plain")" << std::endl
        << R"([a-z]  a range of characters, matches any character in the specified range)" << std::endl
        << R"(       "[a-z]" matches any lowercase alphabetic character in the range "a" through "z")" << std::endl
        << R"([^m-z] a negative range characters, matches any character not in the specified range)" << std::endl
        << R"(       "[m-z]" matches any character not in the range "m" through "z")" << std::endl
        << R"(\d     matches a digit character, equivalent to [0-9])" << std::endl
        << R"(\D     matches a non-digit character, equivalent to [^0-9])" << std::endl
        << R"(\s     matches any white space including space, tab, form-feed, etc; equivalent to "[ \f\n\r\t\v]")" << std::endl
        << R"(\S     matches any nonwhite space character, equivalent to "[^ \f\n\r\t\v]")" << std::endl
        << R"(\w     matches any word character including underscore, equivalent to "[A-Za-z0-9_]")" << std::endl
        << R"(\W     matches any non-word character, equivalent to "[^A-Za-z0-9_]")" << std::endl
        << std::endl;
    return -1;
}

static void action(const std::string& path, const bool sort)
{
    const bool status = mbc::h5::File::enumerateDatasets([&path](const mbc::h5::DataInfo& dataInfo) noexcept
    {
        std::cout << path << ":" << dataInfo.path() << std::endl;

        if (!dataInfo.isValidName())
            std::cerr << "invalid name: " << dataInfo.name() << std::endl;

    }, path.c_str(), mbc::h5::FileDriver::Sec2, sort);
}

int main(const int argc, const char* argv[])  // NOLINT(bugprone-exception-escape)
{
    const option_parser parser{ argc, argv };
    if (parser.is_help(1U))
        return help();

    // The first argument is a file or directory name.
    const std::string& name = parser.get_option(0U);

    // The optional second argument is a regex pattern.
    const std::string& regex = parser.get_option("-r", "--regex");
    const std::shared_ptr<std::regex> pattern = regex.empty() ?
        std::make_shared<std::regex>(".*\\.h5") :
        std::make_shared<std::regex>(regex);

    if (!std::filesystem::exists(name)) {
        std::cerr << name << " does not exist" << std::endl;
        return -1;
    }

    const bool sort = parser.is_option("-s", "--sort");

    if (std::filesystem::is_regular_file(name)) {
        const bool match = pattern ? std::regex_match(name, *pattern) : true;
        if (match)
            action(name, sort);
        return 0;
    }

    if (!std::filesystem::is_directory(name)) {
        std::cerr << name << " is not a regular file or directory" << std::endl;
        return -1;
    }

    for (auto entry = std::filesystem::recursive_directory_iterator(name); entry != std::filesystem::recursive_directory_iterator(); ++entry)
    {
        const auto path = entry->path().string();
        if (!std::filesystem::is_regular_file(path))
            continue;

        const bool match = pattern ? std::regex_match(entry->path().filename().string(), *pattern) : true;
        if (match)
            action(path, sort);
    }
}
