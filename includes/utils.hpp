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


struct NoteHeadingAndType {
    string heading;
    FolderType type;
    unordered_map<string, int> wordFrequencies;
};

string getFolderName(FolderType type) {
    return type == PUBLIC ? "public" : "private";
}

// TRICKY: This function is a factory function that parses a note file and creates a note object
Note* createNoteFromFilename(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) return nullptr;

    string heading, lastModified, contentLine, content;
    getline(file, heading); // Reads the heading line
    getline(file, lastModified); // Reads the last modified line
    getline(file, contentLine); // Skips the "Content:" line

    // Concatenate the rest of the file as the content
    while (getline(file, contentLine)) content += contentLine + "\n";

    // Remove the potential trailing newline
    if (!content.empty()) content.pop_back();

    // Extract the heading text
    heading = heading.substr(heading.find(":") + 2); // assumes "Heading: " is at the start
    
    Note* note;
    if (filepath.find("/public/") != string::npos) // npos means not found
        note = new PublicNote(heading, content);
    else if (filepath.find("/private/") != string::npos) // npos means not found
        note = new PrivateNote(heading, content);
    else return nullptr; // Unknown note type

    return note;
}

Note* loadNoteFromHeadingInDirectory(const string& directory, const string& heading) {
    for (const auto& entry : filesystem::directory_iterator(directory)) {
        string filename = entry.path().filename();
        if (filename == heading + ".txt") return createNoteFromFilename(entry.path());
    }
    return nullptr;
}

void deleteNoteFile(const string& heading, FolderType f) {
    string path = "notes/" + getFolderName(f) + "/";
    path += heading + ".txt";
    cout << "Deleting file: " << path << endl;
    if (!filesystem::exists(path)) {
        cout << "File does not exist." << endl;
        return;
    }
    filesystem::remove(path);
}

// tokenize function - takes a string of text as input , splits it into individual tokens (words) using isstringstream, 
// coverts each token to lowercase and stores all tokens in vector of string objects which is then returned

// takes text - const reference to string and returns a vector of string objects

// isistringstream defined in <sstream> header file - we are creating iss which is input string stream object 
// we are initialising it with the content of text string - input string
// it extracts data from the string and reads it as if it were a stream - sequence of bytes 

// tokens (words) - empty vector of string
// string token - stores each word extracted from text

// while loop continues as long as there are tokens remaining to be extracted from iss
// iss >> token extracts next token from the stream and stores it in token string variable

// then we have a normalisation operation that converts all characters to lowercase starts from begin to end and stores it in begin
// transform - function that applies the operation to each element in input range

// Lambda expression - concise way to define anonymous function objects
// it is passed to the fourth argument to transform 
// [] is the capture clause - empty - it doesnt capture any external variables from surrounding scope
// (unsigned char c) - parameter list - takes c - int type - 0-255 - handles all ASCII char for tolower func
// then is function body - returns the result of calling tolower()
// tolower <cctype> header - this converts all uppercases to lowercases
// then we add the lowercase token to the tokens vector
// return tokens - returns the vector of lowercases 

void text_to_lower(string &s) {for(char &c : s) c = tolower(c);}

vector<string> tokenize(string text) {
    vector<string> tokens;
    string token;
    text_to_lower(text);
    for (char c : text) {
        // If the character is not a space, add it to the current token
        if (c != ' ' && c !='\n' && c != '\t' && c !='\r') token += c;
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

// countOccurrences function - returns an int - takes 2 arguments - tokens - const reference to vector of strings
// initialise the count to 0
// call tokenize function with content string and it stores the resulting vector of strings contentTokens
// unordered map of string (word) and int (frequency of that word) - wordFrequency
// for loop that iterates over token in contentToken vector - for each token it increments the value of int in wordFrequency map
// if token is not already present in the map, a new key value pair is created in the map with initial value set to 1
// for loop that iterates over queryToken in tokens vector - it looks up the frequency of the queryToken in map and adds it to count
// returns count - total occurences of token from tokens vector in content string


int countOccurrences(const vector<string>& tokens, const unordered_map<string, int>& wordFrequencies) {
    int count = 0;
    for (const auto& queryToken : tokens) {
        if (wordFrequencies.find(queryToken) != wordFrequencies.end())
            count += wordFrequencies.at(queryToken);
    }
    cout << "Count: " << count << endl;
    return count;
}

// searchNotes function takes 2 arguments- 
// 1. notes- const reference to a vector of Note pointers
// 2. query- const reference to string 
// tokenize function is called with a query as argument
// relevanceScores - vector which is a pair of Note pointer and int - stores score of each note based on occurence of query tokens
// prints the lenght of the vector
// for loop - iterates over each Note pointer in notes vector
// getContent() - user defined function for note's content
// countOccurences (int) - which is passed with queryTokens vector and string object note->getContent()
// stored in variable contentOcuurences
// note pointer and the count from contentOccurence is a pair pushed back into vector relevanceScores
// sort lambda function - range begin to end - empty clause, a and b are temporary variables 
// it's also a comparison function - compares the second element of the pair (count) 
// for loop that iterates over entry pair - sorted pair - in relevanceScores vector
// couts the heading and count

// void searchNotes(const vector<Note*>& notes, const string& query) {
void searchNotes(const vector<NoteHeadingAndType*>& notes_available, const string& query) {
    vector<string> queryTokens = tokenize(query);
    vector<pair<NoteHeadingAndType*, int>> relevanceScores;

    cout << "Length of notes: " << notes_available.size() << endl;

    for (NoteHeadingAndType* note : notes_available) {
        int contentOccurrences = countOccurrences(queryTokens, note->wordFrequencies);
        relevanceScores.push_back({note, contentOccurrences});
    }

    sort(relevanceScores.begin(), relevanceScores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;  
    });

    
    for (const auto& entry : relevanceScores)
        cout << "Note: " << entry.first->heading << " - Relevance: " << entry.second << endl;
}