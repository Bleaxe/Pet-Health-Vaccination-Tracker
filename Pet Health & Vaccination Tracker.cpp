#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <cstdio>   // for sscanf
#include <algorithm>
#include <cstdlib>  // for rand

using namespace std;

// Helper function to parse date string "YYYY-MM-DD" to time_t
time_t parseDate(const string& dateStr) {
    struct tm tm = {};
    int year, month, day;

    if (sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day) == 3) {
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = 12; // set noon to avoid DST issues
        return mktime(&tm);
    }
    return -1;
}

// Helper function to format time_t to string "YYYY-MM-DD"
string formatDate(time_t time) {
    struct tm *tm = localtime(&time);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm);
    return string(buffer);
}

struct Owner {
    string name;
    string phone;
    string email;
};

struct Vaccination {
    string vaccineName;
    time_t dateGiven;     // -1 if not yet given (expected/planned)
    time_t nextDueDate;   // For given: next booster; For expected: expected date
};

struct Appointment {
    string petName;
    int petId;            // Added ID for reference
    string vaccineName;
    time_t appointmentDate;
};

struct Pet {
    int id;               // Random 4-digit code
    string name;
    string type;          // e.g., "dog", "cat"
    string breed;
    int age; // in years
    Owner owner;
    vector<Vaccination> vaccinations;
};

vector<Pet> pets;
vector<Appointment> appointments;

void displayPets() {
    if (pets.empty()) {
        cout << "No pets added yet.\n";
        return;
    }
    cout << "Current Pets:\n";
    for (const auto& pet : pets) {
        cout << "ID: " << pet.id << " - " << pet.type << " - " << pet.name << " (" << pet.breed << ", Age: " << pet.age << ")\n";
        cout << "   Owner: " << pet.owner.name << "\n";
    }
}

// Function to get vaccine name based on pet type and selection (common options only for cat/dog)
string getVaccineName(const Pet& pet) {
    if (pet.type == "dog") {
        cout << "Common Dog Vaccines:\n";
        cout << "1. DHPP (Distemper, Hepatitis, Parvovirus, Parainfluenza)\n";
        cout << "2. Rabies\n";
        cout << "3. Bordetella\n";
        cout << "4. Leptospirosis\n";
        cout << "5. Lyme\n";
        cout << "Select vaccine number (1-5): ";
    } else if (pet.type == "cat") {
        cout << "Common Cat Vaccines:\n";
        cout << "1. FVRCP (Feline Viral Rhinotracheitis, Calicivirus, Panleukopenia)\n";
        cout << "2. Rabies\n";
        cout << "3. FeLV (Feline Leukemia)\n";
        cout << "4. FIV\n";
        cout << "5. Chlamydia\n";
        cout << "Select vaccine number (1-5): ";
    } else {
        cout << "Enter vaccine name: ";
        string custom;
        getline(cin, custom);
        return custom;
    }

    // Loop to reprompt on invalid input
    while (true) {
        string choiceStr;
        getline(cin, choiceStr);
        int choice = 0;
        try {
            choice = stoi(choiceStr);
        } catch (...) {
            cout << "Invalid input. Please select 1-5: ";
            if (pet.type == "dog") {
                cout << "Common Dog Vaccines:\n1. DHPP\n2. Rabies\n3. Bordetella\n4. Leptospirosis\n5. Lyme\nSelect (1-5): ";
            } else {
                cout << "Common Cat Vaccines:\n1. FVRCP\n2. Rabies\n3. FeLV\n4. FIV\n5. Chlamydia\nSelect (1-5): ";
            }
            continue;
        }

        if (choice >= 1 && choice <= 5) {
            if (pet.type == "dog") {
                vector<string> dogVaccines = {"DHPP", "Rabies", "Bordetella", "Leptospirosis", "Lyme"};
                return dogVaccines[choice - 1];
            } else {
                vector<string> catVaccines = {"FVRCP", "Rabies", "FeLV", "FIV", "Chlamydia"};
                return catVaccines[choice - 1];
            }
        } else {
            cout << "Invalid selection. Please select 1-5: ";
            if (pet.type == "dog") {
                cout << "Common Dog Vaccines:\n1. DHPP\n2. Rabies\n3. Bordetella\n4. Leptospirosis\n5. Lyme\nSelect (1-5): ";
            } else {
                cout << "Common Cat Vaccines:\n1. FVRCP\n2. Rabies\n3. FeLV\n4. FIV\n5. Chlamydia\nSelect (1-5): ";
            }
        }
    }
}

void addPet() {
    Pet pet;
    pet.id = rand() % 9000 + 1000; // Random 4-digit: 1000-9999

    cout << "Enter pet type: ";
    getline(cin, pet.type);
    cout << "Enter pet's name: ";
    getline(cin, pet.name);
    cout << "Enter breed: ";
    getline(cin, pet.breed);
    cout << "Enter age (years): ";
    string ageStr;
    getline(cin, ageStr);
    try {
        pet.age = stoi(ageStr);
        if (pet.age < 0) pet.age = 0;
    } catch (...) {
        cout << "Invalid age. Set to 0.\n";
        pet.age = 0;
    }

    cout << "Enter owner's name: ";
    getline(cin, pet.owner.name);
    cout << "Enter owner's phone: ";
    getline(cin, pet.owner.phone);
    cout << "Enter owner's email: ";
    getline(cin, pet.owner.email);

    pets.push_back(pet);
    cout << "Pet of type '" << pet.type << "' named '" << pet.name << "' (ID: " << pet.id << ") added successfully.\n";
}

void addVaccination() {
    if (pets.empty()) {
        cout << "No pets. Add a pet first.\n";
        return;
    }

    displayPets();
    cout << "Enter pet ID: ";
    string idStr;
    getline(cin, idStr);
    int targetId = 0;
    try {
        targetId = stoi(idStr);
    } catch (...) {
        cout << "Invalid ID.\n";
        return;
    }

    Pet* selectedPet = nullptr;
    for (auto& pet : pets) {
        if (pet.id == targetId) {
            selectedPet = &pet;
            break;
        }
    }
    if (!selectedPet) {
        cout << "Pet ID not found.\n";
        return;
    }

    Vaccination vac;
    vac.vaccineName = getVaccineName(*selectedPet);

    cout << "Enter date given (YYYY-MM-DD, or leave empty if expected/planned): ";
    string dateGivenStr;
    getline(cin, dateGivenStr);
    if (dateGivenStr.empty()) {
        vac.dateGiven = -1; // Expected/planned
    } else {
        vac.dateGiven = parseDate(dateGivenStr);
        if (vac.dateGiven == -1) {
            cout << "Invalid date.\n";
            return;
        }
    }

    cout << "Enter next due date (YYYY-MM-DD): ";
    string nextDueStr;
    getline(cin, nextDueStr);
    vac.nextDueDate = parseDate(nextDueStr);
    if (vac.nextDueDate == -1) {
        cout << "Invalid date.\n";
        return;
    }

    selectedPet->vaccinations.push_back(vac);
    string status = (vac.dateGiven == -1) ? "expected" : "given";
    cout << status << " vaccination '" << vac.vaccineName << "' added for '" << selectedPet->type << "' '" << selectedPet->name << "' (ID: " << selectedPet->id << ").\n";
}

void bookAppointment() {
    if (pets.empty()) {
        cout << "No pets. Add a pet first.\n";
        return;
    }

    displayPets();
    cout << "Enter pet ID: ";
    string idStr;
    getline(cin, idStr);
    int targetId = 0;
    try {
        targetId = stoi(idStr);
    } catch (...) {
        cout << "Invalid ID.\n";
        return;
    }

    const Pet* selectedPet = nullptr;
    for (const auto& pet : pets) {
        if (pet.id == targetId) {
            selectedPet = &pet;
            break;
        }
    }
    if (!selectedPet) {
        cout << "Pet ID not found.\n";
        return;
    }

    Appointment appt;
    appt.petId = targetId;
    appt.petName = selectedPet->name;
    appt.vaccineName = getVaccineName(*selectedPet); // Reuse the function for consistency

    cout << "Enter appointment date (YYYY-MM-DD): ";
    string dateStr;
    getline(cin, dateStr);
    appt.appointmentDate = parseDate(dateStr);
    if (appt.appointmentDate == -1) {
        cout << "Invalid date.\n";
        return;
    }

    appointments.push_back(appt);
    cout << "Appointment for '" << appt.vaccineName << "' booked for '" << selectedPet->type << "' '" << selectedPet->name << "' (ID: " << selectedPet->id << ") on " << formatDate(appt.appointmentDate) << ".\n";
}

void showUpcomingVaccines(int daysAhead) {
    time_t now = time(nullptr);
    time_t limit = now + daysAhead * 24 * 3600;

    cout << "Upcoming Vaccinations in " << daysAhead << " days:\n";
    bool found = false;
    for (const auto& pet : pets) {
        for (const auto& vac : pet.vaccinations) {
            time_t due = vac.nextDueDate;
            if (due >= now && due <= limit) {
                string status = (vac.dateGiven == -1) ? "Expected" : "Next Due";
                cout << "- Pet Type: " << pet.type << ", Name: " << pet.name << " (ID: " << pet.id << ", " << pet.breed << ")\n";
                cout << "  Vaccine: " << vac.vaccineName << "\n";
                cout << "  " << status << ": " << formatDate(due) << "\n";
                cout << "  Given: " << (vac.dateGiven == -1 ? "Not yet" : formatDate(vac.dateGiven)) << "\n";
                cout << "  Owner: " << pet.owner.name << " (Phone: " << pet.owner.phone << ", Email: " << pet.owner.email << ")\n";
                found = true;
            }
        }
    }
    if (!found) {
        cout << "No upcoming vaccinations.\n";
    }
}

void showUpcomingAppointments(int daysAhead) {
    time_t now = time(nullptr);
    time_t limit = now + daysAhead * 24 * 3600;

    cout << "Upcoming Appointments in " << daysAhead << " days:\n";
    bool found = false;
    for (const auto& appt : appointments) {
        if (appt.appointmentDate >= now && appt.appointmentDate <= limit) {
            // Find matching pet
            for (const auto& pet : pets) {
                if (pet.id == appt.petId) {
                    cout << "- Pet Type: " << pet.type << ", Name: " << pet.name << " (ID: " << pet.id << ", " << pet.breed << ")\n";
                    cout << "  Vaccine: " << appt.vaccineName << "\n";
                    cout << "  Date: " << formatDate(appt.appointmentDate) << "\n";
                    cout << "  Owner: " << pet.owner.name << " (Phone: " << pet.owner.phone << ", Email: " << pet.owner.email << ")\n";
                    found = true;
                    break;
                }
            }
        }
    }
    if (!found) {
        cout << "No upcoming appointments.\n";
    }
}

int main() {
    srand(time(NULL)); // Global seed for random IDs
    cout << "Pet Health & Vaccination Tracker\n";

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add Pet\n";
        cout << "2. Add Vaccination\n";
        cout << "3. Book Appointment\n";
        cout << "4. Show Upcoming Vaccinations\n";
        cout << "5. Show Upcoming Appointments\n";
        cout << "6. Exit\n";
        cout << "Choose an option: ";

        int option;
        cin >> option;
        cin.ignore(); // Clear newline

        switch (option) {
            case 1:
                addPet();
                break;
            case 2:
                addVaccination();
                break;
            case 3:
                bookAppointment();
                break;
            case 4: {
                cout << "Days ahead: ";
                int days;
                cin >> days;
                cin.ignore();
                if (days >= 0) {
                    showUpcomingVaccines(days);
                } else {
                    cout << "Invalid days.\n";
                }
                break;
            }
            case 5: {
                cout << "Days ahead: ";
                int days;
                cin >> days;
                cin.ignore();
                if (days >= 0) {
                    showUpcomingAppointments(days);
                } else {
                    cout << "Invalid days.\n";
                }
                break;
            }
            case 6:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid option.\n";
        }
    }
    return 0;
}