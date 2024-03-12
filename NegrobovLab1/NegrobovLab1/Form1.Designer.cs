namespace NegrobovLab1
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            eventExit.Set();
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            BStart = new Button();
            BStop = new Button();
            BSend = new Button();
            LBThreadNums = new ListBox();
            NStartThreads = new NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)NStartThreads).BeginInit();
            SuspendLayout();
            // 
            // BStart
            // 
            BStart.Location = new Point(10, 221);
            BStart.Margin = new Padding(3, 2, 3, 2);
            BStart.Name = "BStart";
            BStart.Size = new Size(82, 22);
            BStart.TabIndex = 0;
            BStart.Text = "Start";
            BStart.UseVisualStyleBackColor = true;
            BStart.Click += BStart_Click;
            // 
            // BStop
            // 
            BStop.Location = new Point(185, 221);
            BStop.Margin = new Padding(3, 2, 3, 2);
            BStop.Name = "BStop";
            BStop.Size = new Size(82, 22);
            BStop.TabIndex = 1;
            BStop.Text = "Stop";
            BStop.UseVisualStyleBackColor = true;
            BStop.Click += BStop_Click;
            // 
            // BSend
            // 
            BSend.Location = new Point(98, 221);
            BSend.Margin = new Padding(3, 2, 3, 2);
            BSend.Name = "BSend";
            BSend.Size = new Size(82, 22);
            BSend.TabIndex = 2;
            BSend.Text = "Send";
            BSend.UseVisualStyleBackColor = true;
            // 
            // LBThreadNums
            // 
            LBThreadNums.FormattingEnabled = true;
            LBThreadNums.ItemHeight = 15;
            LBThreadNums.Location = new Point(10, 9);
            LBThreadNums.Margin = new Padding(3, 2, 3, 2);
            LBThreadNums.Name = "LBThreadNums";
            LBThreadNums.Size = new Size(258, 184);
            LBThreadNums.TabIndex = 3;
            // 
            // NStartThreads
            // 
            NStartThreads.Location = new Point(10, 196);
            NStartThreads.Margin = new Padding(3, 2, 3, 2);
            NStartThreads.Minimum = new decimal(new int[] { 1, 0, 0, 0 });
            NStartThreads.Name = "NStartThreads";
            NStartThreads.Size = new Size(256, 23);
            NStartThreads.TabIndex = 4;
            NStartThreads.Value = new decimal(new int[] { 1, 0, 0, 0 });
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(277, 251);
            Controls.Add(NStartThreads);
            Controls.Add(LBThreadNums);
            Controls.Add(BSend);
            Controls.Add(BStop);
            Controls.Add(BStart);
            Margin = new Padding(3, 2, 3, 2);
            Name = "Form1";
            Text = "NegrobovLab1";
            ((System.ComponentModel.ISupportInitialize)NStartThreads).EndInit();
            ResumeLayout(false);
        }

        #endregion

        private Button BStart;
        private Button BStop;
        private Button BSend;
        private ListBox LBThreadNums;
        private NumericUpDown NStartThreads;
    }
}