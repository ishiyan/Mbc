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
        << "Arguments: file_or_directory_name {regex_pattern}" << std::endl
        << std::endl
        << "Examples:" << std::endl
        << R"(. .*\.[hH]5 Matches all .h5 and .H5 files recursively. This is the default regex pattern if not specified.)" << std::endl
        << R"(foo.h5      Matches ./foo.h5 file.)" << std::endl
        << std::endl
        << "Regex (single-line) crash course:" << std::endl
        << R"(\      Marks the next character as either a special character or escapes a literal. For example, "n" matches the character "n". "\n" matches a newline character. The sequence "\\" matches "\" and "\(" matches "(".)" << std::endl
        << R"(^      Matches the first character in the string.)" << std::endl
        << R"($      Matches the last character in the string.)" << std::endl
        << R"(*      Matches the preceding character zero or more times. For example, "zo*" matches either "z" or "zoo".)" << std::endl
        << R"(+      Matches the preceding character one or more times. For example, "zo+" matches "zoo" but not "z".)" << std::endl
        << R"(?      Matches the preceding character zero or one time. For example, "a?ve?" matches the "ve" in "never".)" << std::endl
        << R"(.      Matches any single character except a newline character.)" << std::endl
        << R"(x|y    Matches either x or y. For example, "z|wood" matches "z" or "wood". "(z|w)oo" matches "zoo" or "wood".)" << std::endl
        << R"({n}    n is a non-negative integer. Matches exactly n times. For example, "o{2}" does not match the "o" in "Bob," but matches the first two o's in "foooood".)" << std::endl
        << R"({n,}   n is a non-negative integer. Matches at least n times. For example, "o{2,}" does not match the "o" in "Bob" and matches all the o's in "foooood." "o{1,}" is equivalent to "o+". "o{0,}" is equivalent to "o*".)" << std::endl
        << R"({n,m}  m and n are non-negative integers. Matches at least n and at most m times. For example, "o{1,3}" matches the first three o's in "foooood." "o{0,1}" is equivalent to "o?".)" << std::endl
        << R"([xyz]  A character set. Matches any one of the enclosed characters. For example, "[abc]" matches the "a" in "plain".)" << std::endl
        << R"([^xyz] A negative character set. Matches any character not enclosed. For example, "[^abc]" matches the "p" in "plain".)" << std::endl
        << R"([a-z]  A range of characters. Matches any character in the specified range. For example, "[a-z]" matches any lowercase alphabetic character in the range "a" through "z".)" << std::endl
        << R"([^m-z] A negative range characters. Matches any character not in the specified range. For example, "[m-z]" matches any character not in the range "m" through "z".)" << std::endl
        << R"(\d     Matches a digit character. Equivalent to [0-9].)" << std::endl
        << R"(\D     Matches a non-digit character. Equivalent to [^0-9].)" << std::endl
        << R"(\s     Matches any white space including space, tab, form-feed, etc. Equivalent to "[ \f\n\r\t\v]".)" << std::endl
        << R"(\S     Matches any nonwhite space character. Equivalent to "[^ \f\n\r\t\v]".)" << std::endl
        << R"(\w     Matches any word character including underscore. Equivalent to "[A-Za-z0-9_]".)" << std::endl
        << R"(\W     Matches any non-word character. Equivalent to "[^A-Za-z0-9_]".)" << std::endl
        << std::endl;
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
    if (parser.is_help(1U))
        return help();

    // The first argument is a file or directory name.
    const std::string& name = parser.get_option(0U);

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

/*

        private enum Command
        {
            None, Days, Ticks, Objects, Files
        }

        private static Command command = Command.None;
        private static Regex objectPathPattern, fileNamePattern;
        private static string fileOrDirectoryName;
        private static long ticksFrom, ticksTo = long.MaxValue;

        static void Main(string[] args)
        {
            if (!ParseArguments(args) || Command.None == command)
                return;
            Repository.InterceptErrorStack();
            TraverseTree(fileOrDirectoryName, PerformOperation);
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA2204:Literals should be spelled correctly")]
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Globalization", "CA1303:Do not pass literals as localized parameters")]
        private static bool ParseArguments(string[] args)
        {
            bool parsed = false, allUsage = false;
            if (args.Length > 0
                && !args[0].StartsWith("-?", StringComparison.OrdinalIgnoreCase)
                && !args[0].StartsWith("-h", StringComparison.OrdinalIgnoreCase))
            {
                if ("days" == args[0])
                {
                    command = Command.Days;
                    if (args.Length == 6)
                    {
                        fileOrDirectoryName = args[1];
                        fileNamePattern = new Regex(args[2]);
                        objectPathPattern = new Regex(args[3]);
                        string s = args[4];
                        var dateTime = new DateTime(
                            int.Parse(s.Substring(0, 4), NumberStyles.Integer, CultureInfo.InvariantCulture),
                            int.Parse(s.Substring(4, 2), NumberStyles.Integer, CultureInfo.InvariantCulture),
                            int.Parse(s.Substring(6, 2), NumberStyles.Integer, CultureInfo.InvariantCulture));
                        ticksFrom = dateTime.Ticks;
                        dateTime = dateTime.AddDays(int.Parse(args[5], NumberStyles.Integer, CultureInfo.InvariantCulture));
                        ticksTo = dateTime.Ticks - 1L;
                        parsed = true;
                    }
                    else if (args.Length == 5)
                    {
                        fileOrDirectoryName = args[1];
                        objectPathPattern = new Regex(args[2]);
                        string s = args[3];
                        var dateTime = new DateTime(
                            int.Parse(s.Substring(0, 4), NumberStyles.Integer, CultureInfo.InvariantCulture),
                            int.Parse(s.Substring(4, 2), NumberStyles.Integer, CultureInfo.InvariantCulture),
                            int.Parse(s.Substring(6, 2), NumberStyles.Integer, CultureInfo.InvariantCulture));
                        ticksFrom = dateTime.Ticks;
                        dateTime = dateTime.AddDays(int.Parse(args[4], NumberStyles.Integer, CultureInfo.InvariantCulture));
                        ticksTo = dateTime.Ticks - 1L;
                        parsed = true;
                    }
                }
                else if ("ticks" == args[0])
                {
                    command = Command.Ticks;
                    if (args.Length == 6)
                    {
                        fileOrDirectoryName = args[1];
                        fileNamePattern = new Regex(args[2]);
                        objectPathPattern = new Regex(args[3]);
                        ticksFrom = long.Parse(args[4], NumberStyles.Integer, CultureInfo.InvariantCulture);
                        ticksTo = long.Parse(args[5], NumberStyles.Integer, CultureInfo.InvariantCulture);
                        parsed = true;
                    }
                    else if (args.Length == 5)
                    {
                        fileOrDirectoryName = args[1];
                        objectPathPattern = new Regex(args[2]);
                        ticksFrom = long.Parse(args[3], NumberStyles.Integer, CultureInfo.InvariantCulture);
                        ticksTo = long.Parse(args[4], NumberStyles.Integer, CultureInfo.InvariantCulture);
                        parsed = true;
                    }
                }
                else if ("objects" == args[0])
                {
                    command = Command.Objects;
                    if (args.Length == 4)
                    {
                        fileOrDirectoryName = args[1];
                        fileNamePattern = new Regex(args[2]);
                        objectPathPattern = new Regex(args[3]);
                        parsed = true;
                    }
                    else if (args.Length == 3)
                    {
                        fileOrDirectoryName = args[1];
                        objectPathPattern = new Regex(args[2]);
                        parsed = true;
                    }
                }
                else if ("files" == args[0])
                {
                    command = Command.Files;
                    if (args.Length == 3)
                    {
                        fileOrDirectoryName = args[1];
                        fileNamePattern = new Regex(args[2]);
                        parsed = true;
                    }
                    else if (args.Length == 2)
                    {
                        fileOrDirectoryName = args[1];
                        parsed = true;
                    }
                }
                else
                    allUsage = true;
            }
            else
                allUsage = true;
            if (allUsage || (!parsed && Command.Days == command))
            {
                Console.WriteLine("Arguments: days    file_or_directory_name {filename_regex_pattern} dataset_regex_pattern yyyymmdd number_of_days");
                Console.WriteLine("Example:   days    c:\\test AEX.h5 .* /T____ 20120605 1");
            }
            if (allUsage || (!parsed && Command.Ticks == command))
            {
                Console.WriteLine("Arguments: ticks   file_or_directory_name {filename_regex_pattern} dataset_regex_pattern ticksFrom ticksTo (inclusive)");
                Console.WriteLine("Example:   ticks   c:\\test AEX.h5 .* /T____ from to");
            }
            if (allUsage || (!parsed && Command.Objects == command))
            {
                Console.WriteLine("Arguments: objects file_or_directory_name {filename_regex_pattern} object_regex_pattern");
                Console.WriteLine("Example:   objects c:\\test\\ebf.h5 /EONIASWAP.?M");
            }
            if (allUsage || (!parsed && Command.Files == command))
            {
                Console.WriteLine("Arguments: files   file_or_directory_name {filename_regex_pattern}");
                Console.WriteLine("Example:   files   c:\\test\\ ebf\\.h.?");
            }
            if (allUsage)
                PrintRegexHelp();
            return parsed;
        }

        private static void TraverseTree(string root, Action<string> action)
        {
            if (Directory.Exists(root))
            {
                string[] entries = Directory.GetFiles(root);
                foreach (string entry in entries)
                {
                    if (null != fileNamePattern && !fileNamePattern.IsMatch(entry))
                            continue;
                    action(entry);
                }
                entries = Directory.GetDirectories(root);
                foreach (string entry in entries)
                    TraverseTree(entry, action);
            }
            else if (File.Exists(root))
            {
                if (null != fileNamePattern)
                {
                    if (fileNamePattern.IsMatch(root))
                        action(root);
                }
                else
                    action(root);
            }
        }

        private static bool DeleteGroupInfo(Repository repository, GroupInfo groupInfo)
        {
            if (null != groupInfo.Parent && (null == objectPathPattern || objectPathPattern.IsMatch(groupInfo.Path, 0)))
            {
                Debug.WriteLine(string.Format(CultureInfo.InvariantCulture, "pattern [{0}] matched groupInfo path [{1}]", objectPathPattern, groupInfo.Path));
                if (repository.Delete(groupInfo.Path, true))
                    return true;
            }
            bool deleted = false;

            // First, try datasets.
            foreach (var dataInfo in groupInfo.Datasets)
            {
                if (null == objectPathPattern || objectPathPattern.IsMatch(dataInfo.Path, 0))
                {
                    Debug.WriteLine(string.Format(CultureInfo.InvariantCulture, "pattern [{0}] matched dataInfo path [{1}]", objectPathPattern, dataInfo.Path));
                    if (repository.Delete(dataInfo.Path, false))
                        deleted = true;
                }
            }
            // Next, try child groups.
            foreach (var grpInfo in groupInfo.Groups)
            {
                if (null == objectPathPattern || objectPathPattern.IsMatch(grpInfo.Path, 0))
                {
                    Debug.WriteLine(string.Format(CultureInfo.InvariantCulture, "pattern [{0}] matched grpInfo path [{1}]", objectPathPattern, grpInfo.Path));
                    if (repository.Delete(grpInfo.Path, false))
                        deleted = true;
                }
                else if (DeleteGroupInfo(repository, grpInfo))
                    deleted = true;
            }
            return deleted;
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1031:DoNotCatchGeneralExceptionTypes")]
        private static void PerformOperation(string sourceFileName)
        {
            try
            {
                if (Command.Files == command)
                {
                    File.Delete(sourceFileName);
                    Trace.TraceInformation("Deleted file {0}", sourceFileName);
                    return;
                }
                if (!sourceFileName.EndsWith(".h5", StringComparison.Ordinal))
                    return;
                using (Repository repository = Repository.OpenReadWrite(sourceFileName, false))
                {
                    if (null == repository)
                    {
                        string msg = string.Format(CultureInfo.InvariantCulture, "[{0}] failed to open file", sourceFileName);
                        Trace.TraceError(msg);
                        return;
                    }
                    if (Command.Days == command || Command.Ticks == command)
                    {
                        List<DataInfo> list = repository.ContentList(false);
                        foreach (var dataInfo in list)
                        {
                            if (!dataInfo.IsValidName)
                                continue;
                            if (null != objectPathPattern && !objectPathPattern.IsMatch(dataInfo.Path))
                                continue;
                            string path = string.Concat(sourceFileName, ":", dataInfo.Parent.Path);
                            using (Instrument instrument = repository.Open(dataInfo.Parent.Path, false))
                            {
                                if (null == instrument)
                                {
                                    Trace.TraceError("[{0}] failed to open the instrument", path);
                                    continue;
                                }
                                if (!instrument.DeleteTickRange(dataInfo, ticksFrom, ticksTo))
                                    Trace.TraceError("[{0}] failed to delete tick range of [{1}, {2}]", path, ticksFrom, ticksTo);
                                else
                                    Trace.TraceInformation("Processed {0}", path);
                            }
                        }
                    }
                    else if (Command.Objects == command)
                    {
                        GroupInfo groupInfo;
                        do
                        {
                            groupInfo = repository.ContentTree(true);
                        } while (DeleteGroupInfo(repository, groupInfo));
                    }
                }
            }
            catch (Exception ex)
            {
                Trace.TraceInformation("Exception: {0}", ex);
            }
        }

*/
