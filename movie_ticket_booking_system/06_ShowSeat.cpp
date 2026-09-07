#pragma once
#include "02_Seat.cpp"

enum class SeatStatus { AVAILABLE, BOOKED };

// Why ShowSeat and not just Seat? Seat A1 exists once on the screen, but its
// status differs per show - A1 may be BOOKED for the 6 PM show and AVAILABLE
// for the 9 PM show. Status belongs to the show, not the physical chair.
class ShowSeat {
private:
    Seat seat;
    SeatStatus status;

public:
    explicit ShowSeat(Seat seat) : seat(seat), status(SeatStatus::AVAILABLE) {}

    Seat getSeat() const { return seat; }
    bool isAvailable() const { return status == SeatStatus::AVAILABLE; }
    SeatStatus getStatus() const { return status; }

    // Encapsulation: status only changes through these controlled methods
    void book() { status = SeatStatus::BOOKED; }
    void release() { status = SeatStatus::AVAILABLE; }
};
