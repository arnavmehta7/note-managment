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

void generateMetadataIfNotExisting(const vector<NoteAndWordsInfo*>& notes_aval) {
    // Load existing metadata
    ifstream inFile("metadata.json");
    json metadata;
    if (inFile.is_open()) {
        inFile >> metadata;
        inFile.close();
    }

    // Update metadata with new notes
    for (NoteAndWordsInfo* noteData : notes_aval) {
        // if the note's content is already in the metadata, skip it
        if (metadata.contains(getFolderName(noteData->type) + "/" + noteData->heading + ".txt")) {
            cout << "Note already in metadata: " << noteData->heading << endl;
            continue;
        }
        
        Note* note = loadNoteFromHeadingInDirectory("notes/"+getFolderName(noteData->type), noteData->heading);
        if (note == nullptr) continue;

        vector<string> tokens = tokenize(note->getContent());
        unordered_map<string, int> wordFrequency;
        string key;

        for (const auto& token : tokens) wordFrequency[token]++;
        if (dynamic_cast<PublicNote*>(note)) 
            key = getFolderName(FolderType::PUBLIC) + "/" + noteData->heading + ".txt";
        else
            key = getFolderName(FolderType::PRIVATE) + "/" + noteData->heading + ".txt";

        metadata[key] = wordFrequency;
    }

    // Write updated metadata back to file
    ofstream outFile("metadata.json");
    outFile << metadata.dump(2); // 4 spaces for indentation
    outFile.close();
}

// overwrite metadata function, takes one single note
void overwriteMetadata(Note* note) {
    ifstream inFile("metadata.json");
    json metadata;
    if (inFile.is_open()) {
        inFile >> metadata;
        inFile.close();
    }

    string content = note->getContent();
    vector<string> tokens = tokenize(content);
    unordered_map<string, int> wordFrequency;
    string key;

    for (const auto& token : tokens) wordFrequency[token]++;
    if (dynamic_cast<PublicNote*>(note)) 
        key = getFolderName(FolderType::PUBLIC) + "/" + note->getHeading() + ".txt";
    else
        key = getFolderName(FolderType::PRIVATE) + "/" + note->getHeading() + ".txt";

    metadata[key] = wordFrequency;

    ofstream outFile("metadata.json");
    outFile << metadata.dump(2); // 4 spaces for indentation
    outFile.close();
}

vector<NoteAndWordsInfo*> loadMetadata() {
    ifstream file("metadata.json");
    json metadata;
    file >> metadata;

    vector<NoteAndWordsInfo*> notes_available;
    for (auto& [key, value] : metadata.items()) {
        string heading = key.substr(key.find_last_of("/") + 1); // Extract the heading from the key
        heading = heading.substr(0, heading.find(".txt")); // Remove the file extension
        FolderType type = key.find("public") != string::npos ? FolderType::PUBLIC : FolderType::PRIVATE;

        unordered_map<string, int> wordFrequencies;
        for (auto& [word, frequency] : value.items()) {
            wordFrequencies[word] = frequency;
        }
        NoteAndWordsInfo* noteHeadingAndType = new NoteAndWordsInfo{heading, type, wordFrequencies};
        notes_available.push_back(noteHeadingAndType);
    }

    vector<FolderType> dirTypes = {FolderType::PUBLIC, FolderType::PRIVATE};
    for (const FolderType& type : dirTypes) {
        string dir = "notes/" + getFolderName(type);
        for (const auto& entry : filesystem::directory_iterator(dir)) {
            string path = entry.path().string();
            string filename = entry.path().filename();
            string heading = filename.substr(0, filename.find(".txt"));
            string key = getFolderName(type) + "/" + filename;

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
    vector <NoteAndWordsInfo*> notes_available = loadMetadata();
    generateMetadataIfNotExisting(notes_available);
    cout << "=================================="<<endl;
    cout << "Initialization Successful"<<endl;
    cout << "=================================="<<endl;
    cout << "Welcome to the note-taking application!\n";

    // TODO: Write a small summary of the tool and what it does, alots of COUT
    char choice;
    do {
        cout << "Choose an option:\n";
        cout << "1. Search notes\n";
        cout << "2. List notes\n";
	    cout << "3. Create a private note -- creates a blank file \n";
	    cout << "4. Modify a private note -- open's the file in your default editor\n";
        cout << "5. Delete a private note\n";
        cout << "6. Print note\n";
        cout << "7. Exit\n";
        cout << "Enter your choice (1-7): ";
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
                searchNotes(notes_available, query);
                break;
            }

            case '2': {
                for (auto& note : notes_available) {
                    cout << "Heading: " << note->heading << endl;
                    cout << "Type: " << (note->type == FolderType::PUBLIC ? "Public" : "Private") << endl;
                    cout << endl;
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
                    delete tempNote; // Free the memory with the outdated version of note

                    cout << "Enter 'y' if you saved the note: ";
                    char saveChoice;
                    cin >> saveChoice;
                    if (saveChoice == 'y') {
                        Note* note = loadNoteFromHeadingInDirectory("notes/private", heading);
                        overwriteMetadata(note);
                        cout << "Note modified successfully.\n";
                    }
                }
                break;
            }
            case '5': {
                string heading;
                cout << "Enter the heading of the private note to delete: ";
                cin.ignore(); // To consume the newline character left by cin
                getline(cin, heading);

                Note* tempNote = loadNoteFromHeadingInDirectory("notes/private", heading);
                if (tempNote != nullptr) {
                    deleteNoteFile(heading, FolderType::PRIVATE);
                    cout << "Note deleted successfully.\n";
                    delete tempNote;
                }
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
                    else {
                        cout << "Note not found.\n";
                    }
                }

                break;
            }
            case '7':
                cout << "Exiting application.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 5.\n";
        }
    } while (choice != '6');

    return 0;
}
