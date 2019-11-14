using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Mbcs.H5;

// ReSharper disable once InconsistentNaming
namespace Mbcsh5ls
{
    // ReSharper disable once InconsistentNaming
    // ReSharper disable once ClassNeverInstantiated.Global
    internal class Mbcsh5ls
    {
        private static ILogger logger;
        private static bool sortContent;
        private static Regex regexPattern;

        internal static int Main(string[] args)
        {
            if (args.Length < 1
                || args[0].StartsWith("-?", StringComparison.OrdinalIgnoreCase)
                || args[0].StartsWith("--?", StringComparison.OrdinalIgnoreCase)
                || args[0].StartsWith("-h", StringComparison.OrdinalIgnoreCase)
                || args[0].StartsWith("--h", StringComparison.OrdinalIgnoreCase))
            {
                Console.WriteLine("Lists datasets in h5 file(s).");
                Console.WriteLine("Arguments: file_or_directory_name {regex_pattern}");
                PrintRegexHelp();
                return -1;
            }

            var serviceProvider = BuildServiceProvider();
            var configuration = serviceProvider.GetService<IConfigurationRoot>();

            logger = serviceProvider.GetService<ILogger<Mbcsh5ls>>();
            sortContent = configuration.GetSection("SortContent").Get<bool>();

            Mbcs.H5.File.InterceptErrorStack((message, isError) =>
            {
                if (isError)
                {
                    logger.LogError($"HDF5: {message}");
                }
                else
                {
                    logger.LogInformation($"HDF5: {message}");
                }
            });

            if (args.Length > 1)
                regexPattern = new Regex(args[1]);
            TraverseTree(args[0], ListDatasets);
            return 0;
        }

        private static IServiceProvider BuildServiceProvider()
        {
            var configuration = new ConfigurationBuilder()
                .SetBasePath(AppContext.BaseDirectory)
                .AddJsonFile("Mbcsh5ls.json", false)
                .Build();

            var serviceCollection = new ServiceCollection()
                .AddSingleton(configuration)
                .AddLogging(loggingBuilder =>
                {
                    loggingBuilder.AddConfiguration(configuration.GetSection("Logging"));
                    loggingBuilder.AddConsole();
                });

            return serviceCollection.BuildServiceProvider();
        }

        private static void ListDatasets(string sourceFileName)
        {
            if (!sourceFileName.EndsWith(".h5", StringComparison.Ordinal))
                return;

            var list = new List<DataInfo>();
            if (!Mbcs.H5.File.EnumerateDatasets(sourceFileName, list, sortContent))
            {
                logger.LogError($"Failed to enumerate datasets in [{sourceFileName}]");
            }

            string file = string.Concat(sourceFileName, ":");
            foreach (var v in list)
            {
                if (null != regexPattern)
                {
                    if (!regexPattern.IsMatch(v.Path))
                        continue;
                }

                Console.WriteLine(string.Concat(file, v.Path));
                if (!v.IsValidName)
                {
                    logger.LogError($"Invalid dataset [{v.Name}] in [{sourceFileName}]");
                }
            }
        }

        private static void TraverseTree(string root, Action<string> action)
        {
            if (Directory.Exists(root))
            {
                string[] entries = Directory.GetFiles(root);
                foreach (string entry in entries)
                    action(entry);
                entries = Directory.GetDirectories(root);
                foreach (string entry in entries)
                    TraverseTree(entry, action);
            }
            else if (System.IO.File.Exists(root))
                action(root);
        }

        private static void PrintRegexHelp()
        {
            Console.WriteLine("Regex (single-line) crash course:");
            Console.WriteLine("\\         Marks the next character as either a special character or escapes a literal. For example, \"n\" matches the character \"n\". \"\\n\" matches a newline character. The sequence \"\\\\\" matches \"\\\" and \"\\(\" matches \"(\".");
            Console.WriteLine("^         Matches the first character in the string.");
            Console.WriteLine("$         Matches the last character in the string.");
            Console.WriteLine("*         Matches the preceding character zero or more times. For example, \"zo*\" matches either \"z\" or \"zoo\".");
            Console.WriteLine("+         Matches the preceding character one or more times. For example, \"zo+\" matches \"zoo\" but not \"z\".");
            Console.WriteLine("?         Matches the preceding character zero or one time. For example, \"a?ve?\" matches the \"ve\" in \"never\".");
            Console.WriteLine(".         Matches any single character except a newline character.");
            Console.WriteLine("(pattern) Matches pattern and remembers the match. The matched substring can be retrieved from the resulting Matches collection, using Item [0]...[n]. To match parentheses characters ( ), use \"\\(\" or \"\\)\".");
            Console.WriteLine("x|y       Matches either x or y. For example, \"z|wood\" matches \"z\" or \"wood\". \"(z|w)oo\" matches \"zoo\" or \"wood\".");
            Console.WriteLine("{n}       n is a non-negative integer. Matches exactly n times. For example, \"o{2}\" does not match the \"o\" in \"Bob,\" but matches the first two o's in \"foooood\".");
            Console.WriteLine("{n,}      n is a non-negative integer. Matches at least n times. For example, \"o{2,}\" does not match the \"o\" in \"Bob\" and matches all the o's in \"foooood.\" \"o{1,}\" is equivalent to \"o+\". \"o{0,}\" is equivalent to \"o*\".");
            Console.WriteLine("{n,m}     m and n are non-negative integers. Matches at least n and at most m times. For example, \"o{1,3}\" matches the first three o's in \"fooooood.\" \"o{0,1}\" is equivalent to \"o?\".");
            Console.WriteLine("[xyz]     A character set. Matches any one of the enclosed characters. For example, \"[abc]\" matches the \"a\" in \"plain\".");
            Console.WriteLine("[^xyz]    A negative character set. Matches any character not enclosed. For example, \"[^abc]\" matches the \"p\" in \"plain\".");
            Console.WriteLine("[a-z]     A range of characters. Matches any character in the specified range. For example, \"[a-z]\" matches any lowercase alphabetic character in the range \"a\" through \"z\".");
            Console.WriteLine("[^m-z]    A negative range characters. Matches any character not in the specified range. For example, \"[m-z]\" matches any character not in the range \"m\" through \"z\".");
            Console.WriteLine("\\b        Matches a word boundary, that is, the position between a word and a space. For example, \"er\\b\" matches the \"er\" in \"never\" but not the \"er\" in \"verb\".");
            Console.WriteLine("\\B        Matches a non-word boundary. \"ea*r\\B\" matches the \"ear\" in \"never early\".");
            Console.WriteLine("\\d        Matches a digit character. Equivalent to [0-9].");
            Console.WriteLine("\\D        Matches a non-digit character. Equivalent to [^0-9].");
            Console.WriteLine("\\s        Matches any white space including space, tab, form-feed, etc. Equivalent to \"[ \\f\\n\\r\\t\\v]\".");
            Console.WriteLine("\\S        Matches any nonwhite space character. Equivalent to \"[^ \\f\\n\\r\\t\\v]\".");
            Console.WriteLine("\\w        Matches any word character including underscore. Equivalent to \"[A-Za-z0-9_]\".");
            Console.WriteLine("\\W        Matches any non-word character. Equivalent to \"[^A-Za-z0-9_]\".");
        }
    }
}
