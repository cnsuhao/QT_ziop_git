using System;
using Dottest.Framework;

namespace DAO.TrendingMSS
{
    [MethodSpecificStubsAssemblyAttribute("DAO.Trending")]
    [MethodSpecificStubsClassAttribute("DAO.Trending.EntityDAO")]
    public class EntityDAOMSS
    {
        [MethodSpecificStubsMethod("DAO.Trending.EntityDAO.GetDataNodeChildrenByPkey(System.UInt64)")]
        public static System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity> GetDataNodeChildrenByPkeyMSS(
        DAO.Trending.EntityDAO target,
        System.UInt64 pkey)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, pkey }, out result))
            {
                return (System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity>)result;
            }
            else
            {
                return target.GetDataNodeChildrenByPkey(pkey);
            }
        }

        [TestSpecificStub("DAO.Trending.EntityDAO.GetDataNodeChildrenByPkey(System.UInt64)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Model.OPCDataSelectorModelTests), "TestGetChildOPCDataNodes01")]
        public static System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity> GetDataNodeChildrenByPkeyTSS(
        DAO.Trending.EntityDAO target,
        System.UInt64 pkey)
        {
            System.Collections.Generic.Dictionary<ulong, Entity.Trending.EtyEntity> dataNodelist = new System.Collections.Generic.Dictionary<ulong, Entity.Trending.EtyEntity>();
            Entity.Trending.EtyEntity entity = new Entity.Trending.EtyEntity();
            entity.Pkey = 1239879;
            entity.DisplayName = "testingDN";
            dataNodelist.Add(1239879, entity);
            return dataNodelist;
        }

        [MethodSpecificStubsMethod("DAO.Trending.EntityDAO.GetAllDataNodesByServerRootName(System.String)")]
        public static System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity> GetAllDataNodesByServerRootNameMSS(
        DAO.Trending.EntityDAO target,
        System.String serverRootName)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, serverRootName }, out result))
            {
                return (System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity>)result;
            }
            else
            {
                return target.GetAllDataNodesByServerRootName(serverRootName);
            }
        }

        [TestSpecificStub("DAO.Trending.EntityDAO.GetAllDataNodesByServerRootName(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(OPCSampleGrpConfig.DotTest.Model.OPCDataSelectorModelTests), "TestGetAllOPCDataNodes01")]
        public static System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity> GetAllDataNodesByServerRootNameTSS(
        DAO.Trending.EntityDAO target,
        System.String serverRootName)
        {
            System.Collections.Generic.Dictionary<System.UInt64, Entity.Trending.EtyEntity>  dataNodelist = new System.Collections.Generic.Dictionary<ulong,Entity.Trending.EtyEntity>();
            Entity.Trending.EtyEntity entity = new Entity.Trending.EtyEntity();
            dataNodelist.Add(1,entity);
            return dataNodelist;
        }

    }
}

