using System;
using Dottest.Framework;

namespace System.Windows.FormsMSS
{
    [MethodSpecificStubsAssemblyAttribute("System.Windows.Forms")]
    [MethodSpecificStubsClassAttribute("System.Windows.Forms.Control")]
    public class ControlMSS
    {
        [MethodSpecificStubsMethod("System.Windows.Forms.Control.CreateGraphics()")]
        public static System.Drawing.Graphics CreateGraphicsMSS(
        System.Windows.Forms.Control target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Drawing.Graphics)result;
            }
            else
            {
                return target.CreateGraphics();
            }
        }

        [TestSpecificStub("System.Windows.Forms.Control.CreateGraphics()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        // [TestSpecificStubContext(typeof(YourTestFixture), "NameOfYourTest" )]
        public static System.Drawing.Graphics CreateGraphicsTSS(
        System.Windows.Forms.Control target)
        {
            return null;
        }

    }
}

