/**************************************************************************
 *  File name  : pmsnap.c
 *  (c) Copyright Carrick von Schoultz 1994. All rights reserved.
 *  (c) Copyright Peter Nielsen 1994. All rights reserved.
 *  Description: 
 *************************************************************************/
#define INCL_WINSCROLLBARS
#define INCL_DOSMISC
#define INCL_DOSFILEMGR
#define INCL_GPILOGCOLORTABLE
#define INCL_GPIBITMAPS
#define INCL_WINPALETTE
#define INCL_WINCLIPBOARD
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WINHOOKS
#define INCL_WININPUT
#define INCL_WINMENUS
#define INCL_WINPOINTERS
#define INCL_WINSHELLDATA
#define INCL_WINSWITCHLIST
#define INCL_WINSYS
#define INCL_WINTIMER
#define INCL_WINTRACKRECT
#define INCL_WINWINDOWMGR

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pmsnap.h"
#include "bitmap.h"
#include "dialogs.h"

/* UNDOCUMENTED!!! */
USHORT _Far16 _Pascal DOSMEMAVAIL (PULONG);

MRESULT EXPENTRY MainWndProc (HWND, ULONG, MPARAM, MPARAM);

/*
BOOL EXPENTRY HelpHook (HAB hab, SHORT sMode, SHORT sTopic, SHORT sSubTopic, PRECTL prclPosition) {
   HWND hwndActive;

   if ((sMode==HLPM_WINDOW) && (sSubTopic==1)) {
      hwndActive=WinQueryActiveWindow (HWND_DESKTOP);
      WinSendMsg(WinQueryHelpInstance (hwndActive), HM_DISPLAY_HELP, MPFROMLONG(MAKEULONG(sTopic,0)), MPFROMLONG(HM_RESOURCEID));
      return TRUE;
      }
   else
      return FALSE;
   }
*/

HWND InstallHelp (HAB hab, HWND hwndFrame, HMQ hmqMain) {
   HELPINIT hiHelp;
   HWND     hwndHelp;
   CHAR     szHelpWindowTitle[80], szHelpLibraryName[80];

   WinLoadString (hab, NULLHANDLE, SZ_HELPWINDOWTITLE, sizeof (szHelpWindowTitle), szHelpWindowTitle);
   WinLoadString (hab, NULLHANDLE, SZ_HELPLIBRARYNAME, sizeof (szHelpLibraryName), szHelpLibraryName);
   memset (&hiHelp, 0, sizeof(HELPINIT));
   hiHelp.cb                       = sizeof(HELPINIT);
   hiHelp.ulReturnCode             = 0;
   hiHelp.pszTutorialName          = NULL;
   hiHelp.phtHelpTable             = (PHELPTABLE)MAKELONG(HELP_ID_MAINWND, 0xFFFF);
   hiHelp.hmodHelpTableModule      = NULLHANDLE;
   hiHelp.hmodAccelActionBarModule = NULLHANDLE;
   hiHelp.idAccelTable             = 0;
   hiHelp.idActionBar              = 0;
   hiHelp.pszHelpWindowTitle       = szHelpWindowTitle;
   hiHelp.fShowPanelId             = CMIC_HIDE_PANEL_ID;
   hiHelp.pszHelpLibraryName       = szHelpLibraryName;
   hwndHelp = WinCreateHelpInstance(hab, &hiHelp);
   if (!hwndHelp || hiHelp.ulReturnCode) {
      CHAR szHelpLoadError[256], szTitle[80];

      WinLoadString (hab, NULLHANDLE, SZ_TITLE, sizeof (szTitle), szTitle);
      WinLoadString (hab, NULLHANDLE, SZ_HELPLOADERROR, sizeof (szHelpLoadError), szHelpLoadError);
      WinDestroyHelpInstance(hwndHelp);
//      WinAlarm(HWND_DESKTOP, WA_ERROR);
      WinMessageBox(HWND_DESKTOP, hwndFrame, szHelpLoadError, szTitle, IDD_MESSAGEBOX, MB_MOVEABLE | MB_OK | MB_CUACRITICAL);
      return (NULLHANDLE);
      }
   WinAssociateHelpInstance(hwndHelp, hwndFrame);
//      WinSetHook(hab, hmqMain, HK_HELP, (PFN)HelpHook, NULLHANDLE);
   return (hwndHelp);
   }

VOID main (INT argc, CHAR *argv[]) {
   ULONG flFrameFlags = FCF_STANDARD | FCF_VERTSCROLL | FCF_HORZSCROLL;
   HAB   habMain = WinInitialize (0);
   HMQ   hmqMain = WinCreateMsgQueue (habMain, 0);
   HWND   hwndFrame, hwndClient, hwndHelp;
   QMSG   qmsg;
   CHAR   szTitle[64];
   ULONG ulClr = SYSCLR_FIELDBACKGROUND;

   WinLoadString (habMain, NULLHANDLE, SZ_UNREGISTERED, sizeof (szTitle), szTitle);
   WinRegisterClass (habMain, szTitle, MainWndProc, CS_SIZEREDRAW | CS_SYNCPAINT, sizeof (PVOID) * 2);

   hwndFrame = WinCreateStdWindow (HWND_DESKTOP, 0L, &flFrameFlags, (PSZ)szTitle, (PSZ)szTitle, 0L, NULLHANDLE, ID_MAINWND, &hwndClient);
   WinSetPresParam (hwndFrame, PP_BACKGROUNDCOLOR, sizeof (ulClr), &ulClr);
   hwndHelp = InstallHelp (habMain, hwndFrame, hmqMain);

   while (WinGetMsg (habMain, &qmsg, (HWND)NULL, 0, 0))
      WinDispatchMsg (habMain, &qmsg);

   if ((BOOL)hwndHelp) {
//      WinReleaseHook (habMain, hmqMain, HK_HELP, (PFN)HelpHook, NULLHANDLE);
      WinDestroyHelpInstance (hwndHelp);
      }
   WinDestroyWindow (hwndFrame);
   WinDestroyMsgQueue (hmqMain);
   WinTerminate (habMain);
   }

LOCAL VOID LoadSettings (HWND hwnd, PSETTINGS pSettings) {
   HAB   hab = WinQueryAnchorBlock (hwnd);
   HINI  hini;
   PSZ   pszIniFileName;
   CHAR  szAppName[80], szKeyName[80], szEnviron[80], szVersion[80], szIni[80];
   ULONG ulBufferSize = sizeof (*pSettings);
   BOOL  flAction;

   WinLoadString (hab, NULLHANDLE, SZ_APPNAME, sizeof (szAppName), szAppName);
   WinLoadString (hab, NULLHANDLE, SZ_KEYNAME, sizeof (szKeyName), szKeyName);
   WinLoadString (hab, NULLHANDLE, SZ_ENVIRON_VAR, sizeof (szEnviron), szEnviron);
   WinLoadString (hab, NULLHANDLE, SZ_INI_FILE, sizeof (szIni), szIni);
   WinLoadString (hab, NULLHANDLE, SZ_VERSION, sizeof (szVersion), szVersion);
//   if (DosScanEnv ((PSZ)szEnviron, (const CHAR **)&pszIniFileName))
   if (DosScanEnv ((PSZ)szEnviron, &pszIniFileName))
      pszIniFileName = szIni;
   hini = PrfOpenProfile (hab, pszIniFileName);
   if (!PrfQueryProfileData (hini, szAppName, szKeyName, pSettings, &ulBufferSize)) {
      SETTINGS settingsDefault = DEFAULTSETTINGS;
      *pSettings = settingsDefault;
      strcpy (pSettings->chVersion, szVersion);
      flAction = SWP_SHOW;   // Let PM decide the best position - don't use SWP_MOVE !
                             // You can also remove SWP_SIZE if you want PM do decide the initial size...
      }
   else {
      flAction = SWP_SIZE | SWP_SHOW | SWP_MOVE | pSettings->windowpos.fMinimized;
      }
   WinSetWindowPos (hwnd, 0, pSettings->windowpos.x, pSettings->windowpos.y, pSettings->windowpos.cx, pSettings->windowpos.cy, flAction);
   PrfCloseProfile (hini);
   }

LOCAL VOID SaveSettings (HWND hwnd, PSETTINGS pSettings) {
   HAB   hab = WinQueryAnchorBlock (hwnd);
   HINI  hini;
   PSZ   pszIniFileName;
   CHAR  szAppName[80], szKeyName[80], szEnviron[80], szIni[80];
   SWP   swp;

   WinLoadString (hab, NULLHANDLE, SZ_APPNAME, sizeof (szAppName), szAppName);
   WinLoadString (hab, NULLHANDLE, SZ_KEYNAME, sizeof (szKeyName), szKeyName);
   WinLoadString (hab, NULLHANDLE, SZ_ENVIRON_VAR, sizeof (szEnviron), szEnviron);
   WinLoadString (hab, NULLHANDLE, SZ_INI_FILE, sizeof (szIni), szIni);
   WinQueryWindowPos (hwnd, &swp);
   if ((swp.fl & SWP_MINIMIZE) || (swp.fl & SWP_MAXIMIZE)) {
   pSettings->windowpos.x  = WinQueryWindowUShort(hwnd, QWS_XRESTORE);
   pSettings->windowpos.y  = WinQueryWindowUShort(hwnd, QWS_YRESTORE);
   pSettings->windowpos.cx = WinQueryWindowUShort(hwnd, QWS_CXRESTORE);
   pSettings->windowpos.cy = WinQueryWindowUShort(hwnd, QWS_CYRESTORE);
   }else{
   pSettings->windowpos.x  = swp.x;
   pSettings->windowpos.y  = swp.y;
   pSettings->windowpos.cx = swp.cx;
   pSettings->windowpos.cy = swp.cy;
   }
   pSettings->windowpos.fMinimized   = swp.fl & SWP_MINIMIZE;
//   if (DosScanEnv ((PSZ)szEnviron, (const CHAR **)&pszIniFileName)) 
   if (DosScanEnv ((PSZ)szEnviron, &pszIniFileName))
      pszIniFileName = szIni;
   hini = PrfOpenProfile (hab, pszIniFileName);
   PrfWriteProfileData (hini, szAppName, szKeyName, pSettings, sizeof (*pSettings));
   PrfCloseProfile (hini);
   }

LOCAL BOOL QueryMem (HAB hab, HWND hwnd, PRECTL prclTrack) {
   BOOL  fQuery = TRUE;
   HPS   hpsDesktop = WinGetPS (HWND_DESKTOP);
   HWND  hwndCapture = WinQueryFocus (HWND_DESKTOP);
   ULONG ulBitCount, ulBitmapSize, ulFreeMem, ulFreePhysMem;
   ULONG ulCx = prclTrack->xRight-prclTrack->xLeft;
   ULONG ulCy = prclTrack->yTop-prclTrack->yBottom;

   DevQueryCaps (GpiQueryDevice (hpsDesktop), CAPS_COLOR_BITCOUNT, 1L, (PLONG)&ulBitCount);
   if (ulBitCount > 8)
      ulBitCount = 24;
   WinReleasePS (hpsDesktop);
   ulBitmapSize = ((((ulCx * ulBitCount + 31) / 8) & ~3) * ulCy);
   DOSMEMAVAIL (&ulFreePhysMem);
   DosQuerySysInfo (QSV_TOTAVAILMEM, QSV_TOTAVAILMEM, &ulFreeMem, sizeof (ulFreeMem));

   if (ulBitmapSize > ulFreeMem){
      CHAR  szTitle[80], szBitmap2Big[100];
      fQuery = FALSE;
      WinLoadString (hab, NULLHANDLE, SZ_TITLE, sizeof (szTitle), szTitle);
      WinLoadString (hab, NULLHANDLE, SZ_BITMAP2BIG, sizeof (szBitmap2Big), szBitmap2Big);
      WinMessageBox (HWND_DESKTOP, hwnd, szBitmap2Big, szTitle, IDD_MESSAGEBOX, MB_MOVEABLE | MB_OK | MB_CUACRITICAL);
   }
   else if (ulBitmapSize > (ulFreePhysMem)){
      CHAR  szTitle[80], szQuestion[300], szFreeMemory[50], szContinue[50], szTemp[20];
      WinLoadString (hab, NULLHANDLE, SZ_TITLE, sizeof (szTitle), szTitle);
      WinLoadString (hab, NULLHANDLE, SZ_PICTURESIZE, sizeof (szQuestion), szQuestion);
      sprintf (szTemp, " %8.3f MB ", (float)ulBitmapSize / 1000000);
      strcat (szQuestion, szTemp);
      WinLoadString (hab, NULLHANDLE, SZ_OFMEMORY, sizeof (szTemp), szTemp);
      strcat (szQuestion, szTemp);
      sprintf (szTemp, "!\n");
      strcat (szQuestion, szTemp);
      WinLoadString (hab, NULLHANDLE, SZ_FREEPHYSMEMORY, sizeof (szFreeMemory), szFreeMemory);
      strcat (szQuestion, szFreeMemory);
      sprintf (szTemp, " %8.3f MB.\n", (float)ulFreePhysMem / 1000000);
      strcat (szQuestion, szTemp);
      WinLoadString (hab, NULLHANDLE, SZ_FREEMEMORY, sizeof (szFreeMemory), szFreeMemory);
      strcat (szQuestion, szFreeMemory);
      sprintf (szTemp, " %8.3f MB.\n", (float)ulFreeMem / 1000000);
      strcat (szQuestion, szTemp);
      WinLoadString (hab, NULLHANDLE, SZ_CONTINUE, sizeof (szContinue), szContinue);
      strcat (szQuestion, szContinue);
      fQuery = ((BOOL)(WinMessageBox (HWND_DESKTOP, hwnd, szQuestion, szTitle, IDD_MESSAGEBOX, MB_MOVEABLE | MB_YESNO | MB_CUAWARNING | MB_DEFBUTTON1) == MBID_YES));
      WinSetFocus (HWND_DESKTOP, hwndCapture);
      }
   return fQuery;
   }
LOCAL VOID RemoveBitmapFromMem(HAB hab, HWND hwnd, PHDC phdc, PHPS phps, PHBITMAP phbm, PHPAL phpal) {
   SIZEL sizl = { 0, 0 };
   if (*phbm) 
      GpiSetBitmap (*phps, NULLHANDLE);
      GpiDeleteBitmap (*phbm);
      *phbm = NULLHANDLE;
   if (*phpal) {
      GpiSelectPalette (*phps, NULLHANDLE);
      GpiDeletePalette (*phpal);
      }
   GpiAssociate (*phps, NULLHANDLE);
   GpiDestroyPS (*phps);
   *phps = GpiCreatePS (hab, *phdc, &sizl, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
   }

LOCAL VOID MenuOnCapture (HWND hwndMenu, BOOL fCapture) {
   WinEnableMenuItem (hwndMenu, IDM_CANCELCAPTURE, fCapture);
   WinEnableMenuItem (hwndMenu, IDM_DESKTOP, !fCapture);
   WinEnableMenuItem (hwndMenu, IDM_ACTIVE, !fCapture);
   WinEnableMenuItem (hwndMenu, IDM_ACTIVECLIENT, !fCapture);
   WinEnableMenuItem (hwndMenu, IDM_REGION, !fCapture);
   }

LOCAL VOID ShowWindow (HWND hwndFrame, BOOL fShow) {
   SWP swp;
   WinQueryWindowPos (hwndFrame, &swp);
   if (swp.fl & SWP_MINIMIZE) {
      CHAR szClassBuffer[80];
      HWND hwndIconText = WinQueryWindow(hwndFrame, QW_NEXT);
      WinQueryClassName (hwndIconText, sizeof (szClassBuffer), szClassBuffer);
      if (!strcmp (szClassBuffer, "#32765"))
         WinShowWindow(hwndIconText, fShow);
      }
   WinShowWindow(hwndFrame, fShow);
   }

LOCAL VOID SetTitle (HWND hwndFrame, CHAR szTitle[64], LONG cxBitmap, LONG cyBitmap) {
   CHAR szNewTitle[64], szTemp[15];
   if (cxBitmap == 0 || cyBitmap == 0)
      WinLoadString (WinQueryAnchorBlock (hwndFrame), NULLHANDLE, SZ_UNREGISTERED, sizeof (szNewTitle), szNewTitle);
   else {
      sprintf (szTemp, " (%dx%d)", cxBitmap, cyBitmap);
      strcpy (szNewTitle, szTitle);
      strcat (szNewTitle, szTemp);
      }
   WinSetWindowText (hwndFrame, szNewTitle);
   }

typedef struct {
   HAB               hab;
   HDC	       hdc;
   HPS               hps;
   HBITMAP           hbm, hbmMask;
   HPOINTER          hptrMagnify, hptrWindow;
   HWND              hwndMenu, hwndFrame;
   HPAL              hpal;
   PBYTE             pbData;
   RECTL             rclTrack;
   SHORT             mpWindow;
   USHORT            usTime;
   BOOL              fCapture, fCaptureWindow, fMouseCapture, fAreaCapHide;
   SHORT             sHSBPos, sVSBPos;
   LONG              cxBitmap, cyBitmap;
   HWND              hwndVSB, hwndHSB;
   CHAR              szTitle[64];
   POINTL            ptlStart, ptlEnd, ptlMagnify, ptlScreen, ptlCrossHair;
   PSETTINGS         pSettings;
   } MAINWINDOWINSTANCEDATA, *PMAINWINDOWINSTANCEDATA;

MRESULT EXPENTRY MainWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PMAINWINDOWINSTANCEDATA pInstanceData = WinQueryWindowPtr (hwnd, QWL_USER);
   PSETTINGS pSettings = WinQueryWindowPtr (hwnd, QWL_USER + sizeof (PVOID));

   switch (msg) {
      case WM_CREATE: {
         SIZEL sizl = { 0, 0 };
         ULONG ulAdditionalGraphics;

         pInstanceData = (PMAINWINDOWINSTANCEDATA)calloc (sizeof (MAINWINDOWINSTANCEDATA), 1);
         WinSetWindowPtr (hwnd, QWL_USER, pInstanceData);
         pInstanceData->hab          = WinQueryAnchorBlock (hwnd);
         pInstanceData->hwndFrame    = WinQueryWindow (hwnd, QW_PARENT);
         pInstanceData->hwndMenu     = WinWindowFromID (pInstanceData->hwndFrame, FID_MENU);
         pInstanceData->hptrMagnify  = NULLHANDLE;
         pInstanceData->hptrWindow   = WinLoadPointer (HWND_DESKTOP, NULLHANDLE, IDP_WINDOW);
         pSettings = (PSETTINGS)calloc (sizeof (SETTINGS), 1);
         WinSetWindowPtr (hwnd, QWL_USER + sizeof (PVOID), pSettings);
         WinLoadString (pInstanceData->hab, (HMODULE)NULL, SZ_TITLE, sizeof (pInstanceData->szTitle), pInstanceData->szTitle);
         if ((pInstanceData->hdc = WinQueryWindowDC (hwnd)) == NULLHANDLE)
            pInstanceData->hdc = WinOpenWindowDC (hwnd);
         pInstanceData->hps = GpiCreatePS (pInstanceData->hab, pInstanceData->hdc, &sizl, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
         DevQueryCaps (pInstanceData->hdc, CAPS_ADDITIONAL_GRAPHICS, 1L, (PLONG)&ulAdditionalGraphics);
         flPaletteManagerAvailable = (ulAdditionalGraphics & CAPS_PALETTE_MANAGER) ? TRUE : FALSE;

         WinSetParent (pInstanceData->hwndVSB = WinWindowFromID (pInstanceData->hwndFrame, FID_VERTSCROLL), HWND_OBJECT, FALSE);
         WinSetParent (pInstanceData->hwndHSB = WinWindowFromID (pInstanceData->hwndFrame, FID_HORZSCROLL), HWND_OBJECT, FALSE);
         WinSendMsg (pInstanceData->hwndFrame, WM_UPDATEFRAME, (MPARAM)(FID_HORZSCROLL | FID_VERTSCROLL), NULL);

         LoadSettings (pInstanceData->hwndFrame, pSettings);
         pInstanceData->ptlScreen.x = WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN);
         pInstanceData->ptlScreen.y = WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN);
         pInstanceData->fAreaCapHide = FALSE;
         WinEnableMenuItem (pInstanceData->hwndMenu, IDM_PALETTEOPTIONS, flPaletteManagerAvailable);
         WinEnableMenuItem (pInstanceData->hwndMenu, IDM_CANCELCAPTURE, FALSE);
         WinSendMsg (pInstanceData->hwndMenu, MM_SETITEMATTR, MPFROM2SHORT (IDM_STRETCH, TRUE), MPFROM2SHORT (MIA_CHECKED, MPFROMSHORT (pSettings->fFitWindow ? MIA_CHECKED : 0)));
         if (!pSettings->fFitWindow) {
            WinSetParent (pInstanceData->hwndVSB, pInstanceData->hwndFrame, FALSE);
            WinSetParent (pInstanceData->hwndHSB, pInstanceData->hwndFrame, FALSE);
            WinSendMsg (pInstanceData->hwndFrame, WM_UPDATEFRAME, (MPARAM)(FID_HORZSCROLL | FID_VERTSCROLL), NULL);
            }

         }
         return ((MRESULT)0);

      case WM_INITMENU:
         switch (SHORT1FROMMP (mp1)) {
            HWND hwndHelp = WinQueryHelpInstance (hwnd);
            WinSendMsg (hwndHelp, HM_SET_ACTIVE_WINDOW, MPFROMHWND(pInstanceData->hwndFrame), MPFROMHWND(pInstanceData->hwndFrame));

            case IDM_FILE:
               WinSendMsg (pInstanceData->hwndMenu, MM_SETITEMATTR, MPFROM2SHORT (IDM_NEW, TRUE), MPFROM2SHORT (MIA_DISABLED, (pInstanceData->hbm != NULLHANDLE) ? 0 : MIA_DISABLED));
               WinSendMsg (pInstanceData->hwndMenu, MM_SETITEMATTR, MPFROM2SHORT (IDM_SAVEAS, TRUE), MPFROM2SHORT (MIA_DISABLED, (pInstanceData->hbm != NULLHANDLE) ? 0 : MIA_DISABLED));
               return ((MRESULT)0);

            case IDM_EDIT: {
               ULONG ulDummy;
               WinSendMsg (pInstanceData->hwndMenu, MM_SETITEMATTR, MPFROM2SHORT (IDM_COPY, TRUE), MPFROM2SHORT (MIA_DISABLED, MPFROMLONG ((pInstanceData->hbm != NULLHANDLE) ? 0 : MIA_DISABLED)));
               WinSendMsg (pInstanceData->hwndMenu, MM_SETITEMATTR, MPFROM2SHORT (IDM_PASTE, TRUE), MPFROM2SHORT (MIA_DISABLED, MPFROMLONG (WinQueryClipbrdFmtInfo (pInstanceData->hab, CF_BITMAP, &ulDummy) ? 0 : MIA_DISABLED)));
               }
               return ((MRESULT)0);

            case IDM_DISPLAY:
               WinSendMsg (pInstanceData->hwndMenu, MM_SETITEMATTR, MPFROM2SHORT (IDM_FITWINDOW, TRUE), MPFROM2SHORT (MIA_DISABLED, (pInstanceData->hbm != NULLHANDLE) ? 0 : MIA_DISABLED));
               return ((MRESULT)0);

            case IDM_HELP:
               WinEnableMenuItem (pInstanceData->hwndMenu, IDM_HELPINDEX, (BOOL)hwndHelp);
               WinEnableMenuItem (pInstanceData->hwndMenu, IDM_HELPGENERAL, (BOOL)hwndHelp);
               WinEnableMenuItem (pInstanceData->hwndMenu, IDM_HELPUSINGHELP, (BOOL)hwndHelp);
               WinEnableMenuItem (pInstanceData->hwndMenu, IDM_HELPKEYS, (BOOL)hwndHelp);
               return ((MRESULT)0);
            }
         break;

      case WM_COMMAND:
         switch (SHORT1FROMMP(mp1)) {

            case IDM_HELPINDEX:
               WinSendMsg(WinQueryHelpInstance (hwnd), HM_HELP_INDEX, (MPARAM)NULL, (MPARAM)NULL);
               return ((MRESULT)0);

            case IDM_HELPGENERAL:
               WinSendMsg(WinQueryHelpInstance (hwnd), HM_EXT_HELP, (MPARAM)NULL, (MPARAM)NULL);
               return ((MRESULT)0);

            case IDM_HELPUSINGHELP:
               WinSendMsg(WinQueryHelpInstance (hwnd), HM_DISPLAY_HELP, (MPARAM)NULL, (MPARAM)NULL);
               return ((MRESULT)0);

            case IDM_HELPKEYS:
               WinSendMsg(WinQueryHelpInstance (hwnd), HM_KEYS_HELP, (MPARAM)NULL, (MPARAM)NULL);
               return ((MRESULT)0);

            case IDM_NEW: {
               CHAR szQuestion[200];
               WinLoadString (pInstanceData->hab, (HMODULE)NULL, SZ_NEWQ, sizeof (szQuestion), szQuestion);
               if (Query (hwnd, szQuestion)) {
                  CHAR szTitle[50];
                  RemoveBitmapFromMem(pInstanceData->hab, hwnd, &pInstanceData->hdc, &pInstanceData->hps, &pInstanceData->hbm, &pInstanceData->hpal);
                  pInstanceData->cxBitmap = 1;
                  pInstanceData->cyBitmap = 1;
//                  WinSendMsg (pInstanceData->hwndFrame, WM_UPDATEFRAME, (MPARAM)(FID_HORZSCROLL | FID_VERTSCROLL), NULL);
                  SetScrollBars (hwnd, pInstanceData->hwndVSB, pInstanceData->hwndHSB, pInstanceData->cxBitmap, pInstanceData->cyBitmap, &pInstanceData->sHSBPos, &pInstanceData->sVSBPos);
                  WinLoadString (pInstanceData->hab, (HMODULE)NULL, SZ_UNREGISTERED, sizeof (szTitle), szTitle);
                  WinSetWindowText(pInstanceData->hwndFrame, szTitle);
                  WinInvalidateRect(hwnd, (PRECTL)NULL, FALSE);
                  }
               }
               return ((MRESULT)0);

            case IDM_SAVEAS:
               SaveAsFile (pInstanceData->hwndFrame, pInstanceData->hbm, pInstanceData->hpal, FALSE);
               return ((MRESULT)0);

            case IDM_EXIT:
               WinPostMsg (pInstanceData->hwndFrame, WM_CLOSE, MPVOID, MPVOID );
               return ((MRESULT)0);

            case IDM_COPY:
               CopyBitmapToClipboard (hwnd, pInstanceData->hbm, pInstanceData->hpal, pSettings->usCopyPalette);
               return ((MRESULT)0);

            case IDM_PASTE:
               if (pInstanceData->hbm)
                  GpiDeleteBitmap (pInstanceData->hbm);
               if (pInstanceData->hpal) {
                  GpiSelectPalette (pInstanceData->hps, NULLHANDLE);
                  GpiDeletePalette (pInstanceData->hpal);
                  }
               PasteClipboardToBitmap (hwnd, &pInstanceData->hbm, &pInstanceData->hpal, pSettings->usCopyPalette);
               GpiSelectPalette (pInstanceData->hps, pInstanceData->hpal);
               QueryBitmapSize (pInstanceData->hbm, &pInstanceData->cxBitmap, &pInstanceData->cyBitmap);
               SetScrollBars (hwnd, pInstanceData->hwndVSB, pInstanceData->hwndHSB, pInstanceData->cxBitmap, pInstanceData->cyBitmap, &pInstanceData->sHSBPos, &pInstanceData->sVSBPos);
               if (pInstanceData->hbm == NULLHANDLE) {
                  BitmapCreationError (hwnd);
                  SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, 0, 0);
                  }
               else
                  SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, pInstanceData->cxBitmap, pInstanceData->cyBitmap);
               WinInvalidateRect (hwnd, NULL, FALSE);
               return ((MRESULT)0);

            case IDM_DESKTOP:
               if (!(pInstanceData->fCapture || pInstanceData->fMouseCapture || pInstanceData->fCaptureWindow)) {
                  WinSetFocus (HWND_DESKTOP, WinQueryDesktopWindow(pInstanceData->hab, NULLHANDLE));
                  pInstanceData->rclTrack.yBottom = 0;
                  pInstanceData->rclTrack.xLeft   = 0;
                  pInstanceData->rclTrack.xRight  = pInstanceData->ptlScreen.x;
                  pInstanceData->rclTrack.yTop    = pInstanceData->ptlScreen.y;
                  if (pSettings->usDelay) {
                     pInstanceData->usTime = pSettings->usDelay;
                     MenuOnCapture (pInstanceData->hwndMenu, TRUE);
                     WinStartTimer (pInstanceData->hab, hwnd, ID_TIMER, 1000);
                     if (pSettings->usHide == HIDE_INSTANTLY)
                        ShowWindow (pInstanceData->hwndFrame, FALSE);
                     }
                  else if (pSettings->usHide != HIDE_NOHIDE) {
                     pInstanceData->usTime = 1;
                     WinStartTimer (pInstanceData->hab, hwnd, ID_TIMER, 250);
                     ShowWindow (pInstanceData->hwndFrame, FALSE);
                     }
                  else {
                     WinPostMsg (hwnd, UM_NEWBITMAP, 0, 0);
                     }
                  }
               return ((MRESULT)0);

            case IDM_ACTIVE:
            case IDM_ACTIVECLIENT:
               if (!(pInstanceData->fCapture || pInstanceData->fMouseCapture || pInstanceData->fCaptureWindow)) {
                  POINTL ptl;
                  pInstanceData->mpWindow = SHORT1FROMMP (mp1);
                  pInstanceData->fCaptureWindow = TRUE;
                  WinSetCapture (HWND_DESKTOP, hwnd);
                  WinQueryPointerPos (HWND_DESKTOP, &ptl);
                  WinSetPointerPos (HWND_DESKTOP, ptl.x++, ptl.y++);
                  if (pSettings->usHide == HIDE_INSTANTLY)
                     ShowWindow (pInstanceData->hwndFrame, FALSE);
                  }
               return ((MRESULT)0);

            case IDM_REGION:
               if (!(pInstanceData->fCapture || pInstanceData->fMouseCapture || pInstanceData->fCaptureWindow)) {
                  ULONG ulDivisor;
                  switch (pSettings->usMagnify) {
                     case MAGNIFY_1_5:
                        ulDivisor = 3;
                        break;
                     default:
                     case MAGNIFY_2_0:
                        ulDivisor = 4;
                        break;
                     case MAGNIFY_2_5:
                        ulDivisor = 5;
                        break;
                     }
                  pInstanceData->ptlMagnify.x = WinQuerySysValue (HWND_DESKTOP, SV_CXICON) / ulDivisor;
                  pInstanceData->ptlMagnify.y = WinQuerySysValue (HWND_DESKTOP, SV_CYICON) / ulDivisor;
                  if (pSettings->usHide == HIDE_INSTANTLY) {
                     pInstanceData->fAreaCapHide = TRUE;
                     pInstanceData->usTime = 1;
                     WinStartTimer (pInstanceData->hab, hwnd, ID_TIMER, 250);
                     ShowWindow (pInstanceData->hwndFrame, FALSE);
                     }
                  else {
                     WinSetCapture (HWND_DESKTOP, hwnd);
                     pInstanceData->fMouseCapture = TRUE;
                     WinQueryPointerPos(HWND_DESKTOP, &pInstanceData->ptlEnd);
                     DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, FALSE);
                     WinSetPointerPos(HWND_DESKTOP, pInstanceData->ptlEnd.x+1, pInstanceData->ptlEnd.y+1);
                     }
                  }
               return ((MRESULT)0);

            case IDM_STRETCH:
               pSettings->fFitWindow = !pSettings->fFitWindow;
               WinSendMsg (pInstanceData->hwndMenu, MM_SETITEMATTR, MPFROM2SHORT (IDM_STRETCH, TRUE), MPFROM2SHORT (MIA_CHECKED, MPFROMSHORT (pSettings->fFitWindow ? MIA_CHECKED : 0)));
               if (pSettings->fFitWindow) {
                  WinSetParent (pInstanceData->hwndVSB, HWND_OBJECT, TRUE);
                  WinSetParent (pInstanceData->hwndHSB, HWND_OBJECT, TRUE);
                  }
               else {
                  WinSetParent (pInstanceData->hwndVSB, pInstanceData->hwndFrame, TRUE);
                  WinSetParent (pInstanceData->hwndHSB, pInstanceData->hwndFrame, TRUE);
                  }
                 WinSendMsg (pInstanceData->hwndFrame, WM_UPDATEFRAME, (MPARAM)(FID_HORZSCROLL | FID_VERTSCROLL), NULL);
               return ((MRESULT)0);

            case IDM_FITWINDOW:
               if (pInstanceData->hbm) {
                  RECTL rclRect;
                  SWP    swp;
                  LONG   lYTop;
                  WinQueryWindowPos (pInstanceData->hwndFrame, &swp);
                  lYTop = swp.y + swp.cy;
                  rclRect.xLeft   = 0;
                  rclRect.xRight  = pInstanceData->cxBitmap;
                  rclRect.yBottom = 0;
                  rclRect.yTop    = pInstanceData->cyBitmap;
                  WinCalcFrameRect(pInstanceData->hwndFrame, &rclRect, FALSE);
                  WinSetWindowPos (pInstanceData->hwndFrame, 0, swp.x, lYTop - (rclRect.yTop - rclRect.yBottom), rclRect.xRight - rclRect.xLeft, rclRect.yTop - rclRect.yBottom, SWP_SIZE | SWP_MOVE);
               }
               return ((MRESULT)0);

            case IDM_MAGNIFY:
               if (pInstanceData->fCapture || pInstanceData->fMouseCapture) {
                  if (pSettings->fMagnify) {
                     WinSetPointer (HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE));
                     if (WinQuerySysValue (HWND_DESKTOP, SV_POINTERLEVEL) == 0)
                        WinShowPointer (HWND_DESKTOP, FALSE);
                     }
                  else {
                     if (WinQuerySysValue (HWND_DESKTOP, SV_POINTERLEVEL) > 0)
                        WinShowPointer (HWND_DESKTOP, TRUE);
                     MagnifyPointer(pInstanceData->hab, &pInstanceData->ptlEnd, &pInstanceData->hptrMagnify, &pInstanceData->ptlMagnify);
                     }
                  pSettings->fMagnify = !pSettings->fMagnify;
                  }
               return ((MRESULT)0);

            case IDM_OPTIONS:
               WinDlgBox (HWND_DESKTOP, hwnd, (PFNWP)OptionsDlgProc, 0, ID_OPTIONS, (PVOID)pSettings);
               return ((MRESULT)0);

            case IDM_PALETTEOPTIONS:
               WinDlgBox (HWND_DESKTOP, hwnd, (PFNWP)PaletteOptionsDlgProc, 0, ID_PALETTEOPTIONS, (PVOID)pSettings);
               return ((MRESULT)0);

            case IDM_CANCELCAPTURE:
               WinStopTimer (pInstanceData->hab, hwnd, ID_TIMER);
               MenuOnCapture (pInstanceData->hwndMenu, FALSE);
               SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, pInstanceData->cxBitmap, pInstanceData->cyBitmap);
               return ((MRESULT)0);

            case IDM_PRODUCTINFO:
               HelpProductInfo (hwnd);
               return ((MRESULT)0);
            }
         break;

         case WM_TIMER:
            if (SHORT1FROMMP(mp1) == ID_TIMER) {
               if (pInstanceData->fAreaCapHide) {
                  WinStopTimer (pInstanceData->hab, hwnd, ID_TIMER);
                  pInstanceData->fAreaCapHide = FALSE;
                  WinSetCapture (HWND_DESKTOP, hwnd);
                  pInstanceData->fMouseCapture = TRUE;
                  WinQueryPointerPos(HWND_DESKTOP, &pInstanceData->ptlEnd);
                  DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, FALSE);
                  WinSetPointerPos(HWND_DESKTOP, pInstanceData->ptlEnd.x+1, pInstanceData->ptlEnd.y+1);
                  }
               else {
                  CHAR szTitle[20];
                  if (--(pInstanceData->usTime) == 0) {
                     WinStopTimer (pInstanceData->hab, hwnd, ID_TIMER);
                     MenuOnCapture (pInstanceData->hwndMenu, FALSE);
                     WinSetWindowText (pInstanceData->hwndFrame, pInstanceData->szTitle);
                     WinPostMsg(hwnd, UM_NEWBITMAP, 0, 0);
                     }
                  else if ((pInstanceData->usTime == 1) && (pSettings->usHide == HIDE_CAPTUREONLY))
                     ShowWindow (pInstanceData->hwndFrame, FALSE);
                  else{
                     sprintf (szTitle, "%d", pInstanceData->usTime);
                     WinSetWindowText (pInstanceData->hwndFrame, szTitle);
                     }
                  }
               }
            break;

         case WM_REALIZEPALETTE: {
            ULONG ulTemp;
            if (WinRealizePalette (pSettings->fAgrPalette ? hwnd : HWND_DESKTOP, pInstanceData->hps, &ulTemp))
               WinInvalidateRect (hwnd, NULL, FALSE);
            return ((MRESULT)FALSE);
            }
            break;

         case UM_NEWBITMAP: {
            ULONG ulTemp;
            if (QueryMem (pInstanceData->hab, hwnd, &pInstanceData->rclTrack)) {
               if (pInstanceData->hbm)
                  GpiDeleteBitmap (pInstanceData->hbm);
               if (pInstanceData->hpal) {
                  GpiSelectPalette (pInstanceData->hps, NULLHANDLE);
                  GpiDeletePalette (pInstanceData->hpal);
                  }
               WinSetCapture (HWND_DESKTOP, hwnd);
               WinEnablePhysInput (HWND_DESKTOP, FALSE);
               WinSetPointer (HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE));
               CopyScreenToBitmap (pInstanceData->hwndFrame, &pInstanceData->rclTrack, &pInstanceData->hbm, &pInstanceData->hpal, pSettings->fFullCapture);
               WinSetPointer (HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE));
               WinEnablePhysInput (HWND_DESKTOP, TRUE);
               WinSetCapture (HWND_DESKTOP, NULLHANDLE);
               GpiSelectPalette (pInstanceData->hps, pInstanceData->hpal);
               WinSetFocus (HWND_DESKTOP, hwnd); 
               if (pSettings->fBeep)
                  WinAlarm (HWND_DESKTOP, WA_NOTE);
               if (pInstanceData->hbm == NULLHANDLE) {
                  BitmapCreationError (hwnd);
                  SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, 0, 0);
                  pInstanceData->cxBitmap = 1;
                  pInstanceData->cyBitmap = 1;
                  }
               else {
                  QueryBitmapSize (pInstanceData->hbm, &pInstanceData->cxBitmap, &pInstanceData->cyBitmap);
                  SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, pInstanceData->cxBitmap, pInstanceData->cyBitmap);
                  }
               if (!pSettings->fFitWindow)
                  SetScrollBars (hwnd, pInstanceData->hwndVSB, pInstanceData->hwndHSB, pInstanceData->cxBitmap, pInstanceData->cyBitmap, &pInstanceData->sHSBPos, &pInstanceData->sVSBPos);
               WinRealizePalette (pSettings->fAgrPalette ? hwnd : HWND_DESKTOP, pInstanceData->hps, &ulTemp);
               WinInvalidateRect (hwnd, NULL, FALSE);
               if (pSettings->usHide != HIDE_NOHIDE)
                  ShowWindow (pInstanceData->hwndFrame, TRUE);
               }
            else {
               if (pSettings->usHide != HIDE_NOHIDE)
                  ShowWindow (pInstanceData->hwndFrame, TRUE);
               SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, pInstanceData->cxBitmap, pInstanceData->cyBitmap);
               }
            }
            return ((MRESULT)0);

         case WM_BUTTON1CLICK:
         case WM_BUTTON2CLICK:
            if (pInstanceData->fCaptureWindow) {
               POINTL ptl;
               SWP     swp;
               WinSetCapture (HWND_DESKTOP, NULLHANDLE);
               pInstanceData->fCaptureWindow = FALSE;
               WinQueryPointerPos (HWND_DESKTOP, &ptl);
               WinSetFocus (HWND_DESKTOP, WinWindowFromPoint (HWND_DESKTOP, &ptl, FALSE)); 
               WinQueryWindowPos (WinQueryActiveWindow (HWND_DESKTOP), &swp);
               if (pInstanceData->mpWindow == IDM_ACTIVE) {
                  pInstanceData->rclTrack.yBottom = swp.y;
                  pInstanceData->rclTrack.xLeft   = swp.x;
                  pInstanceData->rclTrack.xRight  = swp.x + swp.cx;
                  pInstanceData->rclTrack.yTop    = swp.y + swp.cy;
                    }
               else {
                  ULONG ulBorder;
                  LONG  lXBorder=0, lYBorder=0;
                  memset (&pInstanceData->rclTrack, 0, sizeof(RECTL));
                  ulBorder = WinQueryWindowULong(WinQueryActiveWindow (HWND_DESKTOP), QWL_STYLE);
                  if (swp.fl & SWP_MINIMIZE) {
                     lYBorder = WinQuerySysValue (HWND_DESKTOP, SV_CYSIZEBORDER);
                     lXBorder = WinQuerySysValue (HWND_DESKTOP, SV_CXSIZEBORDER);
                     pInstanceData->rclTrack.yBottom = swp.y + lYBorder;
                     pInstanceData->rclTrack.xLeft   = swp.x + lXBorder;
                     pInstanceData->rclTrack.xRight  = swp.x + swp.cx - lXBorder;
                     pInstanceData->rclTrack.yTop    = swp.y + swp.cy - lYBorder;
                     }
                  else {
                     WinQueryWindowRect (WinWindowFromID (WinQueryActiveWindow (HWND_DESKTOP), FID_CLIENT), &pInstanceData->rclTrack);
                     if ((pInstanceData->rclTrack.yTop-pInstanceData->rclTrack.yBottom > 0) && (pInstanceData->rclTrack.xRight-pInstanceData->rclTrack.xLeft > 0)) {
                        if (ulBorder & FS_SIZEBORDER) {
                           lYBorder = WinQuerySysValue (HWND_DESKTOP, SV_CYSIZEBORDER);
                           lXBorder = WinQuerySysValue (HWND_DESKTOP, SV_CXSIZEBORDER);
                           }
                        else if (ulBorder & FS_BORDER) {
                           lYBorder = WinQuerySysValue (HWND_DESKTOP, SV_CYBORDER);
                           lXBorder = WinQuerySysValue (HWND_DESKTOP, SV_CXBORDER);
                           }
                        else if (ulBorder & FS_DLGBORDER) {
                           lYBorder = WinQuerySysValue (HWND_DESKTOP, SV_CYDLGFRAME);
                           lXBorder = WinQuerySysValue (HWND_DESKTOP, SV_CXDLGFRAME);
                           }
                        pInstanceData->rclTrack.yBottom += swp.y + lYBorder;
                        pInstanceData->rclTrack.xLeft   += swp.x + lXBorder;
                        pInstanceData->rclTrack.xRight  += swp.x + lXBorder;
                        pInstanceData->rclTrack.yTop    += swp.y + lYBorder;
                        }
                     else {
                        pInstanceData->rclTrack.yBottom = swp.y;
                        pInstanceData->rclTrack.xLeft   = swp.x;
                        pInstanceData->rclTrack.xRight  = swp.x + swp.cx;
                        pInstanceData->rclTrack.yTop    = swp.y + swp.cy;
                        }
                     }
                  }
               if ((pInstanceData->rclTrack.xRight - pInstanceData->rclTrack.xLeft <= 1) || (pInstanceData->rclTrack.yTop - pInstanceData->rclTrack.yBottom <= 1)) {
                  CHAR szNotAcceptableWindow[200];
                  CHAR szTitle[80];
                  WinLoadString (WinQueryAnchorBlock (hwnd), NULLHANDLE, SZ_TITLE, sizeof (szTitle), szTitle);
                  WinLoadString (WinQueryAnchorBlock (hwnd), NULLHANDLE, SZ_NOTACCEPTABLEW, sizeof (szNotAcceptableWindow), szNotAcceptableWindow);
//                  WinAlarm(HWND_DESKTOP, WA_ERROR);
                  WinMessageBox (HWND_DESKTOP, hwnd, szNotAcceptableWindow, szTitle, IDD_MESSAGEBOX, MB_MOVEABLE | MB_OK | MB_CUACRITICAL);
                  SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, pInstanceData->cxBitmap, pInstanceData->cyBitmap);
                  if (pSettings->usHide != HIDE_NOHIDE)
                     ShowWindow (pInstanceData->hwndFrame, TRUE);
                  break;
                  }
               if (pSettings->usDelay > 0) {
                  pInstanceData->usTime = pSettings->usDelay;
                  WinStartTimer (pInstanceData->hab, hwnd, ID_TIMER, 1000);
                  MenuOnCapture (pInstanceData->hwndMenu, TRUE);
                  }
               else if (pSettings->usHide == HIDE_CAPTUREONLY) {
                  pInstanceData->usTime = 1;
                  WinStartTimer (pInstanceData->hab, hwnd, ID_TIMER, 250);
                  ShowWindow (pInstanceData->hwndFrame, FALSE);
                  }
               else {
                  WinPostMsg (hwnd, UM_NEWBITMAP, 0, 0);
                  }
               }
            break;

      case WM_BUTTON1DOWN:
         if (pInstanceData->fMouseCapture) {
            pInstanceData->fMouseCapture = FALSE;
            pInstanceData->fCapture = TRUE;
            DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, FALSE);
            WinQueryPointerPos (HWND_DESKTOP, &pInstanceData->ptlEnd);
            pInstanceData->ptlStart.x = pInstanceData->ptlEnd.x;
            pInstanceData->ptlStart.y = pInstanceData->ptlEnd.y;
            DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, TRUE);
            }
         break;

      case WM_MOUSEMOVE:
         if (pInstanceData->fMouseCapture) {
            DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, FALSE);
            WinQueryPointerPos (HWND_DESKTOP, &pInstanceData->ptlEnd);
            DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, FALSE);
            if (pSettings->fMagnify)
               MagnifyPointer (pInstanceData->hab, &pInstanceData->ptlEnd, &pInstanceData->hptrMagnify, &pInstanceData->ptlMagnify);
            else {
               if (WinQuerySysValue (HWND_DESKTOP, SV_POINTERLEVEL) == 0)
                  WinShowPointer (HWND_DESKTOP, FALSE);
               }
            return ((MRESULT)0);
            }   
         if (pInstanceData->fCapture) {
            DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, TRUE);
            WinQueryPointerPos (HWND_DESKTOP, &pInstanceData->ptlEnd);
            DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, TRUE);
            if (pSettings->fMagnify)
               MagnifyPointer(pInstanceData->hab, &pInstanceData->ptlEnd, &pInstanceData->hptrMagnify, &pInstanceData->ptlMagnify);
            else {
               if (WinQuerySysValue (HWND_DESKTOP, SV_POINTERLEVEL) == 0)
                  WinShowPointer (HWND_DESKTOP, FALSE);
               }
            return ((MRESULT)0);
            }
         if (pInstanceData->fCaptureWindow) {
            WinSetPointer (HWND_DESKTOP, pInstanceData->hptrWindow);
            return ((MRESULT)0);
            }   
         break;

      case WM_BUTTON1UP:
         if (pInstanceData->fCapture) {
            DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, TRUE);
            if (WinQuerySysValue (HWND_DESKTOP, SV_POINTERLEVEL) > 0)
               WinShowPointer (HWND_DESKTOP, TRUE);
            WinQueryPointerPos (HWND_DESKTOP, &pInstanceData->ptlEnd);
            WinSetCapture (HWND_DESKTOP, NULLHANDLE) ;
            pInstanceData->fCapture = FALSE;

            if (pInstanceData->ptlStart.x <= pInstanceData->ptlEnd.x) {
                pInstanceData->rclTrack.xLeft   = pInstanceData->ptlStart.x;
                pInstanceData->rclTrack.xRight  = pInstanceData->ptlEnd.x+1;
            }else{
               pInstanceData->rclTrack.xLeft   = pInstanceData->ptlEnd.x;
               pInstanceData->rclTrack.xRight  = pInstanceData->ptlStart.x+1;
               }
            if (pInstanceData->ptlStart.y <= pInstanceData->ptlEnd.y) {
               pInstanceData->rclTrack.yBottom = pInstanceData->ptlStart.y;
               pInstanceData->rclTrack.yTop    = pInstanceData->ptlEnd.y+1;
            }else{
               pInstanceData->rclTrack.yBottom = pInstanceData->ptlEnd.y;
               pInstanceData->rclTrack.yTop    = pInstanceData->ptlStart.y+1;
               }

            if (!pSettings->include.fLeft)
                pInstanceData->rclTrack.xLeft++;
            if (!pSettings->include.fRight)
                pInstanceData->rclTrack.xRight--;
            if (!pSettings->include.fTop)
               pInstanceData->rclTrack.yTop--;
            if (!pSettings->include.fBottom)
               pInstanceData->rclTrack.yBottom++;

            if ((pInstanceData->rclTrack.xRight - pInstanceData->rclTrack.xLeft <= 1) || (pInstanceData->rclTrack.yTop - pInstanceData->rclTrack.yBottom <= 1)) {
               SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, pInstanceData->cxBitmap, pInstanceData->cyBitmap);
               if (pSettings->usHide != HIDE_NOHIDE)
                  ShowWindow (pInstanceData->hwndFrame, TRUE);
               return ((MRESULT)0);
               }
            if (pSettings->usDelay > 0) {
               pInstanceData->usTime = pSettings->usDelay;
               WinStartTimer (pInstanceData->hab, hwnd, ID_TIMER, 1000);
               MenuOnCapture (pInstanceData->hwndMenu, TRUE);
               }
            else if (pSettings->usHide == HIDE_CAPTUREONLY) {
               pInstanceData->usTime = 1;
               WinStartTimer (pInstanceData->hab, hwnd, ID_TIMER, 250);
               ShowWindow (pInstanceData->hwndFrame, FALSE);
               }
            else {
               WinPostMsg (hwnd, UM_NEWBITMAP, 0, 0);
               }
            }
            return ((MRESULT)0);

          case WM_SETFOCUS:
             if (!(SHORT1FROMMP(mp2))) {
                if (pInstanceData->fCapture || pInstanceData->fMouseCapture || pInstanceData->fCaptureWindow) {
                   if (pInstanceData->fMouseCapture)
                      DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, FALSE);
                   else if (pInstanceData->fCapture) {
                      DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, TRUE);
                      SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, pInstanceData->cxBitmap, pInstanceData->cyBitmap);
                      }
                   if (WinQuerySysValue (HWND_DESKTOP, SV_POINTERLEVEL) > 0)
                      WinShowPointer (HWND_DESKTOP, TRUE);
                   WinSetCapture (HWND_DESKTOP, NULLHANDLE);
                   pInstanceData->fMouseCapture = FALSE;
                   pInstanceData->fCapture = FALSE;
                   pInstanceData->fCaptureWindow = FALSE;
                   if (pSettings->usHide != HIDE_NOHIDE)
                      ShowWindow (pInstanceData->hwndFrame, TRUE);
                   }
                }
             return ((MRESULT)0);

          case WM_CHAR:
             if (pInstanceData->fCapture || pInstanceData->fMouseCapture || pInstanceData->fCaptureWindow) {
               if (CHARMSG (&msg)->fs & KC_VIRTUALKEY && !(CHARMSG(&msg)->fs & KC_KEYUP)) {
                  switch (CHARMSG(&msg)->vkey) {
                     POINTL ptlTmp;
                     case VK_ESC:
                        if (pInstanceData->fMouseCapture)
                           DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, FALSE);
                        else if (pInstanceData->fCapture) {
                           DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, TRUE);
                           SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, pInstanceData->cxBitmap, pInstanceData->cyBitmap);
                           }
                        if (WinQuerySysValue (HWND_DESKTOP, SV_POINTERLEVEL) > 0)
                           WinShowPointer (HWND_DESKTOP, TRUE);
                        WinSetCapture (HWND_DESKTOP, NULLHANDLE);
                        pInstanceData->fMouseCapture = FALSE;
                        pInstanceData->fCapture = FALSE;
                        pInstanceData->fCaptureWindow = FALSE;
                        if (pSettings->usHide != HIDE_NOHIDE)
                           ShowWindow (pInstanceData->hwndFrame, TRUE);
                        break;
                     case VK_BACKSPACE:
                        if (pInstanceData->fCapture) {
                           DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, TRUE);
                           SetTitle (pInstanceData->hwndFrame, pInstanceData->szTitle, pInstanceData->cxBitmap, pInstanceData->cyBitmap);
                           pInstanceData->fCapture = FALSE;
                           pInstanceData->fMouseCapture = TRUE;
                           DrawCrossHair (pInstanceData->hwndFrame, &pInstanceData->ptlStart, &pInstanceData->ptlEnd, &pSettings->include, &pInstanceData->ptlScreen, FALSE);
                        }
                        break;
                     case VK_UP:
                        WinQueryPointerPos (HWND_DESKTOP, &ptlTmp);
                        ptlTmp.y++;
                        WinSetPointerPos(HWND_DESKTOP, ptlTmp.x, ptlTmp.y);
                        break;
                     case VK_RIGHT:
                        WinQueryPointerPos (HWND_DESKTOP, &ptlTmp);
                        ptlTmp.x++;
                        WinSetPointerPos(HWND_DESKTOP, ptlTmp.x, ptlTmp.y);
                        break;
                     case VK_DOWN:
                        WinQueryPointerPos (HWND_DESKTOP, &ptlTmp);
                        ptlTmp.y--;
                        WinSetPointerPos(HWND_DESKTOP, ptlTmp.x, ptlTmp.y);
                        break;
                     case VK_LEFT:
                        WinQueryPointerPos (HWND_DESKTOP, &ptlTmp);
                        ptlTmp.x--;
                        WinSetPointerPos(HWND_DESKTOP, ptlTmp.x, ptlTmp.y);
                        break;
                  }
               }
             }
             return ((MRESULT)0);

      case WM_SIZE: 
         SetScrollBars (hwnd, pInstanceData->hwndVSB, pInstanceData->hwndHSB, pInstanceData->cxBitmap, pInstanceData->cyBitmap, &pInstanceData->sHSBPos, &pInstanceData->sVSBPos);
         break;

      case WM_HSCROLL: 
         WinScrollWindow (hwnd, HorzScroll (hwnd, pInstanceData->hwndHSB, mp2, &pInstanceData->sHSBPos), 0, NULL, NULL, NULLHANDLE, NULL, SW_INVALIDATERGN);
         break;

      case WM_VSCROLL: 
         WinScrollWindow (hwnd, 0, -VertScroll (hwnd, pInstanceData->hwndVSB, mp2, &pInstanceData->sVSBPos), NULL, NULL, NULLHANDLE, NULL, SW_INVALIDATERGN);
         break;

      case WM_PAINT: {
         RECTL rclUpdate;
         WinBeginPaint (hwnd, pInstanceData->hps, &rclUpdate);
         if (pInstanceData->hbm) {
            ULONG  ulTemp, cx, cy;
            POINTL aptl[2];
            RECTL  rclSrc;
            PRECTL prclSrc = (PRECTL)NULL;
            BOOL   fFill = FALSE;
            WinQueryWindowRect (hwnd, &rclSrc);
            cx = rclSrc.xRight - rclSrc.xLeft;
            cy = rclSrc.yTop - rclSrc.yBottom;
            if (pSettings->fFitWindow) {
               double xFact = ((double)cx) / pInstanceData->cxBitmap;
               double yFact = ((double)cy) / pInstanceData->cyBitmap;
               double fScaleFactor = min (xFact, yFact);
               aptl[1].x += aptl[0].x = (cx - (aptl[1].x = (LONG)ceil ((double)pInstanceData->cxBitmap * fScaleFactor))) / 2; 
               aptl[1].y += aptl[0].y = (cy - (aptl[1].y = (LONG)ceil ((double)pInstanceData->cyBitmap * fScaleFactor))) / 2;
               fFill = (xFact != yFact);
               }
            else {
               if (pInstanceData->cxBitmap < cx) {
                  aptl[0].x     = (cx - pInstanceData->cxBitmap) / 2;
                  rclSrc.xLeft  = 0;
                  rclSrc.xRight = pInstanceData->cxBitmap;
                  fFill = TRUE;
                  }
               else {
                  aptl[0].x     = 0;   
                  rclSrc.xLeft  = pInstanceData->sHSBPos;
                  rclSrc.xRight = rclSrc.xLeft + cx;
                  }
               if (pInstanceData->cyBitmap < cy) {
                  aptl[0].y      = (cy - pInstanceData->cyBitmap) / 2;
                  rclSrc.yBottom = 0;
                  rclSrc.yTop    = pInstanceData->cyBitmap;
                  fFill = TRUE;
                  }
               else {
                  aptl[0].y      = 0;
                  rclSrc.yTop    = pInstanceData->cyBitmap - pInstanceData->sVSBPos;
                  rclSrc.yBottom = rclSrc.yTop - cy;
                  }
               prclSrc = &rclSrc;
               }
            if (fFill)
               WinFillRect (pInstanceData->hps, &rclUpdate, CLR_BLACK);
            WinRealizePalette (pSettings->fAgrPalette ? hwnd : HWND_DESKTOP, pInstanceData->hps, &ulTemp);
            WinDrawBitmap (pInstanceData->hps, pInstanceData->hbm, prclSrc, &aptl[0], 0, 0, (pSettings->fFitWindow) ? DBM_STRETCH : DBM_NORMAL);
            }
         else
            WinFillRect (pInstanceData->hps, &rclUpdate, CLR_BLACK);
         WinEndPaint (pInstanceData->hps);
         }
         return ((MRESULT)0);

      case HM_QUERY_KEYS_HELP:
         return MRFROMLONG(HELP_KEYS_ID_MAINWND);

      case WM_ACTIVATE:
         if (SHORT1FROMMP (mp1))
// Set active help window to this window's parent when activated
            WinSendMsg (WinQueryHelpInstance (hwnd), HM_SET_ACTIVE_WINDOW, MPFROMHWND(pInstanceData->hwndFrame), MPFROMHWND(pInstanceData->hwndFrame));
         else
// clear active help window when this window is deactivated - necessary for message box help, etc. to work properly.
            WinSendMsg (WinQueryHelpInstance (hwnd), HM_SET_ACTIVE_WINDOW, NULL, NULL);
       break;

      case WM_SAVEAPPLICATION:
         SaveSettings (pInstanceData->hwndFrame, pSettings);
         break;
 
      case WM_DESTROY:
         WinSetParent (pInstanceData->hwndVSB, pInstanceData->hwndFrame, FALSE);
         WinSetParent (pInstanceData->hwndHSB, pInstanceData->hwndFrame, FALSE);
         WinDestroyPointer (pInstanceData->hptrMagnify);
         WinDestroyPointer (pInstanceData->hptrWindow);
         if (pInstanceData->hbm) 
            GpiDeleteBitmap (pInstanceData->hbm);
         if (pInstanceData->hpal) {
            GpiSelectPalette (pInstanceData->hps, NULLHANDLE);
            GpiDeletePalette (pInstanceData->hpal);
            }
         GpiAssociate (pInstanceData->hps, NULLHANDLE);
         GpiDestroyPS (pInstanceData->hps);
         free ((PVOID)pSettings);
         free ((PVOID)pInstanceData);
         break;
      }
   return (WinDefWindowProc (hwnd, msg, mp1, mp2));
   }



