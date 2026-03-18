#include <iostream>   // ✅ Allows input/output (cin, cout)
#include <string>     // ✅ Lets us use the string type for text
using namespace std;  // ✅ Avoids writing std:: before standard library functions

// ✅ Function definition is placed outside main
// This function handles adding a book (currently just prints it out)
void addBook(string bn, string author, string code) {
    cout << "Book added: " << bn << " by " << author << " (Code: " << code << ")" << endl;
    // ✅ Here you could later add logic to save to a file or database
}

int main() {          // ✅ Program entry point
    int entries;      // ✅ Variable to store how many books the user will enter
    string bn, author, code; // ✅ Variables for book name, author, and code

    cout << "Number of Book Entries: "; // ✅ Ask user how many books they want to input
    cin >> entries;                     // ✅ Read the number from user
    cin.ignore(); // ✅ Clear leftover newline from input buffer (important before getline)

    // ✅ Loop runs from 1 up to the number of entries
    for (int i = 1; i <= entries; i++) {
        cout << "\nBook " << i << endl; // ✅ Show which book entry we’re on

        cout << "Book Name: ";          // ✅ Prompt for book name
        getline(cin, bn);               // ✅ Read full line (so spaces are allowed)

        cout << "Author: ";             // ✅ Prompt for author name
        getline(cin, author);           // ✅ Read full line

        cout << "Code: ";               // ✅ Prompt for book code
        cin >> code;                    // ✅ Read code (no spaces allowed here)
        cin.ignore(); // ✅ Clear newline left after entering code

        // ✅ Call the function to "add" the book
        addBook(bn, author, code);
    }

    return 0; // ✅ End program successfully
}