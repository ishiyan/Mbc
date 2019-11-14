using System;
using System.Runtime.InteropServices;
using static Mbcs.NativeMethods.NativeMethods;

namespace Mbcs.H5
{
    /// <summary>
    /// Encapsulates the access to the instrument's data.
    /// </summary>
    public abstract class Data : IDisposable
    {
        /// <summary>
        /// The dataset handle.
        /// </summary>
        // ReSharper disable once InconsistentNaming
        protected HandleRef dataHandle;

        /// <summary>
        /// The path to the h5 file.
        /// </summary>
        public string FilePath { get; }

        /// <summary>
        /// The path to the instrument within the h5 file.
        /// </summary>
        public string InstrumentPath { get; }

        /// <summary>
        /// The path to the dataset within the h5 file.
        /// </summary>
        public string DataPath { get; }

        /// <summary>
        /// If the dataset is read-only.
        /// </summary>
        public bool IsReadOnly { get; }

        internal Data(Instrument instrument, string dataPath, IntPtr dataHandle)
        {
            FilePath = instrument.FilePath;
            InstrumentPath = instrument.InstrumentPath;
            IsReadOnly = instrument.IsReadOnly;
            DataPath = dataPath;
            this.dataHandle = new HandleRef(this, dataHandle);
        }

        /// <summary>
        /// The default maximal size in bytes of the read buffer, 0 means unlimited.
        /// </summary>
        public static ulong DefaultMaximumReadBufferBytes
        {
            get => H5DataGetDefaultMaximumReadBufferBytes();
            set => H5DataSetDefaultMaximumReadBufferBytes(value);
        }

        /// <summary>
        /// Release unmanaged resources.
        /// </summary>
        protected abstract void ReleaseUnmanagedResources();

        /// <inheritdoc />
        public void Dispose()
        {
            ReleaseUnmanagedResources();
            GC.SuppressFinalize(this);
        }

        /// <inheritdoc />
        ~Data()
        {
            ReleaseUnmanagedResources();
        }
    }
}
