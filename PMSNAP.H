/**************************************************************************
 *  File name  : pmsnap.h
 *  (c) Copyright Carrick von Schoultz 1994. All rights reserved.
 *  (c) Copyright Peter Nielsen 1994. All rights reserved.
 *  Description: 
 *************************************************************************/
#define LOCAL static
#define UM_NEWBITMAP WM_USER
extern BOOL flPaletteManagerAvailable;

typedef struct {
   HDC hdc;
   HPS hps;
   } MEMORYPS, *PMEMORYPS;

typedef struct {
   LONG x, y;
   LONG cx, cy;
   BOOL fMinimized;
   } WINDOWPOS, *PWINDOWPOS;

typedef struct {
   BOOL        fLeft;
   BOOL        fTop;
   BOOL        fRight;
   BOOL        fBottom;
   } INCLUDETRACK, *PINCLUDETRACK;


#define HIDE_INSTANTLY     0
#define HIDE_CAPTUREONLY   1
#define HIDE_NOHIDE        2

#define INDETERMINATE      2

#define MAGNIFY_1_5        0
#define MAGNIFY_2_0        1
#define MAGNIFY_2_5        2

typedef struct {
   CHAR         chVersion[20];
   USHORT       usDelay;
   USHORT       usHide;
   USHORT       usCopyPalette;
   USHORT       usMagnify;
   BOOL         fAgrPalette;
   BOOL         fBeep;
   BOOL         fMagnify;
   BOOL         fFitWindow;
   BOOL         fFullCapture;
   WINDOWPOS    windowpos;
   INCLUDETRACK include;
   } SETTINGS, *PSETTINGS;

#define DEFAULTSETTINGS {         \
   "",                            \
   0,                             \
   HIDE_CAPTUREONLY,              \
   INDETERMINATE,                 \
   MAGNIFY_2_0,                   \
   FALSE,                         \
   TRUE,                          \
   FALSE,                         \
   FALSE,                         \
   TRUE,                          \
   { 0, 0, 400, 300, 0 },         \
   { FALSE, FALSE, FALSE, FALSE } \
   }

#define ID_TIMER           280
#define ID_MAINWND         300
#define IDP_WINDOW         310

#define SZ_TITLE           401
#define SZ_BITMAPERROR     402
#define SZ_APPNAME         403
#define SZ_VERSION         404
#define SZ_KEYNAME         405
#define SZ_ENVIRON_VAR     406
#define SZ_INI_FILE        407
#define SZ_SAVE            408
#define SZ_PICTURESIZE     409
#define SZ_CONTINUE        410
#define SZ_COPYQ           411
#define SZ_PASTEQ          412
#define SZ_FILEWRITE       413
#define SZ_UNREGISTERED    414
#define SZ_HELPWINDOWTITLE 415
#define SZ_HELPLIBRARYNAME 416
#define SZ_HELPLOADERROR   417
#define SZ_FREEMEMORY      418
#define SZ_NOTACCEPTABLEW  419
#define SZ_NEWQ            420
#define SZ_BITMAP2BIG      421
#define SZ_FREEPHYSMEMORY  422
#define SZ_OFMEMORY        423

#define IDM_CANCELCAPTURE  19
#define IDM_CAPTURE        20
#define IDM_DESKTOP        21
#define IDM_ACTIVE         22
#define IDM_ACTIVECLIENT   23
#define IDM_REGION         24
#define IDM_MAGNIFY        25

#define IDM_EDIT           40
#define IDM_COPY           41
#define IDM_PASTE          42

#define IDM_DISPLAY        50
#define IDM_STRETCH        51
#define IDM_OPTIONS        52
#define IDM_PALETTEOPTIONS 53
#define IDM_FITWINDOW      54

#define IDM_HELP           60
#define IDM_HELPINDEX      61
#define IDM_HELPGENERAL    62
#define IDM_HELPUSINGHELP  63
#define IDM_HELPKEYS       64
#define IDM_PRODUCTINFO    65

#define IDM_FILE           70
#define IDM_SAVEAS         71
#define IDM_EXIT           72
#define IDM_NEW            73

#define ID_PRODUCTINFO      100
#define ID_ABOUT2           105
#define ID_BITMAP           110
#define ID_OPTIONS          120
#define ID_PALETTEOPTIONS   125
#define ID_QOPTIONS         130
#define IDD_MESSAGEBOX      135
#define IDC_BITMAP          180
#define IDC_ICON            190

#define IDC_HIDE_NO        200
#define IDC_HIDE_CAPT      210
#define IDC_HIDE_INST      215
#define IDC_DELAYED        220
#define IDC_BEEP           225
#define IDC_COPYPALETTE    230
#define IDC_AGRPALETTE     235
#define IDC_MAGNIFY        240
#define IDC_MAGNIFYBY      245
#define IDC_INCLUDELEFT    250
#define IDC_INCLUDETOP     255
#define IDC_INCLUDERIGHT   260
#define IDC_INCLUDEBOTTOM  265
#define IDC_FULLCAPTURE    270
#define IDC_VERSION        275
#define IDC_HELP           276

#define HELP_GENERAL_ID_MAINWND  500
#define HELP_ID_MAINWND          505
#define HELP_KEYS_ID_MAINWND     510
#define HELP_SUBHELP_ID_MAINWND  515
#define HELP_SUBHELP_ID_OPTIONS  520
#define HELP_SUBHELP_ID_PALETTEOPTIONS  525
#define HELP_SUBHELP_FILE_DIALOG 530

#define HELP_IDM_FILE            600
#define HELP_IDM_SAVEAS          601
#define HELP_IDM_EXIT            602

#define HELP_IDM_EDIT            610
#define HELP_IDM_COPY            611
#define HELP_IDM_PASTE           612

#define HELP_IDM_CAPTURE         620
#define HELP_IDM_CANCELCAPTURE   621
#define HELP_IDM_ACTIVE          622
#define HELP_IDM_ACTIVECLIENT    623
#define HELP_IDM_DESKTOP         624
#define HELP_IDM_REGION          625

#define HELP_IDM_DISPLAY         630
#define HELP_IDM_STRETCH         631
#define HELP_IDM_OPTIONS         632
#define HELP_IDM_PALETTEOPTIONS  633

#define HELP_ID_OPTIONS          650
#define HELP_IDC_HIDE            651
#define HELP_IDC_BEEP            654
#define HELP_IDC_DELAYED         655
#define HELP_IDC_FULLCAPTURE     656
#define HELP_IDC_MAGNIFY         657
#define HELP_IDC_MAGNIFYBY       658
#define HELP_IDC_INCLUDE         659
#define HELP_DID_OK              663
#define HELP_DID_CANCEL          664

#define HELP_ID_PALETTEOPTIONS   670
#define HELP_IDC_COPYPALETTE     671
#define HELP_IDC_AGRPALETTE      672

#define HELP_ID_SAVEAS           680

