using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models
{
    /// <summary>
    /// Class <c>PillCompartmentModel</c> model for the buttons that users can click to select their
    /// medication schedule inside PillSchedulerView
    /// </summary>
    public class PillCompartmentModel : BaseModel
    {
        #region Properties
        public bool Selected
        {
            get { return _Selected; }
            set { _Selected = value; NotifyPropertyChanged("Selected"); }
        }
        private bool _Selected;

        public bool Enabled
        {
            get { return _Enabled; }
            set { _Enabled = value; NotifyPropertyChanged("Enabled"); }
        }
        private bool _Enabled = true;

        public int Display
        {
            get { return _Display; }
            set { _Display = value; NotifyPropertyChanged("Display"); }
        }
        private int _Display;

        public CompartmentScheduleModel Pill
        {
            get { return _Pill; }
            set { _Pill = value; NotifyPropertyChanged("Pill"); }
        }
        private CompartmentScheduleModel _Pill;

        public bool OwnsSelection { get { return Selected & Enabled; } }

        #endregion Properties
    }
}
