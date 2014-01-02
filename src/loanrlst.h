//----------------------------------------------------------------------------
//                     Konias list of readers for loans
//
//                                 Dvorka
//				                  2002-2003
//----------------------------------------------------------------------------

#ifndef __LOAN_READERS_LIST_H
 #define __LOAN_READERS_LIST_H

 #include "kreader.h"
 #include "loanblst.h"
 #include "konias.h"
 #include "ktv.h"

 /**
  * The content of this list box is filling the list of loaned books
  * (LoanBooksList).
  */
 class LoanReadersList : public TListBox {
	public:
		LoanBooksList *loanBooksList;

		TUnsortedCollection *readerIds;

		KReader *lastReader;
		int lastItem;

		//TDialog* findReader;

		LoanReadersList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar);
		void handleEvent(TEvent& event);
		void setData(void *data);

		void refillLoanList();
		void showCurrentReader();
		void selectReaderWithId(long readerId);
 };

#endif
