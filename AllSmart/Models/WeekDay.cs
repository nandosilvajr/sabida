using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AllSmart.Models
{
    public class WeekDay
    {
        public string Day { get; set; }

        public string DayLong { get; set; }

        public bool IsActive { get; set; }
    }

    public static class WeekDaysData
    {

        public static List<WeekDay> WeekDays { get; set; } = new List<WeekDay> {
           new WeekDay { Day = "Mon", DayLong = "Monday" },
           new WeekDay { Day = "Tue", DayLong = "Tuesday" },
           new WeekDay { Day = "Wed", DayLong = "Wednesday" },
           new WeekDay { Day = "Thu", DayLong = "Thursday" },
           new WeekDay { Day = "Fri", DayLong = "Friday" },
           new WeekDay { Day = "Sat", DayLong = "Saturday" },
           new WeekDay { Day = "Sun", DayLong = "Sunday" }
           };

        public static List<WeekDay> GetWeekDays()
        {
            return WeekDays;
        }
    }
}


