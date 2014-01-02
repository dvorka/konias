#include "readlist.h"
#include "konias.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKIP_SET_DATA 32000

FindReaderData findReaderData;

extern TListBoxRec tListBoxRec;
extern Konias *konias;

ReaderList::ReaderList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar):
	TListBox(bounds, aNumCols, aScrollBar) {
		lastReader=NULL;
		lastItem=-1;
}

void ReaderList::setData(void *data) {
	// dumb function to omit TListBox::setData() if selection == SKIP_SET_DATA

	if(((TListBoxRec*)data)->selection == SKIP_SET_DATA) {
		return;
	}
	else {
		TListBox::setData(data);
	}
}

void ReaderList::editLines2Book() {
		// update readers according to edit boxes
		findReader->getData(&findReaderData);

		if(lastReader!=NULL) {
			lastReader->free();
			lastReader->firstname=strdup(findReaderData.firstname);
			lastReader->surname=strdup(findReaderData.surname);
			lastReader->email=strdup(findReaderData.email);
			lastReader->phone=strdup(findReaderData.phone);
			lastReader->address=strdup(findReaderData.address);
			// leave ID unchanged
		}
}

void ReaderList::handleEvent(TEvent& event) {
	TListBox::handleEvent(event);

	getData(&tListBoxRec);
	if(tListBoxRec.selection!=lastItem) {
		//KReader *&debugReader=lastReader;

		// first store the data from the dialog into last Reader-can be changed
		findReader->getData(&findReaderData);
		if(lastReader!=NULL) {
			// update Reader according to edit boxes
			lastReader->free();
			lastReader->firstname=strdup(findReaderData.firstname);
			lastReader->surname=strdup(findReaderData.surname);
			lastReader->email=strdup(findReaderData.email);
			lastReader->phone=strdup(findReaderData.phone);
			lastReader->address=strdup(findReaderData.address);
			// leave ID unchanged

			// fix list box collection-update descriptor - if it was changed
			char *stringToSet;
			int lng=strlen(lastReader->firstname)+
					strlen(lastReader->surname)+2;
			stringToSet=(char*)malloc(lng);
			strcpy(stringToSet,lastReader->surname);
			strcat(stringToSet," ");
			strcat(stringToSet,lastReader->firstname);

			::free(findReaderData.listBoxRec.collection->at(lastItem));
			findReaderData.listBoxRec.collection->atPut(lastItem,
														stringToSet);
		}

		lastItem=tListBoxRec.selection;
		lastReader=konias->kLibrary.getReader(lastItem);

		if(lastReader!=NULL) {
			strcpy(findReaderData.firstname,lastReader->firstname);
			strcpy(findReaderData.surname,lastReader->surname);
			strcpy(findReaderData.email,lastReader->email);
			strcpy(findReaderData.phone,lastReader->phone);
			strcpy(findReaderData.address,lastReader->address);
			sprintf(findReaderData.id,"%d",lastReader->id);

			findReaderData.listBoxRec.selection=SKIP_SET_DATA;
			findReader->setData(&findReaderData);
		}

		draw();
	}
}

//- eof ----------------------------------------------------------------------
