using System;
using Dottest.Framework;
using STEE.ISCS.Log.DotTest;

namespace System.IOMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.IO.File")]
    public class FileMSS
    {
        [MethodSpecificStubsMethod("System.IO.File.Exists(System.String)")]
        public static System.Boolean ExistsMSSS(System.String path)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { path }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return System.IO.File.Exists(path);
            }
        }

        [TestSpecificStub("System.IO.File.Exists(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(LogHelperTests), "TestsetLogFile02")]
        [TestSpecificStubContext(typeof(LogHelperTests), "TestsetLogFile04")]
        public static System.Boolean ExistsTSSS(System.String path)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestsetLogFile02"))
            {
                return true;
            }
            else //"TestsetLogFile04")]
            {
                if(path.Equals("../config/log/log.xml"))
                {
                    return true;
                }else {return false;}
            }
        }

    }
}

