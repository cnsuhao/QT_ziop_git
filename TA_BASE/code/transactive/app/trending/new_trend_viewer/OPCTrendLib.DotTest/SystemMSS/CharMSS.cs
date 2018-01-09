using System;
using Dottest.Framework;

namespace SystemMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Char")]
    public class CharMSS
    {
        [MethodSpecificStubsMethod("System.Char.GetUnicodeCategory(System.Char)")]
        public static System.Globalization.UnicodeCategory GetUnicodeCategoryMSSS(System.Char c)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { c }, out result))
            {
                return (System.Globalization.UnicodeCategory)result;
            }
            else
            {
                return System.Char.GetUnicodeCategory(c);
            }
        }

        [TestSpecificStub("System.Char.GetUnicodeCategory(System.Char)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ScannerTests), "TestIsIdentifierStartChar12")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ScannerTests), "TestIsIdentifierStartChar121")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ScannerTests), "TestIsIdentifierPartChar12")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ScannerTests), "TestIsIdentifierPartChar121")]
        public static System.Globalization.UnicodeCategory GetUnicodeCategoryTSSS(System.Char c)
        {
            string currentMethodName = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentMethodName.Equals("TestIsIdentifierStartChar121") ||
                currentMethodName.Equals("TestIsIdentifierPartChar121"))
            {
                return System.Globalization.UnicodeCategory.ConnectorPunctuation;
            }
            return System.Globalization.UnicodeCategory.LetterNumber;
        }

    }
}

