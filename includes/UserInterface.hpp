#pragma once
#include "Folder.hpp"
#include "SearchEngine.hpp"

class UserInterface {
    Folder* currentFolder;
    SearchEngine searchEngine;

public:
    UserInterface();
    void displayFolderOptions() const;
    void addNoteToCurrentFolder();
    void displayNotes() const;
    void searchNotes() const;
    ~UserInterface();
};