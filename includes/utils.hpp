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
#include "Folder.hpp"

#include <map>

using namespace std;

// TRICKY: This function is a factory function that parses a note file and creates a note object
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


std::vector<std::string> tokenize(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        // Simple normalization: convert to lowercase
        std::transform(token.begin(), token.end(), token.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        tokens.push_back(token);
    }
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
    

    // Define a struct to hold metadata
struct Metadata {
    std::string author;
    std::string creationDate;
    std::vector<std::string> keywords;
    // Add more fields as needed
};

// Function to parse text file and extract metadata
Metadata extractMetadata(const std::string& filePath) {
    Metadata metadata;
    std::ifstream file(filePath);
    std::string line;

    // Read each line of the file
    while (std::getline(file, line)) {
        // Example: Extract creation date from a line starting with "Date:"
        if (line.find("Date:") == 0) {
            metadata.creationDate = line.substr(6); // Assuming date starts from index 6
        }
        // Example: Extract keywords from a line starting with "Keywords:"
        else if (line.find("Keywords:") == 0) {
            // Split the line by comma and add keywords to the vector
            size_t pos = line.find(":");
            std::string keywordList = line.substr(pos + 1);
            size_t startPos = 0;
            while ((pos = keywordList.find(',', startPos)) != std::string::npos) {
                metadata.keywords.push_back(keywordList.substr(startPos, pos - startPos));
                startPos = pos + 1;
            }
            // Add the last keyword
            metadata.keywords.push_back(keywordList.substr(startPos));
        }
        // Add more conditions for other metadata fields
    }

    return metadata;
}

// Function to read metadata and build index
std::map<std::string, std::pair<int, int>> buildIndex(const std::string& metadataFile) {
    std::map<std::string, std::pair<int, int>> index;
    std::ifstream meta(metadataFile);
    if (!meta.is_open()) {
        std::cerr << "Error opening metadata file\n";
        return index;
    }

    std::string word;
    int start, end;
    while (meta >> word >> start >> end) {
        index[word] = std::make_pair(start, end);
    }

    meta.close();
    return index;
}

// Function to search for a word in text file using metadata
std::string searchWord(const std::string& word, const std::string& textFile, const std::string& metadataFile) {
    std::map<std::string, std::pair<int, int>> index = buildIndex(metadataFile);
    std::ifstream text(textFile);
    if (!text.is_open()) {
        std::cerr << "Error opening text file\n";
        return "";
    }

    if (index.find(word) == index.end()) {
        return "Word not found";
    }

    std::pair<int, int> location = index[word];
    text.seekg(location.first);
    std::string foundWord;
    char ch;
    for (int i = location.first; i <= location.second && text.get(ch); ++i) {
        foundWord += ch;
    }

    text.close();
    return foundWord;
}

// Function to create metadata text file
void createMetadataFile(const std::string& metadataFilePath, const Metadata& metadata)
{
    std::ofstream metadataFile(metadataFilePath);
    if (!metadataFile.is_open()) {
        std::cerr << "Error creating metadata file\n";
        return;
    }

    metadataFile << "Author: " << metadata.author << std::endl;
    metadataFile << "Date: " << metadata.creationDate << std::endl;
    metadataFile << "Keywords: ";
    for (const auto& keyword : metadata.keywords) {
        metadataFile << keyword << ", ";
    }
    metadataFile << std::endl;

    metadataFile.close();
}