using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using MmdViewerCLR;

namespace MmdViewerTestCS
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            var controlVariableCLRWrapper = new ControlVariableCLRWrapper();
            Form1 form1 = new Form1(controlVariableCLRWrapper);
            form1.Show();
            var myApplicationCLR = new MyApplicationCLR(form1.Handle, controlVariableCLRWrapper, form1.ClientRectangle.Width, form1.ClientRectangle.Height);
            while (form1.Created)
            {
                myApplicationCLR.Run();
                Application.DoEvents();
            }
        }
    }
}
