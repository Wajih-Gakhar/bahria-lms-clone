#include "lms.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>

LMS::LMS() {
    // Load data from files
    loadAllData();

    // Add default admin if no admins exist
    if (admins.isEmpty()) {
        addAdmin("Admin", "admin", "admin123");
    }
}

// File Handling - Save Functions
void LMS::saveAllData() {
    saveStudents();
    saveTeachers();
    saveAdmins();
    saveCourses();
    saveSubmissions();
}

void LMS::saveStudents() {
    QFile file("students.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not open students.txt for writing";
        return;
    }

    QTextStream out(&file);
    for (const auto& s : students) {
        out << s.getName() << "|" << s.getID() << "|" << s.getPassword() << "|";

        // Save enrolled courses
        for (int i = 0; i < s.enrolledCourses.size(); i++) {
            out << s.enrolledCourses[i];
            if (i < s.enrolledCourses.size() - 1) {
                out << ",";
            }
        }
        out << "\n";
    }
    file.close();
}

void LMS::saveTeachers() {
    QFile file("teachers.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not open teachers.txt for writing";
        return;
    }

    QTextStream out(&file);
    for (const auto& t : teachers) {
        out << t.getName() << "|" << t.getID() << "|" << t.getPassword() << "|";

        // Save assigned courses
        for (int i = 0; i < t.assignedCourses.size(); i++) {
            out << t.assignedCourses[i];
            if (i < t.assignedCourses.size() - 1) {
                out << ",";
            }
        }
        out << "\n";
    }
    file.close();
}

void LMS::saveAdmins() {
    QFile file("admins.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not open admins.txt for writing";
        return;
    }

    QTextStream out(&file);
    for (const auto& a : admins) {
        out << a.getName() << "|" << a.getID() << "|" << a.getPassword() << "\n";
    }
    file.close();
}

void LMS::saveCourses() {
    QFile file("courses.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not open courses.txt for writing";
        return;
    }

    QTextStream out(&file);
    for (const auto& c : courses) {
        out << c.code << "|" << c.title << "|" << c.teacherID << "|";

        // Save enrolled student IDs
        for (int i = 0; i < c.enrolledStudentIDs.size(); i++) {
            out << c.enrolledStudentIDs[i];
            if (i < c.enrolledStudentIDs.size() - 1) {
                out << ",";
            }
        }
        out << "|";

        // Save assignments
        for (int i = 0; i < c.assignments.size(); i++) {
            out << c.assignments[i].assignmentName << "~"
                << c.assignments[i].courseCode << "~"
                << c.assignments[i].description << "~"
                << c.assignments[i].dueDate << "~"
                << c.assignments[i].totalMarks;
            if (i < c.assignments.size() - 1) {
                out << ",";
            }
        }
        out << "\n";
    }
    file.close();
}

void LMS::saveSubmissions() {
    QFile file("submissions.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not open submissions.txt for writing";
        return;
    }

    QTextStream out(&file);
    for (const auto& sub : submissions) {
        out << sub.studentID << "|"
            << sub.studentName << "|"
            << sub.courseCode << "|"
            << sub.assignmentName << "|"
            << sub.fileName << "|"
            << sub.submissionDate << "|"
            << sub.marks << "|"
            << sub.feedback << "|"
            << (sub.isGraded ? "1" : "0") << "\n";
    }
    file.close();
}

// File Handling - Load Functions
void LMS::loadAllData() {
    loadStudents();
    loadTeachers();
    loadAdmins();
    loadCourses();
    loadSubmissions();
}

void LMS::loadStudents() {
    QFile file("students.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Note: students.txt not found, starting with empty student list";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if (parts.size() >= 3) {
            Student s(parts[0], parts[1], parts[2]);

            // Load enrolled courses
            if (parts.size() >= 4 && !parts[3].isEmpty()) {
                QStringList courses = parts[3].split(",");
                for (const auto& course : courses) {
                    s.enrolledCourses.append(course);
                }
            }

            students.append(s);
        }
    }
    file.close();
}

void LMS::loadTeachers() {
    QFile file("teachers.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Note: teachers.txt not found, starting with empty teacher list";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if (parts.size() >= 3) {
            Teacher t(parts[0], parts[1], parts[2]);

            // Load assigned courses
            if (parts.size() >= 4 && !parts[3].isEmpty()) {
                QStringList courses = parts[3].split(",");
                for (const auto& course : courses) {
                    t.assignedCourses.append(course);
                }
            }

            teachers.append(t);
        }
    }
    file.close();
}

void LMS::loadAdmins() {
    QFile file("admins.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Note: admins.txt not found, will create default admin";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if (parts.size() >= 3) {
            Admin a(parts[0], parts[1], parts[2]);
            admins.append(a);
        }
    }
    file.close();
}

void LMS::loadCourses() {
    QFile file("courses.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Note: courses.txt not found, starting with empty course list";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if (parts.size() >= 3) {
            Course c(parts[0], parts[1], parts[2]);

            // Load enrolled student IDs
            if (parts.size() >= 4 && !parts[3].isEmpty()) {
                QStringList studentIDs = parts[3].split(",");
                for (const auto& id : studentIDs) {
                    c.enrolledStudentIDs.append(id);
                }
            }

            // Load assignments
            if (parts.size() >= 5 && !parts[4].isEmpty()) {
                QStringList assignmentList = parts[4].split(",");
                for (const auto& assignmentStr : assignmentList) {
                    QStringList assignmentParts = assignmentStr.split("~");
                    if (assignmentParts.size() >= 5) {
                        Assignment a;
                        a.assignmentName = assignmentParts[0];
                        a.courseCode = assignmentParts[1];
                        a.description = assignmentParts[2];
                        a.dueDate = assignmentParts[3];
                        a.totalMarks = assignmentParts[4].toInt();
                        c.assignments.append(a);
                    }
                }
            }

            courses.append(c);
        }
    }
    file.close();
}

void LMS::loadSubmissions() {
    QFile file("submissions.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Note: submissions.txt not found, starting with empty submission list";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if (parts.size() >= 9) {
            Submission sub;
            sub.studentID = parts[0];
            sub.studentName = parts[1];
            sub.courseCode = parts[2];
            sub.assignmentName = parts[3];
            sub.fileName = parts[4];
            sub.submissionDate = parts[5];
            sub.marks = parts[6].toInt();
            sub.feedback = parts[7];
            sub.isGraded = (parts[8] == "1");

            submissions.append(sub);
        }
    }
    file.close();
}

// User Management
bool LMS::addStudent(QString name, QString id, QString password) {
    // Check if ID already exists
    for (const auto& s : students) {
        if (s.getID() == id) return false;
    }
    students.append(Student(name, id, password));
    saveStudents();
    return true;
}

bool LMS::addTeacher(QString name, QString id, QString password) {
    for (const auto& t : teachers) {
        if (t.getID() == id) return false;
    }
    teachers.append(Teacher(name, id, password));
    saveTeachers();
    return true;
}

bool LMS::addAdmin(QString name, QString id, QString password) {
    for (const auto& a : admins) {
        if (a.getID() == id) return false;
    }
    admins.append(Admin(name, id, password));
    saveAdmins();
    return true;
}

// Authentication
int LMS::loginStudent(QString id, QString password) {
    for (int i = 0; i < students.size(); i++) {
        if (students[i].getID() == id && students[i].getPassword() == password)
            return i;
    }
    return -1;
}

int LMS::loginTeacher(QString id, QString password) {
    for (int i = 0; i < teachers.size(); i++) {
        if (teachers[i].getID() == id && teachers[i].getPassword() == password)
            return i;
    }
    return -1;
}

int LMS::loginAdmin(QString id, QString password) {
    for (int i = 0; i < admins.size(); i++) {
        if (admins[i].getID() == id && admins[i].getPassword() == password)
            return i;
    }
    return -1;
}

// Course Management
bool LMS::addCourse(QString code, QString title, QString teacherID) {
    // Check if course already exists
    for (const auto& c : courses) {
        if (c.code == code) return false;
    }
    courses.append(Course(code, title, teacherID));

    // Add course to teacher's assigned courses
    for (auto& t : teachers) {
        if (t.getID() == teacherID) {
            t.assignedCourses.append(code);
            break;
        }
    }
    saveCourses();
    saveTeachers();
    return true;
}

bool LMS::enrollStudent(QString studentID, QString courseCode) {
    // Find course
    Course* course = nullptr;
    for (auto& c : courses) {
        if (c.code == courseCode) {
            course = &c;
            break;
        }
    }
    if (!course) return false;

    // Check if already enrolled
    if (course->enrolledStudentIDs.contains(studentID)) return false;

    // Enroll student
    course->enrolledStudentIDs.append(studentID);

    // Add to student's enrolled courses
    for (auto& s : students) {
        if (s.getID() == studentID) {
            s.enrolledCourses.append(courseCode);
            break;
        }
    }
    saveCourses();
    saveStudents();
    return true;
}

QVector<Course> LMS::getAllCourses() {
    return courses;
}

QVector<Course> LMS::getStudentCourses(QString studentID) {
    QVector<Course> result;
    for (const auto& c : courses) {
        if (c.enrolledStudentIDs.contains(studentID)) {
            result.append(c);
        }
    }
    return result;
}

QVector<Course> LMS::getTeacherCourses(QString teacherID) {
    QVector<Course> result;
    for (const auto& c : courses) {
        if (c.teacherID == teacherID) {
            result.append(c);
        }
    }
    return result;
}

Course* LMS::getCourse(QString courseCode) {
    for (auto& c : courses) {
        if (c.code == courseCode) {
            return &c;
        }
    }
    return nullptr;
}

// Assignment Management
bool LMS::addAssignment(QString courseCode, QString assignmentName,
                        QString description, QString dueDate, int totalMarks) {
    Course* course = getCourse(courseCode);
    if (!course) return false;

    // Check if assignment already exists
    for (const auto& a : course->assignments) {
        if (a.assignmentName == assignmentName) return false;
    }

    Assignment assignment;
    assignment.assignmentName = assignmentName;
    assignment.courseCode = courseCode;
    assignment.description = description;
    assignment.dueDate = dueDate;
    assignment.totalMarks = totalMarks;

    course->assignments.append(assignment);
    saveCourses();
    return true;
}

QVector<Assignment> LMS::getCourseAssignments(QString courseCode) {
    Course* course = getCourse(courseCode);
    if (course) {
        return course->assignments;
    }
    return QVector<Assignment>();
}

// Submission Management
bool LMS::submitAssignment(QString studentID, QString courseCode,
                           QString assignmentName, QString fileName) {
    // Check if assignment exists
    Course* course = getCourse(courseCode);
    if (!course) return false;

    bool assignmentExists = false;
    for (const auto& a : course->assignments) {
        if (a.assignmentName == assignmentName) {
            assignmentExists = true;
            break;
        }
    }
    if (!assignmentExists) return false;

    // Check if already submitted (update instead)
    for (auto& sub : submissions) {
        if (sub.studentID == studentID &&
            sub.courseCode == courseCode &&
            sub.assignmentName == assignmentName) {
            sub.fileName = fileName;
            sub.submissionDate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
            saveSubmissions();
            return true;
        }
    }

    // Create new submission
    Submission submission;
    submission.studentID = studentID;
    Student* student = getStudent(studentID);
    if (student) {
        submission.studentName = student->getName();
    }
    submission.courseCode = courseCode;
    submission.assignmentName = assignmentName;
    submission.fileName = fileName;
    submission.submissionDate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    submission.marks = 0;
    submission.feedback = "";
    submission.isGraded = false;

    submissions.append(submission);
    saveSubmissions();
    return true;
}

bool LMS::gradeSubmission(QString studentID, QString courseCode,
                          QString assignmentName, int marks, QString feedback) {
    for (auto& sub : submissions) {
        if (sub.studentID == studentID &&
            sub.courseCode == courseCode &&
            sub.assignmentName == assignmentName) {
            sub.marks = marks;
            sub.feedback = feedback;
            sub.isGraded = true;
            saveSubmissions();
            return true;
        }
    }
    return false;
}

QVector<Submission> LMS::getStudentSubmissions(QString studentID) {
    QVector<Submission> result;
    for (const auto& sub : submissions) {
        if (sub.studentID == studentID) {
            result.append(sub);
        }
    }
    return result;
}

QVector<Submission> LMS::getCourseSubmissions(QString courseCode) {
    QVector<Submission> result;
    for (const auto& sub : submissions) {
        if (sub.courseCode == courseCode) {
            result.append(sub);
        }
    }
    return result;
}

QVector<Submission> LMS::getAssignmentSubmissions(QString courseCode, QString assignmentName) {
    QVector<Submission> result;
    for (const auto& sub : submissions) {
        if (sub.courseCode == courseCode && sub.assignmentName == assignmentName) {
            result.append(sub);
        }
    }
    return result;
}

// Data Retrieval
QVector<Student> LMS::getAllStudents() {
    return students;
}

QVector<Teacher> LMS::getAllTeachers() {
    return teachers;
}

Student* LMS::getStudent(QString id) {
    for (auto& s : students) {
        if (s.getID() == id) {
            return &s;
        }
    }
    return nullptr;
}

Teacher* LMS::getTeacher(QString id) {
    for (auto& t : teachers) {
        if (t.getID() == id) {
            return &t;
        }
    }
    return nullptr;
}

QVector<Student> LMS::getCourseStudents(QString courseCode) {
    QVector<Student> result;
    Course* course = getCourse(courseCode);
    if (course) {
        for (const auto& studentID : course->enrolledStudentIDs) {
            Student* student = getStudent(studentID);
            if (student) {
                result.append(*student);
            }
        }
    }
    return result;
}
