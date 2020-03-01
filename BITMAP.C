/**************************************************************************
 *  File name  : bitmap.c
 *  (c) Copyright Carrick von Schoultz 1994. All rights reserved.
 *  (c) Copyright Peter Nielsen 1994. All rights reserved.
 *  Description: 
 *************************************************************************/
#define INCL_GPITRANSFORMS

#define INCL_GPIBITMAPS
#define INCL_GPILOGCOLORTABLE
#define INCL_GPIPRIMITIVES
#define INCL_WINCLIPBOARD
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WINSCROLLBARS
#define INCL_WININPUT
#define INCL_WINMENUS
#define INCL_WINPOINTERS
#define INCL_WINSHELLDATA
#define INCL_WINSTDFILE
#define INCL_WINSWITCHLIST
#define INCL_WINSYS
#define INCL_WINTRACKRECT
#define INCL_WINWINDOWMGR

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pmsnap.h"
#include "bitmap.h"
#include "dialogs.h"

BOOL  flPaletteManagerAvailable;

BOOL Query (HWND hwnd, PSZ pszText) {
   CHAR  szTitle[80];
   WinLoadString (WinQueryAnchorBlock (hwnd), NULLHANDLE, SZ_TITLE, sizeof (szTitle), szTitle);
   return ((BOOL)(WinMessageBox (HWND_DESKTOP, hwnd, pszText, szTitle, IDD_MESSAGEBOX, MB_MOVEABLE | MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == MBID_YES));
   }

LOCAL VOID CreateMemoryPS (HAB hab, PMEMORYPS pMemoryPS) {
   SIZEL        sizl = { 0, 0 };
   DEVOPENSTRUC dop = { NULL, "DISPLAY" };
   pMemoryPS->hdc = DevOpenDC (hab, OD_MEMORY, "*", 2L, (PDEVOPENDATA)&dop, NULLHANDLE);
   pMemoryPS->hps = GpiCreatePS (hab, pMemoryPS->hdc, &sizl, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
   }

LOCAL VOID DestroyMemoryPS (PMEMORYPS pMemoryPS) {
   GpiAssociate (pMemoryPS->hps, NULLHANDLE);
   GpiDestroyPS (pMemoryPS->hps);
   DevCloseDC (pMemoryPS->hdc);
   }

ULONG QueryImageDataSize (PBITMAPINFOHEADER2 pbmi) {
   return ((((pbmi->cx * pbmi->cBitCount + 31) / 8) & ~3) * pbmi->cy);
   }

LOCAL VOID CopyToClipboard (HWND hwnd, PBITMAPINFOHEADER2 pbmi, PBYTE pbImageData, USHORT usCopyPalette) {
   HAB      hab = WinQueryAnchorBlock (hwnd);
   ULONG    ulFmtInfo;
   MEMORYPS MemoryPS;
   HBITMAP  hbm;
   CHAR     szCopyQuestion[200];
   BOOL     fQuery;

   WinLoadString (hab, NULLHANDLE, SZ_COPYQ, sizeof (szCopyQuestion), szCopyQuestion);
   fQuery = (usCopyPalette > 1) ? Query (hwnd, szCopyQuestion) : (BOOL)usCopyPalette;
   WinOpenClipbrd (hab);
   CreateMemoryPS (hab, &MemoryPS);

   if (pbmi->cBitCount != 24 && flPaletteManagerAvailable && fQuery) {
      HPAL hpal = GpiCreatePalette (hab, 0L, LCOLF_CONSECRGB, 256, (PULONG)(((PBYTE)pbmi) + pbmi->cbFix));
      GpiSelectPalette (MemoryPS.hps, hpal);
      hbm = GpiCreateBitmap (MemoryPS.hps, pbmi, CBM_INIT, pbImageData, (PBITMAPINFO2)pbmi);
      WinSetClipbrdData (hab, hbm, CF_BITMAP, CFI_HANDLE);
      WinSetClipbrdData (hab, hpal, CF_PALETTE, CFI_HANDLE);
      GpiSelectPalette (MemoryPS.hps, NULLHANDLE);
      }
   else {
      GpiCreateLogColorTable (MemoryPS.hps, LCOL_RESET, LCOLF_RGB, 0, 0, NULL);
      hbm = GpiCreateBitmap (MemoryPS.hps, pbmi, CBM_INIT, pbImageData, (PBITMAPINFO2)pbmi);
      WinSetClipbrdData (hab, hbm, CF_BITMAP, CFI_HANDLE);
      if (WinQueryClipbrdFmtInfo (hab, CF_PALETTE, &ulFmtInfo)) {
         WinSetClipbrdData (hab, NULLHANDLE, CF_PALETTE, CFI_HANDLE);
         }
      }

   DestroyMemoryPS (&MemoryPS);
   WinCloseClipbrd (hab);
   }

LOCAL VOID PasteFromClipboard (HWND hwnd, PBITMAPINFOHEADER2 pbmi, PBYTE *ppbImageData, USHORT usCopyPalette) {
   HAB     hab = WinQueryAnchorBlock (hwnd);
   HBITMAP hbm;
   CHAR    szPasteQuestion[200];
   BOOL    fQuery;

   WinLoadString (hab, NULLHANDLE, SZ_PASTEQ, sizeof (szPasteQuestion), szPasteQuestion);
   fQuery = (usCopyPalette > 1) ? Query (hwnd, szPasteQuestion) : (BOOL)usCopyPalette;

   WinOpenClipbrd (hab);
   hbm = WinQueryClipbrdData (hab, CF_BITMAP);

   if (hbm != NULLHANDLE) {
      MEMORYPS MemoryPS;
      HPAL     hpal = WinQueryClipbrdData (hab, CF_PALETTE);

      GpiQueryBitmapInfoHeader (hbm, pbmi);
      if (pbmi->cBitCount < 8)            
         pbmi->cBitCount = 8;
      if (pbmi->cbFix >= 20)
         pbmi->ulCompression = BCA_UNCOMP;   
      *ppbImageData = (PBYTE)malloc (QueryImageDataSize (pbmi));

      CreateMemoryPS (hab, &MemoryPS);
      GpiSetBitmap (MemoryPS.hps, hbm);
      GpiQueryBitmapBits (MemoryPS.hps, 0L, pbmi->cy, *ppbImageData, (PBITMAPINFO2)pbmi);
      DestroyMemoryPS (&MemoryPS);

      if (hpal != NULLHANDLE && flPaletteManagerAvailable && fQuery) {
         GpiQueryPaletteInfo (hpal, NULLHANDLE, 0, 0, 1 << pbmi->cBitCount, (PULONG)(((PBYTE)pbmi) + pbmi->cbFix));
         }
      }

   WinCloseClipbrd (hab);
   }

VOID CopyBitmapToClipboard (HWND hwnd, HBITMAP hbm, HPAL hpal, USHORT usCopyPalette) {
   HAB                hab = WinQueryAnchorBlock (hwnd);
   PBITMAPINFOHEADER2 pbmi = (PBITMAPINFOHEADER2)malloc (BITMAPINFOHEADER2SIZE + 256 * sizeof (RGB2));
   PBYTE                pbImageData;
   MEMORYPS           MemoryPS;

   pbmi->cbFix = BITMAPINFOHEADER2SIZE;
   GpiQueryBitmapInfoHeader (hbm, pbmi);

   pbImageData = (PBYTE)malloc (QueryImageDataSize (pbmi));

   CreateMemoryPS (hab, &MemoryPS);
   GpiSetBitmap (MemoryPS.hps, hbm);
   GpiQueryBitmapBits (MemoryPS.hps, 0L, pbmi->cy, pbImageData, (PBITMAPINFO2)pbmi);
   if (hpal)
      GpiQueryPaletteInfo (hpal, NULLHANDLE, 0, 0, 1 << pbmi->cBitCount, (PULONG)(((PBYTE)pbmi) + pbmi->cbFix));
   DestroyMemoryPS (&MemoryPS);

   CopyToClipboard (hwnd, pbmi, pbImageData, usCopyPalette);

   free ((PVOID)pbImageData);
   free ((PVOID)pbmi);
   }

VOID PasteClipboardToBitmap (HWND hwnd, PHBITMAP phbm, PHPAL phpal, USHORT usCopyPalette) {
   HAB                hab = WinQueryAnchorBlock (hwnd);
   PBITMAPINFOHEADER2 pbmi = (PBITMAPINFOHEADER2)malloc (BITMAPINFOHEADER2SIZE + 256 * sizeof (RGB2));
   PBYTE              pbImageData;
   MEMORYPS           MemoryPS;

   pbmi->cbFix = BITMAPINFOHEADER2SIZE;
   PasteFromClipboard (hwnd, pbmi, &pbImageData, usCopyPalette);

   CreateMemoryPS (hab, &MemoryPS);

   if (pbmi->cBitCount == 24)
      GpiCreateLogColorTable (MemoryPS.hps, LCOL_RESET, LCOLF_RGB, 0, 0, NULL);
   else if (flPaletteManagerAvailable) {
      *phpal =   GpiCreatePalette (hab, 0L, LCOLF_CONSECRGB, 1 << pbmi->cBitCount, (PULONG)(((PBYTE)pbmi) + pbmi->cbFix));
      GpiSelectPalette (MemoryPS.hps, *phpal);
      }          
    *phbm = GpiCreateBitmap (MemoryPS.hps, pbmi, CBM_INIT, pbImageData, (PBITMAPINFO2)pbmi);
   GpiSetBitmap (MemoryPS.hps, *phbm);

   DestroyMemoryPS (&MemoryPS);
   }

VOID DrawCrossHair (HWND hwndFrame, POINTL *pptlStart, PPOINTL pptlEnd, PINCLUDETRACK pInclude, PPOINTL pptlScreen, BOOL fDrawBox){
   CHAR   szTitle[20];
   HPS    hps   = WinGetScreenPS (HWND_DESKTOP);
   LONG   lIdPS = GpiSavePS (hps);
   LONG   lX    = abs(pptlStart->x - pptlEnd->x)+1;
   LONG   lY    = abs(pptlStart->y - pptlEnd->y)+1;
   POINTL ptlStart, ptlEnd;

   GpiSetMix (hps, FM_INVERT);
   if (fDrawBox) {
      GpiMove (hps, pptlStart);
      ptlEnd.x = pptlStart->x;
      ptlEnd.y = pptlEnd->y;
      GpiLine (hps, &ptlEnd);
      GpiMove (hps, pptlStart);
      ptlEnd.x = pptlEnd->x;
      ptlEnd.y = pptlStart->y;
      GpiLine (hps, &ptlEnd);
      }
   ptlStart.x = 0;
   ptlStart.y = pptlEnd->y;
   ptlEnd.x = pptlScreen->x;
   ptlEnd.y = pptlEnd->y;
   GpiMove (hps, &ptlStart);
   GpiLine (hps, &ptlEnd);
   ptlStart.x = pptlEnd->x;
   ptlStart.y = 0;
   ptlEnd.x = pptlEnd->x;
   ptlEnd.y = pptlScreen->y;
   GpiMove (hps, &ptlStart);
   GpiLine (hps, &ptlEnd);

   GpiRestorePS (hps, lIdPS);
   WinReleasePS (hps);
   if (fDrawBox) {
      if (!pInclude->fLeft)
         lX--;
      if (!pInclude->fRight)
         lX--;
      if (!pInclude->fTop)
         lY--;
      if (!pInclude->fBottom)
         lY--;
      if (lX > 0 && lY > 0){
         sprintf (szTitle, "%d x %d", lX, lY);
         WinSetWindowText (hwndFrame, szTitle);
         }
      }
   }

VOID MagnifyPointer (HAB hab, PPOINTL pptl, HPOINTER *phptr, PPOINTL pptlMagnify) {
   PBITMAPINFOHEADER2   pbmi;
   HPS                  hpsDesktop;
   MEMORYPS             MemoryPS;
   POINTL               aptl[4];
   ULONG                ulBitCount;
   POINTERINFO          pointerInfo;
   HPAL                 hpal = NULLHANDLE, hpalDesktop;
   PULONG               pulPalette;
   PBYTE                pbPointerData;
   const ULONG          cxPointer = 64;
   const ULONG          cyPointer = 64;

   WinDestroyPointer (*phptr);
   CreateMemoryPS (hab, &MemoryPS);
   hpsDesktop = WinGetPS (HWND_DESKTOP);
   DevQueryCaps (GpiQueryDevice (hpsDesktop), CAPS_COLOR_BITCOUNT, 1L, (PLONG)&ulBitCount);
   if (ulBitCount > 8)
      ulBitCount = 24;

   pbmi = (PBITMAPINFOHEADER2)malloc (BITMAPINFOHEADER2SIZE + ((ulBitCount > 8) ? 2 : (1 << ulBitCount)) * sizeof (RGB2));
   pbmi->cbFix     = BITMAPINFOHEADER2SIZE;
   pbmi->cx        = cxPointer;
   pbmi->cy        = cyPointer * 2;
   pbmi->cPlanes   = 1;
   pbmi->cBitCount = 1;
   pulPalette = (PULONG)(((PBYTE)pbmi) + pbmi->cbFix);
   pulPalette[0] = 0x000000;
   pulPalette[1] = 0xFFFFFF;
   pbPointerData = (PBYTE)&pulPalette[2];
   memset ((PVOID)pbPointerData, 0, ((cxPointer / 8 + 3) & ~3) * cyPointer * 2);
   pointerInfo.hbmPointer = GpiCreateBitmap (MemoryPS.hps, pbmi, CBM_INIT, pbPointerData, (PBITMAPINFO2)pbmi);

   WinLockVisRegions (HWND_DESKTOP, TRUE);

   if (pbmi->cBitCount == 24)
      GpiCreateLogColorTable (MemoryPS.hps, LCOL_RESET, LCOLF_RGB, 0, 0, NULL);
   else if (flPaletteManagerAvailable) {
      ULONG ulNumColors = GpiQueryRealColors (hpsDesktop, 0, 0, 1 << ulBitCount, (PLONG)(((PBYTE)pbmi) + pbmi->cbFix));
      hpal = GpiCreatePalette (hab, 0L, LCOLF_CONSECRGB, ulNumColors, (PULONG)(((PBYTE)pbmi) + pbmi->cbFix));
      GpiSelectPalette (MemoryPS.hps, hpal);
      }
   else {
      ULONG ulNumColors = GpiQueryLogColorTable (hpsDesktop, 0, 0, 1 << ulBitCount, (PLONG)(((PBYTE)pbmi) + pbmi->cbFix));
      GpiCreateLogColorTable (MemoryPS.hps, LCOL_RESET, LCOLF_CONSECRGB, 0, ulNumColors, (PLONG)(((PBYTE)pbmi) + pbmi->cbFix));
      }
   pbmi->cy             = cyPointer;
   pbmi->cBitCount      = ulBitCount;
   pointerInfo.hbmColor = GpiCreateBitmap (MemoryPS.hps, pbmi, 0, NULL, (PBITMAPINFO2)pbmi);
   free ((PVOID)pbmi);

   aptl[0].x = 0;
   aptl[0].y = 0;
   aptl[1].x = pbmi->cx;
   aptl[1].y = pbmi->cy;
   aptl[2].x = pptl->x - pptlMagnify->x;
   aptl[2].y = pptl->y - pptlMagnify->y;
   aptl[3].x = pptl->x + pptlMagnify->x;
   aptl[3].y = pptl->y + pptlMagnify->y;

   GpiSetBitmap (MemoryPS.hps, pointerInfo.hbmColor);
   if (hpal)
      hpalDesktop = GpiSelectPalette (hpsDesktop, hpal);
   GpiBitBlt (MemoryPS.hps, hpsDesktop, 4, aptl, ROP_SRCCOPY, BBO_IGNORE);
   if (hpal)
      GpiSelectPalette (hpsDesktop, hpalDesktop);
   GpiSetBitmap (MemoryPS.hps, NULLHANDLE);
   WinLockVisRegions (HWND_DESKTOP, FALSE);
   WinReleasePS (hpsDesktop);

   pointerInfo.fPointer = FALSE; 
   pointerInfo.xHotspot = (cxPointer + 1) / 2;
   pointerInfo.yHotspot = (cyPointer + 1) / 2;
   *phptr = WinCreatePointerIndirect (HWND_DESKTOP, &pointerInfo);
   WinSetPointer (HWND_DESKTOP, *phptr);

   GpiDeleteBitmap (pointerInfo.hbmColor);
   GpiDeleteBitmap (pointerInfo.hbmPointer);
   if (hpal) {
      GpiSelectPalette (MemoryPS.hps, NULLHANDLE);
      GpiDeletePalette (hpal);
      }
   DestroyMemoryPS (&MemoryPS);
   }

VOID SetDesktopPos (LONG lPosX, LONG lPosY, BOOL fRepaintNow, HAB hab) {
   PSWP    pswp, pswpBase, pswp2, pswp2Base, pswp3, pswp3Base;
   HWND    hwndMain, ownerhwnd, tophwnd; 
   BOOL    fNotMoved;

   pswpBase = (PSWP)calloc (2500, sizeof (SWP));
   pswp2Base = (PSWP)calloc (2500, sizeof (SWP));
   pswp3Base = (PSWP)calloc (2500, sizeof (SWP));

   pswp = pswpBase;
   pswp2 = pswp2Base;
   pswp3 = pswp3Base;
   
   tophwnd = hwndMain = WinQueryWindow(WinQueryActiveWindow(HWND_DESKTOP), QW_PREVTOP);

   do {
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
               WinQueryWindowPos(hwndIconText, pswp3++);
               }
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
                        WinQueryWindowPos(hwndIconText, pswp3++);
                        }
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
   while (((tophwnd = WinQueryWindow(tophwnd, QW_NEXTTOP)) != hwndMain));

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

   WinSetPointer (HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE));
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
   if (pswp3 > pswp3Base){
      PSWP pswp3Tmp;
      for (pswp3Tmp = pswp3Base; pswp3Tmp < pswp3; pswp3Tmp++){
         pswp3Tmp->x += lPosX;
         pswp3Tmp->y += lPosY;
         pswp3Tmp->fl = SWP_MOVE | SWP_FOCUSDEACTIVATE;
         WinSetWindowUShort (pswp3Tmp->hwnd, QWS_XMINIMIZE, (USHORT)-1);
         WinSetWindowUShort (pswp3Tmp->hwnd, QWS_YMINIMIZE, (USHORT)-1);
         }
      WinSetMultWindowPos(hab, pswp3Base, ((ULONG)pswp3 - (ULONG)pswp3Base) / sizeof (SWP));
      }
   free ((PVOID)pswpBase);
   free ((PVOID)pswp2Base);
   free ((PVOID)pswp3Base);
   WinSetPointer (HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE));
   } // end SetDesktopPos

#define CLAMP(x,lo,hi) (((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi) : (x)))

VOID CopyToBitmap (HPS hpsWork, HPS hpsDesktop, PRECTL prclWorld, HAB hab) {
   LONG cxScreen      = WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN);
   LONG cyScreen      = WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN);
   LONG xLeftScreen   = cxScreen * ((prclWorld->xLeft < 0) ? -((cxScreen - prclWorld->xLeft) / cxScreen) : (prclWorld->xLeft / cxScreen));
   LONG yBottomScreen = cyScreen * ((prclWorld->yBottom < 0) ? -((cyScreen - prclWorld->yBottom) / cyScreen) : (prclWorld->yBottom / cyScreen));
   LONG xRightScreen  = cxScreen * ((prclWorld->xRight < 0) ? -((-prclWorld->xRight) / cxScreen) : (prclWorld->xRight / cxScreen));
   LONG yTopScreen    = cyScreen * ((prclWorld->yTop < 0) ? -((-prclWorld->yTop) / cyScreen) : (prclWorld->yTop / cyScreen));
   LONG yDesktop, xDesktop, xBitmap, yBitmap;
   LONG yPrevious = 0, xPrevious = 0;

   for (yDesktop = yBottomScreen, yBitmap = 0; yDesktop <= yTopScreen; yDesktop += cyScreen) {
      RECTL  rclDesktop;
      POINTL aptl[3];
      rclDesktop.yBottom = CLAMP (prclWorld->yBottom - yDesktop, 0, cyScreen);
      rclDesktop.yTop    = CLAMP (prclWorld->yTop - yDesktop, 0, cyScreen);
      for (xDesktop = xLeftScreen, xBitmap = 0; xDesktop <= xRightScreen; xDesktop += cxScreen) {
         LONG xMove = xPrevious - xDesktop;
         LONG yMove = yPrevious - yDesktop;

         xPrevious = xDesktop;
         yPrevious = yDesktop;

         if ((xMove != 0) || (yMove != 0)){
            SetDesktopPos (xMove, yMove, TRUE, hab);
            }

         rclDesktop.xLeft   = CLAMP (prclWorld->xLeft - xDesktop, 0, cxScreen);
         rclDesktop.xRight  = CLAMP (prclWorld->xRight - xDesktop, 0, cxScreen);
         aptl[0].x = xBitmap;
         aptl[0].y = yBitmap;
         aptl[1].x = xBitmap + rclDesktop.xRight - rclDesktop.xLeft;
         aptl[1].y = yBitmap + rclDesktop.yTop - rclDesktop.yBottom;
         aptl[2].x = rclDesktop.xLeft;
         aptl[2].y = rclDesktop.yBottom;

         WinLockVisRegions (HWND_DESKTOP, TRUE);
         GpiBitBlt (hpsWork, hpsDesktop, 3, aptl, ROP_SRCCOPY, 0);
         WinLockVisRegions (HWND_DESKTOP, FALSE);
         xBitmap = aptl[1].x;
         }
      yBitmap = aptl[1].y;
      }
   if ((xPrevious != 0) || (yPrevious != 0)){
      SetDesktopPos (xPrevious, yPrevious, TRUE, hab);
      }
   }

VOID CopyDesktopToBitmap (HPS hpsWork, HPS hpsDesktop, PRECTL prclDesktop) {
   POINTL aptl[3];

   aptl[0].x = 0;
   aptl[0].y = 0;
   aptl[1].x = prclDesktop->xRight - prclDesktop->xLeft;
   aptl[1].y = prclDesktop->yTop - prclDesktop->yBottom;
   aptl[2].x = prclDesktop->xLeft;
   aptl[2].y = prclDesktop->yBottom;
   WinLockVisRegions (HWND_DESKTOP, TRUE);
   GpiBitBlt (hpsWork, hpsDesktop, 3, aptl, ROP_SRCCOPY, 0);
   WinLockVisRegions (HWND_DESKTOP, FALSE);
   }

VOID CopyScreenToBitmap (HWND hwnd, PRECTL prclTrack, PHBITMAP phbm, PHPAL phpal, BOOL fFullCapture) {
   HAB                  hab = WinQueryAnchorBlock (hwnd);
   PBITMAPINFOHEADER2   pbmi;
   HPS                  hpsDesktop;
   MEMORYPS             MemoryPS;
   ULONG                ulBitCount;
   HBITMAP              hbmTemp;
   RECTL                rclDesktop;

   CreateMemoryPS (hab, &MemoryPS);
   hpsDesktop = WinGetPS (HWND_DESKTOP);
   DevQueryCaps (GpiQueryDevice (hpsDesktop), CAPS_COLOR_BITCOUNT, 1L, (PLONG)&ulBitCount);
   if (ulBitCount > 8)
      ulBitCount = 24;
   pbmi = (PBITMAPINFOHEADER2)malloc (BITMAPINFOHEADER2SIZE + ((ulBitCount > 8) ? 0 : ((1 << ulBitCount) * sizeof (RGB2))));
   pbmi->cbFix     = BITMAPINFOHEADER2SIZE;
   pbmi->cPlanes   = 1;
   pbmi->cBitCount = ulBitCount;
   if (fFullCapture){
      pbmi->cx        = prclTrack->xRight - prclTrack->xLeft;
      pbmi->cy        = prclTrack->yTop - prclTrack->yBottom;
      }
   else {
      LONG cxScreen      = WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN);
      LONG cyScreen      = WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN);
      rclDesktop.yBottom = CLAMP (prclTrack->yBottom, 0, cyScreen);
      rclDesktop.yTop    = CLAMP (prclTrack->yTop, 0, cyScreen);
      rclDesktop.xLeft   = CLAMP (prclTrack->xLeft, 0, cxScreen);
      rclDesktop.xRight  = CLAMP (prclTrack->xRight, 0, cxScreen);
      pbmi->cx           = rclDesktop.xRight - rclDesktop.xLeft;
      pbmi->cy           = rclDesktop.yTop - rclDesktop.yBottom;
      }
   if (pbmi->cBitCount == 24)
      GpiCreateLogColorTable (MemoryPS.hps, LCOL_RESET, LCOLF_RGB, 0, 0, NULL);
   else if (flPaletteManagerAvailable) {
      ULONG ulNumColors = GpiQueryRealColors (hpsDesktop, 0, 0, 1 << ulBitCount, (PLONG)(((PBYTE)pbmi) + pbmi->cbFix));
      *phpal = GpiCreatePalette (hab, 0L, LCOLF_CONSECRGB, ulNumColors, (PULONG)(((PBYTE)pbmi) + pbmi->cbFix));
      GpiSelectPalette (MemoryPS.hps, *phpal);
      }
   hbmTemp = GpiCreateBitmap (MemoryPS.hps, pbmi, 0, NULL, (PBITMAPINFO2)pbmi);
   GpiSetBitmap (MemoryPS.hps, hbmTemp);
   if (fFullCapture)
      CopyToBitmap (MemoryPS.hps, hpsDesktop, prclTrack, hab);
   else
      CopyDesktopToBitmap (MemoryPS.hps, hpsDesktop, &rclDesktop);
   GpiSetBitmap (MemoryPS.hps, NULLHANDLE);
   WinReleasePS (hpsDesktop);
   DestroyMemoryPS (&MemoryPS);
   free ((PVOID)pbmi);
   *phbm = hbmTemp;
   }

VOID QueryBitmapSize (HBITMAP hbm, PLONG plWidth, PLONG plHeight) {
   BITMAPINFOHEADER2 bmi;
   bmi.cbFix = sizeof (bmi);
   GpiQueryBitmapInfoHeader (hbm, &bmi);
   *plWidth = bmi.cx;
   *plHeight = bmi.cy;
   }

LOCAL VOID SetFileExtension (PCHAR pchFileName, PCHAR pchExtension, ULONG ulMaxLen) {
   PCHAR pchExt = (PCHAR)strrchr (pchFileName, '.');
   ULONG i, l;
   ulMaxLen--;
   if (pchExt == NULL) {
      if ((l = strlen (pchFileName)) < ulMaxLen)
         pchFileName[l++] = '.';
      }
   else 
      l = (pchExt - pchFileName) + 1;
   if (l < ulMaxLen) 
      for (i = l; (l < ulMaxLen) && ((pchFileName[l] = pchExtension[l-i]) != '\0'); l++);
   pchFileName[l] = '\0';
   }

LOCAL BOOL SaveBMP (HWND hwnd, PCHAR pszFileName, HBITMAP hbm, HPAL hpal, BOOL f24bit) {
   HAB                  hab = WinQueryAnchorBlock (hwnd);
   HFILE                hFile;
   ULONG                ulTemp;
   MEMORYPS             MemoryPS;
   BOOL                 fSuccess = TRUE;
   ULONG                ulHeaderSize = sizeof (BITMAPFILEHEADER2) - sizeof (BITMAPINFOHEADER2) + 40;
   PBITMAPFILEHEADER2   pbfh = (PBITMAPFILEHEADER2)calloc (ulHeaderSize + 256 * sizeof (RGB2), 1);
   PBYTE                pbImageData;
   ULONG                ulDataSize;

   pbfh->usType         = BFT_BMAP;
   pbfh->bmp2.cbFix     = 40;
   pbfh->bmp2.cBitCount = f24bit ? 24 : 8;
   pbfh->bmp2.cPlanes   = 1;

   GpiQueryBitmapInfoHeader (hbm, &pbfh->bmp2);

   CreateMemoryPS (hab, &MemoryPS);

   if (pbfh->bmp2.cBitCount == 24)
      GpiCreateLogColorTable (MemoryPS.hps, LCOL_RESET, LCOLF_RGB, 0, 0, NULL);
   else if (hpal != NULLHANDLE) {
      GpiSelectPalette (MemoryPS.hps, hpal);
      GpiQueryPaletteInfo (hpal, NULLHANDLE, 0, 0, 1 << pbfh->bmp2.cBitCount, (PULONG)(((PBYTE)&pbfh->bmp2) + pbfh->bmp2.cbFix));
      }
   GpiSetBitmap (MemoryPS.hps, hbm);

   pbImageData = (PBYTE)malloc (ulDataSize = QueryImageDataSize (&pbfh->bmp2));
   GpiQueryBitmapBits (MemoryPS.hps, 0L, pbfh->bmp2.cy, pbImageData, (PBITMAPINFO2)(&pbfh->bmp2));

   pbfh->offBits = ulHeaderSize + (pbfh->bmp2.cBitCount == 24 ? 0 : (1 << pbfh->bmp2.cBitCount)) * sizeof (RGB2);

   if (DosOpen (pszFileName, &hFile, &ulTemp, 0L, FILE_NORMAL, OPEN_ACTION_REPLACE_IF_EXISTS|OPEN_ACTION_CREATE_IF_NEW, OPEN_ACCESS_WRITEONLY|OPEN_SHARE_DENYWRITE|OPEN_FLAGS_SEQUENTIAL, 0L) == 0) {
      ULONG ulBytesWritten;
      DosWrite (hFile, (PVOID)pbfh, pbfh->offBits, &ulBytesWritten);
      if (ulBytesWritten == pbfh->offBits) {
         DosWrite (hFile, pbImageData, ulDataSize, &ulBytesWritten);
         if (ulBytesWritten == ulDataSize)
            fSuccess = TRUE;
         }
      DosClose (hFile);
      }

   if (hpal != NULLHANDLE)
   GpiSelectPalette (MemoryPS.hps, NULLHANDLE);
   DestroyMemoryPS (&MemoryPS);
   free ((PVOID)pbImageData);
   free ((PVOID)pbfh);

   if (!fSuccess){
      CHAR szWriteOpenError[200];
      CHAR  szTitle[80];
      WinLoadString (WinQueryAnchorBlock (hwnd), NULLHANDLE, SZ_TITLE, sizeof (szTitle), szTitle);
      WinLoadString (WinQueryAnchorBlock (hwnd), NULLHANDLE, SZ_FILEWRITE, sizeof (szWriteOpenError), szWriteOpenError);
      WinAlarm(HWND_DESKTOP, WA_ERROR);
      WinMessageBox (HWND_DESKTOP, hwnd, szWriteOpenError, szTitle, IDD_MESSAGEBOX, MB_MOVEABLE | MB_OK | MB_ERROR);
      }
   return (fSuccess);
   }

//MRESULT APIENTRY SaveFilterDlgProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2) {
//   if(msg == WM_HELP) {
//      WinSendMsg (WinQueryHelpInstance (hwnd), HM_DISPLAY_HELP, MPFROM2SHORT(HELP_ID_SAVEAS, NULL), MPFROMSHORT(HM_RESOURCEID));
//      return (MRESULT)FALSE ;
//      }
//   return WinDefFileDlgProc( hwnd, msg, mp1, mp2 );
//   }

BOOL SaveAsFile(HWND hwnd, HBITMAP hbm, HPAL hpal, BOOL f24bit) {
   FILEDLG  FileDlg;

   memset (&FileDlg, 0, sizeof(FILEDLG));                  // Initialise FileDlg to NULL
   FileDlg.cbSize      = sizeof(FILEDLG);                  // Set size
   if ((BOOL)WinQueryHelpInstance (hwnd)){
      FileDlg.fl          = FDS_HELPBUTTON | FDS_CENTER | FDS_SAVEAS_DIALOG; // Open Dialog and centred
//      FileDlg.pfnDlgProc  = (PFNWP)SaveFilterDlgProc;
      }
   else {
      FileDlg.fl = FDS_CENTER | FDS_SAVEAS_DIALOG;         // Open Dialog and centred
      }
   FileDlg.pszTitle    = "Save Bitmap";                    // Dialog title
   FileDlg.pszOKButton = "Save";                           // OK button text
   strcpy (FileDlg.szFullFile, "*.BMP");
   if (!WinFileDlg (HWND_DESKTOP, hwnd, &FileDlg))         // Display dialog
      return (FALSE);
   if (FileDlg.lReturn != DID_OK)
      return (FALSE);
   SetFileExtension (FileDlg.szFullFile, "BMP", sizeof (FileDlg.szFullFile));
   return (SaveBMP (hwnd, FileDlg.szFullFile, hbm, hpal, f24bit));
   }


VOID SetScrollBars (HWND hwnd, HWND hwndVSB, HWND hwndHSB, SHORT cxBitmap, SHORT cyBitmap, PSHORT psVSBPos, PSHORT psHSBPos) {
   SHORT   sHSBRange, sVSBRange;
   RECTL rcl;
   LONG  cx, cy;

   WinQueryWindowRect (hwnd, &rcl);
   cx = rcl.xRight-rcl.xLeft;
   cy = rcl.yTop-rcl.yBottom;

   *psHSBPos = 0;
   *psVSBPos = 0;

   sHSBRange = (cx >= cxBitmap) ? 0 : cxBitmap - cx;
   sVSBRange = (cy >= cyBitmap) ? 0 : cyBitmap - cy;

   WinSendMsg (hwndHSB, SBM_SETTHUMBSIZE, MPFROM2SHORT (cx, cxBitmap), 0);
   WinSendMsg (hwndHSB, SBM_SETSCROLLBAR, MPFROMSHORT (*psHSBPos), MPFROM2SHORT (0, sHSBRange));

   WinSendMsg (hwndVSB, SBM_SETTHUMBSIZE, MPFROM2SHORT (cy, cyBitmap), 0);
   WinSendMsg (hwndVSB, SBM_SETSCROLLBAR, MPFROMSHORT (*psVSBPos), MPFROM2SHORT (0, sVSBRange));
   }


SHORT HorzScroll (HWND hwnd, HWND hwndHSB, MPARAM mp, PSHORT sPosOld) {
   SHORT    sPos, sDiff;
   RECTL    rcl;
   MRESULT  mr = WinSendMsg (hwndHSB, SBM_QUERYRANGE, NULL, NULL);
   SHORT    sLowRange = SHORT1FROMMR (mr), sHighRange = SHORT2FROMMR (mr);

   switch (SHORT2FROMMP (mp)) {
      default:
      case SB_SLIDERTRACK:
      case SB_SLIDERPOSITION:
         sPos = SHORT1FROMMP (mp);
         break;
      case SB_LINELEFT:
         if (sLowRange == *sPosOld)
              goto _HorzExit;
         sPos = max (sLowRange, *sPosOld - 8);
         break;
      case SB_LINERIGHT: 
         if (sHighRange == *sPosOld)
              goto _HorzExit;
         sPos = min (sHighRange, *sPosOld + 8);
         break;
      case SB_PAGELEFT:
         if (sLowRange == *sPosOld)
              goto _HorzExit;
         WinQueryWindowRect (hwnd, &rcl);
         sPos = max (sLowRange, *sPosOld - (rcl.xRight - rcl.xLeft));
         break;
      case SB_PAGERIGHT:
         if (sHighRange == *sPosOld)
              goto _HorzExit;
         WinQueryWindowRect (hwnd, &rcl);
         sPos = min (sHighRange, *sPosOld + (rcl.xRight - rcl.xLeft));
         break;
_HorzExit:
         WinAlarm (HWND_DESKTOP, WA_WARNING);
      case SB_ENDSCROLL:
         return (0);
      }
   WinSendMsg (hwndHSB, SBM_SETPOS, MPFROMSHORT (sPos), NULL);
   sDiff = *sPosOld - sPos;
   *sPosOld = sPos;
   return (sDiff);
   }

SHORT VertScroll (HWND hwnd, HWND hwndVSB, MPARAM mp, PSHORT sPosOld) {
   SHORT    sPos, sDiff;
   RECTL    rcl;
   MRESULT  mr = WinSendMsg (hwndVSB, SBM_QUERYRANGE, NULL, NULL);
   SHORT    sLowRange = SHORT1FROMMR (mr), sHighRange = SHORT2FROMMR (mr);

   switch (SHORT2FROMMP (mp)) {
      default:
      case SB_SLIDERTRACK:
      case SB_SLIDERPOSITION:
         sPos = SHORT1FROMMP (mp);
         break;
      case SB_LINEUP:
         if (sLowRange == *sPosOld)
            goto _VertExit;
         sPos = max (sLowRange, *sPosOld - 8);
         break;
      case SB_LINEDOWN: 
         if (sHighRange == *sPosOld)
            goto _VertExit;
         sPos = min (sHighRange, *sPosOld + 8);
         break;
      case SB_PAGEUP:
         if (sLowRange == *sPosOld)
            goto _VertExit;
         WinQueryWindowRect (hwnd, &rcl);
         sPos = max (sLowRange, *sPosOld - (rcl.yTop - rcl.yBottom));
         break;
      case SB_PAGEDOWN:
         if (sHighRange == *sPosOld)
            goto _VertExit;
         WinQueryWindowRect (hwnd, &rcl);
         sPos = min (sHighRange, *sPosOld + (rcl.yTop - rcl.yBottom));
         break;
_VertExit:
         WinAlarm (HWND_DESKTOP, WA_WARNING);
      case SB_ENDSCROLL:
         return (0);
      }
   WinSendMsg (hwndVSB, SBM_SETPOS, MPFROMSHORT (sPos), NULL);
   sDiff = *sPosOld - sPos;
   *sPosOld = sPos;
   return (sDiff);
   }



