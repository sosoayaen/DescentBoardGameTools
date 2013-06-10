; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mapeditor.h"
LastPage=0

ClassCount=13
Class1=CCanvasScrollView
Class2=CInfoShowDialog
Class3=CLeftFormView
Class4=CMainFrame
Class5=CMapEditorApp
Class6=CAboutDlg
Class7=CMapEditorDoc
Class8=CMapEditorSplitterView
Class9=CExStatusBar

ResourceCount=7
Resource1=IDR_TB_INFO_SHOW
Resource2=IDD_ABOUTBOX
Resource3=IDD_SET_EDITOR
Resource4=IDD_INFO_SHOW
Class10=CHyperLink
Resource5=IDD_SET_MAP
Class11=CMapPropertyDlg
Resource6=IDD_LEFTFORMVIEW_FORM
Class12=CEditorPropertyDlg
Class13=CObjectPropertyView
Resource7=IDR_MAINFRAME

[CLS:CCanvasScrollView]
Type=0
BaseClass=CScrollView
HeaderFile=CanvasScrollView\canvasscrollview.h
ImplementationFile=CanvasScrollView\CanvasScrollView.cpp
LastObject=ID_BUTTON_SHOW_GRID
Filter=C
VirtualFilter=VWC

[CLS:CInfoShowDialog]
Type=0
BaseClass=CDialog
HeaderFile=InfoShowDialog.h
ImplementationFile=InfoShowDialog.cpp
LastObject=IDC_LIST_INFO_SHOW_LOG

[CLS:CLeftFormView]
Type=0
BaseClass=CFormView
HeaderFile=LeftFormView.h
ImplementationFile=LeftFormView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=ID_BUTTON_LOS
Filter=T
VirtualFilter=fWC

[CLS:CMapEditorApp]
Type=0
BaseClass=CWinApp
HeaderFile=MapEditor.h
ImplementationFile=MapEditor.cpp
LastObject=ID_BUTTON_LOS
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MapEditor.cpp
ImplementationFile=MapEditor.cpp
LastObject=IDC_STATIC_MAIL
Filter=W
VirtualFilter=dWC

[CLS:CMapEditorDoc]
Type=0
BaseClass=CDocument
HeaderFile=MapEditorDoc.h
ImplementationFile=MapEditorDoc.cpp
LastObject=CMapEditorDoc
Filter=N
VirtualFilter=DC

[CLS:CMapEditorSplitterView]
Type=0
BaseClass=CView
HeaderFile=MapEditorSplitterView.h
ImplementationFile=MapEditorSplitterView.cpp
LastObject=CMapEditorSplitterView
Filter=C
VirtualFilter=VWC

[CLS:CExStatusBar]
Type=0
BaseClass=CStatusBar
HeaderFile=StatusBar\ExStatusBar.h
ImplementationFile=StatusBar\ExStatusBar.cpp
LastObject=CExStatusBar

[DLG:IDD_INFO_SHOW]
Type=1
Class=CInfoShowDialog
ControlCount=1
Control1=IDC_LIST_INFO_SHOW_LOG,listbox,1353777539

[DLG:IDD_LEFTFORMVIEW_FORM]
Type=1
Class=CLeftFormView
ControlCount=2
Control1=IDC_COMBO_ITEMS_TYPE,combobox,1344339971
Control2=IDC_LIST_IMAGE,SysListView32,1350631428

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC_VERSION,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC_MAIL,static,1342308352
Control6=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME]
Type=1
Class=CMapEditorApp
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_APP_ABOUT
Command6=ID_EDIT_DELETE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_CUT
Command9=ID_EDIT_PASTE
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_REDO
Command12=ID_BUTTON_MOVE_CANVAS
Command13=ID_BUTTON_SELECT_MODE
Command14=ID_BUTTON_LOS
Command15=ID_BUTTON_SHOW_GRID
Command16=ID_BUTTON_BACKGROUND_COLOR
Command17=ID_EDIT_ORA_SIZE
Command18=ID_BUTTON_ZOOMIN
Command19=ID_BUTTON_ZOOMOUT
Command20=ID_BUTTON_ROTATE_LEFT_90
Command21=ID_BUTTON_ROTATE_RIGHT_90
Command22=ID_BUTTON_HIDE_PROPERTY
Command23=ID_BUTTON_SHOW_INFO_DLG
CommandCount=23

[TB:IDR_TB_INFO_SHOW]
Type=1
Class=?
Command1=ID_BUTTON_CLEAR_LOG
Command2=ID_BUTTON_SCROLL_LOG
CommandCount=2

[MNU:IDR_MAINFRAME]
Type=1
Class=CMapEditorApp
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_EXPORT
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_SET_MAP
Command16=ID_SET_EDITOR
Command17=ID_VIEW_TOOLBAR
Command18=ID_VIEW_STATUS_BAR
Command19=ID_MI_SHOW_LAYER_TILE
Command20=ID_MI_SHOW_LAYER_DOOR
Command21=ID_MI_SHOW_LAYER_OBSTACLE
Command22=ID_MI_SHOW_LAYER_OBJECT
Command23=ID_MI_SHOW_LAYER_TREASURE
Command24=ID_MI_SHOW_LAYER_MONSTER
Command25=ID_MI_SHOW_LAYER_MASTER
Command26=ID_APP_ABOUT
CommandCount=26

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_EDIT_CUT
Command14=ID_EDIT_REDO
Command15=ID_EDIT_UNDO
CommandCount=15

[CLS:CHyperLink]
Type=0
HeaderFile=HyperLink.h
ImplementationFile=HyperLink.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CHyperLink

[DLG:IDD_SET_MAP]
Type=1
Class=CMapPropertyDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_PROPERTY_MAP,static,1350697473

[CLS:CMapPropertyDlg]
Type=0
HeaderFile=MapPropertyDlg.h
ImplementationFile=MapPropertyDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMapPropertyDlg
VirtualFilter=dWC

[DLG:IDD_SET_EDITOR]
Type=1
Class=CEditorPropertyDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_PROPERTY_EDITOR,static,1350697473

[CLS:CEditorPropertyDlg]
Type=0
HeaderFile=EditorPropertyDlg.h
ImplementationFile=EditorPropertyDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CEditorPropertyDlg

[CLS:CObjectPropertyView]
Type=0
HeaderFile=ObjectPropertyView.h
ImplementationFile=ObjectPropertyView.cpp
BaseClass=EPropView
Filter=C
VirtualFilter=VWC
LastObject=CObjectPropertyView

