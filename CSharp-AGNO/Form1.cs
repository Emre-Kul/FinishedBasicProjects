using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AgnoCalculator
{
    public partial class Form1 : Form
    {
        AGNOCalculator agnoCalc;
        public Form1()
        {
            InitializeComponent();
            agnoCalc = new AGNOCalculator();
            
       
            PrintAll();
        }
        public void PrintAll() {
            int i ;
            panel2.Controls.Clear();
            for (i = 0; i < agnoCalc.PanelList.Count; i++) 
                panel2.Controls.Add(agnoCalc.PanelList[i]);
  
        }

        private void button2_Click(object sender, EventArgs e)
        {
            panel2.Controls.Clear();
            agnoCalc.Clear();
        }

        private void comboBox_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
     
            if (numericUpDown1.Value <= 0 || textBox1.Text == "") agnoCalc.Calculate(0, 0.0f);
            else agnoCalc.Calculate((int)numericUpDown1.Value,(float)Convert.ToDouble(textBox1.Text));
            MessageBox.Show(agnoCalc.CalculatedAGNO.ToString());
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int i;
            panel2.Controls.Clear();
            agnoCalc.Clear();
            for (i = 0; i < (int)numericUpDown2.Value; i++)
                agnoCalc.AddPanel();
            PrintAll();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            int i,j;
            bool finded;
            char [] textArray;
            textArray = new char[textBox1.Text.Length + 5];
            textArray = textBox1.Text.ToCharArray();
            finded = false;
            for (i = 0; i < textBox1.TextLength; i++) {
                if (finded && textBox1.Text[i] == ',') textArray[i] = ' ';
                if (textBox1.Text[i] != ',' && textBox1.Text[i] != ' ')
                {
                    if ((textBox1.Text[i] < (int)'0' || textBox1.Text[i] > (int)'9'))
                        textArray[i] = ' ';
                }
                else finded = true;
           }

            
            i = 0;
            while (i < textArray.Length) {
                if (textArray[i] == ' ') {
                    j = i;
                    while (j+1 < textArray.Length) {
                        textArray[j] = textArray[j + 1];
                        j++;
                    }
                    textArray[j] = '\0';
                }
                i++;
            }
            textBox1.Text = new String(textArray);
            
        }
    
    }
}
