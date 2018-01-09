using System;
using Dottest.Framework;

namespace SystemMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Type")]
    public class TypeMSS
    {
        [MethodSpecificStubsMethod("System.Type.GetTypeFromProgID(System.String)")]
        public static System.Type GetTypeFromProgIDMSSS(System.String progID)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { progID }, out result))
            {
                return (System.Type)result;
            }
            else
            {
                return System.Type.GetTypeFromProgID(progID);
            }
        }

        [TestSpecificStub("System.Type.GetTypeFromProgID(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestConnect02")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestConnect03")]
        public static System.Type GetTypeFromProgIDTSSS(System.String progID)
        {
            return typeof(OPCTrendLib.OPCDataInterface.IOPCServer);
        }

        [MethodSpecificStubsMethod("System.Type.get_IsValueType()")]
        public static System.Boolean get_IsValueTypeMSS(
        System.Type target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.IsValueType;
            }
        }

        [TestSpecificStub("System.Type.get_IsValueType()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.ConvertHelperTests), "TestErr_ChangeType05")]
        public static System.Boolean get_IsValueTypeTSS(
        System.Type target)
        {
            return false;
        }

        [MethodSpecificStubsMethod("System.Type.GetProperty(System.String,System.Reflection.BindingFlags,System.Reflection.Binder,System.Type,System.Type[],System.Reflection.ParameterModifier[])")]
        public static System.Reflection.PropertyInfo GetPropertyMSS(
        System.Type target,
        System.String name, System.Reflection.BindingFlags bindingAttr, System.Reflection.Binder binder, System.Type returnType, System.Type[] types, System.Reflection.ParameterModifier[] modifiers)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, name, bindingAttr, binder, returnType, types, modifiers }, out result))
            {
                return (System.Reflection.PropertyInfo)result;
            }
            else
            {
                return target.GetProperty(name, bindingAttr, binder, returnType, types, modifiers);
            }
        }

        [TestSpecificStub("System.Type.GetProperty(System.String,System.Reflection.BindingFlags,System.Reflection.Binder,System.Type,System.Type[],System.Reflection.ParameterModifier[])")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.IndexerOpTests), "TestEval04")]
        public static System.Reflection.PropertyInfo GetPropertyTSS(
        System.Type target,
        System.String name, System.Reflection.BindingFlags bindingAttr, System.Reflection.Binder binder, System.Type returnType, System.Type[] types, System.Reflection.ParameterModifier[] modifiers)
        {
            //return target.GetProperty("Items", bindingAttr, binder, null, types, null);
           // return target.GetProperty(name, bindingAttr, binder, returnType, types, modifiers);
           Type type = typeof(propertyGetClass);
           System.Reflection.PropertyInfo  propInfo = type.GetProperty("Property1");
           return propInfo;          
        }

    }


    public class propertyGetClass
    {
        private int myIntArray = 2;
        public propertyGetClass()
        {
           
        }
        public int Property1
        {
            get { return myIntArray; }
            set { myIntArray = value; }
        }
    }

}

