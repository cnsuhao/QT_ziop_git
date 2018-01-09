using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices.ComTypes;
using OPCTrendLib.OPCHeader;
using System.Runtime.InteropServices;

namespace OPCTrendLib.OPCDataInterface
{
    public class EnumString : IEnumString
    {
        public List<string> lStr;
        private int currentpos;

        public EnumString()
        {
            lStr = new List<string>();
            currentpos = 0;
        }

        public int Next(int celt, string[] rgelt, IntPtr pceltFetched)
        {
            int i = 0;
            IntPtr intPtrObj = new IntPtr(2);
            GC.KeepAlive(intPtrObj);
            pceltFetched = intPtrObj;
            //Marshal.StructureToPtr(2, pceltFetched, false);            
            List<string> ResultList = new List<string>();
            ResultList.Add("DP1");
            ResultList.Add("DP2");
            GC.KeepAlive(ResultList);
            rgelt = ResultList.ToArray();


           /* List<string> ResultList = new List<string>();
            while ((i < celt) && (currentpos < lStr.Count))
            {
                ResultList.Add(lStr[currentpos]);
                i++;
                currentpos++;
            }
            GC.KeepAlive(ResultList);
            rgelt = ResultList.ToArray();
            Marshal.StructureToPtr(i, pceltFetched, false);*/
            //return (i == celt) ? HRESULTS.S_OK : HRESULTS.S_FALSE;
            return HRESULTS.S_FALSE;
        }

        public int Skip(int celt)
        {
            if (currentpos + celt <= lStr.Count)
            {
                currentpos += celt;
                return HRESULTS.S_OK;
            }
            else
            {
                currentpos = lStr.Count;
                return HRESULTS.S_FALSE;
            }
        }

        public void Reset()
        {
            currentpos = 0;
        }
        public void Clone(out IEnumString ppenum)
        {
            throw new System.NotImplementedException();
        }
        public void Add(string newMember)
        {
            lStr.Add(newMember);
        }
        public bool Remove(string s)
        {
            return lStr.Remove(s);
        }
    } 

}
