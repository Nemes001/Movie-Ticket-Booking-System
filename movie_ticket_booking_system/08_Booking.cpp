#pragma once
#include <vector>
#include <string>
#include "05_Show.cpp"
#include "06_ShowSeat.cpp"
#include "07_Customer.cpp"
using namespace std;

enum class BookingStatus { PENDING, CONFIRMED, FAILED, CANCELLED };

inline string bookingStatusToString(BookingStatus status) {
    switch (status) {
        case BookingStatus::PENDING:   return "PENDING";
        case BookingStatus::CONFIRMED: return "CONFIRMED";
        case BookingStatus::FAILED:    return "FAILED";
        case BookingStatus::CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

// Aggregation: Booking references existing ShowSeats (borrowed, not owned) -
// if the Booking is cancelled, the ShowSeats survive and flip back to AVAILABLE.
// Association: Booking references a Customer without owning its lifecycle.
class Booking {
private:
    static int nextBookingId; // Static member: shared unique-id generator across all Bookings

    string bookingId;
    Show* show;
    Customer customer;
    vector<ShowSeat*> bookedSeats;
    double totalAmount;
    BookingStatus status;

public:
    Booking(Show* show, Customer customer, vector<ShowSeat*> bookedSeats, double totalAmount)
        : show(show), customer(customer), bookedSeats(bookedSeats),
          totalAmount(totalAmount), status(BookingStatus::PENDING) {
        bookingId = "BK" + to_string(1000 + nextBookingId);
        nextBookingId++;
    }

    string getBookingId() const { return bookingId; }
    Show* getShow() const { return show; }
    vector<ShowSeat*> getSeats() const { return bookedSeats; }
    double getTotalAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }

    // Encapsulation: status only changes through these controlled transitions
    void confirm() { status = BookingStatus::CONFIRMED; }
    void fail() { status = BookingStatus::FAILED; }
    void cancel() { status = BookingStatus::CANCELLED; }
};

int Booking::nextBookingId = 1;
