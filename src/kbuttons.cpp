//----------------------------------------------------------------------------
//                            Konias buttons
//
//                                 Dvorka
//				                  2002-2003
//----------------------------------------------------------------------------

// Loans overview dialog --------------------------------------------------

class LibraryAboutReaderButton : public TButton {

	public:

		LoanReadersList* loanReadersList;

		LibraryAboutReaderButton(LoanReadersList* loanReadersList,
								 const TRect& bounds,
								 const char *aTitle,
								 ushort aCommand,
								 ushort aFlags):
			TButton(bounds, aTitle, aCommand, aFlags) {
			this->loanReadersList=loanReadersList;
		}

		void press() {
			loanReadersList->showCurrentReader();
		}
};

class LibraryLoanAboutButton : public TButton {

	public:

		LoanBooksList* loanBooksList;
		LoanReadersList* loanReadersList;

		LibraryLoanAboutButton(LoanReadersList* loanReadersList,
							   LoanBooksList* loanBooksList,
								 const TRect& bounds,
								 const char *aTitle,
								 ushort aCommand,
								 ushort aFlags):
			TButton(bounds, aTitle, aCommand, aFlags) {
			this->loanBooksList=loanBooksList;
			this->loanReadersList=loanReadersList;
		}

		void press() {
			loanBooksList->showCurrentLoan();
			loanReadersList->refillLoanList();
		}
};

