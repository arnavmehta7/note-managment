#pragma once
#include <vector>
#include <string>
#include "Note.hpp"

// ENUMS, basically mean constants, instead of writing numbers, we use these
enum FolderType { PUBLIC, PRIVATE };

class Folder {
    std::vector<Note*> notes;
    FolderType type;

public:
    Folder(FolderType type);
    // TODO: Remove these, these are redundant
    void addNote(Note* note);
    void display() const;
    ~Folder();

    static std::string getFolderName(FolderType type) {
        return type == PUBLIC ? "public" : "private";
    };
};