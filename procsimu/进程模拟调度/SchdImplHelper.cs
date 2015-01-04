using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimuProcessSchd
{
	public class SchdImplHelper
	{
		static public List<string> GetSimuResultDirectV1(IEnumerable<SchdRestModel> task, bool blocked, Func<IEnumerable<SchdRestModel>, int, IEnumerable<SchdRestModel>> select)
		{
			var list = new List<string>();
			var new_list = task.ToList();
			IEnumerable<SchdRestModel> selected = null;
			for (int i = 0; new_list.Count() > 0; i++)
			{
				if (!blocked || (blocked && selected == null))
					selected = select(new_list, i);
				if (selected.Count() > 0)
				{
					var running = selected.First();
					list.Add(running.task.Name);
					if (running.rest > 1)
					{
						new_list.Find(a => a.task.Name == running.task.Name).rest--;
					}
					else
					{
						if (selected != null)
							selected = null;
						new_list.RemoveAll(a => a.task.Name == running.task.Name);
					}
				}
				else
				{
					if (selected != null)
						selected = null;
					list.Add(null);
				}
			}
			return list;
		}

		static public List<string> GetSimuResultDirectV2(IEnumerable<SchdRestModel> task)
		{
			var list = new List<string>();
			var backup = task.ToList(); // 后备
			var round = new List<SchdRestModel>(); // 轮转
			for (int i = 0; backup.Count > 0 || round.Count > 0; i++)
			{
				SchdRestModel current = null;

				// 检查后备
				if (round.Count == 0)
				{
					round.AddRange(
						from x in backup
						where x.task.StartTime <= i
						orderby x.task.StartTime
						orderby x.task.Priority
						select x);
					backup.RemoveAll(a => a.task.StartTime <= i);
				}
				
				// 处理轮转
				if (round.Count > 0)
				{
					current = round.First();
					round.RemoveAt(0);
				}

				// 取出队头并处理
				if (current != null)
				{
					var min = Math.Min(current.rest, SchdSettings.TimeSpan);
					current.rest -= min;
					for (int j = 0; j < min; j++)
					{
						list.Add(current.task.Name);
					}
					if (current.rest <= 0)
					{
						current = null;
					}
					i += min - 1;
				}
				else
				{
					list.Add(null);
				}

				// 处理后备
				{
					round.AddRange(
						from x in backup
						where x.task.StartTime <= i + 1
						orderby x.task.StartTime
						orderby x.task.Priority
						select x);
					backup.RemoveAll(a => a.task.StartTime <= i + 1);
				}

				// 将当前任务放至队尾
				if (current != null)
				{
					round.Add(current);
				}
			}
			return list;
		}
	}
}
