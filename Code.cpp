#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

enum class Day {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday
};

class Teacher {
public:
    string name;
};

class Student {
public:
    string name;
    string section;
};

class Room {
public:
    string name;
};

class Course {
public:
    string name;
    string time;
    Day day;
    Teacher* teacher;
    Room* room;
    vector<Student*> students;
};

class Section {
public:
    string name;
    vector<Student*> students;
};

class Timetable {
public:
    vector<Course*> courses;
    vector<Section*> sections;

    void addCourse(Course* course) {
        courses.push_back(course);
    }

    void addSection(Section* section) {
        sections.push_back(section);
    }

    void printDayTimetable(Day day) {
        cout << "\nTimetable for " << getDayString(day) << ":\n";
        for (Course* course : courses) {
            if (course->day == day) {
                cout << "> Course: " << course->name << ", Teacher: " << course->teacher->name
                    << "\nTime: " << course->time << ", Room: " << course->room->name << endl;
            }
        }
        cout << endl;
    }

    void printTeacherTimetable(Teacher* teacher) {
        cout << "Teacher Timetable for " << teacher->name << ":\n";
        for (Course* course : courses) {
            if (course->teacher == teacher) {
                cout << "> Course: " << course->name << ", Day: " << getDayString(course->day)
                    << ", Time: " << course->time << ", Room: " << course->room->name << endl;
            }
        }

        cout << endl;
    }

    void printStudentTimetable(Student* student) {
        cout << "Timetable for Student " << student->name << "  in Section " << student->section << ":\n";
        for (Course* course : courses) {
            if (find(course->students.begin(), course->students.end(), student) != course->students.end()) {
                cout << "> Course: " << course->name << ", Teacher: " << course->teacher->name
                    << ", Day: " << getDayString(course->day) << ", Time: " << course->time
                    << ", Room: " << course->room->name << endl;
            }
        }
        cout << endl;
    }

    void Question1(Day day, string time) {
        cout << "Teachers teaching at " << time << " on " << getDayString(day) << ":\n";
        for (Course* course : courses) {
            if (course->day == day && course->time == time) {
                cout << "   Teacher: " << course->teacher->name << ", Course: " << course->name << endl;
            }
        }
        cout << endl;
    }

    void Question2(Day day) {
        cout << "For " << getDayString(day) << ":\n";
        printDayTimetable(day);
    }

    void printSectionTimetable(string sectionName) {
        for (Section* section : sections) {
            if (section->name == sectionName) {
                cout << "Timetable for Section " << sectionName << ":\n";
                for (Student* student : section->students) {
                    printStudentTimetable(student);
                }
                return;
            }
        }
        cout << "Section " << sectionName << " not found.\n";
    }

private:
    string getDayString(Day day) {
        switch (day) {
        case Day::Monday:
            return "Monday";
        case Day::Tuesday:
            return "Tuesday";
        case Day::Wednesday:
            return "Wednesday";
        case Day::Thursday:
            return "Thursday";
        case Day::Friday:
            return "Friday";
        default:
            return "Invalid Day";
        }
    }
};

void readTeachersFromFile(vector<Teacher*>& teachers, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    string name;
    while (getline(file, name)) {
        Teacher* teacher = new Teacher();
        teacher->name = name;
        teachers.push_back(teacher);
    }
    file.close();
    cout << "Teachers read from file: " << filename << endl;
}

void readStudentsFromFile(vector<Student*>& students, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    string line, name, section;
    while (getline(file, line)) {
        stringstream ss(line);
        if (getline(ss, name, ',') && getline(ss, section, ',')) {
            Student* student = new Student();
            student->name = name;
            student->section = section;
            students.push_back(student);
        }
    }
    file.close();
    cout << "Students read from file: " << filename << endl;
}

void readRoomsFromFile(vector<Room*>& rooms, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    string name;
    while (getline(file, name)) {
        Room* room = new Room();
        room->name = name;
        rooms.push_back(room);
    }
    file.close();
    cout << "Rooms read from file: " << filename << endl;
}

void readCoursesFromFile(vector<Course*>& courses, vector<Teacher*>& teachers, vector<Room*>& rooms, vector<Student*>& students, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    string line, name, teacherName, roomName, section;
    int day;
    string time;
    while (getline(file, line)) {
        stringstream ss(line);
        if (getline(ss, name, ',') && getline(ss, teacherName, ',') && getline(ss, time, ',') &&
            getline(ss, roomName, ',')) {
            Course* course = new Course();
            course->name = name;
            course->time = time;
            course->teacher = nullptr;
            for (Teacher* teacher : teachers) {
                if (teacher->name == teacherName) {
                    course->teacher = teacher;
                    break;
                }
            }
            if (!course->teacher) {
                cerr << "Error: Teacher '" << teacherName << "' not found for course '" << name << "'" << endl;
                delete course;
                continue;
            }
            course->room = nullptr;
            for (Room* room : rooms) {
                if (room->name == roomName) {
                    course->room = room;
                    break;
                }
            }
            if (!course->room) {
                cerr << "Error: Room '" << roomName << "' not found for course '" << name << "'" << endl;
                delete course;
                continue;
            }
            if (getline(ss, section, ',')) {
                for (Student* student : students) {
                    if (student->section == section) {
                        course->students.push_back(student);
                    }
                }
            }
            if (getline(ss, line)) {
                stringstream ss2(line);
                if (ss2 >> day) {
                    if (day < 0 || day > 4) {
                        cerr << "Error: Invalid day '" << day << "' for course '" << name << "'" << endl;
                        delete course;
                        continue;
                    }
                    course->day = static_cast<Day>(day);
                }
                else {
                    cerr << "Error: Invalid day format for course '" << name << "'" << endl;
                    delete course;
                    continue;
                }
            }
            else {
                cerr << "Error: Day not found for course '" << name << "'" << endl;
                delete course;
                continue;
            }
            courses.push_back(course);
        }
        else {
            cerr << "Error: Invalid format in line: " << line << endl;
        }
    }
    file.close();
    cout << "Courses read from file: " << filename << endl;
}

int main() {
    vector<Teacher*> teachers;
    vector<Student*> students;
    vector<Room*> rooms;
    vector<Course*> courses;
    vector<Section*> sections;
    Timetable timetable;

    readTeachersFromFile(teachers, "teachers.txt");
    readStudentsFromFile(students, "students.txt");
    readRoomsFromFile(rooms, "rooms.txt");
    readCoursesFromFile(courses, teachers, rooms, students, "courses.txt");

    for (Student* student : students) {
        Section* section = new Section();
        section->name = student->section;
        section->students.push_back(student);
        timetable.addSection(section);
    }

    for (Course* course : courses) {
        timetable.addCourse(course);
    }

    int choice;
    char option;
    do {
        system("cls");
        cout << "\t--------------------------------\n";
        cout << "\t- TIME-TABLE MANAGEMENT SYSTEM -\n";
        cout << "\t--------------------------------\n";
        cout << "\nSelect the option you want to print: \n";
        cout << "1. Print Teacher wise Timetable\n";
        cout << "2. Print Day wise Timetable\n";
        cout << "3. Print Student wise Timetable\n";
        cout << "4. Teacher teaching at 8:30 on Monday\n";
        cout << "5. Timetable for Tuesday\n";
        cout << "6. Print Section wise Timetable\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;
        system("cls");
        switch (choice) {
        case 1:
            for (Teacher* teacher : teachers) {
                timetable.printTeacherTimetable(teacher);
            }
            break;

        case 2:
            for (int i = 0; i < 5; ++i) {
                timetable.printDayTimetable(static_cast<Day>(i));
            }
            break;
        case 3:
            for (Student* student : students) {
                timetable.printStudentTimetable(student);
            }
            break;
        case 4:
            timetable.Question1(Day::Monday, "8:30");
            break;
        case 5:
            timetable.Question2(Day::Tuesday);
            break;
        case 6: {
            string sectionName;
            cout << "Enter section name (e.g., A or B): ";
            cin >> sectionName;
            timetable.printSectionTimetable(sectionName);
            break;
        }
        case 7:
            cout << "Exiting...";
            exit(0);
            break;

        default:
            cout << "Invalid choice\n";
        }
        cout << "\nDo you want to print the Main Menu again? (If yes press Y, if no press N): ";
        cin >> option;
    } while (option == 'Y' || option == 'y');
    cout << "Exiting...";

    // Freeing Memory
    for (Teacher* teacher : teachers) {
        delete teacher;
    }
    for (Student* student : students) {
        delete student;
    }
    for (Room* room : rooms) {
        delete room;
    }
    for (Course* course : courses) {
        delete course;
    }
    for (Section* section : sections) {
        delete section;
    }

    return 0;
}