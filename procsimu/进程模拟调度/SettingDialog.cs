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
	public partial class SettingDialog : Form
	{
		DataView data_view;

		public SettingDialog(DataView view)
		{
			InitializeComponent();

			data_view = view;
		}

		private void SettingDialog_Load(object sender, EventArgs e)
		{
			dataGridView1.DataSource = data_view;
			for (int i = 0; i < dataGridView1.ColumnCount; i++)
				dataGridView1.Columns[i].HeaderText = data_view.Table.Columns[i].Caption;
		}

		private void dataGridView1_DataError(object sender, DataGridViewDataErrorEventArgs e)
		{
			MessageBox.Show(this, e.Exception.Message, "数据验证错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
		}

		private void button1_Click(object sender, EventArgs e)
		{
			Close();
		}

		private void dataGridView1_CellValidating(object sender, DataGridViewCellValidatingEventArgs e)
		{
			switch (e.ColumnIndex)
			{
				case 1:
					{
						int i;
						if (int.TryParse(e.FormattedValue.ToString(), out i))
							if (i < 0)
							{
								e.Cancel = true;
								dataGridView1.Rows[e.RowIndex].ErrorText = "开始时间必须是非负数";
							}
					}
					break;
				case 2:
					{
						int i;
						if (int.TryParse(e.FormattedValue.ToString(), out i))
							if (i <= 0)
							{
								e.Cancel = true;
								dataGridView1.Rows[e.RowIndex].ErrorText = "运行时间必须是正数";
							}
					}
					break;
				default:
					break;
			}
		}
	}
}
