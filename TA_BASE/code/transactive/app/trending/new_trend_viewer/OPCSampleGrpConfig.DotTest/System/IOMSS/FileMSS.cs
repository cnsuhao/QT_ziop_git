using System;
using Dottest.Framework;

namespace System.IOMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.IO.File")]
    public class FileMSS
    {
        [MethodSpecificStubsMethod("System.IO.File.AppendText(System.String)")]
        public static System.IO.StreamWriter AppendTextMSSS(System.String path)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { path }, out result))
            {
                return (System.IO.StreamWriter)result;
            }
            else
            {
                return System.IO.File.AppendText(path);
            }
        }

        [TestSpecificStub("System.IO.File.AppendText(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCDataSelectorControllerTests), "TestWriteEventLog02")]
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Controller.OPCSampleGrpConfigStartControllerTests), "TestWriteEventLog02")]
        public static System.IO.StreamWriter AppendTextTSSS(System.String path)
        {
            throw new Exception();
        }

    }
}

