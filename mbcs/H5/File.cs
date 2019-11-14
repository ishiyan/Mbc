using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using static Mbcs.NativeMethods.NativeMethods;

namespace Mbcs.H5
{
    /// <summary>
    /// Encapsulates an access to a h5 file.
    /// </summary>
    public sealed class File : IDisposable
    {
        private HandleRef fileHandle;

        /// <summary>
        /// The path to the h5 file.
        /// </summary>
        public string FilePath { get; }

        /// <summary>
        /// If the h5 file is read-only.
        /// </summary>
        public bool IsReadOnly { get; }

        /// <summary>
        /// The h5 file driver.
        /// </summary>
        public FileDriver FileDriver { get; }

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        private File(string filePath, IntPtr fileHandle, FileDriver fileDriver, bool isReadOnly)
        {
            FilePath = filePath;
            FileDriver = fileDriver;
            IsReadOnly = isReadOnly;

            this.fileHandle = new HandleRef(this, fileHandle);
        }

        private void ReleaseUnmanagedResources()
        {
            Close();
        }

        /// <inheritdoc />
        public void Dispose()
        {
            ReleaseUnmanagedResources();
            GC.SuppressFinalize(this);
        }

        /// <inheritdoc />
        ~File()
        {
            ReleaseUnmanagedResources();
        }

        /// <summary>
        /// Flushes and closes an access to the h5 file. The instance of the class is not usable after this call.
        /// </summary>
        public void Close()
        {
            if (fileHandle.Handle != IntPtr.Zero)
            {
                H5FileClose(fileHandle.Handle);
                fileHandle = new HandleRef(this, IntPtr.Zero);
            }
        }

        /// <summary>
        /// Flushes the h5 file.
        /// </summary>
        public void Flush()
        {
            if (fileHandle.Handle != IntPtr.Zero)
                H5FileFlush(fileHandle.Handle);
        }

        /// <summary>
        /// Opens a h5 file for reading.
        /// </summary>
        /// <param name="filePath">The h5 file path to open.</param>
        /// <param name="fileDriver">The file driver to use.</param>
        /// <returns>A read-only File object.</returns>
        public static File OpenReadOnly(string filePath, FileDriver fileDriver = FileDriver.Sec2)
        {
            var handle = H5FileOpenReadOnly(filePath, fileDriver);

            return handle == IntPtr.Zero ? null : new File(filePath, handle, fileDriver, true);
        }

        /// <summary>
        /// Opens a h5 file for both reading and writing.
        /// </summary>
        /// <param name="filePath">The h5 file path to open.</param>
        /// <param name="createNonexistent">Create file if it does not exist.</param>
        /// <param name="fileDriver">The file driver to use.</param>
        /// <param name="corkTheCache">If you have memory to spare, you could "cork the cache" until you reached a suitable point to update the metadata in the file and, call H5Fflush.</param>
        /// <returns>A writable File object.</returns>
        public static File OpenReadWrite(string filePath, bool createNonexistent = true, FileDriver fileDriver = FileDriver.Sec2, bool corkTheCache = false)
        {
            var handle = H5FileOpenReadWrite(filePath, createNonexistent, fileDriver, corkTheCache);

            return handle == IntPtr.Zero ? null : new File(filePath, handle, fileDriver, false);
        }

        /// <summary>
        /// Redirects the HDF5 error messages to the .NET trace log file; otherwise, the HDF5 library writes them to the console.
        /// Should be called once at the very beginning.
        /// </summary>
        /// <param name="errorLogger">The error logging callback.</param>
        public static void InterceptErrorStack(ErrorLogger errorLogger)
        {
            H5InterceptErrorStack(errorLogger);
        }

        private static void AddDataInfo(IntPtr groupInfoHandle, DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
        {
            var gi = (GroupInfo)((GCHandle)groupInfoHandle).Target;
            gi.Datasets.Add(new DataInfo(name, gi)
            {
                ParsedDataType = dataType,
                ParsedDataKind = dataKind,
                ParsedOhlcvKind = ohlcvKind,
                ParsedScalarKind = scalarKind,
                ParsedTradeKind = tradeKind,
                ParsedQuoteKind = quoteKind,
                ParsedTimeFrame = timeFrame,
                IsValidName = isValidName,
                Path = path
            });
        }

        private static IntPtr AddGroupInfo(IntPtr groupInfoHandle, string name, string path)
        {
            var gi = (GroupInfo)((GCHandle)groupInfoHandle).Target;
            var childGroupInfo = new GroupInfo(name, gi);
            childGroupInfo.Handle = GCHandle.Alloc(childGroupInfo);
            gi.Groups.Add(childGroupInfo);
            return (IntPtr)childGroupInfo.Handle;
        }

        /// <summary>
        /// Scans a hierarchical tree structure of the content of the h5 file.
        /// </summary>
        /// <param name="filePath">The h5 file path to scan.</param>
        /// <param name="sort">Specifies whether to sort the hierarchical tree structure.</param>
        /// <returns>The root group info object.</returns>
        public static GroupInfo ContentTree(string filePath, bool sort)
        {
            var rootGroupInfo = new GroupInfo();
            rootGroupInfo.Handle = GCHandle.Alloc(rootGroupInfo);
            try
            {
                H5FileEnumerateContentTreeStatic(filePath, FileDriver.Sec2, (IntPtr)rootGroupInfo.Handle, AddDataInfo, AddGroupInfo, sort);
            }
            finally
            {
                rootGroupInfo.ReleaseUnmanagedResources();
            }

            return rootGroupInfo;
        }

        /// <summary>
        /// Scans a hierarchical tree structure of the content of the h5 file.
        /// </summary>
        /// <param name="sort">Specifies whether to sort the hierarchical tree structure.</param>
        /// <returns>The root group info object.</returns>
        public GroupInfo ContentTree(bool sort)
        {
            var rootGroupInfo = new GroupInfo();
            rootGroupInfo.Handle = GCHandle.Alloc(rootGroupInfo);
            try
            {
                if (fileHandle.Handle != IntPtr.Zero)
                    H5FileEnumerateContentTree(fileHandle.Handle, (IntPtr)rootGroupInfo.Handle, AddDataInfo, AddGroupInfo, sort);
            }
            finally
            {
                rootGroupInfo.ReleaseUnmanagedResources();
            }

            return rootGroupInfo;
        }

        /// <summary>
        /// Enumerates datasets of the h5 file.
        /// </summary>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool EnumerateDatasets(DatasetEnumerator datasetEnumerator, bool sort)
        {
            return fileHandle.Handle != IntPtr.Zero && H5FileEnumerateDatasets(fileHandle.Handle, datasetEnumerator, sort);
        }

        /// <summary>
        /// Enumerates datasets of the h5 file.
        /// </summary>
        /// <param name="list">The list to add enumeration into.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool EnumerateDatasets(List<DataInfo> list, bool sort)
        {
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind, TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(new DataInfo(name, null)
                {
                    ParsedDataType = dataType,
                    ParsedDataKind = dataKind,
                    ParsedOhlcvKind = ohlcvKind,
                    ParsedScalarKind = scalarKind,
                    ParsedTradeKind = tradeKind,
                    ParsedQuoteKind = quoteKind,
                    ParsedTimeFrame = timeFrame,
                    IsValidName = isValidName,
                    Path = path
                });
            }

            return fileHandle.Handle != IntPtr.Zero && H5FileEnumerateDatasets(fileHandle.Handle, Enumerator, sort);
        }

        /// <summary>
        /// Enumerates datasets from the specified instrument in the h5 file.
        /// </summary>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="instrumentPath">The instrument to scan.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool EnumerateDatasets(DatasetEnumerator datasetEnumerator, string instrumentPath, bool sort)
        {
            return fileHandle.Handle != IntPtr.Zero && H5FileEnumerateInstrumentDatasets(fileHandle.Handle, datasetEnumerator, instrumentPath, sort);
        }

        /// <summary>
        /// Enumerates datasets from the specified instrument in the h5 file.
        /// </summary>
        /// <param name="list">The list to add enumeration into.</param>
        /// <param name="instrumentPath">The instrument to scan.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public bool EnumerateDatasets(List<DataInfo> list, string instrumentPath, bool sort)
        {
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind,
                TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(new DataInfo(name, null)
                {
                    ParsedDataType = dataType,
                    ParsedDataKind = dataKind,
                    ParsedOhlcvKind = ohlcvKind,
                    ParsedScalarKind = scalarKind,
                    ParsedTradeKind = tradeKind,
                    ParsedQuoteKind = quoteKind,
                    ParsedTimeFrame = timeFrame,
                    IsValidName = isValidName,
                    Path = path
                });
            }

            return fileHandle.Handle != IntPtr.Zero && H5FileEnumerateInstrumentDatasets(fileHandle.Handle, Enumerator, instrumentPath, sort);
        }

        /// <summary>
        /// Enumerates datasets in the h5 file.
        /// </summary>
        /// <param name="filePath">A path to the h5 file.</param>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public static bool EnumerateDatasets(string filePath, DatasetEnumerator datasetEnumerator, bool sort)
        {
            return H5FileEnumerateDatasetsStatic(datasetEnumerator, filePath, FileDriver.Sec2, sort);
        }

        /// <summary>
        /// Enumerates datasets in the h5 file.
        /// </summary>
        /// <param name="filePath">A path to the h5 file.</param>
        /// <param name="list">The list to add enumeration into.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public static bool EnumerateDatasets(string filePath, List<DataInfo> list, bool sort)
        {
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind,
                TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(new DataInfo(name, null)
                {
                    ParsedDataType = dataType,
                    ParsedDataKind = dataKind,
                    ParsedOhlcvKind = ohlcvKind,
                    ParsedScalarKind = scalarKind,
                    ParsedTradeKind = tradeKind,
                    ParsedQuoteKind = quoteKind,
                    ParsedTimeFrame = timeFrame,
                    IsValidName = isValidName,
                    Path = path
                });
            }

            return H5FileEnumerateDatasetsStatic(Enumerator, filePath, FileDriver.Sec2, sort);
        }

        /// <summary>
        /// Enumerates datasets from the specified instrument in the h5 file.
        /// </summary>
        /// <param name="filePath">A path to the h5 file.</param>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="instrumentPath">The instrument to scan.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public static bool EnumerateDatasets(string filePath, DatasetEnumerator datasetEnumerator, string instrumentPath, bool sort)
        {
            return H5FileEnumerateInstrumentDatasetsStatic(datasetEnumerator, filePath, FileDriver.Sec2, instrumentPath, sort);
        }

        /// <summary>
        /// Enumerates datasets from the specified instrument in the h5 file.
        /// </summary>
        /// <param name="filePath">A path to the h5 file.</param>
        /// <param name="list">The list to add enumeration into.</param>
        /// <param name="instrumentPath">The instrument to scan.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        public static bool EnumerateDatasets(string filePath, List<DataInfo> list, string instrumentPath, bool sort)
        {
            void Enumerator(DataType dataType, DataKind dataKind, OhlcvKind ohlcvKind, ScalarKind scalarKind,
                TradeKind tradeKind, QuoteKind quoteKind, DataTimeFrame timeFrame, bool isValidName, string name, string path)
            {
                list.Add(new DataInfo(name, null)
                {
                    ParsedDataType = dataType,
                    ParsedDataKind = dataKind,
                    ParsedOhlcvKind = ohlcvKind,
                    ParsedScalarKind = scalarKind,
                    ParsedTradeKind = tradeKind,
                    ParsedQuoteKind = quoteKind,
                    ParsedTimeFrame = timeFrame,
                    IsValidName = isValidName,
                    Path = path
                });
            }

            return H5FileEnumerateInstrumentDatasetsStatic(Enumerator, filePath, FileDriver.Sec2, instrumentPath, sort);
        }

        /// <summary>
        /// Opens an instrument. The read-only trait is inherited from the opened file.
        /// </summary>
        /// <param name="instrumentPath">The instrument path to open.</param>
        /// <param name="createNonexistent">Create the instrument if it does not exist; works for writable repository files only.</param>
        /// <returns>An instrument object or <c>null</c> in case of error.</returns>
        public Instrument OpenInstrument(string instrumentPath, bool createNonexistent)
        {
            var handle = H5FileOpenInstrument(fileHandle.Handle, instrumentPath, createNonexistent);

            return handle == IntPtr.Zero ? null : new Instrument(this, instrumentPath, handle, IsReadOnly);
        }

        /// <summary>
        /// Deletes an object with all descending objects from the h5 file.
        /// </summary>
        /// <param name="objectPath">The object path to delete.</param>
        /// <param name="deleteEmptyParentPath">Delete the parent path if empty.</param>
        /// <returns>A boolean indicating the success of operation.</returns>
        public bool DeleteObject(string objectPath, bool deleteEmptyParentPath)
        {
            return fileHandle.Handle != IntPtr.Zero && H5FileDeleteObject(fileHandle.Handle, objectPath, deleteEmptyParentPath);
        }

        /// <summary>
        /// Moves an instrument from one path to another.
        /// </summary>
        /// <param name="instrumentPathOld">The original instrument path to move.</param>
        /// <param name="instrumentPathNew">The new moved instrument path.</param>
        /// <returns>A boolean indicating the success of operation.</returns>
        public bool MoveInstrument(string instrumentPathOld, string instrumentPathNew)
        {
            return fileHandle.Handle != IntPtr.Zero && H5FileMoveInstrument(fileHandle.Handle, instrumentPathOld, instrumentPathNew);
        }

        /// <summary>
        /// Renames an instrument. The path to the instrument remains the same, only the instrument name will be changed.
        /// </summary>
        /// <param name="instrumentPath">The instrument path.</param>
        /// <param name="instrumentNewName">The new name of the instrument.</param>
        /// <returns>A boolean indicating the success of operation.</returns>
        public bool RenameInstrument(string instrumentPath, string instrumentNewName)
        {
            return fileHandle.Handle != IntPtr.Zero && H5FileRenameInstrument(fileHandle.Handle, instrumentPath, instrumentNewName);
        }
    }
}
