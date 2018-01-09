using System;
using Dottest.Framework;
using TrendViewer.DotTest;
using TrendViewer.DotTest.View;

namespace STEE.ISCS.MulLanguageMSS
{
    [MethodSpecificStubsAssemblyAttribute("STEE.ISCS.MulLanguage")]
    [MethodSpecificStubsClassAttribute("STEE.ISCS.MulLanguage.StringHelper")]
    public class StringHelperMSS
    {
        [MethodSpecificStubsMethod("STEE.ISCS.MulLanguage.StringHelper.AddStringItem(System.String,System.String,System.String,System.String)")]
        public static void AddStringItemMSS(
        STEE.ISCS.MulLanguage.StringHelper target,
        System.String ID, System.String TextEnglish, System.String TextChinese, System.String Description)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, ID, TextEnglish, TextChinese, Description }, out result))
            {
                return;
            }
            else
            {
                target.AddStringItem(ID, TextEnglish, TextChinese, Description);
            }
        }

        [TestSpecificStub("STEE.ISCS.MulLanguage.StringHelper.AddStringItem(System.String,System.String,System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(LanguageHelperTests), "TestInitAllStrings02")]
        public static void AddStringItemTSS(
        STEE.ISCS.MulLanguage.StringHelper target,
        System.String ID, System.String TextEnglish, System.String TextChinese, System.String Description)
        {
            target.AddStringItem(ID, TextEnglish, TextChinese, Description);
        }

        [MethodSpecificStubsMethod("STEE.ISCS.MulLanguage.StringHelper.getStringValue(System.String,System.String)")]
        public static System.String getStringValueMSS(
        STEE.ISCS.MulLanguage.StringHelper target,
        System.String ID, System.String defaultValue)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, ID, defaultValue }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return target.getStringValue(ID, defaultValue);
            }
        }

        [TestSpecificStub("STEE.ISCS.MulLanguage.StringHelper.getStringValue(System.String,System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(FormulaDataTests), "TestEditDataPointFormula_Load02")]
        public static System.String getStringValueTSS(
        STEE.ISCS.MulLanguage.StringHelper target,
        System.String ID, System.String defaultValue)
        {
            Exception e = new Exception();
            throw e;
        }

    }
}

