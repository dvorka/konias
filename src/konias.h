//----------------------------------------------------------------------------
//                                Konias.h
//
//                                 Dvorka
//				  2002-2003
//----------------------------------------------------------------------------

#ifndef __KONIAS_H
#define __KONIAS_H

#include "gadgets.h"
#include "klibrary.h"
#include "ktv.h"

class Konias:public TApplication {
  private:

    TClockView *clock;
#ifdef DEBUG
    THeapView *heap;
#else
    ActualLibraryView *actualLibraryView;
#endif
    
  public:
    KLibrary kLibrary;
    
    Konias();
    static TStatusLine *initStatusLine(TRect r);
    static TMenuBar *initMenuBar(TRect r);
    
    void handleEvent(TEvent& event);
    void idle();
    
    // methods
    void aboutDlgBox();
    void actualLibraryInfo();
    void quickStore();
    void storeAs();
    void quickLoad();
    void loadAs();
    void backupLibrary();
    void restoreLibrary();
    
    void newReaderDlgBox();
    void findReaderDlgBox();
    
    void newBookDlgBox();
    void findBookDlgBox();
    void findBookCriteriaDlgBox();
    
    void manageLoansDlgBox();
};



struct FindBookData {
    //char searcher[128];
    TListBoxRec listBoxRec;
    char author[128];
    char name[128];
    char bibinfo[128];
    char bibrem[128];
    char id[128];
    ushort state;
};

struct CriteriaDlgBoxData {
    ushort criteria;
};

#endif
