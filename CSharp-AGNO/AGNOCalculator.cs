/*Emre Kul*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AgnoCalculator
{
    class AGNOCalculator
    {
        public float CalculatedAGNO;
        public List<System.Windows.Forms.NumericUpDown> CredictList;
        public List<System.Windows.Forms.ComboBox> GradeList;
        public List<System.Windows.Forms.Panel> PanelList;
       
        private System.Windows.Forms.NumericUpDown tmpNumUpDown;
        private System.Windows.Forms.ComboBox tmpComboBox;
        private System.Windows.Forms.Panel tmpPanel;


        public AGNOCalculator(){
            CalculatedAGNO = 0.0f;
            CredictList = new List<System.Windows.Forms.NumericUpDown>();
            GradeList = new List<System.Windows.Forms.ComboBox>();
            PanelList = new List<System.Windows.Forms.Panel>();
        }
        public void Calculate(int credict,float oldAgno) {
            int i;
            int newcredict = credict;
            float newAgno = 0.0f;
            newAgno = oldAgno*(float)credict;
            for (i = 0; i < PanelList.Count; i++) {
                if (CredictList[i].Value > 0 && GradeList[i].SelectedIndex >= 0){
                    newcredict += (int)CredictList[i].Value;
                    newAgno += (int)CredictList[i].Value * (4.0f - (float)GradeList[i].SelectedIndex * 0.5f);
                }
            }
            CalculatedAGNO = newAgno / newcredict;
        }
        public void Clear()
        {
            CredictList.Clear();
            GradeList.Clear();
            PanelList.Clear();
        }
        public void AddPanel() { 
            
            int yCord = 0;

            tmpNumUpDown = new System.Windows.Forms.NumericUpDown();
            tmpComboBox = new System.Windows.Forms.ComboBox();
            tmpPanel = new System.Windows.Forms.Panel();
        
            
            // numericUpDown2
            // 
            tmpNumUpDown.Location = new System.Drawing.Point(3, 3);
            tmpNumUpDown.Name = "numericUpDown";
            tmpNumUpDown.Size = new System.Drawing.Size(108, 20);
            tmpNumUpDown.TabIndex = 1;
            // 
            // comboBox1
            // 
            tmpComboBox.FormattingEnabled = true;
            tmpComboBox.Items.AddRange(new object[] {
                "AA(4.0)",
                "BA(3.5)",
                "BB(3.0)",
                "CB(2.5)",
                "CC(2.0)",
                "DC(1.5)",
                "DD(1.0)",
                "FD(0.5)",
                "FF,F0(0.0)"
            });
            tmpComboBox.Location = new System.Drawing.Point(132, 2);
            tmpComboBox.Name = "comboBox";
            tmpComboBox.SelectedIndex = 0;
            tmpComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            tmpComboBox.Size = new System.Drawing.Size(121, 21);
            tmpComboBox.TabIndex = 2;
            // 
            // panel1
            // 
       
            tmpPanel.Controls.Add(tmpComboBox);
            tmpPanel.Controls.Add(tmpNumUpDown);
 
            tmpPanel.Name = "panel";
            tmpPanel.Size = new System.Drawing.Size(256, 28);
          
            if (PanelList.Count > 0) {

                yCord = PanelList[PanelList.Count - 1].Location.Y;
                yCord += 30;
                tmpPanel.Location = new System.Drawing.Point(0, yCord);
              
            }
            else tmpPanel.Location = new System.Drawing.Point(0, 0);

            PanelList.Add(tmpPanel);
            GradeList.Add(tmpComboBox);
            CredictList.Add(tmpNumUpDown);
  
        }

    }
}
