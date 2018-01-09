using DAO.Trending;
using Dottest.Framework;
using NUnit.Framework;

namespace DAO.Trending.DotTest.Factories
{
    [TestFixture(), ObjectFactoryType()]
    public class EntityDAOFactory
    {
        [ObjectFactoryMethod]
        [HashCode("/C/yxg", "+v4BNw")]
        public static EntityDAO CreateEntityDAO01()
        {
            EntityDAO entityDAO = new EntityDAO();
            return entityDAO;
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateEntityDAO01()
        {
            EntityDAO entityDAO = CreateEntityDAO01();
            Assert.IsNotNull(entityDAO);
        }

    }
}
