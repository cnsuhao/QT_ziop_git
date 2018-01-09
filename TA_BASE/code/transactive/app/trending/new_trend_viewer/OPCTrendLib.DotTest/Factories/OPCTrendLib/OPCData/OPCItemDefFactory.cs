using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib.OPCData;
using System;
using System.Runtime.InteropServices;

namespace DotTest.Factories.OPCTrendLib.OPCData
{
    [TestFixture(), ObjectFactoryType()]
    public class OPCItemDefFactory
    {
        [ObjectFactoryMethod]
        [HashCode("/TSLmA", "+XKQ5g")]
        public static OPCItemDef CreateOPCItemDef01()
        {
            string id = string.Empty;
            bool activ = false;
            int hclt = -1;
            VarEnum vt = VarEnum.VT_NULL;
            OPCItemDef oPCItemDef = new OPCItemDef(id, activ, hclt, vt);
            return oPCItemDef;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateOPCItemDef01()
        {
            OPCItemDef oPCItemDef = CreateOPCItemDef01();
            Assert.IsNotNull(oPCItemDef);
        }

        [ObjectFactoryMethod]
        [HashCode("/TSLmA", "/ekCiA")]
        public static OPCItemDef CreateOPCItemDef02()
        {
            string id = string.Empty;
            bool activ = true;
            int hclt = 0;
            VarEnum vt = VarEnum.VT_BSTR;
            OPCItemDef oPCItemDef = new OPCItemDef(id, activ, hclt, vt);
            return oPCItemDef;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateOPCItemDef02()
        {
            OPCItemDef oPCItemDef = CreateOPCItemDef02();
            Assert.IsNotNull(oPCItemDef);
        }

        [ObjectFactoryMethod]
        [HashCode("/TSLmA", "+r+/GA")]
        public static OPCItemDef CreateOPCItemDef03()
        {
            string id = string.Empty;
            bool activ = false;
            int hclt = int.MinValue;
            VarEnum vt = VarEnum.VT_I2;
            OPCItemDef oPCItemDef = new OPCItemDef(id, activ, hclt, vt);
            return oPCItemDef;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateOPCItemDef03()
        {
            OPCItemDef oPCItemDef = CreateOPCItemDef03();
            Assert.IsNotNull(oPCItemDef);
        }

        [ObjectFactoryMethod]
        public static OPCItemDef CreateOPCItemDef04()
        {
            OPCItemDef oPCItemDef = new OPCItemDef();
            return oPCItemDef;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateOPCItemDef04()
        {
            OPCItemDef oPCItemDef = CreateOPCItemDef04();
            Assert.IsNotNull(oPCItemDef);
        }

    }
}
