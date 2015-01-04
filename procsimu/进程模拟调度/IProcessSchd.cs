using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimuProcessSchd
{
	public interface IProcessSchd
	{
		void ImportData(IEnumerable<SimpleTask> tasks);

		void SetCallback(Action<string> action);

		SimulationResult Simulate();
	}

	public abstract class ProcessSchd : IProcessSchd
	{
		protected IEnumerable<SimpleTask> origin_tasks;
		protected Action<string> action;

		public void ImportData(IEnumerable<SimpleTask> tasks)
		{
			origin_tasks = tasks;
		}

		public void SetCallback(Action<string> action)
		{
			this.action = action;
		}

		public abstract SimulationResult Simulate();
	}
}
