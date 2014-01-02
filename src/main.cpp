//-------------------------------------------------------------------------
//                   Konias especially developed for Marinka
//
//                                   Dvorka
//                                    2002
//-------------------------------------------------------------------------

#include "konias.h"

Konias *konias;

int main(int argc, char* argv[]) {
	Konias koniasovnik;
	konias=&koniasovnik;

	koniasovnik.run();

	return 0;
}

//- eof ----------------------------------------------------------------------
