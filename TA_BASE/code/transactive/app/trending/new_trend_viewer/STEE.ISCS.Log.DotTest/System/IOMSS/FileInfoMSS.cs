using System;
using Dottest.Framework;
using STEE.ISCS.Log.DotTest;

namespace System.IOMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.IO.FileInfo")]
    public class FileInfoMSS
    {
        [MethodSpecificStubsMethod("System.IO.FileInfo.FileInfo(System.String)")]
        public static System.IO.FileInfo FileInfo_ctor_MSS(System.String fileName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { fileName }, out result))
            {
                return (System.IO.FileInfo)result;
            }
            else
            {
                return new System.IO.FileInfo(fileName);
            }
        }

        [TestSpecificStub("System.IO.FileInfo.FileInfo(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
         [TestSpecificStubContext(typeof(LogHelperTests), "TestsetLogFile03" )]
        public static System.IO.FileInfo FileInfo_ctor_TSS(System.String fileName)
        {
            Object obj = new Object();
            System.IO.FileInfo info = (System.IO.FileInfo)(obj);
            return info;
        }

    }
}

