#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "PublicNote.hpp"
#include "PrivateNote.hpp"
#include "Folder.hpp"
#include "utils.hpp"

using namespace std;

int main() {
    // Load notes from the notes directory
    std::vector<Note*> public_notes, private_notes;
    loadNotesFromDirectory("notes/"+Folder::getFolderName(FolderType::PUBLIC), public_notes);
    loadNotesFromDirectory("notes/"+Folder::getFolderName(FolderType::PRIVATE), private_notes);

    // printing the notes
    for (const auto& note : private_notes) {
        note->display();
    }

    searchNotes(private_notes, "Reminder for school");
    
    // Delete a note
    // cout << "\nDeleting note 'Economics 101'..." << endl;
    // deleteNote(private_notes, "Economics 101", FolderType::PRIVATE);

    return 0;
}
