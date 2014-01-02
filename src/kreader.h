//----------------------------------------------------------------------------
//                             Konias reader
//
//                                 Dvorka
//				                    2002
//----------------------------------------------------------------------------

#ifndef __KREADER_H
 #define __KREADER_H

 #include "ktv.h"

 class KReader {
	private:
		TApplication *app;

	public:

		boolean dialogCanceled;

		long id;
		char *firstname;
		char *surname;
		char *email;
		char *phone;
		char *address;

		KReader();
		KReader(TApplication *app);
		~KReader();

		void free();
		void show();
		void save(int handle);
		void load(int handle);
 };

#endif