using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models.UARTMessages
{
    class LockRowMessage : MessageBase
    {
        public LockRowMessage()
        {
            CTL = Encoding.UTF8.GetBytes("L")[0];
        }
    }
}
