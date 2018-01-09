using System;
using Dottest.Framework;
using TrendViewer.DotTest.Common;
using TrendViewer.DotTest.View;

namespace SystemMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.String")]
    public class StringMSS
    {
        [MethodSpecificStubsMethod("System.String.Split(System.String[],System.StringSplitOptions)")]
        public static System.String[] SplitMSS(
        System.String target,
        System.String[] separator, System.StringSplitOptions options)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, separator, options }, out result))
            {
                return (System.String[])result;
            }
            else
            {
                return target.Split(separator, options);
            }
        }

        [TestSpecificStub("System.String.Split(System.String[],System.StringSplitOptions)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendingHelperTests), "TestCheckValidVariables02")]
        [TestSpecificStubContext(typeof(FormulaDataTests), "TestCheckValidVariables02")]
        public static System.String[] SplitTSS(
        System.String target,
        System.String[] separator, System.StringSplitOptions options)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

