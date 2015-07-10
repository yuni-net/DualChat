using System;
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
        private extern static int find_server(IntPtr chat_manager_handle);

        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void connect_server(IntPtr chat_manager_handle, string user_name);

        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void build_server(IntPtr chat_manager_handle);

        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void send_message(IntPtr chat_manager_handle, string message);

        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int receive_message(IntPtr chat_manager_handle, StringBuilder message);

        [DllImport("DualChat.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void finish_DualChat(IntPtr chat_manager_handle);

        private IntPtr chat_manager;
        private System.Threading.Thread thread4receive;


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
            this.connect_button.Enabled = false;

            this.main_text.AppendText("@サーバーを検索しています。\r\n@しばらくお待ちください…\r\n");

            int did_found = find_server(chat_manager);
            if(did_found != 0)
            {
                this.main_text.AppendText("@サーバーが見つかりました。\r\n@接続しています…\r\n");
                connect_server(chat_manager, this.user_name.Text);
                this.main_text.AppendText("@接続しました。\r\n");
            }
            else
            {
                this.main_text.AppendText("@サーバーは見つかりませんでした。\r\n@このパソコンがサーバーとなります。\r\n");
                build_server(chat_manager);
            }

            thread4receive = new Thread(new ThreadStart(each_frame_func));
            thread4receive.Start();

            this.send_button.Enabled = true;
            this.AcceptButton = this.send_button;
        }

        private void each_frame_func()
        {
            while (true)
            {
                StringBuilder buffer = new StringBuilder(1024);
                int did_receive = receive_message(chat_manager, buffer);
                if (did_receive != 0)
                {
                    this.main_text.AppendText(buffer.ToString() + "\r\n");
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
            text = this.user_name.Text + ":\r\n";

            text = remove_first_atmark(text) + "\r\n\r\n";

            send_message(chat_manager, text);
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
            finish_DualChat(chat_manager);
        }
    }
}
