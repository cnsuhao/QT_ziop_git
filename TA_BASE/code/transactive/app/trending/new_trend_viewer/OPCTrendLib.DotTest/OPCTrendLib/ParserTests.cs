using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib;
//using STEE.ISCS.Log.DotTest.Factories;
using System;

namespace DotTest.OPCTrendLib
{
    [TestFixture()]
    public class ParserTests
    {
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestIsStartableToken01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            //Test Procedure Call
            bool b = (bool) parserAccesor.Call("IsStartableToken", Token.Multiply);
            //Post Condition Check
            Assert.IsFalse(b);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestIsCanFollowIdentifier01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            //Test Procedure Call
            bool b = (bool)parserAccesor.Call("IsCanFollowIdentifier", Token.CharMarker);
            //Post Condition Check
            Assert.IsFalse(b);
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestIsCanFollowConst01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            //Test Procedure Call
            bool b = (bool) parserAccesor.Call("IsCanFollowConst", Token.DateMarker);
            //Post Condition Check
            Assert.IsFalse(b);
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildException01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parserAccesor.SetField("_current", new Context());
            //Test Procedure Call
            ParserException parExcept = (ParserException)parserAccesor.Call("BuildException", Error.CharNotEnd);
            //Post Condition Check
            
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestBuildException02()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            //Test Procedure Call
            ParserException parExcept = (ParserException) parserAccesor.Call("BuildException", Error.CharNotEnd, new Context());
            //Post Condition Check
        }

        [ExpectedException(typeof(ParserException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_ParseConst01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parser.Parse("true#11/02/2013#");
            //Test Procedure Call
            //parserAccesor.Call("ParseConst");
            //Post Condition Check
        }

        [ExpectedException(typeof(ParserException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_CheckStartableToken01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parserAccesor.SetField("_current", new Context());
            //Test Procedure Call
            parserAccesor.Call("CheckStartableToken", Token.Multiply);
            //Post Condition Check
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestParseIndexer01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parser.Parse("str[2]");
            //Test Procedure Call
            //parserAccesor.Call("ParseIndexer", Token.Multiply);
            //Post Condition Check
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestParseMember01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parser.Parse("str.()");
            //Test Procedure Call
            //parserAccesor.Call("ParseIndexer", Token.Multiply);
            //Post Condition Check
        }

        [ExpectedException(typeof(ParserException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_ParseMember01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parser.Parse("str.+");
            //Test Procedure Call
            //parserAccesor.Call("ParseIndexer", Token.Multiply);
            //Post Condition Check
        }


        [ExpectedException(typeof(ParserException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_ParseIdentifier01()
        {
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parser.Parse("Var#13/02/2013#");
            //Test Procedure Call
            //parserAccesor.Call("ParseIndexer", Token.Multiply);
            //Post Condition Check
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestParser01()
        {
            //testing -- 
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parser.Parse("(+123)");
            //Test Procedure Call
            //parserAccesor.Call("ParseIndexer", Token.Multiply);
            //Post Condition Check
        }


        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestParser02()
        {
            //testing -- comma 
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parser.Parse("(,(2+5))");
            //Test Procedure Call
            //parserAccesor.Call("ParseIndexer", Token.Multiply);
            //Post Condition Check
        }


        [ExpectedException(typeof(ParserException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestParser03()
        {
            //testing -- 
            Parser parser = new Parser();
            Accessor parserAccesor = ReflectionAccessor.Wrap(parser);
            parser.Parse("Var.FF()()");
            //Test Procedure Call
            //parserAccesor.Call("ParseIndexer", Token.Multiply);
            //Post Condition Check
        }



    }
}
