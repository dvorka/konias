#define Uses_TRect
#define Uses_TView
#define Uses_TDrawBuffer
#include <tv.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <strstrea.h>
#include <iomanip.h>
#include <alloc.h>
#include <time.h>

#include "gadgets.h"

extern char libraryFile[120];

// ------------- Heap viewer functions

THeapView::THeapView(TRect& r) : TView( r ) {
    oldMem = 0;
    newMem = heapSize();
}


void THeapView::draw() {
    TDrawBuffer buf;
    char c = getColor(2);

    buf.moveChar(0, ' ', c, size.x);
    buf.moveStr(0, heapStr, c);
    writeLine(0, 0, size.x, 1, buf);
}


void THeapView::update() {
    if( (newMem = heapSize()) != oldMem )
        {
        oldMem = newMem;
        drawView();
        }
}


long THeapView::heapSize()
{
    long total = farcoreleft();
    struct farheapinfo heap;
    ostrstream totalStr( heapStr, sizeof heapStr);

    switch( farheapcheck() )
        {
        case _HEAPEMPTY:
            strcpy(heapStr, "     No heap");
            total = -1;
            break;

        case _HEAPCORRUPT:
            strcpy(heapStr, "Heap corrupt");
            total = -2;
            break;

        case _HEAPOK:
            heap.ptr = NULL;
            while(farheapwalk(&heap) != _HEAPEND)
                if(!heap.in_use)
                    total += heap.size;
            totalStr << setw(12) << total << ends;
            break;
        }
    return(total);
}

// -------------- Clock Viewer functions

TClockView::TClockView( TRect& r ) : TView( r ) {
    strcpy(lastTime, "        ");
    strcpy(curTime, "        ");
}


void TClockView::draw() {
    TDrawBuffer buf;
    char c = getColor(2);

    buf.moveChar(0, ' ', c, size.x);
    buf.moveStr(0, curTime, c);
    writeLine(0, 0, size.x, 1, buf);
}


void TClockView::update() {
    time_t t = time(0);
    char *date = ctime(&t);

    date[19] = '\0';
    strcpy(curTime, &date[11]);        /* Extract time. */

    if( strcmp(lastTime, curTime) )
        {
        drawView();
        strcpy(lastTime, curTime);
        }
}

// - actual library ----------------------------------------------------------------------------

ActualLibraryView::ActualLibraryView( TRect& r ) : TView( r ) {
    oldLibraryFile[0]=NULL;
}

void ActualLibraryView::draw() {
    TDrawBuffer buf;
    char c = getColor(2);
    char *s;

    buf.moveChar(0, ' ', c, size.x);
    if(strlen(libraryFile)>12) {
        s=strrchr(libraryFile,'\\');
        buf.moveStr(0, s+1, c);
    }
    else {
        if(libraryFile[0]==NULL) {
            buf.moveStr(0, " ", c);
        }
        else {
            buf.moveStr(0, libraryFile, c);
        }
    }
    writeLine(0, 0, size.x, 1, buf);
}


void ActualLibraryView::update() {
    if( strcmp(libraryFile,oldLibraryFile)) {
        strcpy(oldLibraryFile, libraryFile);
        drawView();
    }
}

// - eof ------------------------------------------------------------------------------------------
