using DAO.Trending;
using Dottest.Framework;
using NUnit.Framework;

namespace DAO.Trending.DotTest.Factories
{
    [TestFixture(), ObjectFactoryType()]
    public class HistDataPointDAOFactory
    {
        [ObjectFactoryMethod]
        [HashCode("/N2s4g", "+7j+Vw")]
        public static HistDataPointDAO CreateHistDataPointDAO01()
        {
            HistDataPointDAO histDataPointDAO = new HistDataPointDAO();
            return histDataPointDAO;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateHistDataPointDAO01()
        {
            HistDataPointDAO histDataPointDAO = CreateHistDataPointDAO01();
            Assert.IsNotNull(histDataPointDAO);
        }

    }
}