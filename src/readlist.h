//----------------------------------------------------------------------------
//                     Konias list of readers for loans
//
//                                 Dvorka
//				                  2002-2003
//----------------------------------------------------------------------------

#ifndef __KREADLIST_H
 #define __KREADLIST_H

 #include "kreader.h"
 #include "konias.h"
 #include "ktv.h"

 class ReaderList : public TListBox {
	public:
		KReader *lastReader;
		int lastItem;

		TDialog* findReader;

		ReaderList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar);
		void handleEvent(TEvent& event);
		void setData(void *data);

		void editLines2Book();
 };

 struct FindReaderData {
	//char searcher[128];
	TListBoxRec listBoxRec;
	char firstname[128];
	char surname[128];
	char email[128];
	char phone[128];
	char id[128];
	char address[128];
 };

#endif
