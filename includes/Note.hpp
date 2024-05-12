#pragma once
#include <string>
#include <ctime>
#include <filesystem>
#include <iostream>
using namespace std;
class Note {
protected:
    string heading;
    string content;
    time_t modificationTimestamp;

public:
    Note(const string& heading, const string& content);
    virtual void edit(const string& newContent) = 0;
    virtual void display() const = 0;
    virtual ~Note() = default;
    
    virtual void save() const = 0;
    void print() const {
        cout << ">>>>> Heading: " << heading << endl;
        cout << content << endl;
    }

    // get heading
    string getHeading() const {
        return heading;
    };

    string getContent() const {
        return content;
    };

};
