#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "admindashboard.h"
#include "teacherdashboard.h"
#include "studentdashboard.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    system = new LMS();

    // Add some sample data for testing
    system->addTeacher("Mam Saima Javed", "T001", "teacher123");
    system->addTeacher("Prof. Abdul Raheem Aleem", "T002", "teacher123");
    system->addStudent("Abdullah Khan", "01-134251-007", "student123");
    system->addStudent("Khaleel Ahmed", "01-134251-030", "student456");

    setWindowTitle("LMS - Bahria University");
    setFixedSize(500, 400);
}

MainWindow::~MainWindow() {
    delete ui;
    delete system;
}

void MainWindow::on_loginButton_clicked() {
    QString id = ui->idLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();
    QString role = ui->roleComboBox->currentText();

    if (id.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter ID and Password");
        return;
    }

    if (role == "Admin") {
        int index = system->loginAdmin(id, password);
        if (index != -1) {
            AdminDashboard *adminDash = new AdminDashboard(system, this);
            adminDash->show();
            this->hide();
        } else {
            QMessageBox::warning(this, "Login Failed", "Invalid Admin credentials");
        }
    }
    else if (role == "Teacher") {
        int index = system->loginTeacher(id, password);
        if (index != -1) {
            TeacherDashboard *teacherDash = new TeacherDashboard(system, id, this);
            teacherDash->show();
            this->hide();
        } else {
            QMessageBox::warning(this, "Login Failed", "Invalid Teacher credentials");
        }
    }
    else if (role == "Student") {
        int index = system->loginStudent(id, password);
        if (index != -1) {
            StudentDashboard *studentDash = new StudentDashboard(system, id, this);
            studentDash->show();
            this->hide();
        } else {
            QMessageBox::warning(this, "Login Failed", "Invalid Student credentials");
        }
    }
}
