#include "loanblst.h"
#include "konias.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKIP_SET_DATA 32000

extern Konias *konias;

LoanBooksList::LoanBooksList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar):
	TListBox(bounds, aNumCols, aScrollBar) {
	loanIds=NULL;
}

void LoanBooksList::setData(void *data) {
	// dumb function to omit TListBox::setData() if selection == SKIP_SET_DATA

	if(((TListBoxRec*)data)->selection == SKIP_SET_DATA) {
		return;
	}
	else {
		TListBox::setData(data);
	}
}

void LoanBooksList::handleEvent(TEvent& event) {
	TListBox::handleEvent(event);
}

void LoanBooksList::showCurrentLoan() {
	if(loanIds!=NULL && loanIds->getCount()>0) {
		long i=0;
		char *string;
		TListBoxRec record;

		TListBox::getData(&record);
		string=(char *)loanIds->at(record.selection);
		i=atol(string);

		KLoan *kloan=konias->kLibrary.getLoanById(i);

		if(kloan==NULL) {
			messageBox("Vìp…jŸka s t¡mto ID neexistuje!",mfError|mfOKButton);
			return;
		}
		else {
			kloan->edit();
			return;
		}
	}

	messageBox("Tento Ÿten ý nem  § dnou vìp…jŸku!",mfInformation|mfOKButton);
}

//- eof ----------------------------------------------------------------------
