namespace SenderReciver
{
    partial class Form1
    {
        /// <summary>
        /// Требуется переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Обязательный метод для поддержки конструктора - не изменяйте
        /// содержимое данного метода при помощи редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.tbxComPort = new System.Windows.Forms.TextBox();
            this.btnOpen = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.tbxPipe = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnAdd = new System.Windows.Forms.Button();
            this.btnRemove = new System.Windows.Forms.Button();
            this.tbxTimeout = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.btnRecive = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.tbxData = new System.Windows.Forms.TextBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.tbxLog = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.tbxAESKey = new System.Windows.Forms.TextBox();
            this.cbxRAW = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(34, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "COM:";
            // 
            // tbxComPort
            // 
            this.tbxComPort.Location = new System.Drawing.Point(52, 6);
            this.tbxComPort.Name = "tbxComPort";
            this.tbxComPort.Size = new System.Drawing.Size(100, 20);
            this.tbxComPort.TabIndex = 1;
            this.tbxComPort.Text = "COM6";
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(158, 4);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(75, 23);
            this.btnOpen.TabIndex = 2;
            this.btnOpen.Text = "Открыть";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(239, 3);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 3;
            this.btnClose.Text = "Закрыть";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // serialPort1
            // 
            this.serialPort1.DtrEnable = true;
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // tbxPipe
            // 
            this.tbxPipe.Location = new System.Drawing.Point(98, 32);
            this.tbxPipe.Name = "tbxPipe";
            this.tbxPipe.Size = new System.Drawing.Size(216, 20);
            this.tbxPipe.TabIndex = 4;
            this.tbxPipe.Text = "0xF0F0F0F0E1000000";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(4, 34);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(80, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Адрес канала:";
            // 
            // btnAdd
            // 
            this.btnAdd.Location = new System.Drawing.Point(320, 29);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(176, 23);
            this.btnAdd.TabIndex = 6;
            this.btnAdd.Text = "Добавить для прослушки";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnRemove
            // 
            this.btnRemove.Location = new System.Drawing.Point(320, 56);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(176, 23);
            this.btnRemove.TabIndex = 7;
            this.btnRemove.Text = "Удалить из прослушиваемых";
            this.btnRemove.UseVisualStyleBackColor = true;
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // tbxTimeout
            // 
            this.tbxTimeout.Location = new System.Drawing.Point(98, 58);
            this.tbxTimeout.Name = "tbxTimeout";
            this.tbxTimeout.Size = new System.Drawing.Size(100, 20);
            this.tbxTimeout.TabIndex = 8;
            this.tbxTimeout.Text = "4000";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 61);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(88, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Таймаут (мсек):";
            // 
            // btnRecive
            // 
            this.btnRecive.Location = new System.Drawing.Point(204, 58);
            this.btnRecive.Name = "btnRecive";
            this.btnRecive.Size = new System.Drawing.Size(110, 23);
            this.btnRecive.TabIndex = 10;
            this.btnRecive.Text = "Получить";
            this.btnRecive.UseVisualStyleBackColor = true;
            this.btnRecive.Click += new System.EventHandler(this.btnRecive_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(4, 86);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(51, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "Данные:";
            // 
            // tbxData
            // 
            this.tbxData.Location = new System.Drawing.Point(98, 83);
            this.tbxData.Name = "tbxData";
            this.tbxData.Size = new System.Drawing.Size(216, 20);
            this.tbxData.TabIndex = 12;
            this.tbxData.Text = "4000";
            // 
            // btnSend
            // 
            this.btnSend.Location = new System.Drawing.Point(320, 81);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(176, 23);
            this.btnSend.TabIndex = 13;
            this.btnSend.Text = "Отправить";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(4, 111);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 13);
            this.label5.TabIndex = 14;
            this.label5.Text = "Лог:";
            // 
            // tbxLog
            // 
            this.tbxLog.Location = new System.Drawing.Point(7, 127);
            this.tbxLog.Multiline = true;
            this.tbxLog.Name = "tbxLog";
            this.tbxLog.Size = new System.Drawing.Size(489, 320);
            this.tbxLog.TabIndex = 15;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(502, 29);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(301, 23);
            this.button1.TabIndex = 16;
            this.button1.Text = "Сгенерировать ключ";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(502, 63);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(60, 13);
            this.label6.TabIndex = 17;
            this.label6.Text = "Ключ AES:";
            // 
            // tbxAESKey
            // 
            this.tbxAESKey.Location = new System.Drawing.Point(502, 83);
            this.tbxAESKey.Multiline = true;
            this.tbxAESKey.Name = "tbxAESKey";
            this.tbxAESKey.Size = new System.Drawing.Size(374, 364);
            this.tbxAESKey.TabIndex = 18;
            // 
            // cbxRAW
            // 
            this.cbxRAW.AutoSize = true;
            this.cbxRAW.Location = new System.Drawing.Point(407, 107);
            this.cbxRAW.Name = "cbxRAW";
            this.cbxRAW.Size = new System.Drawing.Size(52, 17);
            this.cbxRAW.TabIndex = 19;
            this.cbxRAW.Text = "RAW";
            this.cbxRAW.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(888, 459);
            this.Controls.Add(this.cbxRAW);
            this.Controls.Add(this.tbxAESKey);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.tbxLog);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.btnSend);
            this.Controls.Add(this.tbxData);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.btnRecive);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tbxTimeout);
            this.Controls.Add(this.btnRemove);
            this.Controls.Add(this.btnAdd);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbxPipe);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnOpen);
            this.Controls.Add(this.tbxComPort);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Приём-передача";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbxComPort;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.Button btnClose;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.TextBox tbxPipe;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.Button btnRemove;
        private System.Windows.Forms.TextBox tbxTimeout;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnRecive;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tbxData;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbxLog;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tbxAESKey;
        private System.Windows.Forms.CheckBox cbxRAW;
    }
}

