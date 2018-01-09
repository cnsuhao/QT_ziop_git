using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace STEE.ISCS.OPCProxyMSS
{
    [MethodSpecificStubsAssemblyAttribute("STEE.ISCS.OPCProxy")]
    [MethodSpecificStubsClassAttribute("STEE.ISCS.OPCProxy.OPCServerProxy")]
    public class OPCServerProxyMSS
    {
        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.IsOPCServerConnected()")]
        public static System.Boolean IsOPCServerConnectedMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.IsOPCServerConnected();
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.IsOPCServerConnected()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestInitializeTrendViewer01")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestUpdateChartForRealTimeOrMixedMode01")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestUpdateChartForRealTimeOrMixedMode03")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestUpdateChartForRealTimeOrMixedMode02")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartRealTimeOrMixedModeCharting02")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartRealTimeOrMixedModeCharting03")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartRealTimeOrMixedModeCharting04")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartRealTimeOrMixedModeCharting05")]
        public static System.Boolean IsOPCServerConnectedTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestInitializeTrendViewer01"))
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.AddOPCItem(System.String)")]
        public static System.Boolean AddOPCItemMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String opcID)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, opcID }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.AddOPCItem(opcID);
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.AddOPCItem(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestAddDPToOPCServer02")]
        public static System.Boolean AddOPCItemTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String opcID)
        {
            Exception e = new Exception();
            throw e;
        }



        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.GetDataItemValue(System.String)")]
        public static System.Object GetDataItemValueMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String itemID)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, itemID }, out result))
            {
                return (System.Object)result;
            }
            else
            {
                return target.GetDataItemValue(itemID);
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.GetDataItemValue(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestGetLatestDataPointValue01")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestGetLatestDataPointValue02")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestGetLatestDataPointValue03")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestGetLatestDataPointValue04")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestGetLatestDataPointValue07")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestUpdateChartForRealTimeOrMixedMode01")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestUpdateChartForRealTimeOrMixedMode02")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestUpdateChartForRealTimeOrMixedMode03")]
        public static System.Object GetDataItemValueTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target,
        System.String itemID)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (name.Equals("TestGetLatestDataPointValue01"))
            {
                return null;
            }
            if (name.Equals("TestGetLatestDataPointValue02") 
                || name.Equals("TestUpdateChartForRealTimeOrMixedMode01")
                || name.Equals("TestUpdateChartForRealTimeOrMixedMode03"))
            {
                Double obj = new double();
                obj = 1.1;
                return obj;
            }
            if (name.Equals("TestUpdateChartForRealTimeOrMixedMode02"))
            {
                Exception e = new Exception();
                throw e;
            }
            if (name.Equals("TestGetLatestDataPointValue03"))
            {
                Boolean b = new Boolean();
                b = false;
                return b;
            }
            if (name.Equals("TestGetLatestDataPointValue07"))
            {
                Boolean b = new Boolean();
                b = true;
                return b;
            }
            if (name.Equals("TestGetLatestDataPointValue04"))
            {
                return new DateTime(2012, 2, 1);
            }
            return target.GetDataItemValue(itemID);
        }

        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.ReleaseOPC()")]
        public static void ReleaseOPCMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return;
            }
            else
            {
                target.ReleaseOPC();
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.ReleaseOPC()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestTrendView_FormClosed02")]
        public static void ReleaseOPCTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            Exception e = new Exception();
            throw e;
        }

        [MethodSpecificStubsMethod("STEE.ISCS.OPCProxy.OPCServerProxy.RemoveAllOPCItem()")]
        public static System.Boolean RemoveAllOPCItemMSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target }, out result))
            {
                return (System.Boolean)result;
            }
            else
            {
                return target.RemoveAllOPCItem();
            }
        }

        [TestSpecificStub("STEE.ISCS.OPCProxy.OPCServerProxy.RemoveAllOPCItem()")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartRealTimeOrMixedModeCharting02")] 
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartRealTimeOrMixedModeCharting03" )]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartRealTimeOrMixedModeCharting04" )]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestStartRealTimeOrMixedModeCharting05")]
        public static System.Boolean RemoveAllOPCItemTSS(
        STEE.ISCS.OPCProxy.OPCServerProxy target)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if(name.Equals("TestStartRealTimeOrMixedModeCharting03"))
            {
                System.Runtime.InteropServices.COMException e = new System.Runtime.InteropServices.COMException();
                throw e;
                //this exception didn't caught exactly. But caught(Exception e)
            }
            else if (name.Equals("TestStartRealTimeOrMixedModeCharting04"))
            {
                Exception e = new Exception();
                throw e;
            }else{
                return true;
            }
        }

    }
}

