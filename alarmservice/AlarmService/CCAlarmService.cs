using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Speech.Synthesis;
using System.Text;
using System.Threading;
using System.Runtime.InteropServices;

namespace AlarmService
{
    public partial class CCAlarmService : ServiceBase
    {
        private System.Timers.Timer timer = new System.Timers.Timer();
        private Thread speech_thread = null;

        private static void AlarmThread(object obj)
        {
            // 报时逻辑

            DateTime now = (DateTime)obj;

            if (now == null)
            {
                return;
            }

            string[] SPEECH_TIME = new string[] {
                    "午夜", // 23:30 - 00:30
                    "凌晨", // 00:30 - 06:30
                    "上午", // 06:30 - 11:00
                    "中午", // 11:00 - 12:30
                    "下午", // 12:30 - 18:00
                    "晚上", // 18:00 - 23:30
                };

            string SP_TIME = "";

            TimeSpan[] TIME_SPAN = new TimeSpan[] {
                    new TimeSpan(0, 30, 0),
                    new TimeSpan(6, 30, 0),
                    new TimeSpan(11, 30, 0),
                    new TimeSpan(12, 30, 0),
                    new TimeSpan(18, 0, 0),
                    new TimeSpan(23, 30, 0),
                };

            TimeSpan span = now - new DateTime(now.Year, now.Month, now.Day);

            if (span >= TIME_SPAN[5] || span < TIME_SPAN[0])
            {
                SP_TIME = SPEECH_TIME[0];
            }
            else if (span >= TIME_SPAN[0] && span < TIME_SPAN[1])
            {
                SP_TIME = SPEECH_TIME[1];
            }
            else if (span >= TIME_SPAN[1] && span < TIME_SPAN[2])
            {
                SP_TIME = SPEECH_TIME[2];
            }
            else if (span >= TIME_SPAN[2] && span < TIME_SPAN[3])
            {
                SP_TIME = SPEECH_TIME[3];
            }
            else if (span >= TIME_SPAN[3] && span < TIME_SPAN[4])
            {
                SP_TIME = SPEECH_TIME[4];
            }
            else if (span >= TIME_SPAN[4] && span < TIME_SPAN[5])
            {
                SP_TIME = SPEECH_TIME[5];
            }

            string ABOUT_TIME = "现在是 ";

            if (now.Hour == 0)
            {
                // 年月日
                ABOUT_TIME += now.ToString("y年 M月 d日 零点 请注意按时休息 亲");
            }
            else
            {
                // 时间
                if (now.Minute == 0)
                {
                    ABOUT_TIME += SP_TIME + now.ToString(" h点整");
                }
                else
                {
                    ABOUT_TIME += SP_TIME + now.ToString(" h点 m分");
                }
            }

            try
            {
	            using (SpeechSynthesizer reader = new SpeechSynthesizer())
                {
                    reader.SelectVoiceByHints(VoiceGender.Neutral);
                    reader.Volume = 100;
                    reader.SetOutputToDefaultAudioDevice();
                    reader.Speak(ABOUT_TIME);
                }
            }
            catch (System.Exception ex)
            {
                Trace.WriteLine("SpeechSynthesizer 出现错误： " + ex.Message);
            }
        }

        public CCAlarmService()
        {
            InitializeComponent();

            CanShutdown = true;
            CanPauseAndContinue = true;

            speech_thread = new Thread(AlarmThread);

            timer.Interval = 60000.0;            
        }

        private void StartTime(DateTime now)
        {
            new Thread((obj) =>
            {
                if (speech_thread.IsAlive)
                {
                    speech_thread.Join();
                }
                speech_thread = new Thread(AlarmThread);
                speech_thread.Start(obj);
            }).Start(now);
        }

        protected override void OnStart(string[] args)
        {
            int skip = 15;
            if (args.Length >= 1)
            {
                try
                {
                    skip = int.Parse(args[0]);
                }
                catch (System.Exception)
                {
                	
                }
                if (skip > 60 || skip < 1)
                {
                    skip = 15;
                }
            }

            timer.Elapsed += (sender, e) =>
            {

                timer.Stop();

                DateTime now = e.SignalTime;

                if ((now.Minute % skip) == 0)
                {
                    StartTime(now);
                }

                timer.Start();
            };

            StartTime(DateTime.Now);

            timer.Start();
        }

        protected override void OnStop()
        {
            timer.Stop();
        }

        protected override void OnPause()
        {
            timer.Stop();

            if (speech_thread.IsAlive)
            {
                speech_thread.Join();
            }
        }

        protected override void OnContinue()
        {
            if (speech_thread.IsAlive)
            {
                speech_thread.Join();
            }

            StartTime(DateTime.Now);

            timer.Start();
        }

        protected override void OnShutdown()
        {
            timer.Stop();

            if (speech_thread.IsAlive)
            {
                speech_thread.Join();
            }
        }
    }
}
