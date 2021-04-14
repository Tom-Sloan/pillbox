using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models.UARTMessages
{
    public class GetMedicationHistoryMessage: MessageBase
    {
        public GetMedicationHistoryMessage()
        {
            CTL = Encoding.UTF8.GetBytes("R")[0];
        }
    }
}
