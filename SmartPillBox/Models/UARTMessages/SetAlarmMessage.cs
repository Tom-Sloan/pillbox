using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models.UARTMessages
{
    class SetAlarmMessage : MessageBase
    {
        public SetAlarmMessage()
        {
            CTL = Encoding.UTF8.GetBytes("T")[0];
        }
    }
}
