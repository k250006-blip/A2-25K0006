#include <iostream>
#include <string>
using namespace std;

const int    maxVehicles = 200;
const int    maxUsers    = 50;
const int    maxMessages = 50;
const int    maxListings = 200;
const string SystemName  = "Car MarketPlace System";

class IDisplayable {
public:
    virtual void display() const = 0;   // pure virtual – must override
    virtual ~IDisplayable() {}
};

class ISearchable {
public:
    // Returns true when this object satisfies the given keyword/value
    virtual bool matchesCriteria(const string& key, const string& value) const = 0;
    virtual ~ISearchable() {}
};

class IApprovable {
public:
    virtual void approve() = 0;   // pure virtual
    virtual void reject()  = 0;   // pure virtual
    virtual string getStatus() const = 0;
    virtual ~IApprovable() {}
};

class VehicleDetails {
    int    mileage;
    string color;
    string fuelType;
    string transmission;
    string description;

public:
    // Default constructor
    VehicleDetails() : mileage(0), color(""), fuelType(""), transmission(""), description("") {}

    // Parameterized constructor
    VehicleDetails(int m, string c, string f, string t, string d)
        : mileage(m), color(c), fuelType(f), transmission(t), description(d) {}

    int    getMileage()      const { return mileage; }
    string getColor()        const { return color; }
    string getFuelType()     const { return fuelType; }
    string getTransmission() const { return transmission; }

    void updateMileage(int m)  { mileage = m; }
    void updateColor(string c) { color   = c; }

    void display() const {
        cout << "  Mileage     : " << mileage      << " km\n";
        cout << "  Color       : " << color         << "\n";
        cout << "  Fuel Type   : " << fuelType      << "\n";
        cout << "  Transmission: " << transmission  << "\n";
        cout << "  Description : " << description   << "\n";
    }
};

class Vehicle : public IDisplayable, public ISearchable {
protected:                          // protected so derived classes can access
    int    vehicleID;
    string brand;
    string model;
    int    year;
    double price;

    VehicleDetails details;         //VehicleDetails owned by Vehicle

    static int totalVehicles;       // static: shared across ALL Vehicle objects

public:
    // Default constructor
    Vehicle() : vehicleID(0), brand(""), model(""), year(0), price(0.0) {}

    // Parameterized constructor
    Vehicle(int id, string b, string m, int y, double p, VehicleDetails d)
        : vehicleID(id), brand(b), model(m), year(y), price(p), details(d) {
        totalVehicles++;
    }

    // Copy constructor
    Vehicle(const Vehicle& v)
        : vehicleID(v.vehicleID), brand(v.brand), model(v.model),
          year(v.year), price(v.price), details(v.details) {
        totalVehicles++;
    }

    // Getters
    int    getID()      const { return vehicleID; }
    string getBrand()   const { return brand;     }
    string getModel()   const { return model;     }
    double getPrice()   const { return price;     }
    int    getYear()    const { return year;      }
    int    getMileage() const { return details.getMileage(); }

    // Setter
    void updatePrice(double p)                    { price = p; }           // overload 1
    void updatePrice(double p, double discount)   { price = p - discount; } // overload 2 (overloading)

    //virtual function overriding: each subtype displays itself differently
    virtual void displayDetails() const {
        cout << "  [Vehicle] ID: " << vehicleID << "  Brand: " << brand
             << "  Model: " << model << "  Year: " << year << "  Price: " << price << "\n";
        details.display();
    }

    // IDisplayable implementation
    void display() const override { displayDetails(); }

    //match brand or model by keyword
    bool matchesCriteria(const string& key, const string& value) const override {
        if (key == "brand") return brand == value;
        if (key == "model") return model == value;
        return false;
    }

    // Static member function – can be called without an object
    static void showTotalVehicles() {
        cout << "Total Vehicles registered: " << totalVehicles << "\n";
    }

    bool operator==(const Vehicle& other) const {
        return vehicleID == other.vehicleID;
    }

    double operator+(const Vehicle& other) const {
        return price + other.price;
    }

    // Friend declaration (defined after Listing class)
    friend void printVehicleSummary(const Vehicle& v, const class Listing& l);

    virtual ~Vehicle() {}
};

int Vehicle::totalVehicles = 0;

class Car : public Vehicle {
    int    numDoors;
    string bodyType;    // Sedan, SUV, Hatchback …
    bool   hasAC;
    string registrationCity;

public:
    // Default constructor
    Car() : Vehicle(), numDoors(4), bodyType("Sedan"), hasAC(true), registrationCity("") {}

    //calls Vehicle's parameterized constructor
    Car(int id, string b, string m, int y, double p, VehicleDetails d,
        int doors, string bt, bool ac, string city)
        : Vehicle(id, b, m, y, p, d),
          numDoors(doors), bodyType(bt), hasAC(ac), registrationCity(city) {}

    //overrides Vehicle::displayDetails()
    void displayDetails() const override {
        cout << "  [Car] ID: " << vehicleID << "  Brand: " << brand
             << "  Model: " << model << "  Year: " << year << "  Price: " << price << "\n";
        cout << "  Body Type   : " << bodyType         << "\n";
        cout << "  Doors       : " << numDoors         << "\n";
        cout << "  AC          : " << (hasAC ? "Yes" : "No") << "\n";
        cout << "  Reg. City   : " << registrationCity << "\n";
        details.display();
    }

    //extends parent with body-type search
    bool matchesCriteria(const string& key, const string& value) const override {
        if (key == "bodyType") return bodyType == value;
        return Vehicle::matchesCriteria(key, value);   // delegate to base
    }

    string getBodyType() const { return bodyType; }
};

class Bike : public Vehicle {
    int    engineCC;
    string bikeType;    // Sport, Cruiser, Commuter …
    bool   hasFairing;
    string brand125cc;  // common local branding

public:
    // Default constructor
    Bike() : Vehicle(), engineCC(125), bikeType("Commuter"), hasFairing(false), brand125cc("") {}

    // Parameterized constructor
    Bike(int id, string b, string m, int y, double p, VehicleDetails d,
         int cc, string bt, bool fairing)
        : Vehicle(id, b, m, y, p, d),
          engineCC(cc), bikeType(bt), hasFairing(fairing), brand125cc(b) {}

    //overrides Vehicle::displayDetails()
    void displayDetails() const override {
        cout << "  [Bike] ID: " << vehicleID << "  Brand: " << brand
             << "  Model: " << model << "  Year: " << year << "  Price: " << price << "\n";
        cout << "  Bike Type   : " << bikeType   << "\n";
        cout << "  Engine CC   : " << engineCC   << "\n";
        cout << "  Has Fairing : " << (hasFairing ? "Yes" : "No") << "\n";
        details.display();
    }

    int    getEngineCC() const { return engineCC; }
    string getBikeType() const { return bikeType; }
};

class Message : public IDisplayable {
    int    senderID;
    int    receiverID;
    string content;
    string date;
    bool   isRead;

public:
    Message() : senderID(0), receiverID(0), content(""), date(""), isRead(false) {}

    Message(int s, int r, string c, string d)
        : senderID(s), receiverID(r), content(c), date(d), isRead(false) {}

    void markRead()            { isRead  = true;      }
    void editMessage(string c) { content = c;         }
    void deleteMessage()       { content = "Deleted"; }

    int getSenderID()   const { return senderID;   }
    int getReceiverID() const { return receiverID; }

    void display() const override {
        cout << "  From    : " << senderID   << "\n";
        cout << "  To      : " << receiverID << "\n";
        cout << "  Content : " << content    << "\n";
        cout << "  Date    : " << date       << (isRead ? "  [Read]" : "  [Unread]") << "\n";
    }

    bool operator==(const Message& other) const {
        return senderID == other.senderID && receiverID == other.receiverID
               && content == other.content;
    }
};

class Favorites {
    int  vehicleIDs[20];
    int  count;
    string category;
    bool isPublic;
    int  ownerID;

public:
    Favorites() : count(0), category("General"), isPublic(true), ownerID(0) {}

    void addfav(int id)    { if (count < 20) vehicleIDs[count++] = id; }
    void clearFavorites()  { count = 0; }

    void removefav(int id) {
        for (int i = 0; i < count; i++)
            if (vehicleIDs[i] == id) { vehicleIDs[i] = vehicleIDs[--count]; break; }
    }

    void displayfav() const {
        cout << "  Favorites: ";
        for (int i = 0; i < count; i++) cout << vehicleIDs[i] << " ";
        cout << "\n";
    }
};

class User : public IDisplayable {
protected:
    int    userID;
    string name;
    string contact;
    string email;
    string password;

    static int totalUsers;   // static: one counter for all User objects

public:
    // Default constructor
    User() : userID(0), name(""), contact(""), email(""), password("") {}

    // Parameterized constructor
    User(int id, string n, string c, string e, string p)
        : userID(id), name(n), contact(c), email(e), password(p) {
        totalUsers++;
    }

    //justified to duplicate user profile
    User(const User& u)
        : userID(u.userID), name(u.name), contact(u.contact),
          email(u.email),   password(u.password) {}

    int    getID()   const { return userID; }
    string getName() const { return name;   }

    void updateProfile(string n, string c) { name = n; contact = c; }

    //virtual function: each subtype returns its own role string
    virtual string getRole() const { return "User"; }

    // IDisplayable implementation (overrideable)
    virtual void display() const override {
        cout << "  User ID : " << userID << "  Name: " << name
             << "  Role: " << getRole() << "\n";
    }

    static void showTotalUsers() {
        cout << "Total Users: " << totalUsers << "\n";
    }

    virtual ~User() {}
};

int User::totalUsers = 0;

class Buyer : public User {
    double   walletBalance;
    int      totalMessages;
    Favorites fav;                  //Favorites owned by Buyer

    Message* messages[20];          //Messages exist independently
    int      msgCount;

public:
    // Default constructor
    Buyer() : User(), walletBalance(0), totalMessages(0), msgCount(0) {}

    //reuses User's parameterized constructor
    Buyer(int uid, string n, string c, string e, string p, double balance)
        : User(uid, n, c, e, p), walletBalance(balance), totalMessages(0), msgCount(0) {}

    //overrides getRole()
    string getRole() const override { return "Buyer"; }

    void display() const override {
        User::display();
        cout << "  Wallet  : PKR " << walletBalance << "\n";
    }

    void addFavorite(int id)   { fav.addfav(id);    }
    void removeFavorite(int id){ fav.removefav(id); }
    void showFavorites() const { fav.displayfav();  }

    void sendMessage(Message* m) {
        if (msgCount < 20) messages[msgCount++] = m;
    }

    double getWalletBalance() const { return walletBalance; }

    // FRIEND FUNCTION declaration (defined below)
    friend bool isBuyerEligible(const Buyer& b, double price);
};

class Seller : public User {
    int      totalListings;
    int      totalMessages;
    Message* inbox[20];
    int      inboxCount;
    double   rating;                // seller rating out of 5

public:
    // Default constructor
    Seller() : User(), totalListings(0), totalMessages(0), inboxCount(0), rating(0.0) {}

    // Parameterized constructor
    Seller(int uid, string n, string c, string e, string p)
        : User(uid, n, c, e, p),
          totalListings(0), totalMessages(0), inboxCount(0), rating(5.0) {}

    //overrides getRole()
    string getRole() const override { return "Seller"; }

    void display() const override {
        User::display();
        cout << "  Listings: " << totalListings << "  Rating: " << rating << "/5\n";
    }

    void receiveMessage(Message* msg) {
        if (inboxCount < 20) inbox[inboxCount++] = msg;
    }

    void showMessages() const {
        if (inboxCount == 0) { cout << "  No messages.\n"; return; }
        for (int i = 0; i < inboxCount; i++) inbox[i]->display();
    }

    void increaseListings() { totalListings++; }
    int  getListingCount()  const { return totalListings; }

    // FRIEND FUNCTION declaration
    friend void compareSellers(const Seller& a, const Seller& b);
};

class Admin : public User {
    string role;
    int    actionsPerformed;
    bool   isSuperAdmin;

public:
    // Default constructor
    Admin() : User(), role("Moderator"), actionsPerformed(0), isSuperAdmin(false) {}

    // Parameterized constructor
    Admin(int uid, string n, string c, string e, string p, bool superAdmin = false)
        : User(uid, n, c, e, p),
          role(superAdmin ? "SuperAdmin" : "Moderator"),
          actionsPerformed(0), isSuperAdmin(superAdmin) {}

    //overrides getRole()
    string getRole() const override { return role; }

    void display() const override {
        User::display();
        cout << "  Actions : " << actionsPerformed
             << "  SuperAdmin: " << (isSuperAdmin ? "Yes" : "No") << "\n";
    }

    void performAction() { actionsPerformed++; }

    // FRIEND FUNCTION declaration
    friend void adminAudit(const Admin& a);
};

class Listing : public IDisplayable, public IApprovable {
    int    listingID;
    int    sellerID;
    int    vehicleID;
    double listingPrice;
    string status;

public:
    // Default constructor
    Listing() : listingID(0), sellerID(0), vehicleID(0), listingPrice(0), status("Pending") {}

    // Parameterized constructor
    Listing(int l, int s, int v, double p)
        : listingID(l), sellerID(s), vehicleID(v), listingPrice(p), status("Pending") {}

    // Getters
    int    getVehicleID()   const { return vehicleID;    }
    int    getSellerID()    const { return sellerID;     }
    int    getListingID()   const { return listingID;    }
    double getListingPrice()const { return listingPrice; }
    string getStatus()      const override { return status; }

    // IApprovable implementation
    void approve() override { status = "Approved"; }
    void reject()  override { status = "Removed";  }

    void updatePrice(double p) { listingPrice = p; }

    // IDisplayable implementation
    void display() const override {
        cout << "  Listing ID : " << listingID   << "\n";
        cout << "  Vehicle ID : " << vehicleID   << "\n";
        cout << "  Price      : PKR " << listingPrice << "\n";
        cout << "  Status     : " << status       << "\n";
    }

    bool operator==(const Listing& other) const {
        return listingID == other.listingID;
    }

    // Friend function – needs both Vehicle and Listing private data
    friend void printVehicleSummary(const Vehicle& v, const Listing& l);
};

void printVehicleSummary(const Vehicle& v, const Listing& l) {
    cout << "  === Vehicle-Listing Summary ===\n";
    cout << "  Vehicle : [" << v.vehicleID << "] " << v.brand << " " << v.model
         << "  PKR " << v.price << "\n";
    cout << "  Listing : [" << l.listingID << "] Status: " << l.status << "\n";
    cout << "  ================================\n";
}

bool isBuyerEligible(const Buyer& b, double price) {
    return b.walletBalance >= price;
}

void compareSellers(const Seller& a, const Seller& b) {
    cout << "  Seller [" << a.userID << "] listings: " << a.totalListings << "\n";
    cout << "  Seller [" << b.userID << "] listings: " << b.totalListings << "\n";
    if (a.totalListings > b.totalListings)
        cout << "  Winner: Seller " << a.userID << "\n";
    else if (b.totalListings > a.totalListings)
        cout << "  Winner: Seller " << b.userID << "\n";
    else
        cout << "  Both sellers have equal listings.\n";
}

void adminAudit(const Admin& a) {
    cout << "  === Admin Audit Report ===\n";
    cout << "  Admin ID          : " << a.userID          << "\n";
    cout << "  Actions Performed : " << a.actionsPerformed << "\n";
    cout << "  Super Admin       : " << (a.isSuperAdmin ? "Yes" : "No") << "\n";
    cout << "  ==========================\n";
}

class MarketplaceSystem {
    // system owns vehicles and listings directly
    Vehicle* vehicles[200];
    Listing  listings[200];

    // users exist independently; system merely references them
    Buyer*  buyers[50];
    Seller* sellers[50];
    Admin*  admins[20];

    int vCount, lCount, bCount, sCount, aCount;

public:
    MarketplaceSystem() : vCount(0), lCount(0), bCount(0), sCount(0), aCount(0) {
        for (int i = 0; i < 200; i++) vehicles[i] = nullptr;
        for (int i = 0; i < 50;  i++) { buyers[i]  = nullptr; sellers[i] = nullptr; }
        for (int i = 0; i < 20;  i++)   admins[i]  = nullptr;
    }

    // ---- Add methods ----
    void addVehicle(Vehicle* v) {
        // Check for duplicate using operator==
        for (int i = 0; i < vCount; i++)
            if (*vehicles[i] == *v) { cout << "  [Duplicate] Vehicle ID already exists.\n"; return; }
        if (vCount < 200) vehicles[vCount++] = v;
    }

    void addListing(Listing l) {
        // Check for duplicate using operator==
        for (int i = 0; i < lCount; i++)
            if (listings[i] == l) { cout << "  [Duplicate] Listing ID already exists.\n"; return; }
        if (lCount < 200) {
            listings[lCount++] = l;
            // Increase seller's listing count
            for (int j = 0; j < sCount; j++)
                if (sellers[j] && sellers[j]->getID() == l.getSellerID())
                    sellers[j]->increaseListings();
        }
    }

    void addBuyer(Buyer* b)   { if (bCount < 50) buyers[bCount++]   = b; }
    void addSeller(Seller* s) { if (sCount < 50) sellers[sCount++]  = s; }
    void addAdmin(Admin* a)   { if (aCount < 20) admins[aCount++]   = a; }

    // ---- Display methods ----
    void showVehicles() const {
        cout << "\n  ---- All Vehicles ----\n";
        for (int i = 0; i < vCount; i++) vehicles[i]->displayDetails(); // virtual dispatch
    }

    void showListings() const {
        cout << "\n  ---- All Listings ----\n";
        for (int i = 0; i < lCount; i++)
            if (listings[i].getVehicleID() != 0) listings[i].display();
    }

    void showUsers() const {
        cout << "\n  ---- All Users ----\n";
        for (int i = 0; i < bCount;  i++) if (buyers[i])  buyers[i]->display();
        for (int i = 0; i < sCount;  i++) if (sellers[i]) sellers[i]->display();
        for (int i = 0; i < aCount;  i++) if (admins[i])  admins[i]->display();
    }

    // ---- Listing management ----
    void approveListing(int vehicleID) {
        for (int i = 0; i < lCount; i++)
            if (listings[i].getVehicleID() == vehicleID) {
                listings[i].approve();  // IApprovable polymorphism
                for (int j = 0; j < aCount; j++)
                    if (admins[j]) admins[j]->performAction();
                return;
            }
        cout << "  Listing not found.\n";
    }

    void deleteListing(int vehicleID) {
        for (int i = 0; i < lCount; i++)
            if (listings[i].getVehicleID() == vehicleID) {
                listings[i].reject();   // IApprovable polymorphism
                return;
            }
        cout << "  Listing not found.\n";
    }

    void updateListingPrice(int listingID, double newPrice) {
        for (int i = 0; i < lCount; i++)
            if (listings[i].getListingID() == listingID) {
                listings[i].updatePrice(newPrice);
                cout << "  Price updated.\n";
                return;
            }
        cout << "  Listing not found.\n";
    }

    // ---- Search methods (function overloading – POLYMORPHISM) ----
    void searchByBrand(const string& brand) const {
        cout << "\n  ---- Search: Brand = " << brand << " ----\n";
        for (int i = 0; i < vCount; i++)
            if (vehicles[i]->matchesCriteria("brand", brand))  // ISearchable virtual dispatch
                vehicles[i]->displayDetails();
    }

    void searchByBrand(const string& brand, const string& model) const {  // overload
        cout << "\n  ---- Search: Brand = " << brand << ", Model = " << model << " ----\n";
        for (int i = 0; i < vCount; i++)
            if (vehicles[i]->matchesCriteria("brand", brand)
             && vehicles[i]->matchesCriteria("model", model))
                vehicles[i]->displayDetails();
    }

    void searchByPrice(double price) const {
        cout << "\n  ---- Search: Price = " << price << " ----\n";
        for (int i = 0; i < vCount; i++)
            if (vehicles[i]->getPrice() == price) vehicles[i]->displayDetails();
    }

    void searchByYear(int year) const {
        cout << "\n  ---- Search: Year = " << year << " ----\n";
        for (int i = 0; i < vCount; i++)
            if (vehicles[i]->getYear() == year) vehicles[i]->displayDetails();
    }

    void searchByMileage(int mileage) const {
        cout << "\n  ---- Search: Mileage <= " << mileage << " km ----\n";
        for (int i = 0; i < vCount; i++)
            if (vehicles[i]->getMileage() <= mileage) vehicles[i]->displayDetails();
    }

    // ---- Demo helpers for new A02 features ----
    void demoOperatorOverloading() const {
        if (vCount >= 2) {
            cout << "\n  [Operator==] Same vehicle? "
                 << (*vehicles[0] == *vehicles[1] ? "Yes" : "No") << "\n";
            double combined = *vehicles[0] + *vehicles[1];
            cout << "  [Operator+]  Combined price: PKR " << combined << "\n";
        }
    }

    void demoFriendFunctions() const {
        if (vCount > 0 && lCount > 0)
            printVehicleSummary(*vehicles[0], listings[0]);
        if (bCount > 0 && lCount > 0)
            cout << "  [Friend] Buyer eligible for listing? "
                 << (isBuyerEligible(*buyers[0], listings[0].getListingPrice()) ? "Yes" : "No") << "\n";
        if (sCount >= 2)
            compareSellers(*sellers[0], *sellers[1]);
        if (aCount > 0)
            adminAudit(*admins[0]);
    }

    // Find sellers/buyers by index for messaging
    Buyer*  getBuyer(int idx)  const { return (idx<bCount) ? buyers[idx]  : nullptr; }
    Seller* getSeller(int idx) const { return (idx<sCount) ? sellers[idx] : nullptr; }
    Admin*  getAdmin(int idx)  const { return (idx<aCount) ? admins[idx]  : nullptr; }
    int getBuyerCount()  const { return bCount; }
    int getSellerCount() const { return sCount; }

    ~MarketplaceSystem() {
        for (int i = 0; i < vCount; i++) delete vehicles[i];
    }
};

int main() {
    MarketplaceSystem sys;
    int choice;

    cout << "\n\t\t\t" << SystemName << " (Assignment 02)\n";

    do {
        cout << "\n============ MENU ============\n";
        cout << " 1.  Create Buyer\n";
        cout << " 2.  Create Seller\n";
        cout << " 3.  Create Admin\n";
        cout << " 4.  Add Car\n";
        cout << " 5.  Add Bike\n";
        cout << " 6.  Add Listing\n";
        cout << " 7.  Buyer: Add Favourite\n";
        cout << " 8.  Send Message (Buyer → Seller)\n";
        cout << " 9.  View Seller Inbox\n";
        cout << " 10. Approve Listing\n";
        cout << " 11. Remove Listing\n";
        cout << " 12. Search Vehicle\n";
        cout << " 13. Display All Vehicles\n";
        cout << " 14. Display All Listings\n";
        cout << " 15. Update Listing Price\n";
        cout << " 16. Display All Users\n";
        cout << " 17. Demo – Operator Overloading\n";
        cout << " 18. Demo – Friend Functions\n";
        cout << " 19. Show Totals (static)\n";
        cout << " 20. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        cout << "------------------------------\n";

        if (choice == 1) {
            // Create Buyer (derived from User)
            int id; double bal;
            string n, c, e, p;
            cout << "User ID: "; cin >> id; cin.ignore();
            cout << "Name: ";    getline(cin, n);
            cout << "Contact: "; getline(cin, c);
            cout << "Email: ";   getline(cin, e);
            cout << "Password: ";getline(cin, p);
            cout << "Wallet Balance: "; cin >> bal; cin.ignore();

            Buyer* b = new Buyer(id, n, c, e, p, bal);
            sys.addBuyer(b);
            cout << "  Buyer created. Role: " << b->getRole() << "\n";
        }

        else if (choice == 2) {
            int id; string n, c, e, p;
            cout << "User ID: "; cin >> id; cin.ignore();
            cout << "Name: ";    getline(cin, n);
            cout << "Contact: "; getline(cin, c);
            cout << "Email: ";   getline(cin, e);
            cout << "Password: ";getline(cin, p);

            Seller* s = new Seller(id, n, c, e, p);
            sys.addSeller(s);
            cout << "  Seller created. Role: " << s->getRole() << "\n";
        }

        else if (choice == 3) {
            int id; string n, c, e, p; char sa;
            cout << "User ID: ";    cin >> id; cin.ignore();
            cout << "Name: ";       getline(cin, n);
            cout << "Contact: ";    getline(cin, c);
            cout << "Email: ";      getline(cin, e);
            cout << "Password: ";   getline(cin, p);
            cout << "Super Admin? (y/n): "; cin >> sa; cin.ignore();

            Admin* a = new Admin(id, n, c, e, p, (sa=='y'||sa=='Y'));
            sys.addAdmin(a);
            cout << "  Admin created. Role: " << a->getRole() << "\n";
        }

        else if (choice == 4) {
            // Add Car (derived from Vehicle)
            int id, year, mileage, doors; double price; bool ac;
            string brand, model, color, fuel, trans, desc, bodyType, city;
            char acChar;

            cout << "Vehicle ID: "; cin >> id; cin.ignore();
            cout << "Brand: ";      getline(cin, brand);
            cout << "Model: ";      getline(cin, model);
            cout << "Year: ";       cin >> year; cin.ignore();
            cout << "Price: ";      cin >> price; cin.ignore();
            cout << "Mileage (km): ";cin >> mileage; cin.ignore();
            cout << "Color: ";      getline(cin, color);
            cout << "Fuel Type: ";  getline(cin, fuel);
            cout << "Transmission: ";getline(cin, trans);
            cout << "Description: ";getline(cin, desc);
            cout << "Body Type (Sedan/SUV/Hatchback): "; getline(cin, bodyType);
            cout << "Doors: ";      cin >> doors; cin.ignore();
            cout << "AC? (y/n): ";  cin >> acChar; cin.ignore();
            cout << "Reg. City: ";  getline(cin, city);

            VehicleDetails vd(mileage, color, fuel, trans, desc);
            Car* car = new Car(id, brand, model, year, price, vd,
                               doors, bodyType, (acChar=='y'||acChar=='Y'), city);
            sys.addVehicle(car);
            cout << "  Car added successfully.\n";
        }

        else if (choice == 5) {
            // Add Bike (derived from Vehicle)
            int id, year, mileage, cc; double price; bool fairing;
            string brand, model, color, fuel, trans, desc, bikeType;
            char fairChar;

            cout << "Vehicle ID: "; cin >> id; cin.ignore();
            cout << "Brand: ";      getline(cin, brand);
            cout << "Model: ";      getline(cin, model);
            cout << "Year: ";       cin >> year; cin.ignore();
            cout << "Price: ";      cin >> price; cin.ignore();
            cout << "Mileage (km): ";cin >> mileage; cin.ignore();
            cout << "Color: ";      getline(cin, color);
            cout << "Fuel Type: ";  getline(cin, fuel);
            cout << "Transmission: ";getline(cin, trans);
            cout << "Description: ";getline(cin, desc);
            cout << "Bike Type (Sport/Cruiser/Commuter): ";getline(cin, bikeType);
            cout << "Engine CC: ";  cin >> cc; cin.ignore();
            cout << "Has Fairing? (y/n): ";cin >> fairChar; cin.ignore();

            VehicleDetails vd(mileage, color, fuel, trans, desc);
            Bike* bike = new Bike(id, brand, model, year, price, vd,
                                  cc, bikeType, (fairChar=='y'||fairChar=='Y'));
            sys.addVehicle(bike);
            cout << "  Bike added successfully.\n";
        }

        else if (choice == 6) {
            int lid, sid, vid; double p;
            cout << "Listing ID: ";  cin >> lid;
            cout << "Seller ID: ";   cin >> sid;
            cout << "Vehicle ID: ";  cin >> vid;
            cout << "Price: ";       cin >> p;
            cin.ignore();
            sys.addListing(Listing(lid, sid, vid, p));
            cout << "  Listing added (Pending).\n";
        }

        else if (choice == 7) {
            int bidx, vid;
            cout << "Buyer index (0-" << sys.getBuyerCount()-1 << "): "; cin >> bidx;
            cout << "Vehicle ID to favourite: "; cin >> vid; cin.ignore();
            Buyer* b = sys.getBuyer(bidx);
            if (b) { b->addFavorite(vid); b->showFavorites(); }
        }

        else if (choice == 8) {
            int bidx, sidx; string msg;
            cout << "Buyer index: ";  cin >> bidx;
            cout << "Seller index: "; cin >> sidx; cin.ignore();
            cout << "Message: ";      getline(cin, msg);
            Buyer*  b = sys.getBuyer(bidx);
            Seller* s = sys.getSeller(sidx);
            if (b && s) {
                Message* m = new Message(b->getID(), s->getID(), msg, "Today");
                b->sendMessage(m);
                s->receiveMessage(m);
                cout << "  Message sent.\n";
            }
        }

        else if (choice == 9) {
            int sidx;
            cout << "Seller index: "; cin >> sidx; cin.ignore();
            Seller* s = sys.getSeller(sidx);
            if (s) s->showMessages();
        }

        else if (choice == 10) {
            int vid; cout << "Vehicle ID: "; cin >> vid; cin.ignore();
            sys.approveListing(vid);
            cout << "  Listing approved.\n";
        }

        else if (choice == 11) {
            int vid; cout << "Vehicle ID: "; cin >> vid; cin.ignore();
            sys.deleteListing(vid);
            cout << "  Listing removed.\n";
        }

        else if (choice == 12) {
            int ch;
            cout << "Search by: 1-Brand  2-Brand+Model  3-Price  4-Year  5-Mileage\nChoice: ";
            cin >> ch; cin.ignore();
            if (ch == 1) { string b; cout<<"Brand: "; getline(cin,b); sys.searchByBrand(b); }
            else if (ch==2) { string b,m; cout<<"Brand: ";getline(cin,b);cout<<"Model: ";getline(cin,m); sys.searchByBrand(b,m); }
            else if (ch==3) { double p; cout<<"Price: ";cin>>p;cin.ignore(); sys.searchByPrice(p); }
            else if (ch==4) { int y;    cout<<"Year: "; cin>>y; cin.ignore(); sys.searchByYear(y); }
            else if (ch==5) { int ml;   cout<<"Max Mileage: ";cin>>ml;cin.ignore(); sys.searchByMileage(ml); }
        }

        else if (choice == 13) sys.showVehicles();
        else if (choice == 14) sys.showListings();

        else if (choice == 15) {
            int lid; double p;
            cout << "Listing ID: "; cin >> lid;
            cout << "New Price: ";  cin >> p; cin.ignore();
            sys.updateListingPrice(lid, p);
        }

        else if (choice == 16) sys.showUsers();
        else if (choice == 17) sys.demoOperatorOverloading();
        else if (choice == 18) sys.demoFriendFunctions();

        else if (choice == 19) {
            User::showTotalUsers();
            Vehicle::showTotalVehicles();
        }

    } while (choice != 20);

    cout << "\nThank you for using " << SystemName << "!\n";
    return 0;
}
