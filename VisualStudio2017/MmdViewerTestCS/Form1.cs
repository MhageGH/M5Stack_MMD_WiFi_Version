using System;
using System.Drawing;
using System.Windows.Forms;
using MmdViewerCLR;
using System.IO;
using System.Drawing.Imaging;

namespace MmdViewerTestCS
{
    public partial class Form1 : Form
    {
        [System.Runtime.InteropServices.DllImport("User32.dll")]
        private extern static bool PrintWindow(IntPtr hwnd, IntPtr hDC, uint nFlags);
        private ControlVariableCLRWrapper controlVariableCLRWrapper;
        private float[] rotation;

        public Form1(ControlVariableCLRWrapper controlVariableCLRWrapper)
        {
            this.controlVariableCLRWrapper = controlVariableCLRWrapper;
            InitializeComponent();
            foreach (var ici in ImageCodecInfo.GetImageEncoders())
            {
                if (ici.FormatID == ImageFormat.Jpeg.Guid)
                {
                    _jpgEncoder = ici;
                    break;
                }
            }
            rotation = new float[9] { 1, 0, 0, 0, 0, 1, 0, 1, 0 };
            _tcp = new TCPSender(rotation);
        }

        // フォーム開始イベント
        // 視点/注視点を設定
        private void Form1_Load(object sender, EventArgs e)
        {
            controlVariableCLRWrapper.eyePoint = new Vector3(10.0f, 20.0f, -50.0f);
            controlVariableCLRWrapper.lookAtPoint = new Vector3(0, 10.0f, 0);
        }

        // メニューOpenボタンクリックイベント
        // ファイルを開く
        private void openOToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "pmdファイル(*.pmd)|*.pmd|vmdファイル(*.vmd)|*.vmd";
            openFileDialog.RestoreDirectory = true; // これをtrueにしないと、カレントディレクトリがファイルを開いたディレクトリに変更されてしまう！
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                String extension = System.IO.Path.GetExtension(openFileDialog.FileName);
                if (extension == ".pmd")
                {
                    controlVariableCLRWrapper.pmdFileName = openFileDialog.FileName;
                    controlVariableCLRWrapper.openPmdFileFlag = true;
                }
                else if (extension == ".vmd")
                {
                    controlVariableCLRWrapper.vmdFileName = openFileDialog.FileName;
                    controlVariableCLRWrapper.openVmdFileFlag = true;
                }
            }
        }

        // タイマイベント
        // 視点の回転、平行移動
        private void timer1_Tick(object sender, EventArgs e)
        {
            var r = new float[3, 3];
            var view = new float[4, 4];
            r[0, 0] = rotation[0];
            r[0, 1] = rotation[1];
            r[0, 2] = rotation[2];
            r[1, 0] = rotation[3];
            r[1, 1] = rotation[4];
            r[1, 2] = rotation[5];
            r[2, 0] = rotation[6];
            r[2, 1] = rotation[7];
            r[2, 2] = rotation[8];

            view[0, 0] = +r[0, 0];
            view[0, 1] = +r[1, 0];
            view[0, 2] = -r[2, 0];
            view[1, 0] = +r[0, 2];
            view[1, 1] = +r[1, 2];
            view[1, 2] = -r[2, 2];
            view[2, 0] = +r[0, 1];
            view[2, 1] = +r[1, 1];
            view[2, 2] = -r[2, 1];

            view[3, 0] = 0;
            view[3, 1] = -10;
            view[3, 2] = 30.0f;
            view[3, 3] = 1;
            controlVariableCLRWrapper.SetViewMatrix(view);
        }

        // メニューPhysics Enabledボタンクリックイベント
        // 物理演算使用可能フラグを切り替える
        private void physicsEnabledToolStripMenuItem_Click(object sender, EventArgs e)
        {
            controlVariableCLRWrapper.physicsEnabled = ((ToolStripMenuItem)sender).Checked;
        }

        // メニューBone Mesh Enabledボタンクリックイベント
        // ボーンメッシュ使用可能フラグを切り替える
        private void boneMeshEnabledToolStripMenuItem_Click(object sender, EventArgs e)
        {
            controlVariableCLRWrapper.boneMeshEnabled = ((ToolStripMenuItem)sender).Checked;
        }

        // メニューRigid Mesh Enabledボタンクリックイベント
        // 剛体メッシュ使用可能フラグを切り替える
        private void rigidMeshEnabledToolStripMenuItem_Click(object sender, EventArgs e)
        {
            controlVariableCLRWrapper.rigidMeshEnabled = ((ToolStripMenuItem)sender).Checked;
        }

        // メニューJoint Mesh Enabledボタンクリックイベント
        // ジョイントメッシュ使用可能フラグを切り替える
        private void jointMeshEnabledToolStripMenuItem_Click(object sender, EventArgs e)
        {
            controlVariableCLRWrapper.jointMeshEnabled = ((ToolStripMenuItem)sender).Checked;
        }

        // captureボタンクリックイベント
        private void button1_Click(object sender, EventArgs e)
        {
            var bitmapBuf = controlVariableCLRWrapper.GetBitmapBuffer();
            if (bitmapBuf[0] != 0)
            {
                var bitmap = new Bitmap(new MemoryStream(bitmapBuf));
                bitmap.Save("screenshot.bmp");
            }
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            timer2.Enabled = false;
            tcpJPGPrepare();
            timer2.Enabled = true;
        }

        int _jpgQuality = 80;
        EncoderParameters _encParams = new EncoderParameters(1);
        ImageCodecInfo _jpgEncoder = null;
        TCPSender _tcp;

        private void tcpJPGPrepare()
        {
            var bitmapBuf = controlVariableCLRWrapper.GetBitmapBuffer();
            if (bitmapBuf[0] != 0)
            {
                var bitmap = new Bitmap(new MemoryStream(bitmapBuf));
                var bitmap2 = new Bitmap(160, 120);
                var gr2 = Graphics.FromImage(bitmap2);
                gr2.DrawImage(bitmap, 0, 0, 160, 120);
                byte[] rgbValues = { 0 };
                using (var ms = new MemoryStream())
                {
                    _encParams.Param[0] = new EncoderParameter(Encoder.Quality, _jpgQuality);
                    ms.WriteByte(0x4A); // prefix "JPG"
                    ms.WriteByte(0x50);
                    ms.WriteByte(0x47);
                    ms.WriteByte(0);
                    ms.WriteByte(0);
                    bitmap2.Save(ms, _jpgEncoder, _encParams);
                    rgbValues = ms.GetBuffer();
                    if (rgbValues.Length < 65536)
                    {
                        UInt16 len = (UInt16)(rgbValues.Length - 5);
                        rgbValues[3] = (byte)(len & 0xFF);
                        rgbValues[4] = (byte)((len >> 8) & 0xFF);
                        _tcp.setData(rgbValues);
                    }
                }
            }
            controlVariableCLRWrapper.bitmapRevising = true;
        }

        int _port = 63333;
        private void button2_Click(object sender, EventArgs e)
        {
            _tcp.start(textBox1.Text, _port);
            button2.Text = "communication started";
        }
    }
}
