using DAO.Trending.Helper;
using Dottest.Framework;
using Dottest.Framework.RecordState;
using NUnit.Framework;
using System;

namespace DAO.Trending.DotTest.Helper
{
    [TestFixture()]
    public class LocationKeyHelperTests
    {
        private LocationKeyHelper locationKeyHelper;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            //Test all the normal flow of all the functions:

            locationKeyHelper = LocationKeyHelper.GetInstance();
            TestDBInit.openConn();
            
        }

        [Test]
        public void TestAllFunc_OCC()
        {
            //Test all the code of normal flow, as OCC

            //stub: System.Net.Dns.GetHostName();  return "occ-mft-01"
            locationKeyHelper.init();

            string hostName = locationKeyHelper.HostName;
            locationKeyHelper.HostName = hostName;

            bool isOCC = locationKeyHelper.IsOCC;
            locationKeyHelper.IsOCC = isOCC;

            ulong lockey = locationKeyHelper.LocationKey;
            locationKeyHelper.LocationKey = lockey;
        }


        [Test]
        public void Test_AbNormalFlow()
        {
            //Test all the abnormal flow of code:

            //Test InitLocationKeyByHostName function, when m_HostName=="";
            Accessor helperAccessor = ReflectionAccessor.Wrap(locationKeyHelper);
            helperAccessor.SetField("m_HostName", "");
            helperAccessor.Call("InitLocationKeyByHostName");

            //test isOCC()function, when m_LocationKey!= OCCLocKey
            helperAccessor.SetField("m_LocationKey", (System.UInt64)(13));
            helperAccessor.Call("InitIsOCC");
            helperAccessor.SetField("m_LocationKey", (System.UInt64)(1));
            helperAccessor.Call("InitIsOCC");

            //test exception
            //stub: System.Net.Dns.GetHostName();  throw exception
            helperAccessor.Call("InitHostName");
        }


    }
}
