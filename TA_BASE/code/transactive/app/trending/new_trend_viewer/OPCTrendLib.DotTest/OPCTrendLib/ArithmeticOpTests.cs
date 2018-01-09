﻿using Dottest.Framework;
using NUnit.Framework;
using System;
using System.Collections.Generic;
using OPCTrendLib;

namespace DotTest.OPCTrendLib
{
    [TestFixture()]
    class ArithmeticOpTests
    {
        [ExpectedException(typeof(System.NotSupportedException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestProcessArg01()
        {
            //testing --- not primitive types are compared
            ArithmeticOp op = new ArithmeticDivide();
            Accessor opAccessor = ReflectionAccessor.Wrap(op);
            Evaluator evaluater = new Evaluator();
            Result[] argArray = new Result[2];
            Result res1 = new Result(evaluater);
            Result res2 = new Result(2);
            argArray[0] = res1;
            argArray[1] = res2;
            //Test Procedure Call
            opAccessor.Call("ProcessArg", evaluater, argArray);
        }
    }
}