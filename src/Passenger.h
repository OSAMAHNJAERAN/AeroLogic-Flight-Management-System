#ifndef PASSENGER_H
#define PASSENGER_H

#include "Constants.h"
#include <string>

// ============================================================================
// PASSENGER STRUCTURE
// ============================================================================
struct Passenger {
  std::string name;
  std::string seatNumber;
  std::string classType;
  int row;
  int col;
  bool isBooked;
  std::string bookingDate;
  std::string bookingTime;
  std::string origin;
  std::string destination;
  int flightId;

  // Default constructor
  Passenger()
      : name(""), seatNumber(""), classType(""), row(-1), col(-1),
        isBooked(false), bookingDate(""), bookingTime(""), origin(""),
        destination(""), flightId(-1) {}

  // Parameterized constructor
  Passenger(std::string n, std::string seat, std::string cls, int r, int c,
            std::string date, std::string time, std::string org,
            std::string dest, int fId)
      : name(n), seatNumber(seat), classType(cls), row(r), col(c),
        isBooked(true), bookingDate(date), bookingTime(time), origin(org),
        destination(dest), flightId(fId) {}
};

#endif // PASSENGER_H