#include "konias.h"

#include <ALLOC.H>
#include <DOS.H>
#include <STDIO.H>
#include <STRING.H>
#include <IO.H>
#include <FCNTL.H>
#include <SYS\STAT.H>
#include <STDLIB.H>

#include "booklist.h"
#include "kstatusl.h"

extern Konias *konias;
extern TPoint shadowSize;
extern KLoan globalLoan;

CriteriaDlgBoxData criteriaDlgBoxData;
FindBookData findBookData;

char libraryFile[120];

//----------------------------------------------------------------------------

Konias::Konias() :
    TProgInit(&Konias::initStatusLine,
	      &Konias::initMenuBar,
	      &Konias::initDeskTop){
    
    TRect r = getExtent();                      // Create the clock view.
    r.a.x = r.b.x - 9;      r.b.y = r.a.y + 1;
    clock = new TClockView( r );
    insert(clock);

#ifdef DEBUG
    r = getExtent();                            // Create the heap view.
    r.a.x = r.b.x - 13;     r.a.y = r.b.y - 1;
    heap = new THeapView( r );
    insert(heap);
#else
    r = getExtent();
    r.a.x = r.b.x - 13;     r.a.y = r.b.y - 1;
    actualLibraryView = new ActualLibraryView( r );
    insert(actualLibraryView);
#endif


    // disable
    TCommandSet ts;
    ts.enableCmd(cmNewCd);
    ts.enableCmd(cmFindCdCriteria);

    disableCommands(ts);

    strcpy(libraryFile,"_konias_.pac");

    // load data from the disk
    if(libraryFile!=NULL) {
	kLibrary.load(libraryFile);
    }
    else {
	// mark library as not loaded
	kLibrary.lastModified=0;
    }
}

void Konias::handleEvent(TEvent& event) {

    // handle exit
    if(event.message.command==cmQuit) {
        ushort status;
        if(status=messageBox("\003Uloßit knihovnu?",mfConfirmation|mfYesButton|mfNoButton)) {
            switch(status) {
                case cmYes:
                    quickStore();
                    break;
                case cmNo:
                    break;
                default:
                    return;
            }
        }
    }

    TApplication::handleEvent(event);
    if( event.what == evCommand ) {
	switch( event.message.command ) {
	    case cmNewReader:
		newReaderDlgBox();
		break;
	    case cmNewBook:
		newBookDlgBox();
		break;
	    case cmFindLoan:
		manageLoansDlgBox();
		break;
	    case cmNewLoan:
	    case cmFindBookCriteria:
		findBookCriteriaDlgBox();
		break;
	    case cmFindReaderCriteria:
		findReaderDlgBox();
		break;
	    case cmFindBook:
		findBookDlgBox();
		break;
	    case cmQuickStore:
		quickStore();
		break;
	    case cmQuickLoad:
		quickLoad();
		break;
	    case cmFreeKLibrary:
		konias->kLibrary.free();
                libraryFile[0]=NULL;
		break;
	    case cmGetLibraryInfo:
		actualLibraryInfo();
		break;
	    case cmAbout:
		aboutDlgBox();
		break;
	    case cmLoadLibAs:
		loadAs();
		break;
	    case cmStoreLibAs:
		storeAs();
		break;
	    case cmBackupLibrary:
		backupLibrary();
		break;
	    case cmRestoreLibrary:
		restoreLibrary();
		break;
	    case cmVideoMode:
		ushort newMode = TScreen::screenMode ^ TDisplay::smFont8x8;
		if ((newMode & TDisplay::smFont8x8) != 0) {
		    shadowSize.x = 1;
		}
		else {
		    shadowSize.x = 2;
		}
		setScreenMode(newMode);
		break;

	    default:
		return;
	}
	clearEvent( event );            // clear event after handling
    }
}

TMenuBar *Konias::initMenuBar(TRect r) {
    r.b.y = r.a.y + 1;    // set bottom line 1 line below top line

    return new TMenuBar(r,
			*new TSubMenu( "K~o~ni†Á", kbAltO, hcMenuKonias )+
			*new TMenuItem( "~V~ideo m¢d", cmVideoMode, kbNoKey, hcChangeVideoMode, "" ) +
			*new TMenuItem( "~O~ programu", cmAbout, kbNoKey, hcMenuKonias, "" )+
			newLine()+
			*new TMenuItem( "~K~onec", cmQuit, kbAltX, hcExitProgram, "Alt-x" )+
			*new TSubMenu( "K~n~ihovna", kbAltF, hcMenuLibrary )+
			*new TMenuItem( "~O~tev˝i", cmQuickLoad, kbF3, hcLibraryLoad, "F3" )+
			*new TMenuItem( "O~t~ev˝i jako...", cmLoadLibAs, kbAltF3, hcLibraryLoadAs, "Alt-F3" )+
			newLine()+
			*new TMenuItem( "~U~loß", cmQuickStore, kbF2, hcLibraryStore, "F2" )+
			*new TMenuItem( "Uloß ~j~ako...", cmStoreLibAs, kbAltF2, hcLibraryStoreAs, "Alt-F2" )+
			newLine()+
			*new TMenuItem( "Z†~l~ohuj", cmBackupLibrary, kbNoKey, hcLibraryBackup, "" )+
			*new TMenuItem( "O~b~nov", cmRestoreLibrary, kbNoKey, hcLibraryRestore, "" )+
			newLine()+
			*new TMenuItem( "~Z~av˝i", cmFreeKLibrary, kbAltF4, hcLibraryClose, "Alt-F4" )+
			*new TMenuItem( "~P~˝ehled", cmGetLibraryInfo, kbNoKey, hcLibraryInfo, "" )+
			*new TSubMenu( "¨~t~en†˝i", kbNoKey, hcMenuReaders )+
			*new TMenuItem( "~N~ovÏ üten†˝", cmNewReader, kbNoKey, hcNewReader, "" )+
			*new TMenuItem( "~H~led†n°", cmFindReaderCriteria, kbNoKey, hcSearchReader, "" )+
			*new TSubMenu( "~K~n°ßky", kbNoKey, hcMenuBook )+
			*new TMenuItem( "~N~ov† kn°ßka", cmNewBook, kbNoKey, hcNewBook, "" )+
			*new TMenuItem( "~H~led†n°", cmFindBookCriteria, kbNoKey, hcSearchBook, "" )+
			/*
			 *new TSubMenu( "~C~Düka", kbAltC, hcMenuCd )+
			 *new TMenuItem( "~N~ovÇ CDüko", cmNewCd, 0, hcNewCd, "" )+
			 *new TMenuItem( "~H~led†n°", cmFindCdCriteria, kbF3, hcSearchCd, "" )+
			 */
			*new TSubMenu( "~V~ÏpÖjüky", kbAltF, hcMenuLoans )+
			*new TMenuItem( "Nov† vÏpÖjüka ~k~n°ßky", cmFindBookCriteria, 0, hcNewLoanOfBook, "" )+
			//*new TMenuItem( "~N~ov† vÏpÖjüka üten†˝i", cmNewLoanToReader, 0, hcNewLoanToReader, "" )+
			//*new TMenuItem( "~P~˝ehled proÁlÏch vÏpÖjüek", cmFindLoan, 0, hcSearchLoans, "" )+
			*new TMenuItem( "~P~˝ehled vÏpÖjüek", cmFindLoan, kbNoKey, hcSearchLoans, "" )
			);
}

TStatusLine *Konias::initStatusLine(TRect r) {
    r.a.y = r.b.y - 1;     // move top to 1 line above bottom
    return new KStatusLine( r,
			    *new TStatusDef( 0, 0xFFFF )+
			    *new TStatusItem( 0, kbF10, cmMenu ) +
			    *new TStatusItem( "~Alt-X~ Konec", kbAltX, cmQuit )
			    );
}

void Konias::idle() {
    TProgram::idle();
    clock->update();
#ifdef DEBUG
    heap->update();
#else
    actualLibraryView->update();
#endif
}

void Konias::newReaderDlgBox() {
	KReader *r=new KReader((TApplication*)this);
	if(!kLibrary.addReader(r)) {
		delete r;
    }
}

void Konias::findReaderDlgBox() {
    kLibrary.findReaderDlgBox((TApplication*)this,FALSE);
}

void Konias::newBookDlgBox() {
    KBook *b=new KBook((TApplication*)this);
    if(!kLibrary.addBook(b)) {
        delete b;
    }
}

void Konias::findBookCriteriaDlgBox() {

    TDialog *dial = new TDialog( TRect( 0, 0, 37, 11), "Hled†n° kn°ßky" );

    if(dial) {
	dial->options|=ofCentered;
	dial->helpCtx=hcSearchBook;

	TView *b;
	b = new TRadioButtons( TRect(3, 2+1, 33, 2+5),
			       new TSItem( "~T~itul",
					   new TSItem( "~A~utor",
						       new TSItem( "~S~tav",
								   new TSItem( "~B~ibliografick† informace",
                                                                               0
                                                                               //     new TSItem( "~I~d", 0 )
									       )))));
	dial->insert( new TLabel( TRect(2, 1, 18, 2), "Vyber kritÇrium", b ));
	dial->insert( b );

	criteriaDlgBoxData.criteria=0;
	dial->setData(&criteriaDlgBoxData);

	// buttons
	dial->insert(new TButton( TRect( 10, 8, 20, 8+2 ),
				  "~H~ledej",
				  cmYes,
				  bfDefault));
	dial->insert( new TButton( TRect( 23, 8, 33, 8+2 ), "~S~torno", cmCancel,
				   bfNormal ));

	// set default selected
	b->select();

	// show & exec
	ushort retval=deskTop->execView( dial );
	if(retval==cmYes) {
	    dial->getData(&criteriaDlgBoxData);
	    //printf("Criteria: %i",boxData.criteria);
	    findBookDlgBox();
	}
    }

    destroy( dial );
}

class FindBookLoansButton : public TButton {

public:

    TInputLine *bookId;

    FindBookLoansButton(TInputLine *bookId,
			const TRect& bounds,
			const char *aTitle,
			ushort aCommand,
			ushort aFlags):
	TButton(bounds, aTitle, aCommand, aFlags) {
	this->bookId=bookId;
    }

    void press() {
	KLoan *kloan=konias->kLibrary.getLoanBySubject(atoi(bookId->data));
	if(kloan==NULL) {
	    messageBox("Tato kn°ßka nen° vypÖjüena!",mfInformation|mfOKButton);
	}
	else {
	    konias->kLibrary.manageLoans(kloan->readerId, konias);
	}
    }
};

void Konias::findBookDlgBox() {

    TDialog *dial = new TDialog( TRect( 0, 0, 76, 21), "Najdi kn°ßku" );

    if(dial) {
	dial->options|=ofCentered;
	dial->helpCtx=hcSearchBook;

	TView *b;

	/*
	// search edit line
	TInputLine *searcher= new TInputLine( TRect( 2, 2, 35, 3 ), 128 );
	dial->insert( searcher );
	dial->insert( new TLabel( TRect( 1, 1, 24, 2 ),
	"~H~ledej...",
	searcher ));
	*/
                            
			    TUnsortedCollection* listBoxData=new TUnsortedCollection(10,3);
                            
			    // Get book data from library
			    switch(criteriaDlgBoxData.criteria) {
				case 0: // name
				    listBoxData=kLibrary.getBooksByName();
				    break;
				case 1: // author
				    listBoxData=kLibrary.getBooksByAuthor();
				    break;
				case 2: // state
				    listBoxData=kLibrary.getBooksByState();
				    break;
				case 3: // bibInfo
				    listBoxData=kLibrary.getBooksByBibInfo();
				    break;
				case 4: // id
				    listBoxData=kLibrary.getBooksById();
				    break;
				default:
				    listBoxData=kLibrary.getBooksByAuthor();
			    }

			    // build scroll bar + list box
			    int listX=2;
			    int listXx=34;
			    int listY=4;
			    int listYy=16;

			    TScrollBar *myBar=new TScrollBar(TRect(listXx,listY,listXx+1,listYy));
			    dial->insert(myBar);

			    BookList* bookList=new BookList(TRect(listX,listY,listXx,listYy),
							    (ushort)1,
							    myBar);
			    bookList->findBook=dial;
			    bookList->newList(listBoxData);
			    dial->insert(bookList);
			    dial->insert( new TLabel( TRect( listX, listY-1, listX+24, listY ),
						      "K~n~°ßky",
						      bookList ));

			    // RIGHT SIDE: current book selected

			    int l=37; // shifted to right by...

			    TInputLine *ilAuthor, *ilName, *ilBibInfo, *ilBibRem, *ilId;

			    // author
			    ilAuthor = new TInputLine( TRect( l+3, 2, l+37, 3 ), 128 );
			    dial->insert( ilAuthor );
			    dial->insert( new TLabel( TRect( l+2, 1, l+24, 2 ),
						      "~A~utor",
						      ilAuthor ));
			    // name
			    ilName = new TInputLine( TRect( l+3, 5, l+37, 6 ), 128 );
			    dial->insert( ilName );
			    dial->insert( new TLabel( TRect( l+2, 4, l+24, 5 ),
						      "~T~itul",
						      ilName ));
			    // bibinfo
			    ilBibInfo = new TInputLine( TRect( l+3, 8, l+37, 9 ), 128 );
			    dial->insert( ilBibInfo );
			    dial->insert( new TLabel( TRect( l+2, 7, l+27, 8 ),
						      "~B~ibliografickÏ £daj",
						      ilBibInfo ));
			    // remark
			    ilBibRem = new TInputLine( TRect( l+3, 11, l+37, 12 ), 128 );
			    dial->insert( ilBibRem );
			    dial->insert( new TLabel( TRect( l+2, 10, l+27, 11 ),
						      "~P~ozn†mka",
						      ilBibRem ));
			    // id
			    ilId = new TInputLine( TRect( l+22, 14, l+22+6, 15 ), 128 );
			    dial->insert( ilId );
			    ilId->setState(sfDisabled,True);
			    dial->insert( new TLabel( TRect( l+22, 13, l+22+4, 14 ),
						      "~I~D:",
						      ilId ));

			    // status
			    b = new TRadioButtons( TRect( l+3, 14, l+18, 17),
						   new TSItem( "~K~ pÖjüen°",
							       new TSItem( "~P~Öjüen†",
									   new TSItem( "P~r~ezenün°", 0 )
									   )));
			    dial->insert( new TLabel( TRect( l+2, 13, l+8, 14), "~S~tav", b ));
			    dial->insert( b );

			    // RIGHT SIDE: done

			    // buttons
			    listX=l-16;
			    listXx=12;
			    listY=18;
			    listYy=2;

			    dial->insert( new TButton( TRect( listX, listY, listX+listXx, listY+listYy ), "~O~K", cmOK,
						       bfDefault ));
			    listX+=12;
			    dial->insert( new TButton( TRect( listX, listY, listX+listXx, listY+listYy ), "~S~mazat", cmDeleteBook,
                                                       bfNormal ));
			    listX+=12;
			    dial->insert( new FindBookLoansButton( ilId, TRect( listX, listY, listX+listXx, listY+listYy ), "VÏpÖ~j~üky", cmCancel,
								   bfNormal ));
			    listX+=12;
			    dial->insert( new TButton( TRect( listX, listY, listX+listXx+5, listY+listYy ), "~V~ypÖjüit >>", cmLoan,
						       bfNormal ));

			    // default focus
			    bookList->select();



			    // show & exec
			    ushort retval=deskTop->execView( dial );
			    if(retval==cmLoan) {

                                KBook *book=konias->kLibrary.getBookById(bookList->lastBook->id);
                                if(!book->canBeBorrowed()) {
                                    messageBox("Kn°ßka je jiß vypÖjüena!",mfError|mfOKButton);
                                }
                                else {
                                    // set book ID in global loan
                                    if(bookList->lastBook!=NULL) {
                                        globalLoan.subjectType=SUBJ_BOOK;
                                        globalLoan.subjectId=bookList->lastBook->id;
                                        kLibrary.findReaderDlgBox(this,TRUE);
                                    }
                                }
                            }

			    if(retval==cmDeleteBook) {
                                char bookIdString[10];
                                ilId->getData(bookIdString);
                                konias->kLibrary.killBook(konias->kLibrary.getBookById(atol(bookIdString)));
                                messageBox("Kn°ßka byla smaz†na!",mfInformation|mfOKButton);
			    }
                            
			    if(retval==cmOK) {
				// update current record from editline to library
				bookList->editLines2Book();
			    }
    }

    destroy( dial );
}

void Konias::aboutDlgBox() {

    TDialog *aboutBox = new TDialog(TRect(0, 0, 43, 9), "O Koni†Áovi");

    aboutBox->helpCtx=hcAboutKonias;

    aboutBox->insert(
		     new TStaticText(TRect(3, 2, 40, 8),
				     // The Ctrl-C centers the line.
				     "\003Pro \003Mar°nku\003 napsal Yulda\n\003\n"
				     "\003(Ver " KONIAS_VERSION ", Comp: " __DATE__ " " __TIME__ ")"
				     )
		     );

    aboutBox->insert(
		     new TButton(TRect(14+2, 6, 25+2, 8), " OK", cmOK, bfDefault)
		     );

    aboutBox->options |= ofCentered;

    deskTop->execView(aboutBox);

    destroy( aboutBox );
}

void Konias::actualLibraryInfo() {

    TDialog *infoBox = new TDialog(TRect(0, 0, 43, 11), "Profil otev˝enÇ knihovny");

    infoBox->options |= ofCentered;
    infoBox->helpCtx=hcLibraryInfo;
    
    if(kLibrary.lastModified==0) {
	infoBox->insert(new TStaticText(TRect(3, 2, 41, 6),
					"¶†dn† knihovna nen° otev˝ena!"));
    }
    else {
	infoBox->insert(new TStaticText(TRect(3, 2, 41, 7),
					kLibrary.getInfoString()));
    }

    infoBox->insert(
		    new TButton(TRect(17, 8, 27, 10), "~O~K", cmOK, bfDefault)
		    );

    deskTop->execView(infoBox);
    destroy( infoBox );
}

void Konias::manageLoansDlgBox() {
    kLibrary.manageLoans(-1, this);
}

void Konias::quickStore() {
    if(libraryFile[0]==NULL) {
        storeAs();
    }
    else {
        kLibrary.save(libraryFile);
        messageBox("Knihovna byla £spÿÁnÿ uloßena!",mfInformation|mfOKButton);
    }
}

void Konias::storeAs() {
    TFileDialog *d= 
		(TFileDialog *)validView(
                                         new TFileDialog( "*." K_EXTENSION, "Uloß knihovnu", "~J~mÇno", fdOpenButton, 100 ));
    
    if( d != 0 && deskTop->execView( d ) != cmCancel ) {
        char fileName[MAXPATH];
        d->getFileName( fileName );
        strcpy(libraryFile,fileName);

        kLibrary.save(fileName);
        messageBox("Knihovna byla £spÿÁnÿ uloßena!",mfInformation|mfOKButton);
    }
    destroy( d );
}

void Konias::quickLoad() {
    if(libraryFile[0]==NULL) {
        loadAs();
    }
    else {
        if(kLibrary.load(libraryFile)) {
            actualLibraryInfo();
        }
    }
}

void Konias::loadAs() {
    TFileDialog *d= 
        (TFileDialog *)validView(
                                 new TFileDialog( "*." K_EXTENSION, "Otev˝i knihovnu", "~J~mÇno", fdOpenButton, 100 ));
    
    if( d != 0 && deskTop->execView( d ) != cmCancel ) {
        char fileName[MAXPATH];
        d->getFileName( fileName );
        strcpy(libraryFile,fileName);

        if(kLibrary.load(libraryFile)) {
            actualLibraryInfo();
        }
    }
    destroy( d );
}

void Konias::backupLibrary() {
    if(kLibrary.save(K_BACKUP_NAME)) {
        messageBox("Knihovna byla £spÿÁnÿ uloßena!",mfInformation|mfOKButton);
    }
}

void Konias::restoreLibrary() {
    if(kLibrary.load(K_BACKUP_NAME)) {
        actualLibraryInfo();
    }
}

//- eof --------------------------------------------------------------------
