//----------------------------------------------------------------------------
//                            Konias loan list
//
//                                 Dvorka
//				                    2002
//----------------------------------------------------------------------------

#ifndef __KLOANLIST_H
 #define __KLOANLIST_H

 #include "konias.h"
 #include "ktv.h"

 class LoanList : public TListBox {
	public:
		LoanList(const TRect& bounds, ushort aNumCols, TScrollBar *aScrollBar);
		void handleEvent(TEvent& event);
		void setData(void *data);
 };

#endif
