using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib.OPCData;
using OPCTrendLib.OPCHeader;
using OPCTrendLib.OPCDataInterface;


namespace DotTest.OPCTrendLib.OPCData
{
    [TestFixture()]
    public class OpcGroupTests
    {
        OpcGroup group  = null;

        [SetUp]
        public void TestFixtureSetUpMe()
        {
            OPCServerComClass opcServerComObj = new OPCServerComClass();
            IOPCServer opcServerinter = (IOPCServer)opcServerComObj;
            group = new OpcGroup(ref opcServerinter, false, "Grp1", true, 500);
              string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
              if (currentTestMethod.Equals("TestErr_internalAdd02") ||
                  currentTestMethod.Equals("TestErr_internalAdd01"))
              {
                  return;
              }
              group.internalAdd(null, null, 0);           
        }

        [TearDown]
        public void TestFixtureTearDownMe()
        {
            group = null;
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_AddItems01()
        {
            //Testing ---            
            OPCItemDef[] opcitems = new OPCItemDef[1];
            OPCItemDef opcitem = new OPCItemDef("DP1", true, 1, System.Runtime.InteropServices.VarEnum.VT_EMPTY);
            opcitems[0] = opcitem;
            OPCItemResult[] result;
            //Test Procedure Call
            bool b = group.AddItems(opcitems, out result);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_AddItems02()
        {
            //Testing ---  interface method return error code   
            OPCItemDef[] opcitems = new OPCItemDef[1];
            OPCItemDef opcitem = new OPCItemDef("DP1", true, 1, System.Runtime.InteropServices.VarEnum.VT_EMPTY);
            opcitems[0] = opcitem;
            OPCItemResult[] result;
            //Test Procedure Call
            bool b = group.AddItems(opcitems, out result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestAddItems01()
        {
            //Testing ---            
            OPCItemDef[] opcitems = new OPCItemDef[1];
            OPCItemDef opcitem = new OPCItemDef("DP1", true, 1, System.Runtime.InteropServices.VarEnum.VT_EMPTY);
            opcitems[0] = opcitem;
            OPCItemResult[] result;
            //Test Procedure Call
            bool b = group.AddItems(opcitems, out result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestAddItems02()
        {
            //Testing ---    BLob OPC Item       
            OPCItemDef[] opcitems = new OPCItemDef[1];
            OPCItemDef opcitem = new OPCItemDef("DP1", true, 1, System.Runtime.InteropServices.VarEnum.VT_EMPTY);
            byte[] buffer = new byte[100];
            opcitem.Blob = buffer;
            opcitems[0] = opcitem;
            OPCItemResult[] result;
            //Test Procedure Call
            bool b = group.AddItems(opcitems, out result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestAdviseIOPCDataCallback01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            //Test Procedure Call
            groupAccessor.Call("AdviseIOPCDataCallback");           
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCancel201()
        { 
            //Test Procedure Call
            group.Cancel2(1);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateAttrEnumerator01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            //Test Procedure Call
            OpcEnumItemAttributes enumItem = group.CreateAttrEnumerator();

            //
            enumItem = null;
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_CreateAttrEnumerator01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            //Test Procedure Call
            OpcEnumItemAttributes enumItem = group.CreateAttrEnumerator();

            //
            enumItem = null;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_CreateAttrEnumerator02()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            //Test Procedure Call
            OpcEnumItemAttributes enumItem = group.CreateAttrEnumerator();

            //
            enumItem = null;
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_DeletePublic01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            //Test Procedure Call
            group.DeletePublic(true);
        }

        [ExpectedException(typeof(System.InvalidCastException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_DeletePublic02()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            OPCGroupState grpState = new OPCGroupState();
            grpState.Public = true;
            groupAccessor.SetField("state", grpState);
            groupAccessor.SetField("ifServer", null);
            //Test Procedure Call
            group.DeletePublic(true);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestDeletePublic01()
        {
            //Testing ---            
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            OPCGroupState grpState = new OPCGroupState();
            grpState.Public = true;
            groupAccessor.SetField("state", grpState);
            //Test Procedure Call
            group.DeletePublic(true);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestGetEnable02()
        {
            //Testing ---
             Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            bool outResult;
            //Test Procedure Call
            group.GetEnable(out outResult);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void Testgetinterfaces01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            //Test Procedure Call
            groupAccessor.Call("getinterfaces");
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestGetStates01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            //Test Procedure Call
            group.GetStates();
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestMoveToPublic01()
        {
             Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            //Test Procedure Call
            group.MoveToPublic();
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_MoveToPublic01()
        {
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            OPCGroupState grpState = new OPCGroupState();
            grpState.Public = true;
            groupAccessor.SetField("state", grpState);
            //Test Procedure Call
            group.MoveToPublic();
        }


        [ExpectedException(typeof(System.InvalidCastException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_MoveToPublic02()
        {
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            groupAccessor.SetField("ifMgt", null);
            //Test Procedure Call
            group.MoveToPublic();
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestOnCancelComplete01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            group.CancelCompleted += new CancelCompleteEventHandler(group_CancelCompleted);
            //Test Procedure Call
            ((IOPCDataCallback)group).OnCancelComplete(2,1);
        }

        void group_CancelCompleted(object sender, CancelCompleteEventArgs e)
        {
            
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestOnDataChange01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            System.IntPtr phClientItems = new System.IntPtr(1);
            System.IntPtr pvValues = new System.IntPtr(9999);
            System.IntPtr pwQualities = new System.IntPtr(1);
            System.IntPtr pftTimeStamps= new System.IntPtr(1);
            System.IntPtr ppErrors= new System.IntPtr(1);
            group.DataChanged += new DataChangeEventHandler(group_DataChanged);
            //Test Procedure Call
            ((IOPCDataCallback)group).OnDataChange(2, 1, 1, 2, 1, phClientItems, pvValues,pwQualities,pftTimeStamps,ppErrors);
        }

        void group_DataChanged(object sender, DataChangeEventArgs e)
        {
            
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestOnReadComplete01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            System.IntPtr phClientItems = new System.IntPtr(1);
            System.IntPtr pvValues = new System.IntPtr(9999);
            System.IntPtr pwQualities = new System.IntPtr(1);
            System.IntPtr pftTimeStamps = new System.IntPtr(1);
            System.IntPtr ppErrors = new System.IntPtr(1);
            group.ReadCompleted += new ReadCompleteEventHandler(group_ReadCompleted);
            //Test Procedure Call
            ((IOPCDataCallback)group).OnReadComplete(2, 1, 1, 2, 1, phClientItems, pvValues, pwQualities, pftTimeStamps, ppErrors);
        }

        void group_ReadCompleted(object sender, ReadCompleteEventArgs e)
        {
            
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestOnWriteComplete01()
        {
            //Testing ---
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            System.IntPtr phClientItems = new System.IntPtr(1);
            System.IntPtr ppErrors = new System.IntPtr(1);
            group.WriteCompleted += new WriteCompleteEventHandler(group_WriteCompleted);
            //Test Procedure Call
            ((IOPCDataCallback)group).OnWriteComplete(2, 1, 1, 1, phClientItems, ppErrors);
        }

        void group_WriteCompleted(object sender, WriteCompleteEventArgs e)
        {
            
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestQualityToString01()
        {
            //Testing ---            
            //Test Procedure Call
            string result = OpcGroup.QualityToString(253);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Remove01()
        {
            //Testing ---            
            //Test Procedure Call
            group.Remove(true);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Read01()
        {
            //Testing ---   out parameters are null
            int[] arrHSrv = new int[1];
            OPCItemState[] arrStat = null;
            //Test Procedure Call
            bool b = group.Read(OPCDATASOURCE.OPC_DS_CACHE, arrHSrv, out arrStat);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Read02()
        {
            //Testing ---   interface Read method return error code
            int[] arrHSrv = new int[1];
            OPCItemState[] arrStat = null;
            //Test Procedure Call
            bool b = group.Read(OPCDATASOURCE.OPC_DS_CACHE, arrHSrv, out arrStat);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestRead01()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            OPCItemState[] arrStat = null;
            //Test Procedure Call
            bool b = group.Read(OPCDATASOURCE.OPC_DS_CACHE, arrHSrv, out arrStat);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestRead11()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            OPCItemState[] arrStat = null;
            //Test Procedure Call
            bool b = group.Read(OPCDATASOURCE.OPC_DS_CACHE, arrHSrv, out arrStat);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestRead02()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            OPCItemState[] arrStat = null;
            int cancelId = 0;
            int[] arrErr = new int[1];
            //Test Procedure Call
            bool b = group.Read(arrHSrv, 1, out cancelId, out arrErr);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Read20()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            OPCItemState[] arrStat = null;
            int cancelId = 0;
            int[] arrErr = new int[1];
            //Test Procedure Call
            bool b = group.Read(arrHSrv, 1, out cancelId, out arrErr);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestRefresh201()
        {
            //Testing ---   
            int cancelId = 0;
            //Test Procedure Call
            group.Refresh2(OPCDATASOURCE.OPC_DS_CACHE, 1, out cancelId);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestRemoveItems01()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.RemoveItems(arrHSrv, out arrErr);
        }

        [ExpectedException(typeof(System.OutOfMemoryException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_RemoveItems01()
        {
            //Testing ---   interface method return error code
            int[] arrHSrv = new int[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.RemoveItems(arrHSrv, out arrErr);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestSetActiveState01()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.SetActiveState(arrHSrv, true, out arrErr);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_SetActiveState01()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.SetActiveState(arrHSrv, true, out arrErr);
        }



        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestSetClientHandles01()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            int[] arrHClt = new int[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.SetClientHandles(arrHSrv, arrHClt, out arrErr);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_SetClientHandles01()
        {
            //Testing ---   
            int[] arrHSrv = new int[2];
            int[] arrHClt = new int[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.SetClientHandles(arrHSrv, arrHClt, out arrErr);
        }


        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_SetClientHandles02()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            int[] arrHClt = new int[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.SetClientHandles(arrHSrv, arrHClt, out arrErr);
        }


        [ExpectedException(typeof(System.InvalidCastException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_internalAdd01()
        {
            //Testing ---   
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            OPCGroupState grpState = new OPCGroupState();
            grpState.Public = true;
            groupAccessor.SetField("state", grpState);
            groupAccessor.SetField("ifServer", null);
            //Test Procedure Call
            groupAccessor.Call("internalAdd", null, null, 0);
        }

        [ExpectedException(typeof(System.InvalidCastException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_internalAdd02()
        {
            //Testing ---   
            Accessor groupAccessor = ReflectionAccessor.Wrap(group);
            //Test Procedure Call
            groupAccessor.Call("internalAdd", null, null, 0);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestSetDatatypes01()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            System.Runtime.InteropServices.VarEnum[] arrVT = new System.Runtime.InteropServices.VarEnum[1];
            arrVT[0] = System.Runtime.InteropServices.VarEnum.VT_EMPTY;
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.SetDatatypes(arrHSrv, arrVT, out arrErr);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_SetDatatypes01()
        {
            //Testing ---   
            int[] arrHSrv = new int[2];
            System.Runtime.InteropServices.VarEnum[] arrVT = new System.Runtime.InteropServices.VarEnum[1];
            arrVT[0] = System.Runtime.InteropServices.VarEnum.VT_EMPTY;
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.SetDatatypes(arrHSrv, arrVT, out arrErr);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_SetDatatypes02()
        {
            //Testing ---   
            int[] arrHSrv = new int[1];
            System.Runtime.InteropServices.VarEnum[] arrVT = new System.Runtime.InteropServices.VarEnum[1];
            arrVT[0] = System.Runtime.InteropServices.VarEnum.VT_EMPTY;
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.SetDatatypes(arrHSrv, arrVT, out arrErr);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestSetEnable01()
        {
            //Testing ---   
            //Test Procedure Call
            group.SetEnable(true);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestSetName01()
        {
            //Testing ---   
            //Test Procedure Call
            group.SetName("Grpname2");
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestValidateItems01()
        {
            //Testing --- 
            OPCItemDef[] arrDef = new OPCItemDef[1];
            OPCItemDef opcDef = new OPCItemDef("DP1", true, 1, System.Runtime.InteropServices.VarEnum.VT_EMPTY);
            arrDef[0] = opcDef;  
            bool blobUpd = false;
            OPCItemResult[] arrRes = null;
            //Test Procedure Call
            bool b = group.ValidateItems(arrDef, blobUpd, out arrRes);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestValidateItems02()
        {
            //Testing --- Blob OPCItem
            OPCItemDef[] arrDef = new OPCItemDef[1];
            OPCItemDef opcDef = new OPCItemDef("DP1", true, 1, System.Runtime.InteropServices.VarEnum.VT_EMPTY);
            opcDef.Blob = new byte[100];
            arrDef[0] = opcDef;
            bool blobUpd = false;
            OPCItemResult[] arrRes = null;
            //Test Procedure Call
            bool b = group.ValidateItems(arrDef, blobUpd, out arrRes);
        }


        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_ValidateItems01()
        {
            //Testing --- error code returned by interface
            OPCItemDef[] arrDef = new OPCItemDef[1];
            OPCItemDef opcDef = new OPCItemDef("DP1", true, 1, System.Runtime.InteropServices.VarEnum.VT_EMPTY);
            arrDef[0] = opcDef;
            bool blobUpd = false;
            OPCItemResult[] arrRes = null;
            //Test Procedure Call
            bool b = group.ValidateItems(arrDef, blobUpd, out arrRes);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_ValidateItems02()
        {
            //Testing --- error code is empty or output is null
            OPCItemDef[] arrDef = new OPCItemDef[1];
            OPCItemDef opcDef = new OPCItemDef("DP1", true, 1, System.Runtime.InteropServices.VarEnum.VT_EMPTY);
            arrDef[0] = opcDef;
            bool blobUpd = false;
            OPCItemResult[] arrRes = null;
            //Test Procedure Call
            bool b = group.ValidateItems(arrDef, blobUpd, out arrRes);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestWrite01()
        {
            //Testing --- 
            int[] arrHSrv = new int[1];
            object[] arrVal = new object[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.Write(arrHSrv, arrVal, out arrErr);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Write10()
        {
            //Testing --- 
            int[] arrHSrv = new int[2];
            object[] arrVal = new object[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.Write(arrHSrv, arrVal, out arrErr);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Write11()
        {
            //Testing --- 
            int[] arrHSrv = new int[1];
            object[] arrVal = new object[1];
            int[] arrErr = null;
            //Test Procedure Call
            bool b = group.Write(arrHSrv, arrVal, out arrErr);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestWrite21()
        {
            //Testing --- 
            int[] arrHSrv = new int[1];
            object[] arrVal = new object[1];
            int[] arrErr = null;
            int cancelId = 0;
            //Test Procedure Call
            bool b = group.Write(arrHSrv, arrVal, 1, out cancelId, out arrErr);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Write21()
        {
            //Testing --- 
            int[] arrHSrv = new int[2];
            object[] arrVal = new object[1];
            int[] arrErr = null;
            int cancelId = 0;
            //Test Procedure Call
            bool b = group.Write(arrHSrv, arrVal, 1, out cancelId, out arrErr);
        }

        [ExpectedException(typeof(System.Runtime.InteropServices.COMException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Write22()
        {
            //Testing --- error code  return
            int[] arrHSrv = new int[1];
            object[] arrVal = new object[1];
            int[] arrErr = null;
            int cancelId = 0;
            //Test Procedure Call
            bool b = group.Write(arrHSrv, arrVal, 1, out cancelId, out arrErr);
        }




        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestGet_Active01()
        {
            //Testing --- 
            
            //Test Procedure Call
            bool result = group.Active;
        }
        
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestSet_Active01()
        {
            //Testing --- 
            
            //Test Procedure Call
            group.Active = true;
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestHandleClient01()
        {
            //Testing --- 

            //Test Procedure Call
            int handleCl = group.HandleClient;
            group.HandleClient = 3;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestHandleServer01()
        {
            //Testing --- 

            //Test Procedure Call
            int handleServ = group.HandleServer;            
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestLocaleID01()
        {
            //Testing --- 

            //Test Procedure Call
            int localId = group.LocaleID;
            group.LocaleID = 1;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestName01()
        {
            //Testing --- 

            //Test Procedure Call
            string name = group.Name;
            group.Name = "grp2";
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestPercentDeadband01()
        {
            //Testing --- 

            //Test Procedure Call
            float deadBand = group.PercentDeadband;
            group.PercentDeadband = deadBand;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestPublic01()
        {
            //Testing --- 

            //Test Procedure Call
            bool publicFlag = group.Public;            
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestTimeBias01()
        {
            //Testing --- 

            //Test Procedure Call
            int timeBais = group.TimeBias;
            group.TimeBias = timeBais;
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestUpdateRate01()
        {
            //Testing --- 

            //Test Procedure Call
            int updateRate = group.UpdateRate;
            group.UpdateRate = updateRate;
        }

    }
}


