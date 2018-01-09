using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib;

namespace DotTest.OPCTrendLib
{
    [TestFixture()]
    public class EvaluatorTests
    {
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        [HashCode("+KhvJA", "/cRPag")]
        public void TestEvaluatorConstructor01()
        {
            Evaluator evaluator = new Evaluator();
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBinaryOpExecutor01()
        {           
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            bool result = (bool) eval.Eval("2>5");
            //Post Condition Check
            Assert.IsFalse(result);           
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBinaryAndExecutor01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            bool result = (bool)eval.Eval("(5>3)&&(1<5)");
            //Post Condition Check
            Assert.IsTrue(result);

            //Test Procedure Call
            result = (bool)eval.Eval("(2>5)&&(1<5)");
            //Post Condition Check
            Assert.IsFalse(result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBinaryOrExecutor01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            bool result = (bool)eval.Eval("(2<5)||(1>1)");
            //Post Condition Check
            Assert.IsTrue(result);

            //Test Procedure Call
            result = (bool)eval.Eval("(2>5)||(1>2)");
            //Post Condition Check
            Assert.IsFalse(result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestConditionalOpExecutor01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            string result = (string)eval.Eval("(2>1)?\"true\":\"false\"");
            //Post Condition Check
            Assert.AreEqual("true",result);

            //Test Procedure Call
            result = (string)eval.Eval("(1>2)?\"true\":\"false\"");
            //Post Condition Check
            Assert.AreEqual("false", result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestConditionalOpExecutor02()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("true ? 1 : 0");
            //Post Condition Check
            Assert.AreEqual(1, result);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestCreateExecutorCore01()
        {
            Evaluator eval = new Evaluator();
            Accessor evalAccessor = ReflectionAccessor.Wrap(eval);
            MemberOp op = new MemberOp();
            ExprNode node = new ExprNode(op);
            //Test Procedure Call
            IExecutor executer = (IExecutor)evalAccessor.Call("CreateExecutorCore", node);            
            //Post Condition Check
            Assert.IsInstanceOfType(typeof(Executor), executer);           

        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestVariableHolder01()
        {
            Evaluator eval = new Evaluator();
            ParameterVariableHolder parameterHolder = new ParameterVariableHolder();
            //Test Procedure Call
            eval.VariableHolder = parameterHolder;
            //Post Condition Check
            Assert.AreEqual(parameterHolder, eval.VariableHolder);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestKeyword01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            bool result = (bool)eval.Eval("true||false");
            //Post Condition Check
            Assert.IsTrue(result);
        }

        [ExpectedException(typeof(System.NullReferenceException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestInvalidKeyword01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            bool result = (bool)eval.Eval("go||false");
            //Post Condition Check
            //Assert.IsTrue(result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestAddition01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("2+5");
            //Post Condition Check
            Assert.AreEqual(7, result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBitwiseOR01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            uint result = (uint)eval.Eval("~0");
            //Post Condition Check
            Assert.AreEqual(4294967295, result);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBooleanNot01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            bool result = (bool)eval.Eval("!true");
            //Post Condition Check
            Assert.IsFalse(result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestArithmeticDivide01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("16/2");
            //Post Condition Check
            Assert.AreEqual((16 / 2), result);
        }


        [ExpectedException(typeof(System.InvalidCastException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_ArithmeticDivide01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("null/2");
            //Post Condition Check            
        }

        [ExpectedException(typeof(System.InvalidCastException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_ArithmeticDivide2()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("2/null");
            //Post Condition Check            
        }

        
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestArithmeticDivide02()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            System.Single result = (System.Single)eval.Eval("10.2/2");
            //Post Condition Check
            Assert.AreEqual((10.2 / 2), result);


            //Test Procedure Call
            result = (System.Single)eval.Eval("10/2.0");
            //Post Condition Check
            Assert.AreEqual((10 / 2.0), result);
            
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestArithmeticMod01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("16%2");
            //Post Condition Check
            Assert.AreEqual((16 % 2), result);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestArithmeticMultiply01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("16*2");
            //Post Condition Check
            Assert.AreEqual((16 * 2), result);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestArithmeticSub01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("16-2");
            //Post Condition Check
            Assert.AreEqual((16 - 2), result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBinaryShift01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("2>>1");
            //Post Condition Check
            Assert.AreEqual((2 >> 1), result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBinaryShift02()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("2<<1");
            //Post Condition Check
            Assert.AreEqual((2 << 1), result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestVariable01()
        {
            Evaluator eval = new Evaluator();
            ParameterVariableHolder parameters = new ParameterVariableHolder();
            Accessor parametersAccessor = ReflectionAccessor.Wrap(parameters);
            ParameterCollection param = new ParameterCollection();
            Accessor paramAccessor = ReflectionAccessor.Wrap(param);
            System.Collections.Hashtable paramTable = new System.Collections.Hashtable();
            Parameter p1 = new Parameter("TestingVariable");
            paramTable.Add("Var1", p1);
            paramAccessor.SetField("_parameters", paramTable);
            parametersAccessor.SetField("_parameters", param);
            eval.VariableHolder = parameters;
            //Test Procedure Call
            string result = (string)eval.Eval("Var1");
            //Post Condition Check
            Assert.AreEqual("TestingVariable", result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestVariable02()
        {
            Evaluator eval = new Evaluator();
            ParameterVariableHolder parameters = new ParameterVariableHolder();
            Accessor parametersAccessor = ReflectionAccessor.Wrap(parameters);
            ParameterCollection param = new ParameterCollection();
            Accessor paramAccessor = ReflectionAccessor.Wrap(param);
            System.Collections.Hashtable paramTable = new System.Collections.Hashtable();
            Parameter p1 = new Parameter(null);
            paramTable.Add("Var1", p1);
            paramAccessor.SetField("_parameters", paramTable);
            parametersAccessor.SetField("_parameters", param);
            eval.VariableHolder = parameters;
            //Test Procedure Call
            eval.Eval("Var1");
            //Post Condition Check            
        }

        [ExpectedException(typeof(EvalException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestVariable03()
        {
            Evaluator eval = new Evaluator();
            ParameterVariableHolder parameters = new ParameterVariableHolder();
            Accessor parametersAccessor = ReflectionAccessor.Wrap(parameters);
            ParameterCollection param = new ParameterCollection();
            Accessor paramAccessor = ReflectionAccessor.Wrap(param);
            System.Collections.Hashtable paramTable = new System.Collections.Hashtable();
            Parameter p1 = new Parameter(null);
            paramTable.Add("Var1", p1);
            paramAccessor.SetField("_parameters", paramTable);
            parametersAccessor.SetField("_parameters", param);
            eval.VariableHolder = parameters;
            //Test Procedure Call
            eval.Eval("Var2");
            //Post Condition Check            
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBitwiseAnd01()
        {
            Evaluator eval = new Evaluator();         
            //Test Procedure Call
            int result = (int)eval.Eval("1&1");
            //Post Condition Check     
            Assert.AreEqual((1 & 1), result);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBitwiseXor01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("1^1");
            //Post Condition Check     
            Assert.AreEqual((1 ^ 1), result);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestMultipleOperation01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("(1+2)+3/4");
            //Post Condition Check     
            //Assert.AreEqual(((1 + 2) + 3 / 4), result);


            //Test Procedure Call
            result = (int)eval.Eval("((1+2)+(4/4))*((33/11)+(255/5))");
            //Post Condition Check     
            //Assert.AreEqual(((1 + 2) + 3 / 4), result);

            //Test Procedure Call
            result = (int)eval.Eval("(1*2)+(4/2)");
            //Post Condition Check     
            //Assert.AreEqual(((1 + 2) + 3 / 4), result);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestHexaDecimalAddition01()
        {
            Evaluator eval = new Evaluator();
            //Test Procedure Call
            int result = (int)eval.Eval("0x01+0x02");
            //Post Condition Check     
            //Assert.AreEqual(((1 + 2) + 3 / 4), result);           
        }
       
    }
}
