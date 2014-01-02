//----------------------------------------------------------------------------
//                              Konias book
//
//                                 Dvorka
//				    2002
//----------------------------------------------------------------------------

#ifndef __KBOOK_H
#define __KBOOK_H

#include "ktv.h"

class KBook {

  private:
    TApplication *app;

  public:

    boolean dialogCanceled;

#define BOOK_FREE 0
#define BOOK_BORROWED 1
#define BOOK_RETIRED 2

    enum BookStatus {
        FREE=BOOK_FREE,
        BORROWED=BOOK_BORROWED,
        RETIRED=BOOK_RETIRED
    };

    long id;
    char *author;
    char *name;
    char *bibInfo;
    char *bibRem;
    int  state;

    KBook();
    KBook(TApplication *app); // construct book from a dialog

    boolean canBeBorrowed();
    boolean borrow();
    boolean giveBack();

    void show();
    void free();
    void save(int handle);
    void load(int handle);
    ~KBook();
};

#endif
