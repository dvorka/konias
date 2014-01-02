#include "kbook.h"
#include "konias.h"

#include <ALLOC.H>
#include <IO.H>
#include <STDIO.H>
#include <STRING.H>

extern Konias *konias;

struct NewKBookDialog {
	char author[128];
	char name[128];
	char bibInfo[128];
	char bibRem[128];
	int state;
} newKBookDialog;

// methods ----------------------------------------------------------------

KBook::KBook() {
	dialogCanceled=TRUE;

	author=NULL;
	name=NULL;
	bibInfo=NULL;
	bibRem=NULL;
	state=FREE;
}

KBook::KBook(TApplication *app) {
	dialogCanceled=TRUE;
	author=NULL;
	name=NULL;
	bibInfo=NULL;
	bibRem=NULL;
	state=FREE;

	this->app=app;

	// create dialog to get data
	TDialog *dial = new TDialog( TRect( 20, 1, 60, 22), "Nov  kn¡§ka" );
	TInputLine *ilAuthor;
	TInputLine *ilName;
	TInputLine *ilBibInfo, *ilBibRem;

	if(dial) {
		TView *b;
		dial->helpCtx=hcNewBook;

		// author
		ilAuthor = new TInputLine( TRect( 3, 2, 37, 3 ), 128 );
		dial->insert( ilAuthor );
		dial->insert( new TLabel( TRect( 2, 1, 24, 2 ),
								  "~A~utor",
								  ilAuthor ));

		// name
		ilName = new TInputLine( TRect( 3, 5, 37, 6 ), 128 );
		dial->insert( ilName );
		dial->insert( new TLabel( TRect( 2, 4, 24, 5 ),
								  "~T~itul",
								  ilName ));

		// bibinfo
		ilBibInfo = new TInputLine( TRect( 3, 8, 37, 9 ), 128 );
		dial->insert( ilBibInfo );
		dial->insert( new TLabel( TRect( 2, 7, 27, 8 ),
								  "~B~ibliografickì £daj",
								  ilBibInfo ));

		// poznamka
		ilBibRem = new TInputLine( TRect( 3, 11, 37, 12 ), 128 );
		dial->insert( ilBibRem );
		dial->insert( new TLabel( TRect( 2, 10, 27, 11 ),
								  "~P~ozn mka",
								  ilBibRem ));

		// status
		b = new TRadioButtons( TRect( 3, 14, 18, 17),
			new TSItem( "~K~ p…jŸen¡",
			new TSItem( "P…jŸ~e~n ",
			new TSItem( "P~r~ezenŸn¡", 0 )
			)));
		dial->insert( new TLabel( TRect( 2, 13, 8, 14), "Stav", b ));
		dial->insert( b );

		// buttons
		dial->insert( new TButton( TRect( 14, 18, 24, 20 ), "~O~K", cmOK,
					bfDefault ));
		dial->insert( new TButton( TRect( 27, 18, 37, 20 ), "~S~torno", cmCancel,
					bfNormal ));

		// set default selected
		ilAuthor->select();

		// show & exec
		ushort retval=app->deskTop->execView( dial );

		if(retval==cmOK) {
			dialogCanceled=FALSE;

			// rip data
			author=strdup(ilAuthor->data);
			name=strdup(ilName->data);
			bibInfo=strdup(ilBibInfo->data);
			bibRem=strdup(ilBibRem->data);
			id=konias->kLibrary.getNextBookId();
			dial->getData(&newKBookDialog);
			state=newKBookDialog.state;
		}
		else {
			dialogCanceled=TRUE;
		}
	}

	// destroy dialog
	app->destroy( dial );
}

boolean KBook::canBeBorrowed() {
    if(state==BOOK_FREE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

boolean KBook::borrow() {
    if(state==BOOK_FREE) {
        state=BOOK_BORROWED;
        return TRUE;
    } else {
        if(state==BOOK_BORROWED) {
			messageBox("Kn¡§ka je ji§ vyp…jŸena!",mfError|mfOKButton);
		}
		else {
			messageBox("Kn¡§ka je vyýazena a proto nem…§e bìt vyp…jŸena!",mfError|mfOKButton);
		}
    }

    return FALSE;
}

boolean KBook::giveBack() {

    return FALSE;
}

void KBook::free() {
	if(author!=NULL) {
		::free(author);
		author=NULL;
	}

	if(name!=NULL) {
		::free(name);
		name=NULL;
	}

	if(bibInfo!=NULL) {
		::free(bibInfo);
		bibInfo=NULL;
	}

	if(bibRem!=NULL) {
		::free(bibRem);
		bibRem=NULL;
	}
}

void KBook::save(int handle) {
	size_t lng;

	write(handle,&id,sizeof(long));

	lng=strlen(author)+1;
	write(handle,&lng,sizeof(size_t));
	write(handle,author,lng);

	lng=strlen(name)+1;
	write(handle,&lng,sizeof(size_t));
	write(handle,name,lng);

	lng=strlen(bibInfo)+1;
	write(handle,&lng,sizeof(size_t));
	write(handle,bibInfo,lng);

	lng=strlen(bibRem)+1;
	write(handle,&lng,sizeof(size_t));
	write(handle,bibRem,lng);

	write(handle,&state,sizeof(int));
}

void KBook::load(int handle) {
	size_t lng;

	read(handle,&id,sizeof(long));
    
	read(handle,&lng,sizeof(size_t));
	author=(char *)farmalloc(lng);
	read(handle,author,lng);

	read(handle,&lng,sizeof(size_t));
	name=(char *)farmalloc(lng);
	read(handle,name,lng);

	read(handle,&lng,sizeof(size_t));
	bibInfo=(char *)farmalloc(lng);
	read(handle,bibInfo,lng);

	read(handle,&lng,sizeof(size_t));
	bibRem=(char *)farmalloc(lng);
	read(handle,bibRem,lng);

	read(handle,&state,sizeof(int));
}

void KBook::show() {
	// create dialog to get data
	TDialog *dial = new TDialog( TRect( 20, 1, 60, 22), "O kn¡§ce" );
	TInputLine *ilAuthor;
	TInputLine *ilName;
	TInputLine *ilBibInfo, *ilBibRem;

	if(dial) {
		TView *b;
		dial->helpCtx=hcNewBook;

		// author
		ilAuthor = new TInputLine( TRect( 3, 2, 37, 3 ), 128 );
		ilAuthor->setState(sfDisabled,True);
		dial->insert( ilAuthor );
		dial->insert( new TLabel( TRect( 2, 1, 24, 2 ),
								  "~A~utor",
								  ilAuthor ));

		// name
		ilName = new TInputLine( TRect( 3, 5, 37, 6 ), 128 );
		ilName->setState(sfDisabled,True);
		dial->insert( ilName );
		dial->insert( new TLabel( TRect( 2, 4, 24, 5 ),
								  "~J~m‚no knihy",
								  ilName ));

		// bibinfo
		ilBibInfo = new TInputLine( TRect( 3, 8, 37, 9 ), 128 );
		ilBibInfo->setState(sfDisabled,True);
		dial->insert( ilBibInfo );
		dial->insert( new TLabel( TRect( 2, 7, 27, 8 ),
								  "~B~ibliografickì £daj",
								  ilBibInfo ));

		// poznamka
		ilBibRem = new TInputLine( TRect( 3, 11, 37, 12 ), 128 );
		ilBibRem->setState(sfDisabled,True);
		dial->insert( ilBibRem );
		dial->insert( new TLabel( TRect( 2, 10, 27, 11 ),
								  "~P~ozn mka",
								  ilBibRem ));

		// status
		b = new TRadioButtons( TRect( 3, 14, 18, 17),
			new TSItem( "~K~ p…jŸen¡",
			new TSItem( "P…jŸ~e~n ",
			new TSItem( "~V~yýazen ", 0 )
			)));
		b->setState(sfDisabled,True);
		dial->insert( new TLabel( TRect( 2, 13, 8, 14), "Stav", b ));
		dial->insert( b );

		// buttons
		dial->insert( new TButton( TRect( 14, 18, 24, 20 ), "~O~K", cmOK,
					bfDefault ));

		// fill it with data
		strcpy(newKBookDialog.author,author);
		strcpy(newKBookDialog.name,name);
		strcpy(newKBookDialog.bibInfo,bibInfo);
		strcpy(newKBookDialog.bibRem,bibRem);
		newKBookDialog.state=state;

		dial->setData(&newKBookDialog);

		// show & exec
		app->deskTop->execView( dial );
	}

	// destroy dialog
	app->destroy( dial );
}

KBook::~KBook() {
	free();
}

//- eof ----------------------------------------------------------------------
