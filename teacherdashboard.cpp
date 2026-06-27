#include "teacherdashboard.h"
#include "ui_teacherdashboard.h"
#include <QMessageBox>
#include <QHeaderView>

TeacherDashboard::TeacherDashboard(LMS *lms, QString tid, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeacherDashboard),
    system(lms),
    teacherID(tid)
{
    ui->setupUi(this);

    Teacher* teacher = system->getTeacher(teacherID);
    if (teacher) {
        setWindowTitle("Teacher Dashboard - " + teacher->getName());
        ui->welcomeLabel->setText("Welcome, " + teacher->getName());
    }

    // --- START: NEW TABLE CONFIGURATION ---

    // Configure students table
    ui->studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->studentsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->studentsTable->verticalHeader()->setDefaultSectionSize(40);
    ui->studentsTable->setAlternatingRowColors(true);

    // Configure submissions table
    ui->submissionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->submissionsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->submissionsTable->verticalHeader()->setDefaultSectionSize(40);
    ui->submissionsTable->setAlternatingRowColors(true);

    // --- END: NEW TABLE CONFIGURATION ---

    refreshCourseList();
}

TeacherDashboard::~TeacherDashboard()
{
    delete ui;
}

void TeacherDashboard::on_addAssignmentButton_clicked()
{
    QString courseCode = ui->assignCourseCombo->currentText().split(" - ").first();
    QString assignmentName = ui->assignmentNameEdit->text().trimmed();
    QString description = ui->assignmentDescEdit->toPlainText().trimmed();
    QString dueDate = ui->dueDateEdit->date().toString("yyyy-MM-dd");
    int totalMarks = ui->totalMarksSpinBox->value();

    if (courseCode.isEmpty() || assignmentName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Course and assignment name are required");
        return;
    }

    if (system->addAssignment(courseCode, assignmentName, description, dueDate, totalMarks)) {
        QMessageBox::information(this, "Success", "Assignment added successfully");
        ui->assignmentNameEdit->clear();
        ui->assignmentDescEdit->clear();
        ui->totalMarksSpinBox->setValue(100);
        refreshAssignmentList();
    } else {
        QMessageBox::warning(this, "Error", "Assignment already exists in this course");
    }
}

void TeacherDashboard::on_viewSubmissionsButton_clicked()
{
    loadSubmissions();
}

void TeacherDashboard::on_gradeButton_clicked()
{
    int row = ui->submissionsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a submission to grade");
        return;
    }

    QString studentID = ui->submissionsTable->item(row, 0)->text();
    QString courseCode = ui->submissionsTable->item(row, 2)->text();
    QString assignmentName = ui->submissionsTable->item(row, 3)->text();

    int marks = ui->marksSpinBox->value();
    QString feedback = ui->feedbackEdit->toPlainText();

    if (system->gradeSubmission(studentID, courseCode, assignmentName, marks, feedback)) {
        QMessageBox::information(this, "Success", "Submission graded successfully");
        loadSubmissions();
    } else {
        QMessageBox::warning(this, "Error", "Failed to grade submission");
    }
}

void TeacherDashboard::on_viewStudentsButton_clicked()
{
    QString courseCode = ui->courseCombo->currentText().split(" - ").first();
    if (courseCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a course");
        return;
    }

    ui->studentsTable->clearContents();
    ui->studentsTable->setRowCount(0);

    QVector<Student> students = system->getCourseStudents(courseCode);
    ui->studentsTable->setRowCount(students.size());

    for (int i = 0; i < students.size(); i++) {
        ui->studentsTable->setItem(i, 0, new QTableWidgetItem(students[i].getID()));
        ui->studentsTable->setItem(i, 1, new QTableWidgetItem(students[i].getName()));
    }
    for (int i = 0; i < students.size(); i++) {
        ui->studentsTable->setItem(i, 0, new QTableWidgetItem(students[i].getID()));
        ui->studentsTable->setItem(i, 1, new QTableWidgetItem(students[i].getName()));
    }

    // Reset column stretch after populating
    ui->studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TeacherDashboard::on_logoutButton_clicked()
{
    this->close();
    parentWidget()->show();
}

void TeacherDashboard::on_courseCombo_currentIndexChanged(int index)
{
    if (index >= 0) {
        refreshAssignmentList();
    }
}

void TeacherDashboard::on_assignmentCombo_currentIndexChanged(int index)
{
    if (index >= 0) {
        loadSubmissions();
    }
}

void TeacherDashboard::refreshCourseList()
{
    ui->courseCombo->clear();
    ui->assignCourseCombo->clear();

    QVector<Course> courses = system->getTeacherCourses(teacherID);
    for (const auto& c : courses) {
        QString item = c.code + " - " + c.title;
        ui->courseCombo->addItem(item);
        ui->assignCourseCombo->addItem(item);
    }

    if (courses.size() > 0) {
        refreshAssignmentList();
    }
}

void TeacherDashboard::refreshAssignmentList()
{
    ui->assignmentCombo->clear();

    QString courseCode = ui->courseCombo->currentText().split(" - ").first();
    if (courseCode.isEmpty()) return;

    QVector<Assignment> assignments = system->getCourseAssignments(courseCode);
    for (const auto& a : assignments) {
        ui->assignmentCombo->addItem(a.assignmentName);
    }
}

void TeacherDashboard::loadSubmissions()
{
    ui->submissionsTable->clearContents();
    ui->submissionsTable->setRowCount(0);

    QString courseCode = ui->courseCombo->currentText().split(" - ").first();
    QString assignmentName = ui->assignmentCombo->currentText();

    if (courseCode.isEmpty() || assignmentName.isEmpty()) return;

    QVector<Submission> submissions = system->getAssignmentSubmissions(courseCode, assignmentName);
    ui->submissionsTable->setRowCount(submissions.size());

    for (int i = 0; i < submissions.size(); i++) {
        ui->submissionsTable->setItem(i, 0, new QTableWidgetItem(submissions[i].studentID));
        ui->submissionsTable->setItem(i, 1, new QTableWidgetItem(submissions[i].studentName));
        ui->submissionsTable->setItem(i, 2, new QTableWidgetItem(submissions[i].courseCode));
        ui->submissionsTable->setItem(i, 3, new QTableWidgetItem(submissions[i].assignmentName));
        ui->submissionsTable->setItem(i, 4, new QTableWidgetItem(submissions[i].fileName));
        ui->submissionsTable->setItem(i, 5, new QTableWidgetItem(submissions[i].submissionDate));
        ui->submissionsTable->setItem(i, 6, new QTableWidgetItem(
                                                submissions[i].isGraded ? QString::number(submissions[i].marks) : "Not Graded"
                                                ));
        ui->submissionsTable->setItem(i, 7, new QTableWidgetItem(submissions[i].feedback));
    }
    ui->submissionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
