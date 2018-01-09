using System;
using System.Collections.Generic;

using System.Text;

namespace DAO.Trending.DotTest
{
    class TestDBInit
    {
        //Note! Has oracle 10 and 9 ,2 clients, should check the correct tnsnames.ora
       public const string localConnectionString1 = "Oracle;Data Source = C955;User Id = IT271350_5; Password = IT271350_5;";
        //public const string localConnectionString1 = "Oracle;Data Source = TRANSACT;User Id = IT01271350_5; Password = IT01271350_5;";
       //  public const string localConnectionString1 = "Oracle;Data Source = XP_LUO_830;User Id = IT01271350; Password = IT01271350;";
        
        public const string localConnectionString2 = "MySql;Database = tra_occ;User Id = IT271350_5; Password = IT271350_5;Host = 192.168.123.43;Port=3306;";
        public static bool dbConnected = false;

        public const string SCADA_PKEY_STR = "10000052";
        public const ulong SCADA_PKEY = 10000052;
        public const string SERVER_ROOT_NAME = "Scada";
        public const string OPCSERVER_NAME = "TransActiveDataSource";
        public const string HOST_NAME = "127.0.0.1";
            

        public static void openConn()
        {
            DAO.Trending.Common.DBConnectionStrings.GetInstance().AddConnectionString(localConnectionString1);
            dbConnected = DAO.Trending.Common.SimpleDatabase.GetInstance().OpenConnection();
                
        }
    }
}
