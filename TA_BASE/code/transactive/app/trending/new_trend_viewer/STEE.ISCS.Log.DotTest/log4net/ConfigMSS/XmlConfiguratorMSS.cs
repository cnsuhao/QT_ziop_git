using System;
using Dottest.Framework;
using STEE.ISCS.Log.DotTest;

namespace log4net.ConfigMSS
{
    [MethodSpecificStubsAssemblyAttribute("log4net")]
    [MethodSpecificStubsClassAttribute("log4net.Config.XmlConfigurator")]
    public class XmlConfiguratorMSS
    {
        [MethodSpecificStubsMethod("log4net.Config.XmlConfigurator.Configure(System.IO.FileInfo)")]
        public static void ConfigureMSSS(System.IO.FileInfo configFile)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { configFile }, out result))
            {
                return;
            }
            else
            {
                log4net.Config.XmlConfigurator.Configure(configFile);
            }
        }

        [TestSpecificStub("log4net.Config.XmlConfigurator.Configure(System.IO.FileInfo)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(LogHelperTests), "TestsetLogFile04")]
        public static void ConfigureTSSS(System.IO.FileInfo configFile)
        {
            return;
        }

    }
}

