#pragma once
#include "Note.hpp"
using namespace std;
class PrivateNote : public Note {
public:
    PrivateNote(const string& heading, const string& content);
    void edit(const string& heading) override;
    void display() const override;
    void save() const override;
};
