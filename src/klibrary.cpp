#include "klibrary.h"
#include "readlist.h"
#include "loanrlst.h"
#include "loanblst.h"

#include <ALLOC.H>
#include <FCNTL.H>
#include <SYS\STAT.H>
#include <DOS.H>
#include <STDLIB.H>
#include <STDIO.H>
#include <STRING.H>
#include <IO.H>

extern KLoan globalLoan;
extern Konias* konias;
extern char libraryFile[120];

//----------------------------------------------------------------------------

KLibrary::KLibrary() {
	books=NULL;
	readers=NULL;
	loans=NULL;

	readersLng=0;
	booksLng=0;
	loansLng=0;

	readersMaxId=1;
	booksMaxId=1;
	loansMaxId=1;
}

void KLibrary::free() {
	int i;

	if(books!=NULL) {
		for(i=0; i<booksLng; i++) {
			delete books[i];
		}

		farfree(books);

		books=NULL;
		booksLng=0;
		booksMaxId=0;
	}

	if(readers!=NULL) {
		for(i=0; i<readersLng; i++) {
			delete readers[i];
		}

		farfree(readers);

		readers=NULL;
		readersLng=0;
		readersMaxId=0;
	}

	if(loans!=NULL) {
		for(i=0; i<loansLng; i++) {
			delete loans[i];
		}

		farfree(loans);

		loans=NULL;
		loansLng=0;
		loansMaxId=0;
	}

	lastModified=0;
}

char* KLibrary::getInfoString() {

	struct date dt;
	struct time tm;

	unixtodos(lastModified,&dt,&tm);

	sprintf(msg,
				" Knihovna: %s\n"
                " Posledn¡ zmØna: %2d:%02d:%02d %u.%u.%u\n"
                " Kn¡§ek  : %d\n"
                " ¬ten ý… : %d\n"
                " Vìp…jŸek: %d\n",
                libraryFile,
                tm.ti_hour,
                tm.ti_min,
                tm.ti_sec,
                dt.da_day,
                dt.da_mon,
                dt.da_year,
                booksLng,
                readersLng,
                loansLng
                );
	return msg;
}

unsigned int KLibrary::getNextBookId() {
	return ++booksMaxId;
}

unsigned int KLibrary::getNextReaderId() {
	return ++readersMaxId;
}

unsigned int KLibrary::getNextLoanId() {
	return ++loansMaxId;
}

/**
 * Returns FALSE if the creation of user was canceled.
 */
boolean KLibrary::addBook(KBook *newBook) {
	if(newBook->dialogCanceled==TRUE) {
		return FALSE;
	}

	booksLng++;

	if(books==NULL) {
		// creating array of length 1
		books=(KBook**)farmalloc(sizeof(KBook*)*booksLng);
                // KBook** books=new KBook *[booksLng];
		books[0]=newBook;
	}
	else {
		books=(KBook**)farrealloc(books,sizeof(KBook*)*booksLng);
		books[booksLng-1]=newBook;

		#ifdef DEBUG
		int i;
		for(i=0; i<booksLng; i++) {
			printf("\nBook: %s",books[i]->name);
		}
		#endif
	}

        return TRUE;
}

void KLibrary::killBook(KBook *book) {
    if(books!=NULL) {
        // find the book
        for(int i=0; i<booksLng; i++) {
            if(books[i]==book) {
                // replace current book with the last one in the array
                delete book;
                books[i]=books[booksLng-1];
                booksLng--;
                if(booksLng!=0) {
                    books=(KBook**)farrealloc(books,sizeof(KBook*)*booksLng);
                } else {
                    farfree(books);
                    books=NULL;
                }
                
#ifdef DEBUG
                int d;
                for(d=0; d<booksLng; d++) {
                    printf("\nName %s",books[d]->name);
                }
#endif
                return;
            }
        }
    }
}

void KLibrary::killLoan(KLoan *loan) {
    if(loans!=NULL) {
        int i;
        for(i=0; i<loansLng; i++) {
            if(loans[i]==loan) {
                delete loan;

                if(i==0) {
                    farfree(loans);
                    loans=NULL;
                    loansLng=0;
                    loansMaxId=0;
                    return;
                }
                
                loans[i]=loans[loansLng-1];
                loansLng--;
                loans=(KLoan**)farrealloc(loans,sizeof(KLoan*)*loansLng);
                
#ifdef DEBUG
                int d;
                for(d=0; d<loansLng; d++) {
                    printf("\nId %d",loans[d]->id);
                }
#endif
                return;
            }
        }
    }
}

int compareByName( const void *a, const void *b) {
   return( strcmp((*((KBook**)a))->name,
				  (*((KBook**)b))->name));
}

TUnsortedCollection* KLibrary::getBooksByName() {
	// array must be shaked to be sorted according to name
	qsort(books, booksLng, sizeof(books[0]), compareByName);

	TUnsortedCollection* result=new TUnsortedCollection(10,3);

	if(books!=NULL) {

		int i;
		for(i=0; i<booksLng; i++) {
			result->insert(books[i]->name);
		}
	}

	return result;
}

int compareByAuthor( const void *a, const void *b) {
   return( strcmp((*((KBook**)a))->author,
				  (*((KBook**)b))->author));
}

TUnsortedCollection* KLibrary::getBooksByAuthor() {
	// array must be shaked to be sorted according to name
	qsort(books, booksLng, sizeof(books[0]), compareByAuthor);

	TUnsortedCollection* result=new TUnsortedCollection(10,3);

	if(books!=NULL) {

		int i;
		for(i=0; i<booksLng; i++) {
			result->insert(books[i]->author);
		}
	}

	return result;
}


int compareByState( const void *a, const void *b) {
   if((*((KBook**)a))->state == (*((KBook**)b))->state) {
		return 0;
   }
   return((*((KBook**)a))->state>(*((KBook**)b))->state?1:-1);
}

TUnsortedCollection* KLibrary::getBooksByState() {
	// array must be shaked to be sorted according to name
	qsort(books, booksLng, sizeof(books[0]), compareByState);

	TUnsortedCollection* result=new TUnsortedCollection(10,3);

	if(books!=NULL) {

		int i;
		for(i=0; i<booksLng; i++) {
			switch(books[i]->state) {
				case 0:
					result->insert(K_TO_BORROW);
					break;
				case 1:
					result->insert(K_BORROWED);
					break;
				case 2:
					result->insert(K_RETIRED);
					break;
			}
		}
	}

	return result;
}

int compareByBibInfo( const void *a, const void *b) {
    return( strcmp((*((KBook**)a))->bibInfo,
                   (*((KBook**)b))->bibInfo));
}

TUnsortedCollection* KLibrary::getBooksByBibInfo() {
	// array must be shaked to be sorted according to name
	qsort(books, booksLng, sizeof(books[0]), compareByBibInfo);

	TUnsortedCollection* result=new TUnsortedCollection(10,3);

	if(books!=NULL) {

		int i;
		for(i=0; i<booksLng; i++) {
			result->insert(books[i]->bibInfo);
		}
	}

	return result;
}

int compareById( const void *a, const void *b) {
    return((*((KBook**)a))->id>(*((KBook**)b))->id?0:1);
}

TUnsortedCollection* KLibrary::getBooksById() {
	// array must be shaked to be sorted according to name
	qsort(books, booksLng, sizeof(books[0]), compareById);

	TUnsortedCollection* result=new TUnsortedCollection(10,3);

	if(books!=NULL) {

		int i;
		for(i=0; i<booksLng; i++) {
			result->insert(ltoa(books[i]->id,
                                            (char*)malloc(7),
                                            10));
		}
	}
        
	return result;
}

KBook* KLibrary::getBook(unsigned int i) {
	if(books==NULL || i>=booksLng) {
		return NULL;
	}
	else {
		return books[i];
	}
}

KBook* KLibrary::getBookById(unsigned int i) {

	for(int j=0; j<booksLng; j++) {
		if(i==books[j]->id) {
			return books[j];
		}
	}

	return NULL;
}

KReader* KLibrary::getReader(unsigned int i) {
	if(readers==NULL || i>=readersLng) {
		return NULL;
	}
	else {
		return readers[i];
	}
}

KReader* KLibrary::getReaderById(unsigned int i) {

	for(int j=0; j<readersLng; j++) {
		if(i==readers[j]->id) {
			return readers[j];
		}
	}

	return NULL;
}

/**
 * Returns FALSE if the creation of user was canceled.
 */
boolean KLibrary::addReader(KReader *newReader) {
	if(newReader->dialogCanceled==TRUE) {
            return FALSE;
	}

	readersLng++;

	if(readers==NULL) {
		// creating array of length 1
		readers=(KReader**)farmalloc(sizeof(KReader*)*readersLng);
		readers[0]=newReader;
	}
	else {
		readers=(KReader**)farrealloc(readers,sizeof(KReader*)*readersLng);
		readers[readersLng-1]=newReader;

		#ifdef DEBUG
		int i;
		for(i=0; i<readersLng; i++) {
			printf("\nName: %s, Id: %d, Lng %d, Max %d",readers[i]->firstname,readers[i]->id, readersLng, readersMaxId);
		}
		#endif
	}

	return TRUE;
}

void KLibrary::killReader(KReader *reader) {
    if(readers!=NULL && reader!=NULL) {
        // find the reader
        int i;
        for(i=0; i<readersLng; i++) {
            if(readers[i]==reader) {
                delete reader;
                readers[i]=readers[readersLng-1];
                readersLng--;
                if(readersLng!=0) {
                    readers=(KReader**)farrealloc(readers,sizeof(KReader*)*readersLng);
                } else {
                    farfree(readers);
                    readers=NULL;
                }
                
#ifdef DEBUG
                int d;
                for(d=0; d<readersLng; d++) {
                    printf("\nReader: %s",readers[d]->surname);
                }
#endif
                return;
            }
        }
    }
}



int cmpReaders( const void *a, const void *b) {
   return( strcmp((*((KReader**)a))->surname,
				  (*((KReader**)b))->surname));
}

TUnsortedCollection* KLibrary::getReaders() {
	TUnsortedCollection *t;
	return getReaders(t);
}

TUnsortedCollection* KLibrary::getReaders(TUnsortedCollection *&readerIds) {
	// array must be shaked to be sorted according to name
	qsort(readers, readersLng, sizeof(readers[0]), cmpReaders);

	TUnsortedCollection* result=new TUnsortedCollection(10,3);
	readerIds=new TUnsortedCollection(10,3);

	if(readers!=NULL) {
		int i;
		size_t lng;
		char *longname, *s;

		for(i=0; i<readersLng; i++) {
			lng=strlen(readers[i]->firstname)+
				strlen(readers[i]->surname)
				+2;
			longname=(char*)malloc(lng);
			strcpy(longname,readers[i]->surname);
			strcat(longname," ");
			strcat(longname,readers[i]->firstname);
			result->insert(longname);

			s=new char[10];
			ltoa(readers[i]->id,s,10);
			readerIds->insert(s);
		}
	}

	return result;
}

class FindReaderLoansButton : public TButton {

	public:

		TInputLine *readerId;

		FindReaderLoansButton(TInputLine *readerId,
							  const TRect& bounds,
							  const char *aTitle,
							  ushort aCommand,
							  ushort aFlags):
			TButton(bounds, aTitle, aCommand, aFlags) {
			this->readerId=readerId;
		}

		void press() {
			konias->kLibrary.manageLoans(atoi(readerId->data), konias);
		}
};

void KLibrary::findReaderDlgBox(TApplication* app, boolean finishLoan) {
	TDialog *dial = new TDialog( TRect( 0, 0, 76, 21), "Najdi Ÿten ýe" );

	if(dial) {
		dial->options|=ofCentered;
		dial->helpCtx=hcSearchReader;

		TView *b;
		TUnsortedCollection* listBoxData=getReaders();

		// build scroll bar + list box
		int listX=2;
		int listXx=34;
		int listY=4;
		int listYy=16;

		TScrollBar *myBar=new TScrollBar(TRect(listXx,listY,listXx+1,listYy));
		dial->insert(myBar);

		ReaderList* readerList=new ReaderList(TRect(listX,listY,listXx,listYy),
											  (ushort)1,
											  myBar);
		readerList->findReader=dial;
		readerList->newList(listBoxData);
		dial->insert(readerList);
		dial->insert( new TLabel( TRect( listX, listY-1, listX+24, listY ),
								  "¬te~n~ ýi",
								  readerList ));

		// RIGHT SIDE: current book selected

		int l=37; // shifted to right by...

		TInputLine *ilAuthor;
		TInputLine *ilName;
		TInputLine *ilBibInfo, *ilBibRem;

		ilAuthor = new TInputLine( TRect( l+3, 2, l+37, 3 ), 128 );
		dial->insert( ilAuthor );
		dial->insert( new TLabel( TRect( l+2, 1, l+24, 2 ),
								  "~J~m‚no",
								  ilAuthor ));

		ilName = new TInputLine( TRect( l+3, 5, l+37, 6 ), 128 );
		dial->insert( ilName );
		dial->insert( new TLabel( TRect( l+2, 4, l+24, 5 ),
								  "~P~ý¡jmen¡",
								  ilName ));

		ilBibInfo = new TInputLine( TRect( l+3, 8, l+37, 9 ), 128 );
		dial->insert( ilBibInfo );
		dial->insert( new TLabel( TRect( l+2, 7, l+27, 8 ),
								  "~E~mail",
								  ilBibInfo ));

		ilBibRem = new TInputLine( TRect( l+3, 11, l+20+9, 12 ), 128 );
		dial->insert( ilBibRem );
		dial->insert( new TLabel( TRect( l+2, 10, l+27, 11 ),
								  "~T~elef‚n",
								  ilBibRem ));

		TInputLine *ilId = new TInputLine( TRect( l+22+9, 11, l+28+9, 12 ), 128 );
		ilId->setState(sfDisabled,True);
		dial->insert( ilId );
		dial->insert( new TLabel( TRect( l+22+8, 10, l+26+9, 11 ),
								  "~I~D:",
								  ilId ));

		b = new TInputLine( TRect( l+3, 14, l+37, 15 ), 128 );
		dial->insert( b );
		dial->insert( new TLabel( TRect( l+2, 13, l+24, 14 ),
								  "~A~dresa:",
								  b ));

		// RIGHT SIDE: done

		// buttons
                if(finishLoan==TRUE) {
                    dial->insert(new TButton( TRect( l+2, 16, l+17, 18 ), "~V~yp…jŸit >>", 
                                              cmFinishLoan,
                                              bfDefault ));
		}

		listX=l+2;
		listXx=12;
		listY=18;
		listYy=2;
                
		dial->insert( new FindReaderLoansButton( ilId, TRect( listX, listY, listX+listXx, listY+listYy ), "Vìp…jŸ~k~y", 
                                                         cmCancel,
                                                         bfNormal ));
		listX+=12;
		dial->insert( new TButton( TRect( listX, listY, listX+listXx, listY+listYy ), "~S~mazat", 
                                           cmDeleteReader,
                                           bfNormal ));
		listX+=12;
                if(finishLoan==TRUE) {
                    dial->insert( new TButton( TRect( listX, listY, listX+listXx-1, listY+listYy ), "~O~K", 
                                               cmOK,
                                               bfNormal ));
                }
                else {
                    dial->insert( new TButton( TRect( listX, listY, listX+listXx-1, listY+listYy ), "~O~K", 
                                               cmOK,
                                               bfDefault ));
                }

		// default focus
                readerList->select();

		// show & exec
		ushort retval=app->deskTop->execView( dial );

		if(retval==cmOK) {
			// update current record from editline to library
			readerList->editLines2Book();
		}

		if(retval==cmFinishLoan) {
			globalLoan.readerId=readerList->lastReader->id;
			addLoan(new KLoan(globalLoan.readerId,
							  globalLoan.subjectId,
							  globalLoan.subjectType,
							  konias));
		}

		if(retval==cmDeleteReader) {
                    char readerIdString[10];
                    ilId->getData(readerIdString);
                    konias->kLibrary.killReader(konias->kLibrary.getReaderById(atol(readerIdString)));
                    messageBox("¬ten ý byl smaz n!",mfInformation|mfOKButton);
                }
	}

	app->destroy( dial );
}

void KLibrary::addLoan(KLoan *newLoan) {
	loansLng++;

	if(loans==NULL) {
		// creating array of length 1
		loans=(KLoan**)farmalloc(sizeof(KLoan*)*loansLng);
		loans[0]=newLoan;
	}
	else {
		loans=(KLoan**)farrealloc(loans,sizeof(KLoan*)*loansLng);
		loans[loansLng-1]=newLoan;

		#ifdef DEBUG
		int i;
		for(i=0; i<loansLng; i++) {
			printf("\nReader %d",loans[i]->readerId);
		}
		#endif
	}
}

boolean KLibrary::save(char *fileName) {
	int i, handle, length, res;

	if((handle = open(fileName,
					  O_WRONLY | O_CREAT | O_TRUNC | O_BINARY,
					  S_IREAD | S_IWRITE)) == -1) {
		messageBox("Nemohu ulo§it knihovnu!",mfError|mfOKButton);
		return FALSE;
	}

	// time stamps
		struct date dt;
		struct time tm;
		getdate(&dt);
		gettime(&tm);
		lastModified = dostounix(&dt, &tm);
		write(handle, &lastModified, sizeof(long));

	// IDs to use
		write(handle, &booksMaxId, sizeof(unsigned int));
		write(handle, &readersMaxId, sizeof(unsigned int));
		write(handle, &loansMaxId, sizeof(unsigned int));

	// readers
		// number of records
		write(handle, &readersLng, sizeof(unsigned int));

		if(readers!=NULL) {
			for(i=0; i<readersLng; i++) {
				readers[i]->save(handle);
			}
		}

	// books
		// number of records
		write(handle, &booksLng, sizeof(unsigned int));

		if(books!=NULL) {
			for(i=0; i<booksLng; i++) {
				books[i]->save(handle);
			}
		}

	// loans
		// number of records
		write(handle, &loansLng, sizeof(unsigned int));

		if(loans!=NULL) {
			for(i=0; i<loansLng; i++) {
				loans[i]->save(handle);
			}
		}

	close(handle);

        return TRUE;
}

boolean KLibrary::load(char *fileName) {
	int i, handle, length, res;

	// make clean

	free();


	if ((handle=open(fileName,
					 O_RDONLY | O_BINARY,
					 S_IWRITE | S_IREAD)) == -1) {
		messageBox("Nemohu otevý¡t knihovnu!",mfError|mfOKButton);
		return FALSE;
	}

	// time stamps
		read(handle, &lastModified, sizeof(long));

	// IDs to use
		read(handle, &booksMaxId, sizeof(unsigned int));
		read(handle, &readersMaxId, sizeof(unsigned int));
		read(handle, &loansMaxId, sizeof(unsigned int));

	// readers
		// number of records
		read(handle, &readersLng, sizeof(unsigned int));

		if(readersLng>0) {
			readers=(KReader**)malloc(sizeof(KReader*)*readersLng);

			for(i=0; i<readersLng; i++) {
				readers[i]=new KReader();
				readers[i]->load(handle);
			}
		}

	// books
		// number of records
		read(handle, &booksLng, sizeof(unsigned int));

		if(booksLng>0) {
			books=(KBook**)malloc(sizeof(KBook*)*booksLng);

			for(i=0; i<booksLng; i++) {
				books[i]=new KBook();
				books[i]->load(handle);
			}
		}

	// loans
		// number of records
		read(handle, &loansLng, sizeof(unsigned int));

		if(loansLng>0) {
			loans=(KLoan**)malloc(sizeof(KLoan*)*loansLng);

			for(i=0; i<loansLng; i++) {
				loans[i]=new KLoan();
				loans[i]->load(handle);
			}
		}

	close(handle);
        return TRUE;
}

char stringBuffer[128];
char stringNumber[25];

#include "kbuttons.cpp"

void KLibrary::manageLoans(long readerId, TApplication* app) {

	TDialog *dial = new TDialog( TRect( 1, 1, 77, 22), "Vìp…jŸky" );

	if(dial) {
		dial->options|=ofCentered;
		dial->helpCtx=hcNewLoan;

		TView *b;
		int listX, listXx, listY, listYy;

		// list of loans
		listX=2;
		listXx=58;
		listY=9;
		listYy=14;

		TScrollBar *myBar2=new TScrollBar(TRect(listXx,listY,listXx+1,listYy));
		dial->insert(myBar2);
		LoanBooksList* loanBooksList=new LoanBooksList(TRect(listX,listY,listXx,listYy),
													   (ushort)1,
													   myBar2);

		dial->insert(loanBooksList);
		dial->insert( new TLabel( TRect( listX, listY-1, listX+24, listY ),
								  "¬ten ýovy ~v~ìp…jŸky:",
								  loanBooksList ));

		// list of readers
		listX=2;
		listXx=58;
		listY=2;
		listYy=7;

		TScrollBar *myBar=new TScrollBar(TRect(listXx,listY,listXx+1,listYy));
		dial->insert(myBar);

		LoanReadersList* loanReadersList=new LoanReadersList(TRect(listX,listY,listXx,listYy),
															 (ushort)1,
															 myBar);
		loanReadersList->newList(getReaders(loanReadersList->readerIds));
		loanReadersList->selectReaderWithId(readerId);
		dial->insert(loanReadersList);
		dial->insert( new TLabel( TRect( listX, listY-1, listX+24, listY ),
								  "¬~t~en ý:",
								  loanReadersList ));

		// list of readers must know the list of loans
		loanReadersList->loanBooksList=loanBooksList;



		// overall information on loans
		dial->insert( new TLabel( TRect( 1, 18, 75, 19 ),
								  "________________________________________________________________________",
								  NULL));
		strcpy(stringBuffer,"Celkem vìp…jŸek: ");
		itoa(loansLng,stringNumber,10);
		strcat(stringBuffer,stringNumber);
		dial->insert( new TStaticText( TRect( 3, 19, 30, 20 ),
								  stringBuffer));

		// buttons
		listXx=14;
		listY=2;
		dial->insert( new LibraryAboutReaderButton(
						loanReadersList,
						TRect( 60, listY, 60+listXx, listY+2 ),
						"~O~ Ÿten ýi",
						cmAboutReader,
						bfNormal ));

		listY=9;
		dial->insert( new LibraryLoanAboutButton(
						loanReadersList,
						loanBooksList,
						TRect( 60, listY, 60+listXx, listY+2 ),
						"O vì~p~…jŸce",
						cmAboutLoan,
						bfNormal ));

		listY+=7;
		dial->insert( new TButton( TRect( 46, listY, 46+listXx, listY+2 ), "O~K~", cmOK,
					bfDefault ));
		dial->insert( new TButton( TRect( 60, listY, 60+listXx, listY+2 ), "~S~torno", cmCancel,
					bfNormal ));



		// default focus
		loanReadersList->select();

		// show & exec
		ushort retval=app->deskTop->execView( dial );

		if(retval==cmOK) {
			// add new loan to structures
			//dial->getData(&newKLoanDialog);
		}
	}

	app->destroy( dial );
}

KLoan* KLibrary::getLoanById(unsigned int i) {

	for(int j=0; j<loansLng; j++) {
		if(i==loans[j]->id) {
			return loans[j];
		}
	}

	return NULL;
}

TUnsortedCollection* KLibrary::getLoansByReader(unsigned int readerId,
												TUnsortedCollection *&loanIds) {

	TUnsortedCollection* result=new TUnsortedCollection(10,3);

	if(loans!=NULL) {
		unsigned int i;
		char *name;
		KBook *kbook;
		char *s;

		loanIds=new TUnsortedCollection(10,3);

		for(i=0; i<loansLng; i++) {
			if(loans[i]->readerId == readerId) {
				kbook=getBookById(loans[i]->subjectId);
				if(kbook!=NULL) {
					name=new char[strlen(kbook->name)+1+(loans[i]->returned?10:0)];
					strcpy(name,kbook->name);
					if(loans[i]->returned) {
						strcat(name," (vr cena)");
					}
					result->insert(name);

									// agatka

					s=new char[10];
					ltoa(loans[i]->id,s,10);
					loanIds->insert(s);
				}
			}
		}
	}

	return result;

}

KLoan *KLibrary::getLoanBySubject(unsigned int subjectId) {

	for(int j=0; j<loansLng; j++) {
		if(subjectId==loans[j]->subjectId) {
			return loans[j];
		}
	}

	return NULL;

}

//- eof ----------------------------------------------------------------------
