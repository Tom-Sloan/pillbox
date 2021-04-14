using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models.UARTMessages
{
    class OpenCompartmentMessage : MessageBase
    {
        public OpenCompartmentMessage()
        {
            CTL = Encoding.UTF8.GetBytes("M")[0];
        }
    }
}
