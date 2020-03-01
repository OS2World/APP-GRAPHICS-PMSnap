VOID SetDesktopPos (LONG lPosX, LONG lPosY, BOOL fRepaintNow, HAB hab) {
   PSWP    pswp, pswpBase, pswp2, pswp2Base;
   HWND    hwndMain, ownerhwnd, tophwnd; 
   BOOL    fNotMoved;

   pswpBase = (PSWP)calloc (2500, sizeof (SWP));
   pswp2Base = (PSWP)calloc (2500, sizeof (SWP));

   pswp = pswpBase;
   pswp2 = pswp2Base;
   
   tophwnd = hwndMain = WinQueryActiveWindow(HWND_DESKTOP);

   WinQueryWindowPos(hwndMain, pswp);
   if (pswp->fl & SWP_MINIMIZE){
      CHAR szClassBuffer[80];
      HWND hwndIconText = WinQueryWindow(hwndMain, QW_NEXT);
      WinQueryClassName (hwndIconText, sizeof (szClassBuffer), szClassBuffer);
      if (!strcmp (szClassBuffer, "#32765")){
         WinQueryWindowPos(hwndIconText, pswp); //*(pswp + 1) = *pswp;
         pswp->x += lPosX;
         pswp->y += lPosY;
         pswp->fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
         pswp++;
         }
      WinQueryWindowPos(hwndMain, pswp);
      }
   pswp->x += lPosX;
   pswp->y += lPosY;
   pswp->fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
   pswp++;

   while (((tophwnd = WinQueryWindow(tophwnd, QW_NEXTTOP)) != hwndMain)) {
      fNotMoved = TRUE;
      if (WinQueryWindow(tophwnd, QW_FRAMEOWNER) == NULLHANDLE) {
         PSWP pswpTmp = pswpBase;
         while (pswpTmp < pswp){
            if (tophwnd == pswpTmp++->hwnd){
               fNotMoved = FALSE;
               break;
               }
            }
         if (fNotMoved == FALSE)
            continue;
         WinQueryWindowPos(tophwnd, pswp);
         if (pswp->fl & SWP_MINIMIZE){
            CHAR szClassBuffer[80];
            HWND hwndIconText = WinQueryWindow(tophwnd, QW_NEXT);
            WinQueryClassName (hwndIconText, sizeof (szClassBuffer), szClassBuffer);
            if (!strcmp (szClassBuffer, "#32765")){
               WinQueryWindowPos(hwndIconText, pswp); //*(pswp + 1) = *pswp;
               pswp->x += lPosX;
               pswp->y += lPosY;
               pswp->fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
               pswp++;
               }
            WinQueryWindowPos(tophwnd, pswp);
            }
         pswp->x += lPosX;
         pswp->y += lPosY;
         pswp->fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
         pswp++;
         } 
      else {
         if (WinQueryWindow (tophwnd, QW_FRAMEOWNER) == hwndMain)
            continue;
         WinQueryWindowPos(tophwnd, pswp2++);
         ownerhwnd = tophwnd;
         while((ownerhwnd = WinQueryWindow(ownerhwnd, QW_FRAMEOWNER)) != NULLHANDLE) {
            if (ownerhwnd != hwndMain){
               if (WinQueryWindow(ownerhwnd, QW_FRAMEOWNER) == NULLHANDLE){
                  PSWP pswpTmp;   
                  for (pswpTmp = pswpBase; pswpTmp < pswp; pswpTmp++){
                     if (ownerhwnd == pswpTmp->hwnd){
                        fNotMoved = FALSE;
                        break;
                        }
                     }
                  if (fNotMoved == FALSE)
                     continue;
                  WinQueryWindowPos(ownerhwnd, pswp);
                  if (pswp->fl & SWP_MINIMIZE){
                     CHAR szClassBuffer[80];
                     HWND hwndIconText = WinQueryWindow (ownerhwnd, QW_NEXT);
                     WinQueryClassName (hwndIconText, sizeof (szClassBuffer), szClassBuffer);
                     if (!strcmp (szClassBuffer, "#32765")){
                        WinQueryWindowPos(hwndIconText, pswp); //*(pswp + 1) = *pswp;
                        pswp->x += lPosX;
                        pswp->y += lPosY;
                        pswp->fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
                        pswp++;
                        }
                     WinQueryWindowPos(ownerhwnd, pswp);
                     }
                  pswp->x += lPosX;
                  pswp->y += lPosY;
                  pswp->fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
                  pswp++;
                  }
               else {
                  WinQueryWindowPos(ownerhwnd, pswp2++);
                  }
               }
            }
         }
      }

   WinSetMultWindowPos(hab, pswpBase, ((ULONG)pswp - (ULONG)pswpBase) / sizeof (SWP));

   if (pswp2 > pswp2Base){
      PSWP pswp2Tmp;
      for (pswp2Tmp = pswp2Base; pswp2Tmp < pswp2; pswp2Tmp++){
         if (WinQueryWindow(pswp2Tmp->hwnd, QW_FRAMEOWNER) != NULLHANDLE){
            pswp2Tmp->x += lPosX;
            pswp2Tmp->y += lPosY;
            pswp2Tmp->fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
            }
         }
      WinSetMultWindowPos(hab, pswp2Base, ((ULONG)pswp2 - (ULONG)pswp2Base) / sizeof (SWP));
      }
   {  
   PSWP pswpTmp;
   for (pswpTmp = pswpBase; pswpTmp < pswp; pswpTmp++)
      WinSendMsg(pswpTmp->hwnd, WM_PAINT, 0, 0);
   }
   {
   PSWP pswp2Tmp;
   for (pswp2Tmp = pswp2Base; pswp2Tmp < pswp2; pswp2Tmp++)
      WinSendMsg(pswp2Tmp->hwnd, WM_PAINT, 0, 0);
   }
   free ((PVOID)pswpBase);
   free ((PVOID)pswp2Base);
   } // end SetDesktopPos
