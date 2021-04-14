using SmartPillBox.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace SmartPillBox.Commands
{
    class UpdateViewCommand: ICommand
    {
        private MainWindowViewModel _viewModel;

        public UpdateViewCommand(MainWindowViewModel viewModel)
        {
            _viewModel = viewModel;
        }

        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            Console.WriteLine("Update View Execute reached");
            var newView = parameter.ToString();
            if (newView == "History") _viewModel.SelectedViewModel = _viewModel.MedicationHistoryViewModel;
            else if (newView == "Editor") _viewModel.SelectedViewModel = _viewModel.PillSchedulerViewModel;
            else if (newView == "Regime")
            {
                PillSchedulerViewModel vm = (PillSchedulerViewModel)_viewModel.PillSchedulerViewModel;
                _viewModel.PillScheduleViewModel = new PillScheduleViewModel(vm);
                _viewModel.SelectedViewModel = _viewModel.PillScheduleViewModel;                
            }
        }

        
    }

   
}
