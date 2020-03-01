:userdoc.

:h1 res=500 name=HELP_GENERAL_ID_MAINWND.General Help
:i1 id=helpGeneral.General Help
:p.PMSnap is a screen capture utility, and this is the online documentation for PMSnap.
:p.You have selected the :hp2.General help:ehp2. choice to display this help.
To get more information for PMSnap you can select other choices in the :hp2.Help:ehp2. menu:
:ul.
:li.:hp2.Help Index:ehp2.; an index of references to help topics
:li.:hp2.Using help:ehp2.; information about using the help facility
:li.:hp2.Keys help:ehp2.; a list and description of keys you can use
:li.:hp2.About PMSnap...:ehp2.; the product information for PMSnap
:eul.
:p.You can also select an item in PMSnap and press F1 to get detailed information about that item.
:p.For example, if you request help while a menu bar choice is highlighted,
you get specific information about that choice. If you are in a window,
you get general or specific information that is related to that window.

:h1 res=510 name=HELP_KEYS_ID_MAINWND.Keys help
:i1 id=helpKeys.Keys help
:p.When two key names are joined by a plus sign (+),
use these two keys together. Hold down the first key
and press the second key.
:parml tsize=15 break=none.
:hp2.
:pt.Ctrl + Ins
:ehp2.
:link reftype=hd res=611.
:pd.Copy
:elink.
:hp2.
:pt.Shift + Ins
:ehp2.
:link reftype=hd res=612.
:pd.Paste
:elink.
:hp2.
:pt.Esc
:ehp2.
:link reftype=hd res=621.
:pd.Cancel Capture
:elink.
:hp2.
:pt.Ctrl + W
:ehp2.
:pd.Capture 
:link reftype=hd res=622.
Window
:elink.
:hp2.
:pt.Ctrl + I
:ehp2.
:pd.Capture 
:link reftype=hd res=623.
Window Interior
:elink.
:hp2.
:pt.Ctrl + S
:ehp2.
:pd.Capture 
:link reftype=hd res=624.
Screen
:elink.
:hp2.
:pt.Ctrl + A
:ehp2.
:pd.Capture 
:link reftype=hd res=625.
Area of Screen
:elink.
:hp2.
:pt.F4
:ehp2.
:link reftype=hd res=631.
:pd.Fit in window
:elink.
:hp2.
:pt.Ctrl + M
:ehp2.
:link reftype=hd res=650.
:pd.Magnifying pointer
:elink.
:hp2.
:pt.Ctrl + X
:ehp2.
:link reftype=hd res=602.
:pd.Exit
:elink.
:hp2.
:pt.F3
:ehp2.
:link reftype=hd res=602.
:pd.Exit
:elink.
:eparml.

:h1 res=600 name=HELP_IDM_FILE.File
:i1 id=FileMenu.File
:p.Help for :hp2.File:ehp2. pulldown menu items:
:parml tsize=15 break=none.
:link reftype=hd res=601.
:pt.Save as...
:elink.
:link reftype=hd res=602.
:pt.Exit
:elink.
:eparml.

:h1 res=601 name=HELP_IDM_SAVEAS.Save as
:i2 refid=FileMenu.Save as
:p.Use :hp2.Save as...:ehp2. to open a
:link reftype=hd res=680.
Save Bitmap
:elink.
dialog to save the bitmap to a file.
(OS/2 2.x and Windows 3.x, 8 and 24-bit bitmaps.)
:p.Disabled if nothing to save.

:h1 res=602 name=HELP_IDM_EXIT.Exit
:i2 refid=FileMenu.Exit
:p.:hp2.Exit:ehp2. quits PMSnap and returns you to OS/2.
:p.(shortcut key&colon. :hp2.Ctrl+X:ehp2. or :hp2.F3:ehp2.)

:h1 res=610 name=HELP_IDM_EDIT.Edit
:i1 id=EditMenu.Edit
:p.Help for :hp2.Edit:ehp2. pulldown menu items:
:parml tsize=15 break=none.
:link reftype=hd res=611.
:pt.Copy
:elink.
:link reftype=hd res=612.
:pt.Paste
:elink.
:eparml.

:h1 res=611 name=HELP_IDM_COPY.Copy
:i2 refid=EditMenu.Copy
:p.:hp2.Copy:ehp2. bitmap to clipboard.
:p.If your display driver supports Palette Manager, and if "Palette options"
"Copy/Paste palette to/from clipboard", is grayed, a dialog asks whether you
want to copy palette to clipboard or not.
:p.Disabled if nothing to copy.
:p.(shortcut key&colon. :hp2.Ctrl+Insert:ehp2.)

:h1 res=612 name=HELP_IDM_PASTE.Paste
:i2 refid=EditMenu.Paste
:p.:hp2.Paste:ehp2. bitmap to clipboard.
:p.If your display driver supports Palette Manager, and if "Palette options"
 "Copy/Paste palette to/from clipboard", is grayed, a dialog asks whether you
want to paste palette from clipboard or not.
:p.Disabled if nothing to paste.
:p.(shortcut key&colon. :hp2.Shift-Insert:ehp2.)

:h1 res=620 name=HELP_IDM_CAPTURE.Capture
:i1 id=CaptureMenu.Capture
:p.Help for :hp2.Capture:ehp2. pulldown menu items:
:parml tsize=15 break=none.
:link reftype=hd res=621.
:pt.Cancel Capture
:elink.
:link reftype=hd res=622.
:pt.Window
:elink.
:link reftype=hd res=623.
:pt.Window Interior
:elink.
:link reftype=hd res=624.
:pt.Screen
:elink.
:link reftype=hd res=625.
:pt.Area of Screen
:elink.
:eparml.

:h1 res=621 name=HELP_IDM_CANCELCAPTURE.Cancel Capture
:i2 refid=CaptureMenu.Cancel Capture
:p.:hp2.Cancel Capture:ehp2. cancels a started (but not yet finished) capture.
:p.Disabled when you are not capturing anything.
:p.(shortcut key&colon. :hp2.ESC:ehp2.)

:h1 res=622 name=HELP_IDM_ACTIVE.Window
:i2 refid=CaptureMenu.Window
:p.:hp2.Window:ehp2. captures a specified window on the screen.
:p.PMSnap first changes the pointer to a red pointer pointing on a
mini-window. Select the window you want to capture by pointing
at, and clicking mouse button 1.
:p.If "Allow full capture" checked and if window size exceeds screen size
(either vertically, horizontally or both) PMSnap displays a dialog where
you can choose if you want to continue or not.
:p.(shortcut key&colon. :hp2.Ctrl+W:ehp2.)

:h1 res=623 name=HELP_IDM_ACTIVECLIENT.Window Interior
:i2 refid=CaptureMenu.Window Interior
:p.:hp2.Window Interior:ehp2. captures a specified window interior on the screen.
:p.PMSnap first changes the pointer to a red pointer pointing on a
mini-window. Select the window interior you want to capture by pointing
at, and clicking mouse button 1.
:p.If "Allow full capture" checked and if window interior size exceeds screen size
(either vertically, horizontally or both) PMSnap displays a dialog where
you can choose if you want to continue or not.
:note.Some windows (ex. dialog windows) do not have a client area,
and PMSnap is not able to determine what you want to capture, so
PMSnap captures the entire window.
:note."Win-OS/2 window" sessions are captured without their border
but for example titlebars and menubars are captured.
:p.(shortcut key&colon. :hp2.Ctrl+I:ehp2.)

:h1 res=624 name=HELP_IDM_DESKTOP.Screen
:i2 refid=CaptureMenu.Screen
:p.:hp2.Screen:ehp2. captures the entire screen.
:p.(shortcut key&colon. :hp2.Ctrl+S:ehp2.)

:h1 res=625 name=HELP_IDM_REGION.Area of Screen
:i2 refid=CaptureMenu.Area of Screen
:p.:hp2.Area of Screen:ehp2. captures a specified area of screen.
:p.PMSnap first changes the pointer to a crosshair (alternatively with a
:link reftype=hd res=650.
Magnifying pointer
:elink.
in the centre of the crosshair, see
:link reftype=hd res=650.
Options
:elink.
for details). Place the pointer where you want to start your capture;
press mouse button 1 (keep it down); drag the crosshair so that the
tracking rectangle covers the area you want to capture;
release mouse button 1. You may alternatively use cursor (arrow)
keys for precise tracking.
:p.Pressing backspace-button during tracking gives you the
opportunity to restart capture. ESC cancels capture.
:p.Ctrl+M toggles 
:link reftype=hd res=650.
Magnifying pointer
:elink.
on/off.
:note.You can change the settings for the tracking rectangle in
the Options-dialog; whether the tracking rectangle itself is part
of the capture area or not. 
:note.If you use :hp2.Area of Screen:ehp2. capture on animated areas
(the window is repainted while the crosshair is on top of that window),
parts of the crosshair, are sometimes left in the window. If you want
to get rid of this garbage just resize, hide or minimize that window
(this causes the window to be repainted). We recommend you use
:link reftype=hd res=622.
Window
:elink.
or
:link reftype=hd res=623.
Window Interior
:elink.
capture on animated areas, and then
when you have the captured window in PMSnap, you can use "Area of
Screen" to capture detailed parts of that window. 
:p.(shortcut key&colon. :hp2.Ctrl+A:ehp2.)

:h1 res=630 name=HELP_IDM_DISPLAY.Display
:i1 id=DisplayMenu.Display
:p.Help for :hp2.Display:ehp2. pulldown menu items:
:parml tsize=15 break=none.
:link reftype=hd res=631.
:pt.Fit in Window
:elink.
:link reftype=hd res=632.
:pt.Options...
:elink.
:link reftype=hd res=633.
:pt.Palette options...
:elink.
:eparml.

:h1 res=631 name=HELP_IDM_STRETCH.Fit in Window
:i2 refid=DisplayMenu.Fit in Window
:p.If unchecked; shows the bitmap in its actual size.
If picture bigger than PMSnap client area, use scrollbars
to view hidden parts.
:p.If checked; shows the bitmap stretched (keeping its proportions)
to fit the PMSnap client area.
:p.(shortcut key&colon. :hp2.F4:ehp2.)

:h1 res=632 name=HELP_IDM_OPTIONS.Options...
:i2 refid=DisplayMenu.Options...
:p.:hp2.Options...:ehp2. opens a
:link reftype=hd res=650.
Options
:elink.
dialog.

:h1 res=633 name=HELP_IDM_PALETTEOPTIONS.Palette options...
:i2 refid=DisplayMenu.Palette options...
:p.:hp2.Palette options...:ehp2. opens a
:link reftype=hd res=670.
Palette options
:elink.
dialog.

:h1 res=650 name=HELP_ID_OPTIONS.Options
:i1 id=OptionsDialog.Options
:p.Help for :hp2.Options:ehp2. dialog items:
:parml tsize=15 break=none.
:link reftype=hd res=651.
:pt.Hide PMSnap during capture
:elink.
:link reftype=hd res=654.
:pt.Sound after capture
:elink.
:link reftype=hd res=655.
:pt.Capture delayed
:elink.
:link reftype=hd res=656.
:pt.Allow full capture
:elink.
:link reftype=hd res=657.
:pt.Magnifying pointer
:elink.
:link reftype=hd res=658.
:pt.Magnify by
:elink.
:link reftype=hd res=659.
:pt.Include rectangle
:elink.
:link reftype=hd res=663.
:pt.OK
:elink.
:link reftype=hd res=664.
:pt.Cancel
:elink.
:eparml.

:h1 res=651 name=HELP_IDC_HIDE.Hide PMSnap during capture
:i2 refid=OptionsDialog.Hide PMSnap during capture
:p.

:h1 res=654 name=HELP_IDC_BEEP.Sound after capture
:i2 refid=OptionsDialog.Sound after capture
:p.

:h1 res=655 name=HELP_IDC_DELAYED.Capture delayed
:i2 refid=OptionsDialog.Capture delayed
:p.

:h1 res=656 name=HELP_IDC_FULLCAPTURE.Allow full capture
:i2 refid=OptionsDialog.Allow full capture
:p.

:h1 res=657 name=HELP_IDC_MAGNIFY.Magnifying pointer
:i2 refid=OptionsDialog.Magnifying pointer
:p.

:h1 res=658 name=HELP_IDC_MAGNIFYBY.Magnify by
:i2 refid=OptionsDialog.Magnify by
:p.

:h1 res=659 name=HELP_IDC_INCLUDE.Include rectangle
:i2 refid=OptionsDialog.Include rectangle
:p.

:h1 res=663 name=HELP_DID_OK.OK
:i2 refid=OptionsDialog.OK
:p.You use the :hp2.OK:ehp2. button to ...

:h1 res=664 name=HELP_DID_CANCEL.Cancel
:i2 refid=OptionsDialog.Cancel
:p.You use the :hp2.Cancel:ehp2. button to ...

:h1 res=670 name=HELP_ID_PALETTEOPTIONS.Palette options
:i1 id=PaletteOptionsDialog.Palette options
:p.Help for :hp2.Palette options:ehp2. dialog items:
:parml tsize=15 break=none.
:link reftype=hd res=671.
:pt.Copy/Paste palette to/from clipboard
:elink.
:link reftype=hd res=672.
:pt.Aggressive on palette
:elink.
:link reftype=hd res=663.
:pt.OK
:elink.
:link reftype=hd res=664.
:pt.Cancel
:elink.
:eparml.

:h1 res=671 name=HELP_IDC_COPYPALETTE.Copy/Paste palette to/from clipboard
:i2 refid=OptionsDialog.Copy/Paste palette to/from clipboard
:p.

:h1 res=672 name=HELP_IDC_AGRPALETTE.Aggressive on palette
:i2 refid=OptionsDialog.Aggressive on palette
:p.

:h1 res=680 name=HELP_ID_SAVEAS.Save Bitmap
:i1 id=SaveAsDialog.Save Bitmap
:p.You use the :hp2.Save Bitmap:ehp2. dialog to name and save a new file.
:p.Give a name and press :hp2.Save:ehp2. button, to cancel press :hp2.Cancel:ehp2. button.
:note.If file exists, the previous file is overwritten!
:note.PMSnap forces a :hp2.BMP:ehp2. file extension!

:euserdoc.


