#include "UserInterface.hpp"
#include <iostream>

UserInterface::UserInterface() {
    currentFolder = nullptr; // Initially no folder is selected
}

void UserInterface::displayFolderOptions() const {
    // Placeholder method
    std::cout << "Displaying folder options..." << std::endl;
}

void UserInterface::addNoteToCurrentFolder() {
    // Placeholder method
    std::cout << "Adding a note to the current folder..." << std::endl;
}

void UserInterface::displayNotes() const {
    if (currentFolder) {
        currentFolder->display();
    } else {
        std::cout << "No folder selected." << std::endl;
    }
}

void UserInterface::searchNotes() const {
    // Placeholder logic for search
    std::cout << "Searching notes..." << std::endl;
}

UserInterface::~UserInterface() {
    delete currentFolder;
}