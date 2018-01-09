using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using OPCTrendLib.OPCHeader;
using System.Runtime.InteropServices.ComTypes;

namespace OPCTrendLib.OPCDataInterface
{
    [Guid("DCB048B7-F467-44b4-A9AD-05F8D0487073")]
    class OPCServerComClass : IOPCServer, IOPCCommon, IOPCBrowseServerAddressSpace, IOPCItemProperties, IConnectionPointContainer, IOPCServerPublicGroups
    {
        void IOPCServer.AddGroup([In, MarshalAs(UnmanagedType.LPWStr)] string szName, [In, MarshalAs(UnmanagedType.Bool)] bool bActive, [In] int dwRequestedUpdateRate, [In] int hClientGroup, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0, SizeConst = 1)] int[] pTimeBias, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0, SizeConst = 1)] float[] pPercentDeadband, [In] int dwLCID, out int phServerGroup, out int pRevisedUpdateRate, [In] ref Guid riid, [MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
             string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
             if (currentTestMethod.Equals("TestErr_internalAdd02"))
             {
                 ppUnk = null;
                 pRevisedUpdateRate = 500;
                 phServerGroup = 1;
             }
             else
             {
                 IOPCServer opcServerinter = (IOPCServer)this;
                 //OPCTrendLib.OPCData.OpcGroup group = new OPCTrendLib.OPCData.OpcGroup(ref opcServerinter, false, szName, bActive, dwRequestedUpdateRate);
                 OPCGroupStateMgtClass opcGrp = new OPCGroupStateMgtClass();
                 ppUnk = opcGrp;
                 pRevisedUpdateRate = 500;
                 phServerGroup = 1;
             }
        }
        void IOPCServer.GetErrorString([In] int dwError, [In] int dwLocale, [MarshalAs(UnmanagedType.LPWStr)] out string ppString)
        {
            ppString = "Error testing";
        }
        void IOPCServer.GetGroupByName([In, MarshalAs(UnmanagedType.LPWStr)] string szName, [In] ref Guid riid, [MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
            ppUnk = "GroupNameTesting";
        }
        void IOPCServer.GetStatus([MarshalAs(UnmanagedType.LPStruct)] out SERVERSTATUS ppServerStatus)
        {
            ppServerStatus = new OPCTrendLib.OPCDataInterface.SERVERSTATUS();
            ppServerStatus.eServerState = OPCTrendLib.OPCDataInterface.OPCSERVERSTATE.OPC_STATUS_SUSPENDED;
            ppServerStatus.ftStartTime = DateTime.Now.ToFileTime();
        }
        void IOPCServer.RemoveGroup([In] int hServerGroup, [In, MarshalAs(UnmanagedType.Bool)] bool bForce)
        {

        }
        int IOPCServer.CreateGroupEnumerator([In] int dwScope, [In] ref Guid riid, [MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
            ppUnk = null;
            return 0;
        }

        void IOPCCommon.SetLocaleID([In] int dwLcid)
        {

        }
        void IOPCCommon.GetLocaleID(out int pdwLcid)
        {
            pdwLcid = 0;
        }

        int IOPCCommon.QueryAvailableLocaleIDs(out int pdwCount, out IntPtr pdwLcid)
        {
            pdwCount = 1;

            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_QueryAvailableLocaleIDs01"))
            {
                pdwLcid = new IntPtr(0);
                return HRESULTS.OPC_E_DUPLICATENAME;
            }
            else if (currentTestMethod.Equals("TestErr_QueryAvailableLocaleIDs02"))
            {
                pdwCount = 0;
                pdwLcid = new IntPtr(1);
            }
            else
            {
                pdwCount = 1;
                pdwLcid = new IntPtr((int)1);               
            }            
            return HRESULTS.S_OK;
        }
        void IOPCCommon.SetClientName([In, MarshalAs(UnmanagedType.LPWStr)] string szName)
        {

        }

        void IOPCCommon.GetErrorString([In] int dwError, [MarshalAs(UnmanagedType.LPWStr)] out string ppString)
        {
            ppString = "Error testing";
        }


        void IOPCBrowseServerAddressSpace.QueryOrganization([MarshalAs(UnmanagedType.U4)] out OPCNAMESPACETYPE pNameSpaceType)
        {
            pNameSpaceType = OPCNAMESPACETYPE.OPC_NS_HIERARCHIAL;
        }
        void IOPCBrowseServerAddressSpace.ChangeBrowsePosition([In, MarshalAs(UnmanagedType.U4)] OPCBROWSEDIRECTION dwBrowseDirection, [In, MarshalAs(UnmanagedType.LPWStr)] string szName)
        {

        }
        int IOPCBrowseServerAddressSpace.BrowseOPCItemIDs([In, MarshalAs(UnmanagedType.U4)] OPCBROWSETYPE dwBrowseFilterType, [In, MarshalAs(UnmanagedType.LPWStr)] string szFilterCriteria, [In, MarshalAs(UnmanagedType.U2)] short vtDataTypeFilter, [In, MarshalAs(UnmanagedType.U4)] OPCACCESSRIGHTS dwAccessRightsFilter, [MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestBrowse01"))
            {
                EnumString enumStr = new EnumString();
                enumStr.Add("DP1");
                enumStr.Add("DP2");
                ppUnk = enumStr;
            }
            else
                ppUnk = null;
            return 1;
        }
        void IOPCBrowseServerAddressSpace.GetItemID([In, MarshalAs(UnmanagedType.LPWStr)] string szItemDataID, [MarshalAs(UnmanagedType.LPWStr)] out string szItemID)
        {
            szItemID = "testing";
        }

        int IOPCBrowseServerAddressSpace.BrowseAccessPaths([In, MarshalAs(UnmanagedType.LPWStr)] string szItemID, [MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestBrowseAccessPaths01"))
            {
                EnumString enumStr = new EnumString();
                enumStr.Add("AccessPath1");
                enumStr.Add("AccessPath2");
                ppUnk = enumStr;
            }
            else
                ppUnk = null;
            return 1;
        }



        void IOPCItemProperties.QueryAvailableProperties([In, MarshalAs(UnmanagedType.LPWStr)] string szItemID, out int dwCount, out IntPtr ppPropertyIDs, out IntPtr ppDescriptions, out IntPtr ppvtDataTypes)
        {
             string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
             if (currentTestMethod.Equals("TestErr_QueryAvailableProperties01"))
             {
                 dwCount = 1;
                 ppPropertyIDs = new IntPtr(0);
                 ppDescriptions = new IntPtr(0);
                 ppvtDataTypes = new IntPtr(0);
             }
             else
             {
                 dwCount = 1;
                 ppPropertyIDs = new IntPtr(1);
                 ppDescriptions = new IntPtr(1);
                 ppvtDataTypes = new IntPtr(1);
             }
            
        }

        int IOPCItemProperties.GetItemProperties([In, MarshalAs(UnmanagedType.LPWStr)] string szItemID, [In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] int[] pdwPropertyIDs, out IntPtr ppvData, out IntPtr ppErrors)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_GetItemProperties01"))
            {
                ppvData = new IntPtr(1);
                ppErrors = new IntPtr(1);
                return HRESULTS.OPC_E_NOTFOUND;
            }
            else if (currentTestMethod.Equals("TestErr_GetItemProperties02"))
            {
                ppvData = new IntPtr(0);
                ppErrors = new IntPtr(0);
                return HRESULTS.S_OK;
            }
            else
            {
                ppvData = new IntPtr(1);
                ppErrors = new IntPtr(1);
                return HRESULTS.S_OK;
            }
        }

        int IOPCItemProperties.LookupItemIDs([In, MarshalAs(UnmanagedType.LPWStr)] string szItemID, [In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] int[] pdwPropertyIDs, out IntPtr ppszNewItemIDs, out IntPtr ppErrors)
        {
             string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
             if (currentTestMethod.Equals("TestErr_LookupItemIDs01"))
             {
                 ppszNewItemIDs = new IntPtr(1);
                 ppErrors = new IntPtr(1);
                 return HRESULTS.E_OUTOFMEMORY;
             }
             else if (currentTestMethod.Equals("TestErr_LookupItemIDs02"))
             {
                 ppszNewItemIDs = new IntPtr(0);
                 ppErrors = new IntPtr(0);
                 return 1;
             }
             else
             {
                 ppszNewItemIDs = new IntPtr(1);
                 ppErrors = new IntPtr(1);
                 return 1;
             }
            
        }

        void IConnectionPointContainer.EnumConnectionPoints(out IEnumConnectionPoints ppEnum)
        {
            ppEnum = null;
        }
        void IConnectionPointContainer.FindConnectionPoint(ref Guid riid, out IConnectionPoint ppCP)
        {
            //ppCP = null;
            ppCP = new SampleConnectionPoint();
        }

        void IOPCServerPublicGroups.GetPublicGroupByName([In, MarshalAs(UnmanagedType.LPWStr)] string szName, [In] ref Guid riid, [MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
            ppUnk = new OPCGroupStateMgtClass();
        }
        void IOPCServerPublicGroups.RemovePublicGroup([In] int hServerGroup, [In, MarshalAs(UnmanagedType.Bool)] bool bForce)
        {

        }


       

    }

    public class OPCGroupStateMgtClass : IOPCGroupStateMgt, IOPCItemMgt, IOPCSyncIO, IOPCAsyncIO2, IConnectionPointContainer, IOPCPublicGroupStateMgt
    {
        void IOPCGroupStateMgt.GetState(out int pUpdateRate, [MarshalAs(UnmanagedType.Bool)] out bool pActive, [MarshalAs(UnmanagedType.LPWStr)] out string ppName, out int pTimeBias, out float pPercentDeadband, out int pLCID, out int phClientGroup, out int phServerGroup)
        {
            pUpdateRate = 500;
            pLCID = 1;
            phClientGroup = 1;
            phServerGroup = 1;
            pTimeBias = 1;
            ppName = "Grp1";
            pPercentDeadband = 1.0F;
            pActive = true;
        }
        void IOPCGroupStateMgt.SetState([In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0, SizeConst = 1)] int[] pRequestedUpdateRate, out int pRevisedUpdateRate, [In, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.Bool, SizeParamIndex = 0, SizeConst = 1)] bool[] pActive, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0, SizeConst = 1)] int[] pTimeBias, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0, SizeConst = 1)] float[] pPercentDeadband, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0, SizeConst = 1)] int[] pLCID, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0, SizeConst = 1)] int[] phClientGroup)
        {
            pRevisedUpdateRate = 500;
        }
        void IOPCGroupStateMgt.SetName([In, MarshalAs(UnmanagedType.LPWStr)] string szName)
        {

        }
        void IOPCGroupStateMgt.CloneGroup([In, MarshalAs(UnmanagedType.LPWStr)] string szName, [In] ref Guid riid, [MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
            ppUnk = null;
        }


        int IOPCItemMgt.AddItems([In] int dwCount, [In] IntPtr pItemArray, out IntPtr ppAddResults, out IntPtr ppErrors)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_AddItems01"))
            {
                ppAddResults = new IntPtr(1);
                ppErrors = new IntPtr(0);
                return 1;
            }
            else if (currentTestMethod.Equals("TestErr_AddItems02"))
            {
                ppAddResults = new IntPtr(1);
                ppErrors = new IntPtr(1);
                return HRESULTS.OPC_E_INVALIDCONFIGFILE;
            }
            else
            {
                //ppAddResults = new IntPtr(1);
                OPCTrendLib.OPCData.OPCItemResult result = new OPCTrendLib.OPCData.OPCItemResult();
                result.HandleServer = 1;
                result.CanonicalDataType = VarEnum.VT_EMPTY;
                result.AccessRights = OPCACCESSRIGHTS.OPC_READABLE;
                result.Blob = null;
                result.Error = 0;
                //ppAddResults = Marshal.AllocHGlobal(Marshal.SizeOf(result));
                //Marshal.StructureToPtr(result, ppAddResults, false);
                ppAddResults = new IntPtr(1);
                ppErrors = new IntPtr(1);
                return 1;
            }

        }

        int IOPCItemMgt.ValidateItems([In] int dwCount, [In] IntPtr pItemArray, [In, MarshalAs(UnmanagedType.Bool)] bool bBlobUpdate, out IntPtr ppValidationResults, out IntPtr ppErrors)
        {
              string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
              if (currentTestMethod.Equals("TestErr_ValidateItems01"))
              {
                  ppValidationResults = new IntPtr(1);
                  ppErrors = new IntPtr(1);
                  return HRESULTS.OPC_E_UNKNOWNPATH;
              }
              else if (currentTestMethod.Equals("TestErr_ValidateItems02"))
              {
                  ppValidationResults = new IntPtr(0);
                  ppErrors = new IntPtr(0);
                  return HRESULTS.S_OK;
              }
              else
              {
                  ppValidationResults = new IntPtr(1);
                  ppErrors = new IntPtr(1);
                  return 1;
              }
            
        }

        int IOPCItemMgt.RemoveItems([In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] int[] phServer, out IntPtr ppErrors)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_RemoveItems01"))
            {
                ppErrors = new IntPtr(0);
                return HRESULTS.E_OUTOFMEMORY;
            }
            else
            {
                ppErrors = new IntPtr(0);
                return HRESULTS.S_OK;
            }
            
        }

        int IOPCItemMgt.SetActiveState([In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] int[] phServer, [In, MarshalAs(UnmanagedType.Bool)] bool bActive, out IntPtr ppErrors)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_SetActiveState01"))
            {
                ppErrors = new IntPtr(0);
                return HRESULTS.E_ABORT;
            }
            else
            {
                ppErrors = new IntPtr(0);
                return HRESULTS.S_OK;
            }
        }

        int IOPCItemMgt.SetClientHandles([In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] int[] phServer, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] int[] phClient, out IntPtr ppErrors)
        {
              string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
              if (currentTestMethod.Equals("TestErr_SetClientHandles02"))
              {
                  ppErrors = new IntPtr(0);
                  return HRESULTS.OPC_E_UNKNOWNITEMID;
              }
              else
              {
                  ppErrors = new IntPtr(0);
                  return HRESULTS.S_OK;
              }
            
        }

        int IOPCItemMgt.SetDatatypes([In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] int[] phServer, [In] IntPtr pRequestedDatatypes, out IntPtr ppErrors)
        {
             string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
             if (currentTestMethod.Equals("TestErr_SetDatatypes02"))
             {
                 ppErrors = new IntPtr(0);
                 return HRESULTS.OPC_E_UNKNOWNPATH;
             }
             else
             {
                 ppErrors = new IntPtr(0);
                 return HRESULTS.S_OK;
             }
            
        }

        int IOPCItemMgt.CreateEnumerator([In] ref Guid riid, [MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_CreateAttrEnumerator01"))
            {
                ppUnk = null;
                return HRESULTS.OPC_E_UNKNOWNITEMID;
            }
            else if (currentTestMethod.Equals("TestErr_CreateAttrEnumerator02"))
            {
                ppUnk = null;
                return 1;
            }
            else
            {
                ppUnk = new SampleEnumOPCItemAttributes();
                return HRESULTS.S_OK;
            }

        }



        int IOPCSyncIO.Read([In, MarshalAs(UnmanagedType.U4)] OPCDATASOURCE dwSource, [In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] int[] phServer, out IntPtr ppItemValues, out IntPtr ppErrors)
        {

             string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
             if (currentTestMethod.Equals("TestErr_Read01"))
             {
                 ppItemValues = new IntPtr(1);
                 ppErrors = new IntPtr(0);
                 return HRESULTS.S_OK;
             }
             else if (currentTestMethod.Equals("TestErr_Read02"))
             {
                 ppItemValues = new IntPtr(1);
                 ppErrors = new IntPtr(0);
                 return HRESULTS.OPC_E_NOTFOUND;
             }
             else
             {
                 ppItemValues = new IntPtr(1);
                 ppErrors = new IntPtr(1);
                 return HRESULTS.S_OK;
             }
        }

        int IOPCSyncIO.Write([In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] int[] phServer, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] object[] pItemValues, out IntPtr ppErrors)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_Write11"))
            {
                ppErrors = new IntPtr(0);
                return HRESULTS.E_FAIL;
            }
            else
            {
                ppErrors = new IntPtr(0);
                return HRESULTS.S_OK;
            }
            
        }


        int IOPCAsyncIO2.Read([In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] int[] phServer, [In] int dwTransactionID, out int pdwCancelID, out IntPtr ppErrors)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_Read20"))
            {
                pdwCancelID = 1;
                ppErrors = new IntPtr(0);
                return HRESULTS.OPC_E_DUPLICATENAME;
            }
            else
            {
                pdwCancelID = 1;
                ppErrors = new IntPtr(0);
                return HRESULTS.S_OK;
            }

        }
        int IOPCAsyncIO2.Write([In] int dwCount, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] int[] phServer, [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] object[] pItemValues, [In] int dwTransactionID, out int pdwCancelID, out IntPtr ppErrors)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_Write22"))
            {
                pdwCancelID = 1;
                ppErrors = new IntPtr(0);
                return HRESULTS.OPC_E_INVALIDITEMID;
            }
            else
            {
                pdwCancelID = 1;
                ppErrors = new IntPtr(0);
                return HRESULTS.S_OK;
            }
        }
        void IOPCAsyncIO2.Refresh2([In, MarshalAs(UnmanagedType.U4)] OPCDATASOURCE dwSource, [In] int dwTransactionID, out int pdwCancelID)
        {
            pdwCancelID = 1;
        }
        void IOPCAsyncIO2.Cancel2([In] int dwCancelID)
        {

        }
        void IOPCAsyncIO2.SetEnable([In, MarshalAs(UnmanagedType.Bool)] bool bEnable)
        {

        }
        void IOPCAsyncIO2.GetEnable([MarshalAs(UnmanagedType.Bool)] out bool pbEnable)
        {
            pbEnable = true;
        }


        void IConnectionPointContainer.EnumConnectionPoints(out IEnumConnectionPoints ppEnum)
        {
            ppEnum = null;
        }
        void IConnectionPointContainer.FindConnectionPoint(ref Guid riid, out IConnectionPoint ppCP)
        {
            ppCP = new SampleConnectionPoint();
        }

        void IOPCPublicGroupStateMgt.GetState([MarshalAs(UnmanagedType.Bool)] out bool pPublic)
        {
            pPublic= true;
        }
        void IOPCPublicGroupStateMgt.MoveToPublic()
        {

        }

    }

    public class SampleConnectionPoint:IConnectionPoint
    {
        void IConnectionPoint.Advise(object pUnkSink, out int pdwCookie)
        {
            pdwCookie = 1;
        }

        void IConnectionPoint.EnumConnections(out IEnumConnections ppEnum)
        {
            ppEnum = null;
        }

        void IConnectionPoint.GetConnectionInterface(out Guid pIID)
        {
            pIID = Guid.NewGuid();
        }

        void IConnectionPoint.GetConnectionPointContainer(out IConnectionPointContainer ppCPC)
        {
            ppCPC = null;
        }

        void IConnectionPoint.Unadvise(int dwCookie)
        {
            string currentTestMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentTestMethod.Equals("TestErr_Disconnect01"))
            {
                throw new Exception();
            }
        }
    }


    public class SampleEnumOPCItemAttributes : IEnumOPCItemAttributes
    {
        void IEnumOPCItemAttributes.Next([In] int celt, out IntPtr ppItemArray, out int pceltFetched)
        {
            ppItemArray = new IntPtr(1);
            pceltFetched = 1;
        }

        void IEnumOPCItemAttributes.Skip([In] int celt)
        {

        }
        void IEnumOPCItemAttributes.Reset()
        {

        }
        void IEnumOPCItemAttributes.Clone([MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
            ppUnk = null;
        }
    }

}
