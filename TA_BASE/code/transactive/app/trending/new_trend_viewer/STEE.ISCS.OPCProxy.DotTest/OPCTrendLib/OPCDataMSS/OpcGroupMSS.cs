using System;
using Dottest.Framework;

namespace OPCTrendLib.OPCDataMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCTrendLib")]
    [MethodSpecificStubsClassAttribute("OPCTrendLib.OPCData.OpcGroup")]
    public class OpcGroupMSS
    {
        [MethodSpecificStubsMethod("OPCTrendLib.OPCData.OpcGroup.AddItems(OPCTrendLib.OPCData.OPCItemDef[],out OPCTrendLib.OPCData.OPCItemResult[])")]
        public static System.Boolean AddItemsMSS(
        OPCTrendLib.OPCData.OpcGroup target,
        OPCTrendLib.OPCData.OPCItemDef[] arrDef, out OPCTrendLib.OPCData.OPCItemResult[] arrRes)
        {
            arrRes = null; object result;
            // NOTE: out and ref parameters are not passed from a test specific stub
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, arrDef, arrRes }, out result))
            {
                string currentmethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
                if (currentmethod.Equals("TestAddOPCItem04"))
                {
                    arrRes = new OPCTrendLib.OPCData.OPCItemResult[1];
                    OPCTrendLib.OPCData.OPCItemResult resu = new OPCTrendLib.OPCData.OPCItemResult();
                    resu.Error = OPCTrendLib.OPCHeader.HRESULTS.OPC_E_UNKNOWNITEMID;
                    arrRes[0] = resu;
                }
                else if (currentmethod.Equals("TestAddOPCItem06"))
                {
                    arrRes = null;
                }
                else if (currentmethod.Equals("TestAddOPCItem08"))
                {
                    arrRes = new OPCTrendLib.OPCData.OPCItemResult[1];
                }
                else if (currentmethod.Equals("TestAddOPCItem07"))
                {
                    arrRes = new OPCTrendLib.OPCData.OPCItemResult[1];
                    OPCTrendLib.OPCData.OPCItemResult resu = new OPCTrendLib.OPCData.OPCItemResult();
                    resu.Error = OPCTrendLib.OPCHeader.HRESULTS.S_OK;
                    resu.AccessRights = OPCTrendLib.OPCDataInterface.OPCACCESSRIGHTS.OPC_WRITEABLE;
                    arrRes[0] = resu;
                }
                else
                {
                    arrRes = new OPCTrendLib.OPCData.OPCItemResult[1];
                    OPCTrendLib.OPCData.OPCItemResult resu = new OPCTrendLib.OPCData.OPCItemResult();
                    resu.Error = OPCTrendLib.OPCHeader.HRESULTS.S_OK;
                    resu.AccessRights = OPCTrendLib.OPCDataInterface.OPCACCESSRIGHTS.OPC_READABLE;
                    arrRes[0] = resu;
                }
                return true;
            }
            else
            {
                return target.AddItems(arrDef, out arrRes);
            }
        }

        [TestSpecificStub("OPCTrendLib.OPCData.OpcGroup.AddItems(OPCTrendLib.OPCData.OPCItemDef[],out OPCTrendLib.OPCData.OPCItemResult[])")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC03")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC04")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem02")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem04")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem06")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem07")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem08")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem09")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem10")]
        public static System.Boolean AddItemsTSS(
        OPCTrendLib.OPCData.OpcGroup target,
        OPCTrendLib.OPCData.OPCItemDef[] arrDef, out OPCTrendLib.OPCData.OPCItemResult[] arrRes)
        {
            arrRes = new OPCTrendLib.OPCData.OPCItemResult[1];
            return true;
        }

        [MethodSpecificStubsMethod("OPCTrendLib.OPCData.OpcGroup.Refresh2(OPCTrendLib.OPCDataInterface.OPCDATASOURCE,System.Int32,out System.Int32)")]
        public static void Refresh2MSS(
        OPCTrendLib.OPCData.OpcGroup target,
        OPCTrendLib.OPCDataInterface.OPCDATASOURCE sourceMode, System.Int32 transactionID, out System.Int32 cancelID)
        {
            cancelID = 0; object result;
            // NOTE: out and ref parameters are not passed from a test specific stub
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, sourceMode, transactionID, cancelID }, out result))
            {
                string currentmethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
                if (currentmethod.Equals("TestAddOPCItem09"))
                {
                    throw new System.Runtime.InteropServices.COMException();
                }
                else if (currentmethod.Equals("TestAddOPCItem10"))
                {
                    throw new System.Runtime.InteropServices.COMException("Error -- The RPC server is unavailable.");
                }
                return;
            }
            else
            {
                target.Refresh2(sourceMode, transactionID, out cancelID);
            }
        }

        [TestSpecificStub("OPCTrendLib.OPCData.OpcGroup.Refresh2(OPCTrendLib.OPCDataInterface.OPCDATASOURCE,System.Int32,out System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC03")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestReconnectAndAddToOPC04")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem02")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem09")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestAddOPCItem10")]
        public static void Refresh2TSS(
        OPCTrendLib.OPCData.OpcGroup target,
        OPCTrendLib.OPCDataInterface.OPCDATASOURCE sourceMode, System.Int32 transactionID, out System.Int32 cancelID)
        {
            cancelID = 1;
            return;
        }

        [MethodSpecificStubsMethod("OPCTrendLib.OPCData.OpcGroup.RemoveItems(System.Int32[],out System.Int32[])")]
        public static System.Boolean RemoveItemsMSS(
        OPCTrendLib.OPCData.OpcGroup target,
        System.Int32[] arrHSrv, out System.Int32[] arrErr)
        {
            arrErr = null; object result;
            // NOTE: out and ref parameters are not passed from a test specific stub
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, arrHSrv, arrErr }, out result))
            {
                arrErr = new Int32[1];
                string currentmethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
                if (currentmethod.Equals("TestRemoveOPCItem03"))
                {
                    throw new Exception("Error 0X40005660. The RPC server is unavailable.");
                }
                return true;
            }
            else
            {
                return target.RemoveItems(arrHSrv, out arrErr);
            }
        }

        [TestSpecificStub("OPCTrendLib.OPCData.OpcGroup.RemoveItems(System.Int32[],out System.Int32[])")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestRemoveOPCItem02")]
        [TestSpecificStubContext(typeof(STEE.ISCS.OPCProxy.DotTest.OPCServerProxyTests), "TestRemoveOPCItem03")]
        public static System.Boolean RemoveItemsTSS(
        OPCTrendLib.OPCData.OpcGroup target,
        System.Int32[] arrHSrv, out System.Int32[] arrErr)
        {
            arrErr = new Int32[1];   
             /*string currentmethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
             if (currentmethod.Equals("TestRemoveOPCItem03"))
             {
                 throw new Exception("Error 0X40005660. The RPC server is unavailable.");
             }*/
            return true;
        }

    }
}


