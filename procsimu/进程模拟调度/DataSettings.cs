using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;

namespace SimuProcessSchd
{
	class UniqueAttribute : Attribute
	{
	}

	public class SimpleTask
	{
		[Unique]
		[DefaultValue("Task")]
		[Description("作业名")]
		public string Name { set; get; }

		[DefaultValue(0)]
		[Description("开始时间")]
		public int StartTime { set; get; }

		[DefaultValue(1)]
		[Description("运行时间")]
		public int ExecuteTime { set; get; }

		[DefaultValue(0)]
		[Description("优先级")]
		public int Priority { set; get; }
	}

	public class RuntimeData
	{
		DataTable data = new DataTable();

		public RuntimeData()
		{
			InitTestTable();
		}

		public DataView GetView()
		{
			return data.DefaultView;
		}

		void InitTestTable()
		{
			data.TableName = "任务设置";
			SimuHelpers.GetTaskFields().ToList().ForEach(a =>
			 {
				 var new_column = new DataColumn();
				 new_column.ColumnName = a.Name;
				 new_column.DataType = a.PropertyType;
				 new_column.AllowDBNull = false;
				 {
					 var attr = a.ConvertTo<DescriptionAttribute>();
					 if (attr != null) new_column.Caption = attr.Description;
				 }
				 {
					 var attr = a.ConvertTo<DefaultValueAttribute>();
					 if (attr != null) new_column.DefaultValue = attr.Value;
				 }
				 {
					 var attr = a.ConvertTo<UniqueAttribute>();
					 if (attr != null) new_column.Unique = true;
				 }

				 data.Columns.Add(new_column);
			 });
			InitTestData();
		}

		void InitTestData()
		{
			data.Rows.Add(new object[] { "A", 0, 3, 5 });
			data.Rows.Add(new object[] { "B", 2, 6, 2 });
			data.Rows.Add(new object[] { "C", 4, 4, 3 });
			data.Rows.Add(new object[] { "D", 6, 5, 4 });
			data.Rows.Add(new object[] { "E", 8, 2, 1 });
		}
	}
}
