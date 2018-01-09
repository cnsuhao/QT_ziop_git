using System;
using Dottest.Framework;

namespace System.Runtime.InteropServicesMSS
{
    [MethodSpecificStubsAssemblyAttribute("mscorlib")]
    [MethodSpecificStubsClassAttribute("System.Runtime.InteropServices.Marshal")]
    public class MarshalMSS
    {
        [MethodSpecificStubsMethod("System.Runtime.InteropServices.Marshal.ReleaseComObject(System.Object)")]
        public static System.Int32 ReleaseComObjectMSSS(System.Object o)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { o }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                // return System.Runtime.InteropServices.Marshal.ReleaseComObject(o);
                return 0;
            }
        }

        [TestSpecificStub("System.Runtime.InteropServices.Marshal.ReleaseComObject(System.Object)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        /*[TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCHeader.OPCServerListTests), "TestListAllServer01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCHeader.OPCServerListTests), "TestListAllServer02")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCHeader.OPCServerListTests), "TestListAllServer03")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCHeader.OPCServerListTests), "TestListAllServerData01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestConnect03")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestAddGroup01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestAdviseIOPCShutdown01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestBrowse01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestGetPublicGroup01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestErr_GetPublicGroup01")]*/
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestErr_Disconnect02")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestErr_Remove01")]
        public static System.Int32 ReleaseComObjectTSSS(System.Object o)
        {
            throw new Exception();
        }

        [MethodSpecificStubsMethod("System.Runtime.InteropServices.Marshal.ReadInt32(System.IntPtr)")]
        public static System.Int32 ReadInt32MSSS(System.IntPtr ptr)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { ptr }, out result))
            {
                return (System.Int32)result;
            }
            else
            {
                return System.Runtime.InteropServices.Marshal.ReadInt32(ptr);
            }
        }

        [TestSpecificStub("System.Runtime.InteropServices.Marshal.ReadInt32(System.IntPtr)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestGetItemProperties01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestLookupItemIDs01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestQueryAvailableProperties01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestAddItems01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestOnReadComplete01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestOnDataChange01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestOnWriteComplete01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestRead01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestValidateItems01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcEnumItemAttributesTests), "TestNext01")]
        public static System.Int32 ReadInt32TSSS(System.IntPtr ptr)
        {
            ptr = new IntPtr((int)System.Runtime.InteropServices.VarEnum.VT_I2);

            return 0;
        }

        [MethodSpecificStubsMethod("System.Runtime.InteropServices.Marshal.GetObjectForNativeVariant(System.IntPtr)")]
        public static System.Object GetObjectForNativeVariantMSSS(System.IntPtr pSrcNativeVariant)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { pSrcNativeVariant }, out result))
            {
                return (System.Object)result;
            }
            else
            {
                return System.Runtime.InteropServices.Marshal.GetObjectForNativeVariant(pSrcNativeVariant);
            }
        }

        [TestSpecificStub("System.Runtime.InteropServices.Marshal.GetObjectForNativeVariant(System.IntPtr)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestGetItemProperties01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestOnDataChange01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestOnReadComplete01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestRead01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcEnumItemAttributesTests), "TestNext01")]
        public static System.Object GetObjectForNativeVariantTSSS(System.IntPtr pSrcNativeVariant)
        {
            return 3;
        }

        [MethodSpecificStubsMethod("System.Runtime.InteropServices.Marshal.Copy(System.IntPtr,System.Int32[],System.Int32,System.Int32)")]
        public static void CopyMSSS(System.IntPtr source, System.Int32[] destination, System.Int32 startIndex, System.Int32 length)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { source, destination, startIndex, length }, out result))
            {
                return;
            }
            else
            {
                System.Runtime.InteropServices.Marshal.Copy(source, destination, startIndex, length);
            }
        }

        [TestSpecificStub("System.Runtime.InteropServices.Marshal.Copy(System.IntPtr,System.Int32[],System.Int32,System.Int32)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestQueryAvailableLocaleIDs01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestRead02")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestSetDatatypes01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestSetClientHandles01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestRemoveItems01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestSetActiveState01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestWrite01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestWrite21")]
        public static void CopyTSSS(System.IntPtr source, System.Int32[] destination, System.Int32 startIndex, System.Int32 length)
        {
            destination = new int[1];
            destination[0] = 1;
        }

        [MethodSpecificStubsMethod("System.Runtime.InteropServices.Marshal.ReadInt16(System.IntPtr)")]
        public static System.Int16 ReadInt16MSSS(System.IntPtr ptr)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { ptr }, out result))
            {
                return (System.Int16)result;
            }
            else
            {
                return System.Runtime.InteropServices.Marshal.ReadInt16(ptr);
            }
        }

        [TestSpecificStub("System.Runtime.InteropServices.Marshal.ReadInt16(System.IntPtr)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcServerTests), "TestQueryAvailableProperties01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestAddItems01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestOnReadComplete01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestOnDataChange01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestRead01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestValidateItems01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcEnumItemAttributesTests), "TestNext01")]
        public static System.Int16 ReadInt16TSSS(System.IntPtr ptr)
        {
            //case - OnDataChange method and OnReadComplete method pvValues ptr 
            if (ptr.ToInt32() == 9999)
            {
                return 10;
            }
            ptr = new IntPtr((int)System.Runtime.InteropServices.VarEnum.VT_I2);
            return 0;
        }

        [MethodSpecificStubsMethod("System.Runtime.InteropServices.Marshal.ReadInt64(System.IntPtr)")]
        public static System.Int64 ReadInt64MSSS(System.IntPtr ptr)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { ptr }, out result))
            {
                return (System.Int64)result;
            }
            else
            {
                return System.Runtime.InteropServices.Marshal.ReadInt64(ptr);
            }
        }

        [TestSpecificStub("System.Runtime.InteropServices.Marshal.ReadInt64(System.IntPtr)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestOnDataChange01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestOnReadComplete01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestRead01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcGroupTests), "TestValidateItems01")]
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcEnumItemAttributesTests), "TestNext01")]
        public static System.Int64 ReadInt64TSSS(System.IntPtr ptr)
        {
            return 1;
        }

        [MethodSpecificStubsMethod("System.Runtime.InteropServices.Marshal.PtrToStringUni(System.IntPtr)")]
        public static System.String PtrToStringUniMSSS(System.IntPtr ptr)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { ptr }, out result))
            {
                return (System.String)result;
            }
            else
            {
                return System.Runtime.InteropServices.Marshal.PtrToStringUni(ptr);
            }
        }

        [TestSpecificStub("System.Runtime.InteropServices.Marshal.PtrToStringUni(System.IntPtr)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(DotTest.OPCTrendLib.OPCData.OpcEnumItemAttributesTests), "TestNext01")]
        public static System.String PtrToStringUniTSSS(System.IntPtr ptr)
        {
            return "AccessPath1\\Path2";
        }

    }
}

