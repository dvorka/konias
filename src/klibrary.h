//----------------------------------------------------------------------------
//                             Konias library
//
//                                 Dvorka
//				                    2002
//----------------------------------------------------------------------------

#ifndef __KLIBRARY_H
 #define __KLIBRARY_H

 #include <array.h>

 #include "kbook.h"
 #include "kloan.h"
 #include "kreader.h"
 #include "ktv.h"

 /*
	Klibrary is set of users and set of books. It provides
	methods for persistence, searching, etc.
 */
 class KLibrary {

	private:
		// pointer to array of pointers (to KBooks)
		KReader **readers;
		KBook **books;
		KLoan **loans;

		// array length
		unsigned int readersLng;
		unsigned int booksLng;
		unsigned int loansLng;

		// max used ID
		unsigned int booksMaxId;
		unsigned int readersMaxId;
		unsigned int loansMaxId;

	public:
		long lastModified;

		// intra messages
		char msg[500];

		KLibrary();

		void free();
		char* getInfoString();

		unsigned int getNextBookId();
		unsigned int getNextReaderId();
		unsigned int getNextLoanId();

		boolean addBook(KBook *newBook);
		void killBook(KBook *book);
		TUnsortedCollection* getBooksByAuthor();
		TUnsortedCollection* getBooksByName();
		TUnsortedCollection* getBooksByState();
		TUnsortedCollection* getBooksByBibInfo();
		TUnsortedCollection* getBooksById();
		KBook* getBook(unsigned int i);
		KBook* getBookById(unsigned int i);

		boolean addReader(KReader *newReader);
		void killReader(KReader *reader);
		void findReaderDlgBox(TApplication* app, boolean finishLoan);
		TUnsortedCollection* getReaders();
		TUnsortedCollection* getReaders(TUnsortedCollection *&readerIds);
		KReader* getReader(unsigned int i);
		KReader* getReaderById(unsigned int i);

		void addLoan(KLoan *newLoan);
		void killLoan(KLoan *loan);
		void manageLoans(long readerId, TApplication* app);
		TUnsortedCollection* getLoansByReader(unsigned int readerId, TUnsortedCollection  *&loanIds);
		KLoan* getLoanById(unsigned int i);
		KLoan* getLoanBySubject(unsigned int subjectId);

		boolean save(char* fileName);
		boolean load(char* fileName);
 };

#endif
