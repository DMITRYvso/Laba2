classDiagram
    class Passenger {
        -string fullName
        -string passport
        +Passenger(string name, string passportNum)
        +getFullName() string
        +getPassport() string
        +isValidPassport(string passport) bool
        +isValidName(string name) bool
    }

    class Tariff {
        -string destination
        -double price
        -TariffStatus status
        +Tariff(string dest, double pr)
        +getDestination() string
        +getPrice() double
        +getStatus() TariffStatus
        +setStatus(TariffStatus newStatus) void
        +getStatusString() string
        +isValidPrice(double price) bool
        +isValidDestination(string dest) bool
    }

    class Ticket {
        -Passenger passenger
        -Tariff tariff
        +Ticket(Passenger pass, Tariff tar)
        +getPassenger() Passenger
        +getTariff() Tariff
        +getPrice() double
    }

    class AirportManager {
        -static AirportManager* instance
        -vector~Passenger~ passengers
        -vector~Tariff~ tariffs
        -vector~Ticket~ tickets
        -AirportManager()
        +getInstance() AirportManager*
        +addPassenger(Passenger passenger) void
        +addTariff(Tariff tariff) void
        +sellTicket(int passengerIndex, int tariffIndex) bool
        +getPassengers() vector~Passenger~
        +getTariffs() vector~Tariff~
        +getTickets() vector~Ticket~
        +calculatePassengerTotal(string passport) double
        +calculateAllTicketsTotal() double
        +passportExists(string passport) bool
        +destinationExists(string destination) bool
        +getAvailableTariffsCount() int
    }

    enum TariffStatus {
        <<enumeration>>
        AVAILABLE
        SOLD
    }

    Passenger "1" -- "*" Ticket : имеет
    Tariff "1" -- "*" Ticket : использует
    AirportManager "1" -- "*" Passenger : управляет
    AirportManager "1" -- "*" Tariff : управляет
    AirportManager "1" -- "*" Ticket : управляет
    Tariff --> TariffStatus : имеет статус
