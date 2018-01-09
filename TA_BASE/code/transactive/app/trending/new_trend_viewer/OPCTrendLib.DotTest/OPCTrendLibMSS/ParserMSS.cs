using System;
using Dottest.Framework;
using DotTest.OPCTrendLib;
namespace OPCTrendLibMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCTrendLib")]
    [MethodSpecificStubsClassAttribute("OPCTrendLib.Parser")]
    public class ParserMSS
    {
        [MethodSpecificStubsMethod("OPCTrendLib.Parser.IsCanFollowConst(OPCTrendLib.Token)")]
        public static System.Boolean IsCanFollowConstMSSS(/*OPCTrendLib.Token*/ object token)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { token }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return (System.Boolean)Helper.CallStaticMethod(typeof(OPCTrendLib.Parser), "IsCanFollowConst", new object[] { token });
            }
        }

        [TestSpecificStub("OPCTrendLib.Parser.IsCanFollowConst(OPCTrendLib.Token)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(ParserTests), "TestErr_ParseConst01" )]
        public static System.Boolean IsCanFollowConstTSSS(/*OPCTrendLib.Token*/ object token)
        {
            //return (System.Boolean)Helper.CallStaticMethod(typeof(OPCTrendLib.Parser), "IsCanFollowConst", new object[] { token });
            return false;
        }

    }
}

