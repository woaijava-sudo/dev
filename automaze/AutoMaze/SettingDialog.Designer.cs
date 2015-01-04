namespace AutoMaze
{
	partial class SettingDialog
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.textBoxSeed = new System.Windows.Forms.TextBox();
			this.labelMazeSize = new System.Windows.Forms.Label();
			this.comboBoxCol = new System.Windows.Forms.ComboBox();
			this.comboBoxBorder = new System.Windows.Forms.ComboBox();
			this.comboBoxRow = new System.Windows.Forms.ComboBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.btnOK = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.textBoxSeed);
			this.groupBox1.Controls.Add(this.labelMazeSize);
			this.groupBox1.Controls.Add(this.comboBoxCol);
			this.groupBox1.Controls.Add(this.comboBoxBorder);
			this.groupBox1.Controls.Add(this.comboBoxRow);
			this.groupBox1.Controls.Add(this.label3);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.label5);
			this.groupBox1.Controls.Add(this.label4);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Location = new System.Drawing.Point(12, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(210, 197);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "局面";
			// 
			// textBoxSeed
			// 
			this.textBoxSeed.ImeMode = System.Windows.Forms.ImeMode.Disable;
			this.textBoxSeed.Location = new System.Drawing.Point(77, 159);
			this.textBoxSeed.MaxLength = 20;
			this.textBoxSeed.Name = "textBoxSeed";
			this.textBoxSeed.Size = new System.Drawing.Size(115, 21);
			this.textBoxSeed.TabIndex = 14;
			this.textBoxSeed.WordWrap = false;
			this.textBoxSeed.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxSeed_KeyPress);
			// 
			// labelMazeSize
			// 
			this.labelMazeSize.Location = new System.Drawing.Point(75, 97);
			this.labelMazeSize.Name = "labelMazeSize";
			this.labelMazeSize.Size = new System.Drawing.Size(99, 12);
			this.labelMazeSize.TabIndex = 0;
			// 
			// comboBoxCol
			// 
			this.comboBoxCol.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxCol.FormattingEnabled = true;
			this.comboBoxCol.Location = new System.Drawing.Point(77, 59);
			this.comboBoxCol.Name = "comboBoxCol";
			this.comboBoxCol.Size = new System.Drawing.Size(78, 20);
			this.comboBoxCol.TabIndex = 11;
			this.comboBoxCol.SelectedIndexChanged += new System.EventHandler(this.comboBox2_SelectedIndexChanged);
			// 
			// comboBoxBorder
			// 
			this.comboBoxBorder.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxBorder.FormattingEnabled = true;
			this.comboBoxBorder.Location = new System.Drawing.Point(77, 127);
			this.comboBoxBorder.Name = "comboBoxBorder";
			this.comboBoxBorder.Size = new System.Drawing.Size(78, 20);
			this.comboBoxBorder.TabIndex = 12;
			this.comboBoxBorder.SelectedIndexChanged += new System.EventHandler(this.comboBoxBorder_SelectedIndexChanged);
			// 
			// comboBoxRow
			// 
			this.comboBoxRow.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxRow.FormattingEnabled = true;
			this.comboBoxRow.Location = new System.Drawing.Point(77, 24);
			this.comboBoxRow.Name = "comboBoxRow";
			this.comboBoxRow.Size = new System.Drawing.Size(78, 20);
			this.comboBoxRow.TabIndex = 10;
			this.comboBoxRow.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(6, 62);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(65, 12);
			this.label3.TabIndex = 0;
			this.label3.Text = "垂直大小：";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(6, 97);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(65, 12);
			this.label2.TabIndex = 0;
			this.label2.Text = "局面大小：";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(6, 162);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(41, 12);
			this.label5.TabIndex = 0;
			this.label5.Text = "种子：";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(6, 130);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(65, 12);
			this.label4.TabIndex = 0;
			this.label4.Text = "边框组细：";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 27);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(65, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "水平大小：";
			// 
			// btnOK
			// 
			this.btnOK.Location = new System.Drawing.Point(8, 215);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(75, 23);
			this.btnOK.TabIndex = 5;
			this.btnOK.Text = "确定";
			this.btnOK.UseVisualStyleBackColor = true;
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.Location = new System.Drawing.Point(147, 215);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(75, 23);
			this.btnCancel.TabIndex = 6;
			this.btnCancel.Text = "取消";
			this.btnCancel.UseVisualStyleBackColor = true;
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// SettingDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(231, 247);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "SettingDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "设置";
			this.Load += new System.EventHandler(this.SettingDialog_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label labelMazeSize;
		private System.Windows.Forms.ComboBox comboBoxRow;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.ComboBox comboBoxCol;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ComboBox comboBoxBorder;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.TextBox textBoxSeed;
		private System.Windows.Forms.Label label5;
	}
}