#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// ============================================================================
// ANSI COLOR CODES
// ============================================================================
namespace Color {
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";
const std::string BOLD = "\033[1m";
const std::string DIM = "\033[2m";
const std::string BG_BLUE = "\033[44m";
const std::string BG_RED = "\033[41m";
const std::string BG_GREEN = "\033[42m";
} // namespace Color

// ============================================================================
// GLOBAL CONSTANTS
// ============================================================================
const int ROWS = 12;            // Total rows in aircraft
const int COLS = 6;             // Seats per row (A-F)
const int FIRST_CLASS_ROWS = 2; // Rows 0-1 are First Class
const int MAX_FLIGHTS = 20;     // Total number of flights in schedule
const int MAX_PASSENGERS =
    MAX_FLIGHTS * ROWS * COLS; // Maximum capacity: 72 * 20 = 1440 passengers

// ============================================================================
// FLIGHT CONSTANTS
// ============================================================================
const std::string FLIGHT_CODE = "AL-6213";
const std::string FLIGHT_DATE = "26 JAN 2026";
const std::string FLIGHT_ROUTE = "KUL -> SYD";
const std::string DEPARTURE_TIME = "10:30 AM";
const std::string ARRIVAL_TIME = "09:45 PM";

#endif // CONSTANTS_H
