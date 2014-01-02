#include "booklist.h"                            	
#include "konias.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKIP_SET_DATA 32000

TListBoxRec tListBoxRec;

extern Konias *konias;
extern FindBookData findBookData;
extern CriteriaDlgBoxData criteriaDlgBoxData;

BookList::BookList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar):
	TListBox(bounds, aNumCols, aScrollBar) {
		lastBook=NULL;
		lastItem=-1;
}

void BookList::setData(void *data) {
	// dumb function to omit TListBox::setData() if selection == SKIP_SET_DATA

	if(((TListBoxRec*)data)->selection == SKIP_SET_DATA) {
		return;
	}
	else {
		TListBox::setData(data);
	}
}

void BookList::editLines2Book() {

	// update book according to edit boxes
	findBook->getData(&findBookData);

	if(lastBook!=NULL) {
		lastBook->free();
		lastBook->name=strdup(findBookData.name);
		lastBook->author=strdup(findBookData.author);
		lastBook->bibInfo=strdup(findBookData.bibinfo);
		lastBook->bibRem=strdup(findBookData.bibrem);
		lastBook->state=findBookData.state;
		// leave ID unchanged
	}
}

void BookList::handleEvent(TEvent& event) {
	TListBox::handleEvent(event);

	getData(&tListBoxRec);
	if(tListBoxRec.selection!=lastItem) {
		// first store the data from the dialog into last book-can be changed
		findBook->getData(&findBookData);
		if(lastBook!=NULL) {
			// update book according to edit boxes
			lastBook->free();
			lastBook->name=strdup(findBookData.name);
			lastBook->author=strdup(findBookData.author);
			lastBook->bibInfo=strdup(findBookData.bibinfo);
			lastBook->bibRem=strdup(findBookData.bibrem);
			lastBook->state=findBookData.state;
			// leave ID unchanged

			// fix list box collection-update descriptor - if it was changed
			char *stringToSet;
			switch(criteriaDlgBoxData.criteria) {
				case 0: // name
					stringToSet=lastBook->name;
					break;
				case 1: // author
					stringToSet=lastBook->author;
					break;
				case 2: // state
					switch(lastBook->state) {
						case 0:
							stringToSet=K_TO_BORROW;
							break;
						case 1:
							stringToSet=K_BORROWED;
							break;
						case 2:
							stringToSet=K_RETIRED;
							break;
					}
					break;
				case 3: // bibInfo
					stringToSet=lastBook->bibInfo;
					break;
				case 4: // bibRem
					stringToSet=lastBook->bibRem;
					break;
				case 5: // id
					stringToSet=ltoa(lastBook->id,(char*)malloc(7),10);
					break;
				default:
					stringToSet=lastBook->name;
			}
			// remove string from list box, no deallocation
			//findBookData.listBoxRec.collection->atRemove(findBookData.listBoxRec.selection);
			// and insert (changed) string into list box for the leaved book (atInsert)
			//findBookData.listBoxRec.collection->atInsert(findBookData.listBoxRec.selection,stringToSet);
			findBookData.listBoxRec.collection->atPut(lastItem,
													  stringToSet);
		}

		lastItem=tListBoxRec.selection;
		// get newly choosen book (put current book into lastBook)
		lastBook=konias->kLibrary.getBook(lastItem);

		if(lastBook!=NULL) {
			// fill in the dialog according to current book
			strcpy(findBookData.name,lastBook->name);
			strcpy(findBookData.author,lastBook->author);
			strcpy(findBookData.bibinfo,lastBook->bibInfo);
			strcpy(findBookData.bibrem,lastBook->bibRem);
			findBookData.state=lastBook->state;
			sprintf(findBookData.id,"%d",lastBook->id);

			findBookData.listBoxRec.selection=SKIP_SET_DATA;
			findBook->setData(&findBookData);
		}

		draw();
	}
}

//- eof ----------------------------------------------------------------------
