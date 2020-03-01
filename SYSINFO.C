#define INCL_DOS
#define INCL_WIN
#define INCL_ERRORS

#include <os2.h>
#include <stdio.h>
#include "sysinfo.h"

VOID QueryAllValues (HWND);

extern HAB hab;

CHAR szSysValues[QSV_MAX][18] = {
   "MAX_PATH_LENGTH  ",
   "MAX_TEXT_SESSIONS",
   "MAX_PM_SESSIONS  ",
   "MAX_VDM_SESSIONS ",
   "BOOT_DRIVE       ",
   "DYN_PRI_VARIATION",
   "MAX_WAIT         ",
   "MIN_SLICE        ",
   "MAX_SLICE        ",
   "PAGE_SIZE        ",
   "VERSION_MAJOR    ",
   "VERSION_MINOR    ",
   "VERSION_REVISION ",
   "MS_COUNT         ",
   "TIME_LOW         ",
   "TIME_HIGH        ",
   "TOTPHYSMEM       ",
   "TOTRESMEM        ",
   "TOTAVAILMEM      ",
   "MAXPRMEM         ",
   "MAXSHMEM         ",
   "TIMER_INTERVAL   ",
   "MAX_COMP_LENGTH  "
   };

VOID QueryAllValues (HWND hwnd) {
   ULONG ulValues[QSV_MAX];
   CHAR  szText[31];
   ULONG ulInx;

   DosQuerySysInfo (QSV_MAX_PATH_LENGTH, QSV_MAX, &ulValues, sizeof (ulValues));
   for (ulInx = 0; ulInx < QSV_MAX; ulInx++) {
      sprintf (szText, "%17s   0x%08lx", szSysValues[ulInx], ulValues[ulInx]);
      WinUpper (hab, 0, 0, szText);
      szText[21] = 'x';
      WinSendDlgItemMsg (hwnd, IDC_SYSINFOLIST, LM_INSERTITEM, (MPARAM)LIT_END, szText);
      }
   return;
   }

MRESULT EXPENTRY SysInfoDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   BOOL    fHandled = TRUE;
   MRESULT mReturn  = 0;
   HWND    hwndListBox;
   SWP     swp;

   switch (msg) {

      case WM_INITDLG:
         QueryAllValues (hwnd);
         break;

      case WM_WINDOWPOSCHANGED:
         if (((PSWP)mp1)->fl & SWP_SHOW) {
            hwndListBox = WinWindowFromID (hwnd, IDC_SYSINFOLIST);
            WinQueryWindowPos (hwndListBox, &swp);
            WinSetWindowPos (hwndListBox, 0L, (((PSWP)mp1)->cx - swp.cx) >> 1, swp.y, 0L, 0L, SWP_MOVE);
            }
         fHandled = FALSE;
         break;

      default:
         fHandled = FALSE;
         break;

      }

   if (!fHandled)
      mReturn = WinDefDlgProc (hwnd, msg, mp1, mp2);

   return (mReturn);
   }
