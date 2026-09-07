#pragma once
#include <vector>
#include "02_Seat.cpp"
using namespace std;

// Composition: Screen owns its Seats - if the Screen is removed, its
// specific seats go with it (they are not reassigned elsewhere).
class Screen {
private:
    int screenNumber;
    vector<Seat> seats;

public:
    Screen(int screenNumber, vector<Seat> seats)
        : screenNumber(screenNumber), seats(seats) {}

    int getScreenNumber() const { return screenNumber; }
    vector<Seat>& getSeats() { return seats; }
};
