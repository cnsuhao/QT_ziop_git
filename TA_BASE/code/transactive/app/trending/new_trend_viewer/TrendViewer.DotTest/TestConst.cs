namespace TrendViewer.DotTest
{
    class TestConst
    {
        public const string TEST_VIEW_ID = "";
        public const string GROUP_NAME = "TEST"  ;
        public const string HIST_GROUP_NAME = "unitTest_histDPGrp";
        public const string MARK_GROUP_NAME = "UnitTest_MarkerGrp";
        public const string FORMULA_GROUP_NAME = "UnitTestFormulaGrp";
        public const string DP_NAME = "DBG.ECS.TVS.TT01.aiiRTSA-Temp.Value"; //todo, set a dp name here which belongs to configuration: GROUP_NAME

        public const string HIST_DP_NAME = "histDP2"; //set a dp name here which belongs to configuration: unitTest_histDPGrp
        public const string FORMULA_LBL_NAME = "Lbl1";  //belong to UnitTestFormulaGrp
        public const string MARKER_NAME = "unitTest_marker1";  //belong to UnitTest_MarkerGrp
        public const ulong DATANODE_KEY = 10038731; 

        public const string HIST_GROUP_NAME_NOLOG = "AAA";
        //public const string CONNECTION_STR = "Data Source = C955;User Id = IT271350_5; Password = IT271350_5;";
        public const string CONNECTION_STR = "Oracle;Data Source = C955;User Id = IT271350_5; Password = IT271350_5;";
        
        public const string SERVER = "TransActiveDataSource";
        public const string HOST = "127.0.0.1";
    }
}