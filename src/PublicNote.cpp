#include "PublicNote.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

PublicNote::PublicNote(const string& heading, const string& content)
    : Note (heading, content) {}

void PublicNote::edit(const string& newContent) {
    cout << "Public Notes can never be edited";
}

void PublicNote::display() const {
    cout << ">>>>> Heading: " << heading << endl;
    cout << "Last Modified: " << ctime(&modificationTimestamp) << endl;
}

void PublicNote::save() const {
    cout << "Public Notes can never be saved" << endl;
    // filesystem::create_directories("notes/public"); // Ensure the directory exists
    // string filename = "notes/public/" + heading + ".txt";
    // ofstream file(filename);
    // if (!file) {
    //     cerr << "Error opening file for note: " << heading << endl;
    //     return;
    // }
    // file << "Last Modified: " << ctime(&modificationTimestamp);
}