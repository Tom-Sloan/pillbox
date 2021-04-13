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
        private SerialPort _serialPort;

        /**
         * Stores usage data provided by the pillbox.
         * data is stored as unix time stamps (doubles) - make class?
         */
        public HashSet<double> PillBoxData
        {
            get { return _PillBoxData; }
            set { _PillBoxData = value; NotifyPropertyChanged("serialPort"); }
        }
        private HashSet<double> _PillBoxData;


        public bool Connect()
        {
            try 
            {
                Port.PortName = "COM3";
                Port.Parity = Parity.None;
                Port.Handshake = Handshake.None;
                Port.BaudRate = 300;
                Port.DataBits = 8;
                Port.StopBits = StopBits.One;
                Port.ReceivedBytesThreshold = 576;

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

        private void WriteToSerial(byte[] data)
        {
            Port.Write(data, 0, PACKET_LENGTH);
        }

        private void Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            // Read all the incoming data in the port's buffer (timestamps)
            var data = (Port.ReadExisting());
            
            //split timestamps into an array by the comma between each
            var splitData = data.Split(',');

            foreach (string s in splitData)
            {
                //check format of message using regex?

                var timeStamp = Convert.ToDouble(s);
                PillBoxData.Add(timeStamp);
            }
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
            byte[] message = new byte[16];
            //free blocks
            message[0] = 0;
            message[1] = 0;
            message[2] = 0;

            //row and col can be left empty
            message[3] = 0;
            message[4] = 0;

            //control byte set to ask for timestamps - "T"?
            message[5] = 84;

            //timestamp - can be left as 0 for each
            message[6] = 0;
            message[7] = 0;
            message[8] = 0;
            message[9] = 0;
            message[10] = 0;
            message[11] = 0;
            message[12] = 0;
            message[13] = 0;
            message[14] = 0;
            message[15] = 0;

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
