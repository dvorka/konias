#include "loanlist.h"
#include "konias.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKIP_SET_DATA 32000

extern Konias *konias;

LoanList::LoanList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar):
	TListBox(bounds, aNumCols, aScrollBar) {
}

void LoanList::setData(void *data) {
	// dumb function to omit TListBox::setData() if selection == SKIP_SET_DATA

	if(((TListBoxRec*)data)->selection == SKIP_SET_DATA) {
		return;
	}
	else {
		TListBox::setData(data);
	}
}

void LoanList::handleEvent(TEvent& event) {
	TListBox::handleEvent(event);
}

//- eof ----------------------------------------------------------------------
