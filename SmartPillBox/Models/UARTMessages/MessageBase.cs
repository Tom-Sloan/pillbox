using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Models.UARTMessages
{
    public abstract class MessageBase
    {
        public byte INTERVAL_SELECTOR { get; set; } = 0x30;

        public byte ALARM_SELECTOR { get; set; } = 0x30;

        public byte EXPANSION_NUMBER { get; set; } = 0x30;

        public byte ROW { get; set; } = 0x30;

        public byte COL { get; set; } = 0x30;

        public byte CTL { get; set; } = 0x30;

        public byte[] TIMESTAMP { get; set; } = new byte[] { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 };

        override public string ToString()
        {
            //Make a data message for the pillbox
            byte[] bytes = ToBytes();           
            //Convert to string
            var message = Encoding.UTF8.GetString(bytes);

            return message;
        }

        public byte[] ToBytes()
        {
            //Make a data message for the pillbox
            byte[] bytes = new byte[16];
            bytes[0] = INTERVAL_SELECTOR;
            bytes[1] = ALARM_SELECTOR;
            bytes[2] = EXPANSION_NUMBER;
            bytes[3] = ROW;
            bytes[4] = COL;
            bytes[5] = CTL;
            Buffer.BlockCopy(TIMESTAMP, 0, bytes, 6, 10);

            return bytes;
        }
    }


}
