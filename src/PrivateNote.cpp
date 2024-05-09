#include "PrivateNote.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
PrivateNote::PrivateNote(const string& heading, const string& content)
    : Note (heading, content) {
        // save();
    }

void PrivateNote::edit(const string& heading) {
    // Open the text file in the default text editor
    string command = "open notes/private/" + heading + ".txt"; // For macOS
    cout << "Opening file in text editor...\n";
    int result = system(command.c_str());
    
    // Check if the system call was successful
    if (result == -1) {
        cerr << "Error: Failed to open text editor.\n";
        return;
    }
    time(&modificationTimestamp);
}


void PrivateNote::display() const {
    cout << ">>>>> Heading: " << heading << endl;
    // cout << "Content:\n" << content << endl;
    cout << "Last Modified: " << ctime(&modificationTimestamp) << endl;
}

void PrivateNote::save() const {
    filesystem::create_directories("notes/private"); // Ensure the directory exists
    string filename = "notes/private/" + heading + ".txt";
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for note: " << heading << endl;
        return;
    }
    file << "Heading: " << heading << "\n";
    file << "Last Modified: " << ctime(&modificationTimestamp);
    file << "Content:\n" << content;
}
