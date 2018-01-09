using System;
using Dottest.Framework;
using DotTest.OPCTrendLib;

namespace OPCTrendLibMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCTrendLib")]
    [MethodSpecificStubsClassAttribute("OPCTrendLib.Scanner")]
    public class ScannerMSS
    {
        [MethodSpecificStubsMethod("OPCTrendLib.Scanner.IsIdentifierStartChar(System.Char)")]
        public static System.Boolean IsIdentifierStartCharMSS(
            /*OPCTrendLib.Scanner*/ object target,
        System.Char c)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, c }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return (System.Boolean)Helper.CallMethod(target, "IsIdentifierStartChar", new object[] { c });
            }
        }

        [TestSpecificStub("OPCTrendLib.Scanner.IsIdentifierStartChar(System.Char)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(ScannerTests), "TestIsIdentifierPartChar12")]
        [TestSpecificStubContext(typeof(ScannerTests), "TestIsIdentifierPartChar121")]
        public static System.Boolean IsIdentifierStartCharTSS(
            /*OPCTrendLib.Scanner*/ object target,
        System.Char c)
        {
            //return (System.Boolean)Helper.CallMethod(target, "IsIdentifierStartChar", new object[] { c });
            return false;
        }

    }
}

