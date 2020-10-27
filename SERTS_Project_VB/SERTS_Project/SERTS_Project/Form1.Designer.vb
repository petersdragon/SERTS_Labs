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
        Me.SuspendLayout()
        '
        'playPauseButton
        '
        Me.playPauseButton.Location = New System.Drawing.Point(189, 278)
        Me.playPauseButton.Name = "playPauseButton"
        Me.playPauseButton.Size = New System.Drawing.Size(75, 23)
        Me.playPauseButton.TabIndex = 5
        Me.playPauseButton.Text = "Play/Pause"
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
        Me.refreshButton.Location = New System.Drawing.Point(371, 278)
        Me.refreshButton.Name = "refreshButton"
        Me.refreshButton.Size = New System.Drawing.Size(75, 23)
        Me.refreshButton.TabIndex = 3
        Me.refreshButton.Text = "Refresh"
        Me.refreshButton.UseVisualStyleBackColor = True
        '
        'SerialPort1
        '
        Me.SerialPort1.PortName = "COM6"
        '
        'stopButton
        '
        Me.stopButton.Location = New System.Drawing.Point(281, 278)
        Me.stopButton.Name = "stopButton"
        Me.stopButton.Size = New System.Drawing.Size(75, 23)
        Me.stopButton.TabIndex = 6
        Me.stopButton.Text = "Stop"
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
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(800, 450)
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
End Class
