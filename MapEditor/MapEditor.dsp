# Microsoft Developer Studio Project File - Name="MapEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MapEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MapEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MapEditor.mak" CFG="MapEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MapEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MapEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MapEditor - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "inc" /I "../comm" /I "../propertyviewlib" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D WINVER=0x0500 /Fp"Release/pch/MapEditor.pch" /Yu"stdafx.h" /Fo"Release/obj/" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 cximage.lib lua5.1.lib zextract.lib version.lib codeconvert.lib /nologo /subsystem:windows /machine:I386 /out:"../bin/DescentAssist.exe" /libpath:"../lib/release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MapEditor - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "inc" /I "../comm" /I "../propertyviewlib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D WINVER=0x0500 /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cximaged.lib lua5.1.lib zextract.lib version.lib codeconvert.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../bin/DescentAssist.exe" /pdbtype:sept /libpath:"../lib/debug" /libpath:"../lib"

!ENDIF 

# Begin Target

# Name "MapEditor - Win32 Release"
# Name "MapEditor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CanvasScrollView\CanvasScrollView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorPropertyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBar\ExStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=..\comm\FVersion.cpp
# End Source File
# Begin Source File

SOURCE=..\comm\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoShowDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\LogManager\LogManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MapEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\MapEditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MapEditorSplitterView.cpp
# End Source File
# Begin Source File

SOURCE=.\MapPropertyDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\comm\MemBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\MESplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectPropertyView.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyObject.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolbar\TrueColorToolBar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CanvasScrollView\CanvasScrollView.h
# End Source File
# Begin Source File

SOURCE=.\comm.h
# End Source File
# Begin Source File

SOURCE=.\EditorPropertyDlg.h
# End Source File
# Begin Source File

SOURCE=.\StatusBar\ExStatusBar.h
# End Source File
# Begin Source File

SOURCE=..\comm\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\InfoShowDialog.h
# End Source File
# Begin Source File

SOURCE=.\LeftFormView.h
# End Source File
# Begin Source File

SOURCE=.\LogManager\LogManager.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MapEditor.h
# End Source File
# Begin Source File

SOURCE=.\MapEditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\MapEditorSplitterView.h
# End Source File
# Begin Source File

SOURCE=.\MapPropertyDlg.h
# End Source File
# Begin Source File

SOURCE=.\MESplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\ObjectPropertyView.h
# End Source File
# Begin Source File

SOURCE=.\PropertyObject.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolbar\TrueColorToolBar.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BMP\bmp_debugToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BMP\bmp_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BMP\bmp_toolbar_disabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CURSOR\cursor_dragging.cur
# End Source File
# Begin Source File

SOURCE=.\res\CURSOR\cursor_hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\CURSOR\cursor_rectangle.cur
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\debug_dialog_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\MainToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MapEditor.ico
# End Source File
# Begin Source File

SOURCE=.\MapEditor.rc
# End Source File
# Begin Source File

SOURCE=.\res\MapEditor.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MapEditorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\xpstyle.manifest
# End Source File
# End Target
# End Project
