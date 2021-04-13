using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SmartPillBox.Models;

namespace SmartPillBox.ViewModels
{
    /// <summary>
    /// Class <c>PillScheduleViewModel</c> the viewmodel which serves as the datacontext for the
    /// PillScheduleView
    /// </summary>
    class PillScheduleViewModel : BaseViewModel
    {
        public PillSchedulerViewModel PillScheduler
        {
            get { return _PillScheduler; }
            set { _PillScheduler = value; NotifyPropertyChanged("PillScheduler"); }
        }
        private PillSchedulerViewModel _PillScheduler;

        public ObservableCollection<CompartmentScheduleModel> Compartments
        {
            get { return _Compartments; }
            set { _Compartments = value; NotifyPropertyChanged("Compartments"); }
        }
        private ObservableCollection<CompartmentScheduleModel> _Compartments;

        public PillScheduleViewModel(BaseViewModel pills)
        {
            PillScheduler = (PillSchedulerViewModel) pills;
            Compartments = MakeCompartmentSchedule();
        }

        /// <summary>
        /// Method <c></c>
        /// </summary> MakeCompartmentSchedule Creates a new CompartmentSchedule that is used to represent
        /// the smart pillbox within the PillScheduleView
        /// <returns></returns>
        private ObservableCollection<CompartmentScheduleModel> MakeCompartmentSchedule()
        {
            ObservableCollection<CompartmentScheduleModel> comps = new ObservableCollection<CompartmentScheduleModel>();
            foreach(PillCompartmentModel comp in PillScheduler.BoxShape.Shape)
            {
                if(comp.Pill == null) 
                    comps.Add(new CompartmentScheduleModel()); 
                else
                    comps.Add(comp.Pill);
            }

            return comps;
        }

        /// <summary>
        /// Debugging method to Print the Compartment schedule to the console
        /// </summary>
        private void PrintCompartmentSchedule()
        {
            Console.WriteLine("--------------------");
            foreach (CompartmentScheduleModel compPill in Compartments)
            {
                Console.WriteLine(compPill.PillName);
                Console.WriteLine(compPill.PillTime);
                Console.WriteLine(compPill.PillNote);
                Console.WriteLine("--------------------");
            }
        }



        /*
        public Dictionary<String, ScheduleDay> PillSchedule
        {
            get { return _PillSchedule; }
            set { _PillSchedule = value; NotifyPropertyChanged("PillSchedule"); }
        }
        private Dictionary<String, ScheduleDay> _PillSchedule;


        public ObservableCollection<CompartmentScheduleModel> Compartments
        {
            get { return _Compartments; }
            set { _Compartments = value; NotifyPropertyChanged("Compartments"); }
        }
        private ObservableCollection<CompartmentScheduleModel> _Compartments;

        public ObservableCollection<PillModel> Pills
        {
            get { return _Pills; }
            set { _Pills = value; NotifyPropertyChanged("Pills"); }
        }
        private ObservableCollection<PillModel> _Pills;

        public int Rows
        {
            get { return _Rows; }
            set { _Rows = value; NotifyPropertyChanged("Rows"); }
        }
        private int _Rows;

        #region Constructor
        public PillScheduleViewModel(ObservableCollection<PillModel> pills)
        {
            PillSchedule = MakeBlankPillSchedule();
            Pills = pills;
        }
        #endregion

        public Dictionary<String, ScheduleDay> MakeBlankPillSchedule()
        {
            Dictionary<String, ScheduleDay> week = new Dictionary<String, ScheduleDay>(7);
            week.Add("Sunday", new ScheduleDay("Sunday"));
            week.Add("Monday", new ScheduleDay("Monday"));
            week.Add("Tuesday", new ScheduleDay("Tuesday"));
            week.Add("Wednesday", new ScheduleDay("Wednesday"));
            week.Add("Thursday", new ScheduleDay("Thursday"));
            week.Add("Friday", new ScheduleDay("Friday"));
            week.Add("Saturday", new ScheduleDay("Saturday"));

            return week;
        }

        public ObservableCollection<CompartmentScheduleModel> MakeScheduleDisplay()
        {
            /*
             * Doesn't work need to pass the Singleton Box Shape
             */
        /*
            ObservableCollection<CompartmentScheduleModel> sched = new ObservableCollection<CompartmentScheduleModel>();
            foreach (PillModel p in Pills)
            {
                foreach(PillCompartmentModel comp in p.Shape.Shape)
                {
                    sched.Add(new CompartmentScheduleModel(p.Name, p.Time, p.Notes, comp.Display));
                }
            }

            Rows = Pills[0].Shape.Rows;
            return sched;
        }

        #region Methods
        public void UpdateSchedule()
        {
            foreach (KeyValuePair<String, ScheduleDay> entry in PillSchedule)
            {
                entry.Value.ClearDaySchedule();
            }

            foreach (PillModel pm in Pills)
            {
                List<ScheduleItem> items = MakeScheduleItemList(pm);
                FillScheduleDay(items);
            }
        }

        public List<ScheduleItem> MakeScheduleItemList(PillModel pm)
        {
            List<ScheduleItem> scheduleItems = new List<ScheduleItem>();
            for (int i = 0; i < pm.Days.ToArray().Length; i++)
            {
                if (pm.Days.ToArray()[i])
                {
                    scheduleItems.Add(new ScheduleItem(pm.Name, pm.Time, pm.Notes, i));
                }
            }
            return scheduleItems;
        }

        private void FillScheduleDay(List<ScheduleItem> items)
        {

            foreach (ScheduleItem i in items)
            {
                PillSchedule[i.PillDay].AddScheduleItem(i);
            }
        }
        #endregion
        */
    }
}
