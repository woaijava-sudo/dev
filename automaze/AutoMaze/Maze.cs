using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutoMaze
{
	public class Maze
	{
		private readonly int MinArea = 4; // 最小分割面积
		private readonly int RecConnect = 7; // 分割打通概率分母
		private readonly int DoorBlank = 6; // 门的空白区域

		public const byte Closed = 0;
		public const byte Open = 1;
		public const byte RightBranch = 10; // 正确分支
		public const byte WrongBranch = 11; // 错误分支
		public const byte EndBranch = 12; // 终结分支
		public const byte StartFlag = 100; // 起点
		public const byte EndFlag = 101; // 终点

		public enum DirectionType
		{
			Left,
			Right,
			Up,
			Down
		}

		public enum MazeType
		{
			DFS,
			RND,
			BFS,
			REC
		}

		public enum SolveType
		{
			NIL,
			DFS,
			BFS,
			HEU
		}

		byte[,] MapRowData, MapColData;
		Rectangle MapRect;
		RectangleF MapRectF;
		SizeF SizeZoomF;
		Size MapSize;

		IEnumerable<float> MapRows, MapCols;

		Size MzSize;
		Bitmap MzBitmap;
		Graphics MzGraphics;
		byte[,] MzMap;
		byte[,] MzSolveMap = null;
		List<Point> MzSolvePath = null;

		readonly Point PtStart;
		readonly Point PtEnd;

		Random rand;

		public Maze(Size size)
		{
			MapSize = new Size(MazeSettings.CurrentRowSize, MazeSettings.CurrentColSize);
			MapRowData = new byte[MapSize.Width, MapSize.Height - 1]; // 横向门 --
			MapColData = new byte[MapSize.Width - 1, MapSize.Height]; // 纵向门 ||
			MapRect = new Rectangle(Point.Empty, MapSize);
			MzSize = size;
			MzBitmap = new Bitmap(size.Width, size.Height);
			MzGraphics = Graphics.FromImage(MzBitmap);
			MapRectF = new RectangleF(Point.Empty, MzSize);
			MapRectF.Inflate(-MazeSettings.CurrentBorderSize, -MazeSettings.CurrentBorderSize);
			PtStart = Point.Empty;
			PtEnd = new Point(MapSize.Width - 1, MapSize.Height - 1); 
			GenerateGridLines();
			GenerateMaze();
		}

		public void GetCurrentMazeInfo(Action<IDictionary<string, object>> action)
		{
			var dict = new Dictionary<string, object>();
			dict["size"] = string.Format("{0}x{1}", MazeSettings.CurrentRowSize, MazeSettings.CurrentColSize);
			dict["seed"] = MazeSettings.CurrentSeed;
			dict["gen"] = GetCurrentMazeDesc();
			dict["solve"] = GetCurrentSolveDesc();
			action(dict);
		}

		string GetCurrentMazeDesc()
		{
			switch (MazeSettings.CurrentMazeType)
			{
				case MazeType.DFS:
					return "深度优先遍历";
				case MazeType.RND:
					return "随机";
				case MazeType.BFS:
					return "广度优先遍历";
				case MazeType.REC:
					return "递归分割";
			}
			return "未知";
		}

		string GetCurrentSolveDesc()
		{
			switch (MazeSettings.CurrentSolveType)
			{
				case SolveType.DFS:
					return "深度优先搜索";
				case SolveType.BFS:
					return "广度优先搜索";
				case SolveType.HEU:
					return "启发式搜索";
			}
			return "未知";
		}

		void GenerateMazeZero()
		{
			for (int col = 0; col < MapSize.Width; col++)
				for (int row = 0; row < MapSize.Height; row++)
					MzMap[col, row] = Open;
		}

		void GenerateMazeFromMapToState()
		{
			for (int col = 0; col < MapSize.Width; col++)
				for (int row = 0; row < MapSize.Height - 1; row++)
					if (MzMap[col, row] == Open && MzMap[col, row + 1] == Open)
						SetDoorState(new Point(col, row), DirectionType.Down, Open);
			for (int col = 0; col < MapSize.Width - 1; col++)
				for (int row = 0; row < MapSize.Height; row++)
					if (MzMap[col, row] == Open && MzMap[col + 1, row] == Open)
						SetDoorState(new Point(col, row), DirectionType.Right, Open);
		}

		public void GenerateMaze()
		{
			rand = new Random(MazeSettings.CurrentSeed);
			MzMap = new byte[MapSize.Width, MapSize.Height];
			switch (MazeSettings.CurrentMazeType)
			{
				case MazeType.DFS:
					GenerateMazeDFS();
					break;
				case MazeType.RND:
					GenerateMazeRND();
					break;
				case MazeType.BFS:
					GenerateMazeBFS();
					break;
				case MazeType.REC:
					GenerateMazeREC();
					break;
			}
		}

		private void GenerateMazeDFS()
		{
			int BlockNum = 0;
			var PtCurrent = PtStart;
			var PtBlock = new DirectionType[4];
			var stack = new Stack<Point>();
			MzMap[PtStart.X, PtStart.Y] = Open;

			do
			{
				// 检测周围有没有未连通的格子
				BlockNum = 0;

				// 检查左侧
				if (PtCurrent.X > 0 && MzMap[PtCurrent.X - 1, PtCurrent.Y] == Closed)
				{
					PtBlock[BlockNum] = DirectionType.Left;
					BlockNum++;
				}

				// 检查右侧
				if (PtCurrent.X < MapSize.Width - 1 && MzMap[PtCurrent.X + 1, PtCurrent.Y] == Closed)
				{
					PtBlock[BlockNum] = DirectionType.Right;
					BlockNum++;
				}

				// 检查上方
				if (PtCurrent.Y > 0 && MzMap[PtCurrent.X, PtCurrent.Y - 1] == Closed)
				{
					PtBlock[BlockNum] = DirectionType.Up;
					BlockNum++;
				}

				// 检查下方
				if (PtCurrent.Y < MapSize.Height - 1 && MzMap[PtCurrent.X, PtCurrent.Y + 1] == Closed)
				{
					PtBlock[BlockNum] = DirectionType.Down;
					BlockNum++;
				}

				// 选出下一个当前格
				if (BlockNum > 0)
				{
					// 随机选择一个邻格
					BlockNum = rand.Next(BlockNum);

					// 把当前格入栈
					stack.Push(PtCurrent);

					// 连通邻格，并将邻格指定为当前格
					SetDoorState(PtCurrent, PtBlock[BlockNum], Open);
					switch (PtBlock[BlockNum])
					{
						case DirectionType.Left: PtCurrent.X--; break;
						case DirectionType.Right: PtCurrent.X++; break;
						case DirectionType.Up: PtCurrent.Y--; break;
						case DirectionType.Down: PtCurrent.Y++; break;
					}

					// 标记当前格
					MzMap[PtCurrent.X, PtCurrent.Y] = Open;
				}
				else if (stack.Count > 0)
				{
					// 将栈顶作为当前格
					PtCurrent = stack.Pop();
				}
			}
			while (stack.Count > 0);
		}

		private void GenerateMazeRND()
		{
			for (int col = 0; col < MapSize.Width; col++)
				for (int row = 0; row < MapSize.Height - 1; row++)
					if (rand.Next(2) == 1)
						MapRowData[col, row] = Open;
			for (int col = 0; col < MapSize.Width - 1; col++)
				for (int row = 0; row < MapSize.Height; row++)
					if (rand.Next(2) == 1)
						MapColData[col, row] = Open;
		}

		private void GenerateMazeBFS()
		{
			GenerateMazeBFS_Internal(0, 0, MapSize.Width, MapSize.Height);
		}

		private void GenerateMazeBFS_Internal(int x, int y, int cx, int cy)
		{
			int BlockIndex = 0; // 随机墙的索引
			var BlockPos = new List<Tuple<Point, DirectionType>>();
			var PtCurrent = PtStart;
			MzMap[PtStart.X, PtStart.Y] = Open;

			// 记录邻墙
			if (PtCurrent.X > 0)
			{
				BlockPos.Add(new Tuple<Point, DirectionType>(PtCurrent, DirectionType.Left));
			}
			if (PtCurrent.Y > 0)
			{
				BlockPos.Add(new Tuple<Point, DirectionType>(PtCurrent, DirectionType.Up));
			}
			if (PtCurrent.X < cx - 1)
			{
				BlockPos.Add(new Tuple<Point, DirectionType>(PtCurrent, DirectionType.Right));
			}
			if (PtCurrent.Y < cy - 1)
			{
				BlockPos.Add(new Tuple<Point, DirectionType>(PtCurrent, DirectionType.Down));
			}

			while (BlockPos.Count > 0)
			{
				// 随机选一面墙
				BlockIndex = rand.Next(BlockPos.Count);
				var _Object = BlockPos[BlockIndex];

				// 找出此墙对面的目标格
				switch (_Object.Item2)
				{
					case DirectionType.Left:
						PtCurrent.X = _Object.Item1.X - 1;
						PtCurrent.Y = _Object.Item1.Y;
						break;
					case DirectionType.Up:
						PtCurrent.X = _Object.Item1.X;
						PtCurrent.Y = _Object.Item1.Y - 1;
						break;
					case DirectionType.Right:
						PtCurrent.X = _Object.Item1.X + 1;
						PtCurrent.Y = _Object.Item1.Y;
						break;
					case DirectionType.Down:
						PtCurrent.X = _Object.Item1.X;
						PtCurrent.Y = _Object.Item1.Y + 1;
						break;
				}

				// 如果目标格尚未连通
				if (MzMap[PtCurrent.X, PtCurrent.Y] == Closed)
				{
					// 连通目标格
					MzMap[PtCurrent.X, PtCurrent.Y] = Open;
					SetDoorState(_Object.Item1, _Object.Item2, Open);

					// 添加目标格的邻格
					if (PtCurrent.X > 0 && MzMap[PtCurrent.X - 1, PtCurrent.Y] == Closed)
					{
						BlockPos.Add(new Tuple<Point, DirectionType>(PtCurrent, DirectionType.Left));
					}
					if (PtCurrent.Y > 0 && MzMap[PtCurrent.X, PtCurrent.Y - 1] == Closed)
					{
						BlockPos.Add(new Tuple<Point, DirectionType>(PtCurrent, DirectionType.Up));
					}
					if (PtCurrent.X < cx - 1 && MzMap[PtCurrent.X + 1, PtCurrent.Y] == Closed)
					{
						BlockPos.Add(new Tuple<Point, DirectionType>(PtCurrent, DirectionType.Right));
					}
					if (PtCurrent.Y < cy - 1 && MzMap[PtCurrent.X, PtCurrent.Y + 1] == Closed)
					{
						BlockPos.Add(new Tuple<Point, DirectionType>(PtCurrent, DirectionType.Down));
					}
				}

				// 移除此墙
				BlockPos.RemoveAt(BlockIndex);
			}
		}

		private void GenerateMazeREC()
		{
			GenerateMazeZero();
			GenerateMazeREC_Recusive(0, 0, MapSize.Width - 1, MapSize.Height - 1);
			GenerateMazeREC_RandomConnect();
			GenerateMazeFromMapToState();
		}

		private void GenerateMazeREC_RandomConnect()
		{
			for (int col = 0; col < MapSize.Width; col++)
				for (int row = 0; row < MapSize.Height; row++)
					if (MzMap[col, row] == Closed && rand.Next(RecConnect) == 0)
							MzMap[col, row] = Open;
		}

		private void GenerateMazeREC_Recusive(int x1, int y1, int x2, int y2)
		{
			// 要分隔区域的尺寸
			var size = new Size(x2 - x1, y2 - y1);
			
			// 检查是否达到了最小分割面积
			if (MinArea > 3 && size.Width * size.Height <= MinArea)
					return;
			
			// 假设分割点不存在
			var pt = new Point(-1, -1);
			
			// 产生随机分割点
			if (size.Width > 1)
			{
				pt.X = x1 + 1 + rand.Next(size.Width / 2) * 2;
			}
			if (size.Height > 1)
			{
				pt.Y = y1 + 1 + rand.Next(size.Height / 2) * 2;
			}
			
			// 没有继续分割的必要
			if (pt.X == -1 && pt.Y == -1)
			{
				return;
			}
			
			// 补上墙壁
			if (pt.X != -1)
			{
				for (var y = y1; y <= y2; y++)
				{
					MzMap[pt.X, y] = Closed;
				}
			}
			if (pt.Y != -1)
			{
				for (var x = x1; x <= x2; x++)
				{
					MzMap[x, pt.Y] = Closed;
				}
			}
			
			// 为确保连通，随机打通墙壁且不产生环路，并递归分割子区域
			if (pt.X != -1 && pt.Y != -1)
			{
				var side = rand.Next(4);
				if (side != 0)
				{
					MzMap[pt.X, y1 + rand.Next((pt.Y - 1 - y1) / 2 + 1) * 2] = Open;
				}
				if (side != 1)
				{
					MzMap[pt.X + 1 + rand.Next(((x2 - pt.X - 1) / 2 + 1)) * 2, pt.Y] = Open;
				}
				if (side != 2)
				{
					MzMap[pt.X, pt.Y + 1 + rand.Next((y2 - pt.Y - 1) / 2 + 1) * 2] = Open;
				}
				if (side != 3)
				{
					MzMap[x1 + rand.Next((pt.X - 1 - x1) / 2 + 1) * 2, pt.Y] = Open;
				}
				GenerateMazeREC_Recusive(x1, y1, pt.X - 1, pt.Y - 1);
				GenerateMazeREC_Recusive(pt.X + 1, y1, x2, pt.Y - 1);
				GenerateMazeREC_Recusive(pt.X + 1, pt.Y + 1, x2, y2);
				GenerateMazeREC_Recusive(x1, pt.Y + 1, pt.X - 1, y2);
			}
			else if (pt.X == -1)
			{
				MzMap[x1 + rand.Next(size.Width / 2 + 1) * 2, pt.Y] = Open;
				GenerateMazeREC_Recusive(x1, y1, x2, pt.Y - 1);
				GenerateMazeREC_Recusive(x1, pt.Y + 1, x2, y2);
			}
			else if (pt.Y == -1)
			{
				MzMap[pt.X, y1 + rand.Next(size.Height / 2 + 1) * 2] = Open;
				GenerateMazeREC_Recusive(x1, y1, pt.X - 1, y2);
				GenerateMazeREC_Recusive(pt.X + 1, y1, x2, y2);
			}
		}

		public string SolveMaze()
		{
			MzSolveMap = new byte[MapSize.Width, MapSize.Height];
			MzSolvePath = new List<Point>();
			switch (MazeSettings.CurrentSolveType)
			{
				case SolveType.DFS:
					return SolveMazeDFS();
				case SolveType.BFS:
					return SolveMazeBFS();
				case SolveType.HEU:
					return SolveMazeHEU();
			}

			return "Unknown Solve Type";
		}

		string SolveMazeXFSInternal(Action<List<NormalNode>, NormalNode> action)
		{
			var Open_Table = new List<NormalNode>();
			var Closed_Table = new List<NormalNode>();
			var Answer = new NormalNode();
			var cx = MapSize.Width - 1;
			var cy = MapSize.Height - 1;

			var Start = PtStart;
			var End = PtEnd;
			Debug.Assert(Start != End);

			Open_Table.Add(new NormalNode() { Coordinate = Start, Parent = null });
			NormalNode PathEndNode = null;

			var Opens = 0;
			var Closeds = 0;
			long tick = 0;

			var watch = new Stopwatch();
			watch.Start();

			while (Open_Table.Count > 0)
			{
				var ExpandNode = Open_Table.First();
				Open_Table.RemoveAt(0);

				if (ExpandNode.Coordinate == End)
				{
					PathEndNode = ExpandNode;
					Opens = Open_Table.Count;
					Closeds = Closed_Table.Count;
					tick = watch.ElapsedTicks;
				}

				var PtCurrent = ExpandNode.Coordinate;

				var cnt = 0;
				if (PtCurrent.X > 0 && GetDoorState(PtCurrent, DirectionType.Left) == Open && MzSolveMap[PtCurrent.X - 1, PtCurrent.Y] == Closed)
				{
					action(Open_Table, new NormalNode() { Coordinate = new Point(PtCurrent.X - 1, PtCurrent.Y), Parent = ExpandNode }); MzSolveMap[PtCurrent.X - 1, PtCurrent.Y] = WrongBranch; cnt++;
				}
				if (PtCurrent.Y > 0 && GetDoorState(PtCurrent, DirectionType.Up) == Open && MzSolveMap[PtCurrent.X, PtCurrent.Y - 1] == Closed)
				{
					action(Open_Table, new NormalNode() { Coordinate = new Point(PtCurrent.X, PtCurrent.Y - 1), Parent = ExpandNode }); MzSolveMap[PtCurrent.X, PtCurrent.Y - 1] = WrongBranch; cnt++;
				}
				if (PtCurrent.X < cx && GetDoorState(PtCurrent, DirectionType.Right) == Open && MzSolveMap[PtCurrent.X + 1, PtCurrent.Y] == Closed)
				{
					action(Open_Table, new NormalNode() { Coordinate = new Point(PtCurrent.X + 1, PtCurrent.Y), Parent = ExpandNode }); MzSolveMap[PtCurrent.X + 1, PtCurrent.Y] = WrongBranch; cnt++;
				}
				if (PtCurrent.Y < cy && GetDoorState(PtCurrent, DirectionType.Down) == Open && MzSolveMap[PtCurrent.X, PtCurrent.Y + 1] == Closed)
				{
					action(Open_Table, new NormalNode() { Coordinate = new Point(PtCurrent.X, PtCurrent.Y + 1), Parent = ExpandNode }); MzSolveMap[PtCurrent.X, PtCurrent.Y + 1] = WrongBranch; cnt++;
				}

				watch.Stop();
				if (cnt == 0)
				{
					MzSolveMap[PtCurrent.X, PtCurrent.Y] = EndBranch;
					var it = ExpandNode.Parent as NormalNode;
					while (it != null)
					{
						if (MzSolveMap[it.Coordinate.X, it.Coordinate.Y] == WrongBranch)
						{
							it.Branches--;
							if (it.Branches != 0)
								break;
						}
						it = it.Parent as NormalNode;
					}
				}
				else if (cnt == 1)
				{
					MzSolveMap[PtCurrent.X, PtCurrent.Y] = Open;
				}
				else
				{
					MzSolveMap[PtCurrent.X, PtCurrent.Y] = WrongBranch;
					ExpandNode.Branches = cnt;
				}
				watch.Start();

				Closed_Table.Add(ExpandNode);
			}

			watch.Stop();

			if (PathEndNode == null)
			{
				return "搜索失败，没有最终解";
			}

			MzSolveMap[Start.X, Start.Y] = StartFlag;
			MzSolveMap[End.X, End.Y] = EndFlag;

			var iter = PathEndNode;
			while (iter != null)
			{
				var pt = iter.Coordinate;
				if (MzSolveMap[pt.X, pt.Y] == WrongBranch)
					MzSolveMap[pt.X, pt.Y] = RightBranch;
				MzSolvePath.Insert(0, pt);
				iter = iter.Parent as NormalNode;
			}

			return string.Format(
@"步数：{0}
OPEN表总数：{1}
CLOSED表总数：{2}
所耗时钟周期：{3}"
				, MzSolvePath.Count - 1, Opens, Closeds, tick);
		}

		private string SolveMazeDFS()
		{
			return SolveMazeXFSInternal((a, b) => a.Insert(0, b));
		}

		private string SolveMazeBFS()
		{
			return SolveMazeXFSInternal((a, b) => a.Add(b));
		}

		private string SolveMazeHEU()
		{
			return SolveMazeHEU_Internal();
		}

		private string SolveMazeHEU_Internal()
		{
			var Open_Table = new List<HeuNode>();
			var Closed_Table = new List<HeuNode>();
			var Answer = new HeuNode();
			var cx = MapSize.Width - 1;
			var cy = MapSize.Height - 1;

			var Start = PtStart;
			var End = PtEnd;
			Debug.Assert(Start != End);

			Open_Table.Add(new HeuNode() { Coordinate = Start, Parent = null, Level = 0, Cost = End.X + End.Y - Start.X - Start.Y });
			NormalNode PathEndNode = null;

			var Opens = 0;
			var Closeds = 0;
			long tick = 0;

			var watch = new Stopwatch();
			watch.Start();

			while (Open_Table.Count > 0)
			{
				Open_Table.Sort(new Comparison<HeuNode>((a, b) => a.Cost - b.Cost));

				var ExpandNode = Open_Table.First();
				Open_Table.RemoveAt(0);

				var PossibleResult = Open_Table.Where(a => a.Cost == ExpandNode.Cost).ToList().Find(a => a.Coordinate == End);
				if (PossibleResult != null)
				{
					ExpandNode = PossibleResult;
				}

				if (ExpandNode.Coordinate == End)
				{
					PathEndNode = ExpandNode;
					Opens = Open_Table.Count;
					Closeds = Closed_Table.Count;
					tick = watch.ElapsedTicks;
				}

				var PtCurrent = ExpandNode.Coordinate;
				var TempList = new List<HeuNode>();

				if (PtCurrent.X > 0 && GetDoorState(PtCurrent, DirectionType.Left) == Open)
				{
					TempList.Add(new HeuNode() { Coordinate = new Point(PtCurrent.X - 1, PtCurrent.Y)});
				}
				if (PtCurrent.Y > 0 && GetDoorState(PtCurrent, DirectionType.Up) == Open)
				{
					TempList.Add(new HeuNode() { Coordinate = new Point(PtCurrent.X, PtCurrent.Y - 1)});
				}
				if (PtCurrent.X < cx && GetDoorState(PtCurrent, DirectionType.Right) == Open)
				{
					TempList.Add(new HeuNode() { Coordinate = new Point(PtCurrent.X + 1, PtCurrent.Y)});
				}
				if (PtCurrent.Y < cy && GetDoorState(PtCurrent, DirectionType.Down) == Open)
				{
					TempList.Add(new HeuNode() { Coordinate = new Point(PtCurrent.X, PtCurrent.Y + 1)});
				}

				var cnt = 0;
				TempList.ForEach(a =>
				{
					a.Parent = ExpandNode;
					a.Level = ExpandNode.Level + 1;
					a.Cost = a.Level + End.X + End.Y - a.Coordinate.X - a.Coordinate.Y;

					var open_found = Open_Table.Find(b => b.Coordinate == a.Coordinate);
					var closed_found = open_found == null ? Closed_Table.Find(b => b.Coordinate == a.Coordinate) : null;
					var found = open_found ?? closed_found;
					if (found == null)
					{
						Open_Table.Add(a);
						cnt++;
					}
					else if (found.Cost > a.Cost)
					{
						found.Cost = a.Cost;
						found.Parent = ExpandNode;
						found.Level = a.Level;
						if (closed_found != null)
						{
							Open_Table.Add(found);
							Closed_Table.Remove(found);
						}
					}
				});

				watch.Stop();
				if (cnt == 0)
				{
					MzSolveMap[PtCurrent.X, PtCurrent.Y] = EndBranch;
					var it = ExpandNode.Parent as NormalNode;
					while (it != null)
					{
						if (MzSolveMap[it.Coordinate.X, it.Coordinate.Y] == WrongBranch)
						{
							it.Branches--;
							if (it.Branches != 0)
								break;
						}
						it = it.Parent as NormalNode;
					}
				}
				else if (cnt == 1)
				{
					MzSolveMap[PtCurrent.X, PtCurrent.Y] = Open;
				}
				else
				{
					MzSolveMap[PtCurrent.X, PtCurrent.Y] = WrongBranch;
					ExpandNode.Branches = cnt;
				}
				watch.Start();

				Closed_Table.Add(ExpandNode);
			}

			watch.Stop();

			if (PathEndNode == null)
			{
				return "搜索失败，没有最终解";
			}

			MzSolveMap[Start.X, Start.Y] = StartFlag;
			MzSolveMap[End.X, End.Y] = EndFlag;

			var iter = PathEndNode;
			while (iter != null)
			{
				var pt = iter.Coordinate;
				if (MzSolveMap[pt.X, pt.Y] == WrongBranch)
					MzSolveMap[pt.X, pt.Y] = RightBranch;
				MzSolvePath.Insert(0, pt);
				iter = iter.Parent as NormalNode;
			}

			return string.Format(
@"步数：{0}
OPEN表总数：{1}
CLOSED表总数：{2}
所耗时钟周期：{3}"
				, MzSolvePath.Count - 1, Opens, Closeds, tick);
		}

		void ValidPoint(Point point)
		{
			if (!MapRect.Contains(point))
				throw new ArgumentOutOfRangeException("point");
		}

		Point MakePoint(Point point, DirectionType direction)
		{
			switch (direction)
			{
				case DirectionType.Left:
					return new Point(point.X - 1, point.Y);
				case DirectionType.Right:
					return new Point(point.X + 1, point.Y);
				case DirectionType.Up:
					return new Point(point.X, point.Y - 1);
				case DirectionType.Down:
					return new Point(point.X, point.Y + 1);
			}

			throw new ArgumentException("direction");
		}

		public void SetDoorState(Point point, DirectionType direction, byte newState)
		{
			ValidPoint(point);
			Point newPoint = MakePoint(point, direction);
			ValidPoint(newPoint);

			switch (direction)
			{
				case DirectionType.Left:
					MapColData[point.X - 1, point.Y] = newState; break;
				case DirectionType.Right:
					MapColData[point.X, point.Y] = newState; break;
				case DirectionType.Up:
					MapRowData[point.X, point.Y - 1] = newState; break;
				case DirectionType.Down:
					MapRowData[point.X, point.Y] = newState; break;
			}
		}

		public byte GetDoorState(Point point, DirectionType direction)
		{
			ValidPoint(point);
			Point newPoint = MakePoint(point, direction);
			ValidPoint(newPoint);

			switch (direction)
			{
				case DirectionType.Left:
					return MapColData[point.X - 1, point.Y];
				case DirectionType.Right:
					return MapColData[point.X, point.Y];
				case DirectionType.Up:
					return MapRowData[point.X, point.Y - 1];
				case DirectionType.Down:
					return MapRowData[point.X, point.Y];
			}

			throw new ArgumentException("direction");
		}

		PointF GetHalfLinePoint(Point point)
		{
			return new PointF((MapCols.ElementAt(point.X) + MapCols.ElementAt(point.X + 1)) / 2, (MapRows.ElementAt(point.Y) + MapRows.ElementAt(point.Y + 1)) / 2);
		}

		void GenerateGridLines()
		{
			SizeZoomF = new SizeF(MapRectF.Width / MapSize.Width, MapRectF.Height / MapSize.Height);
			MapCols = Enumerable.Range(0, MapSize.Width + 1).ToList().ConvertAll(
				new Converter<int, float>(a => MapRectF.X + a * SizeZoomF.Width) 
				);
			MapRows = Enumerable.Range(0, MapSize.Height + 1).ToList().ConvertAll(
				new Converter<int, float>(a => MapRectF.Y + a * SizeZoomF.Height)
				);
			MapRectF.Location = MapRectF.Location;
			MapRectF.Width = MapSize.Width * SizeZoomF.Width;
			MapRectF.Height = MapSize.Height * SizeZoomF.Height;
		}

		void DrawBGBorder()
		{
			using (var border = new Pen(Color.Black, MazeSettings.CurrentBorderSize))
			{
				MapRows.ToList().ForEach(a => MzGraphics.DrawLine(border, new PointF(MapRectF.X, a), new PointF(MapRectF.Right, a)));
				MapCols.ToList().ForEach(a => MzGraphics.DrawLine(border, new PointF(a, MapRectF.Y), new PointF(a, MapRectF.Bottom)));
			}
		}

		void DrawBGDotted()
		{
			using (var dotted = new Pen(Color.Gray, 1))
			{
				dotted.DashStyle = DashStyle.Dash;
				MapRows.ToList().Aggregate((a, n) =>
				{
					var x = (a + n) / 2;
					MzGraphics.DrawLine(dotted, new PointF(MapRectF.X, x), new PointF(MapRectF.Right, x));
					return n;
				});
				MapCols.ToList().Aggregate((a, n) =>
				{
					var x = (a + n) / 2;
					MzGraphics.DrawLine(dotted, new PointF(x, MapRectF.Y), new PointF(x, MapRectF.Bottom));
					return n;
				});
			}
		}

		void DrawFGRowMask(Pen empty, int x, int y)
		{
			if ((x >= 0 && x < MapSize.Width) && (y >= 0 && y <= MapSize.Height))
			{
				var X = MapCols.ElementAt(x);
				var Y = MapRows.ElementAt(y);
				var Blank = SizeZoomF.Width / DoorBlank;
				MzGraphics.DrawLine(empty, new PointF(X + Blank, Y), new PointF(X + SizeZoomF.Width - Blank, Y));
			}
		}

		void DrawFGColMask(Pen empty, int x, int y)
		{
			if ((x >= 0 && x <= MapSize.Width) && (y >= 0 && y < MapSize.Height))
			{
				var X = MapCols.ElementAt(x);
				var Y = MapRows.ElementAt(y);
				var Blank = SizeZoomF.Height / DoorBlank;
				MzGraphics.DrawLine(empty, new PointF(X, Y + Blank), new PointF(X, Y + SizeZoomF.Height - Blank));
			}
		}

		private void DrawFGDoorInternal(Pen empty)
		{
			for (int col = 0; col < MapSize.Width; col++)
				for (int row = 0; row < MapSize.Height - 1; row++)
					if (MapRowData[col, row] == Open)
						DrawFGRowMask(empty, col, row + 1);
			for (int col = 0; col < MapSize.Width - 1; col++)
				for (int row = 0; row < MapSize.Height; row++)
					if (MapColData[col, row] == Open)
						DrawFGColMask(empty, col + 1, row);
		}

		void DrawFGDoor()
		{
			using (var empty = new Pen(Color.WhiteSmoke, MazeSettings.CurrentBorderSize + 2))
			{
				DrawFGColMask(empty, 0, 0);
				DrawFGColMask(empty, MapSize.Width, MapSize.Height - 1);
				DrawFGDoorInternal(empty);
			}
		}

		void DrawFGPoint()
		{
			using (SolidBrush purple = new SolidBrush(Color.Purple),
						red = new SolidBrush(Color.Red),
						green = new SolidBrush(Color.Green),
						darkred = new SolidBrush(Color.DarkRed))
			{
				for (int col = 0; col < MapSize.Width; col++)
				{
					for (int row = 0; row < MapSize.Height; row++)
					{
						SolidBrush brush = null;
						switch (MzSolveMap[col, row])
						{
							case StartFlag:
							case EndFlag:
								brush = purple;
								break;
							case RightBranch:
								brush = green;
								break;
							case WrongBranch:
								brush = red;
								break;
							case EndBranch:
								brush = darkred;
								break;
						}
						if (brush != null)
						{
							RectangleF rect = new RectangleF(GetHalfLinePoint(new Point(col, row)), SizeF.Empty);
							rect.Inflate(new SizeF(SizeZoomF.Width / 4, SizeZoomF.Height / 4));
							MzGraphics.FillEllipse(brush, rect);
						}
					}
				}
			}
		}

		void DrawFGPathInternal()
		{
			using (var blue = new Pen(Color.Blue, 3))
			{
				var _Path = MzSolvePath.ConvertAll(new Converter<Point, PointF>(a => a));
				_Path.Aggregate(GetHalfLinePoint(MzSolvePath.First()), (a, n) =>
				{
					var _New = GetHalfLinePoint(new Point((int)n.X, (int)n.Y));
					MzGraphics.DrawLine(blue, a, _New);
					return _New;
				});
			}
		}

		void DrawUnvisitedRoom()
		{
			if (MzSolvePath == null || MzSolvePath.Count == 0)
				return;

			SolidBrush brush = new SolidBrush(Color.LightGray);
			for (int col = 0; col < MapSize.Width; col++)
				for (int row = 0; row < MapSize.Height; row++)
					if (MzSolveMap[col, row] == Closed)
					{
						RectangleF rect = new RectangleF(new PointF(MapCols.ElementAt(col), MapRows.ElementAt(row)), SizeZoomF);
						rect.Inflate(new SizeF(-1, -1));
						MzGraphics.FillRectangle(brush, rect);
					}
		}

		void DrawFGPath()
		{
			if (MzSolvePath == null || MzSolvePath.Count == 0)
				return;

			DrawFGPathInternal();
			DrawFGPoint();
		}

		public void DrawNormalMaze(Action<Bitmap> action)
		{
			MzGraphics.Clear(Color.WhiteSmoke);
			DrawUnvisitedRoom();
			DrawBGBorder();
			DrawBGDotted();
			DrawFGDoor();
			DrawFGPath();
			action(MzBitmap);
		}
	}

	internal class NodeBase
	{
		public Point Coordinate { set; get; }

		public NodeBase Parent { set; get; }
	}

	internal class NormalNode : NodeBase
	{
		public int Branches { set; get; }
	}

	internal class HeuNode : NormalNode
	{
		public int Level { set; get; }
		
		public int Cost { set; get; }
	}
}
