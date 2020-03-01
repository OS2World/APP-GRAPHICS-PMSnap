/*            BOOL  fQuery=TRUE;
            if (((pInstanceData->rclTrack.yTop-pInstanceData->rclTrack.yBottom > pInstanceData->ptlScreen.y) || (pInstanceData->rclTrack.xRight-pInstanceData->rclTrack.xLeft > pInstanceData->ptlScreen.x)) && pSettings->fFullCapture) {
               CHAR szQuestion[200], szFreeMemory[50], szContinue[50], szTemp[20];
               HWND hwndCapture = WinQueryFocus (HWND_DESKTOP);
               ULONG ulBitCount;
               float flMemory;
               ULONG ulCx = pInstanceData->rclTrack.xRight-pInstanceData->rclTrack.xLeft;
               ULONG ulCy = pInstanceData->rclTrack.yTop-pInstanceData->rclTrack.yBottom;
               HPS  hpsDesktop = WinGetPS (HWND_DESKTOP);

               DevQueryCaps (GpiQueryDevice (hpsDesktop), CAPS_COLOR_BITCOUNT, 1L, (PLONG)&ulBitCount);
               if (ulBitCount > 8)
                  ulBitCount = 24;
               WinReleasePS (hpsDesktop);
               flMemory = (float)((((ulCx * ulBitCount + 31) / 8) & ~3) * ulCy) / 1000000;
               sprintf (szTemp, " %d x %d.\n", ulCx, ulCy);
               WinLoadString (pInstanceData->hab, NULLHANDLE, SZ_PICTURESIZE, sizeof (szQuestion), szQuestion);
               strcat (szQuestion, szTemp);
               WinLoadString (pInstanceData->hab, NULLHANDLE, SZ_FREEMEMORY, sizeof (szFreeMemory), szFreeMemory);
               strcat (szQuestion, szFreeMemory);
               sprintf (szTemp, " %6.2f MB.\n", flMemory);
               strcat (szQuestion, szTemp);
               WinLoadString (pInstanceData->hab, NULLHANDLE, SZ_CONTINUE, sizeof (szContinue), szContinue);
               strcat (szQuestion, szContinue);
               fQuery = Query (hwnd, szQuestion);
               WinSetFocus (HWND_DESKTOP, hwndCapture);
               }
            if (fQuery) {
*/

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
   else if (ulBitmapSize > (ulFreePhysMem / 2)){
      CHAR  szTitle[80], szQuestion[300], szFreeMemory[50], szContinue[50], szTemp[20];
      WinLoadString (hab, NULLHANDLE, SZ_TITLE, sizeof (szTitle), szTitle);
      WinLoadString (hab, NULLHANDLE, SZ_PICTURESIZE, sizeof (szQuestion), szQuestion);
      sprintf (szTemp, " %7.3f MB.\n", (float)ulBitmapSize / 1000000);
      strcat (szQuestion, szTemp);
      WinLoadString (hab, NULLHANDLE, SZ_FREEPHYSMEMORY, sizeof (szFreeMemory), szFreeMemory);
      strcat (szQuestion, szFreeMemory);
      sprintf (szTemp, " %7.3f MB.\n", (float)ulFreePhysMem / 1000000);
      strcat (szQuestion, szTemp);
      WinLoadString (hab, NULLHANDLE, SZ_FREEMEMORY, sizeof (szFreeMemory), szFreeMemory);
      strcat (szQuestion, szFreeMemory);
      sprintf (szTemp, " %7.3f MB.\n", (float)ulFreeMem / 1000000);
      strcat (szQuestion, szTemp);
      WinLoadString (hab, NULLHANDLE, SZ_CONTINUE, sizeof (szContinue), szContinue);
      strcat (szQuestion, szContinue);
      fQuery = ((BOOL)(WinMessageBox (HWND_DESKTOP, hwnd, szQuestion, szTitle, IDD_MESSAGEBOX, MB_MOVEABLE | MB_YESNO | MB_CUAWARNING | MB_DEFBUTTON1) == MBID_YES));
      WinSetFocus (HWND_DESKTOP, hwndCapture);
      }
   return fQuery;
   }
