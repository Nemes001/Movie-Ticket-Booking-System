#pragma once
#include <string>
using namespace std;

enum class SeatType { SILVER, GOLD, PLATINUM };

// Constants instead of magic numbers (clean code checklist)
const double SILVER_PRICE = 150.0;
const double GOLD_PRICE = 250.0;
const double PLATINUM_PRICE = 400.0;

inline string seatTypeToString(SeatType type) {
    switch (type) {
        case SeatType::SILVER:   return "SILVER";
        case SeatType::GOLD:     return "GOLD";
        case SeatType::PLATINUM: return "PLATINUM";
    }
    return "UNKNOWN";
}

inline double priceForSeatType(SeatType type) {
    switch (type) {
        case SeatType::SILVER:   return SILVER_PRICE;
        case SeatType::GOLD:     return GOLD_PRICE;
        case SeatType::PLATINUM: return PLATINUM_PRICE;
    }
    return 0.0;
}

// Encapsulation: seat never knows its booking status - that belongs to ShowSeat
class Seat {
private:
    string seatNumber;
    SeatType seatType;

public:
    Seat(string seatNumber, SeatType seatType)
        : seatNumber(seatNumber), seatType(seatType) {}

    string getSeatNumber() const { return seatNumber; }
    SeatType getSeatType() const { return seatType; }
};
