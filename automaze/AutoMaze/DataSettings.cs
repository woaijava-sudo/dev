using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoMaze
{
	public static class MazeSettings
	{
		public static readonly int MinSize = 4;
		public static readonly int MaxSize = 30;
		public static readonly int MinBorderSize = 1;
		public static readonly int MaxBorderSize = 20;
		public static readonly int DefaultSize = 4;
		public static readonly int DefaultSeed = 0;
		public static readonly int DefaultBorderSize = 2;
		public static readonly Maze.MazeType DefaultMazeType = Maze.MazeType.DFS;
		public static readonly Maze.SolveType DefaultSolveType = Maze.SolveType.NIL;

		static object lck = new object();

		static int currentRowSize = DefaultSize;
		static int currentColSize = DefaultSize;
		static int currentBorderSize = DefaultBorderSize;
		static Maze.MazeType currentMazeType = DefaultMazeType;
		static Maze.SolveType currentSolveType = DefaultSolveType;
		static Maze currentMaze = null;

		public static int CurrentRowSize
		{
			get
			{
				return currentRowSize;
			}
			set
			{
				if (currentRowSize <= MaxSize && currentRowSize >= MinSize)
					currentRowSize = value;
			}
		}

		public static int CurrentColSize
		{
			get
			{
				return currentColSize;
			}
			set
			{
				if (currentColSize <= MaxSize && currentColSize >= MinSize)
					currentColSize = value;
			}
		}

		static int currentSeed = DefaultSeed;
		public static int CurrentSeed
		{
			get
			{
				return currentSeed;
			}
			set
			{
				currentSeed = value;
			}
		}

		public static int CurrentBorderSize
		{
			get
			{
				return currentBorderSize;
			}
			set
			{
				if (currentBorderSize <= MaxBorderSize && currentBorderSize >= MinBorderSize)
					currentBorderSize = value;
			}
		}

		public static Maze.MazeType CurrentMazeType
		{
			get
			{
				return currentMazeType;
			}
			set
			{
				currentMazeType = value;
			}
		}

		public static Maze.SolveType CurrentSolveType
		{
			get
			{
				return currentSolveType;
			}
			set
			{
				currentSolveType = value;
			}
		}

		public static Maze CurrentMaze
		{
			get
			{
				lock (lck)
				{
					return currentMaze;
				}
			}
			set
			{
				lock (lck)
				{
					currentMaze = value;
				}
			}
		}
	}
}
