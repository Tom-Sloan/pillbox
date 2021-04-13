using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models
{
    /// <summary>
    /// Class <c>PillModel</c> models a medication inside the PillSchedulerView form
    /// </summary>
    public class PillModel : BaseModel
    {
        #region Properties
        public string Name
        {
            get { return _Name; }
            set { _Name = value; NotifyPropertyChanged("Name"); }
        }
        private string _Name;


        public PillBoxShape Shape
        {
            get { return _Shape; }
            set { _Shape = value; NotifyPropertyChanged("Shape"); }
        }
        private PillBoxShape _Shape;


        public String Time
        {
            get { return _Time; }
            set { _Time = value; NotifyPropertyChanged("Time"); }
        }
        private String _Time;

        public String Notes
        {
            get { return _Notes; }
            set { _Notes = value; NotifyPropertyChanged("Notes"); }
        }
        private String _Notes;

        public String Date
        {
            get { return _Date; }
            set { _Date = value; NotifyPropertyChanged("Date"); }
        }
        private String _Date;

        #endregion

        #region Constructor
        public PillModel(int numRows) { Shape = new PillBoxShape(numRows); }
        public PillModel(PillBoxShape shape) { Shape = shape; }
        #endregion

        #region Methods
        public void UpdateShapeSelection(PillBoxShape shape)
        {
            for (int i = 0; i < shape.Shape.Count; i++)
            {
                //if disabled in singleton and isn't this pill
                if (!shape.Shape[i].Enabled & !Shape.Shape[i].Selected)
                    //disable actions
                    Shape.Shape[i].Enabled = false;
            }
        }
        #endregion Methods
    }
}
