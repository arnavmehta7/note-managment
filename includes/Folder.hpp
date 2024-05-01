#pragma once
#include <vector>
#include <string>
#include "Note.hpp"

enum FolderType { PUBLIC, PRIVATE };

class Folder {
    std::vector<Note*> notes;
    FolderType type;

public:
    Folder(FolderType type);
    void addNote(Note* note);
    void deleteNote(const std::string& noteHeading);
    void display() const;
    ~Folder();

    static std::string getFolderName(FolderType type) {
        return type == PUBLIC ? "public" : "private";
    };
};