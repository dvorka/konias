//----------------------------------------------------------------------------
//                            Konias book list
//
//                                 Dvorka
//				                    2002
//----------------------------------------------------------------------------

#ifndef __KBOOKLIST_H
 #define __KBOOKLIST_H

 #include "kbook.h"
 #include "konias.h"
 #include "ktv.h"

 class BookList : public TListBox {
	public:
		KBook *lastBook;
		int lastItem;

		TDialog* findBook;

		BookList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar);
		void handleEvent(TEvent& event);
		void setData(void *data);

		void editLines2Book();
 };

#endif
