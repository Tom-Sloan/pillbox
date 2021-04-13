using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.ViewModels
{
    /// <summary>
    /// Class <c>BaseViewModel</c> the base viewmodel class which all viewmodels extend
    /// </summary>
    class BaseViewModel : INotifyPropertyChanged
    {
        #region Notify
        public virtual event PropertyChangedEventHandler PropertyChanged;
        protected virtual void NotifyPropertyChanged(params string[] propertyNames)
        {
            if (PropertyChanged != null)
            {
                foreach (string propertyName in propertyNames)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
                }
                PropertyChanged(this, new PropertyChangedEventArgs("HasError"));
            }

        }
        #endregion
    }
}
