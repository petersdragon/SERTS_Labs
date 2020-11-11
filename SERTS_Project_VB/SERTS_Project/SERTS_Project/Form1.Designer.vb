<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.playPauseButton = New System.Windows.Forms.Button()
        Me.filesList = New System.Windows.Forms.ListBox()
        Me.refreshButton = New System.Windows.Forms.Button()
        Me.SerialPort1 = New System.IO.Ports.SerialPort(Me.components)
        Me.stopButton = New System.Windows.Forms.Button()
        Me.selectedSongLabel = New System.Windows.Forms.Label()
        Me.SelectButton = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'playPauseButton
        '
        Me.playPauseButton.Font = New System.Drawing.Font("Microsoft Sans Serif", 16.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.playPauseButton.Location = New System.Drawing.Point(189, 262)
        Me.playPauseButton.Name = "playPauseButton"
        Me.playPauseButton.Size = New System.Drawing.Size(41, 39)
        Me.playPauseButton.TabIndex = 5
        Me.playPauseButton.Text = "⏯️"
        Me.playPauseButton.UseVisualStyleBackColor = True
        '
        'filesList
        '
        Me.filesList.FormattingEnabled = True
        Me.filesList.ItemHeight = 16
        Me.filesList.Location = New System.Drawing.Point(189, 12)
        Me.filesList.Name = "filesList"
        Me.filesList.Size = New System.Drawing.Size(257, 244)
        Me.filesList.TabIndex = 4
        '
        'refreshButton
        '
        Me.refreshButton.Font = New System.Drawing.Font("Microsoft Sans Serif", 16.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.refreshButton.Location = New System.Drawing.Point(284, 262)
        Me.refreshButton.Name = "refreshButton"
        Me.refreshButton.Size = New System.Drawing.Size(39, 39)
        Me.refreshButton.TabIndex = 3
        Me.refreshButton.Text = "⟳"
        Me.refreshButton.UseVisualStyleBackColor = True
        '
        'SerialPort1
        '
        Me.SerialPort1.PortName = "COM4"
        '
        'stopButton
        '
        Me.stopButton.Font = New System.Drawing.Font("Microsoft Sans Serif", 16.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.stopButton.Location = New System.Drawing.Point(236, 262)
        Me.stopButton.Name = "stopButton"
        Me.stopButton.Size = New System.Drawing.Size(42, 39)
        Me.stopButton.TabIndex = 6
        Me.stopButton.Text = "⏹️"
        Me.stopButton.UseVisualStyleBackColor = True
        '
        'selectedSongLabel
        '
        Me.selectedSongLabel.AutoSize = True
        Me.selectedSongLabel.Location = New System.Drawing.Point(186, 314)
        Me.selectedSongLabel.Name = "selectedSongLabel"
        Me.selectedSongLabel.Size = New System.Drawing.Size(215, 17)
        Me.selectedSongLabel.TabIndex = 7
        Me.selectedSongLabel.Text = "Selected Song Is Displayed Here"
        Me.selectedSongLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'SelectButton
        '
        Me.SelectButton.Location = New System.Drawing.Point(329, 262)
        Me.SelectButton.Name = "SelectButton"
        Me.SelectButton.Size = New System.Drawing.Size(117, 39)
        Me.SelectButton.TabIndex = 8
        Me.SelectButton.Text = "Select"
        Me.SelectButton.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(800, 450)
        Me.Controls.Add(Me.SelectButton)
        Me.Controls.Add(Me.selectedSongLabel)
        Me.Controls.Add(Me.stopButton)
        Me.Controls.Add(Me.playPauseButton)
        Me.Controls.Add(Me.filesList)
        Me.Controls.Add(Me.refreshButton)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents playPauseButton As Button
    Friend WithEvents filesList As ListBox
    Friend WithEvents refreshButton As Button
    Friend WithEvents SerialPort1 As IO.Ports.SerialPort
    Friend WithEvents stopButton As Button
    Friend WithEvents selectedSongLabel As Label
    Friend WithEvents SelectButton As Button
End Class
