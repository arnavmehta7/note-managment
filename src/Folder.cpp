#include "Folder.hpp"
#include "Note.hpp"
#include <iostream>


Folder::Folder(FolderType type):
    type(type) {}

void Folder::addNote(Note* note) {
    notes.push_back(note);
}

void Folder::deleteNote(const std::string& noteHeading) {
    notes.erase(std::remove_if(notes.begin(), notes.end(),
                               [&](Note* note){ return note->getHeading() == noteHeading; }),
                notes.end());
}

void Folder::display() const {
    for (const auto& note : notes) {
        note->display();
    }
}

Folder::~Folder() {
    for (Note* note : notes) {
        delete note;
    }
}