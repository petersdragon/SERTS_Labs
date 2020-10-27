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
        Me.sendFileButton = New System.Windows.Forms.Button()
        Me.filesList = New System.Windows.Forms.ListBox()
        Me.showFilesButton = New System.Windows.Forms.Button()
        Me.SerialPort1 = New System.IO.Ports.SerialPort(Me.components)
        Me.SuspendLayout()
        '
        'sendFileButton
        '
        Me.sendFileButton.Location = New System.Drawing.Point(285, 66)
        Me.sendFileButton.Name = "sendFileButton"
        Me.sendFileButton.Size = New System.Drawing.Size(75, 23)
        Me.sendFileButton.TabIndex = 5
        Me.sendFileButton.Text = "Send"
        Me.sendFileButton.UseVisualStyleBackColor = True
        '
        'filesList
        '
        Me.filesList.FormattingEnabled = True
        Me.filesList.ItemHeight = 16
        Me.filesList.Location = New System.Drawing.Point(189, 108)
        Me.filesList.Name = "filesList"
        Me.filesList.Size = New System.Drawing.Size(422, 276)
        Me.filesList.TabIndex = 4
        '
        'showFilesButton
        '
        Me.showFilesButton.Location = New System.Drawing.Point(189, 66)
        Me.showFilesButton.Name = "showFilesButton"
        Me.showFilesButton.Size = New System.Drawing.Size(75, 23)
        Me.showFilesButton.TabIndex = 3
        Me.showFilesButton.Text = "Show Files"
        Me.showFilesButton.UseVisualStyleBackColor = True
        '
        'SerialPort1
        '
        Me.SerialPort1.PortName = "COM5"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(800, 450)
        Me.Controls.Add(Me.sendFileButton)
        Me.Controls.Add(Me.filesList)
        Me.Controls.Add(Me.showFilesButton)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents sendFileButton As Button
    Friend WithEvents filesList As ListBox
    Friend WithEvents showFilesButton As Button
    Friend WithEvents SerialPort1 As IO.Ports.SerialPort
End Class
