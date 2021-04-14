using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models.UARTMessages
{
    class PlayAlarmMessage : MessageBase
    {
        public PlayAlarmMessage()
        {
            CTL = Encoding.UTF8.GetBytes("P")[0];
        }
    }
}
