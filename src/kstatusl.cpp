#include "kstatusl.h"

const char* KStatusLine::hint(ushort aHelpCtx) {
	switch(aHelpCtx) {
				case hcLibraryRestore:
					return "Obnoven¡ knihovny ze z lo§n¡ diskety...";
				case hcLibraryLoad:
					return "Otevýen¡ knihovny...";
				case hcLibraryStore:
					return "Ulo§en¡ knihovny...";
				case hcLibraryLoadAs:
					return "Otevýen¡ knihovny podle jm‚na...";
				case hcLibraryStoreAs:
					return "Ulo§en¡ knihovny pod jm‚nem...";
				case hcLibraryClose:
					return "Zavýen¡ aktu ln¡ knihovny...";
				case hcLibraryInfo:
					return "Profil aktu ln¡ knihovny...";
				case hcLibraryBackup:
					return "Z lohov n¡ knihovny na disketu...";
				case hcAboutKonias:
				case hcMenuKonias:
					return "O Koni çovi...";
				case hcMenuLibrary:
					return "Spr va knihoven - otevýen¡, zavýen¡, ulo§en¡, ...";
				case hcChangeVideoMode:
					return "ZmØna rozliçen¡ - 25 nebo 43/50 ý dek)";
				case hcMenuBook:
					return "Spr va kn¡§ek";
				case hcNewBook:
					return "Registrace nov‚ kn¡§ky...";
				case hcSearchBook:
					return "Vyp…jŸen¡, hled n¡, editace, smaz n¡ kn¡§ky...";
				case hcMenuReaders:
					return "Spr va Ÿten ý…";
				case hcNewReader:
					return "Registrace nov‚ho Ÿten ýe...";
				case hcSearchReader:
					return "Hled n¡, editace, smaz n¡ Ÿten ýe...";
				case hcMenuLoans:
					return "Spr va vìp…jŸek";
				case hcNewLoanToReader:
					return "Nov  vìp…jŸka Ÿten ýi...";
				case hcNewLoanOfBook:
					return "Zaznamen n¡ vìp…jŸky kn¡§ky...";
				case hcSearchLoans:
					return "Hled n¡, editace, smaz n¡ vìp…jŸky...";
				case hcMenuCd:
					return "Spr va CDŸek";
				case hcNewCd:
					return "Registrace nov‚ho CDŸka...";
				case hcSearchCd:
					return "Vyp…jŸen¡, hled n¡, editace, smaz n¡ CDŸka...";
				case hcExitProgram:
					return "UkonŸen¡ Koni çe...";
				default:
					return "Koni ç";
	}
}

//- eof ----------------------------------------------------------------------
