/**************************************************************************
 *  File name  : dialogs.h
 *  (c) Copyright Carrick von Schoultz 1994. All rights reserved.
 *  (c) Copyright Peter Nielsen 1994. All rights reserved.
 *  Description:  This header file contains the application-wide function
 *                declarations
 *************************************************************************/
#ifndef INCL_DIALOGS
#define INCL_DIALOGS

VOID BitmapCreationError (HWND hwnd);
VOID HelpProductInfo (HWND hwndClient);
MRESULT EXPENTRY OptionsDlgProc (HWND hwnd, LONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY PaletteOptionsDlgProc (HWND hwnd, LONG msg, MPARAM mp1, MPARAM mp2);

#endif
