using SmartPillBox.Models.UARTMessages;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartPillBox.Connections
{
    public class UsbConnection : INotifyPropertyChanged
    {
        //PACKET_LENGTH = length of the messages sent back and forth between pillbox
        private const int PACKET_LENGTH = 16;

        /**
         * The serial port which is used to connect with the pillbox.         * 
         */
        public SerialPort Port
        {
            get { return _serialPort; }
            set { _serialPort = value; NotifyPropertyChanged("serialPort"); }
        }
        private SerialPort _serialPort = new SerialPort();

        /**
         * Stores usage data provided by the pillbox.
         * data is stored as unix time stamps (doubles) - make class?
         */
        public HashSet<string> PillBoxData
        {
            get { return _PillBoxData; }
            set { _PillBoxData = value; NotifyPropertyChanged("serialPort"); }
        }
        private HashSet<string> _PillBoxData = new HashSet<string>();


        public bool Connect()
        {
            if (!Port.IsOpen)
            {
                try
                {
                    Port.PortName = "COM3";
                    Port.Parity = Parity.None;
                    Port.Handshake = Handshake.None;
                    Port.BaudRate = 115200;
                    Port.DataBits = 8;
                    Port.StopBits = StopBits.One;

                    Port.DataReceived += Port_DataReceived;
                    Port.ErrorReceived += Port_Error;


                    Port.Open();
                    return true;
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }
                return false;
            }
            return false;
            
        }

        private void WriteToSerial(MessageBase data)
        {
            //Port.Write(data.ToBytes(), 0, PACKET_LENGTH);
            var message = data.ToString();
            Console.WriteLine(message);
            Port.Write(message);
        }


        private void Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            // Read all the incoming data in the port's buffer (timestamps)
            var data = Port.ReadExisting();
            //split timestamps into an array by the comma between each
            var splitData = data.Split('\n');

            foreach (string s in splitData)
            {
                //check format of message using regex?
                if(s.Length == 16)
                {
                    PillBoxData.Add(s);
                }                
            }
            Console.WriteLine("Size of PillBoxData in UsbConn: " + PillBoxData.Count);
        }

        private void Port_Error(object sender, SerialErrorReceivedEventArgs e)
        {
            // Show all the incoming data in the port's buffer
            Console.WriteLine(Port.ReadExisting());
        }

        private DateTime UnixTimeStampToDateTime(double timestamp)
        {
            DateTime newDateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0);
            newDateTime.AddSeconds(timestamp);
            return newDateTime;
        }

        public void AskForUsageData()
        {
            MessageBase message = new GetMedicationHistoryMessage();
            WriteToSerial(message);           
        }

        #region Notify
        public virtual event PropertyChangedEventHandler PropertyChanged;
        protected virtual void NotifyPropertyChanged(params string[] propertyNames)
        {
            if (PropertyChanged != null)
            {
                foreach (string propertyName in propertyNames)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
                }
                PropertyChanged(this, new PropertyChangedEventArgs("HasError"));
            }
        }
        #endregion
    }
}
