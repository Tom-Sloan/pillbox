using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models
{
    /// <summary>
    /// Class <c>CompatmentScheduleModel</c> A model class used to represent a pill box
    /// "compartment" in the PillScheduleView.
    /// </summary>
    public class CompartmentScheduleModel : BaseModel
    {
        public string PillName { get; set; } = "";
        public string PillTime { get; set; } = "";
        public string PillDay { get; set; } = "";
        public string PillNote { get; set; } = "";

        public CompartmentScheduleModel() { }

        public CompartmentScheduleModel(string name, string time, string Note)
        {
            PillName = name;
            PillTime = time;
            PillNote = Note;
        }
    }
}
