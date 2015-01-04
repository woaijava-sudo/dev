using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;

namespace SimuProcessSchd
{
	public class SchdFactory
	{
		static public IProcessSchd CreateSchdInstance(SchdType type)
		{
			return Assembly.GetExecutingAssembly().CreateInstance("SimuProcessSchd.Schd" + type.ToString()) as IProcessSchd;
		}
	}
}
