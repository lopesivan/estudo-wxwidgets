from wxPython.wx import *
from wxPython.xrc import *

class MyFileDropTarget(wxFileDropTarget):
    def __init__(self, ctrlText):
        wxFileDropTarget.__init__(self)
        self.ctrlText = ctrlText
        self.ctrlText.SetDropTarget(self)

    def OnDropFiles(self, x, y, filenames):
        for file in filenames:
            self.ctrlText.SetValue(open(file, 'r').read())

class MyApp(wxApp):
    def OnInit(self):
        self.res = wxXmlResource("acme.xrc", wxXRC_NO_SUBCLASSING)
        self.InitDialog()
        self.InitDialogNewProject()
        self.InitFrame()
        return True

    def InitFrame(self):
        self.frame = self.res.LoadFrame(None, "ID_FRAME")
        EVT_MENU(self.frame, XRCID("wxID_EXIT"), self.OnQuit)
        EVT_CLOSE(self.frame, self.OnCloseWindow)
        self.frame.Show(1)
        self.SetTopWindow(self.frame)

    def InitDialog(self):
        self.dialog = self.res.LoadDialog(None, "ID_NOTEBOOK_DIALOG")
        self.dialog.Show(1)

    def InitDialogNewProject(self):
        self.dialogNewProject = self.res.LoadDialog(None, "ID_NEW_PROJECT_DIALOG")
        MyFileDropTarget(XRCCTRL(self.dialogNewProject, "ID_TEXTCTRL_DESCRIPTION"))
        self.dialogNewProject.Show(1)

    def OnCheckbox(self, event):
        wxMessageBox("hello")

    def OnQuit(self, event):
        self.frame.Close(True)

    def OnCloseWindow(self, event):
        self.frame.Destroy()
        self.dialog.Destroy()
        self.dialogNewProject.Destroy()

def main():
    wxInitAllImageHandlers()
    app = MyApp(0)
    app.MainLoop()

if __name__ == '__main__':
    main()
