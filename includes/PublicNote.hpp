#pragma once
#include "Note.hpp"

class PublicNote : public Note {
public:
    PublicNote(const std::string& heading, const std::string& content);
    void edit(const std::string& newContent) override;
    void display() const override;
    void save() const override;
};