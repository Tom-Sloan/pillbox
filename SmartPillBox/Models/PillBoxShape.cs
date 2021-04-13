using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models
{
    /// <summary>
    /// Class <c>PillBoxShape</c> models the shape and contents of the pillbox.
    /// </summary>
    public class PillBoxShape : BaseModel
    {
        public int Rows
        {
            get { return _Rows; }
            set { _Rows = value; NotifyPropertyChanged("Rows"); }
        }
        private int _Rows;

        public ObservableCollection<PillCompartmentModel> Shape
        {
            get { return _Shape; }
            set { _Shape = value; NotifyPropertyChanged("Shape"); }
        }
        private ObservableCollection<PillCompartmentModel> _Shape;

        public PillBoxShape(int numRows)
        {
            Rows = numRows;
            Shape = new ObservableCollection<PillCompartmentModel>();
            for (int i = 1; i <= Rows; i++)
            {
                /*
                 * Rows = 1, (1,7)
                 * Rows = 2, (1,7) | (8,14)
                 * Rows = 3, (1,7) | (8,14) | (15,21)
                 * Rows = 4, (1,7) | (8,14) | (15,21) | (22,28)
                 */
                AddRow(1 + 7 * (i - 1), 7 * i);
            }
        }


        private void AddRow(int start, int end)
        {
            for (int i = start; i <= end; i++)
                Shape.Add(new PillCompartmentModel
                {
                    Display = i
                }); ;
        }

        public void DisableChecked()
        {
            foreach(PillCompartmentModel comp in Shape)
            {
                if (comp.Selected) comp.Enabled = false;
            }
        }
    }
}
