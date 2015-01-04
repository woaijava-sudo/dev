using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimuProcessSchd
{
	public class SchdSettings
	{
		private static int _TimeSpan = 1;

		public static int TimeSpan { set { _TimeSpan = value; } get { return _TimeSpan; } }

		public static readonly int MaxTimeSpan = 10;
		public static readonly int MinTimeSpan = 1;
	}

	public class VisualSettings
	{
		public static readonly int TimeSpanWidth = 30;
		public static readonly int TaskHeight = 30;
		public static readonly int TaskSpacing = 20;
		public static readonly int TaskBlanking = 20;
		public static readonly int TimeHeight = 20;
		public static readonly int DegreeHeight = 5;
		public static readonly int DispTextWidth = 100;
		public static readonly int Margin = 5;
	}

	public class DuringStruct
	{
		public int start_time { set; get; }

		public int exec_time { set; get; }
	}

	public class SimulationResult
	{
		public List<SimpleTask> tasks { set; get; }

		public List<string> cpu_clips { set; get; }
	}

	public class VisualStruct
	{
		public DuringStruct during { set; get; }

		public List<Tuple<int, int>> cpu_clips { set; get; }
	}

	public class VisualData
	{
		public Dictionary<string, VisualStruct> visual { set; get; }

		public List<int> switches { set; get; }
	}
}
