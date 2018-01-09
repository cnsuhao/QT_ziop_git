using DAO.Trending.Common;
using Dottest.Framework;
using NUnit.Framework;
using System.Threading;

namespace DAO.Trending.DotTest.Common
{
    [TestFixture()]
    public class DbStatusMonitorThreadTests
    {
        private DbStatusMonitorThread monitorThread;

        [TestFixtureSetUp]
        public void FixtureSetUp()
        {
            monitorThread = new DbStatusMonitorThread();
        }

        [Test]
        public void TestAllFunctions()
        {
            monitorThread.AddItem(TestDBInit.localConnectionString1);

            bool isTerminated = monitorThread.IsTerminated();

           // monitorThread.Run();

            monitorThread.TerminateThread();
        }

        [Test]
        public void TestThreadMemberFunction01()
        {
           // monitorThread.TerminateThread();
            Accessor threadAccessor = ReflectionAccessor.Wrap(monitorThread);
            threadAccessor.SetField("m_terminate", false);
            monitorThread.UpdateStatusHandler += DBConnectionStrings.GetInstance().UpdateDBStatus;
            monitorThread.ThreadMemberFunction(TestDBInit.localConnectionString1);

            threadAccessor.SetField("m_terminate", true);
            monitorThread.ThreadMemberFunction(TestDBInit.localConnectionString1);
        }
        [Test]
        public void TestRun02()
        {
            DbStatusMonitorThread monitorThreadTemp = new DbStatusMonitorThread();

            ThreadStart threadStartFunction = new ThreadStart(monitorThreadTemp.Run);
            Thread workerThread = new Thread(threadStartFunction);

            monitorThreadTemp.UpdateStatusHandler += DBConnectionStrings.GetInstance().UpdateDBStatus;

            monitorThreadTemp.AddItem(TestDBInit.localConnectionString1);
           
//             try
//             {
//                 monitorThreadTemp.Run();
//             }
//             catch (System.Exception ex)
//             {
//             	
//             }
            
            workerThread.Start();

            //stub localList.Remove(localList[i]); throw exception
          
             Thread.Sleep(1000);
            monitorThreadTemp.TerminateThread();
                
        }

        [Test]
        public void TestRun03()
        {
            DbStatusMonitorThread monitorThreadTemp = new DbStatusMonitorThread();

            ThreadStart threadStartFunction = new ThreadStart(monitorThreadTemp.Run);
            Thread workerThread = new Thread(threadStartFunction);
            monitorThreadTemp.UpdateStatusHandler += DBConnectionStrings.GetInstance().UpdateDBStatus;

            Accessor threadAccessor = ReflectionAccessor.Wrap(monitorThreadTemp);
            threadAccessor.SetField("m_terminate", false);
            workerThread.Start();
            monitorThreadTemp.AddItem(TestDBInit.localConnectionString1);

            Thread.Sleep(300);
            monitorThreadTemp.TerminateThread();
        }

    }
}
