using DAO.Trending;
using Dottest.Framework;
using NUnit.Framework;
using Entity.Trending;
using System.Collections.Generic;

namespace DAO.Trending.DotTest
{
    [TestFixture()]
    public class DataPointDAOTests
    {
        private DataPointDAO dataPointDao;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            dataPointDao = new DataPointDAO();
            TestDBInit.openConn();
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("/zwomQ", "+tukyQ")]
        public void TestDataPointDAOConstructor01()
        {
            DataPointDAO dataPointDAO = new DataPointDAO();
        }

        [Test]
        public void TestGetAllDPGrpNames01()
        {
            dataPointDao.GetAllDPGrpNames();

        }


        [Test]
        public void TestInsertDPListToGrp_Get_Delete_01()
        {
            List<EtyDataPoint> datapointList = new List<EtyDataPoint>();
            EtyDataPoint dp = new EtyDataPoint();
            dp.DisplayName = "test_DispName";
            dp.Description = "test_desc";
            dp.DPName = "test_DP_Name";
            string grpName = "test_grpName";
            dp.DPServer = "TransActiveDataSource";
            dp.DPHost = TestDBInit.HOST_NAME;
            dp.DPLblName = "LblName";
            datapointList.Add(dp);

            dataPointDao.SaveDPListToGrp(datapointList, grpName);
            //dataPointDao.InsertDPListToGrp(datapointList, grpName);

            dataPointDao.GetDPByCfgName(grpName);

            dataPointDao.DeleteAllDPInGrp(grpName);
        }

        [Test]
        public void TestSave02()
        {
            //test rollback
            dataPointDao.SaveDPListToGrp(null, "");
        }

        [Test]
        public void TestException()
        {
            //test exceptions
            dataPointDao.GetAllDPGrpNames();
            dataPointDao.GetDPByCfgName("Temp");
        }
    }
}
