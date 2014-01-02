#include "kreader.h"
#include "konias.h"

#include <ALLOC.H>
#include <IO.H>
#include <STDIO.H>
#include <STRING.H>

extern Konias *konias;

struct NewKReaderDialog {
    char firstname[128];
    char surname[128];
    char email[128];
    char phone[128];
    char address[128];
} newKReaderDialog;

// methods ---------------------------------------------------------------

KReader::KReader() {
    dialogCanceled=TRUE;
    
    firstname=NULL;
    surname=NULL;
    email=NULL;
    phone=NULL;
    address=NULL;
}

KReader::KReader(TApplication *app) {
    firstname=NULL;
    surname=NULL;
    email=NULL;
    phone=NULL;
    address=NULL;

    this->app=app;
    
    TDialog *dial = new TDialog( TRect( 20, 1, 73, 17), "Novì Ÿten ý" );
    
    if(dial) {
        dial->options|=ofCentered;
        dial->helpCtx=hcNewReader;
        
        TView *b;
        
        TInputLine *nameLine = new TInputLine( TRect( 3, 2, 25, 3 ), 128 );
        dial->insert( nameLine );
        dial->insert( new TLabel( TRect( 2, 1, 13, 2 ),
                                  "~J~m‚no",
                                  nameLine ));
        
#define XOFF 25
        
        b = new TInputLine( TRect( 3+XOFF, 2, 25+XOFF, 3 ), 128 );
        dial->insert( b );
        dial->insert( new TLabel( TRect( 2+XOFF, 1, 13+XOFF, 2 ),
                                  "~P~ý¡jmen¡",
                                  b ));
        
        b = new TInputLine( TRect( 3, 5, 50, 6 ), 128 );
        dial->insert( b );
        dial->insert( new TLabel( TRect( 2, 4, 24, 5 ),
                                  "~E~mail",
                                  b ));
        
        b = new TInputLine( TRect( 3, 8, 50, 9 ), 128 );
        dial->insert( b );
        dial->insert( new TLabel( TRect( 2, 7, 27, 8 ),
                                  "~T~elef‚n",
                                  b ));
        
        b = new TInputLine( TRect( 3, 11, 50, 12 ), 128 );
        dial->insert( b );
        dial->insert( new TLabel( TRect( 2, 10, 27, 11 ),
                                  "~A~dresa",
                                  b ));
        
        // buttons
        dial->insert( new TButton( TRect( 27, 13, 37, 15 ), "~O~K", cmOK,
                                   bfDefault ));
        dial->insert( new TButton( TRect( 40, 13, 50, 15 ), "~S~torno", cmCancel,
                                   bfNormal ));
        
        nameLine->select();
        
        // show & exec
        ushort retval=app->deskTop->execView( dial );
        
        if(retval==cmOK) {
            dialogCanceled=FALSE;
            
            // rip data
            dial->getData(&newKReaderDialog);
            firstname=strdup(newKReaderDialog.firstname);
            surname=strdup(newKReaderDialog.surname);
            phone=strdup(newKReaderDialog.phone);
            email=strdup(newKReaderDialog.email);
            address=strdup(newKReaderDialog.address);
            id=konias->kLibrary.getNextReaderId();
        }
        else {
            dialogCanceled=TRUE;
        }
    }
    
    app->destroy( dial );
}

void KReader::show() {
    TDialog *dial = new TDialog( TRect( 20, 1, 73, 17), "O Ÿten ýi" );
    
    if(dial) {
        dial->options|=ofCentered;
        dial->helpCtx=hcNewReader;
        
        TView *b;
        
        TInputLine *nameLine = new TInputLine( TRect( 3, 2, 25, 3 ), 128 );
        nameLine->setState(sfDisabled,True);
        dial->insert( nameLine );
        dial->insert( new TLabel( TRect( 2, 1, 13, 2 ),
                                  "~J~m‚no",
                                  nameLine ));
        
#define XOFF 25
        
        b = new TInputLine( TRect( 3+XOFF, 2, 25+XOFF, 3 ), 128 );
        b->setState(sfDisabled,True);
        dial->insert( b );
        dial->insert( new TLabel( TRect( 2+XOFF, 1, 13+XOFF, 2 ),
                                  "~P~ý¡jmen¡",
                                  b ));
        
        b = new TInputLine( TRect( 3, 5, 50, 6 ), 128 );
        b->setState(sfDisabled,True);
        dial->insert( b );
        dial->insert( new TLabel( TRect( 2, 4, 24, 5 ),
                                  "~E~mail",
                                  b ));
        
        b = new TInputLine( TRect( 3, 8, 50, 9 ), 128 );
        b->setState(sfDisabled,True);
        dial->insert( b );
        dial->insert( new TLabel( TRect( 2, 7, 27, 8 ),
                                  "~T~elef‚n",
								  b ));
        
        b = new TInputLine( TRect( 3, 11, 50, 12 ), 128 );
        b->setState(sfDisabled,True);
        dial->insert( b );
        dial->insert( new TLabel( TRect( 2, 10, 27, 11 ),
                                  "~A~dresa",
                                  b ));
        
        // buttons
        dial->insert( new TButton( TRect( 27+11, 13, 37+13, 15 ), "~O~K", cmOK,
                                   bfDefault ));
        
        // fill data
        strcpy(newKReaderDialog.firstname,firstname);
        strcpy(newKReaderDialog.surname,surname);
        strcpy(newKReaderDialog.phone,phone);
        strcpy(newKReaderDialog.email,email);
        strcpy(newKReaderDialog.address,address);
        dial->setData(&newKReaderDialog);
        
        // show & exec
        app->deskTop->execView( dial );
    }
    
    app->destroy( dial );
}


void KReader::free() {
    if(firstname!=NULL) {
        ::free(firstname);
        firstname=NULL;
    }
    
    if(surname!=NULL) {
        ::free(surname);
        surname=NULL;
	}
    
    if(phone!=NULL) {
        ::free(phone);
        phone=NULL;
    }
    
    if(email!=NULL) {
        ::free(email);
        email=NULL;
    }
    
    if(address!=NULL) {
        ::free(address);
        address=NULL;
    }
    
}

void KReader::save(int handle) {
    size_t lng;
    
    write(handle,&id,sizeof(long));
    
    lng=strlen(firstname)+1;
    write(handle,&lng,sizeof(size_t));
    write(handle,firstname,lng);
    
    lng=strlen(surname)+1;
    write(handle,&lng,sizeof(size_t));
    write(handle,surname,lng);
    
    lng=strlen(email)+1;
    write(handle,&lng,sizeof(size_t));
    write(handle,email,lng);
    
    lng=strlen(phone)+1;
    write(handle,&lng,sizeof(size_t));
    write(handle,phone,lng);
    
    lng=strlen(address)+1;
    write(handle,&lng,sizeof(size_t));
    write(handle,address,lng);
}

void KReader::load(int handle) {
    size_t lng;

    read(handle,&id,sizeof(long));
    
    read(handle,&lng,sizeof(size_t));
    firstname=(char *)farmalloc(lng);
    read(handle,firstname,lng);
    
    read(handle,&lng,sizeof(size_t));
    surname=(char *)farmalloc(lng);
    read(handle,surname,lng);
    
    read(handle,&lng,sizeof(size_t));
    email=(char *)farmalloc(lng);
    read(handle,email,lng);

    read(handle,&lng,sizeof(size_t));
    phone=(char *)farmalloc(lng);
    read(handle,phone,lng);
    
    read(handle,&lng,sizeof(size_t));
    address=(char *)farmalloc(lng);
    read(handle,address,lng);
}

KReader::~KReader() {
	free();
}

//- eof ---------------------------------------------------------------------
