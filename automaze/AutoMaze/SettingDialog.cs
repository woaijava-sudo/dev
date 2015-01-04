using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AutoMaze
{
	public partial class SettingDialog : Form
	{
		public SettingDialog()
		{
			InitializeComponent();
		}
		private void SettingDialog_Load(object sender, EventArgs e)
		{
			GenerateSize();
		}

		void GenerateSize()
		{
			var range = Enumerable.Range(MazeSettings.MinSize, MazeSettings.MaxSize - MazeSettings.MinSize + 1);
			var array = range.Cast<object>().ToArray();
			comboBoxRow.Items.AddRange(array);
			comboBoxRow.SelectedItem = MazeSettings.CurrentRowSize;
			comboBoxCol.Items.AddRange(array);
			comboBoxCol.SelectedItem = MazeSettings.CurrentColSize;
			comboBoxBorder.Items.AddRange(Enumerable.Range(MazeSettings.MinBorderSize, MazeSettings.MaxBorderSize - MazeSettings.MinBorderSize + 1).Cast<object>().ToArray());
			comboBoxBorder.SelectedItem = MazeSettings.CurrentBorderSize;
			textBoxSeed.Text = MazeSettings.CurrentSeed.ToString();
		}

		void DisplaySize()
		{
			labelMazeSize.Text = string.Format("{0}x{1}", comboBoxRow.SelectedItem, comboBoxCol.SelectedItem);
		}

		bool SaveChanges()
		{
			try
			{
				MazeSettings.CurrentRowSize = int.Parse(comboBoxRow.SelectedItem.ToString());
				MazeSettings.CurrentColSize = int.Parse(comboBoxCol.SelectedItem.ToString());
				MazeSettings.CurrentBorderSize = int.Parse(comboBoxBorder.SelectedItem.ToString());
				MazeSettings.CurrentSeed = int.Parse(textBoxSeed.Text);
				return true;
			}
			catch (FormatException)
			{
				MessageBox.Show(this, "输入格式错误！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			catch (OverflowException)
			{
				MessageBox.Show(this, "输入值溢出！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}

			return false;
		}

		private void btnOK_Click(object sender, EventArgs e)
		{
			if (!SaveChanges())
				return;
			this.DialogResult = DialogResult.OK;
			Close();
		}

		private void btnCancel_Click(object sender, EventArgs e)
		{
			this.DialogResult = DialogResult.Cancel;
			Close();
		}

		private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
		{
			DisplaySize();
		}

		private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
		{
			DisplaySize();
		}

		private void comboBoxBorder_SelectedIndexChanged(object sender, EventArgs e)
		{
		}

		private void textBoxSeed_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (!Char.IsNumber(e.KeyChar) && e.KeyChar != '\n' && e.KeyChar != '\b')
				e.Handled = true;
		}
	}
}
