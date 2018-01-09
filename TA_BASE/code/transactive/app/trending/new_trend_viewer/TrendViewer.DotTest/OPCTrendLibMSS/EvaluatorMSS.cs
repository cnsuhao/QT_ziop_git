using System;
using Dottest.Framework;
using TrendViewer.DotTest.View;

namespace OPCTrendLibMSS
{
    [MethodSpecificStubsAssemblyAttribute("OPCTrendLib")]
    [MethodSpecificStubsClassAttribute("OPCTrendLib.Evaluator")]
    public class EvaluatorMSS
    {
        [MethodSpecificStubsMethod("OPCTrendLib.Evaluator.Eval(System.String)")]
        public static System.Object EvalMSS(
        OPCTrendLib.Evaluator target,
        System.String text)
        {
            object result;
            if (TestSpecificStubsUtil.RunTestSpecificStub(System.Reflection.MethodBase.GetCurrentMethod(), new object[] { target, text }, out result))
            {
                return (System.Object)result;
            }
            else
            {
                return target.Eval(text);
            }
        }

        [TestSpecificStub("OPCTrendLib.Evaluator.Eval(System.String)")]
        // TODO: uncomment the following attribute and provide a test fixture and a name of its test
        [TestSpecificStubContext(typeof(TrendViewTests), "TestUpdateChartForRealTimeOrMixedMode01")]
        [TestSpecificStubContext(typeof(TrendViewTests), "TestUpdateChartForRealTimeOrMixedMode03")]
        public static System.Object EvalTSS(
        OPCTrendLib.Evaluator target,
        System.String text)
        {
            string name = Dottest.Framework.Stubs.CurrentTestMethod.Name;
            if(name.Equals("TestUpdateChartForRealTimeOrMixedMode01"))
            {
                Double d = new double();
                d = 1;
                return d;
            }
            else{
                Exception e= new Exception();
                throw e;
            }

        }

    }
}

