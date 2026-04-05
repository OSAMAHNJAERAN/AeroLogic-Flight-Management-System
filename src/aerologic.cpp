/**
 * ============================================================================
 *                         AEROLOGIC AIRLINE RESERVATION SYSTEM
 * ============================================================================
 *
 * Course: TDS6213 Data Structures and Algorithms
 * Institution: Multimedia University (MMU)
 *
 * Description:
 * A sophisticated C++ airline reservation system implementing core data
 * structures and algorithms including 2D arrays, Bubble Sort, and Linear
 * Search. The system provides full CRUD operations with an intuitive visual
 * interface.
 *
 * ============================================================================
 */

#include "FlightSystem.h"
#include "Utility.h"
#include <chrono>
#include <iostream>
#include <limits>
#include <thread>

using namespace std;

int main() {
  // Setup environment
  setupConsole();

  // Initialize Flight System
  FlightSystem sys;
  sys.initialize();

  // LOGIN SYSTEM
  string username, password;
  bool authenticated = false;

  while (!authenticated) {
    clearScreen();
    displayHeader();
    cout << Color::BOLD << Color::YELLOW << "\n    AUTHENTICATION REQUIRED"
         << Color::RESET << endl;
    cout << "\n    Username: ";
    cin >> username;
    cout << "    Password: ";
    cin >> password;

    // Clear buffer after reading strings
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (username == "admin" && password == "mmu123") {
      authenticated = true;
      sys.setAdmin(true);
      cout << Color::GREEN
           << "\n    ✓ Admin Login Successful! Accessing system..."
           << Color::RESET << endl;
      this_thread::sleep_for(chrono::milliseconds(1000));
    } else if (username == "user" && password == "123") {
      authenticated = true;
      sys.setAdmin(false);
      cout << Color::GREEN
           << "\n    ✓ Passenger Login Successful! Accessing system..."
           << Color::RESET << endl;
      this_thread::sleep_for(chrono::milliseconds(1000));
    } else {
      cout << Color::RED << "\n    ✗ Invalid Credentials! Please try again."
           << Color::RESET << endl;
      pressEnterToContinue();
    }
  }

  // Startup
  sys.startupSequence();

  int choice;
  bool running = true;

  // Main Loop
  while (running) {
    clearScreen();
    displayHeader();
    choice = sys.displayMenu();

    switch (choice) {
    case 1:
      sys.bookSeat();
      break;
    case 2:
      sys.cancelBooking();
      break;
    case 3:
      sys.modifyBooking();
      break;
    case 4:
      sys.displaySeatMap();
      pressEnterToContinue();
      break;
    case 5:
      sys.searchPassenger();
      break;
    case 6:
      sys.displayAllPassengers();
      break;
    case 7:
      sys.displayStatistics();
      break;
    case 8:
      sys.printBoardingPasses();
      break;
    case 9:
      sys.undoLastBooking();
      break;
    case 0:
      sys.exitSequence();
      running = false;
      break;
    default:
      cout << Color::RED << "\n    ✗ Invalid choice! Please select 0-8."
           << Color::RESET << endl;
      pressEnterToContinue();
      break;
    }
  }

  return 0;
}
