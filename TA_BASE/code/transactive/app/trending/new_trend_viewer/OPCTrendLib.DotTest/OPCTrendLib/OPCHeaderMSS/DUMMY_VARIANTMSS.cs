using System;
using Dottest.Framework;
using DotTest.OPCTrendLib.OPCData;

namespace OPCTrendLib.OPCHeaderMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCTrendLib")]
    [MethodSpecificStubsClassAttribute("OPCTrendLib.OPCHeader.DUMMY_VARIANT")]
    public class DUMMY_VARIANTMSS
    {
        [MethodSpecificStubsMethod("OPCTrendLib.OPCHeader.DUMMY_VARIANT.VariantClear(System.IntPtr)")]
        public static System.Int32 VariantClearMSSS(System.IntPtr addrofvariant)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { addrofvariant }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                return OPCTrendLib.OPCHeader.DUMMY_VARIANT.VariantClear(addrofvariant);
            }
        }

        [TestSpecificStub("OPCTrendLib.OPCHeader.DUMMY_VARIANT.VariantClear(System.IntPtr)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OpcServerTests), "TestGetItemProperties01")]
        [TestSpecificStubContext(typeof(OpcGroupTests), "TestRead01")]
        [TestSpecificStubContext(typeof(OpcEnumItemAttributesTests), "TestNext01")]
        public static System.Int32 VariantClearTSSS(System.IntPtr addrofvariant)
        {
            return 1;
        }

    }
}

