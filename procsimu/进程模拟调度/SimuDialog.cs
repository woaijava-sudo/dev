using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SimuProcessSchd
{
	public partial class SimuDialog : Form
	{
		BackgroundWorker worker = new BackgroundWorker();
		RuntimeData data = new RuntimeData();

		public SimuDialog()
		{
			InitializeComponent();
		}

		private void SimuDialog_Load(object sender, EventArgs e)
		{
			comboBox1.DataSource = SimuHelpers.InitSelectedData();
			comboBox1.DisplayMember = "Key";
			comboBox1.ValueMember = "Value";
			comboBox2.Items.AddRange(Enumerable.Range(SchdSettings.MinTimeSpan, SchdSettings.MaxTimeSpan - SchdSettings.MinTimeSpan + 1).Cast<object>().ToArray());
			comboBox2.SelectedIndex = 0;
			InitWorker();
		}

		private void InitWorker()
		{
			worker.DoWork += worker_DoWork;
			worker.RunWorkerCompleted += worker_RunWorkerCompleted;
		}

		void worker_DoWork(object sender, DoWorkEventArgs e)
		{
			var type = (SchdType)Enum.ToObject(typeof(SchdType), e.Argument);
			e.Result = new RumtimeSchd(data.GetView().ToTable()).GetSchdBitmap(type, a => Invoke(new Action(() => listBox1.Items.Add(a))));
		}

		void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			EnableAllControls(true);

			if (e.Error != null)
			{
				MessageBox.Show(this, e.Error.Message, "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}

			var bitmap = e.Result as Bitmap;
			if (bitmap == null)
			{
				MessageBox.Show(this, "画图过程出错", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}

			pictureBox1.Image = bitmap;
			pictureBox1.Refresh();
		}

		private bool TestWorker(int type = 0)
		{
			if (worker.IsBusy)
			{
				if (type == 0)
					MessageBox.Show(this, "正在进行仿真，暂不支持任何控制操作！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
				else if (type == 1)
					MessageBox.Show(this, "当前没有仿真操作！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}
			return true;
		}

		private void EnableAllControls(bool enable)
		{
			comboBox1.Enabled = enable;
			comboBox2.Enabled = enable;
			btnStart.Enabled = enable;
			btnSetting.Enabled = enable;
		}

		private void btnStart_Click(object sender, EventArgs e)
		{
			if (!TestWorker())
				return;

			EnableAllControls(false);
			listBox1.Items.Clear();
			worker.RunWorkerAsync(comboBox1.SelectedValue);
		}

		private void btnSetting_Click(object sender, EventArgs e)
		{
			if (!TestWorker())
				return;

			using (var settings = new SettingDialog(data.GetView()))
			{
				settings.ShowDialog(this);
			}
		}

		private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
		{
			SchdSettings.TimeSpan = int.Parse(comboBox2.SelectedItem.ToString());
		}
	}
}
