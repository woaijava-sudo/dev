using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimuProcessSchd
{
	public class SchdImplHelper
	{
		/// <summary>
		/// 通用调度算法版本一
		/// </summary>
		/// <param name="task">运行时处理队列</param>
		/// <param name="blocked">是否独占式调度（非剥夺）</param>
		/// <param name="select">自定义任务竞取函数</param>
		/// <returns>模拟调度结果</returns>
		static public List<string> GetSimuResultDirectV1(IEnumerable<SchdRestModel> task, bool blocked, Func<IEnumerable<SchdRestModel>, int, IEnumerable<SchdRestModel>> select)
		{
			var list = new List<string>();//用于存放结果
			var new_list = task.ToList();//当前任务队列的拷贝，用于调度
			IEnumerable<SchdRestModel> selected = null;
			//进行运行时调度，当前队列为空时结束，i为时刻
			for (int i = 0; new_list.Count() > 0; i++)
			{
				//当规定为可剥夺时，初始化任务竞取函数，用于选取下一任务
				if (!blocked || (blocked && selected == null))
					selected = select(new_list, i);
				if (selected.Count() > 0)//可以选取下一任务
				{
					var running = selected.First();//取下一任务队列头
					list.Add(running.task.Name);//当前时刻处理该任务并记录任务名
					if (running.rest > 1)//当前任务还有剩余运行时间
					{
						//当前运行的任务的剩余时间减一
						new_list.Find(a => a.task.Name == running.task.Name).rest--;
					}
					else//当前任务已完成
					{
						if (selected != null)
							selected = null;//清空竞取函数
						//从调度队列中除去该任务
						new_list.RemoveAll(a => a.task.Name == running.task.Name);
					}
				}
				else//已经没有任务可选取，收尾工作
				{
					if (selected != null)
						selected = null;
					list.Add(null);
				}
			}
			return list;
		}

		/// <summary>
		/// 通用调度算法版本二
		/// </summary>
		/// <param name="task">起始的任务队列</param>
		/// <returns>模拟调度结果</returns>
		static public List<string> GetSimuResultDirectV2(IEnumerable<SchdRestModel> task)
		{
			var list = new List<string>();
			//采用双队列，后备和轮转
			var backup = task.ToList(); // 后备队列
			var round = new List<SchdRestModel>(); // 轮转队列
			//当双队列都不为空时，继续运行，i为时刻
			for (int i = 0; backup.Count > 0 || round.Count > 0; i++)
			{
				SchdRestModel current = null;

				// 检查后备
				if (round.Count == 0)//当前轮转队列为空时，取后备队列
				{
					round.AddRange(
						from x in backup
						where x.task.StartTime <= i
						orderby x.task.StartTime//按起始时间
						orderby x.task.Priority//按优先级
						select x);
					backup.RemoveAll(a => a.task.StartTime <= i);//做移动操作，将旧的删除
				}
				
				// 处理轮转
				if (round.Count > 0)//轮转不为空，则有任务
				{
					current = round.First();//取队列头
					round.RemoveAt(0);//清除
				}

				// 取出队头并处理
				if (current != null)
				{
					var min = Math.Min(current.rest, SchdSettings.TimeSpan);//轮转时独占时长的限制
					current.rest -= min;//减去当前运行时间
					for (int j = 0; j < min; j++)
					{
						list.Add(current.task.Name);//添加运行记录
					}
					if (current.rest <= 0)
					{
						current = null;//没有剩余时间则丢弃
					}
					i += min - 1;//时刻表推进，由于for循环对i有自增操作，所以这里减一
				}
				else
				{
					list.Add(null);//结束标记
				}

				// 再次处理后备
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
