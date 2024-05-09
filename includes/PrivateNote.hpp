#pragma once
#include "Note.hpp"

class PrivateNote : public Note {
public:
    PrivateNote(const std::string& heading, const std::string& content);
    void edit(const std::string& heading) override;
    void display() const override;
    void save() const override;
};
