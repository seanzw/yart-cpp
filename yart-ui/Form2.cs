using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace yart_ui {
    public partial class Form2 : Form {
        private Image Image { get; set; }

        public Form2() {
            InitializeComponent();
        }

        private void exitToolStripMenuItem_Click(Object sender, EventArgs e) {
            this.Close();
        }

        public void ShowImage(String fileName) {
            this.Image = Image.FromFile(fileName);
            this.pictureBox1.Image = this.Image;
        }

        private void saveImageToolStripMenuItem_Click(Object sender, EventArgs e) {
            var saveFileDialog = new SaveFileDialog {
                Title = @"Save output image",
                Filter = @"image files|*.png",
                InitialDirectory = Environment.CurrentDirectory + "\\..\\..\\..\\test"
            };

            if (saveFileDialog.ShowDialog() == DialogResult.OK) {
                this.Image.Save(saveFileDialog.FileName);
            }
            
        }
    }
}
