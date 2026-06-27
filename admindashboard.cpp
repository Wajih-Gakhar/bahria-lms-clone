#include "admindashboard.h"
#include "ui_admindashboard.h"
#include <QMessageBox>
#include <QHeaderView>

AdminDashboard::AdminDashboard(LMS *lms, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminDashboard),
    system(lms)
{
    ui->setupUi(this);
    setWindowTitle("Admin Dashboard - LMS");

    // Configure table appearance
    ui->dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->dataTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->dataTable->verticalHeader()->setDefaultSectionSize(40); // Set row height to 40 pixels
    ui->dataTable->setAlternatingRowColors(true);

    // Populate combo boxes
    refreshStudentList();
    refreshTeacherList();
    refreshCourseList();

    // Update statistics
    ui->studentCountLabel->setText(QString::number(system->getStudentCount()));
    ui->teacherCountLabel->setText(QString::number(system->getTeacherCount()));
    ui->courseCountLabel->setText(QString::number(system->getCourseCount()));
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

void AdminDashboard::on_addStudentButton_clicked()
{
    QString name = ui->studentNameEdit->text().trimmed();
    QString id = ui->studentIdEdit->text().trimmed();
    QString password = ui->studentPasswordEdit->text();

    if (name.isEmpty() || id.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required");
        return;
    }

    if (system->addStudent(name, id, password)) {
        QMessageBox::information(this, "Success", "Student added successfully");
        ui->studentNameEdit->clear();
        ui->studentIdEdit->clear();
        ui->studentPasswordEdit->clear();
        refreshStudentList();
        ui->studentCountLabel->setText(QString::number(system->getStudentCount()));
    } else {
        QMessageBox::warning(this, "Error", "Student ID already exists");
    }
}

void AdminDashboard::on_addTeacherButton_clicked()
{
    QString name = ui->teacherNameEdit->text().trimmed();
    QString id = ui->teacherIdEdit->text().trimmed();
    QString password = ui->teacherPasswordEdit->text();

    if (name.isEmpty() || id.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required");
        return;
    }

    if (system->addTeacher(name, id, password)) {
        QMessageBox::information(this, "Success", "Teacher added successfully");
        ui->teacherNameEdit->clear();
        ui->teacherIdEdit->clear();
        ui->teacherPasswordEdit->clear();
        refreshTeacherList();
        ui->teacherCountLabel->setText(QString::number(system->getTeacherCount()));
    } else {
        QMessageBox::warning(this, "Error", "Teacher ID already exists");
    }
}

void AdminDashboard::on_addCourseButton_clicked()
{
    QString code = ui->courseCodeEdit->text().trimmed();
    QString title = ui->courseTitleEdit->text().trimmed();
    QString teacherID = ui->courseTeacherCombo->currentText().split(" - ").first();

    if (code.isEmpty() || title.isEmpty() || teacherID.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required");
        return;
    }

    if (system->addCourse(code, title, teacherID)) {
        QMessageBox::information(this, "Success", "Course added successfully");
        ui->courseCodeEdit->clear();
        ui->courseTitleEdit->clear();
        refreshCourseList();
        ui->courseCountLabel->setText(QString::number(system->getCourseCount()));
    } else {
        QMessageBox::warning(this, "Error", "Course code already exists");
    }
}

void AdminDashboard::on_enrollStudentButton_clicked()
{
    QString studentID = ui->enrollStudentCombo->currentText().split(" - ").first();
    QString courseCode = ui->enrollCourseCombo->currentText().split(" - ").first();

    if (studentID.isEmpty() || courseCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select student and course");
        return;
    }

    if (system->enrollStudent(studentID, courseCode)) {
        QMessageBox::information(this, "Success", "Student enrolled successfully");
    } else {
        QMessageBox::warning(this, "Error", "Enrollment failed. Student may already be enrolled.");
    }
}

void AdminDashboard::on_viewStudentsButton_clicked()
{
    ui->dataTable->clearContents();
    ui->dataTable->setRowCount(0);
    ui->dataTable->setColumnCount(3);
    ui->dataTable->setHorizontalHeaderLabels({"ID", "Name", "Enrolled Courses"});

    QVector<Student> students = system->getAllStudents();
    ui->dataTable->setRowCount(students.size());

    for (int i = 0; i < students.size(); i++) {
        ui->dataTable->setItem(i, 0, new QTableWidgetItem(students[i].getID()));
        ui->dataTable->setItem(i, 1, new QTableWidgetItem(students[i].getName()));
        ui->dataTable->setItem(i, 2, new QTableWidgetItem(QString::number(students[i].enrolledCourses.size())));
    }

    // Reset column stretch after populating
    ui->dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void AdminDashboard::on_viewTeachersButton_clicked()
{
    ui->dataTable->clearContents();
    ui->dataTable->setRowCount(0);
    ui->dataTable->setColumnCount(3);
    ui->dataTable->setHorizontalHeaderLabels({"ID", "Name", "Assigned Courses"});

    QVector<Teacher> teachers = system->getAllTeachers();
    ui->dataTable->setRowCount(teachers.size());

    for (int i = 0; i < teachers.size(); i++) {
        ui->dataTable->setItem(i, 0, new QTableWidgetItem(teachers[i].getID()));
        ui->dataTable->setItem(i, 1, new QTableWidgetItem(teachers[i].getName()));
        ui->dataTable->setItem(i, 2, new QTableWidgetItem(QString::number(teachers[i].assignedCourses.size())));
    }

    // Reset column stretch after populating
    ui->dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void AdminDashboard::on_viewCoursesButton_clicked()
{
    ui->dataTable->clearContents();
    ui->dataTable->setRowCount(0);
    ui->dataTable->setColumnCount(4);
    ui->dataTable->setHorizontalHeaderLabels({"Code", "Title", "Teacher ID", "Enrolled Students"});

    QVector<Course> courses = system->getAllCourses();
    ui->dataTable->setRowCount(courses.size());

    for (int i = 0; i < courses.size(); i++) {
        ui->dataTable->setItem(i, 0, new QTableWidgetItem(courses[i].code));
        ui->dataTable->setItem(i, 1, new QTableWidgetItem(courses[i].title));
        ui->dataTable->setItem(i, 2, new QTableWidgetItem(courses[i].teacherID));
        ui->dataTable->setItem(i, 3, new QTableWidgetItem(QString::number(courses[i].enrolledStudentIDs.size())));
    }

    // Reset column stretch after populating
    ui->dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void AdminDashboard::on_logoutButton_clicked()
{
    this->close();
    parentWidget()->show();
}

void AdminDashboard::refreshStudentList()
{
    ui->enrollStudentCombo->clear();
    QVector<Student> students = system->getAllStudents();
    for (const auto& s : students) {
        ui->enrollStudentCombo->addItem(s.getID() + " - " + s.getName());
    }
}

void AdminDashboard::refreshTeacherList()
{
    ui->courseTeacherCombo->clear();
    QVector<Teacher> teachers = system->getAllTeachers();
    for (const auto& t : teachers) {
        ui->courseTeacherCombo->addItem(t.getID() + " - " + t.getName());
    }
}

void AdminDashboard::refreshCourseList()
{
    ui->enrollCourseCombo->clear();
    QVector<Course> courses = system->getAllCourses();
    for (const auto& c : courses) {
        ui->enrollCourseCombo->addItem(c.code + " - " + c.title);
    }
}
