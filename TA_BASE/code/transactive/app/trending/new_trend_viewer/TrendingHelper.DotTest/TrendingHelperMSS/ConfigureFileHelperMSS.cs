using System;
using Dottest.Framework;
using TrendingHelper.DotTest;

namespace TrendingHelperMSS
{
    [MethodSpecificStubsAssemblyAttribute("TrendingHelper")]
    [MethodSpecificStubsClassAttribute("TrendingHelper.ConfigureFileHelper")]
    public class ConfigureFileHelperMSS
    {
        [MethodSpecificStubsMethod("TrendingHelper.ConfigureFileHelper.GetINIDataString(System.String,System.String,System.String,System.Int32,System.String)")]
        public static System.String GetINIDataStringMSSS(System.String entryCategory, System.String entryKey, System.String entryDefault, System.Int32 entrySize, System.String entryFile)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { entryCategory, entryKey, entryDefault, entrySize, entryFile }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return TrendingHelper.ConfigureFileHelper.GetINIDataString(entryCategory, entryKey, entryDefault, entrySize, entryFile);
            }
        }

        [TestSpecificStub("TrendingHelper.ConfigureFileHelper.GetINIDataString(System.String,System.String,System.String,System.Int32,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(ConfigureFileHelperTests), "TestInit02")]
        public static System.String GetINIDataStringTSSS(System.String entryCategory, System.String entryKey, System.String entryDefault, System.Int32 entrySize, System.String entryFile)
        {
            if (entryCategory.Equals("ENCODING_CHANGE"))
            {
                return "true";
            }
            else
            {
                return TrendingHelper.ConfigureFileHelper.GetINIDataString(entryCategory, entryKey, entryDefault, entrySize, entryFile);
            }
           
        }

    }
}

