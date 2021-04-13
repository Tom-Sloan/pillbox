using Newtonsoft.Json;
using SmartPillBox.Commands;
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
    /// Class <c>MainWindowViewModel</c> viewmodel which serves as the datacontext for MainWindow.
    /// </summary>
    class MainWindowViewModel : BaseViewModel
    {
        #region Properties
        public ObservableCollection<PillModel> Pills
        {
            get { return _Pills; }
            set { _Pills = value; NotifyPropertyChanged("Pills"); }
        }
        private ObservableCollection<PillModel> _Pills;

        public BaseViewModel SelectedViewModel
        {
            get { return _selectedViewModel; }
            set { _selectedViewModel = value; NotifyPropertyChanged("SelectedViewModel"); }
        }
        private BaseViewModel _selectedViewModel = new MedicationHistoryViewModel();

        public BaseViewModel MedicationHistoryViewModel
        {
            get { return _MedicationHistoryViewModel; }
            set { _MedicationHistoryViewModel = value; NotifyPropertyChanged("MedicationHistoryViewModel"); }
        }
        private BaseViewModel _MedicationHistoryViewModel = new MedicationHistoryViewModel();

        public BaseViewModel PillSchedulerViewModel
        {
            get { return _PillSchedulerViewModel; }
            set { _PillSchedulerViewModel = value; NotifyPropertyChanged("PillSchedulerViewModel"); }
        }
        private BaseViewModel _PillSchedulerViewModel = new MedicationHistoryViewModel();

        public BaseViewModel PillScheduleViewModel
        {
            get { return _PillScheduleViewModel; }
            set { _PillScheduleViewModel = value; NotifyPropertyChanged("PillScheduleViewModel"); }
        }
        private BaseViewModel _PillScheduleViewModel = new MedicationHistoryViewModel();
        

        public ICommand UpdateViewCommand { get; set; }

        #endregion Properties


        #region Constructor
        public MainWindowViewModel()
        {
            //upon contrsuction create a blank Pill Schedule and initialize the UpdateViewCommand
            Pills = new ObservableCollection<PillModel>();
            UpdateViewCommand = new UpdateViewCommand(this);  
            
            //build with data from pill box
            MedicationHistoryViewModel = new MedicationHistoryViewModel();

            //replace with json saved version of these objects
            PillSchedulerViewModel = new PillSchedulerViewModel(Pills);
            PillScheduleViewModel = new PillScheduleViewModel(PillSchedulerViewModel);            
        }
        #endregion Constructor

        #region Save state
        /*
         * Initial attempt at serialization
        public void SaveState() 
        {
            string jsonData = JsonConvert.SerializeObject(SerializableVM);

            Microsoft.Win32.SaveFileDialog dialog = new Microsoft.Win32.SaveFileDialog();
            dialog.InitialDirectory = RootStoragePath;
            dialog.RestoreDirectory = true;
            dialog.FileName = "ViewModel"; // Default file name
            dialog.DefaultExt = ".json"; // Default file extension
            dialog.Filter = "Json files (*.json)|*.json|Text files (*.txt)|*.txt"; // Filter files by extension

            // Show save file dialog box
            Nullable<bool> result = dialog.ShowDialog();

            // Process save file dialog box results
            if (result == true)
            {
                // Save document
                File.WriteAllText(dialog.FileName, jsonData);
            }
        }

        public void LoadState() { }
        */

        #endregion Save state
    }
}
