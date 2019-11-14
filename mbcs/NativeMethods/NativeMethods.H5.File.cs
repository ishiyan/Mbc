using System;
using System.Runtime.InteropServices;
using System.Security;
using Mbcs.H5;

namespace Mbcs.NativeMethods
{
    internal static partial class NativeMethods
    {
        /// <summary>
        /// Flushes and closes an access to the h5 file. The <see paramref="fileHandle"/> is not valid after this call.
        /// </summary>
        /// <param name="fileHandle">The file handle.</param>
        [DllImport(DllName, EntryPoint = "h5FileClose", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void H5FileClose(IntPtr fileHandle);

        /// <summary>
        /// Flushes the h5 file.
        /// </summary>
        /// <param name="fileHandle">The file handle.</param>
        [DllImport(DllName, EntryPoint = "h5FileFlush", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void H5FileFlush(IntPtr fileHandle);

        /// <summary>
        /// Redirects the HDF5 error messages to the caller-supplied callback function; otherwise, the HDF5 library writes them to the console.
        /// Should be called once at the very beginning.
        /// </summary>
        /// <param name="errorLogger">The error logging callback.</param>
        [DllImport(DllName, EntryPoint = "h5InterceptErrorStack", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern void H5InterceptErrorStack(ErrorLogger errorLogger);

        /// <summary>
        /// Enumerates datasets of the h5 file.
        /// </summary>
        /// <param name="fileHandle">The h5 file handle.</param>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5FileEnumerateDatasets", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5FileEnumerateDatasets(IntPtr fileHandle, DatasetEnumerator datasetEnumerator, [MarshalAs(UnmanagedType.U1)] bool sort);

        /// <summary>
        /// Enumerates datasets from the specified instrument in the h5 file.
        /// </summary>
        /// <param name="fileHandle">The h5 file handle.</param>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="instrumentPath">The instrument to scan.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5FileEnumerateInstrumentDatasets", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5FileEnumerateInstrumentDatasets(IntPtr fileHandle, DatasetEnumerator datasetEnumerator, string instrumentPath, [MarshalAs(UnmanagedType.U1)] bool sort);

        /// <summary>
        /// Enumerates datasets of the h5 file.
        /// </summary>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="filePath">A path to the h5 file.</param>
        /// <param name="fileDriver">The file driver to use.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5FileEnumerateDatasetsStatic", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5FileEnumerateDatasetsStatic(DatasetEnumerator datasetEnumerator, string filePath, FileDriver fileDriver, [MarshalAs(UnmanagedType.U1)] bool sort);

        /// <summary>
        /// Enumerates datasets from the specified instrument in the h5 file.
        /// </summary>
        /// <param name="datasetEnumerator">A callback function to enumerate datasets.</param>
        /// <param name="filePath">A path to the h5 file.</param>
        /// <param name="fileDriver">The file driver to use.</param>
        /// <param name="instrumentPath">The instrument to scan.</param>
        /// <param name="sort">Specifies whether to sort the datasets. Extra memory is allocated to sort the datasets.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5FileEnumerateInstrumentDatasetsStatic", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5FileEnumerateInstrumentDatasetsStatic(DatasetEnumerator datasetEnumerator, string filePath, FileDriver fileDriver, string instrumentPath, [MarshalAs(UnmanagedType.U1)] bool sort);

        /// <summary>
        /// Opens a h5 file for reading.
        /// </summary>
        /// <param name="filePath">A path to the h5 file.</param>
        /// <param name="fileDriver">The file driver to use.</param>
        /// <returns>A handle to a file object or <c>IntPtr.Zero</c> in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5FileOpenReadOnly", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5FileOpenReadOnly(string filePath, FileDriver fileDriver);

        /// <summary>
        /// Opens a h5 file for both reading and writing.
        /// </summary>
        /// <param name="filePath">A path to the h5 file.</param>
        /// <param name="createNonexistent">Create file if it does not exist.</param>
        /// <param name="fileDriver">The file driver to use.</param>
        /// <param name="corkTheCache">If you have memory to spare, you could "cork the cache" until you reached a suitable point to update the metadata in the file, and call flush.</param>
        /// <returns>A handle to a file object or <c>IntPtr.Zero</c> in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5FileOpenReadWrite", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5FileOpenReadWrite(string filePath, [MarshalAs(UnmanagedType.U1)] bool createNonexistent, FileDriver fileDriver, [MarshalAs(UnmanagedType.U1)] bool corkTheCache);

        /// <summary>
        /// Opens an instrument.
        /// </summary>
        /// <param name="fileHandle">The h5 file handle.</param>
        /// <param name="instrumentPath">The instrument path to open.</param>
        /// <param name="createNonexistent">Create the instrument if it does not exist; works for writable h5 files only.</param>
        /// <returns>A handle to a instrument object or <c>IntPtr.Zero</c> in case of error.</returns>
        [DllImport(DllName, EntryPoint = "h5FileOpenInstrument", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [SuppressUnmanagedCodeSecurity]
        public static extern IntPtr H5FileOpenInstrument(IntPtr fileHandle, string instrumentPath, [MarshalAs(UnmanagedType.U1)] bool createNonexistent);

        /// <summary>
        /// Deletes an object with all descending objects from the h5 file.
        /// </summary>
        /// <param name="fileHandle">The h5 file handle.</param>
        /// <param name="objectPath">The object path to delete.</param>
        /// <param name="deleteEmptyParentPath">Delete the parent path if empty.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5FileDeleteObject", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5FileDeleteObject(IntPtr fileHandle, string objectPath, [MarshalAs(UnmanagedType.U1)] bool deleteEmptyParentPath);

        /// <summary>
        /// Moves an instrument from one path to another within the same h5 file.
        /// </summary>
        /// <param name="fileHandle">The h5 file handle.</param>
        /// <param name="instrumentPathOld">The old instrument path.</param>
        /// <param name="instrumentPathNew">The new moved instrument path.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5FileMoveInstrument", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5FileMoveInstrument(IntPtr fileHandle, string instrumentPathOld, string instrumentPathNew);

        /// <summary>
        /// Renames an instrument. The path to the instrument remains the same, only the name will be changed.
        /// </summary>
        /// <param name="fileHandle">The h5 file handle.</param>
        /// <param name="instrumentPath">The instrument path including the name.</param>
        /// <param name="instrumentNewName">The new name of the instrument.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5FileRenameInstrument", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5FileRenameInstrument(IntPtr fileHandle, string instrumentPath, string instrumentNewName);

        /// <summary>
        /// Scans a hierarchical tree structure of the content of the opened h5 file.
        /// </summary>
        /// <param name="fileHandle">The h5 file handle.</param>
        /// <param name="groupInfoHandle">The root group info handle.</param>
        /// <param name="addDataInfo">Callback function to add child data info to the group info.</param>
        /// <param name="addGroupInfo">Callback function to add child group info to the group info.</param>
        /// <param name="sort">Specifies whether to sort the hierarchical tree structure.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5FileEnumerateContentTree", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5FileEnumerateContentTree(IntPtr fileHandle, IntPtr groupInfoHandle, GroupInfoAddDataInfo addDataInfo, GroupInfoAddGroupInfo addGroupInfo, bool sort);

        /// <summary>
        /// Scans a hierarchical tree structure of the content of the opened h5 file.
        /// </summary>
        /// <param name="filePath">A path to the h5 file.</param>
        /// <param name="fileDriver">The file driver to use.</param>
        /// <param name="groupInfoHandle">The root group info handle.</param>
        /// <param name="addDataInfo">Callback function to add child data info to the group info.</param>
        /// <param name="addGroupInfo">Callback function to add child group info to the group info.</param>
        /// <param name="sort">Specifies whether to sort the hierarchical tree structure.</param>
        /// <returns>A boolean indicating the success of the operation.</returns>
        [DllImport(DllName, EntryPoint = "h5FileEnumerateContentTreeStatic", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        [SuppressUnmanagedCodeSecurity]
        public static extern bool H5FileEnumerateContentTreeStatic(string filePath, FileDriver fileDriver, IntPtr groupInfoHandle, GroupInfoAddDataInfo addDataInfo, GroupInfoAddGroupInfo addGroupInfo, bool sort);
    }
}
