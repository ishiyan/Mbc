using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Mbcs.H5
{
    /// <summary>
    /// Encapsulates a HDF5 group info.
    /// </summary>
    public sealed class GroupInfo : ObjectInfo, IDisposable
    {
        /// <summary>
        /// The datasets.
        /// </summary>
        public List<DataInfo> Datasets { get; } = new List<DataInfo>();

        /// <summary>
        /// The groups.
        /// </summary>
        public List<GroupInfo> Groups { get; } = new List<GroupInfo>();

        /// <summary>
        /// The handle allocated by GCHandle.Alloc().
        /// </summary>
        internal GCHandle Handle { get; set; }

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        /// <param name="name">The name of the group.</param>
        /// <param name="parent">The parent group info instance.</param>
        internal GroupInfo(string name, GroupInfo parent)
            : base(name, parent)
        {
        }

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        internal GroupInfo()
        {
        }

        /// <summary>
        /// Releases unmanaged resources.
        /// </summary>
        internal void ReleaseUnmanagedResources()
        {
            if (Handle.IsAllocated)
                Handle.Free();
            foreach (GroupInfo g in Groups)
            {
                g.ReleaseUnmanagedResources();
            }
        }

        /// <inheritdoc />
        public void Dispose()
        {
            ReleaseUnmanagedResources();
            GC.SuppressFinalize(this);
        }

        /// <inheritdoc />
        ~GroupInfo()
        {
            ReleaseUnmanagedResources();
        }
    }
}
