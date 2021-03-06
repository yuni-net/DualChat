﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static IntPtr begin_DualChat();

        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int join_guild(IntPtr chat_manager_handle, string user_name);

        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void send_message(IntPtr chat_manager_handle, string message);

        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int receive_message(IntPtr chat_manager_handle, StringBuilder message);

        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void finish_DualChat(IntPtr chat_manager_handle);

        private IntPtr chat_manager;
        private System.Threading.Thread thread4receive;
        private bool thread_should_run;


        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            chat_manager = begin_DualChat();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void connect_button_Click(object sender, EventArgs e)
        {
            this.user_name.Enabled = false;
            this.connect_button.Enabled = false;

            this.main_text.AppendText("@DualChatをしている他のマシンに接続しています…\r\n@DualChatをしている人が居ない場合は何も起こりません。\r\n\r\n");

            join_guild(chat_manager, this.user_name.Text);

            thread_should_run = true;
            thread4receive = new Thread(new ThreadStart(each_frame_func));
            thread4receive.Start();

            this.send_button.Enabled = true;
            this.AcceptButton = this.send_button;
        }

        private void each_frame_func()
        {
            while (thread_should_run)
            {
                StringBuilder buffer = new StringBuilder(1024);
                int did_receive = receive_message(chat_manager, buffer);
                if (did_receive != 0)
                {
                    this.main_text.AppendText(buffer.ToString() + "\r\n\r\n");
                }
            }
        }

        private void send_button_Click(object sender, EventArgs e)
        {
            if(this.send_text.Text=="")
            {
                return;
            }

            string text = string.Copy(this.send_text.Text);
            this.send_text.Text = "";

            text = remove_first_atmark(text);
            text = this.user_name.Text + ":\r\n" + text;

            send_message(chat_manager, text);

            this.main_text.AppendText(text + "\r\n\r\n");
        }

        private string remove_first_atmark(string text)
        {
            if(text[0] == '@')
            {
                return text.Substring(1);
            }

            return text;
        }

        private void main_text_TextChanged(object sender, EventArgs e)
        {

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (thread4receive != null)
            {
                thread_should_run = false;
                thread4receive.Join();
                thread4receive = null;
            }

            finish_DualChat(chat_manager);
        }
    }
}
