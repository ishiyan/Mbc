namespace Mbcs.H5
{
    /// <summary>
    /// The duplicate time tick policy enumeration.
    /// </summary>
    public enum DuplicateTimeTicks
    {
        /// <summary>Fail the execution.</summary>
        Fail = 0,

        /// <summary>Drop the sample.</summary>
        Skip = 1,

        /// <summary>Update the sample.</summary>
        Update = 2
    }
}
