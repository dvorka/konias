//----------------------------------------------------------------------------
//                            Konias status line
//
//                                 Dvorka
//				                    2002
//----------------------------------------------------------------------------

#ifndef __KSTATUS_LINE_H
 #define __KSTATUS_LINE_H

 #include "ktv.h"

 class KStatusLine : public TStatusLine {
	public:
		KStatusLine(const TRect& bounds, TStatusDef& aDefs):
			TStatusLine(bounds,aDefs) {
		}

		virtual const char* hint(ushort aHelpCtx);
 };

#endif
