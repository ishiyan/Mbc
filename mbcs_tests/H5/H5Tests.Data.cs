using Microsoft.VisualStudio.TestTools.UnitTesting;
using Mbcs.H5;

namespace Mbcs.Tests.H5
{
    public partial class H5Tests
    {
        [TestMethod]
        public void H5_Data_DefaultMaximumReadBufferBytes_HasCorrectValue()
        {
            Assert.AreEqual(1024UL * 1024UL, Data.DefaultMaximumReadBufferBytes);
        }

        [TestMethod]
        public void H5_Data_DefaultMaximumReadBufferBytes_WhenSet_HasCorrectValue()
        {
            var value = 123UL;
            var oldValue = Data.DefaultMaximumReadBufferBytes;
            Data.DefaultMaximumReadBufferBytes = value;

            Assert.AreEqual(value, Data.DefaultMaximumReadBufferBytes);

            Data.DefaultMaximumReadBufferBytes = oldValue;
        }
    }
}
