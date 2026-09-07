#include <iostream>
#include <vector>
#include <sstream>
#include "01_Movie.cpp"
#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "13_BookingService.cpp"
using namespace std;

// Builds the standard 9-seat layout used for every screen in this demo.
vector<Seat> makeSeats() {
    vector<Seat> seats;
    seats.push_back(Seat("A1", SeatType::SILVER));
    seats.push_back(Seat("A2", SeatType::SILVER));
    seats.push_back(Seat("A3", SeatType::SILVER));
    seats.push_back(Seat("A4", SeatType::SILVER));
    seats.push_back(Seat("B1", SeatType::GOLD));
    seats.push_back(Seat("B2", SeatType::GOLD));
    seats.push_back(Seat("B3", SeatType::GOLD));
    seats.push_back(Seat("C1", SeatType::PLATINUM));
    seats.push_back(Seat("C2", SeatType::PLATINUM));
    return seats;
}

// Pure display function - no side effects, never books anything (clean code checklist).
void printSeatLayout(Show& show) {
    cout << "\n  SCREEN-" << show.getScreen().getScreenNumber() << "  " << show.getStartTime()
         << "  |  " << show.getMovie().getTitle() << endl;

    SeatType types[] = { SeatType::SILVER, SeatType::GOLD, SeatType::PLATINUM };
    for (SeatType t : types) {
        cout << "  " << seatTypeToString(t) << "\t";
        for (ShowSeat& ss : show.getShowSeats()) {
            if (ss.getSeat().getSeatType() == t) {
                cout << ss.getSeat().getSeatNumber() << (ss.isAvailable() ? "[ ] " : "[X] ");
            }
        }
        cout << endl;
    }
    cout << "\n  ( [ ] = available   [X] = booked )\n" << endl;
}

vector<string> splitSeats(const string& input) {
    vector<string> result;
    stringstream ss(input);
    string token;
    while (getline(ss, token, ',')) {
        while (!token.empty() && token.front() == ' ') token.erase(token.begin());
        while (!token.empty() && token.back() == ' ') token.pop_back();
        if (!token.empty()) result.push_back(token);
    }
    return result;
}

int main() {
    Cinema cinema("PVR");
    cinema.addScreen(Screen(1, makeSeats()));
    cinema.addScreen(Screen(2, makeSeats()));

    Movie movie1("3 Idiots", "Hindi", 170);
    Movie movie2("Interstellar", "English", 169);
    vector<Movie> movies = { movie1, movie2 };

    vector<Show> shows;
    shows.push_back(Show(movie1, cinema.getScreens()[0], "06:00 PM"));
    shows.push_back(Show(movie1, cinema.getScreens()[1], "09:00 PM"));
    shows.push_back(Show(movie2, cinema.getScreens()[0], "10:00 PM"));

    BookingService bookingService;
    Customer customer("Guest", "9999999999");

    cout << "===== MOVIE TICKET BOOKING =====" << endl;

    int choice = -1;
    while (choice != 0) {
        cout << "\n1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit" << endl;
        cout << "Choose: ";

        if (!(cin >> choice)) {
            // Edge case 4: invalid menu choice never crashes the program
            cout << "  Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            cout << endl;
            for (size_t i = 0; i < movies.size(); i++) {
                cout << "  [" << (i + 1) << "] " << movies[i].getTitle() << "\t"
                     << movies[i].getLanguage() << "  " << movies[i].getDuration() << " min" << endl;
            }

        } else if (choice == 2) {
            cout << "\nChoose movie: ";
            int m;
            if (!(cin >> m) || m < 1 || m > (int)movies.size()) {
                cout << "  Invalid movie choice." << endl;
                cin.clear(); cin.ignore(10000, '\n');
                continue;
            }
            Movie& chosenMovie = movies[m - 1];

            vector<Show*> matchingShows;
            for (Show& s : shows) {
                if (s.getMovie().getTitle() == chosenMovie.getTitle()) matchingShows.push_back(&s);
            }
            if (matchingShows.empty()) {
                cout << "  No shows for this movie." << endl;
                continue;
            }

            for (size_t i = 0; i < matchingShows.size(); i++) {
                cout << "  [" << (i + 1) << "] Screen-" << matchingShows[i]->getScreen().getScreenNumber()
                     << "  " << matchingShows[i]->getStartTime() << endl;
            }
            cout << "Choose show: ";
            int sIdx;
            if (!(cin >> sIdx) || sIdx < 1 || sIdx > (int)matchingShows.size()) {
                cout << "  Invalid show choice." << endl;
                cin.clear(); cin.ignore(10000, '\n');
                continue;
            }
            Show* chosenShow = matchingShows[sIdx - 1];

            printSeatLayout(*chosenShow);

            cin.ignore();
            cout << "Seats (e.g. A1,B2): ";
            string seatInput;
            getline(cin, seatInput);
            vector<string> seatNums = splitSeats(seatInput);
            if (seatNums.empty()) {
                cout << "  No seats entered." << endl;
                continue;
            }

            cout << "Pay by: 1.UPI  2.Card  3.Cash > ";
            int payMethod;
            if (!(cin >> payMethod) || payMethod < 1 || payMethod > 3) {
                cout << "  Invalid payment method." << endl;
                cin.clear(); cin.ignore(10000, '\n');
                continue;
            }

            cout << "Simulate a failed payment for this demo? 1.No  2.Yes > ";
            int simChoice;
            cin >> simChoice;
            bool simulateFailure = (simChoice == 2);

            bookingService.bookSeats(chosenShow, seatNums, customer, payMethod, simulateFailure);

        } else if (choice == 3) {
            cin.ignore();
            cout << "Enter booking id to cancel: ";
            string id;
            getline(cin, id);
            bookingService.cancelBooking(id);

        } else if (choice == 4) {
            cin.ignore();
            cout << "Enter booking id to view: ";
            string id;
            getline(cin, id);
            Booking* b = bookingService.getBooking(id);
            if (b == nullptr) cout << "  No booking found." << endl;
            else TicketPrinter::printTicket(b);

        } else if (choice == 0) {
            cout << "  Goodbye!" << endl;

        } else {
            cout << "  Invalid choice." << endl;
        }
    }

    return 0;
}
