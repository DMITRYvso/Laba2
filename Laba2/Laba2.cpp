#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <windows.h>

using namespace std;

enum class TicketStatus {
    AVAILABLE,
    SOLD
};

class Passenger {
private:
    string fullName;
    string passport;

public:
    Passenger(const string& name, const string& passportNum)
        : fullName(name), passport(passportNum) {
    }

    string getFullName() const { return fullName; }
    string getPassport() const { return passport; }

    static bool isValidPassport(const string& passport) {
        return passport.length() == 10 &&
            all_of(passport.begin(), passport.end(), [](char c) {
            return isdigit(c);
                });
    }

    static bool isValidName(const string& name) {
        if (name.empty()) return false;

        for (char c : name) {

            if (!((c >= 'А' && c <= 'я') || c == ' ' || c == '-' ||
                (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
                return false;
            }
        }
        return true;
    }
};

class Tariff {
private:
    string destination;
    double price;

public:
    Tariff(const string& dest, double pr) : destination(dest), price(pr) {}

    string getDestination() const { return destination; }
    double getPrice() const { return price; }

    static bool isValidPrice(double price) {
        return price > 0 || price < 100000;
    }

    static bool isValidDestination(const string& dest) {
        return !dest.empty();
    }
};

class Ticket {
private:
    Passenger passenger;
    Tariff tariff;
    TicketStatus status;

public:
    Ticket(const Passenger& pass, const Tariff& tar, TicketStatus stat = TicketStatus::SOLD)
        : passenger(pass), tariff(tar), status(stat) {
    }

    Passenger getPassenger() const { return passenger; }
    Tariff getTariff() const { return tariff; }
    TicketStatus getStatus() const { return status; }

    void setStatus(TicketStatus stat) { status = stat; }

    double getPrice() const {
        return tariff.getPrice();
    }
};

class AirportManager {
private:
    static AirportManager* instance;
    vector<Passenger> passengers;
    vector<Tariff> tariffs;
    vector<Ticket> tickets;

    AirportManager() = default;

public:
    AirportManager(const AirportManager&) = delete;
    AirportManager& operator=(const AirportManager&) = delete;

    static AirportManager* getInstance() {
        if (instance == nullptr) {
            instance = new AirportManager();
        }
        return instance;
    }

    ~AirportManager() {
        passengers.clear();
        tariffs.clear();
        tickets.clear();
    }

    void addPassenger(const Passenger& passenger) {
        passengers.push_back(passenger);
    }

    void addTariff(const Tariff& tariff) {
        tariffs.push_back(tariff);
    }

    void sellTicket(int passengerIndex, int tariffIndex) {
        if (passengerIndex >= 0 && passengerIndex < passengers.size() &&
            tariffIndex >= 0 && tariffIndex < tariffs.size()) {
            Ticket ticket(passengers[passengerIndex], tariffs[tariffIndex]);
            tickets.push_back(ticket);
            cout << "Билет успешно продан!" << endl;
        }
    }

    const vector<Passenger>& getPassengers() const {
        return passengers;
    }

    const vector<Tariff>& getTariffs() const {
        return tariffs;
    }

    const vector<Ticket>& getTickets() const {
        return tickets;
    }

    double calculatePassengerTotal(const string& passport) const {
        double total = 0;
        for (const auto& ticket : tickets) {
            if (ticket.getPassenger().getPassport() == passport) {
                total += ticket.getPrice();
            }
        }
        return total;
    }

    double calculateAllTicketsTotal() const {
        double total = 0;
        for (const auto& ticket : tickets) {
            total += ticket.getPrice();
        }
        return total;
    }

    bool passportExists(const string& passport) const {
        for (const auto& passenger : passengers) {
            if (passenger.getPassport() == passport) {
                return true;
            }
        }
        return false;
    }

    bool destinationExists(const string& destination) const {
        for (const auto& tariff : tariffs) {
            if (tariff.getDestination() == destination) {
                return true;
            }
        }
        return false;
    }
};

AirportManager* AirportManager::instance = nullptr;

string inputString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);

        if (!input.empty()) {
            return input;
        }
        cout << "Ошибка: ввод не может быть пустым. Попробуйте снова." << endl;
    }
}

string inputName() {
    string name;
    while (true) {
        name = inputString("Введите ФИО пассажира: ");
        if (Passenger::isValidName(name)) {
            return name;
        }
        cout << "Ошибка: ФИО должно содержать только буквы, пробелы и дефисы. Попробуйте снова." << endl;
    }
}

string inputPassport() {
    string passport;
    while (true) {
        passport = inputString("Введите номер паспорта (10 цифр): ");
        if (Passenger::isValidPassport(passport)) {
            if (!AirportManager::getInstance()->passportExists(passport)) {
                return passport;
            }
            cout << "Ошибка: пассажир с таким номером паспорта уже существует." << endl;
        }
        else {
            cout << "Ошибка: номер паспорта должен состоять из 10 цифр. Попробуйте снова." << endl;
        }
    }
}

string inputDestination() {
    string destination;
    while (true) {
        destination = inputString("Введите направление: ");
        if (Tariff::isValidDestination(destination)) {
            if (!AirportManager::getInstance()->destinationExists(destination)) {
                return destination;
            }
            cout << "Ошибка: тариф для этого направления уже существует." << endl;
        }
        else {
            cout << "Ошибка: направление не может быть пустым. Попробуйте снова." << endl;
        }
    }
}

double inputPrice() {
    double price;
    while (true) {
        cout << "Введите цену тарифа: ";
        cin >> price;

        if (cin.good() && Tariff::isValidPrice(price)&& price>0 && price<1000000) {
            cin.ignore(1000, '\n'); 
            return price;
        }

        cout << "Ошибка: цена должна быть положительным числом, меньше 1 000 000. Попробуйте снова." << endl;
        cin.clear();
        cin.ignore(1000, '\n'); 
    }
}

int inputMenuChoice(int min, int max) {
    int choice;
    while (true) {
        cout << "Выберите опцию (" << min << "-" << max << "): ";
        cin >> choice;

        if (cin.good() && choice >= min && choice <= max) {
            cin.ignore(1000, '\n'); 
            return choice;
        }

        cout << "Ошибка: введите число от " << min << " до " << max << "." << endl;
        cin.clear();
        cin.ignore(1000, '\n'); 
    }
}


void displayMainMenu() {
    cout << "\n=== СИСТЕМА УПРАВЛЕНИЯ АЭРОПОРТОМ ===" << endl;
    cout << "1. Добавить пассажира" << endl;
    cout << "2. Добавить тариф" << endl;
    cout << "3. Продать билет" << endl;
    cout << "4. Расчет стоимости билетов пассажира" << endl;
    cout << "5. Общая стоимость всех проданных билетов" << endl;
    cout << "6. Показать всех пассажиров" << endl;
    cout << "7. Показать все тарифы" << endl;
    cout << "8. Показать все проданные билеты" << endl;
    cout << "0. Выход" << endl;
}

void addPassengerMenu() {
    cout << "\n--- ДОБАВЛЕНИЕ ПАССАЖИРА ---" << endl;
    string name = inputName();
    string passport = inputPassport();

    AirportManager::getInstance()->addPassenger(Passenger(name, passport));
    cout << "Пассажир успешно добавлен!" << endl;
}

void addTariffMenu() {
    cout << "\n--- ДОБАВЛЕНИЕ ТАРИФА ---" << endl;
    string destination = inputDestination();
    double price = inputPrice();

    AirportManager::getInstance()->addTariff(Tariff(destination, price));
    cout << "Тариф успешно добавлен!" << endl;
}

void sellTicketMenu() {
    auto manager = AirportManager::getInstance();
    const auto& passengers = manager->getPassengers();
    const auto& tariffs = manager->getTariffs();

    if (passengers.empty()) {
        cout << "Ошибка: нет зарегистрированных пассажиров." << endl;
        return;
    }

    if (tariffs.empty()) {
        cout << "Ошибка: нет доступных тарифов." << endl;
        return;
    }

    cout << "\n--- ПРОДАЖА БИЛЕТА ---" << endl;

    cout << "Выберите пассажира:" << endl;
    for (size_t i = 0; i < passengers.size(); ++i) {
        cout << i + 1 << ". " << passengers[i].getFullName()
            << " (Паспорт: " << passengers[i].getPassport() << ")" << endl;
    }

    int passengerChoice = inputMenuChoice(1, static_cast<int>(passengers.size())) - 1;

    cout << "Выберите тариф:" << endl;
    for (size_t i = 0; i < tariffs.size(); ++i) {
        cout << i + 1 << ". " << tariffs[i].getDestination()
            << " - " << tariffs[i].getPrice() << " руб." << endl;
    }

    int tariffChoice = inputMenuChoice(1, static_cast<int>(tariffs.size())) - 1;

    manager->sellTicket(passengerChoice, tariffChoice);
}

void calculatePassengerTotalMenu() {
    auto manager = AirportManager::getInstance();
    const auto& passengers = manager->getPassengers();

    if (passengers.empty()) {
        cout << "Ошибка: нет зарегистрированных пассажиров." << endl;
        return;
    }

    cout << "\n--- РАСЧЕТ СТОИМОСТИ БИЛЕТОВ ПАССАЖИРА ---" << endl;
    cout << "Выберите пассажира:" << endl;

    for (size_t i = 0; i < passengers.size(); ++i) {
        cout << i + 1 << ". " << passengers[i].getFullName()
            << " (Паспорт: " << passengers[i].getPassport() << ")" << endl;
    }

    int choice = inputMenuChoice(1, static_cast<int>(passengers.size())) - 1;
    string passport = passengers[choice].getPassport();
    double total = manager->calculatePassengerTotal(passport);

    cout << "Общая стоимость билетов пассажира: " << total << " руб." << endl;
}

void displayAllPassengers() {
    auto manager = AirportManager::getInstance();
    const auto& passengers = manager->getPassengers();

    if (passengers.empty()) {
        cout << "Нет зарегистрированных пассажиров." << endl;
        return;
    }

    cout << "\n--- СПИСОК ПАССАЖИРОВ ---" << endl;
    for (size_t i = 0; i < passengers.size(); ++i) {
        cout << i + 1 << ". " << passengers[i].getFullName()
            << " | Паспорт: " << passengers[i].getPassport() << endl;
    }
}

void displayAllTariffs() {
    auto manager = AirportManager::getInstance();
    const auto& tariffs = manager->getTariffs();

    if (tariffs.empty()) {
        cout << "Нет доступных тарифов." << endl;
        return;
    }

    cout << "\n--- СПИСОК ТАРИФОВ ---" << endl;
    for (size_t i = 0; i < tariffs.size(); ++i) {
        cout << i + 1 << ". " << tariffs[i].getDestination()
            << " - " << tariffs[i].getPrice() << " руб." << endl;
    }
}

void displayAllTickets() {
    auto manager = AirportManager::getInstance();
    const auto& tickets = manager->getTickets();

    if (tickets.empty()) {
        cout << "Нет проданных билетов." << endl;
        return;
    }

    cout << "\n--- СПИСОК ПРОДАННЫХ БИЛЕТОВ ---" << endl;
    for (size_t i = 0; i < tickets.size(); ++i) {
        const auto& ticket = tickets[i];
        cout << i + 1 << ". " << ticket.getPassenger().getFullName()
            << " | " << ticket.getTariff().getDestination()
            << " | " << ticket.getPrice() << " руб." << endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Добро пожаловать в систему управления аэропортом!" << endl;

    int choice;
    do {
        displayMainMenu();
        choice = inputMenuChoice(0, 8);

        switch (choice) {
        case 1:
            addPassengerMenu();
            break;
        case 2:
            addTariffMenu();
            break;
        case 3:
            sellTicketMenu();
            break;
        case 4:
            calculatePassengerTotalMenu();
            break;
        case 5:
            cout << "\nОбщая стоимость всех проданных билетов: "
                << AirportManager::getInstance()->calculateAllTicketsTotal()
                << " руб." << endl;
            break;
        case 6:
            displayAllPassengers();
            break;
        case 7:
            displayAllTariffs();
            break;
        case 8:
            displayAllTickets();
            break;
        case 0:
            cout << "Выход из программы. До свидания!" << endl;
            break;
        }
    } while (choice != 0);

    delete AirportManager::getInstance();

    return 0;
}