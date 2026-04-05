#ifndef FLIGHTSYSTEM_H
#define FLIGHTSYSTEM_H

#include "Constants.h"
#include "DataStructures.h" // Include the custom data structures
#include "Passenger.h"
#include <string>

// Flight struct is now defined in DataStructures.h as FlightStruct to include
// WaitlistQueue We will alias it for backward compatibility in code if needed,
// or just use FlightStruct
typedef FlightStruct Flight;

class FlightSystem {
private:
  // REPLACED: Passenger passengers[MAX_PASSENGERS]; -> Linked List & Hash Table
  PassengerList passengerList;     // Primary storage (Linked List)
  PassengerHashTable passengerMap; // For O(1) Search (Hash Table)

  // NEW FEATURES
  BookingStack undoStack; // For Undo functionality (Stack)

  // WaitlistQueue waitlists[MAX_FLIGHTS]; // REMOVED: Now inside Flight object

  int passengerCount; // Still useful for quick stats
  // Flight flights[MAX_FLIGHTS]; // REMOVED
  FlightList flightList; // NEW: Linked List for Flights

  // PERMISSION SYSTEM
  bool isAdmin; // Role status: true=Admin, false=Passenger

  // Helper Functions
  void initializeFlights();
  void removePassenger(std::string name);

  int countAvailableSeats(Flight *flight, const std::string &classType);
  int countOccupiedSeats(Flight *flight, const std::string &classType);
  std::string getClassType(int row);
  bool isSeatAvailable(Flight *flight, int row, int col);

public:
  void addPassenger(const std::string &name, int row, int col,
                    const std::string &date, const std::string &time,
                    const std::string &origin, const std::string &destination,
                    int flightId, Flight *flight, bool saveChanges = true);

  FlightSystem();
  void setAdmin(bool status); // Set Role
  void initialize();
  void startupSequence();
  void exitSequence();

  int displayMenu();

  // Core Features
  void bookSeat();
  void cancelBooking();
  void modifyBooking();
  void displaySeatMap();
  void displaySeatMap(Flight *flight);
  void displayFlightSchedule(int page = 1);
  void searchPassenger();
  void displayAllPassengers();
  void displayStatistics();
  void printBoardingPasses();
  void displayBoardingPass(Passenger *p); // Changed to take pointer

  // NEW FEATURE METHOD
  void undoLastBooking();
};

#endif // FLIGHTSYSTEM_H