#pragma once
#include <vector>
#include <string>
#include "03_Screen.cpp"
using namespace std;

// Composition: Cinema owns its Screens - a Screen has no meaning
// outside the Cinema it belongs to.
class Cinema {
private:
    string name;
    vector<Screen> screens;

public:
    explicit Cinema(string name) : name(name) {}

    void addScreen(Screen screen) { screens.push_back(screen); }
    vector<Screen>& getScreens() { return screens; }
    string getName() const { return name; }
};
