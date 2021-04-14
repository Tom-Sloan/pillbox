using SmartPillBox.Commands;
using SmartPillBox.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace SmartPillBox.ViewModels
{
    /// <summary>
    /// Class <c>PillSchedulerViewModel</c>
    /// </summary>
    class PillSchedulerViewModel : BaseViewModel
    {
        #region Properties
        public ObservableCollection<PillModel> Pills
        {
            get { return _Pills; }
            set { _Pills = value; NotifyPropertyChanged("Pills");}
        }
        private ObservableCollection<PillModel> _Pills;

        public PillModel SelectedPill
        {
            get { return _SelectedPill; }
            set { _SelectedPill = value; UpdatePillFilledState(); NotifyPropertyChanged("SelectedPill"); }
        }
        private PillModel _SelectedPill;

        public ObservableCollection<int> PillBoxRows
        {
            get { return _PillBoxRows; }
            set { _PillBoxRows = value; NotifyPropertyChanged("PillBoxRows"); }
        }
        private ObservableCollection<int> _PillBoxRows = new ObservableCollection<int>{ 1, 2, 3, 4 };

        public int SelectedRowAmount
        {
            get { return _SelectedRowAmount; }
            set { _SelectedRowAmount = value; BoxShape = new PillBoxShape(SelectedRowAmount); NotifyPropertyChanged("SelectedRowAmount"); }
        }
        private int _SelectedRowAmount = 1;

        public PillBoxShape BoxShape
        {
            get { return _BoxShape; }
            set { _BoxShape = value; NotifyPropertyChanged("Shape"); }
        }
        private PillBoxShape _BoxShape;

        public PillSchedulerViewModel(ObservableCollection<PillModel> pills)
        {
            BoxShape = new PillBoxShape(SelectedRowAmount);
            Pills = pills;
        }
        #endregion Properties

        #region Methods
        /// <summary>
        /// Method<c>UpdatePillFilledState</c> updates a singleton "PillBoxShape" with information from each of the pills
        /// disabling compartments that have already been filled
        /// </summary>
        private void UpdatePillFilledState()
        {
            foreach (PillModel p in Pills)
            {
                DisableSelectedPillsInBoxShape(p);
            }

            foreach (PillModel p in Pills)
            {
                DisableSelectedPillsInPillModels(p);
            }

            /*
            bool[] checkSelected = new bool[SelectedRowAmount];
            foreach (PillModel p in Pills)
            {
                checkSelected = ReEnableRedactedPillCompartmentsInBoxShape(p, checkSelected);
            }
            */
        }

        private void DisableSelectedPillsInBoxShape(PillModel p)
        {
            for(int i=0; i < BoxShape.Shape.Count; i++)
            {
                //if pill is in this slot, disable from main list
                if (p.Shape.Shape[i].Selected)
                {
                    BoxShape.Shape[i].Enabled = false;
                    BoxShape.Shape[i].Selected = true;
                    BoxShape.Shape[i].Pill = new CompartmentScheduleModel(p.Name, p.Time, p.Notes);
                }
            }
            //FinalBoxShapeSelectionUpdate();
        }

        private void DisableSelectedPillsInPillModels(PillModel p)
        {
            for (int i = 0; i < BoxShape.Shape.Count; i++)
            {
                //if boxshape slot is disabled and this pill doesn't fill the slot, disable it in the pill
                if (!BoxShape.Shape[i].Enabled && !p.Shape.Shape[i].Selected)
                {
                    p.Shape.Shape[i].Enabled = false;
                }
            }
        }

        private bool[] ReEnableRedactedPillCompartmentsInBoxShape(PillModel p, bool[] checkSelected)
        {
            for (int i = 0; i < BoxShape.Shape.Count; i++)
            {
                //if pill is in this slot, disable from main list
                if (p.Shape.Shape[i].Selected)
                {
                    checkSelected[i] = true;
                }
            }
            return checkSelected;
        }

        private void WriteShapeState()
        {
            foreach(PillCompartmentModel comp in BoxShape.Shape)
            {
                Console.WriteLine(comp.Display + ": " + comp.Enabled);
            }
        }
        #endregion Methods

        #region Commands
        public ICommand AddPillCommand => new RelayCommand(o => AddPillButtonClick("AddPillsButton"));

        public void AddPillButtonClick(Object sender)
        {
            var newPill = new PillModel(SelectedRowAmount);
            /*
            if (Pills.Count == 0)
                BoxShape = newPill.Shape;            
            else
                newPill.UpdateShapeSelection(BoxShape);
            */

            Pills.Add(newPill);
            SelectedPill = newPill;
            WriteShapeState();
        }
        public ICommand RemovePillCommand => new RelayCommand(o => RemovePillButtonClick("RemovePillsButton"));

        public void RemovePillButtonClick(Object sender)
        {
            MessageBoxResult diagResult = MessageBox.Show("Are you sure you want to remove " + SelectedPill.Name +"?" , "", MessageBoxButton.YesNo);
            if(diagResult == MessageBoxResult.Yes)
            {
                Pills.Remove(SelectedPill);
                SelectedPill = Pills.FirstOrDefault();
            }
            
        }
        public ICommand ClearPillsCommand => new RelayCommand(o => ClearPillsButtonClick("ClearPillsButton"));

        public void ClearPillsButtonClick(Object sender)
        {
            MessageBoxResult diagResult = MessageBox.Show("Are you sure you want to clear your Medication List", "", MessageBoxButton.YesNo);
            if (diagResult == MessageBoxResult.Yes)
            {
                Pills.Clear();
                BoxShape = new PillBoxShape(SelectedRowAmount);
            }            
        }
        #endregion
    }
}
