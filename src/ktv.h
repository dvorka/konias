//-------------------------------------------------------------------------
//                     Turbo Vision defines for Konias
//
//								  Dvorka
//                                 2002
//-------------------------------------------------------------------------

#ifndef __KTV_H
 #define __KTV_H

 // Compile stuff ---------------------------------------------------------

 #define KONIAS_VERSION "0.7"

 // compile version with extra debug information
 #define DEBUG

 // TV defines ------------------------------------------------------------

 #define Uses_TApplication
 #define Uses_TKeys
 #define Uses_TRect
 #define Uses_TMenuBar
 #define Uses_TSubMenu
 #define Uses_TMenuItem
 #define Uses_TStatusLine
 #define Uses_TStatusItem
 #define Uses_TStatusDef
 #define Uses_TDeskTop
 #define Uses_TFileDialog

 #define Uses_TDialog
 #define Uses_TButton
 #define Uses_TLabel
 #define Uses_TInputLine
 #define Uses_TRadioButtons
 #define Uses_TCheckBoxes
 #define Uses_TSItem
 #define Uses_TListBox
 #define Uses_TListViewer
 #define Uses_TScrollBar
 #define Uses_TStringCollection

 #define Uses_TScreen
 #define Uses_MsgBox
 #define Uses_TEvent

 #include <tv.h>

 #include <object.h>

 // - My types ----------------------------------------------------------

 #define boolean char

 #define TRUE 1
 #define FALSE !TRUE

 // - TV commands - (100-255, 1000-65K)---------------------------------

 const int cmNop               = 100;
 const int cmNewReader         = 101;
 const int cmNewBook           = 102;
 const int cmAbout             = 103;
 const int cmVideoMode         = 104;
 const int cmFindReader        = 105;
 const int cmFindBook          = 106;
 const int cmFindBookCriteria  = 107;
 const int cmQuickStore        = 108;
 const int cmQuickLoad         = 109;
 const int cmFreeKLibrary      = 110;
 const int cmGetLibraryInfo    = 111;
 const int cmNewLoan           = 112; // not used
 const int cmFindReaderCriteria= 113;
 const int cmFindLoan          = 114;
 const int cmBackupLibrary     = 115;
 const int cmLoan              = cmYes;
 const int cmNewCd             = 117;
 const int cmFindCdCriteria    = 118;
 const int cmFinishLoan        = cmYes;
 const int cmNewLoanOfBook     = 119;
 const int cmNewLoanToReader   = 120;
 const int cmAboutReader       = 121;
 const int cmAboutLoan         = 122;
 const int cmAboutBook         = 123;
 const int cmReturnLoan        = 124;
 const int cmDeleteLoan        = cmYes;
 const int cmLoadLibAs         = 125;
 const int cmStoreLibAs        = 126;
 const int cmRestoreLibrary    = 127;
 const int cmDeleteReader      = cmNo;
 const int cmDeleteBook        = cmNo;

 // help context
 const int hcMenuKonias     = 300;
 const int hcChangeVideoMode= 301;
 const int hcExitProgram    = 302;
 const int hcMenuLibrary    = 303;
 const int hcLibraryLoad    = 304;
 const int hcLibraryStore   = 305;
 const int hcLibraryClose   = 306;
 const int hcLibraryBackup  = 307;
 const int hcLibraryInfo    = 308;
 const int hcMenuBook       = 309;
 const int hcNewBook        = 310;
 const int hcSearchBook     = 311;
 const int hcMenuReaders    = 312;
 const int hcNewReader      = 313;
 const int hcSearchReader   = 314;
 const int hcMenuLoans      = 315;
 const int hcNewLoan        = 316; // not used
 const int hcSearchLoans    = 317;
 const int hcAboutKonias    = 318;
 const int hcMenuCd         = 319;
 const int hcNewCd          = 320;
 const int hcSearchCd       = 321;
 const int hcNewLoanOfBook  = 322;
 const int hcNewLoanToReader= 323;
 const int hcLibraryLoadAs  = 324;
 const int hcLibraryStoreAs = 325;
 const int hcLibraryRestore = 326;


 // - Common classes ----------------------------------------------------

 class TUnsortedCollection:public TStringCollection {
	public:
		TUnsortedCollection(short aLimit, short aDelta):
			TStringCollection(aLimit,aDelta) {
		}

		virtual int compare(void *key1, void *key2) {
			return -1;
		}
 };

 struct TListBoxRec {
	TCollection *collection;
	ushort selection;
 };

 // bundle - like -------------------------------------------------------

 #define K_TO_BORROW "K p…jŸen¡"
 #define K_BORROWED  "P…jŸen "
 #define K_RETIRED   "Vyýazen "

 #define K_EXTENSION "pac"
 #define K_BACKUP_NAME "A:\zaloha." K_EXTENSION

#endif
