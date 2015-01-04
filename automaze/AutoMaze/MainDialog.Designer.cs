namespace AutoMaze
{
	partial class MainDialog
	{
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		/// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows 窗体设计器生成的代码

		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		private void InitializeComponent()
		{
			this.display = new System.Windows.Forms.PictureBox();
			this.btnDeepFirst = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.btnHeuristicALG = new System.Windows.Forms.Button();
			this.btnWideFirst = new System.Windows.Forms.Button();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.btnSetting = new System.Windows.Forms.Button();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.btnRand = new System.Windows.Forms.Button();
			this.btnDFS = new System.Windows.Forms.Button();
			this.btnRD = new System.Windows.Forms.Button();
			this.btnRP = new System.Windows.Forms.Button();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.lblSolve = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.lblGen = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.lblSeed = new System.Windows.Forms.Label();
			this.lblSize = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox5 = new System.Windows.Forms.GroupBox();
			this.lblStatus = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.display)).BeginInit();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.groupBox5.SuspendLayout();
			this.SuspendLayout();
			// 
			// display
			// 
			this.display.Dock = System.Windows.Forms.DockStyle.Left;
			this.display.Location = new System.Drawing.Point(0, 0);
			this.display.Name = "display";
			this.display.Size = new System.Drawing.Size(562, 562);
			this.display.TabIndex = 0;
			this.display.TabStop = false;
			// 
			// btnDeepFirst
			// 
			this.btnDeepFirst.Location = new System.Drawing.Point(6, 20);
			this.btnDeepFirst.Name = "btnDeepFirst";
			this.btnDeepFirst.Size = new System.Drawing.Size(75, 23);
			this.btnDeepFirst.TabIndex = 20;
			this.btnDeepFirst.Text = "深度优先";
			this.btnDeepFirst.UseVisualStyleBackColor = true;
			this.btnDeepFirst.Click += new System.EventHandler(this.btnDeepFirst_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.btnHeuristicALG);
			this.groupBox1.Controls.Add(this.btnWideFirst);
			this.groupBox1.Controls.Add(this.btnDeepFirst);
			this.groupBox1.Location = new System.Drawing.Point(574, 162);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(200, 86);
			this.groupBox1.TabIndex = 3;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "搜索算法";
			// 
			// btnHeuristicALG
			// 
			this.btnHeuristicALG.Location = new System.Drawing.Point(87, 20);
			this.btnHeuristicALG.Name = "btnHeuristicALG";
			this.btnHeuristicALG.Size = new System.Drawing.Size(75, 23);
			this.btnHeuristicALG.TabIndex = 21;
			this.btnHeuristicALG.Text = "启发式";
			this.btnHeuristicALG.UseVisualStyleBackColor = true;
			this.btnHeuristicALG.Click += new System.EventHandler(this.btnHeuristicALG_Click);
			// 
			// btnWideFirst
			// 
			this.btnWideFirst.Location = new System.Drawing.Point(6, 49);
			this.btnWideFirst.Name = "btnWideFirst";
			this.btnWideFirst.Size = new System.Drawing.Size(75, 23);
			this.btnWideFirst.TabIndex = 22;
			this.btnWideFirst.Text = "广度优先";
			this.btnWideFirst.UseVisualStyleBackColor = true;
			this.btnWideFirst.Click += new System.EventHandler(this.btnWideFirst_Click);
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.btnSetting);
			this.groupBox2.Location = new System.Drawing.Point(574, 13);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(200, 56);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "设置";
			// 
			// btnSetting
			// 
			this.btnSetting.Location = new System.Drawing.Point(6, 20);
			this.btnSetting.Name = "btnSetting";
			this.btnSetting.Size = new System.Drawing.Size(156, 23);
			this.btnSetting.TabIndex = 11;
			this.btnSetting.Text = "参数设置";
			this.btnSetting.UseVisualStyleBackColor = true;
			this.btnSetting.Click += new System.EventHandler(this.btnSetting_Click);
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.Add(this.btnRand);
			this.groupBox3.Controls.Add(this.btnDFS);
			this.groupBox3.Controls.Add(this.btnRD);
			this.groupBox3.Controls.Add(this.btnRP);
			this.groupBox3.Location = new System.Drawing.Point(574, 75);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(198, 81);
			this.groupBox3.TabIndex = 2;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "生成算法";
			// 
			// btnRand
			// 
			this.btnRand.Location = new System.Drawing.Point(6, 49);
			this.btnRand.Name = "btnRand";
			this.btnRand.Size = new System.Drawing.Size(75, 23);
			this.btnRand.TabIndex = 15;
			this.btnRand.Text = "随机";
			this.btnRand.UseVisualStyleBackColor = true;
			this.btnRand.Click += new System.EventHandler(this.btnRND_Click);
			// 
			// btnDFS
			// 
			this.btnDFS.Location = new System.Drawing.Point(6, 20);
			this.btnDFS.Name = "btnDFS";
			this.btnDFS.Size = new System.Drawing.Size(75, 23);
			this.btnDFS.TabIndex = 13;
			this.btnDFS.Text = "深度优先";
			this.btnDFS.UseVisualStyleBackColor = true;
			this.btnDFS.Click += new System.EventHandler(this.btnDFS_Click);
			// 
			// btnRD
			// 
			this.btnRD.Location = new System.Drawing.Point(87, 49);
			this.btnRD.Name = "btnRD";
			this.btnRD.Size = new System.Drawing.Size(75, 23);
			this.btnRD.TabIndex = 16;
			this.btnRD.Text = "递归分割";
			this.btnRD.UseVisualStyleBackColor = true;
			this.btnRD.Click += new System.EventHandler(this.btnREC_Click);
			// 
			// btnRP
			// 
			this.btnRP.Location = new System.Drawing.Point(87, 20);
			this.btnRP.Name = "btnRP";
			this.btnRP.Size = new System.Drawing.Size(75, 23);
			this.btnRP.TabIndex = 14;
			this.btnRP.Text = "广度优先";
			this.btnRP.UseVisualStyleBackColor = true;
			this.btnRP.Click += new System.EventHandler(this.btnBFS_Click);
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.lblSolve);
			this.groupBox4.Controls.Add(this.label4);
			this.groupBox4.Controls.Add(this.lblGen);
			this.groupBox4.Controls.Add(this.label3);
			this.groupBox4.Controls.Add(this.lblSeed);
			this.groupBox4.Controls.Add(this.lblSize);
			this.groupBox4.Controls.Add(this.label2);
			this.groupBox4.Controls.Add(this.label1);
			this.groupBox4.Location = new System.Drawing.Point(574, 450);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(200, 100);
			this.groupBox4.TabIndex = 4;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "地图信息";
			// 
			// lblSolve
			// 
			this.lblSolve.Location = new System.Drawing.Point(85, 86);
			this.lblSolve.Name = "lblSolve";
			this.lblSolve.Size = new System.Drawing.Size(90, 12);
			this.lblSolve.TabIndex = 0;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(6, 86);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(65, 12);
			this.label4.TabIndex = 0;
			this.label4.Text = "搜索算法：";
			// 
			// lblGen
			// 
			this.lblGen.Location = new System.Drawing.Point(85, 64);
			this.lblGen.Name = "lblGen";
			this.lblGen.Size = new System.Drawing.Size(90, 12);
			this.lblGen.TabIndex = 0;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(6, 64);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(65, 12);
			this.label3.TabIndex = 0;
			this.label3.Text = "生成算法：";
			// 
			// lblSeed
			// 
			this.lblSeed.Location = new System.Drawing.Point(85, 42);
			this.lblSeed.Name = "lblSeed";
			this.lblSeed.Size = new System.Drawing.Size(90, 12);
			this.lblSeed.TabIndex = 0;
			// 
			// lblSize
			// 
			this.lblSize.Location = new System.Drawing.Point(85, 20);
			this.lblSize.Name = "lblSize";
			this.lblSize.Size = new System.Drawing.Size(90, 12);
			this.lblSize.TabIndex = 0;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(6, 42);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(41, 12);
			this.label2.TabIndex = 0;
			this.label2.Text = "种子：";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 20);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(41, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "大小：";
			// 
			// groupBox5
			// 
			this.groupBox5.Controls.Add(this.lblStatus);
			this.groupBox5.Location = new System.Drawing.Point(574, 387);
			this.groupBox5.Name = "groupBox5";
			this.groupBox5.Size = new System.Drawing.Size(200, 57);
			this.groupBox5.TabIndex = 5;
			this.groupBox5.TabStop = false;
			this.groupBox5.Text = "状态";
			// 
			// lblStatus
			// 
			this.lblStatus.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lblStatus.Location = new System.Drawing.Point(3, 17);
			this.lblStatus.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
			this.lblStatus.Name = "lblStatus";
			this.lblStatus.Size = new System.Drawing.Size(194, 37);
			this.lblStatus.TabIndex = 0;
			this.lblStatus.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// MainDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(784, 562);
			this.Controls.Add(this.groupBox5);
			this.Controls.Add(this.groupBox4);
			this.Controls.Add(this.groupBox3);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.display);
			this.DoubleBuffered = true;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.Name = "MainDialog";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "迷宫问题 - [22121912]";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainDialog_FormClosing);
			this.Load += new System.EventHandler(this.MainDialog_Load);
			((System.ComponentModel.ISupportInitialize)(this.display)).EndInit();
			this.groupBox1.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			this.groupBox3.ResumeLayout(false);
			this.groupBox4.ResumeLayout(false);
			this.groupBox4.PerformLayout();
			this.groupBox5.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PictureBox display;
		private System.Windows.Forms.Button btnDeepFirst;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button btnHeuristicALG;
		private System.Windows.Forms.Button btnWideFirst;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Button btnSetting;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.Button btnDFS;
		private System.Windows.Forms.Button btnRP;
		private System.Windows.Forms.Button btnRand;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label lblSolve;
		private System.Windows.Forms.Label lblGen;
		private System.Windows.Forms.Label lblSeed;
		private System.Windows.Forms.Label lblSize;
		private System.Windows.Forms.GroupBox groupBox5;
		private System.Windows.Forms.Label lblStatus;
		private System.Windows.Forms.Button btnRD;
	}
}

