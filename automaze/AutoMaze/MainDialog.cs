using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace AutoMaze
{
	public partial class MainDialog : Form
	{
		enum WorkerType
		{
			Gen,
			Solve
		}

		BackgroundWorker worker = new BackgroundWorker();

		public MainDialog()
		{
			InitializeComponent();
		}

		public Size DisplaySize { get { return display.Size; } }

		private void MainDialog_Load(object sender, EventArgs e)
		{
			InitWorker();
			Init(WorkerType.Gen);
		}

		void Init(WorkerType type)
		{
			if (worker.IsBusy)
			{
				MessageBox.Show(this, "任务正在运行！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			else
			{
				worker.RunWorkerAsync(type);
			}
		}

		void InitWorker()
		{
			worker.DoWork += worker_DoWork;
			worker.RunWorkerCompleted += worker_RunWorkerCompleted;
		}

		void worker_DoWork(object sender, DoWorkEventArgs e)
		{
			WorkerType type = (WorkerType)Enum.ToObject(typeof(WorkerType), e.Argument);
			var dict = new Dictionary<string, object>();
			dict["type"] = type;
			e.Result = dict;

			switch (type)
			{
				case WorkerType.Gen:
					{
						var maze = new Maze(display.Size);
						ShowStatus("正在生成地图");
						maze.GenerateMaze();
						MazeSettings.CurrentMaze = maze;
					}
					break;
				case WorkerType.Solve:
					{
						var maze = MazeSettings.CurrentMaze;
						if (maze == null)
							throw new Exception("地图未生成");
						ShowStatus("正在搜索路径");
						dict["result"] = maze.SolveMaze();
					}
					break;
			}
		}

		void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			if (e.Error != null)
			{
				MessageBox.Show(this, e.Error.Message, "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
			else if (e.Cancelled)
			{
				MessageBox.Show(this, "操作已被取消", "信息", MessageBoxButtons.OK, MessageBoxIcon.Information);
				return;
			}

			IDictionary<string, object> dict = e.Result as IDictionary<string, object>;
			if (dict != null)
			{
				WorkerType type = (WorkerType)Enum.ToObject(typeof(WorkerType), dict["type"]);
				switch (type)
				{
					case WorkerType.Gen:
						ShowStatus("生成完毕");
						break;
					case WorkerType.Solve:
						var result = dict["result"];
						ShowStatus("搜索完毕");
						MessageBox.Show(this, result.ToString(), "结果", MessageBoxButtons.OK, MessageBoxIcon.Information);
						break;
				}
			}
			else
			{
				ShowStatus("出现错误");
			}

			DrawMaze();
		}

		void DrawMaze()
		{
			var maze = MazeSettings.CurrentMaze;
			if (maze == null)
				return;

			maze.GetCurrentMazeInfo(dict =>
			{
				lblSize.Text = dict["size"].ToString();
				lblSeed.Text = dict["seed"].ToString();
				lblGen.Text = dict["gen"].ToString();
				lblSolve.Text = dict["solve"].ToString();
			});
			maze.DrawNormalMaze(a =>
			{
				display.Image = a;
				display.Refresh();
			});
		}

		private void btnDeepFirst_Click(object sender, EventArgs e)
		{
			SolveMazeInternal(Maze.SolveType.DFS);
		}

		private void btnWideFirst_Click(object sender, EventArgs e)
		{
			SolveMazeInternal(Maze.SolveType.BFS);
		}

		private void btnHeuristicALG_Click(object sender, EventArgs e)
		{
			SolveMazeInternal(Maze.SolveType.HEU);
		}

		void ShowStatus(string message)
		{
			Invoke(new Action(() => lblStatus.Text = message));
		}

		void GeneratMazeInternal(Maze.MazeType type)
		{
			if (MazeSettings.CurrentMazeType == type)
				MazeSettings.CurrentSeed = new Random().Next();

			MazeSettings.CurrentMazeType = type;
			Init(WorkerType.Gen);
		}

		void SolveMazeInternal(Maze.SolveType type)
		{
			MazeSettings.CurrentSolveType = type;
			Init(WorkerType.Solve);
		}

		private void btnDFS_Click(object sender, EventArgs e)
		{
			GeneratMazeInternal(Maze.MazeType.DFS);
		}

		private void btnRND_Click(object sender, EventArgs e)
		{
			GeneratMazeInternal(Maze.MazeType.RND);
		}

		private void btnBFS_Click(object sender, EventArgs e)
		{
			GeneratMazeInternal(Maze.MazeType.BFS);
		}
		private void btnREC_Click(object sender, EventArgs e)
		{
			GeneratMazeInternal(Maze.MazeType.REC);
		}

		private void MainDialog_FormClosing(object sender, FormClosingEventArgs e)
		{
			if (worker.IsBusy)
			{
				MessageBox.Show("任务正在运行！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
				e.Cancel = true;
			}
		}

		private void btnSetting_Click(object sender, EventArgs e)
		{
			if (worker.IsBusy)
			{
				MessageBox.Show(this, "任务正在运行！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}

			using (var dlg = new SettingDialog())
			{
				var result = dlg.ShowDialog(this);
				if (result != DialogResult.OK)
					return;

				Init(WorkerType.Gen);
			}
		}
	}
}
