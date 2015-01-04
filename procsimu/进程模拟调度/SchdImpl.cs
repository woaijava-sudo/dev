using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimuProcessSchd
{
	/// <summary>
	/// 先来先服务
	/// </summary>
	public class SchdFCFS : ProcessSchd
	{
		public override SimulationResult Simulate()
		{
			var result = new SimulationResult() { tasks = origin_tasks.ToList() };
			result.cpu_clips = SchdImplHelper.GetSimuResultDirectV1(
				from x in origin_tasks
				orderby x.StartTime
				select new SchdRestModel
				{
					task = x,
					rest = x.ExecuteTime,
				},
				true,
				(a, i) =>
					from x in a
					where x.task.StartTime <= i
					select x
				);
			return result;
		}
	}

	/// <summary>
	/// 最短作业优先
	/// </summary>
	public class SchdSJF : ProcessSchd
	{
		public override SimulationResult Simulate()
		{
			var result = new SimulationResult() { tasks = origin_tasks.ToList() };
			result.cpu_clips = SchdImplHelper.GetSimuResultDirectV1(
				from x in origin_tasks
				orderby x.StartTime
				select new SchdRestModel
				{
					task = x,
					rest = x.ExecuteTime,
				},
				true,
				(a, i) =>
					from x in a
					where x.task.StartTime <= i
					orderby x.task.ExecuteTime
					select x
				);
			return result;
		}
	}

	/// <summary>
	/// 最短剩余时间优先
	/// </summary>
	public class SchdSRTF : ProcessSchd
	{
		public override SimulationResult Simulate()
		{
			var result = new SimulationResult() { tasks = origin_tasks.ToList() };
			result.cpu_clips = SchdImplHelper.GetSimuResultDirectV1(
				from x in origin_tasks
				orderby x.ExecuteTime
				select new SchdRestModel
				{
					task = x,
					rest = x.ExecuteTime,
				},
				false,
				(a, i) =>
					from x in a
					where x.task.StartTime <= i
					orderby x.rest
					select x
				);
			return result;
		}
	}

	/// <summary>
	/// 最高响应比优先
	/// </summary>
	public class SchdHRRF : ProcessSchd
	{
		public override SimulationResult Simulate()
		{
			var result = new SimulationResult() { tasks = origin_tasks.ToList() };
			result.cpu_clips = SchdImplHelper.GetSimuResultDirectV1(
				from x in origin_tasks
				orderby x.StartTime
				select new SchdRestModel
				{
					task = x,
					rest = x.ExecuteTime,
				},
				true,
				(a, i) =>
 					from y in
 						(from x in a
 						where x.task.StartTime <= i
 						select new SchdRestModel
 						{
 							task = x.task,
 							rest = x.rest,
 							temp = ((double)i - (double)x.task.StartTime) / (double)x.task.ExecuteTime
 						})
					orderby y.temp descending
 					select y
				);
			return result;
		}
	}

	public class SchdPRIOR : ProcessSchd
	{
		public override SimulationResult Simulate()
		{
			var result = new SimulationResult() { tasks = origin_tasks.ToList() };
			result.cpu_clips = SchdImplHelper.GetSimuResultDirectV1(
				from x in origin_tasks
				orderby x.StartTime
				select new SchdRestModel
				{
					task = x,
					rest = x.ExecuteTime,
				},
				false,
				(a, i) =>
					(from x in a
					 where x.task.StartTime <= i
					 orderby x.task.Priority
					 select x)
				);
			return result;
		}
	}

	public class SchdRR : ProcessSchd
	{
		public override SimulationResult Simulate()
		{
			var result = new SimulationResult() { tasks = origin_tasks.ToList() };
			result.cpu_clips = SchdImplHelper.GetSimuResultDirectV2(
				from x in origin_tasks
				select new SchdRestModel
				{
					task = x,
					rest = x.ExecuteTime,
				});
			return result;
		}
	}
}
