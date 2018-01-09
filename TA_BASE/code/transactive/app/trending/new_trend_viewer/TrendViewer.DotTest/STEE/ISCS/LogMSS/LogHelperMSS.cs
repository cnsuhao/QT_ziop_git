using System;
using Dottest.Framework;
using TrendViewer.DotTest;
using TrendViewer.DotTest.View;

namespace STEE.ISCS.LogMSS
{
    [MethodSpecificStubsAssemblyAttribute("STEE.ISCS.Log")]
    [MethodSpecificStubsClassAttribute("STEE.ISCS.Log.LogHelper")]
    public class LogHelperMSS
    {
        [MethodSpecificStubsMethod("STEE.ISCS.Log.LogHelper.setLogFile(System.String)")]
        public static void setLogFileMSSS(System.String file)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { file }, out result))
            {
                return;
            }
            else
            {
                STEE.ISCS.Log.LogHelper.setLogFile(file);
            }
        }

        [TestSpecificStub("STEE.ISCS.Log.LogHelper.setLogFile(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(ProgramTests), "TestCreateWin32EventAndSetLogFile03")]
        public static void setLogFileTSSS(System.String file)
        {
            Exception e = new Exception();
            throw e;
        }




        [MethodSpecificStubsMethod("STEE.ISCS.Log.LogHelper.Info(System.String,System.String,System.String)")]
        public static void InfoMSSS(System.String className, System.String functionName, System.String msg)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { className, functionName, msg }, out result))
            {
                return;
            }
            else
            {
                STEE.ISCS.Log.LogHelper.Info(className, functionName, msg);
            }
        }

        [TestSpecificStub("STEE.ISCS.Log.LogHelper.Info(System.String,System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
         [TestSpecificStubContext(typeof(ProgramTests), "TestMain01" )]
        public static void InfoTSSS(System.String className, System.String functionName, System.String msg)
        {
            //STEE.ISCS.Log.LogHelper.Info(className, functionName, msg);
            Exception e = new Exception();
            throw e;
        }

    }
}

