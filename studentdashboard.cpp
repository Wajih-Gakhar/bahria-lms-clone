#include "studentdashboard.h"
#include "ui_studentdashboard.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>

StudentDashboard::StudentDashboard(LMS *lms, QString sid, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudentDashboard),
    system(lms),
    studentID(sid)
{
    ui->setupUi(this);

    Student* student = system->getStudent(studentID);
    if (student) {
        setWindowTitle("Student Dashboard - " + student->getName());
        ui->welcomeLabel->setText("Welcome, " + student->getName());
    }

    // Configure courses table
    ui->coursesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->coursesTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->coursesTable->verticalHeader()->setDefaultSectionSize(40);
    ui->coursesTable->setAlternatingRowColors(true);

    // Configure assignments table
    ui->assignmentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->assignmentsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->assignmentsTable->verticalHeader()->setDefaultSectionSize(40);
    ui->assignmentsTable->setAlternatingRowColors(true);

    // Configure submissions table
    ui->submissionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->submissionsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->submissionsTable->verticalHeader()->setDefaultSectionSize(40);
    ui->submissionsTable->setAlternatingRowColors(true);

    refreshCourseList();
}

StudentDashboard::~StudentDashboard()
{
    delete ui;
}

void StudentDashboard::on_viewCoursesButton_clicked()
{
    ui->coursesTable->clearContents();
    ui->coursesTable->setRowCount(0);

    QVector<Course> courses = system->getStudentCourses(studentID);
    ui->coursesTable->setRowCount(courses.size());

    for (int i = 0; i < courses.size(); i++) {
        Teacher* teacher = system->getTeacher(courses[i].teacherID);
        QString teacherName = teacher ? teacher->getName() : "Unknown";

        ui->coursesTable->setItem(i, 0, new QTableWidgetItem(courses[i].code));
        ui->coursesTable->setItem(i, 1, new QTableWidgetItem(courses[i].title));
        ui->coursesTable->setItem(i, 2, new QTableWidgetItem(teacherName));
        ui->coursesTable->setItem(i, 3, new QTableWidgetItem(QString::number(courses[i].assignments.size())));
    }

    // Reset column stretch after populating
    ui->coursesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void StudentDashboard::on_viewAssignmentsButton_clicked()
{
    QString courseCode = ui->courseCombo->currentText().split(" - ").first();
    if (courseCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a course");
        return;
    }

    ui->assignmentsTable->clearContents();
    ui->assignmentsTable->setRowCount(0);

    QVector<Assignment> assignments = system->getCourseAssignments(courseCode);
    ui->assignmentsTable->setRowCount(assignments.size());

    for (int i = 0; i < assignments.size(); i++) {
        ui->assignmentsTable->setItem(i, 0, new QTableWidgetItem(assignments[i].assignmentName));
        ui->assignmentsTable->setItem(i, 1, new QTableWidgetItem(assignments[i].description));
        ui->assignmentsTable->setItem(i, 2, new QTableWidgetItem(assignments[i].dueDate));
        ui->assignmentsTable->setItem(i, 3, new QTableWidgetItem(QString::number(assignments[i].totalMarks)));
    }

    // Reset column stretch after populating
    ui->assignmentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void StudentDashboard::on_submitButton_clicked()
{
    QString courseCode = ui->submitCourseCombo->currentText().split(" - ").first();
    QString assignmentName = ui->assignmentCombo->currentText();
    QString fileName = ui->filePathEdit->text().trimmed();

    if (courseCode.isEmpty() || assignmentName.isEmpty() || fileName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields and select a file");
        return;
    }

    if (system->submitAssignment(studentID, courseCode, assignmentName, fileName)) {
        QMessageBox::information(this, "Success", "Assignment submitted successfully");
        ui->filePathEdit->clear();
    } else {
        QMessageBox::warning(this, "Error", "Failed to submit assignment");
    }
}

void StudentDashboard::on_viewSubmissionsButton_clicked()
{
    ui->submissionsTable->clearContents();
    ui->submissionsTable->setRowCount(0);

    QVector<Submission> submissions = system->getStudentSubmissions(studentID);
    ui->submissionsTable->setRowCount(submissions.size());

    for (int i = 0; i < submissions.size(); i++) {
        ui->submissionsTable->setItem(i, 0, new QTableWidgetItem(submissions[i].courseCode));
        ui->submissionsTable->setItem(i, 1, new QTableWidgetItem(submissions[i].assignmentName));
        ui->submissionsTable->setItem(i, 2, new QTableWidgetItem(submissions[i].fileName));
        ui->submissionsTable->setItem(i, 3, new QTableWidgetItem(submissions[i].submissionDate));
        ui->submissionsTable->setItem(i, 4, new QTableWidgetItem(
                                                submissions[i].isGraded ? QString::number(submissions[i].marks) : "Pending"
                                                ));
        ui->submissionsTable->setItem(i, 5, new QTableWidgetItem(submissions[i].feedback));
    }

    // Reset column stretch after populating
    ui->submissionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void StudentDashboard::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Select Assignment File",
                                                    "",
                                                    "All Files (*.*);;PDF Files (*.pdf);;Documents (*.doc *.docx)");

    if (!fileName.isEmpty()) {
        ui->filePathEdit->setText(fileName);
    }
}

void StudentDashboard::on_logoutButton_clicked()
{
    this->close();
    parentWidget()->show();
}

void StudentDashboard::on_courseCombo_currentIndexChanged(int index)
{
    if (index >= 0) {
        refreshAssignmentList();
    }
}

void StudentDashboard::refreshCourseList()
{
    ui->courseCombo->clear();
    ui->submitCourseCombo->clear();

    QVector<Course> courses = system->getStudentCourses(studentID);
    for (const auto& c : courses) {
        QString item = c.code + " - " + c.title;
        ui->courseCombo->addItem(item);
        ui->submitCourseCombo->addItem(item);
    }

    if (courses.size() > 0) {
        refreshAssignmentList();
    }
}

void StudentDashboard::refreshAssignmentList()
{
    ui->assignmentCombo->clear();

    QString courseCode = ui->submitCourseCombo->currentText().split(" - ").first();
    if (courseCode.isEmpty()) return;

    QVector<Assignment> assignments = system->getCourseAssignments(courseCode);
    for (const auto& a : assignments) {
        ui->assignmentCombo->addItem(a.assignmentName);
    }
}
