//----------------------------------------------------------------------------
//                       Konias loan - list of books
//
//                                 Dvorka
//				                  2002-2003
//----------------------------------------------------------------------------

#ifndef __LOAN_BOOKS_LIST_H
 #define __LOAN_BOOKS_LIST_H

 #include "konias.h"
 #include "ktv.h"

 class LoanBooksList : public TListBox {
	public:
		TUnsortedCollection *loanIds;

		LoanBooksList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar);
		void handleEvent(TEvent& event);
		void setData(void *data);
		void showCurrentLoan();
 };

#endif
