#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "PublicNote.hpp"
#include "PrivateNote.hpp"
#include "utils.hpp"
#include "Note.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// overwrite metadata function, takes one single note and it is ALWAYS PRIVATE
void overwriteMetadata(Note* note) {
    ifstream inFile("metadata.json");
    json metadata;
    inFile >> metadata;

    string content = note->getContent();
    vector<string> tokens = tokenize(content);
    unordered_map<string, int> wordFrequency;
    string key;

    for (const string& token : tokens) wordFrequency[token]++;

    // if (dynamic_cast<PublicNote*>(note)) 
    //     key = getFolderName(FolderType::PUBLIC) + "/" + note->getHeading() + ".txt";
    key = getFolderName(FolderType::PRIVATE) + "/" + note->getHeading() + ".txt";

    metadata[key] = wordFrequency;

    ofstream outFile("metadata.json");
    outFile << metadata.dump(2); // 4 spaces for indentation
    outFile.close();
    inFile.close(); // we should close a file in the end
}

vector<NoteAndWordsInfo*> loadMetadata() {
    ifstream file("metadata.json"); // ifstream means input file stream
    json metadata; // object from json's class
    file >> metadata;

    // just parse the metadata file and create a vector of NoteAndWordsInfo objects
    vector<NoteAndWordsInfo*> notes_available;
    for (auto& [key, value] : metadata.items()) {
        // key filename - public/<name>.txt
        // value will be {word1: <freq>, ...}
        string heading = key.substr(key.find_last_of("/") + 1); // Extract the heading from the key
        heading = heading.substr(0, heading.find(".txt")); // Remove the file extension
        FolderType type = key.find("public") != string::npos ? FolderType::PUBLIC : FolderType::PRIVATE;  // npos means not found

        unordered_map<string, int> wordFrequencies;
        // {word: int, ...}
        // {}
        // {word1: 2}
        // {word1: 2, word2: 3}
        for (auto& [word, frequency] : value.items()) wordFrequencies[word] = frequency;

        NoteAndWordsInfo* noteHeadingAndType = new NoteAndWordsInfo{heading, type, wordFrequencies};
        notes_available.push_back(noteHeadingAndType);
    }


    vector<FolderType> dirTypes = {FolderType::PUBLIC, FolderType::PRIVATE};
    for (const FolderType& type : dirTypes) {
        string dir = "notes/" + getFolderName(type);
        for (const auto& entry : filesystem::directory_iterator(dir)) {
            string filename = entry.path().filename();
            string heading = filename.substr(0, filename.find(".txt"));
            string key = getFolderName(type) + "/" + filename;
            // private/FileName.txt
            if (!metadata.contains(key)) {
                cout << "Generating metadata for note: " << heading << endl; 
                unordered_map<string, int> wordFrequency;

                Note* note = loadNoteFromHeadingInDirectory(dir, heading);
                for (string& token : tokenize(note->getContent()))
                    wordFrequency[token]++;
                delete note;

                metadata[key] = wordFrequency;
                notes_available.push_back(new NoteAndWordsInfo{heading, type, wordFrequency});
            }
        }
    }
    // saving the metadata
    ofstream outFile("metadata.json");
    outFile << metadata.dump(2);
    // closing the files
    outFile.close();
    file.close();
    return notes_available;

}


int main() {
    /*
    1. No need of creating public/private notes vectors.
    2. Load metadata from metadata.json file and populate notes_available vector.
    3. Use notes_available vector to display notes and search notes.
    4. Create, modify and delete notes using the notes_available vector.
    5. Update metadata.json file after creating, modifying and deleting notes.
    */
//  just read the metadata file
//  confirm if all notes in directory are present in metadata file
    vector <NoteAndWordsInfo*> notes_available = loadMetadata();
    cout << "=================================="<<endl;
    cout << "Initialization Successful"<<endl;
    cout << "=================================="<<endl;
    cout << "\nWelcome to ASE!\n";

    // TODO: Write a small summary of the tool and what it does, alots of COUT
    char choice;
    do {
        cout << "\nHow may I help you?\n";
        cout << "1. Search \n";
        cout << "2. Show all notes \n";
        cout << "3. Add new note\n";
        cout << "4. Edit notes\n";
        cout << "5. Remove notes\n";
        cout << "6. Print note\n";
        cout << "7. Exit\n";
        cout << "Your choice (1-6): ";
        cin >> choice;
        cout << endl;

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
                searchNotes(notes_available, query);
                break;
            }

            case '2': {
                // Showing all public notes first and then private notes
                cout << ">>>>> Public Notes:\n";
                cout << endl;
                for (auto& note : notes_available) {
                    if (note->type == FolderType::PUBLIC) {
                        cout << "Heading: " << note->heading << endl;
                        cout << endl;
                    }
                }
                cout << endl;
                
                cout << ">>>>> Private Notes:\n";
                cout << endl;
                for (auto& note : notes_available) {
                    if (note->type == FolderType::PRIVATE) {
                        cout << "Heading: " << note->heading << endl;
                        cout << endl;
                    }
                }
                break;
            }

            // this is for creation of new note
            // we take heading as input and put it in heading - we assign heading to content and make a new note (dynamic allocation)
            // we save it and note content is saved 
            // private_note object is pushed back into private_note vector 
	        case '3': {
                string heading;
                cout << "Enter the HEADING of the new note:\n";
                cin.ignore(); 
                getline(cin, heading);
                Note* note = new PrivateNote(heading, "");
                note->save();
                notes_available.push_back(new NoteAndWordsInfo{heading, FolderType::PRIVATE, {}});
                overwriteMetadata(note);    
                delete note;
                break;
            }
	        case '4': {
                string heading;
                cout << "Enter the heading private note to modify: ";
                cin.ignore(); // To consume the newline character left by cin
                getline(cin, heading);
                Note* tempNote = loadNoteFromHeadingInDirectory("notes/private", heading);
                if (tempNote != nullptr) {
                    tempNote->edit(heading);
                    delete tempNote; //x Free the memory with the outdated version of note

                    cout << "Enter 'y' if you saved the note: ";
                    char saveChoice;
                    cin >> saveChoice;
                    if (saveChoice == 'y') {
                        Note* note = loadNoteFromHeadingInDirectory("notes/private", heading);
                        overwriteMetadata(note);
                        // update the notes_available vector, by deleting the old note and adding the new note
                        for (auto& note : notes_available) {
                            if (note->heading == heading) {
                                delete note;
                                note = new NoteAndWordsInfo{heading, FolderType::PRIVATE, {}};
                                break;
                            }
                        }
                        cout << "Note modified successfully.\n";
                    }
                }
                else cout << "Sorry, Note not found.\n";
                break;
            }
            case '5': {
                string heading;
                cout << "Enter the heading of the private note to delete: ";
                cin.ignore(); // To consume the newline character left by cin
                getline(cin, heading);
                deleteNoteFile(heading, FolderType::PRIVATE);
                break;
            }
            case '6': {
                string heading;
                cout << "Enter the heading of the note to print: ";
                cin.ignore(); // To consume the newline character left by cin
                getline(cin, heading);

                Note* note = loadNoteFromHeadingInDirectory("notes/private", heading);
                if (note != nullptr) {
                    note->print();
                    delete note;
                }
                else {
                    Note* note = loadNoteFromHeadingInDirectory("notes/public", heading);
                    if (note != nullptr) {
                        note->print();
                        delete note;
                    }
                    else cout << "Note not found.\n";
                }
                break;
            }
            case '7':
                cout << "\nBa Bye, happy learning!\n\nn";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 7.\n";
        }
    } while (choice != '7');

    return 0;
}
