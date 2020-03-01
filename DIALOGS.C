/**************************************************************************
 *  File name  : dialogs.c
 *  (c) Copyright Carrick von Schoultz 1994. All rights reserved.
 *  (c) Copyright Peter Nielsen 1994. All rights reserved.
 *  Description: 
 *************************************************************************/
#define  INCL_WINBUTTONS
#define  INCL_WINDIALOGS
#define  INCL_WINFRAMEMGR
#define  INCL_WINHELP
#define  INCL_WININPUT
#define  INCL_WINMENUS
#define  INCL_WINSTDSPIN
#define  INCL_WINSYS
#define  INCL_WINWINDOWMGR
#include <os2.h>
#include <string.h>
#include "pmsnap.h"
#include "dialogs.h"

VOID BitmapCreationError (HWND hwnd) {
   HAB   hab = WinQueryAnchorBlock (hwnd);
   CHAR  szTitle[80];
   CHAR  szError[80];
   WinLoadString (hab, (HMODULE)NULL, SZ_TITLE, sizeof (szTitle), szTitle);
   WinLoadString (hab, (HMODULE)NULL, SZ_BITMAPERROR, sizeof (szError), szError);
   WinMessageBox (HWND_DESKTOP, hwnd, szError, szTitle, 0, MB_MOVEABLE | MB_OK | MB_ICONEXCLAMATION) ;
   }

USHORT ausMoveCloseMenu[] = { 2, SC_MOVE, SC_CLOSE };

VOID SetSysMenu (HWND hwnd, PUSHORT ausItem) {
   MENUITEM mi;
   ULONG    i, ulPos = 0;
   USHORT   usItems, usLastItem, usCount = *ausItem++;

   WinSendDlgItemMsg (hwnd, FID_SYSMENU, MM_QUERYITEM, MPFROM2SHORT (SC_SYSMENU, FALSE), MPFROMP (&mi));
   usItems = SHORT1FROMMR (WinSendMsg (mi.hwndSubMenu, MM_QUERYITEMCOUNT, NULL, NULL));
   while (usItems--) {
      USHORT usItem = SHORT1FROMMR (WinSendMsg (mi.hwndSubMenu, MM_ITEMIDFROMPOSITION, MPFROMLONG (ulPos), NULL));
      for (i = 0; usItem != ausItem[i] && i < usCount; i++);
      if (usItem == ausItem[i] || (usItem > 0x8100 && usLastItem <= 0x8100)) {
         usLastItem = usItem;
         ulPos++;
         }
      else
         WinSendMsg (mi.hwndSubMenu, MM_DELETEITEM, MPFROM2SHORT (usItem, TRUE), NULL);
      }
   if (usLastItem > 0x8100)
      WinSendMsg (mi.hwndSubMenu, MM_DELETEITEM, MPFROM2SHORT (usLastItem, TRUE), NULL);
   }

MRESULT EXPENTRY About2DlgProc (HWND hwnd, LONG msg, MPARAM mp1, MPARAM mp2) {
   switch(msg) {
      case WM_INITDLG:
         SetSysMenu (hwnd, ausMoveCloseMenu);
         return ((MRESULT)FALSE);

      case WM_COMMAND:
         WinDismissDlg (hwnd, TRUE);
         break;
      }
   return (WinDefDlgProc(hwnd, msg, mp1, mp2));
   }

MRESULT EXPENTRY ProductInfoDlgProc (HWND hwnd, LONG msg, MPARAM mp1, MPARAM mp2) {
   switch(msg) {
      case WM_INITDLG:
         {CHAR  szVersion[80];
         SetSysMenu (hwnd, ausMoveCloseMenu);
         WinLoadString (WinQueryAnchorBlock (hwnd), NULLHANDLE, SZ_VERSION, sizeof (szVersion), szVersion);
         WinSetDlgItemText(hwnd, IDC_VERSION, szVersion);
         }return ((MRESULT)FALSE);

      case WM_COMMAND:
         switch (SHORT1FROMMP(mp1)) {
            case DID_OK:
               WinDismissDlg (hwnd, TRUE);
               return ((MRESULT)0);

            case IDC_ICON:
               WinDlgBox (HWND_DESKTOP, hwnd, (PFNWP)About2DlgProc, 0, ID_ABOUT2, (PVOID)NULL);
               return ((MRESULT)0);
            }
         break;
      }
   return (WinDefDlgProc(hwnd, msg, mp1, mp2));
   }

VOID HelpProductInfo (HWND hwnd) {
   WinDlgBox (HWND_DESKTOP, hwnd, (PFNWP)ProductInfoDlgProc, 0, ID_PRODUCTINFO, (PVOID)NULL);
   }

MRESULT EXPENTRY OptionsDlgProc (HWND hwnd, LONG msg, MPARAM mp1, MPARAM mp2) {
   PSETTINGS pSettings = (PSETTINGS)WinQueryWindowPtr (hwnd, QWL_USER);

   switch(msg) {
      case WM_INITDLG:
         SetSysMenu (hwnd, ausMoveCloseMenu);
         WinSetWindowPtr (hwnd, QWL_USER, (PVOID)(pSettings = (PSETTINGS)mp2));
         {
         HWND hwndDelaySPBN = WinWindowFromID (hwnd, IDC_DELAYED);
         WinSendMsg (hwndDelaySPBN, SPBM_SETLIMITS, MPFROMSHORT (60), MPFROMSHORT (0));
         WinSendMsg (hwndDelaySPBN, SPBM_SETCURRENTVALUE, MPFROMSHORT (pSettings->usDelay), NULL);
         }
         {
         HWND hwndMagnifySPBN = WinWindowFromID (hwnd, IDC_MAGNIFYBY);
         static PSZ  apszMagnifySPBNValues[3] = { "1.5", "2.0", "2.5" };
         const  usMagnifySPBNValues = 3;
         WinSendMsg (hwndMagnifySPBN, SPBM_SETTEXTLIMIT, MPFROMSHORT (3), NULL);
         WinSendMsg (hwndMagnifySPBN, SPBM_SETARRAY, MPFROMP (&apszMagnifySPBNValues[0]), MPFROMSHORT (usMagnifySPBNValues));
         WinSendMsg (hwndMagnifySPBN, SPBM_SETCURRENTVALUE, MPFROMSHORT ((pSettings->usMagnify >= usMagnifySPBNValues) ? MAGNIFY_2_0 : pSettings->usMagnify), NULL);
         }
         {
         USHORT usItemID = IDC_HIDE_CAPT;
         switch (pSettings->usHide) {
            case HIDE_INSTANTLY:
               usItemID = IDC_HIDE_INST;
               break;
            case HIDE_NOHIDE:
               usItemID = IDC_HIDE_NO;
               break;
            }
         WinCheckButton (hwnd, usItemID, TRUE);
         }
         WinCheckButton (hwnd, IDC_BEEP, pSettings->fBeep);
         WinCheckButton (hwnd, IDC_FULLCAPTURE, pSettings->fFullCapture);
         WinCheckButton (hwnd, IDC_INCLUDELEFT, pSettings->include.fLeft);
         WinCheckButton (hwnd, IDC_INCLUDETOP, pSettings->include.fTop);
         WinCheckButton (hwnd, IDC_INCLUDERIGHT, pSettings->include.fRight);
         WinCheckButton (hwnd, IDC_INCLUDEBOTTOM, pSettings->include.fBottom);
         WinCheckButton (hwnd, IDC_MAGNIFY, pSettings->fMagnify);
         WinEnableControl (hwnd, IDC_MAGNIFYBY, pSettings->fMagnify);
         WinEnableControl (hwnd, IDC_HELP, (BOOL)WinQueryHelpInstance (hwnd));
         return ((MRESULT)FALSE);

      case WM_CONTROL:
         switch (SHORT1FROMMP (mp1)) {
            case IDC_MAGNIFY:
               WinEnableControl (hwnd, IDC_MAGNIFYBY, (BOOL)WinSendMsg ((HWND)mp2, BM_QUERYCHECK, (MPARAM)NULL, (MPARAM)NULL));
               break;
            }
            break;

      case WM_COMMAND:
         switch (SHORT1FROMMP(mp1)) {
            case DID_OK:
               WinSendDlgItemMsg (hwnd, IDC_MAGNIFYBY, SPBM_QUERYVALUE, MPFROMP (&pSettings->usMagnify), MPFROM2SHORT (0, SPBQ_UPDATEIFVALID));
               WinSendDlgItemMsg (hwnd, IDC_DELAYED, SPBM_QUERYVALUE, MPFROMP (&pSettings->usDelay), MPFROM2SHORT (0, SPBQ_UPDATEIFVALID));
               pSettings->usHide = HIDE_CAPTUREONLY;
               if (WinQueryButtonCheckstate (hwnd, IDC_HIDE_NO))
                  pSettings->usHide = HIDE_NOHIDE;
               else if (WinQueryButtonCheckstate (hwnd, IDC_HIDE_INST))
                  pSettings->usHide = HIDE_INSTANTLY;
               pSettings->fBeep           = WinQueryButtonCheckstate (hwnd, IDC_BEEP);
               pSettings->fFullCapture    = WinQueryButtonCheckstate (hwnd, IDC_FULLCAPTURE);
               pSettings->fMagnify        = WinQueryButtonCheckstate (hwnd, IDC_MAGNIFY);
               pSettings->include.fLeft   = WinQueryButtonCheckstate (hwnd, IDC_INCLUDELEFT);
               pSettings->include.fTop    = WinQueryButtonCheckstate (hwnd, IDC_INCLUDETOP);
               pSettings->include.fRight  = WinQueryButtonCheckstate (hwnd, IDC_INCLUDERIGHT);
               pSettings->include.fBottom = WinQueryButtonCheckstate (hwnd, IDC_INCLUDEBOTTOM);
               // Fall through
            case DID_CANCEL:
               WinDismissDlg (hwnd, FALSE);
               return ((MRESULT)0);
            }
         break;
/*
      case WM_HELP:
         {
         SHORT idItem = WinQueryWindowUShort(WinQueryFocus(HWND_DESKTOP), QWS_ID);
         idItem = (idItem < IDC_HIDE_NO) ? HELP_ID_OPTIONS : ((idItem > IDC_FULLCAPTURE) ? HELP_ID_OPTIONS : idItem);
//         WinSendMsg (hwndHelp, HM_DISPLAY_HELP, MPFROM2SHORT(idItem, NULL), MPFROMSHORT(HM_RESOURCEID));
         WinSendMsg (hwndHelp, HM_DISPLAY_HELP, MPFROM2SHORT(HELP_ID_OPTIONS, NULL), MPFROMSHORT(HM_RESOURCEID));
         }
         break;
*/
      case WM_CLOSE:
         WinSendMsg (hwnd, WM_COMMAND, MPFROMSHORT (DID_OK), MPFROMP (NULL));
         break;
      }
   return (WinDefDlgProc(hwnd, msg, mp1, mp2));
   }

MRESULT EXPENTRY PaletteOptionsDlgProc (HWND hwnd, LONG msg, MPARAM mp1, MPARAM mp2) {
   PSETTINGS pSettings = (PSETTINGS)WinQueryWindowPtr (hwnd, QWL_USER);

   switch(msg) {
      case WM_INITDLG:
         SetSysMenu (hwnd, ausMoveCloseMenu);
         WinSetWindowPtr (hwnd, QWL_USER, (PVOID)(pSettings = (PSETTINGS)mp2));
         WinCheckButton (hwnd, IDC_COPYPALETTE, pSettings->usCopyPalette);
         WinCheckButton (hwnd, IDC_AGRPALETTE, pSettings->fAgrPalette);
         WinEnableControl (hwnd, IDC_HELP, (BOOL)WinQueryHelpInstance (hwnd));
         return ((MRESULT)FALSE);

      case WM_COMMAND:
         switch (SHORT1FROMMP(mp1)) {
            case DID_OK:
               pSettings->usCopyPalette   = WinQueryButtonCheckstate (hwnd, IDC_COPYPALETTE);
               pSettings->fAgrPalette     = WinQueryButtonCheckstate (hwnd, IDC_AGRPALETTE);
               // Fall through
            case DID_CANCEL:
               WinDismissDlg (hwnd, FALSE);
               return ((MRESULT)0);
            }
         break;
/*
      case WM_HELP:
         {
         SHORT idItem = WinQueryWindowUShort(WinQueryFocus(HWND_DESKTOP), QWS_ID);
         idItem = (idItem < IDC_HIDE_NO) ? HELP_ID_OPTIONS : ((idItem > IDC_FULLCAPTURE) ? HELP_ID_OPTIONS : idItem);
//         WinSendMsg (hwndHelp, HM_DISPLAY_HELP, MPFROM2SHORT(idItem, NULL), MPFROMSHORT(HM_RESOURCEID));
         WinSendMsg (hwndHelp, HM_DISPLAY_HELP, MPFROM2SHORT(HELP_ID_OPTIONS, NULL), MPFROMSHORT(HM_RESOURCEID));
         }
         break;
*/
      case WM_CLOSE:
         WinSendMsg (hwnd, WM_COMMAND, MPFROMSHORT (DID_OK), MPFROMP (NULL));
         break;
      }
   return (WinDefDlgProc(hwnd, msg, mp1, mp2));
   }
