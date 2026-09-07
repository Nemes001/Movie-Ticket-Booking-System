#pragma once
#include <vector>
#include <string>
#include "01_Movie.cpp"
#include "03_Screen.cpp"
#include "06_ShowSeat.cpp"
using namespace std;

// Aggregation: Show borrows a Movie and a Screen - cancelling this Show
// does not destroy either of them.
// Composition: Show owns its ShowSeats - they only mean something for
// this specific screening.
class Show {
private:
    Movie movie;
    Screen screen;
    string startTime;
    vector<ShowSeat> showSeats;

public:
    Show(Movie movie, Screen screen, string startTime)
        : movie(movie), screen(screen), startTime(startTime) {
        for (Seat& s : this->screen.getSeats()) {
            showSeats.push_back(ShowSeat(s));
        }
    }

    Movie getMovie() const { return movie; }
    Screen getScreen() const { return screen; }
    string getStartTime() const { return startTime; }
    vector<ShowSeat>& getShowSeats() { return showSeats; }

    ShowSeat* findShowSeat(const string& seatNumber) {
        for (ShowSeat& ss : showSeats) {
            if (ss.getSeat().getSeatNumber() == seatNumber) return &ss;
        }
        return nullptr;
    }
};
