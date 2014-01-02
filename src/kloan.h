//----------------------------------------------------------------------------
//                               Konias loan
//
//                                 Dvorka
//				    2002
//----------------------------------------------------------------------------

#ifndef __KLOAN_H
#define __KLOAN_H

#include <dos.h>

#include "kbook.h"
#include "kreader.h"

enum LoanSubject {
    SUBJ_VOID=0,
    SUBJ_BOOK=1,
    SUBJ_CD=2
};

 class KLoan {
   private:
    TApplication *app;
    
   public:

    long id;                 // ID of this loan
    
    LoanSubject subjectType; // book or CD
    
    long subjectId;          // ID of book/CD
    long readerId;           // ID of the loaner
    
    struct date from;        // loaned from
    struct date to;          // loaned to
    
    boolean returned;        // true if in book is back again
    
    KLoan();
    KLoan(long readerId,
          long subjectId,
          LoanSubject subjectType,
          TApplication *app);
    void edit();             // edit this loan
    ~KLoan();
    
    void free();
    void save(int handle);
    void load(int handle);
 };

#endif
