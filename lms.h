#ifndef LMS_H
#define LMS_H

#include <QString>
#include <QVector>
#include <QMap>

// Base Person Class
class Person {
protected:
    QString name;
    QString id;
    QString password;

public:
    Person() {}
    Person(QString n, QString i, QString p) : name(n), id(i), password(p) {}

    void setName(QString n) { name = n; }
    void setID(QString i) { id = i; }
    void setPassword(QString p) { password = p; }

    QString getName() const { return name; }
    QString getID() const { return id; }
    QString getPassword() const { return password; }
};

// Derived Classes
class Student : public Person {
public:
    Student() : Person() {}
    Student(QString n, QString i, QString p) : Person(n, i, p) {}
    QVector<QString> enrolledCourses;
};

class Teacher : public Person {
public:
    Teacher() : Person() {}
    Teacher(QString n, QString i, QString p) : Person(n, i, p) {}
    QVector<QString> assignedCourses;
};

class Admin : public Person {
public:
    Admin() : Person() {}
    Admin(QString n, QString i, QString p) : Person(n, i, p) {}
};

// Assignment Structure
struct Assignment {
    QString assignmentName;
    QString courseCode;
    QString description;
    QString dueDate;
    int totalMarks;
};

// Course Class
class Course {
public:
    QString code;
    QString title;
    QString teacherID;
    QVector<QString> enrolledStudentIDs;
    QVector<Assignment> assignments;

    Course() {}
    Course(QString c, QString t, QString tid) : code(c), title(t), teacherID(tid) {}
};

// Submission Class
class Submission {
public:
    QString studentID;
    QString studentName;
    QString courseCode;
    QString assignmentName;
    QString fileName;
    QString submissionDate;
    int marks;
    QString feedback;
    bool isGraded;

    Submission() : marks(0), isGraded(false) {}
};

// Main LMS Controller Class
class LMS {
private:
    QVector<Student> students;
    QVector<Teacher> teachers;
    QVector<Admin> admins;
    QVector<Course> courses;
    QVector<Submission> submissions;

public:
    LMS();

    // User Management
    bool addStudent(QString name, QString id, QString password);
    bool addTeacher(QString name, QString id, QString password);
    bool addAdmin(QString name, QString id, QString password);

    // Authentication
    int loginStudent(QString id, QString password);
    int loginTeacher(QString id, QString password);
    int loginAdmin(QString id, QString password);

    // Course Management
    bool addCourse(QString code, QString title, QString teacherID);
    bool enrollStudent(QString studentID, QString courseCode);
    QVector<Course> getAllCourses();
    QVector<Course> getStudentCourses(QString studentID);
    QVector<Course> getTeacherCourses(QString teacherID);
    Course* getCourse(QString courseCode);

    // Assignment Management
    bool addAssignment(QString courseCode, QString assignmentName,
                       QString description, QString dueDate, int totalMarks);
    QVector<Assignment> getCourseAssignments(QString courseCode);

    // Submission Management
    bool submitAssignment(QString studentID, QString courseCode,
                          QString assignmentName, QString fileName);
    bool gradeSubmission(QString studentID, QString courseCode,
                         QString assignmentName, int marks, QString feedback);
    QVector<Submission> getStudentSubmissions(QString studentID);
    QVector<Submission> getCourseSubmissions(QString courseCode);
    QVector<Submission> getAssignmentSubmissions(QString courseCode, QString assignmentName);

    // Data Retrieval
    QVector<Student> getAllStudents();
    QVector<Teacher> getAllTeachers();
    Student* getStudent(QString id);
    Teacher* getTeacher(QString id);
    QVector<Student> getCourseStudents(QString courseCode);

    // Statistics
    int getStudentCount() const { return students.size(); }
    int getTeacherCount() const { return teachers.size(); }
    int getCourseCount() const { return courses.size(); }
    int getSubmissionCount() const { return submissions.size(); }

    // File Handling Functions
    void saveAllData();
    void loadAllData();

private:
    void saveStudents();
    void saveTeachers();
    void saveAdmins();
    void saveCourses();
    void saveSubmissions();

    void loadStudents();
    void loadTeachers();
    void loadAdmins();
    void loadCourses();
    void loadSubmissions();
};

#endif // LMS_H
