using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models.UARTMessages
{
    class UnlockRowMessage : MessageBase
    {
        public UnlockRowMessage()
        {
            CTL = Encoding.UTF8.GetBytes("U")[0];
        }
    }
}
