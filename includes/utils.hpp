#pragma once
#include "utils.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "PublicNote.hpp"
#include "PrivateNote.hpp"
#include "Folder.hpp"
using namespace std;

Note* createNoteFromFilename(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return nullptr;

    std::string heading, lastModified, contentLine, content;
    std::getline(file, heading); // Reads the heading line
    std::getline(file, lastModified); // Reads the last modified line
    std::getline(file, contentLine); // Skips the "Content:" line

    // Concatenate the rest of the file as the content
    while (std::getline(file, contentLine)) {
        content += contentLine + "\n";
    }

    // Remove the potential trailing newline
    if (!content.empty()) content.pop_back();

    // Extract the heading text
    heading = heading.substr(heading.find(":") + 2); // assumes "Heading: " is at the start
    
    Note* note;
    if (filepath.find("/public/") != std::string::npos) {
        note = new PublicNote(heading, content);
    } else if (filepath.find("/private/") != std::string::npos) {
        note = new PrivateNote(heading, content);
    } else {
        return nullptr; // Unknown note type
    }

    return note;
}

void loadNotesFromDirectory(const std::string& directory, std::vector<Note*>& notes) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        Note* note = createNoteFromFilename(entry.path());
        if (note != nullptr) {
            notes.push_back(note);
        }
    }
}

void deleteNoteFromMemory(vector<Note*>& notes, const std::string& heading) {
    auto it = std::find_if(notes.begin(), notes.end(), [&](const Note* note) {
        string noteHeading = note->getHeading();
        cout << "Comparing: " << noteHeading << " with " << heading << endl;
        return noteHeading == heading;
    });

    // If the note is found, delete it from memory
    if (it != notes.end()) {
        delete *it;
        notes.erase(it);
    }
}

void deleteNoteFile(const std::string& heading, FolderType f) {
    std::string path = "notes/" + Folder::getFolderName(f) + "/";
    path += heading + ".txt";
    cout << "Deleting file: " << path << endl;
    if (!std::filesystem::exists(path)) {
        cout << "File does not exist." << endl;
        return;
    }
    std::filesystem::remove(path);
}

void deleteNote(vector<Note*>& notes, const std::string& heading, FolderType f) {
    deleteNoteFromMemory(notes, heading);
    deleteNoteFile(heading, f);
}

