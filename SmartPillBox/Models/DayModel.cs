using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models
{
    /// <summary>
    /// Class <c>DayModel</c> models a day within the calendar-like view from MedicationHistoryView
    /// </summary>
    public class DayModel : BaseModel
    {
        #region Properties
        public DateTime Date
        {
            get { return _Date; }
            set { _Date = value; NotifyPropertyChanged("Date"); }
        }
        private DateTime _Date;

        public String Month { get { return Date.ToString("MMM"); } }

        public String PillName
        {
            get { return _PillName; }
            set { _PillName = value; NotifyPropertyChanged("PillName"); }
        }
        private String _PillName;

        public bool Taken
        {
            get { return _Taken; }
            set { _Taken = value; NotifyPropertyChanged("Taken"); }
        }
        private bool _Taken = true;

        public System.Windows.Media.SolidColorBrush Background
        {
            get 
            {
                if (Taken)
                    return System.Windows.Media.Brushes.White;
                else
                    return System.Windows.Media.Brushes.Red;
            }
        }
        #endregion
    }
}
