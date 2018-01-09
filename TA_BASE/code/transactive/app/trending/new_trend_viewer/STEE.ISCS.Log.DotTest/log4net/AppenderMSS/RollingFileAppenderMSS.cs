using System;
using Dottest.Framework;
using STEE.ISCS.Log.DotTest;

namespace log4net.AppenderMSS
{
    [MethodSpecificStubsAssemblyAttribute("log4net")]
    [MethodSpecificStubsClassAttribute("log4net.Appender.RollingFileAppender")]
    public class RollingFileAppenderMSS
    {
        [MethodSpecificStubsMethod("log4net.Appender.RollingFileAppender.ActivateOptions()")]
        public static void ActivateOptionsMSS(
        log4net.Appender.RollingFileAppender target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.ActivateOptions();
            }
        }

        [TestSpecificStub("log4net.Appender.RollingFileAppender.ActivateOptions()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(LogHelperTests), "TestsetLogFile03")]
        public static void ActivateOptionsTSS(
        log4net.Appender.RollingFileAppender target)
        {
            return;
        }

    }
}

