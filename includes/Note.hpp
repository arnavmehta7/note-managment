#pragma once
#include <string>
#include <ctime>
#include <filesystem>

class Note {
protected:
    std::string heading;
    std::string content;
    std::time_t modificationTimestamp;

public:
    Note(const std::string& heading, const std::string& content);
    virtual void edit(const std::string& newContent) = 0;
    virtual void display() const = 0;
    virtual ~Note() = default;
    
    virtual void save() const = 0;

    // get heading
    std::string getHeading() const {
        return heading;
    };

    std::string getContent() const {
        return content;
    };

};
