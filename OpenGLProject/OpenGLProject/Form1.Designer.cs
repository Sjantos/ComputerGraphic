namespace OpenGLProject
{
	partial class Form1
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.panelMenu = new System.Windows.Forms.Panel();
			this.panelRender = new System.Windows.Forms.Panel();
			this.glControl = new OpenGL.GlControl();
			this.panelRender.SuspendLayout();
			this.SuspendLayout();
			// 
			// panelMenu
			// 
			this.panelMenu.Dock = System.Windows.Forms.DockStyle.Right;
			this.panelMenu.Location = new System.Drawing.Point(584, 0);
			this.panelMenu.Name = "panelMenu";
			this.panelMenu.Size = new System.Drawing.Size(200, 561);
			this.panelMenu.TabIndex = 0;
			// 
			// panelRender
			// 
			this.panelRender.Controls.Add(this.glControl);
			this.panelRender.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panelRender.Location = new System.Drawing.Point(0, 0);
			this.panelRender.Name = "panelRender";
			this.panelRender.Size = new System.Drawing.Size(584, 561);
			this.panelRender.TabIndex = 1;
			// 
			// glControl
			// 
			this.glControl.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
			this.glControl.ColorBits = ((uint)(24u));
			this.glControl.DepthBits = ((uint)(0u));
			this.glControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.glControl.Location = new System.Drawing.Point(0, 0);
			this.glControl.MultisampleBits = ((uint)(0u));
			this.glControl.Name = "glControl";
			this.glControl.Size = new System.Drawing.Size(584, 561);
			this.glControl.StencilBits = ((uint)(0u));
			this.glControl.TabIndex = 0;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(784, 561);
			this.Controls.Add(this.panelRender);
			this.Controls.Add(this.panelMenu);
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Form1";
			this.panelRender.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel panelMenu;
		private System.Windows.Forms.Panel panelRender;
		private OpenGL.GlControl glControl;
	}
}

