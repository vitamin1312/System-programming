using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace NegrobovLab1
{
    public partial class Form1 : Form
    {

        [DllImport("TransportLib", CharSet = CharSet.Ansi)]
        static extern void StartThread();

        [DllImport("TransportLib", CharSet = CharSet.Ansi)]
        static extern void StopThread();

        [DllImport("TransportLib", CharSet = CharSet.Ansi)]
        static extern void StopAllThreads();

        [DllImport("TransportLib", CharSet = CharSet.Ansi)]
        static extern void SendMessageTo(int addr, StringBuilder sb);


        Process childProcess = null;


        public Form1()
        {
            InitializeComponent();
        }

        private void BStart_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                LBThreadNums.Items.Clear();
                childProcess = Process.Start("NegrobovApp");
                LBThreadNums.Items.Add("Main");

            }
            else
            {
                int nThreads = Convert.ToInt32(NStartThreads.Value);

                for (int i = 0; i < nThreads; ++i)
                {
                    StartThread();
                    if (LBThreadNums.Items.Count == 1)
                    {
                        LBThreadNums.Items.Add((LBThreadNums.Items.Count).ToString());
                        LBThreadNums.Items.Add("All threads");
                    }
                    else
                    {
                        LBThreadNums.Items.Insert(LBThreadNums.Items.Count - 1, (LBThreadNums.Items.Count - 1).ToString());
                    }
                }
            }

        }

        private void BStop_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                LBThreadNums.Items.Clear();
                return;
            }

            StopThread();

            if (LBThreadNums.Items.Count >= 4)
            {
                LBThreadNums.Items.RemoveAt(LBThreadNums.Items.Count - 2);
            }

            else if (LBThreadNums.Items.Count == 3)
            {
                LBThreadNums.Items.RemoveAt(2);
                LBThreadNums.Items.RemoveAt(1);
            }

            else if (LBThreadNums.Items.Count == 1)
            {
                LBThreadNums.Items.Clear();
            }
        }

        private void BSend_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                LBThreadNums.Items.Clear();
                return;
            }

            int index = LBThreadNums.SelectedIndex;
            string message = TBMessage.Text;

            if (index == -1)
                return;
            else if (index == LBThreadNums.Items.Count - 1)
                SendMessageTo(-1, new StringBuilder(message));
            else
                SendMessageTo(index, new StringBuilder(message));
        }
    }
}