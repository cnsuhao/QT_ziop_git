using System;
using Dottest.Framework;
using STEE.ISCS.Log.DotTest;

namespace System.ReflectionMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Reflection.Assembly")]
    public class AssemblyMSS
    {
        [MethodSpecificStubsMethod("System.Reflection.Assembly.GetEntryAssembly()")]
        public static System.Reflection.Assembly GetEntryAssemblyMSSS()
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { }, out result))
            {
                return (System.Reflection.Assembly)result;
            }
            else
            {
                // return System.Reflection.Assembly.GetEntryAssembly();
                System.Reflection.Assembly a = System.Reflection.Assembly.GetEntryAssembly();
                return a;
            }
        }

        [TestSpecificStub("System.Reflection.Assembly.GetEntryAssembly()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(LogHelperTests), "TestsetLogFile01")]
        public static System.Reflection.Assembly GetEntryAssemblyTSSS()
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;

            //return System.Reflection.Assembly.GetEntryAssembly();
            Object obj = new Object();
            System.Reflection.Assembly ret = (System.Reflection.Assembly)(obj);  //this cast is not correct, will throw exception
            return ret;



        }

        [MethodSpecificStubsMethod("System.Reflection.Assembly.GetName()")]
        public static System.Reflection.AssemblyName GetNameMSS(
        System.Reflection.Assembly target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Reflection.AssemblyName)result;
            }
            else
            {
                return target.GetName();
            }
        }

        [TestSpecificStub("System.Reflection.Assembly.GetName()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(LogHelperTests), "TestsetLogFile02")]
        public static System.Reflection.AssemblyName GetNameTSS(
        System.Reflection.Assembly target)
        {
            return new System.Reflection.AssemblyName("");
        }

    }
}

