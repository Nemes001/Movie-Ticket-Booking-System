#pragma once
#include <iostream>
#include "08_Booking.cpp"
using namespace std;

// Single responsibility: printing only. No side effects - never modifies
// booking, seat, or payment state.
class TicketPrinter {
public:
    static void printTicket(Booking* booking) {
        vector<ShowSeat*> seats = booking->getSeats();

        cout << "\n  ================ TICKET ================" << endl;
        cout << "  Booking ID : " << booking->getBookingId() << endl;
        cout << "  Movie      : " << booking->getShow()->getMovie().getTitle() << endl;
        cout << "  Screen     : Screen-" << booking->getShow()->getScreen().getScreenNumber()
             << "  " << booking->getShow()->getStartTime() << endl;

        cout << "  Seats      : ";
        for (size_t i = 0; i < seats.size(); i++) {
            cout << seats[i]->getSeat().getSeatNumber();
            if (i != seats.size() - 1) cout << ", ";
        }
        cout << endl;

        cout << "  Amount     : Rs." << booking->getTotalAmount()
             << "\tStatus: " << bookingStatusToString(booking->getStatus()) << endl;
        cout << "  =========================================\n" << endl;
    }
};
