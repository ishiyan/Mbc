using System.Runtime.InteropServices;

namespace Mbcs.H5
{
    /// <summary>
    /// Encapsulates a HDF5 object info.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public class ObjectInfo
    {
        /// <summary>
        /// The name of the object.
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// The full path to the object.
        /// </summary>
        public string Path { get; internal set; }

        /// <summary>
        /// The parent group.
        /// </summary>
        public GroupInfo Parent { get; }

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        /// <param name="name">The name of the object.</param>
        /// <param name="parent">The parent group info instance.</param>
        internal ObjectInfo(string name, GroupInfo parent)
        {
            Name = name;
            Path = parent == null ? name : string.Concat(parent.Path, "/", name);
            Parent = parent;
        }

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        internal ObjectInfo()
        {
            Name = "/";
            Path = string.Empty;
            Parent = null;
        }
    }
}
