namespace Mbcs.H5
{
    /// <summary>
    /// The file driver enumeration.
    /// </summary>
    public enum FileDriver
    {
         /// <summary>The default Section 2 POSIX driver.</summary>
         Sec2 = 0,
         /// <summary>The standard I/O driver.</summary>
         StdIo = 1,
         /// <summary>File contents are stored only in memory until the file is closed and is written to disk.</summary>
         Core = 2,
         /// <summary>The logging driver. Logs the file access information and statistics to the [filename].fapl_log file.</summary>
         Log = 3
    }
}
