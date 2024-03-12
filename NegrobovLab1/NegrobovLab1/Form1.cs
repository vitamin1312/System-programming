using System.Diagnostics;

namespace NegrobovLab1
{
    public partial class Form1 : Form
    {
        Process childProcess = null;
        EventWaitHandle eventStart = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        EventWaitHandle eventConfirm = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        EventWaitHandle eventClose = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseProcNegrobov");
        EventWaitHandle eventExit = new EventWaitHandle(false, EventResetMode.AutoReset, "ExitProcNegrobov");

        int threadsCounter = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void BStart_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                LBThreadNums.Items.Clear();
                childProcess = Process.Start("NegrobovConsoleApp.exe");
                eventConfirm.WaitOne();
                LBThreadNums.Items.Add("Main");
                LBThreadNums.Items.Add("All threads");
                threadsCounter++;

            }
            else
            {
                int nThreads = Convert.ToInt32(NStartThreads.Value);

                for (int i = 0; i < nThreads; ++i)
                {
                    eventStart.Set();
                    eventConfirm.WaitOne();
                    LBThreadNums.Items.Add((threadsCounter).ToString());
                    threadsCounter++;
                }
            }

        }

        private void BStop_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
                return;
  
            eventClose.Set();
            eventConfirm.WaitOne();
            threadsCounter--;

            if (threadsCounter >= 2)
            {
                LBThreadNums.Items.RemoveAt(threadsCounter + 1);
            }

            else if (threadsCounter == 1)
            {
                LBThreadNums.Items.RemoveAt(2);
            }

            else if (threadsCounter == 0)
            {
                LBThreadNums.Items.Clear();
            }
 
        }
    }
}