        case IDM_NEW: {
         CHAR szQuestion[200];
         WinLoadString (pInstanceData->hab, (HMODULE)NULL, SZ_NEWQ, sizeof (szQuestion), szQuestion);
         if (Query (hwnd, szQuestion)) {
            CHAR szTitle[50];
            RemoveBitmapFromMem(pInstanceData->hab, hwnd, &pInstanceData->hdc, &pInstanceData->hps, &pInstanceData->hbm, &pInstanceData->hpal);
            WinLoadString (pInstanceData->hab, (HMODULE)NULL, SZ_UNREGISTERED, sizeof (szTitle), szTitle);
            WinSetWindowText(pInstanceData->hwndFrame, szTitle);
            WinInvalidateRect(hwnd, (PRECTL)NULL, FALSE);
            }
         }
         break;

VOID RemoveBitmapFromMem(HAB hab, HWND hwnd, PHDC phdc, PHPS phps, PHBITMAP phbm, PHPAL phpal) {
   SIZEL sizl = { 0, 0 };
   if (*phbm) 
      GpiDeleteBitmap (*phbm);
   if (*phpal) {
      GpiSelectPalette (*phps, NULLHANDLE);
      GpiDeletePalette (*phpal);
      }
   GpiAssociate (*phps, NULLHANDLE);
   GpiDestroyPS (*phps);
   *phps = GpiCreatePS (hab, *phdc, &sizl, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
   }

/*****************************************************************************/
/*  Abstract: RemoveBitmapFromMem()                                          */
/*    This function totally frees the bitmap, presentation space, and        */
/*    device context, and then sets all of the pointers to these items to    */
/*    NULL.                                                                  */
/*****************************************************************************/
VOID RemoveBitmapFromMem(PHDC phdc, PHPS phps, PHBITMAP phbm)
{
  if (phbm) {
    if (phps && *phps) {
        /* Remove old bitmap... */
        GpiSetBitmap (*phps, (HBITMAP)NULL);
    }
    if (*phbm) {
      GpiDeleteBitmap(*phbm);
    }

    /* Free up the PS... */
    if (phps) {
      if (*phps) {
        GpiAssociate(*phps, (HDC)NULL);
        GpiDestroyPS(*phps);
      }
      *phps = (HPS)NULL;
    }
    *phbm = (HBITMAP)NULL;
  } else if (phps) {
    if (*phps) {
      /* Remove old bitmap... */
      GpiSetBitmap (*phps, (HBITMAP)NULL);
      /* Free up the PS... */
      GpiAssociate(*phps, (HDC)NULL);
      GpiDestroyPS(*phps);
    }
    *phps = (HPS)NULL;
  }

  if (phdc) {
    if (*phdc) {
      /* Remove old DC... */
      DevCloseDC(*phdc);
    }
    *phdc = (HDC)NULL;
  }
}




   hpsDesktop = WinGetPS (HWND_DESKTOP);
   DevQueryCaps (GpiQueryDevice (hpsDesktop), CAPS_COLOR_BITCOUNT, 1L, (PLONG)&ulBitCount);
   if (ulBitCount > 8)
      ulBitCount = 24;
   WinReleasePS (hpsDesktop);
((((ulCx * ulBitCount + 31) / 8) & ~3) * ulCy)/1000000

   {
   LONG lKeyState;
   WinEnablePhysInput (HWND_DESKTOP, TRUE);
   WinAlarm (HWND_DESKTOP, WA_NOTE);
   do {
      lKeyState = WinGetPhysKeyState(HWND_DESKTOP, VK_SPACE);
      } 
   while (lKeyState >= 0); // || (lScanCode!=VK_SPACE) || (lScanCode!=VK_ENTER) || (lScanCode!=VK_ESC) || (lScanCode!=VK_BUTTON1) || (lScanCode!=VK_BUTTON2));
   WinEnablePhysInput (HWND_DESKTOP, FALSE);
   }


GpiQueryPageViewport(hpsDesktop, &rclTemp);
sprintf (szDim, "Vieport: %d x %d %d x %d", rclTemp.xLeft, rclTemp.yBottom, rclTemp.xRight, rclTemp.yTop);
Query (HWND_DESKTOP, szDim);
rclTemp.yBottom -=500;
rclTemp.yTop    -=500;
GpiSetPageViewport(hpsDesktop, &rclTemp);
WinSendMsg(HWND_DESKTOP, WM_PAINT, 0, 0);
sprintf (szDim, "Vieport: %d x %d %d x %d", rclTemp.xLeft, rclTemp.yBottom, rclTemp.xRight, rclTemp.yTop);
Query (HWND_DESKTOP, szDim);
rclTemp.yBottom +=500;
rclTemp.yTop    +=500;
GpiSetPageViewport(hpsDesktop, &rclTemp);
WinSendMsg(HWND_DESKTOP, WM_PAINT, 0, 0);
sprintf (szDim, "Vieport: %d x %d %d x %d", rclTemp.xLeft, rclTemp.yBottom, rclTemp.xRight, rclTemp.yTop);
Query (HWND_DESKTOP, szDim);

/*
VOID CopyScreenToBitmap (HWND hwnd, PRECTL prclTrack, PHBITMAP phbm, PHPAL phpal) {
   HAB                  hab = WinQueryAnchorBlock (hwnd);
   PBITMAPINFOHEADER2   pbmi;
   HPS                  hpsDesktop;
   MEMORYPS             MemoryPS;
   POINTL               aptl[3];
   ULONG                ulBitCount;

   CreateMemoryPS (hab, &MemoryPS);
   hpsDesktop = WinGetPS (HWND_DESKTOP);
   DevQueryCaps (GpiQueryDevice (hpsDesktop), CAPS_COLOR_BITCOUNT, 1L, (PLONG)&ulBitCount);
   if (ulBitCount > 8)
      ulBitCount = 24;
   pbmi = (PBITMAPINFOHEADER2)malloc (BITMAPINFOHEADER2SIZE + ((ulBitCount == 24) ? 0 : (1 << ulBitCount)) * sizeof(RGB2));
   pbmi->cbFix     = BITMAPINFOHEADER2SIZE;
   pbmi->cx        = prclTrack->xRight - prclTrack->xLeft;
   pbmi->cy        = prclTrack->yTop   - prclTrack->yBottom;
   pbmi->cPlanes   = 1;
   pbmi->cBitCount = ulBitCount;
   aptl[0].x = 0;
   aptl[0].y = 0;
   aptl[1].x = pbmi->cx;
   aptl[1].y = pbmi->cy;
   aptl[2].x = prclTrack->xLeft;
   aptl[2].y = prclTrack->yBottom;
   WinLockVisRegions (HWND_DESKTOP, TRUE);
   if (pbmi->cBitCount == 24)
      GpiCreateLogColorTable (MemoryPS.hps, LCOL_RESET, LCOLF_RGB, 0, 0, NULL);
   else if (flPaletteManagerAvailable) {
      GpiQueryRealColors (hpsDesktop, 0, 0, 256, (PLONG)(((PBYTE)pbmi) + pbmi->cbFix));
      *phpal =   GpiCreatePalette (hab, 0L, LCOLF_CONSECRGB, 1 << pbmi->cBitCount, (PULONG)(((PBYTE)pbmi) + pbmi->cbFix));
      GpiSelectPalette (MemoryPS.hps, *phpal);
      }
   *phbm = GpiCreateBitmap (MemoryPS.hps, pbmi, 0, NULL, (PBITMAPINFO2)pbmi);
   GpiSetBitmap (MemoryPS.hps, *phbm);
   GpiBitBlt (MemoryPS.hps, hpsDesktop, 3, aptl, ROP_SRCCOPY, 0);
   WinLockVisRegions (HWND_DESKTOP, FALSE);
   WinReleasePS (hpsDesktop);
   DestroyMemoryPS (&MemoryPS);
   free ((PVOID)pbmi);
   }
*/


   while(((tophwnd = WinQueryWindow(tophwnd, QW_NEXTTOP)) != hwndMain) && i < 100) {
      fNotMoved = TRUE;
      if (WinQueryWindow(tophwnd, QW_FRAMEOWNER) == NULLHANDLE) {
         for (z=0; z <= i; z++){
            if (tophwnd == swp[z].hwnd){
               fNotMoved = FALSE;
               break;
               }
            }
         if (fNotMoved == FALSE) continue;
         WinQueryWindowPos(tophwnd, (PSWP)&swp[i]);
         if (swp[i].fl & SWP_MINIMIZE){
            CHAR szClassBuffer[80];
            HWND hwndIconText = WinQueryWindow(tophwnd, QW_NEXT);
            WinQueryClassName (hwndIconText, sizeof (szClassBuffer), szClassBuffer);
            if (!strcmp (szClassBuffer, "#32765")){
               swp[i].x += lPosX;
               swp[i].y += lPosY;
               swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
               }
            }
         swp[i].x += lPosX;
         swp[i].y += lPosY;
         swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
         } 
      else {
         if (WinQueryWindow(tophwnd, QW_FRAMEOWNER) == hwndMain) continue;
         WinQueryWindowPos(tophwnd, (PSWP)&swp2[j++]);
         ownerhwnd = tophwnd;
         while((ownerhwnd = WinQueryWindow(ownerhwnd, QW_FRAMEOWNER)) != NULLHANDLE) {
            if (ownerhwnd != hwndMain){
               if (WinQueryWindow(ownerhwnd, QW_FRAMEOWNER) == NULLHANDLE){
                  for (z=0; z < i; z++){
                     if (ownerhwnd == swp[z].hwnd){
                        fNotMoved = FALSE;
                        break;
                        }
                     }
                  if (fNotMoved == FALSE) continue;
                  WinQueryWindowPos(ownerhwnd, (PSWP)&swp[i]);
                  if (swp[i].fl & SWP_MINIMIZE){
                     CHAR szClassBuffer[80];
                     HWND hwndIconText = WinQueryWindow(ownerhwnd, QW_NEXT);
                     WinQueryClassName (hwndIconText, sizeof (szClassBuffer), szClassBuffer);
                     if (!strcmp (szClassBuffer, "#32765")){
                        swp[i].x += lPosX;
                        swp[i].y += lPosY;
                        swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
                        }
                     }
                  swp[i].x += lPosX;
                  swp[i].y += lPosY;
                  swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
                  } 
               else
                  WinQueryWindowPos(ownerhwnd, (PSWP)&swp2[j++]);
               }
            }
         }
      }

   WinSetMultWindowPos(hab, pswp, i-1);

   if (j > 0){
   for (z=0; z <= j; z++){
         if (WinQueryWindow(swp2[z].hwnd, QW_FRAMEOWNER) != NULLHANDLE){
            if (swp2[z].fl & SWP_MINIMIZE){
               CHAR szClassBuffer[80];
               HWND hwndIconText = WinQueryWindow(swp2[z].hwnd, QW_NEXT);
               WinQueryClassName (hwndIconText, sizeof (szClassBuffer), szClassBuffer);
               if (!strcmp (szClassBuffer, "#32765")){
                  swp2[z].x += lPosX;
                  swp2[z].y += lPosY;
                  swp2[z].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
                  }
               }
            else {
               swp2[z].x += lPosX;
               swp2[z].y += lPosY;
               swp2[z].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
               }
            }
         }
         WinSetMultWindowPos(hab, pswp2, j);
      }


// fungerande flyttning (iconer flyttas inte)
 while(((tophwnd = WinQueryWindow(tophwnd, QW_NEXTTOP)) != hwndMain) && i < 100) {
	fNotMoved = TRUE;
	if (WinQueryWindow(tophwnd, QW_FRAMEOWNER) == NULLHANDLE) {
		for (z=0; z <= i; z++){
			if (tophwnd == swp[z].hwnd){
				fNotMoved = FALSE;
				break;
			}
		}
		if (fNotMoved == FALSE) continue;
		WinQueryWindowPos(tophwnd, (PSWP)&swp[i]);
		if (!(swp[i].fl & SWP_MINIMIZE)){
			if (!(swp[i].fl & SWP_HIDE)){
				swp[i].x += lPosX;
				swp[i].y += lPosY;
				swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
			}
		}
	} else {
		if (WinQueryWindow(tophwnd, QW_FRAMEOWNER) == hwndMain) continue;
		WinQueryWindowPos(tophwnd, (PSWP)&swp2[j++]);
		ownerhwnd = tophwnd;
		while((ownerhwnd = WinQueryWindow(ownerhwnd, QW_FRAMEOWNER)) != NULLHANDLE) {
			if (ownerhwnd != hwndMain){
				if (WinQueryWindow(ownerhwnd, QW_FRAMEOWNER) == NULLHANDLE){
					for (z=0; z < i; z++){
						if (ownerhwnd == swp[z].hwnd){
							fNotMoved = FALSE;
							break;
						}
					}
					if (fNotMoved == FALSE) continue;
					WinQueryWindowPos(ownerhwnd, (PSWP)&swp[i]);
					if (!(swp[i].fl & SWP_MINIMIZE)){
						if (!(swp[i].fl & SWP_HIDE)){
							swp[i].x += lPosX;
							swp[i].y += lPosY;
							swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
						}
					}
				} else {
					WinQueryWindowPos(ownerhwnd, (PSWP)&swp2[j++]);
				}
			}
		}
	}
 }

 WinSetMultWindowPos(hab, pswp, i);

 if (j > 0){
	for (z=0; z <= j; z++){
		if (WinQueryWindow(swp2[z].hwnd, QW_FRAMEOWNER) != NULLHANDLE){
			if (!(swp2[z].fl & SWP_MINIMIZE)){
				if (!(swp2[z].fl & SWP_HIDE)){
					swp2[z].x += lPosX;
					swp2[z].y += lPosY;
					swp2[z].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
				}
			}
		}
	}
	WinSetMultWindowPos(hab, pswp2, j);
 }


//extra testkod
//		WinQueryWindowPos(tophwnd, (PSWP)&swp2[j++]);
   WinQueryWindowPos(tophwnd, (PSWP)&swp2[j]);
   if (swp2[j].fl & SWP_MINIMIZE){
      CHAR szClassBuffer[80];
      HWND hwndIconText = WinQueryWindow(tophwnd, QW_NEXT);
      j++;
      WinQueryClassName (hwndIconText, sizeof (szClassBuffer), szClassBuffer);
      if (!strcmp (szClassBuffer, "#32765")){
         WinQueryWindowPos(hwndIconText, (PSWP)&swp2[j++]);
         }
      }
   else {
      j++;
      }

//					WinQueryWindowPos(ownerhwnd, (PSWP)&swp2[j++]);
   WinQueryWindowPos(ownerhwnd, (PSWP)&swp2[j]);
   if (swp2[j].fl & SWP_MINIMIZE){
      CHAR szClassBuffer[80];
      HWND hwndIconText = WinQueryWindow(ownerhwnd, QW_NEXT);
      j++;
      WinQueryClassName (hwndIconText, sizeof (szClassBuffer), szClassBuffer);
      if (!strcmp (szClassBuffer, "#32765")){
         WinQueryWindowPos(hwndIconText, (PSWP)&swp2[j++]);
         }
      }
   else {
      j++;
      }
