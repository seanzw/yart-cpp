using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace yart_ui {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
            this.YartDirectory = new DirectoryInfo(Environment.CurrentDirectory + "\\..\\..\\..").FullName;
            this.YartExecutable = $"{this.YartDirectory}\\x64\\Release\\yart-cpp.exe";
        }

        private String YartDirectory { get; }
        private String YartExecutable { get; }
        private String FileName { get; set; }
        private String OutputFileName { get; set; }

        private void button1_Click(Object sender, EventArgs e) {
            var yartDirectory = Environment.CurrentDirectory + "/../../..";

            var startInfo = new ProcessStartInfo {
                FileName = $"{yartDirectory}/x64/Release/yart-cpp.exe",
                WorkingDirectory = yartDirectory,
                Arguments = $"{yartDirectory}/test/env.test",
                //RedirectStandardOutput = true,
                //RedirectStandardError = true,
                UseShellExecute = true,
            };

            var process = Process.Start(startInfo);
            process.WaitForExit();

            this.stdout.Text = process.StandardOutput.ReadToEnd() + process.StandardError.ReadToEnd();
        }

        private void Form1_Load(Object sender, EventArgs e) {

        }

        private void openSceneFileToolStripMenuItem_Click(Object sender, EventArgs e) {
            var openFileDialog = new OpenFileDialog {
                Title = @"Open scene file",
                Filter = @"scene files|*.*",
                InitialDirectory = Environment.CurrentDirectory + "\\..\\..\\..\\test"
            };

            if (openFileDialog.ShowDialog() == DialogResult.OK) {
                this.FileName = openFileDialog.FileName;
            }

            this.stdout.Text = File.ReadAllText(this.FileName);
        }


        private void exitToolStripMenuItem_Click(Object sender, EventArgs e) {
            Application.Exit();
        }

        private void button2_Click(Object sender, EventArgs e) {

            var task = new Task(
                () => {

                    var startInfo = new ProcessStartInfo {
                        FileName = this.YartExecutable,
                        Arguments = this.FileName.Replace('\\', '/'),
                        WorkingDirectory = this.YartDirectory + "\\x64\\Release",
                        //RedirectStandardOutput = true,
                        //RedirectStandardError = true,
                        //UseShellExecute = false,
                        //CreateNoWindow = true,
                    };

                    var process = new Process {
                        StartInfo = startInfo,
                        EnableRaisingEvents = true
                    };

                    //process.OutputDataReceived += OnOutputDataReceived;
                    process.Start();
                    //process.BeginOutputReadLine();
                    process.WaitForExit();

                    
                }
            );

            task.ContinueWith(
                (_) => {
                    var outputLine = this.stdout.Text.Split('\n').First(x => x.StartsWith("output"));
                    var first = outputLine.IndexOf('\"');
                    var last = outputLine.LastIndexOf('\"');
                    var outputFileName = outputLine.Substring(first + 1, last - first - 1);
                    outputFileName = this.YartDirectory + "\\x64\\Release\\" + outputFileName;
                    var image = Image.FromFile(outputFileName);

                    var form = new Form2();
                    form.ShowImage(outputFileName);
                    form.Show();
                },
                TaskScheduler.FromCurrentSynchronizationContext()
            );

            task.Start();

            //Task.Factory.ContinueWhenAll(
            //    new [] { task },
            //    (tasks) => {
            //        var outputLine = this.stdout.Text.Split('\n').First(x => x.StartsWith("output"));
            //        var first = outputLine.IndexOf('\"');
            //        var last = outputLine.LastIndexOf('\"');
            //        var outputFileName = outputLine.Substring(first + 1, last - first - 1);
            //        outputFileName = this.YartDirectory + "\\" + outputFileName;
            //        var image = Image.FromFile(outputFileName);

            //        var form = new Form2();
            //        form.ShowImage(outputFileName);
            //        form.Show();
            //    }
            //);

            //while (!process.HasExited) {
            //    var ch = process.StandardOutput.Read();
            //    this.stdout.BeginInvoke(
            //        new handler ReadOutputLoop,

            //    );
            //    //this.stdout.Text += (Char)ch;
            //}

            //process.WaitForExit();
            //while (!process.HasExited) {
            //    //Application.DoEvents();

            //}

            //while (!process.StandardOutput.EndOfStream) {
            //    var ch = process.StandardOutput.Read();
            //    this.stdout.Text += (Char)ch;
            //}

            //process.WaitForExit();
            //var output = process.StandardOutput;
            //var input = process.StandardInput;

        }

        private void ReadOutputLoop(int ch) {
            this.stdout.Text += (Char)ch;
        }

        private void OnOutputDataReceived(Object process, DataReceivedEventArgs line) {
            if (this.stdout.InvokeRequired) {
                this.stdout.BeginInvoke(
                    new DataReceivedEventHandler(OnOutputDataReceived),
                    process,
                    line
                );
            } else {
                this.stdout.Text += line.Data + "\r\n";
            }
        }
    }
}
