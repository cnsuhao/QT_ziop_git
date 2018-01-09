using System;
using Dottest.Framework;

namespace STEE.ISCS.LogMSS
{
    [MethodSpecificStubsAssemblyAttribute("STEE.ISCS.Log")]
    [MethodSpecificStubsClassAttribute("STEE.ISCS.Log.LogHelper")]
    public class LogHelperMSS
    {
        [MethodSpecificStubsMethod("STEE.ISCS.Log.LogHelper.Error(System.String,System.String,System.Exception)")]
        public static void ErrorMSSS(System.String className, System.String functionName, System.Exception e)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { className, functionName, e }, out result))
            {
                return;
            }
            else
            {
               // STEE.ISCS.Log.LogHelper.Error(className, functionName, e);
                return;
            }
        }

        [TestSpecificStub("STEE.ISCS.Log.LogHelper.Error(System.String,System.String,System.Exception)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static void ErrorTSSS(System.String className, System.String functionName, System.Exception e)
        {
            STEE.ISCS.Log.LogHelper.Error(className, functionName, e);
        }

    }
}

