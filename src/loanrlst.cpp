#include "loanrlst.h"
#include "konias.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKIP_SET_DATA 32000

TListBoxRec loanReadersRecord;
TListBoxRec recordOfLoans;
extern Konias *konias;

LoanReadersList::LoanReadersList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar):
	TListBox(bounds, aNumCols, aScrollBar) {
		lastReader=NULL;
		lastItem=-1;
}

void LoanReadersList::setData(void *data) {
	// dumb function to omit TListBox::setData() if selection == SKIP_SET_DATA

	if(((TListBoxRec*)data)->selection == SKIP_SET_DATA) {
		return;
	}
	else {
		TListBox::setData(data);
	}
}

void LoanReadersList::handleEvent(TEvent& event) {
	TListBox::handleEvent(event);

	getData(&loanReadersRecord);
	if(loanReadersRecord.selection!=lastItem) {
		// propagate reader's loans into the LoanBooksList...
		lastItem=loanReadersRecord.selection;
		lastReader=konias->kLibrary.getReader(lastItem);

		if(lastReader!=NULL) {
			refillLoanList();
		}

		draw();
	}
}

void LoanReadersList::refillLoanList() {

	if(loanBooksList->loanIds != NULL) {
		loanBooksList->loanIds->freeAll();
	}

	TUnsortedCollection* c=konias->kLibrary.getLoansByReader(lastReader->id,
																	 loanBooksList->loanIds);
	recordOfLoans.collection=c;
	recordOfLoans.selection=0;

	loanBooksList->setData(&recordOfLoans);
}

void LoanReadersList::showCurrentReader() {
	TListBoxRec record;

	TListBox::getData(&record);
	long i=atol((char *)readerIds->at(record.selection));

	konias->kLibrary.getReaderById(i)->show();
}

void LoanReadersList::selectReaderWithId(long readerId) {
	// find according to ID and then select
	if(readerId>=0) {
		for(short i=0; i<readerIds->getCount(); i++)  {
			if(atol((char *)readerIds->at(i))==readerId) {
				focusItem(i);
				selectItem(i);
				draw();
				return;
			}
		}
	}
}

//- eof ----------------------------------------------------------------------
