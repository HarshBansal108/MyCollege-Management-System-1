#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
#include<cctype>
#include<cstdlib>
using namespace std;

class User{
    int user_id;
    string fullName;
    string email;
    string password;
    string role;
    int collegeId;
    string department;
public:
    User(string fullName, string email, string password, string role, int collegeId, string department){
        this->user_id = 0;
        this->fullName = fullName;
        this->email = email;
        this->password = password;
        this->role = role;
        this->collegeId = collegeId;
        this->department = department;
    }
    void displayUser(){
        cout << "User ID: " << user_id << endl;
        cout << "Full Name: " << fullName << endl;
        cout << "Email: " << email << endl;
        cout << "Role: " << role << endl;
        cout << "College ID: " << collegeId << endl;
        cout << "Department: " << department << endl;
    }

    // Setters
    void setUserId(int user_id){
        this->user_id = user_id;
    }

    void setFullName(string fullName){
        this->fullName = fullName;
    }

    void setEmail(string email){
        this->email = email;
    }

    void setPassword(string password){
        this->password = password;
    }

    void setRole(string role){
        this->role = role;
    }

    void setCollegeId(int collegeId){
        this->collegeId = collegeId;
    }

    void setDepartment(string department)
    {
        this->department = department;
    }

    // Getters
    int getUserId(){
        return user_id;
    }

    string getFullName(){
        return fullName;
    }

    string getEmail()
    {
        return email;
    }

    string getPassword(){
        return password;
    }

    string getRole(){
        return role;
    }

    int getCollegeId(){
        return collegeId;
    }

    string getDepartment(){
        return department;
    }
};


/*Student and Faculty (each stores the IDs of their friends) */

class Student : public User{
    vector<int> friendIds;
public:
    Student(User u) : User(u) {}

    void addFriend(int id){
        if(find(friendIds.begin(), friendIds.end(), id) == friendIds.end()){
            friendIds.push_back(id);
        }
    }

    vector<int> getFriendIds(){
        return friendIds;
    }
};

class Faculty : public User{
    vector<int> friendIds;
public:
    Faculty(User u) : User(u) {}

    void addFriend(int id){
        if(find(friendIds.begin(), friendIds.end(), id) == friendIds.end()){
            friendIds.push_back(id);
        }
    }

    vector<int> getFriendIds(){
        return friendIds;
    }
};

// Adjacency list: user ID -> list of connected user IDs
typedef map<int, vector<int> > AdjList;


/* ---------- Helper functions ---------- */

string toLower(string s){
    for(int i = 0; i < s.length(); i++){
        if(s[i] >= 'A' && s[i] <= 'Z'){
            s[i] += 32;
        }
    }
    return s;
}

// Reads one full line; exits cleanly if the input stream ends.
string readLine(const string& s){
    string line;
    cout << s;
    if(!getline(cin, line)){
        cout << endl << "Input ended. Exiting." << endl;
        exit(0);
    }
    return line;
}

string readNonEmpty(const string& s){
    while(true){
        string line = readLine(s);
        if(!line.empty()){
            return line;
        }
        cout << "This field cannot be empty, please try again!" << endl;
    }
}

int readInt(const string& s){
    while(true){
        string line = readLine(s);
        // Accept only digits (max 9 so the value always fits in an int)
        bool valid = !line.empty() && line.size() <= 9;
        for(int i = 0; i < line.size(); i++){
            if(!isdigit(line[i])){
                valid = false;
            }
        }
        if(valid){
            return stoi(line);
        }
        cout << "Invalid number, please try again!" << endl;
    }
}

bool validEmail(const string& e){
    int at = e.find('@');
    if(at == string::npos || at == 0 || e.find(' ') != string::npos){
        return false;
    }
    int dot = e.find('.', at);
    bool ret = (dot != string::npos) && (dot > at + 1) && (dot + 1 < e.size());
    return ret;
}

int findByEmail(vector<User>& usr, const string& email){
    for(int i = 0; i < usr.size(); i++){
        string str = toLower(usr[i].getEmail());
        if(str== toLower(email)){
            return i;
        }
    }
    return -1;
}

bool collegeIdExists(vector<User>& usr, int collegeId){
    for(int i = 0; i < usr.size(); i++){
        if(usr[i].getCollegeId() == collegeId){
            return true;
        }
    }
    return false;
}

// Copies a User into the Student or Faculty list depending on its role
void addToRoleList(User& u, vector<Student>& students, vector<Faculty>& faculty){
    if(u.getRole() == "student"){
        students.push_back(Student(u));
    }else if(u.getRole() == "faculty"){
        faculty.push_back(Faculty(u));
    }
}

// Adds a two-way connection between users a and b (no duplicates)
void addEdge(AdjList& adjList, int a, int b){
    if(find(adjList[a].begin(), adjList[a].end(), b) == adjList[a].end()){
        adjList[a].push_back(b);
    }
    if(find(adjList[b].begin(), adjList[b].end(), a) == adjList[b].end()){
        adjList[b].push_back(a);
    }
}

// Builds the adjacency list from the friend IDs stored inside every Student and Faculty
void buildAdjList(vector<Student>& students, vector<Faculty>& faculty, AdjList& adjList){
    for(int i = 0; i < students.size(); i++){
        vector<int> temp = students[i].getFriendIds();
        for(int j = 0; j < temp.size(); j++){
            addEdge(adjList, students[i].getUserId(), temp[j]);
        }
    }
    for(int i = 0; i < faculty.size(); i++){
        vector<int> temp = faculty[i].getFriendIds();
        for(int j = 0; j < temp.size(); j++){
            addEdge(adjList, faculty[i].getUserId(), temp[j]);
        }
    }
}


// Returns true if users a and b are already connected
bool isConnected(AdjList& adjList, int a, int b){
    AdjList::iterator it = adjList.find(a);
    if(it == adjList.end()){
        return false;
    }
    return find(it->second.begin(), it->second.end(), b) != it->second.end();
}

// Stores friendId inside the Student or Faculty object that has the given user ID
void addFriendById(vector<Student>& students, vector<Faculty>& faculty, int id, int friendId){
    for(int i = 0; i < students.size(); i++){
        if(students[i].getUserId() == id){
            students[i].addFriend(friendId);
            return;
        }
    }
    for(int i = 0; i < faculty.size(); i++){
        if(faculty[i].getUserId() == id){
            faculty[i].addFriend(friendId);
            return;
        }
    }
}

// Connects two users: updates the adjacency list and both users' friend ID lists
void connectUsers(AdjList& adjList, vector<Student>& students, vector<Faculty>& faculty, int a, int b){
    addEdge(adjList, a, b);
    addFriendById(students, faculty, a, b);
    addFriendById(students, faculty, b, a);
}


/* ---------- Features ---------- */

void registerUser(vector<User>& usr, int& nextId){
    cout << endl << "--- MyCollege Registration ---" << endl;

    string name = readNonEmpty("Enter full name: ");

    string email;
    while(true){
        email = readNonEmpty("Enter email: ");
        if(!validEmail(email)){
            cout << "Invalid email format, please try again!" << endl;
        }else if(findByEmail(usr, email) != -1){
            cout << "This email is already registered, please use another!" << endl;
        }else{
            break;
        }
    }

    string password;
    while(true){
        password = readNonEmpty("Enter password (min 6 characters): ");
        if(password.size() < 6){
            cout << "Password is too short, please try again!" << endl;
        }else{
            break;
        }
    }

    string role;
    while(true){
        role = toLower(readNonEmpty("Select role (student/faculty): "));
        if(role == "student" || role == "faculty"){
            break;
        }
        cout << "Invalid role , please try again!" << endl;
    }

    int collegeId;
    while(true){
        collegeId = readInt("Enter college ID: ");
        if(collegeIdExists(usr, collegeId)){
            cout << "This college ID is already registered, please try again!" << endl;
        }else{
            break;
        }
    }

    string department = readNonEmpty("Enter department: ");

    User newUser(name, email, password, role, collegeId, department);

    newUser.setUserId(nextId++);
    usr.push_back(newUser);

    cout << endl << "Registration successful!" << endl;
    usr.back().displayUser();
}

void searchUser(vector<User>& usr, AdjList& adjList, vector<Student>& students, vector<Faculty>& faculty, User& me){
    string name = toLower(readNonEmpty("Search for a user by name: "));
    int found = 0;
    for(int i = 0; i < usr.size(); i++){
        if(toLower(usr[i].getFullName()).find(name) != string::npos){
            cout << endl;
            usr[i].displayUser();
            found++;

            if(usr[i].getUserId() == me.getUserId()){
                cout << "-> This is you." << endl;
            }else if(isConnected(adjList, me.getUserId(), usr[i].getUserId())){
                cout << "-> Already in your connections." << endl;
            }else{
                cout << "-> Not in your connections yet." << endl;
                string ans;
                while(true){
                    ans = toLower(readNonEmpty("Add to connections? (y/n): "));
                    if(ans == "y" || ans == "yes" || ans == "n" || ans == "no"){
                        break;
                    }
                    cout << "Please enter y or n." << endl;
                }
                if(ans == "y" || ans == "yes"){
                    connectUsers(adjList, students, faculty, me.getUserId(), usr[i].getUserId());
                    cout << "Connection added!" << endl;
                }else{
                    cout << "Okay, connection not added." << endl;
                }
            }
        }
    }
    if(found == 0){
        cout << "No such user with this name" << endl;
    }
}

// Prints everyone the logged-in user is connected to, using the adjacency list
void showConnections(vector<User>& usr, AdjList& adjList, User& me){
    AdjList::iterator it = adjList.find(me.getUserId());
    if(it == adjList.end() || it->second.empty()){
        cout << endl << "You have no connections yet." << endl;
        return;
    }
    cout << endl << "--- Your Connections (" << it->second.size() << ") ---" << endl;
    for(int i = 0; i < it->second.size(); i++){
        for(int j = 0; j < usr.size(); j++){
            if(usr[j].getUserId() == it->second[i]){
                cout << usr[j].getUserId() << ". " << usr[j].getFullName() << " (" << usr[j].getRole() << ", " << usr[j].getDepartment() << ")" << endl;
            }
        }
    }
}

void userMenu(vector<User>& usr, User& me, AdjList& adjList, vector<Student>& students, vector<Faculty>& faculty){
    int choice;
    do{
        cout << endl << "<--- Welcome, " << me.getFullName() << " (" << me.getRole() << ") --->" << endl;
        cout << "1. View my profile" << endl;
        cout << "2. Search for a user by name" << endl;
        cout << "3. Show connections" << endl;
        cout << "4. Logout" << endl;
        choice = readInt("Enter your choice: ");
        switch(choice){
            case 1:
                cout << endl;
                me.displayUser();
                break;
            case 2:
                searchUser(usr, adjList, students, faculty, me);
                break;
            case 3:
                showConnections(usr, adjList, me);
                break;
            case 4:
                cout << "Logged out successfully." << endl;
                break;
            default:
                cout << "Invalid choice, please try again!" << endl;
        }
    }while(choice != 4);
}

void loginUser(vector<User>& usr, AdjList& adjList, vector<Student>& students, vector<Faculty>& faculty){
    cout << endl << "<--- MyCollege Login --->" << endl;
    const int MAX_ATTEMPTS = 3;
    for(int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++){
        string email = readNonEmpty("Enter email: ");
        string password = readNonEmpty("Enter password: ");

        int idx = findByEmail(usr, email);
        if(idx != -1 && usr[idx].getPassword() == password){
            cout << endl << "Login successful!" << endl;
            userMenu(usr, usr[idx], adjList, students, faculty);
            return;
        }
        cout << "Incorrect email or password. Attempts left: " << (MAX_ATTEMPTS - attempt) << endl;
    }
    cout << "Too many failed attempts. Returning to main menu." << endl;
}


int main(){
    vector<User> usr = {
        User("Harsh Bansal", "harshxyz@email.com", "harsh123", "student", 1234, "Computer Science"),
        User("Ankit Kumar", "ankitxyz@email.com", "ankit123", "faculty", 9978, "Computer Science"),
        User("Aman Gupta", "amanxyz@email.com", "aman123", "student", 8765, "Computer Science")
    };
    for(int i = 0; i < usr.size(); i++){
        usr[i].setUserId(i + 1);
    }
    int nextId = usr.size() + 1;

    // Split the users into Student and Faculty objects based on their role
    vector<Student> students;
    vector<Faculty> faculty;
    for(int i = 0; i < usr.size(); i++){
        addToRoleList(usr[i], students, faculty);
    }

    // Sample friendships (stored as friend IDs inside the Student / Faculty objects)
    students[0].addFriend(2);   // Harsh Bansal -> Ankit Kumar
    faculty[0].addFriend(3);    // Ankit Kumar  -> Aman Gupta

    // Build the adjacency list from those friend IDs
    AdjList adjList;
    buildAdjList(students, faculty, adjList);

    int input;
    /* USER LOGIN AND AUTHENTICATION */
    do{
        cout << endl << "<------- MY COLLEGE -------->" << endl;
        cout << "Enter 1 for Registration" << endl;
        cout << "Enter 2 for Login" << endl;
        cout << "Enter 3 for Exit" << endl;
        input = readInt("\nEnter your input here : ");

        switch(input){
            case 1:
                registerUser(usr, nextId);
                addToRoleList(usr.back(), students, faculty);
                break;
            case 2:
                loginUser(usr, adjList, students, faculty);
                break;
            case 3:
                cout << "Thank you for using MyCollege. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid input, please enter 1, 2 or 3." << endl;
        }
    }while(input != 3);
    
    return 0;
}