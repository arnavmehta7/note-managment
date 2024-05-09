#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "PublicNote.hpp"
#include "PrivateNote.hpp"
#include "utils.hpp"
#include "Note.hpp"

using namespace std;

int main() {
    std::vector<Note*> public_notes, private_notes;
    loadNotesFromDirectory("notes/"+getFolderName(FolderType::PUBLIC), public_notes);
    loadNotesFromDirectory("notes/"+getFolderName(FolderType::PRIVATE), private_notes);

    // TODO: Write a small summary of the tool and what it does, alots of COUT
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
        // TODO: Add a functionality to create a new Note, very easy you just need to do like PrivateNote("heading", "content")
        cin >> choice;

        switch (choice) {
            case '1': {
                string query;
                cout << "Enter search query: ";
                cin.ignore(); // To consume the newline character left by cin
                getline(cin, query);
                // TODO: Change the searchNotes function to print the notes nicely in formatted form
                searchNotes(private_notes, query);
                break;
            }
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
	    case '3': {
            string heading, content;
                    cout << "Enter the HEADING of the new note:\n";
                    cin.ignore(); // To consume the newline character left by cin
                    getline(cin, heading);
            //cin >> heading;
            content = heading;
            //cout << "Enter the Content:\n";
            //cin >> content;
            
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
