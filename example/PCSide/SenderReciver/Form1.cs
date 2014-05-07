using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.IO;

using System.Runtime.InteropServices;

namespace SenderReciver
{
    public partial class Form1 : Form
    {

       [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct SerialCommand
        {
         //   [FieldOffset(0)]
            public byte cmd;
           // [FieldOffset(4)]
            public UInt64 Pipe;
            //[FieldOffset(9)]
            public UInt16 DataSize;
            //[FieldOffset(11)]
            public UInt32 TimeOut;
           public byte[] CryptoKey;
        };


       [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct SerialResult
        {
         //  [FieldOffset(0)]
            public byte cmd;
          // [FieldOffset(1)]
            public SByte result;
          // [FieldOffset(2)]
            public Int64 Pipe;
           //[FieldOffset(10)]
            public Int16 DataSize;
        };

        enum Commands
        {
            SCMD_SEND_DATA = 0xf1,
            SCMD_RECIVE_DATA = 0xf2,
            SCMD_ADD_LISTENING_PIPE = 0xf3,
            SCMD_REMOVE_LISTENING_PIPE = 0xf4
        };

        enum Results
        {
            ERROR_SUCCESS = 1,
            ERROR_NO_FREE_PIPE = -1,
            ERROR_PIPE_NOT_FOUND = -2,
            ERROR_TIMEOUT = -3,
            ERROR_NO_ACTIVE_PIPE = -4,
            ERROR_NO_DATA = -5,
            ERROR_RADIO_TRANSMITT = -6,
            RECIVING_PIPES_MAXCOUNT = 5
        };

        public Form1()
        {
            InitializeComponent();

            Random R = new Random();
            Byte[] Key = new Byte[32];
            for (int i = 0; i < 32; i++)
                Key[i] = (byte)R.Next(0, 256);
            tbxAESKey.Text = Convert.ToBase64String(Key);
        }

        private delegate void delVStr(string Str);

        private void AddToLog(string Str)
        {
            if (this.InvokeRequired)
            {
                delVStr del = new delVStr(AddToLog);
                this.Invoke(del, new object[] { Str });
            }
            else
            {
                tbxLog.Text += DateTime.Now.ToString("HH:mm:ss: ") + Str + Environment.NewLine;
                tbxLog.SelectionStart = tbxLog.Text.Length;
                tbxLog.ScrollToCaret();
                tbxLog.Refresh();
            }
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.BaudRate = 57600;
                serialPort1.PortName = tbxComPort.Text;
                serialPort1.Open();
            }
            catch (Exception Err)
            {
                MessageBox.Show("Ошибка открытия порта: " + Err.Message);
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
                serialPort1.Close();
        }

        private SerialResult FillResult(byte[] Data, out byte[] AdditionalData)
        {
            BinaryReader B = new BinaryReader(new MemoryStream(Data));

            SerialResult Res;
            Res.cmd = B.ReadByte();
            Res.result = B.ReadSByte();
            Res.Pipe = B.ReadInt64();
            Res.DataSize = B.ReadInt16();

            if (Res.DataSize > 0)
                AdditionalData = B.ReadBytes(Res.DataSize);
            else
                AdditionalData = new byte[0];

     
            return Res;
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            int LastCount = serialPort1.BytesToRead;
            DateTime N = DateTime.Now;
            do
            {
                LastCount = serialPort1.BytesToRead;
                Thread.Sleep(100);
            }
            while (serialPort1.IsOpen && (serialPort1.BytesToRead != LastCount || serialPort1.BytesToRead < System.Runtime.InteropServices.Marshal.SizeOf(typeof(SerialResult))) && (N - DateTime.Now).TotalMilliseconds < 500);

            if (!serialPort1.IsOpen)
                return;
            
            if (serialPort1.BytesToRead < System.Runtime.InteropServices.Marshal.SizeOf(typeof(SerialResult)))
                return;

            LastCount = serialPort1.BytesToRead;
            byte[] Buf = new byte[LastCount];
            serialPort1.Read(Buf, 0, LastCount);

            if (cbxRAW.Checked)
                AddToLog(Encoding.Default.GetString(Buf));
            else
            {
                   byte[] AdditionalData;
                     SerialResult SR = FillResult(Buf, out AdditionalData);
                    AddToLog(string.Format("Message: {0}, Cmd: {1}, Pipe: {2}, DataSize: {3}, AdditionalData: {4}", ((Results)SR.result).ToString("F"),
                   ((Commands)SR.cmd).ToString("F"), SR.Pipe, SR.DataSize, Encoding.Default.GetString(AdditionalData)));
            }
       
            
        }

        private static byte[] RawSerialize(SerialCommand anything)
        {
            List<byte> Res = new List<byte>();
            Res.AddRange(BitConverter.GetBytes(anything.cmd).Take(1));
            Res.AddRange(BitConverter.GetBytes(anything.Pipe).Take(8));
            Res.AddRange(BitConverter.GetBytes(anything.DataSize).Take(2));
            Res.AddRange(BitConverter.GetBytes(anything.TimeOut).Take(4));
            Res.AddRange(anything.CryptoKey);
            return Res.ToArray();
        }

        private void SendData(byte[] Data, int Size)
        {
            int BlockCount;
            if (Size % 64 == 0)
                BlockCount = Size / 64;
            else
                BlockCount = Size / 64 + 1;

            for (int i = 0; i < BlockCount; i++)
            {
                serialPort1.Write(Data, i * 64, Math.Min(64, Size - i * 64));
                Thread.Sleep(400);
            }
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
                return;
            SerialCommand SC;
            SC.cmd = (byte)Commands.SCMD_ADD_LISTENING_PIPE;
            SC.DataSize = 0;
            SC.Pipe = UInt64.Parse(tbxPipe.Text.Replace("0x", ""), System.Globalization.NumberStyles.HexNumber);
            SC.TimeOut = Convert.ToUInt32(tbxTimeout.Text);
            SC.CryptoKey = Convert.FromBase64String(tbxAESKey.Text).ToArray();
            byte[] Raw = RawSerialize(SC);
            SendData(Raw, Raw.Count());
            
        }

        private void btnRemove_Click(object sender, EventArgs e)
        {
            SerialCommand SC;
            SC.cmd = (byte)Commands.SCMD_REMOVE_LISTENING_PIPE;
            SC.DataSize = 0;
            SC.Pipe = UInt64.Parse(tbxPipe.Text.Replace("0x", ""), System.Globalization.NumberStyles.HexNumber);
            SC.TimeOut = Convert.ToUInt32(tbxTimeout.Text);
            SC.CryptoKey = Convert.FromBase64String(tbxAESKey.Text).ToArray();
            byte[] Raw = RawSerialize(SC);
            SendData(Raw,  Raw.Count());
        }

        private void btnRecive_Click(object sender, EventArgs e)
        {
            SerialCommand SC;
            SC.cmd = (byte)Commands.SCMD_RECIVE_DATA;
            SC.DataSize = 4;
            SC.Pipe = UInt64.Parse(tbxPipe.Text.Replace("0x", ""), System.Globalization.NumberStyles.HexNumber);
            SC.TimeOut = Convert.ToUInt32(tbxTimeout.Text);
            SC.CryptoKey = Convert.FromBase64String(tbxAESKey.Text).ToArray();
            byte[] Raw = RawSerialize(SC);
            SendData(Raw, Raw.Count());
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            SerialCommand SC;
            SC.cmd = (byte)Commands.SCMD_SEND_DATA;
            SC.DataSize = (UInt16)(Encoding.Default.GetByteCount(tbxData.Text) + 1);
            SC.Pipe = UInt64.Parse(tbxPipe.Text.Replace("0x", ""), System.Globalization.NumberStyles.HexNumber);
            SC.TimeOut = Convert.ToUInt32(tbxTimeout.Text);
            SC.CryptoKey = Convert.FromBase64String(tbxAESKey.Text).ToArray();
            List<byte> Raw =  new List<byte>(RawSerialize(SC));
            Raw.AddRange(Encoding.Default.GetBytes(tbxData.Text));
            Raw.Add(0);
           // SC.DataSize = (ushort)(Encoding.Default.GetByteCount(tbxData.Text));
            SendData(Raw.ToArray(), Raw.Count());

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Random R = new Random();
            Byte[] Key = new Byte[32];
            for (int i = 0; i < 32; i++)
                Key[i] = (byte)R.Next(0, 256);
            tbxAESKey.Text = Convert.ToBase64String(Key);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
