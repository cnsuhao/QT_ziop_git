using DAO.Trending;
using Dottest.Framework;
using NUnit.Framework;
using Entity.Trending;
using System.Collections.Generic;

namespace DAO.Trending.DotTest
{
    [TestFixture()]
    public class MarkerDAOTests
    {
        private MarkerDAO markerDAO;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            markerDAO = new MarkerDAO();
            TestDBInit.openConn();
        }


        [Test]
        public void TestGetAllMarkGrpNames()
        {
            markerDAO.GetAllMarkGrpNames();
        }

        [Test]
        public void Test_Delete_Insert_GetMarkGrp()
        {
            string markGrpName = "UnitTest_MarkerGrp";

            markerDAO.DeleteAllMarkerInGrp(markGrpName);

            List<EtyMarker> markerList = new List<EtyMarker>();
            EtyMarker marker1=new EtyMarker();
            marker1.MarkerEnabled = true;
            EtyMarker marker2=new EtyMarker();
            marker2.MarkerEnabled = false;
            marker1.MarkerName = "unitTest_marker1";
            marker2.MarkerName = "unitTest_marker2";
            markerList.Add(marker1);
            markerList.Add(marker2);

            //markerDAO.InsertMarkerListToGrp(markerList, markGrpName);
            markerDAO.SaveMarkerListToGrp(markerList, markGrpName);

            markerDAO.GetMarkersByCfgName(markGrpName);
            markerDAO.DeleteAllMarkerInGrp(markGrpName);
        }

        [Test]
        public void TestSave02()
        {
            markerDAO.SaveMarkerListToGrp(null, "");
        }

        [Test]
        public void TestExceptions()
        {
            markerDAO.GetAllMarkGrpNames();
            markerDAO.GetMarkersByCfgName("LRC HIGH TEMP");
        }
    }
}
