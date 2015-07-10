namespace WindowsFormsApplication1
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.user_name = new System.Windows.Forms.TextBox();
            this.connect_button = new System.Windows.Forms.Button();
            this.main_text = new System.Windows.Forms.TextBox();
            this.send_text = new System.Windows.Forms.TextBox();
            this.send_button = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.label1.Location = new System.Drawing.Point(6, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(40, 16);
            this.label1.TabIndex = 0;
            this.label1.Text = "名前";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // user_name
            // 
            this.user_name.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.user_name.Location = new System.Drawing.Point(53, 8);
            this.user_name.MaxLength = 127;
            this.user_name.Name = "user_name";
            this.user_name.Size = new System.Drawing.Size(210, 23);
            this.user_name.TabIndex = 1;
            // 
            // connect_button
            // 
            this.connect_button.Location = new System.Drawing.Point(269, 8);
            this.connect_button.Name = "connect_button";
            this.connect_button.Size = new System.Drawing.Size(62, 23);
            this.connect_button.TabIndex = 2;
            this.connect_button.Text = "接続!";
            this.connect_button.UseVisualStyleBackColor = true;
            this.connect_button.Click += new System.EventHandler(this.connect_button_Click);
            // 
            // main_text
            // 
            this.main_text.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.main_text.Location = new System.Drawing.Point(9, 37);
            this.main_text.Multiline = true;
            this.main_text.Name = "main_text";
            this.main_text.ReadOnly = true;
            this.main_text.Size = new System.Drawing.Size(322, 450);
            this.main_text.TabIndex = 3;
            this.main_text.TextChanged += new System.EventHandler(this.main_text_TextChanged);
            // 
            // send_text
            // 
            this.send_text.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.send_text.Location = new System.Drawing.Point(9, 493);
            this.send_text.MaxLength = 127;
            this.send_text.Name = "send_text";
            this.send_text.Size = new System.Drawing.Size(267, 23);
            this.send_text.TabIndex = 4;
            // 
            // send_button
            // 
            this.send_button.Enabled = false;
            this.send_button.Location = new System.Drawing.Point(282, 493);
            this.send_button.Name = "send_button";
            this.send_button.Size = new System.Drawing.Size(49, 23);
            this.send_button.TabIndex = 5;
            this.send_button.Text = "送信!";
            this.send_button.UseVisualStyleBackColor = true;
            this.send_button.Click += new System.EventHandler(this.send_button_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(344, 526);
            this.Controls.Add(this.send_button);
            this.Controls.Add(this.send_text);
            this.Controls.Add(this.main_text);
            this.Controls.Add(this.connect_button);
            this.Controls.Add(this.user_name);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Dual Chat";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox user_name;
        private System.Windows.Forms.Button connect_button;
        private System.Windows.Forms.TextBox main_text;
        private System.Windows.Forms.TextBox send_text;
        private System.Windows.Forms.Button send_button;
    }
}

