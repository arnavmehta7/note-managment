#include "PublicNote.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

PublicNote::PublicNote(const string& heading, const string& content)
    : Note (heading, content) {}

void PublicNote::edit(const string& newContent) {
    content = newContent;
    time(&modificationTimestamp); // update modification timestamp
    save(); // save the note after editing
}

void PublicNote::display() const {
    cout << ">>>>> Heading: " << heading << endl;
    cout << "Last Modified: " << ctime(&modificationTimestamp) << endl;
}

void PublicNote::save() const {
    filesystem::create_directories("notes/public"); // Ensure the directory exists
    string filename = "notes/public/" + heading + ".txt";
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for note: " << heading << endl;
        return;
    }
    file << "Heading: " << heading << "\n";
    file << "Last Modified: " << ctime(&modificationTimestamp);
    file << "Content:\n" << content;
}