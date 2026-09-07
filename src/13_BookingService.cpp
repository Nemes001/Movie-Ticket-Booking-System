#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
using namespace std;

// Orchestrator only: BookingService calls PriceCalculator and Payment,
// it does not implement pricing or payment logic itself.
// Dependency inversion (Step H, D): BookingService depends on the Payment
// abstraction, never on a concrete UpiPayment/CardPayment/CashPayment type.
class BookingService {
private:
    map<string, Booking*> bookings;

public:
    Booking* bookSeats(Show* show, const vector<string>& seatNumbers, Customer customer,
                        int paymentMethod, bool simulateFailure) {
        vector<ShowSeat*> selected;

        // FR4: validate ALL seats before touching any state. Reject the
        // whole booking if any seat is invalid or already booked.
        for (const string& num : seatNumbers) {
            ShowSeat* ss = show->findShowSeat(num);
            if (ss == nullptr) {
                cout << "  Invalid seat number: " << num << ". Booking rejected, no changes made." << endl;
                return nullptr;
            }
            if (!ss->isAvailable()) {
                cout << "  Seat " << num << " is already booked. Booking rejected, no changes made." << endl;
                return nullptr;
            }
            selected.push_back(ss);
        }

        double total = PriceCalculator::calculateTotal(selected);

        Booking* booking = new Booking(show, customer, selected, total);
        bookings[booking->getBookingId()] = booking;

        Payment* payment = nullptr;
        if (paymentMethod == 1) payment = new UpiPayment(simulateFailure);
        else if (paymentMethod == 2) payment = new CardPayment(simulateFailure);
        else payment = new CashPayment(simulateFailure);

        bool paid = payment->pay(total); // runtime polymorphism: exact pay() picked at runtime
        delete payment;

        if (!paid) {
            // FR6: failed payment must NOT confirm the booking; seats stay free.
            booking->fail();
            cout << "  Booking " << booking->getBookingId()
                 << " NOT confirmed. Seats remain available." << endl;
            return booking;
        }

        for (ShowSeat* ss : selected) ss->book();
        booking->confirm();

        TicketPrinter::printTicket(booking);
        return booking;
    }

    bool cancelBooking(const string& bookingId) {
        auto it = bookings.find(bookingId);
        if (it == bookings.end()) {
            cout << "  No booking found with id " << bookingId << endl;
            return false;
        }

        Booking* booking = it->second;
        if (booking->getStatus() == BookingStatus::CANCELLED) {
            cout << "  Booking " << bookingId << " is already cancelled." << endl;
            return false;
        }
        if (booking->getStatus() != BookingStatus::CONFIRMED) {
            cout << "  Booking " << bookingId << " cannot be cancelled (status: "
                 << bookingStatusToString(booking->getStatus()) << ")" << endl;
            return false;
        }

        for (ShowSeat* ss : booking->getSeats()) ss->release();
        booking->cancel();
        cout << "  Booking " << bookingId << " cancelled. Seats released." << endl;
        return true;
    }

    Booking* getBooking(const string& bookingId) {
        auto it = bookings.find(bookingId);
        if (it == bookings.end()) return nullptr;
        return it->second;
    }
};
