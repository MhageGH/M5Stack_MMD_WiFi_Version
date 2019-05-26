namespace MmdViewerTestCS
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fILEFToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openOToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionOToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.physicsEnabledToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.boneMeshEnabledToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.rigidMeshEnabledToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.jointMeshEnabledToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.button1 = new System.Windows.Forms.Button();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.button2 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fILEFToolStripMenuItem,
            this.optionOToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(638, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fILEFToolStripMenuItem
            // 
            this.fILEFToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openOToolStripMenuItem});
            this.fILEFToolStripMenuItem.Name = "fILEFToolStripMenuItem";
            this.fILEFToolStripMenuItem.Size = new System.Drawing.Size(54, 20);
            this.fILEFToolStripMenuItem.Text = "FILE(&F)";
            // 
            // openOToolStripMenuItem
            // 
            this.openOToolStripMenuItem.Name = "openOToolStripMenuItem";
            this.openOToolStripMenuItem.Size = new System.Drawing.Size(120, 22);
            this.openOToolStripMenuItem.Text = "Open(&O)";
            this.openOToolStripMenuItem.Click += new System.EventHandler(this.openOToolStripMenuItem_Click);
            // 
            // optionOToolStripMenuItem
            // 
            this.optionOToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.physicsEnabledToolStripMenuItem,
            this.boneMeshEnabledToolStripMenuItem,
            this.rigidMeshEnabledToolStripMenuItem,
            this.jointMeshEnabledToolStripMenuItem});
            this.optionOToolStripMenuItem.Name = "optionOToolStripMenuItem";
            this.optionOToolStripMenuItem.Size = new System.Drawing.Size(73, 20);
            this.optionOToolStripMenuItem.Text = "Option(&O)";
            // 
            // physicsEnabledToolStripMenuItem
            // 
            this.physicsEnabledToolStripMenuItem.CheckOnClick = true;
            this.physicsEnabledToolStripMenuItem.Name = "physicsEnabledToolStripMenuItem";
            this.physicsEnabledToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
            this.physicsEnabledToolStripMenuItem.Text = "Physics Enabled";
            this.physicsEnabledToolStripMenuItem.Click += new System.EventHandler(this.physicsEnabledToolStripMenuItem_Click);
            // 
            // boneMeshEnabledToolStripMenuItem
            // 
            this.boneMeshEnabledToolStripMenuItem.CheckOnClick = true;
            this.boneMeshEnabledToolStripMenuItem.Name = "boneMeshEnabledToolStripMenuItem";
            this.boneMeshEnabledToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
            this.boneMeshEnabledToolStripMenuItem.Text = "Bone Mesh Enabled";
            this.boneMeshEnabledToolStripMenuItem.Click += new System.EventHandler(this.boneMeshEnabledToolStripMenuItem_Click);
            // 
            // rigidMeshEnabledToolStripMenuItem
            // 
            this.rigidMeshEnabledToolStripMenuItem.CheckOnClick = true;
            this.rigidMeshEnabledToolStripMenuItem.Name = "rigidMeshEnabledToolStripMenuItem";
            this.rigidMeshEnabledToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
            this.rigidMeshEnabledToolStripMenuItem.Text = "Rigid Mesh Enabled";
            this.rigidMeshEnabledToolStripMenuItem.Click += new System.EventHandler(this.rigidMeshEnabledToolStripMenuItem_Click);
            // 
            // jointMeshEnabledToolStripMenuItem
            // 
            this.jointMeshEnabledToolStripMenuItem.CheckOnClick = true;
            this.jointMeshEnabledToolStripMenuItem.Name = "jointMeshEnabledToolStripMenuItem";
            this.jointMeshEnabledToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
            this.jointMeshEnabledToolStripMenuItem.Text = "Joint Mesh Enabled";
            this.jointMeshEnabledToolStripMenuItem.Click += new System.EventHandler(this.jointMeshEnabledToolStripMenuItem_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(563, 1);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "capture";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // timer2
            // 
            this.timer2.Enabled = true;
            this.timer2.Interval = 1;
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(482, 1);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Start";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(376, 3);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 19);
            this.textBox1.TabIndex = 3;
            this.textBox1.Text = "192.168.11.8";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(638, 478);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "MMD Viewer";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fILEFToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openOToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionOToolStripMenuItem;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ToolStripMenuItem physicsEnabledToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem boneMeshEnabledToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem rigidMeshEnabledToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem jointMeshEnabledToolStripMenuItem;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox textBox1;
    }
}

