using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Reflection;
using System.Text;

namespace SimuProcessSchd
{
	public enum SchdType
	{
		[Description("先来先服务")]
		FCFS,
		[Description("最短作业优先")]
		SJF,
		[Description("最短剩余时间优先")]
		SRTF,
		[Description("最高响应比优先")]
		HRRF,
		[Description("优先级调度")]
		PRIOR,
		[Description("轮转调度")]
		RR,
	}

	static public class SimuHelpers
	{
		static public DataView InitSelectedData()
		{
			var dt = new DataTable();
			dt.TableName = "调度算法";
			dt.Columns.Add("Key", typeof(string));
			dt.Columns.Add("Value", typeof(SchdType));
			GetTypeFields().ToList().ForEach(a =>
			{
				dt.Rows.Add(new object[]
				{
					a.ConvertTo<DescriptionAttribute>().Description,
					a.GetRawConstantValue()
				});
			});
			return dt.DefaultView;
		}

		static public IEnumerable<FieldInfo> GetTypeFields()
		{
			return from x in typeof(SchdType).GetFields()
				   where x.FieldType == typeof(SchdType)
				   select x;
		}

		static public IEnumerable<PropertyInfo> GetTaskFields()
		{
			return from x in typeof(SimpleTask).GetProperties()
				   select x;
		}

		static public T ConvertTo<T>(this FieldInfo info) where T : Attribute
		{
			return Attribute.GetCustomAttribute(info, typeof(T)) as T;
		}

		static public T ConvertTo<T>(this PropertyInfo info) where T : Attribute
		{
			return Attribute.GetCustomAttribute(info, typeof(T)) as T;
		}
	}

	public class RumtimeSchd
	{
		DataTable table;
		IEnumerable<SimpleTask> tasks;
		Action<string> action;
		IEnumerable<FieldInfo> type_fields;
		IEnumerable<PropertyInfo> task_fields;

		public RumtimeSchd(DataTable table)
		{
			this.table = table;
			type_fields = SimuHelpers.GetTypeFields();
			task_fields = SimuHelpers.GetTaskFields();
		}

		public Bitmap GetSchdBitmap(SchdType type, Action<string> action)
		{
			this.action = action;
			tasks = ConvertTableToTask();
			if (tasks.Count() == 0)
				throw new Exception("任务为空！");

			ReportTask(type);
			var schd = SchdFactory.CreateSchdInstance(type);
			schd.ImportData(tasks);
			schd.SetCallback(action);
			var result = schd.Simulate();
			var visual = ConvertResultToVisual(result);
			ReportStat(visual);
			var bitmap = DrawSchd(visual);
			return bitmap;
		}

		void ReportTask(SchdType type)
		{
			var current_type = (from x in type_fields
							   where x.Name == type.ToString()
							   select x).Single();
			action(string.Format("选择调度算法 = {0} | {1}",
				(Attribute.GetCustomAttribute(current_type, typeof(DescriptionAttribute)) as DescriptionAttribute).Description,
				current_type.Name));
			StringBuilder sb = new StringBuilder();
			sb.Append('-', 50);
			var split = sb.ToString();
			action(split);
			tasks.ToList().ForEach(a =>
			{
				sb.Clear();
				task_fields.ToList().ForEach(b =>
				{
					sb.AppendFormat("{0} = {1}  ", b.ConvertTo<DescriptionAttribute>().Description, b.GetValue(a, null).ToString().PadRight(3));
				});
				action(sb.ToString());
			});
			action(split);
		}

		void ReportStat(VisualData visual)
		{
			var new_list = from task in tasks
					join vs in visual.visual
					on task.Name equals vs.Key
					select new
					{
						// 期望时间（执行时间）
						PredictTime = (double)task.ExecuteTime,
						// 实际执行时间（周转时间）
						ExecuteTime = (double)vs.Value.during.exec_time
					};
			var T = (from list in new_list
					select list.ExecuteTime).Average();
			var W = (from list in new_list
					 select list.ExecuteTime / list.PredictTime).Average();
			action(new string('-', 50));
			action(string.Format("平均周转时间 = {0:f}", T));
			action(string.Format("平均带权周转时间 = {0:f}", W));
		}

		IEnumerable<SimpleTask> ConvertTableToTask()
		{
			var result = new List<SimpleTask>();
			for (int i = 0; i < table.Rows.Count; i++)
			{
				var new_task = new SimpleTask();
				task_fields.ToList().ForEach(a =>
				{
					a.SetValue(new_task, table.Rows[i][a.Name], null);
				});
				result.Add(new_task);
			}
			return result;
		}

		VisualData ConvertResultToVisual(SimulationResult result)
		{
			if (result == null)
				throw new NullReferenceException("结果为空！");

			var convert_result = new Dictionary<string, VisualStruct>();
			result.tasks.ForEach(a =>
				convert_result[a.Name] = new VisualStruct
				{
					during = new DuringStruct(),
					cpu_clips = new List<Tuple<int, int>>()
				});

			var clips = result.cpu_clips.ToList();
			var swth = new List<int>();
			string last = null;
			int start = 0, exec = 0;

			clips.Add(null);
			for (int i = 0; i < clips.Count; i++)
			{
				string current = clips[i];
				if (current != null)
				{
					if (last == null)
					{
						start = i; exec = 1;
					}
					else if (current == last)
					{
						exec++;
					}
				}
				if (last != null && current != last)
				{
					action(string.Format("T = {0}   Execute {1}   During = {2}   End = {3}",
						start.ToString().PadRight(3), last.ToString().PadRight(3),
						exec.ToString().PadRight(3), (start + exec).ToString().PadRight(3)));
					convert_result[last].cpu_clips.Add(new Tuple<int, int>(start, exec));
					start = i; exec = 1;
					swth.Add(i);
				}
				last = current;
			}

			foreach (var kv in convert_result)
			{
				var end = kv.Value.cpu_clips.Last();
				var task = result.tasks.Find(a => a.Name == kv.Key);
				kv.Value.during.start_time = task.StartTime;
				kv.Value.during.exec_time = end.Item1 + end.Item2 - task.StartTime;
			}

			return new VisualData() { visual = convert_result, switches = swth };
		}

		Bitmap DrawSchd(VisualData result)
		{
			var max_time = result.visual.Values.ToList().ConvertAll(new Converter<VisualStruct, int>(a => a.during.start_time + a.during.exec_time)).Max();
			Size size = new Size(VisualSettings.Margin * 2 + VisualSettings.DispTextWidth + VisualSettings.TimeSpanWidth * (max_time + 1),
				result.visual.Count * (VisualSettings.TaskHeight + VisualSettings.TaskSpacing) + VisualSettings.TaskBlanking + VisualSettings.TimeHeight * 2);
			Bitmap bitmap = new Bitmap(size.Width, size.Height);
			Graphics graphics = Graphics.FromImage(bitmap);
			graphics.Clear(Color.WhiteSmoke);
			{
				int w = 0, h = 0;
				var font = new Font("Microsoft Yahei", 12);
				var format = new StringFormat() { Alignment = StringAlignment.Near, LineAlignment = StringAlignment.Center };
				var brush = new SolidBrush(Color.Black);
				var blue = new SolidBrush(Color.Cyan);
				var gray = new SolidBrush(Color.LightGray);
				var pen = new Pen(Color.Black);
				// 主体
				foreach (var fk in result.visual)
				{
					w = VisualSettings.Margin;
					h += VisualSettings.TaskSpacing;
					// 任务名
					graphics.DrawString(fk.Key, font, brush, new Rectangle(w, h, VisualSettings.DispTextWidth, VisualSettings.TaskHeight), format);
					w += VisualSettings.DispTextWidth + fk.Value.during.start_time * VisualSettings.TimeSpanWidth;
					var fill_rect = new Rectangle(w, h, fk.Value.during.exec_time * VisualSettings.TimeSpanWidth, VisualSettings.TaskHeight);
					// 任务主体
					graphics.FillRectangle(gray, fill_rect);
					var clips = fk.Value.cpu_clips;
					w = VisualSettings.Margin + VisualSettings.DispTextWidth;
					foreach (var tp in clips)
					{
						// CPU
						var rt2 = new Rectangle(w + tp.Item1 * VisualSettings.TimeSpanWidth, h,
							tp.Item2 * VisualSettings.TimeSpanWidth, VisualSettings.TaskHeight);
						graphics.FillRectangle(blue, rt2);
						graphics.DrawRectangle(pen, rt2);
					}
					graphics.DrawRectangle(pen, fill_rect);
					h += VisualSettings.TaskHeight;
				}
				// 刻度
				w = VisualSettings.Margin;
				h += VisualSettings.TaskBlanking;
				graphics.DrawLine(pen, w, h, size.Width - VisualSettings.Margin, h);
				w += VisualSettings.DispTextWidth;
				// 虚线
				{
					var p = new Pen(Color.Black);
					p.DashPattern = new float[] { 9, 3 };
					var hh = result.visual.Count * (VisualSettings.TaskHeight + VisualSettings.TaskSpacing) + VisualSettings.TaskBlanking;
					foreach (var i in result.switches)
					{
						var ww = w + i * VisualSettings.TimeSpanWidth;
						graphics.DrawLine(p, ww, 0, ww, hh);
					}
				}
				// 坐标
				format.Alignment = StringAlignment.Center;
				for (int i = 0; i <= max_time; i++)
				{
					var pt = new Point(w, h);
					var pt1 = new Point(w, h + VisualSettings.DegreeHeight);
					graphics.DrawLine(pen, pt, pt1);
					pt.Offset(0, VisualSettings.TimeHeight + VisualSettings.TimeHeight / 2);
					var rt = new Rectangle(pt, Size.Empty);
					rt.Inflate(VisualSettings.TimeSpanWidth / 2, VisualSettings.TimeHeight / 2);
					rt.Inflate(-2, -2);
					graphics.DrawString(i.ToString(), font, brush, rt, format);
					w += VisualSettings.TimeSpanWidth;
				}
			}
			return bitmap;
		}
	}
}
