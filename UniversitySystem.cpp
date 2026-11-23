#include <iostream>
#include <string>
using namespace std;

struct Course
{
    string courseName;
    Course *next;
};

struct Student
{
    int rollNo;
    string name;
    int age;

    Course *courses;

    Student *next;
    Student *hashNext;
};

const int SIZE = 10;

Student *head = NULL;
Student *hashTable[SIZE] = {NULL};

// Hash Function
int hashFunction(int rollNo)
{
    return rollNo % SIZE;
}

// Search Student
Student* searchStudent(int rollNo)
{
    int index = hashFunction(rollNo);

    Student *temp = hashTable[index];

    while (temp != NULL)
    {
        if (temp->rollNo == rollNo)
            return temp;

        temp = temp->hashNext;
    }

    return NULL;
}

// Register Student
void registerStudent()
{
    Student *s = new Student;

    cout << "Enter Roll Number: ";
    cin >> s->rollNo;

    if (searchStudent(s->rollNo) != NULL)
    {
        cout << "Roll Number Already Exists!\n";
        delete s;
        return;
    }

    cout << "Enter Student Name: ";
    cin >> s->name;

    cout << "Enter Student Age: ";
    cin >> s->age;

    s->courses = NULL;

    // Insert into Linked List
    s->next = head;
    head = s;

    // Insert into Hash Table using Chaining
    int index = hashFunction(s->rollNo);

    s->hashNext = hashTable[index];
    hashTable[index] = s;

    cout << "Student Registered Successfully!\n";
}

// Update Student
void updateStudent()
{
    int roll;

    cout << "Enter Roll Number: ";
    cin >> roll;

    Student *s = searchStudent(roll);

    if (s == NULL)
    {
        cout << "Student Not Found!\n";
        return;
    }

    cout << "Enter New Name: ";
    cin >> s->name;

    cout << "Enter New Age: ";
    cin >> s->age;

    cout << "Student Updated Successfully!\n";
}

// Enroll Student in Course
void enrollCourse()
{
    int roll;

    cout << "Enter Roll Number: ";
    cin >> roll;

    Student *s = searchStudent(roll);

    if (s == NULL)
    {
        cout << "Student Not Found!\n";
        return;
    }

    Course *c = new Course;

    cout << "Enter Course Name: ";
    cin >> c->courseName;

    c->next = s->courses;
    s->courses = c;

    cout << "Course Enrolled Successfully!\n";
}

// Withdraw Student from Course
void withdrawCourse()
{
    int roll;

    string courseName;

    cout << "Enter Roll Number: ";
    cin >> roll;

    Student *s = searchStudent(roll);

    if (s == NULL)
    {
        cout << "Student Not Found!\n";
        return;
    }

    cout << "Enter Course Name: ";
    cin >> courseName;

    Course *temp = s->courses;
    Course *prev = NULL;

    while (temp != NULL &&
           temp->courseName != courseName)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        cout << "Course Not Found!\n";
        return;
    }

    if (prev == NULL)
    {
        s->courses = temp->next;
    }
    else
    {
        prev->next = temp->next;
    }

    delete temp;

    cout << "Course Withdrawn Successfully!\n";
}

// Delete Student
void deleteStudent()
{
    int roll;

    cout << "Enter Roll Number: ";
    cin >> roll;

    Student *s = searchStudent(roll);

    if (s == NULL)
    {
        cout << "Student Not Found!\n";
        return;
    }

    // Delete from Linked List
    if (head == s)
    {
        head = s->next;
    }
    else
    {
        Student *temp = head;

        while (temp->next != s)
        {
            temp = temp->next;
        }

        temp->next = s->next;
    }

    // Delete from Hash Table
    int index = hashFunction(roll);

    if (hashTable[index] == s)
    {
        hashTable[index] = s->hashNext;
    }
    else
    {
        Student *temp = hashTable[index];

        while (temp->hashNext != s)
        {
            temp = temp->hashNext;
        }

        temp->hashNext = s->hashNext;
    }

    // Delete student's courses
    Course *c = s->courses;

    while (c != NULL)
    {
        Course *temp = c;
        c = c->next;
        delete temp;
    }

    delete s;

    cout << "Student Deleted Successfully!\n";
}

// Display Students
void displayStudents()
{
    Student *temp = head;

    if (temp == NULL)
    {
        cout << "No Student Records Found!\n";
        return;
    }

    cout << "\n===== STUDENT RECORDS =====\n";

    while (temp != NULL)
    {
        cout << "\nRoll Number: " << temp->rollNo;
        cout << "\nName: " << temp->name;
        cout << "\nAge: " << temp->age;

        cout << "\nCourses: ";

        Course *c = temp->courses;

        if (c == NULL)
            cout << "No Courses Enrolled";

        while (c != NULL)
        {
            cout << c->courseName << " ";
            c = c->next;
        }

        cout << "\n--------------------------\n";

        temp = temp->next;
    }
}

// Main Function
int main()
{
    int choice;

    do
    {
        cout << "\n===== UNIVERSITY SYSTEM =====\n";

        cout << "1. Register Student\n";
        cout << "2. Update Student\n";
        cout << "3. Enroll Course\n";
        cout << "4. Withdraw Course\n";
        cout << "5. Delete Student\n";
        cout << "6. Display Students\n";
        cout << "7. Search Student\n";
        cout << "8. Exit\n";

        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                registerStudent();
                break;

            case 2:
                updateStudent();
                break;

            case 3:
                enrollCourse();
                break;

            case 4:
                withdrawCourse();
                break;

            case 5:
                deleteStudent();
                break;

            case 6:
                displayStudents();
                break;

            case 7:
            {
                int roll;

                cout << "Enter Roll Number: ";
                cin >> roll;

                Student *s = searchStudent(roll);

                if (s != NULL)
                {
                    cout << "Student Found!\n";
                    cout << "Name: " << s->name << endl;
                    cout << "Age: " << s->age << endl;
                }
                else
                {
                    cout << "Student Not Found!\n";
                }

                break;
            }

            case 8:
                cout << "Exiting Program...\n";
                break;

            default:
                cout << "Invalid Choice!\n";
        }

    } while (choice != 8);

    return 0;
}
