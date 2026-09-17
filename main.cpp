#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

// ============================================================
//                 STUDENT FEEDBACK SYSTEM
// ============================================================
// Project: EduFeed - Student Feedback Management System
// Language: C++
// Storage: Text Files
// ============================================================


// ============================================================
//                    DATA STRUCTURES
// ============================================================

struct Student
{
    string id;
    string name;
    string password;
    string department;
};

struct Teacher
{
    string id;
    string name;
    string subject;
    string department;
};

struct Feedback
{
    string studentId;
    string teacherId;

    int teachingQuality;
    int subjectKnowledge;
    int communication;
    int courseCoverage;
    int interaction;

    double overallRating;

    bool anonymous;

    string comment;
};


// ============================================================
//                    GLOBAL VARIABLES
// ============================================================

vector<Student> students;
vector<Teacher> teachers;
vector<Feedback> feedbacks;

const string STUDENT_FILE = "students.txt";
const string TEACHER_FILE = "teachers.txt";
const string FEEDBACK_FILE = "feedback.txt";


// ============================================================
//                    UTILITY FUNCTIONS
// ============================================================

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


void printLine(char ch = '=', int length = 70)
{
    cout << string(length, ch) << "\n";
}


void printHeader(const string& title)
{
    cout << "\n";
    printLine('=');
    cout << setw(42) << title << "\n";
    printLine('=');
}


string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");

    if (first == string::npos)
        return "";

    size_t last = str.find_last_not_of(" \t\r\n");

    return str.substr(first, last - first + 1);
}


bool containsPipe(const string& str)
{
    return str.find('|') != string::npos;
}


string getNonEmptyInput(const string& prompt)
{
    string input;

    while (true)
    {
        cout << prompt;
        getline(cin, input);

        input = trim(input);

        if (input.empty())
        {
            cout << "Input cannot be empty. Please try again.\n";
        }
        else if (containsPipe(input))
        {
            cout << "The character '|' is not allowed.\n";
        }
        else
        {
            return input;
        }
    }
}


int getInteger(const string& prompt, int minValue, int maxValue)
{
    int value;

    while (true)
    {
        cout << prompt;

        if (cin >> value && value >= minValue && value <= maxValue)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input. Enter a number between "
             << minValue << " and " << maxValue << ".\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}


string getPassword(const string& prompt)
{
    string password;

    while (true)
    {
        cout << prompt;
        getline(cin, password);

        password = trim(password);

        if (password.empty())
        {
            cout << "Password cannot be empty.\n";
        }
        else if (containsPipe(password))
        {
            cout << "The character '|' is not allowed.\n";
        }
        else
        {
            return password;
        }
    }
}


// ============================================================
//                    FILE HANDLING
// ============================================================

void saveStudents()
{
    ofstream file(STUDENT_FILE);

    if (!file)
    {
        cout << "Error opening students file.\n";
        return;
    }

    for (const Student& s : students)
    {
        file << s.id << "|"
             << s.name << "|"
             << s.password << "|"
             << s.department << "\n";
    }

    file.close();
}


void saveTeachers()
{
    ofstream file(TEACHER_FILE);

    if (!file)
    {
        cout << "Error opening teachers file.\n";
        return;
    }

    for (const Teacher& t : teachers)
    {
        file << t.id << "|"
             << t.name << "|"
             << t.subject << "|"
             << t.department << "\n";
    }

    file.close();
}


void saveFeedbacks()
{
    ofstream file(FEEDBACK_FILE);

    if (!file)
    {
        cout << "Error opening feedback file.\n";
        return;
    }

    for (const Feedback& f : feedbacks)
    {
        file << f.studentId << "|"
             << f.teacherId << "|"
             << f.teachingQuality << "|"
             << f.subjectKnowledge << "|"
             << f.communication << "|"
             << f.courseCoverage << "|"
             << f.interaction << "|"
             << fixed << setprecision(2) << f.overallRating << "|"
             << f.anonymous << "|"
             << f.comment << "\n";
    }

    file.close();
}


// ============================================================
//                    LOAD STUDENTS
// ============================================================

void loadStudents()
{
    students.clear();

    ifstream file(STUDENT_FILE);

    if (!file)
        return;

    string line;

    while (getline(file, line))
    {
        if (trim(line).empty())
            continue;

        stringstream ss(line);
        Student s;

        getline(ss, s.id, '|');
        getline(ss, s.name, '|');
        getline(ss, s.password, '|');
        getline(ss, s.department, '|');

        if (!s.id.empty())
        {
            students.push_back(s);
        }
    }

    file.close();
}


// ============================================================
//                    LOAD TEACHERS
// ============================================================

void loadTeachers()
{
    teachers.clear();

    ifstream file(TEACHER_FILE);

    if (!file)
        return;

    string line;

    while (getline(file, line))
    {
        if (trim(line).empty())
            continue;

        stringstream ss(line);
        Teacher t;

        getline(ss, t.id, '|');
        getline(ss, t.name, '|');
        getline(ss, t.subject, '|');
        getline(ss, t.department, '|');

        if (!t.id.empty())
        {
            teachers.push_back(t);
        }
    }

    file.close();
}


// ============================================================
//                    LOAD FEEDBACK
// ============================================================

void loadFeedbacks()
{
    feedbacks.clear();

    ifstream file(FEEDBACK_FILE);

    if (!file)
        return;

    string line;

    while (getline(file, line))
    {
        if (trim(line).empty())
            continue;

        stringstream ss(line);

        Feedback f;

        string anonymousValue;

        getline(ss, f.studentId, '|');
        getline(ss, f.teacherId, '|');

        string temp;

        getline(ss, temp, '|');
        f.teachingQuality = stoi(temp);

        getline(ss, temp, '|');
        f.subjectKnowledge = stoi(temp);

        getline(ss, temp, '|');
        f.communication = stoi(temp);

        getline(ss, temp, '|');
        f.courseCoverage = stoi(temp);

        getline(ss, temp, '|');
        f.interaction = stoi(temp);

        getline(ss, temp, '|');
        f.overallRating = stod(temp);

        getline(ss, anonymousValue, '|');
        f.anonymous = (anonymousValue == "1");

        getline(ss, f.comment, '|');

        feedbacks.push_back(f);
    }

    file.close();
}


// ============================================================
//                    LOAD ALL DATA
// ============================================================

void loadAllData()
{
    loadStudents();
    loadTeachers();
    loadFeedbacks();
}


// ============================================================
//                    ID GENERATORS
// ============================================================

string generateStudentId()
{
    int maxNumber = 0;

    for (const Student& s : students)
    {
        if (s.id.length() > 1 && s.id[0] == 'S')
        {
            try
            {
                int number = stoi(s.id.substr(1));
                maxNumber = max(maxNumber, number);
            }
            catch (...)
            {
                // Ignore invalid IDs
            }
        }
    }

    return "S" + string(
        max(3, (int)to_string(maxNumber + 1).length()) -
        to_string(maxNumber + 1).length(),
        '0'
    ) + to_string(maxNumber + 1);
}


string generateTeacherId()
{
    int maxNumber = 0;

    for (const Teacher& t : teachers)
    {
        if (t.id.length() > 1 && t.id[0] == 'T')
        {
            try
            {
                int number = stoi(t.id.substr(1));
                maxNumber = max(maxNumber, number);
            }
            catch (...)
            {
                // Ignore invalid IDs
            }
        }
    }

    return "T" + string(
        max(3, (int)to_string(maxNumber + 1).length()) -
        to_string(maxNumber + 1).length(),
        '0'
    ) + to_string(maxNumber + 1);
}


// ============================================================
//                    FIND FUNCTIONS
// ============================================================

Student* findStudentById(const string& id)
{
    for (Student& s : students)
    {
        if (s.id == id)
            return &s;
    }

    return nullptr;
}


Teacher* findTeacherById(const string& id)
{
    for (Teacher& t : teachers)
    {
        if (t.id == id)
            return &t;
    }

    return nullptr;
}


// ============================================================
//                    STUDENT REGISTRATION
// ============================================================

void studentRegistration()
{
    clearScreen();

    printHeader("STUDENT REGISTRATION");

    Student s;

    s.id = generateStudentId();

    cout << "Your Student ID is: " << s.id << "\n\n";

    s.name = getNonEmptyInput("Enter your name: ");

    s.department = getNonEmptyInput("Enter your department: ");

    while (true)
    {
        s.password = getPassword("Create password: ");

        string confirmPassword =
            getPassword("Confirm password: ");

        if (s.password == confirmPassword)
            break;

        cout << "Passwords do not match. Try again.\n";
    }

    students.push_back(s);

    saveStudents();

    cout << "\n";
    printLine('-');

    cout << "Registration successful!\n";
    cout << "Student ID : " << s.id << "\n";
    cout << "Name       : " << s.name << "\n";
    cout << "Department : " << s.department << "\n";

    printLine('-');

    pauseScreen();
}


// ============================================================
//                    STUDENT LOGIN
// ============================================================

void studentLogin();


// ============================================================
//                    VIEW TEACHERS
// ============================================================

void viewTeachers()
{
    clearScreen();

    printHeader("AVAILABLE TEACHERS");

    if (teachers.empty())
    {
        cout << "No teachers available.\n";
        pauseScreen();
        return;
    }

    cout << left
         << setw(10) << "ID"
         << setw(25) << "Teacher Name"
         << setw(25) << "Subject"
         << setw(15) << "Department"
         << "\n";

    printLine('-');

    for (const Teacher& t : teachers)
    {
        cout << left
             << setw(10) << t.id
             << setw(25) << t.name
             << setw(25) << t.subject
             << setw(15) << t.department
             << "\n";
    }

    pauseScreen();
}


// ============================================================
//              CHECK DUPLICATE FEEDBACK
// ============================================================

bool hasAlreadySubmitted(
    const string& studentId,
    const string& teacherId)
{
    for (const Feedback& f : feedbacks)
    {
        if (f.studentId == studentId &&
            f.teacherId == teacherId)
        {
            return true;
        }
    }

    return false;
}


// ============================================================
//              CALCULATE OVERALL RATING
// ============================================================

double calculateAverage(
    int teachingQuality,
    int subjectKnowledge,
    int communication,
    int courseCoverage,
    int interaction)
{
    return (
        teachingQuality +
        subjectKnowledge +
        communication +
        courseCoverage +
        interaction
    ) / 5.0;
}


// ============================================================
//                    SUBMIT FEEDBACK
// ============================================================

void submitFeedback(const string& studentId)
{
    clearScreen();

    printHeader("SUBMIT FEEDBACK");

    if (teachers.empty())
    {
        cout << "No teachers are currently available.\n";
        pauseScreen();
        return;
    }

    cout << "Available Teachers:\n\n";

    cout << left
         << setw(10) << "ID"
         << setw(25) << "Teacher"
         << setw(25) << "Subject"
         << "\n";

    printLine('-');

    for (const Teacher& t : teachers)
    {
        cout << left
             << setw(10) << t.id
             << setw(25) << t.name
             << setw(25) << t.subject
             << "\n";
    }

    printLine('-');

    string teacherId =
        getNonEmptyInput("Enter Teacher ID: ");

    Teacher* teacher = findTeacherById(teacherId);

    if (teacher == nullptr)
    {
        cout << "Teacher not found.\n";
        pauseScreen();
        return;
    }

    if (hasAlreadySubmitted(studentId, teacherId))
    {
        cout << "\nYou have already submitted feedback for "
             << teacher->name
             << " - "
             << teacher->subject
             << ".\n";

        cout << "Duplicate feedback is not allowed.\n";

        pauseScreen();
        return;
    }

    Feedback f;

    f.studentId = studentId;
    f.teacherId = teacherId;

    cout << "\n";
    printLine('-');

    cout << "Teacher : " << teacher->name << "\n";
    cout << "Subject : " << teacher->subject << "\n";

    printLine('-');

    cout << "\nRate each category from 1 to 5.\n";
    cout << "1 = Poor\n";
    cout << "2 = Fair\n";
    cout << "3 = Good\n";
    cout << "4 = Very Good\n";
    cout << "5 = Excellent\n\n";

    f.teachingQuality =
        getInteger("Teaching Quality       : ", 1, 5);

    f.subjectKnowledge =
        getInteger("Subject Knowledge      : ", 1, 5);

    f.communication =
        getInteger("Communication          : ", 1, 5);

    f.courseCoverage =
        getInteger("Course Coverage        : ", 1, 5);

    f.interaction =
        getInteger("Student Interaction    : ", 1, 5);

    f.overallRating = calculateAverage(
        f.teachingQuality,
        f.subjectKnowledge,
        f.communication,
        f.courseCoverage,
        f.interaction
    );

    cout << "\nOverall Rating: "
         << fixed << setprecision(2)
         << f.overallRating
         << " / 5\n";

    int anonymousChoice =
        getInteger(
            "\nSubmit anonymously?\n1. Yes\n2. No\nChoice: ",
            1,
            2
        );

    f.anonymous = (anonymousChoice == 1);

    f.comment =
        getNonEmptyInput("\nEnter your comments: ");

    feedbacks.push_back(f);

    saveFeedbacks();

    cout << "\n";
    printLine('=');

    cout << "FEEDBACK SUBMITTED SUCCESSFULLY!\n";

    printLine('=');

    pauseScreen();
}


// ============================================================
//                PERFORMANCE CLASSIFICATION
// ============================================================

string getPerformance(double rating)
{
    if (rating >= 4.5)
        return "EXCELLENT";

    if (rating >= 4.0)
        return "VERY GOOD";

    if (rating >= 3.0)
        return "GOOD";

    if (rating >= 2.0)
        return "NEEDS IMPROVEMENT";

    return "POOR";
}


// ============================================================
//              GET TEACHER AVERAGE RATING
// ============================================================

double getTeacherAverage(const string& teacherId)
{
    double total = 0;
    int count = 0;

    for (const Feedback& f : feedbacks)
    {
        if (f.teacherId == teacherId)
        {
            total += f.overallRating;
            count++;
        }
    }

    if (count == 0)
        return 0;

    return total / count;
}


// ============================================================
//              COUNT TEACHER FEEDBACK
// ============================================================

int getTeacherFeedbackCount(const string& teacherId)
{
    int count = 0;

    for (const Feedback& f : feedbacks)
    {
        if (f.teacherId == teacherId)
            count++;
    }

    return count;
}


// ============================================================
//             VIEW MY SUBMITTED FEEDBACK
// ============================================================

void viewMyFeedback(const string& studentId)
{
    clearScreen();

    printHeader("MY SUBMITTED FEEDBACK");

    bool found = false;

    for (const Feedback& f : feedbacks)
    {
        if (f.studentId == studentId)
        {
            found = true;

            Teacher* teacher =
                findTeacherById(f.teacherId);

            cout << "\nTeacher : "
                 << (teacher ? teacher->name : "Unknown")
                 << "\n";

            cout << "Subject : "
                 << (teacher ? teacher->subject : "Unknown")
                 << "\n";

            cout << "Teaching Quality   : "
                 << f.teachingQuality << "/5\n";

            cout << "Subject Knowledge  : "
                 << f.subjectKnowledge << "/5\n";

            cout << "Communication      : "
                 << f.communication << "/5\n";

            cout << "Course Coverage    : "
                 << f.courseCoverage << "/5\n";

            cout << "Interaction        : "
                 << f.interaction << "/5\n";

            cout << "Overall Rating     : "
                 << fixed << setprecision(2)
                 << f.overallRating
                 << "/5\n";

            cout << "Anonymous          : "
                 << (f.anonymous ? "Yes" : "No")
                 << "\n";

            cout << "Comment            : "
                 << f.comment
                 << "\n";

            printLine('-');
        }
    }

    if (!found)
    {
        cout << "You have not submitted any feedback yet.\n";
    }

    pauseScreen();
}


// ============================================================
//                CHANGE STUDENT PASSWORD
// ============================================================

void changePassword(const string& studentId)
{
    clearScreen();

    printHeader("CHANGE PASSWORD");

    Student* student = findStudentById(studentId);

    if (!student)
    {
        cout << "Student not found.\n";
        pauseScreen();
        return;
    }

    string oldPassword =
        getPassword("Enter current password: ");

    if (oldPassword != student->password)
    {
        cout << "Incorrect current password.\n";
        pauseScreen();
        return;
    }

    string newPassword =
        getPassword("Enter new password: ");

    string confirmPassword =
        getPassword("Confirm new password: ");

    if (newPassword != confirmPassword)
    {
        cout << "Passwords do not match.\n";
        pauseScreen();
        return;
    }

    student->password = newPassword;

    saveStudents();

    cout << "Password changed successfully.\n";

    pauseScreen();
}


// ============================================================
//                    STUDENT DASHBOARD
// ============================================================

void studentDashboard(const string& studentId)
{
    while (true)
    {
        clearScreen();

        Student* student =
            findStudentById(studentId);

        if (!student)
            return;

        printHeader("STUDENT DASHBOARD");

        cout << "Welcome, " << student->name << "!\n";
        cout << "Student ID : " << student->id << "\n";
        cout << "Department : " << student->department << "\n\n";

        printLine('-');

        cout << "1. View Available Teachers\n";
        cout << "2. Submit Feedback\n";
        cout << "3. View My Submitted Feedback\n";
        cout << "4. Change Password\n";
        cout << "5. Logout\n";

        printLine('-');

        int choice =
            getInteger("Enter your choice: ", 1, 5);

        switch (choice)
        {
        case 1:
            viewTeachers();
            break;

        case 2:
            submitFeedback(studentId);
            break;

        case 3:
            viewMyFeedback(studentId);
            break;

        case 4:
            changePassword(studentId);
            break;

        case 5:
            return;
        }
    }
}


// ============================================================
//                    STUDENT LOGIN
// ============================================================

void studentLogin()
{
    clearScreen();

    printHeader("STUDENT LOGIN");

    if (students.empty())
    {
        cout << "No students registered yet.\n";
        cout << "Please register first.\n";
        pauseScreen();
        return;
    }

    string id =
        getNonEmptyInput("Student ID: ");

    string password =
        getPassword("Password: ");

    Student* student =
        findStudentById(id);

    if (student &&
        student->password == password)
    {
        cout << "\nLogin successful!\n";

        pauseScreen();

        studentDashboard(student->id);
    }
    else
    {
        cout << "\nInvalid Student ID or password.\n";
        pauseScreen();
    }
}


// ============================================================
//                    ADD TEACHER
// ============================================================

void addTeacher()
{
    clearScreen();

    printHeader("ADD TEACHER");

    Teacher t;

    t.id = generateTeacherId();

    cout << "Generated Teacher ID: "
         << t.id << "\n\n";

    t.name =
        getNonEmptyInput("Enter teacher name: ");

    t.subject =
        getNonEmptyInput("Enter subject: ");

    t.department =
        getNonEmptyInput("Enter department: ");

    teachers.push_back(t);

    saveTeachers();

    cout << "\nTeacher added successfully!\n";
    cout << "Teacher ID: " << t.id << "\n";

    pauseScreen();
}


// ============================================================
//                    REMOVE TEACHER
// ============================================================

void removeTeacher()
{
    clearScreen();

    printHeader("REMOVE TEACHER");

    if (teachers.empty())
    {
        cout << "No teachers available.\n";
        pauseScreen();
        return;
    }

    viewTeachers();

    string id =
        getNonEmptyInput("Enter Teacher ID to remove: ");

    auto it = find_if(
        teachers.begin(),
        teachers.end(),
        [&](const Teacher& t)
        {
            return t.id == id;
        }
    );

    if (it == teachers.end())
    {
        cout << "Teacher not found.\n";
        pauseScreen();
        return;
    }

    cout << "\nTeacher found: "
         << it->name << "\n";

    int choice =
        getInteger(
            "Are you sure?\n1. Yes\n2. No\nChoice: ",
            1,
            2
        );

    if (choice == 1)
    {
        teachers.erase(it);

        saveTeachers();

        cout << "Teacher removed successfully.\n";
    }
    else
    {
        cout << "Operation cancelled.\n";
    }

    pauseScreen();
}


// ============================================================
//                  VIEW ALL FEEDBACK
// ============================================================

void viewAllFeedback()
{
    clearScreen();

    printHeader("ALL FEEDBACK");

    if (feedbacks.empty())
    {
        cout << "No feedback has been submitted yet.\n";
        pauseScreen();
        return;
    }

    int feedbackNumber = 1;

    for (const Feedback& f : feedbacks)
    {
        Teacher* teacher =
            findTeacherById(f.teacherId);

        Student* student =
            findStudentById(f.studentId);

        cout << "\nFeedback #" << feedbackNumber++ << "\n";

        printLine('-');

        cout << "Teacher ID   : " << f.teacherId << "\n";

        cout << "Teacher Name : "
             << (teacher ? teacher->name : "Unknown")
             << "\n";

        cout << "Subject      : "
             << (teacher ? teacher->subject : "Unknown")
             << "\n";

        if (f.anonymous)
        {
            cout << "Student      : Anonymous\n";
        }
        else
        {
            cout << "Student      : "
                 << (student ? student->name : "Unknown")
                 << " (" << f.studentId << ")\n";
        }

        cout << "\nRatings:\n";

        cout << "Teaching Quality  : "
             << f.teachingQuality << "/5\n";

        cout << "Subject Knowledge : "
             << f.subjectKnowledge << "/5\n";

        cout << "Communication     : "
             << f.communication << "/5\n";

        cout << "Course Coverage   : "
             << f.courseCoverage << "/5\n";

        cout << "Interaction       : "
             << f.interaction << "/5\n";

        cout << "Overall Rating    : "
             << fixed << setprecision(2)
             << f.overallRating
             << "/5\n";

        cout << "Comment           : "
             << f.comment << "\n";

        printLine('-');
    }

    pauseScreen();
}


// ============================================================
//                TEACHER-WISE REPORT
// ============================================================

void teacherWiseFeedback()
{
    clearScreen();

    printHeader("TEACHER-WISE FEEDBACK REPORT");

    if (teachers.empty())
    {
        cout << "No teachers available.\n";
        pauseScreen();
        return;
    }

    viewTeachers();

    string teacherId =
        getNonEmptyInput("Enter Teacher ID: ");

    Teacher* teacher =
        findTeacherById(teacherId);

    if (!teacher)
    {
        cout << "Teacher not found.\n";
        pauseScreen();
        return;
    }

    clearScreen();

    printHeader("TEACHER REPORT");

    cout << "Teacher     : "
         << teacher->name << "\n";

    cout << "Subject     : "
         << teacher->subject << "\n";

    cout << "Department  : "
         << teacher->department << "\n";

    int count = 0;

    double teaching = 0;
    double knowledge = 0;
    double communication = 0;
    double coverage = 0;
    double interaction = 0;
    double overall = 0;

    for (const Feedback& f : feedbacks)
    {
        if (f.teacherId == teacherId)
        {
            count++;

            teaching += f.teachingQuality;
            knowledge += f.subjectKnowledge;
            communication += f.communication;
            coverage += f.courseCoverage;
            interaction += f.interaction;
            overall += f.overallRating;
        }
    }

    if (count == 0)
    {
        cout << "\nNo feedback available for this teacher.\n";
        pauseScreen();
        return;
    }

    cout << "\nNumber of Feedbacks: "
         << count << "\n\n";

    cout << fixed << setprecision(2);

    cout << "Teaching Quality   : "
         << teaching / count << "/5\n";

    cout << "Subject Knowledge  : "
         << knowledge / count << "/5\n";

    cout << "Communication      : "
         << communication / count << "/5\n";

    cout << "Course Coverage    : "
         << coverage / count << "/5\n";

    cout << "Student Interaction: "
         << interaction / count << "/5\n";

    double average = overall / count;

    cout << "\nOverall Rating     : "
         << average << "/5\n";

    cout << "Performance        : "
         << getPerformance(average)
         << "\n";

    printLine('-');

    cout << "\nComments:\n\n";

    int commentNumber = 1;

    for (const Feedback& f : feedbacks)
    {
        if (f.teacherId == teacherId)
        {
            cout << commentNumber++
                 << ". "
                 << f.comment
                 << "\n";
        }
    }

    pauseScreen();
}


// ============================================================
//                GENERATE COMPLETE REPORT
// ============================================================

void generateReport()
{
    clearScreen();

    printHeader("FEEDBACK PERFORMANCE REPORT");

    if (teachers.empty())
    {
        cout << "No teachers available.\n";
        pauseScreen();
        return;
    }

    cout << left
         << setw(10) << "ID"
         << setw(25) << "Teacher"
         << setw(20) << "Subject"
         << setw(12) << "Feedbacks"
         << setw(10) << "Rating"
         << "\n";

    printLine('-');

    for (const Teacher& t : teachers)
    {
        int count =
            getTeacherFeedbackCount(t.id);

        double average =
            getTeacherAverage(t.id);

        cout << left
             << setw(10) << t.id
             << setw(25) << t.name
             << setw(20) << t.subject
             << setw(12) << count;

        if (count == 0)
        {
            cout << setw(10) << "N/A";
        }
        else
        {
            cout << setw(10)
                 << fixed << setprecision(2)
                 << average;
        }

        cout << "\n";
    }

    printLine('-');

    cout << "\nPerformance Scale:\n";

    cout << "4.5 - 5.0 : EXCELLENT\n";
    cout << "4.0 - 4.49: VERY GOOD\n";
    cout << "3.0 - 3.99: GOOD\n";
    cout << "2.0 - 2.99: NEEDS IMPROVEMENT\n";
    cout << "1.0 - 1.99: POOR\n";

    pauseScreen();
}


// ============================================================
//             SORT TEACHERS BY RATING
// ============================================================

void sortTeachersByRating()
{
    clearScreen();

    printHeader("TEACHERS SORTED BY RATING");

    vector<Teacher> sortedTeachers = teachers;

    sort(
        sortedTeachers.begin(),
        sortedTeachers.end(),
        [](const Teacher& a, const Teacher& b)
        {
            return getTeacherAverage(a.id)
                > getTeacherAverage(b.id);
        }
    );

    cout << left
         << setw(10) << "ID"
         << setw(25) << "Teacher"
         << setw(20) << "Subject"
         << setw(12) << "Feedbacks"
         << setw(10) << "Rating"
         << "\n";

    printLine('-');

    for (const Teacher& t : sortedTeachers)
    {
        int count =
            getTeacherFeedbackCount(t.id);

        double average =
            getTeacherAverage(t.id);

        cout << left
             << setw(10) << t.id
             << setw(25) << t.name
             << setw(20) << t.subject
             << setw(12) << count;

        if (count == 0)
        {
            cout << "N/A";
        }
        else
        {
            cout << fixed
                 << setprecision(2)
                 << average;
        }

        cout << "\n";
    }

    pauseScreen();
}


// ============================================================
//                    SEARCH FEEDBACK
// ============================================================

void searchFeedback()
{
    clearScreen();

    printHeader("SEARCH FEEDBACK");

    cout << "1. Search by Student ID\n";
    cout << "2. Search by Teacher ID\n";
    cout << "3. Search by Teacher Name\n";
    cout << "4. Search by Course\n";

    printLine('-');

    int choice =
        getInteger("Enter choice: ", 1, 4);

    string searchText =
        getNonEmptyInput("Enter search text: ");

    bool found = false;

    int resultNumber = 1;

    for (const Feedback& f : feedbacks)
    {
        Teacher* teacher =
            findTeacherById(f.teacherId);

        Student* student =
            findStudentById(f.studentId);

        bool match = false;

        switch (choice)
        {
        case 1:
            match =
                (f.studentId == searchText);
            break;

        case 2:
            match =
                (f.teacherId == searchText);
            break;

        case 3:
            if (teacher)
            {
                match =
                    (teacher->name.find(searchText)
                     != string::npos);
            }
            break;

        case 4:
            if (teacher)
            {
                match =
                    (teacher->subject.find(searchText)
                     != string::npos);
            }
            break;
        }

        if (match)
        {
            found = true;

            cout << "\nResult #"
                 << resultNumber++
                 << "\n";

            printLine('-');

            cout << "Teacher : "
                 << (teacher ? teacher->name : "Unknown")
                 << "\n";

            cout << "Course  : "
                 << (teacher ? teacher->subject : "Unknown")
                 << "\n";

            if (f.anonymous)
            {
                cout << "Student : Anonymous\n";
            }
            else
            {
                cout << "Student : "
                     << (student ? student->name : "Unknown")
                     << "\n";
            }

            cout << "Rating  : "
                 << fixed << setprecision(2)
                 << f.overallRating
                 << "/5\n";

            cout << "Comment : "
                 << f.comment
                 << "\n";
        }
    }

    if (!found)
    {
        cout << "\nNo matching feedback found.\n";
    }

    pauseScreen();
}


// ============================================================
//                    ADMIN LOGIN
// ============================================================

void adminLogin();


// ============================================================
//                    ADMIN DASHBOARD
// ============================================================

void adminDashboard()
{
    while (true)
    {
        clearScreen();

        printHeader("ADMIN DASHBOARD");

        cout << "1. Add Teacher\n";
        cout << "2. View Teachers\n";
        cout << "3. Remove Teacher\n";
        cout << "4. View All Feedback\n";
        cout << "5. Teacher-wise Feedback\n";
        cout << "6. Generate Feedback Report\n";
        cout << "7. Search Feedback\n";
        cout << "8. Sort Teachers by Rating\n";
        cout << "9. View Statistics\n";
        cout << "10. Logout\n";

        printLine('-');

        int choice =
            getInteger("Enter your choice: ", 1, 10);

        switch (choice)
        {
        case 1:
            addTeacher();
            break;

        case 2:
            viewTeachers();
            break;

        case 3:
            removeTeacher();
            break;

        case 4:
            viewAllFeedback();
            break;

        case 5:
            teacherWiseFeedback();
            break;

        case 6:
            generateReport();
            break;

        case 7:
            searchFeedback();
            break;

        case 8:
            sortTeachersByRating();
            break;

        case 9:
            // Statistics handled below
            {
                clearScreen();

                printHeader("SYSTEM STATISTICS");

                cout << "Total Students : "
                     << students.size()
                     << "\n";

                cout << "Total Teachers : "
                     << teachers.size()
                     << "\n";

                cout << "Total Feedback : "
                     << feedbacks.size()
                     << "\n";

                if (!feedbacks.empty())
                {
                    double totalRating = 0;

                    for (const Feedback& f : feedbacks)
                    {
                        totalRating += f.overallRating;
                    }

                    cout << "System Average : "
                         << fixed
                         << setprecision(2)
                         << totalRating / feedbacks.size()
                         << "/5\n";
                }
                else
                {
                    cout << "System Average : N/A\n";
                }

                pauseScreen();
            }
            break;

        case 10:
            return;
        }
    }
}


// ============================================================
//                    ADMIN LOGIN
// ============================================================

void adminLogin()
{
    clearScreen();

    printHeader("ADMIN LOGIN");

    const string ADMIN_USERNAME = "admin";
    const string ADMIN_PASSWORD = "admin123";

    string username =
        getNonEmptyInput("Username: ");

    string password =
        getPassword("Password: ");

    if (username == ADMIN_USERNAME &&
        password == ADMIN_PASSWORD)
    {
        cout << "\nAdmin login successful!\n";

        pauseScreen();

        adminDashboard();
    }
    else
    {
        cout << "\nInvalid admin username or password.\n";

        pauseScreen();
    }
}


// ============================================================
//                  ADD SAMPLE TEACHERS
// ============================================================

void addDefaultTeachers()
{
    if (!teachers.empty())
        return;

    Teacher t1;

    t1.id = "T001";
    t1.name = "Dr. Sharma";
    t1.subject = "Data Structures";
    t1.department = "CSE";

    Teacher t2;

    t2.id = "T002";
    t2.name = "Dr. Verma";
    t2.subject = "Operating Systems";
    t2.department = "CSE";

    Teacher t3;

    t3.id = "T003";
    t3.name = "Dr. Singh";
    t3.subject = "Database Management";
    t3.department = "CSE";

    teachers.push_back(t1);
    teachers.push_back(t2);
    teachers.push_back(t3);

    saveTeachers();
}


// ============================================================
//                    ABOUT PROJECT
// ============================================================

void aboutProject()
{
    clearScreen();

    printHeader("ABOUT EDufEED");

    cout << "Project Name : EduFeed\n";
    cout << "Project Type : Student Feedback Management System\n";
    cout << "Language     : C++\n";
    cout << "Storage      : Text File Database\n\n";

    cout << "Features:\n";

    cout << "1. Student Registration\n";
    cout << "2. Student Login\n";
    cout << "3. Admin Login\n";
    cout << "4. Teacher Management\n";
    cout << "5. Feedback Submission\n";
    cout << "6. Rating System\n";
    cout << "7. Anonymous Feedback\n";
    cout << "8. Duplicate Prevention\n";
    cout << "9. Feedback Reports\n";
    cout << "10. Search\n";
    cout << "11. Sorting\n";
    cout << "12. Statistics\n";
    cout << "13. Password Management\n";

    cout << "\nAdmin Login:\n";
    cout << "Username: admin\n";
    cout << "Password: admin123\n";

    pauseScreen();
}


// ============================================================
//                    MAIN MENU
// ============================================================

void mainMenu()
{
    while (true)
    {
        clearScreen();

        cout << "\n";

        printLine('=');

        cout << "             EDUFEED\n";
        cout << "       STUDENT FEEDBACK SYSTEM\n";

        printLine('=');

        cout << "\n";

        cout << "1. Student Login\n";
        cout << "2. Student Registration\n";
        cout << "3. Admin Login\n";
        cout << "4. About Project\n";
        cout << "5. Exit\n";

        printLine('-');

        int choice =
            getInteger("Enter your choice: ", 1, 5);

        switch (choice)
        {
        case 1:
            studentLogin();
            break;

        case 2:
            studentRegistration();
            break;

        case 3:
            adminLogin();
            break;

        case 4:
            aboutProject();
            break;

        case 5:
            clearScreen();

            printLine('=');

            cout << "Thank you for using EduFeed!\n";

            cout << "Goodbye!\n";

            printLine('=');

            return;
        }
    }
}


// ============================================================
//                         MAIN
// ============================================================

int main()
{
    // Load existing data from files
    loadAllData();

    // Add default teachers only if database is empty
    addDefaultTeachers();

    // Start application
    mainMenu();

    return 0;
}