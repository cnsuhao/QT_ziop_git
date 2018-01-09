using System;
using Dottest.Framework;
using TrendViewer.DotTest;

namespace TrendViewerMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendViewer")]
    [MethodSpecificStubsClassAttribute("TrendViewer.LanguageHelper")]
    public class LanguageHelperMSS
    {
        [MethodSpecificStubsMethod("TrendViewer.LanguageHelper.InitAllStrings()")]
        public static void InitAllStringsMSSS()
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { }, out result))
            {
                return;
            }
            else
            {
                Helper.CallStaticMethod("TrendViewer.LanguageHelper", "InitAllStrings", new object[] { });
            }
        }

        [TestSpecificStub("TrendViewer.LanguageHelper.InitAllStrings()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(ProgramTests), "TestMain02")]
        public static void InitAllStringsTSSS()
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

