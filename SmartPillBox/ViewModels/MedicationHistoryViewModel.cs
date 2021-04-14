using SmartPillBox.Commands;
using SmartPillBox.Connections;
using SmartPillBox.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace SmartPillBox.ViewModels
{
    /// <summary>
    /// Class <c>MedicationHistoryViewModel</c> Viewmodel that serves as datacontext for the 
    /// Medication History View
    /// </summary>
    class MedicationHistoryViewModel : BaseViewModel
    {
        public ObservableCollection<DayModel> Day
        {
            get;
            set;
        }

        public UsbConnection Conn
        {
            get { return _Conn; }
            set { _Conn = value; NotifyPropertyChanged("Notes"); }
        }
        private UsbConnection _Conn;

        public int Rows { get; set; } = 13;

        #region Constructor
        /// <summary>
        /// Initial constructor 
        /// </summary>
        public MedicationHistoryViewModel()
        {
            Day = new ObservableCollection<DayModel>();
            for(int i = 1; i < 31; i++)
            {
                Day.Add(new DayModel
                {
                    Date = new DateTime(2021, 03, i)
                }); ;
            }

            for (int i = 1; i < 30; i++)
            {
                Day.Add(new DayModel
                {
                    Date = new DateTime(2021, 04, i)
                }); ;
            }

            Conn = new UsbConnection();
        }
        #endregion

        #region Commands
        public ICommand ConnectCommand => new RelayCommand(o => ConnectToDeviceClick("ConnectButton"));

        /// <summary>
        /// Method <c>ConnectToDeviceClick</c> run when Connect To Device button is pressed.
        /// currently a debug method - to be cleaned up when write to pillbox is funcitonal.
        /// </summary>
        /// <param name="sender"></param>
        public void ConnectToDeviceClick(Object sender)
        {
            //connect to device on COM3 (pillbox microcontroller)
            Conn.Connect();

            //send message asking for usage data
            Console.WriteLine("Status of Port: " + Conn.Port.IsOpen.ToString());
            
            Conn.AskForUsageData();

            //Messy GUI sleep -> could be implemented in cleaner way
            System.Threading.Thread.Sleep(50);

            //receive data
            HashSet<string> usageData = Conn.PillBoxData;
            Console.WriteLine("Size of PillBoxData in ViewModel: " + Conn.PillBoxData.Count);
            
            Day.Clear();

            //fill History View with data
            foreach (string line in usageData)
            {
                Console.WriteLine(line);
                var free = line.Substring(0, 3);
                var loc = line.Substring(3, 2);
                var ctl = line.Substring(5, 1);
                var timestamp = Convert.ToDouble(line.Substring(6, 10));
                var dtTime = UnixTimeStampToDouble(timestamp);
                //pillbox response data should keep change CTL Byte to new message type when alarms are missed
                if (ctl != "T")
                {
                    Day.Add(new DayModel
                    {
                        Date = dtTime,
                        Taken = false
                    }); ;
                    Console.WriteLine(dtTime.ToString());
                }
                else
                {
                    Day.Add(new DayModel
                    {
                        Date = dtTime
                    }); ;
                }
            }
        }

        private DateTime UnixTimeStampToDouble(double timestamp)
        {
            DateTime newTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);
            newTime = newTime.AddSeconds(timestamp).ToLocalTime();
            return newTime;
            
        }
        /*
        public void ConnectToDeviceClick(Object sender)
        {
            Day.Clear();
            string line;
            System.IO.StreamReader file = new System.IO.StreamReader("C:/Users/Dave/Desktop/SYSC 4907/testdata.txt");
            while ((line = file.ReadLine()) != null)
            {
                var free = line.Substring(0, 3);
                var loc = line.Substring(3, 2);
                var ctl = line.Substring(5, 1);
                var timestamp = Convert.ToDouble(line.Substring(6,10));
                var dtTime = UnixTimeStampToDouble(timestamp);
                if(ctl !="T")
                {
                    Day.Add(new DayModel
                    {
                        Date = dtTime,
                        Taken = false
                    }); ;
                    Console.WriteLine(dtTime.ToString());
                }
                else
                {
                    Day.Add(new DayModel
                    {
                        Date = dtTime
                    }); ;
                }               
            }

            Rows = (int)Math.Ceiling((double)Day.Count / 7);
            Console.WriteLine(Rows);
            //read in txt file test C:\Users\Dave\Desktop\SYSC 4907\testdata.txt

            //parse lines into 
        }
        */

        #endregion
    }
}
