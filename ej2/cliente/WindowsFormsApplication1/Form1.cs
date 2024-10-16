using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        Socket server;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Inicialización adicional si es necesaria.
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // Creamos un IPEndPoint con el IP del servidor y puerto del servidor
            // al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse(IP.Text); // Asegúrate de tener un TextBox llamado IP
            IPEndPoint ipep = new IPEndPoint(direc, 9007);

            // Creamos el socket
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep); // Intentamos conectar el socket
                this.BackColor = Color.Green;
                MessageBox.Show("Conectado");
            }
            catch (SocketException ex)
            {
                // Si hay excepción imprimimos error y salimos del programa con return
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (op1.Checked) // Convertir Celsius a Fahrenheit
            {
                string mensaje = "1/" + temperatura.Text; // Asumiendo que tienes un TextBox para la temperatura
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                // Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0]; // Elimina el carácter nulo

                MessageBox.Show("La temperatura en Fahrenheit es: " + mensaje);
            }
            else if (op2.Checked) // Asegúrate de que tienes un RadioButton para Fahrenheit a Celsius
            {
                string mensaje = "2/" + temperatura.Text; // Asumiendo que tienes un TextBox para la temperatura
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                // Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0]; // Elimina el carácter nulo

                MessageBox.Show("La temperatura en Celsius es: " + mensaje);
            }
            else
            {
                MessageBox.Show("Por favor selecciona una opción de conversión.");
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string mensaje = "0/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg); // Debes enviar el mensaje para desconectarte

            // Nos desconectamos
            this.BackColor = Color.Gray;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }

        private void Bonito_CheckedChanged(object sender, EventArgs e)
        {
            // Manejo de eventos si es necesario.
        }

        private void nombre_TextChanged(object sender, EventArgs e)
        {
            // Manejo de eventos si es necesario.
        }
    }
}
