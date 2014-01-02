#include "kloan.h"

#include "konias.h"

#include <dos.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern Konias *konias;

// loan about to reused by other modules (e.g. when creating one)
KLoan globalLoan;

char who[128];
char what[128];

struct NewKLoanDialog {
    char fromD[4];
    char fromM[4];
    char fromY[6];

    char toD[4];
    char toM[4];
    char toY[6];
} newKLoanDialog;

struct EditKLoanDialog {
    char fromD[4];
    char fromM[4];
    char fromY[6];

    char toD[4];
    char toM[4];
    char toY[6];

    unsigned int status;
} editKLoanDialog;

extern Konias *konias;

// methods ---------------------------------------------------------------

KLoan::KLoan() {
    id=0;
    subjectType=SUBJ_VOID;
    subjectId=0;
    readerId=0;

    from.da_day=0;
    from.da_mon=0;
    from.da_year=0;

    to.da_day=0;
    to.da_mon=0;
    to.da_year=0;

    returned=FALSE;
}

KLoan::KLoan(long readerId,
             long subjectId,
             LoanSubject subjectType,
             TApplication *app) {

    this->readerId=readerId;
    this->subjectId=subjectId;
    this->subjectType=subjectType;
    this->returned=FALSE;

    // show dialog
    this->app=app;

    TDialog *dial = new TDialog( TRect( 20, 1, 73, 17), "Nov  vìp…jŸka" );

    if(dial) {
        dial->options|=ofCentered;
        dial->helpCtx=hcNewLoan;

        TView *b;

        strcpy(who,
               konias->kLibrary.getReaderById(globalLoan.readerId)->surname);
        if(strcmp("",who)!=0) {
            strcat(who,
                   " ");
        }
        strcat(who,
               konias->kLibrary.getReaderById(globalLoan.readerId)->firstname);
        b = new TStaticText( TRect( 3+2, 2, 50, 3 ), who);
        dial->insert( b );
        if(globalLoan.subjectType==SUBJ_BOOK) {
            dial->insert( new TLabel( TRect( 2, 1, 24, 2 ),
                                      "~¬ten ý~",
                                      b ));
        } else {
            dial->insert( new TLabel( TRect( 2, 1, 24, 2 ),
                                      "~PosluchaŸ~",
                                      b ));
        }



        if(globalLoan.subjectType==SUBJ_BOOK) {
            strcpy(what,
                   konias->kLibrary.getBookById(globalLoan.subjectId)->author);
            strcat(what,
                   ": ");
            strcat(what,
                   konias->kLibrary.getBookById(globalLoan.subjectId)->name);

            b = new TStaticText( TRect( 3+2, 5, 50, 6 ), what );
            dial->insert( b );
            dial->insert( new TLabel( TRect( 2, 4, 24, 5 ),
                                      "~Kn¡§ka~",
                                      b ));
        }
        else {
            strcpy(what,
                   konias->kLibrary.getBookById(globalLoan.subjectId)->name);

            b = new TStaticText( TRect( 3+2, 5, 50, 6 ), what );
            dial->insert( b );
            dial->insert( new TLabel( TRect( 2, 4, 24, 5 ),
                                      "~CDŸko~",
                                      b ));
        }


        b=new TInputLine( TRect( 3, 8, 5+3, 9 ), 4 );
        dial->insert(b);
        dial->insert(new TStaticText( TRect( 5+3, 8, 6+3, 9 ), "."));
        dial->insert(new TLabel( TRect( 2, 7, 2+10, 8 ),"~O~d",b ));
        dial->insert(new TInputLine( TRect( 7+3, 8, 9+6, 9 ), 4 ));
        dial->insert(new TStaticText( TRect( 9+6, 8, 10+6, 9 ), "."));
        dial->insert(new TInputLine( TRect( 11+6, 8, 15+9, 9 ), 6 ));

        b=new TInputLine( TRect( 3, 8+3, 5+3, 9+3 ), 4 );
        dial->insert(b);
        dial->insert(new TStaticText( TRect( 5+3, 8+3, 6+3, 9+3 ), "."));
        dial->insert(new TLabel( TRect( 2, 7+3, 2+10, 8+3 ),"~D~o",b ));
        dial->insert(new TInputLine( TRect( 7+3, 8+3, 9+6, 9+3 ), 4 ));
        dial->insert(new TStaticText( TRect( 9+6, 8+3, 10+6, 9+3 ), "."));
        dial->insert(new TInputLine( TRect( 11+6, 8+3, 15+9, 9+3 ), 6 ));

        // buttons
        b=new TButton( TRect( 27, 13, 39, 15 ), "Do~k~onŸit", cmOK, bfDefault );
        dial->insert(b);
        dial->insert( new TButton( TRect( 41, 13, 51, 15 ), "~S~torno", cmCancel,
                                   bfNormal ));

        b->select();

        struct date d;
        getdate(&d);
        sprintf(newKLoanDialog.fromD,"%d",d.da_day);
        sprintf(newKLoanDialog.fromM,"%d",d.da_mon);
        sprintf(newKLoanDialog.fromY,"%d",d.da_year);

        // invent next month
        if(++d.da_mon>12) {
            d.da_mon=1;
            d.da_year++;
        }

        sprintf(newKLoanDialog.toD,"%d",d.da_day);
        sprintf(newKLoanDialog.toM,"%d",d.da_mon);
        sprintf(newKLoanDialog.toY,"%d",d.da_year);

        dial->setData(&newKLoanDialog);

        // show & exec
        ushort retval=app->deskTop->execView( dial );

        // finishing the loan
        if(retval==cmOK) {
            // add new loan to structures
            dial->getData(&newKLoanDialog);

            KBook *book=konias->kLibrary.getBookById(subjectId);
            if(book->borrow()) {
                // only datums are loaded
                from.da_day=(char)atoi(newKLoanDialog.fromD);
                from.da_mon=(char)atoi(newKLoanDialog.fromM);
                from.da_year=atoi(newKLoanDialog.fromY);
                
                to.da_day=(char)atoi(newKLoanDialog.toD);
                to.da_mon=(char)atoi(newKLoanDialog.toM);
                to.da_year=atoi(newKLoanDialog.toY);
                
                // get new loan ID
                id=konias->kLibrary.getNextLoanId();                
            }
        }
        
        app->destroy( dial );
    }
}

class LoanAboutReaderButton : public TButton {

public:

    KReader* kreader;

    LoanAboutReaderButton(KReader* kreader,
                          const TRect& bounds,
                          const char *aTitle,
                          ushort aCommand,
                          ushort aFlags):
        TButton(bounds, aTitle, aCommand, aFlags) {
        this->kreader=kreader;
    }

    void press() {
        kreader->show();
    }
};

class LoanAboutBookButton : public TButton {

public:

    KBook* kbook;

    LoanAboutBookButton(KBook* kbook,
                        const TRect& bounds,
                        const char *aTitle,
                        ushort aCommand,
                        ushort aFlags):
        TButton(bounds, aTitle, aCommand, aFlags) {
        this->kbook=kbook;
    }

    void press() {
        kbook->show();
    }
};

/**
 * Edit this loan
 */
void KLoan::edit() {
    TDialog *dial = new TDialog( TRect( 20, 1, 73, 17), "Editace vìp…jŸky" );

    if(dial) {
        dial->options|=ofCentered;
        dial->helpCtx=hcNewLoan;

        TView *b;

        strcpy(who,
               konias->kLibrary.getReaderById(readerId)->surname);
        if(strcmp("",who)!=0) {
            strcat(who,
                   " ");
        }
        strcat(who,
               konias->kLibrary.getReaderById(readerId)->firstname);
        b = new TStaticText( TRect( 3+2, 2, 50, 3 ), who);
        dial->insert( b );


        if(subjectType==SUBJ_BOOK) {
            dial->insert( new TLabel( TRect( 2, 1, 24, 2 ),
                                      "¬ten ý",
                                      NULL ));
        } else {
            dial->insert( new TLabel( TRect( 2, 1, 24, 2 ),
                                      "PosluchaŸ",
                                      NULL ));
        }

        if(subjectType==SUBJ_BOOK) {
            strcpy(what,
                   konias->kLibrary.getBookById(subjectId)->author);
            strcat(what,
                   ": ");
            strcat(what,
                   konias->kLibrary.getBookById(subjectId)->name);

            b = new TStaticText( TRect( 3+2, 5, 50, 6 ), what );
            dial->insert( b );
            dial->insert( new TLabel( TRect( 2, 4, 24, 5 ),
                                      "Kn¡§ka",
                                      NULL ));
        }
        else {
            strcpy(what,
                   konias->kLibrary.getBookById(subjectId)->name);

            b = new TStaticText( TRect( 3+2, 5, 50, 6 ), what );
            dial->insert( b );
            dial->insert( new TLabel( TRect( 2, 4, 24, 5 ),
                                      "~CDŸko~",
                                      b ));
        }


        b=new TInputLine( TRect( 3, 8, 5+3, 9 ), 4 );
        dial->insert(b);
        dial->insert(new TStaticText( TRect( 5+3, 8, 6+3, 9 ), "."));
        dial->insert(new TLabel( TRect( 2, 7, 2+10, 8 ),"~O~d",b ));
        dial->insert(new TInputLine( TRect( 7+3, 8, 9+6, 9 ), 4 ));
        dial->insert(new TStaticText( TRect( 9+6, 8, 10+6, 9 ), "."));
        dial->insert(new TInputLine( TRect( 11+6, 8, 15+9, 9 ), 6 ));

        b=new TInputLine( TRect( 3, 8+3, 5+3, 9+3 ), 4 );
        dial->insert(b);
        dial->insert(new TStaticText( TRect( 5+3, 8+3, 6+3, 9+3 ), "."));
        dial->insert(new TLabel( TRect( 2, 7+3, 2+10, 8+3 ),"~D~o",b ));
        dial->insert(new TInputLine( TRect( 7+3, 8+3, 9+6, 9+3 ), 4 ));
        dial->insert(new TStaticText( TRect( 9+6, 8+3, 10+6, 9+3 ), "."));
        dial->insert(new TInputLine( TRect( 11+6, 8+3, 15+9, 9+3 ), 6 ));

        // status
        b = new TCheckBoxes( TRect( 30, 11, 30+15, 11+1),
                             new TSItem( "~V~r cena",0));
        dial->insert( b );
        dial->insert( new TLabel(TRect(29,10,29+10,10+1),"Stav",b));
        editKLoanDialog.status=returned;

        // buttons
        int posX, posXx, posY, posYy;
        posX=38;
        posXx=13;
        posY=3;
        posYy=2;
        dial->insert( new LoanAboutReaderButton( konias->kLibrary.getReaderById(readerId), TRect( posX, posY, posX+posXx, posY+posYy ), "~O~ Ÿten ýi", cmAboutReader,
                                                 bfNormal ));
        posY+=3;
        dial->insert( new LoanAboutBookButton( konias->kLibrary.getBookById(subjectId), TRect( posX, posY, posX+posXx, posY+posYy ), "O ~k~n¡§ce", cmOK,
                                               bfNormal ));


        dial->insert( new TButton( TRect( 27-14+4, 13, 39-14+4, 15 ), "~U~pravit", cmOK,
                                   bfDefault ));
        dial->insert( new TButton( TRect( 27+2, 13, 39+2, 15 ), "S~m~azat", cmDeleteLoan,
                                   bfNormal ));
        dial->insert( new TButton( TRect( 41, 13, 51, 15 ), "~S~torno", cmCancel,
                                   bfNormal ));

        b->select();

        struct date d;
        getdate(&d);
        sprintf(editKLoanDialog.fromD,"%d",d.da_day);
        sprintf(editKLoanDialog.fromM,"%d",d.da_mon);
        sprintf(editKLoanDialog.fromY,"%d",d.da_year);

        // invent next month
        if(++d.da_mon>12) {
            d.da_mon=1;
            d.da_year++;
        }

        sprintf(editKLoanDialog.toD,"%d",d.da_day);
        sprintf(editKLoanDialog.toM,"%d",d.da_mon);
        sprintf(editKLoanDialog.toY,"%d",d.da_year);

        dial->setData(&editKLoanDialog);

        // show & exec
        ushort retval=app->deskTop->execView( dial );

        // modify the loan
        if(retval==cmOK) {
            // modify this loan
            dial->getData(&editKLoanDialog);

            // only datums are loaded
            from.da_day=(char)atoi(editKLoanDialog.fromD);
            from.da_mon=(char)atoi(editKLoanDialog.fromM);
            from.da_year=atoi(editKLoanDialog.fromY);

            to.da_day=(char)atoi(editKLoanDialog.toD);
            to.da_mon=(char)atoi(editKLoanDialog.toM);
            to.da_year=atoi(editKLoanDialog.toY);

            returned=editKLoanDialog.status;

			// mark book as borrowed/free acording to status (add method)
            KBook *book=konias->kLibrary.getBookById(subjectId);
            if(returned) {
                // avoid making retired books free
                if(book->state!=BOOK_BORROWED) {
                    book->state=BOOK_FREE;
                }
            }
            else {
                if(book->state!=BOOK_FREE) {
					messageBox("Kn¡§ka nem…§e bìt vyp…jŸena - je buÔ vyýazena, nebo ji§ vyp…jŸena.",mfConfirmation|mfOKButton);
                } else {
					book->state=BOOK_BORROWED;
				}
			}
		}

		if(retval==cmDeleteLoan) {
			KBook *book=konias->kLibrary.getBookById(subjectId);

			// mark book as free
			if(book->state==BOOK_BORROWED) {
				if(messageBox("Kn¡§ka je vìp…jŸen  - m  bìt nyn¡ vr cena?",mfConfirmation|mfYesButton|mfNoButton)==cmYes) {
#ifdef DEBUG
					puts("Returning book...");
#endif
					book->state=BOOK_FREE;
				}
			}

			konias->kLibrary.killLoan(this);
		}

		app->destroy( dial );
	}
}

void KLoan::free() {
	// does nothing :-)
}

void KLoan::save(int handle) {
    write(handle,&id,sizeof(long));

    write(handle,&subjectType,sizeof(int));

    write(handle,&subjectId,sizeof(long));
    write(handle,&readerId,sizeof(long));

    write(handle,&(from.da_day),sizeof(char));
    write(handle,&(from.da_mon),sizeof(char));
    write(handle,&(from.da_year),sizeof(int));

    write(handle,&(to.da_day),sizeof(char));
    write(handle,&(to.da_mon),sizeof(char));
    write(handle,&(to.da_year),sizeof(int));
}

void KLoan::load(int handle) {
    read(handle,&id,sizeof(long));

    read(handle,&subjectType,sizeof(int));

    read(handle,&subjectId,sizeof(long));
    read(handle,&readerId,sizeof(long));

    read(handle,&(from.da_day),sizeof(char));
    read(handle,&(from.da_mon),sizeof(char));
    read(handle,&(from.da_year),sizeof(int));

    read(handle,&(to.da_day),sizeof(char));
    read(handle,&(to.da_mon),sizeof(char));
    read(handle,&(to.da_year),sizeof(int));
}

KLoan::~KLoan() {
    free();
}

//- eof ---------------------------------------------------------------------
