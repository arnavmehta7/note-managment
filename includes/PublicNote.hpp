#pragma once
#include "Note.hpp"
using namespace std;
class PublicNote : public Note {
public:
    PublicNote(const string& heading, const string& content);
    void edit(const string& newContent) override;
    void display() const override;
    void save() const override;
};