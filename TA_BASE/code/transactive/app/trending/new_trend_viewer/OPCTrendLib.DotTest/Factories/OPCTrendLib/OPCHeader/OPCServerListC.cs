using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace OPCTrendLib.OPCHeader
{
    [Guid("DCB048B7-F467-44b4-A9AD-05F8D0487073")]
    class OPCServerListC:IOPCServerList
    {
        void IOPCServerList.EnumClassesOfCategories([In] int cImplemented, [In] ref Guid catidImpl, [In] int cRequired, [In] ref Guid catidReq, [MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
             string currentMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
             if (currentMethod.Equals("TestListAllServer04"))
             {
                 ppUnk = null;
             }  
             else if(currentMethod.Equals("TestListAllServer05"))
             {
                 IEnumGUID e = null;
                 ppUnk = e;
                 //ppUnk = new object();
             }
             else
             {
                 ppUnk = new EnumGUID();
             }           
        }
        void IOPCServerList.GetClassDetails([In] ref Guid clsid, [MarshalAs(UnmanagedType.LPWStr)] out string ppszProgID, [MarshalAs(UnmanagedType.LPWStr)] out string ppszUserType)
        {
            ppszUserType = "";
            ppszProgID = "";

        }   
        void IOPCServerList.CLSIDFromProgID([In, MarshalAs(UnmanagedType.LPWStr)] string szProgId, out Guid clsid)
        {
            clsid = new Guid("DCB048B7-F467-44b4-A9AD-05F8D0487073");
        }

    }

    [Guid("73E396A0-3A99-469a-A7C9-81B1675F86FE")]
    class EnumGUID: IEnumGUID
    {
        void IEnumGUID.Next([In] int celt, [In] IntPtr rgelt, out int pceltFetched)
        {
            string currentMethod = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if (currentMethod.Equals("TestListAllServer06"))
            {
                pceltFetched = 0;
            }
            else
            {
                pceltFetched = 2;
            }
            
        }
        void IEnumGUID.Skip([In] int celt)
        {
        }
        void IEnumGUID.Reset()
        {

        }
        void IEnumGUID.Clone([MarshalAs(UnmanagedType.IUnknown)] out object ppUnk)
        {
            ppUnk = null;
        }
    }


}
