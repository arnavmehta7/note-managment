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
        cout << "Choose an option:\n";
        cout << "1. Search notes\n";
        cout << "2. List notes\n";
	    cout << "3. Create a PRIVATE note\n";
	    cout << "4. Modify a PRIVATE note\n";
        cout << "5. Delete a PRIVATE note\n";
        cout << "6. Exit\n";
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        // the user input is stored in choice
        // switch statement- executes different code blocks according to values of choice variable

        // cin.ignore() - to ignore or discard characters in the input stream - \n 
        // <iostream> header (STL lib)
        // cin reads input from buffer until it encounters white space char , user does the \n command to subit their input
        // it remains as buffer so when you will use cin again, it will interpret the \n as input causing unexpected behaviour

        // getline takes input (cin) and puts it in query, also no input buffer of \n so we don't have to use cin.ignore() after
        // <string> header
        // searchNotes function- private_notes vector and query string

        switch (choice) {
            case '1': {
                string query;
                cout << "Enter search query: ";
                cin.ignore();  
                getline(cin, query);
                searchNotes(private_notes, query);
                break;
            }

            // listing
            // for loop iterates over each note in public and private_notes vector and displays it

            case '2': {
                cout << "Listing notes sorted by time of creation:\n";
                // TODO: Create function to sort notes by creation time -- use GPT
                // sortNotesByCreationTime(private_notes);
                cout << "Public Notes" << endl;
                for (const auto& note : public_notes) note ->display();
                cout << "Private Notes" << endl;
                for (const auto& note : private_notes) note->display();
                // TODO: Also list public notes, after sorting
                break;
            }

            // this is for creation of new note
            // we take heading as input and put it in heading - we assign heading to content and make a new note (dynamic allocation)
            // we save it and note content is saved 
            // private_note object is pushed back into private_note vector 


	        case '3': {
            string heading, content;
                cout << "Enter the HEADING of the new note:\n";
                cin.ignore(); 
                getline(cin, heading);
                content = heading;
            
                Note* note = new PrivateNote(heading, content);
                note->save();
                private_notes.push_back(note);
                    
                break;
            }
	        case '4': {
                string heading;
                cout << "Enter the heading private note to modify: ";
                cin.ignore(); // To consume the newline character left by cin
                getline(cin, heading);
		        for (auto& note : private_notes) {
                    if (note->getHeading() == heading) {
                        dynamic_cast<PrivateNote*>(note)->edit(heading);
                        break;
                    }
                }
                //edit(file);
                // TODO link the code to txt file to directly open the txt folder.
                break;
            }
            case '5': {
                string heading;
                cout << "Enter the heading of the private note to delete: ";
                cin.ignore(); // To consume the newline character left by cin
                getline(cin, heading);
                // TODO: Consider if user should rather put in index of note to delete
                deleteNote(private_notes, heading, FolderType::PRIVATE);
                break;
            }
            case '6':
                cout << "Exiting application.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 5.\n";
        }
    } while (choice != '6');

    for (auto& note : public_notes) delete note;
    for (auto& note : private_notes)delete note;
    return 0;
}
