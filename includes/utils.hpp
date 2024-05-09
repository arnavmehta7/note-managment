#pragma once
#include "utils.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "PublicNote.hpp"
#include "PrivateNote.hpp"
using namespace std;

// ENUMS, basically mean constants, instead of writing numbers, we use these
enum FolderType { PUBLIC, PRIVATE };

std::string getFolderName(FolderType type) {
    return type == PUBLIC ? "public" : "private";
}

// TRICKY: This function is a factory function that parses a note file and creates a note object
Note* createNoteFromFilename(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return nullptr;

    std::string heading, lastModified, contentLine, content;
    std::getline(file, heading); // Reads the heading line
    std::getline(file, lastModified); // Reads the last modified line
    std::getline(file, contentLine); // Skips the "Content:" line

    // Concatenate the rest of the file as the content
    while (std::getline(file, contentLine)) content += contentLine + "\n";

    // Remove the potential trailing newline
    if (!content.empty()) content.pop_back();

    // Extract the heading text
    heading = heading.substr(heading.find(":") + 2); // assumes "Heading: " is at the start
    
    Note* note;
    if (filepath.find("/public/") != std::string::npos) // npos means not found
        note = new PublicNote(heading, content);
    else if (filepath.find("/private/") != std::string::npos) // npos means not found
        note = new PrivateNote(heading, content);
    else return nullptr; // Unknown note type

    return note;
}

void loadNotesFromDirectory(const std::string& directory, std::vector<Note*>& notes) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        Note* note = createNoteFromFilename(entry.path());
        if (note != nullptr) notes.push_back(note);
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
    std::string path = "notes/" + getFolderName(f) + "/";
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

void text_to_lower(string &s) {        
    for(char &c : s) c = tolower(c);
}

std::vector<std::string> tokenize(std::string text) {
    std::vector<std::string> tokens;
    std::string token;
    text_to_lower(text);
    for (char c : text) {
        // If the character is not a space, add it to the current token
        if (c != ' ') token += c;
        else {
            // If a space is encountered, add the current token to the tokens vector
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
    }
    // Add the last token if the input string doesn't end with a space
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

int countOccurrences(const std::vector<std::string>& tokens, const std::string& content) {
    int count = 0;
    auto contentTokens = tokenize(content);
    std::unordered_map<std::string, int> wordFrequency;

    for (const auto& token : contentTokens) {
        wordFrequency[token]++;
    }

    for (const auto& queryToken : tokens) {
        count += wordFrequency[queryToken];
    }

    return count;
}

void searchNotes(const std::vector<Note*>& notes, const std::string& query) {
    std::vector<std::string> queryTokens = tokenize(query);
    std::vector<std::pair<Note*, int>> relevanceScores;

    // len of elements in notes
    cout << "Length of notes: " << notes.size() << endl;

    for (Note* note : notes) {
        int contentOccurrences = countOccurrences(queryTokens, note->getContent());
        relevanceScores.push_back({note, contentOccurrences});
    }

    std::sort(relevanceScores.begin(), relevanceScores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; // Sort in descending order of occurrences
    });

    // Display sorted results
    for (const auto& entry : relevanceScores) {
        std::cout << "Note: " << entry.first->getHeading() << " - Relevance: " << entry.second << std::endl;
    }
}
