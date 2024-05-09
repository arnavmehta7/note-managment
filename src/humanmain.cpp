#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
// #include "PublicNote.hpp"
#include "includes/PublicNote.hpp"
#include "includes/PrivateNote.hpp"
#include "includes/Folder.hpp"
#include "includes/utils.hpp"

using namespace std;

int main() {

    // two vectors- public and private (dynamic arrays) are declared 
    // it stores the pointer to the Note object of Note class 

    vector<Note*> public_notes, private_notes;

    // they are calls to the function with two arguments 
    // 1. the path to the directory where the notes are stored  (string concatenation)
    // 2. reference to the vector where loaded notes should be stored 
    // there is a Folder class that provides getFolderName static method
    // it takes a FolderType enum value - public or private and returns the folder name as string

    loadNotesFromDirectory("notes/"+Folder::getFolderName(FolderType::PUBLIC), public_notes);
    loadNotesFromDirectory("notes/"+Folder::getFolderName(FolderType::PRIVATE), private_notes);

    // choice stores the user input
    // do while loop- code inside the loop will be executed atleast once and then repeatedly until the condition in while keywoard 
    // is no longer true

    char choice;
    do {
        cout << "Welcome to ASE - an Academic Search Engine - to ace your academia " << endl;
        cout << "This tool allows you to perform the following operations!" << endl;
        cout << "1. Search notes\n";
        cout << "2. List notes\n";
        cout << "3. Delete a PRIVATE note\n";
        cout << "4. Exit\n";
        cout << "Enter your preferred choice (1-4): " << endl;
        cin >> choice;

        // the user input is stored in choice
        // switch statement- executes different code blocks according to values of choice variable

        // cin.ignore() - to ignore or discard characters in the input stream - \n 
        // <iostream> header (STL lib)
        // cin reads input from buffer until it encounters white space char , user does the \n command to subit their input
        // it remains as buffer so when you will use cin again, it will interpret the \n as input causing unexpected behaviour

        // getline takes input (cin) and puts it in query, also no input buffer of \n so we don't have to use cin.ignore() after
        // <string> header
        // 

        switch (choice) {
            case '1': {
                string query;
                cout << "Enter search query: ";
                cin.ignore();  
                getline(cin, query);
                searchNotes(private_notes, query);
                break;
            }
            case '2': {
                cout << "Listing notes sorted by time of creation:\n";
                // TODO: Create function to sort notes by creation time -- use GPT
                // sortNotesByCreationTime(private_notes);
                for (const auto& note : private_notes) note->display();

                // TODO: Also list public notes, after sorting
                break;
            }
            case '3': {
                string heading;
                cout << "Enter the heading of the private note to delete: ";
                cin.ignore(); // To consume the newline character left by cin
                getline(cin, heading);
                // TODO: Consider if user should rather put in index of note to delete
                deleteNote(private_notes, heading, FolderType::PRIVATE);
                break;
            }
            case '4':
                cout << "Exiting application.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4.\n";
        }
    } while (choice != '4');

    for (auto& note : public_notes) delete note;
    for (auto& note : private_notes)delete note;
    return 0;
}