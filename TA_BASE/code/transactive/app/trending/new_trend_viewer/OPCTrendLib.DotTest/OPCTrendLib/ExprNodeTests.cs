﻿using System;
using System.Collections.Generic;
using System.Text;
using Dottest.Framework;
using NUnit.Framework;
using OPCTrendLib;

namespace DotTest.OPCTrendLib
{
    [TestFixture()]
    class ExprNodeTests
    {
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestLastOperand01()
        {
            BooleanAnd expression = new BooleanAnd();
            ExprNode exprNode = new ExprNode(expression);
            Accessor nodeAccessor = ReflectionAccessor.Wrap(exprNode);
            System.Collections.ArrayList operands = new System.Collections.ArrayList();
            nodeAccessor.SetField("_operands", operands);
            //Test Procedure Call
            ExprNode exprNode1 = exprNode.LastOperand;
            //Post Condition Check
            Assert.IsNull(exprNode1);
        }

        [ExpectedException(typeof(System.ArgumentException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Remove01()
        {
            BooleanAnd expression = new BooleanAnd();
            ExprNode exprNode = new ExprNode(expression);
            ExprNode exprNode1 = new ExprNode(expression);
            Accessor nodeAccessor = ReflectionAccessor.Wrap(exprNode);
            //Test Procedure Call
            nodeAccessor.Call("Remove", exprNode1);
            //Post Condition Check            
        }

        [ExpectedException(typeof(System.ArgumentException))]
        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestErr_Add01()
        {
            BooleanAnd expression = new BooleanAnd();
            ExprNode exprNode = new ExprNode(expression);
            ExprNode exprNode1 = new ExprNode(expression);
            Accessor nodeAccessor = ReflectionAccessor.Wrap(exprNode);
            Accessor nodeAccessor1 = ReflectionAccessor.Wrap(exprNode1);
            nodeAccessor1.SetField("_parent", exprNode);
            //Test Procedure Call
            nodeAccessor.Call("Add", exprNode1);
            //Post Condition Check            
        }

        [TestCaseUnverified("Test case not verified")]
        [TestCaseAutogenerated]
        [TestAuthor("user")]
        [Test]
        public void TestRemoveAt01()
        {
            BooleanAnd expression = new BooleanAnd();
            ExprNode exprNode = new ExprNode(expression);
            Accessor nodeAccessor = ReflectionAccessor.Wrap(exprNode);
            System.Collections.ArrayList operands = new System.Collections.ArrayList();
            Constant operand1 = new Constant(1);
            ExprNode expr1 = new ExprNode(operand1);
            Constant operand2 = new Constant(2);
            ExprNode expr2 = new ExprNode(operand2);
            operands.Add(expr1);
            operands.Add(expr2);
            nodeAccessor.SetField("_operands", operands);
            //Test Procedure Call
            nodeAccessor.Call("RemoveAt", 1);
            //Post Condition Check            
        }

    }
}
