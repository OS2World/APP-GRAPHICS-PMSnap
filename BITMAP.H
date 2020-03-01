/**************************************************************************
 *  File name  : bitmap.h
 *  (c) Copyright Carrick von Schoultz 1994. All rights reserved.
 *  (c) Copyright Peter Nielsen 1994. All rights reserved.
 *  Description:  This header file contains the application-wide function
 *                declarations
 *************************************************************************/

#ifndef INCL_BITMAP
#define INCL_BITMAP

#define BITMAPINFOHEADER2SIZE 16

BOOL Query (HWND hwnd, PSZ pszText);
VOID  DrawCrossHair (HWND hwndFrame, POINTL *pptlStart, PPOINTL pptlEnd, PINCLUDETRACK pInclude, PPOINTL pptlScreen, BOOL fDrawBox);
VOID  MagnifyPointer (HAB hab, PPOINTL pptl, HPOINTER *phptr, PPOINTL pptlMagnify);
VOID  PasteClipboardToBitmap (HWND hwnd, PHBITMAP phbm, PHPAL phpal, USHORT usCopyPalette);
VOID  CopyBitmapToClipboard (HWND hwnd, HBITMAP hbm, HPAL hpal, USHORT usCopyPalette);
VOID  CopyScreenToBitmap (HWND hwnd, PRECTL prclTrack, PHBITMAP phbm, PHPAL phpal, BOOL fFullCapture);
BOOL  SaveAsFile (HWND hwnd, HBITMAP hbm, HPAL hpal, BOOL f24bit);
VOID  QueryBitmapSize (HBITMAP hbm, PLONG plWidth, PLONG plHeight);
VOID  SetScrollBars (HWND hwnd, HWND hwndVSB, HWND hwndHSB, SHORT cxBitmap, SHORT cyBitmap, PSHORT psVSBPos, PSHORT psHSBPos);
SHORT VertScroll (HWND hwnd, HWND hwndVSB, MPARAM mp, PSHORT sPosOld);
SHORT HorzScroll (HWND hwnd, HWND hwndHSB, MPARAM mp, PSHORT sPosOld);

#endif
