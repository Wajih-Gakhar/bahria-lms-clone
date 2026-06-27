#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QMainWindow>
#include "lms.h"

namespace Ui {
class AdminDashboard;
}

class AdminDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminDashboard(LMS *lms, QWidget *parent = nullptr);
    ~AdminDashboard();

private slots:
    void on_addStudentButton_clicked();
    void on_addTeacherButton_clicked();
    void on_addCourseButton_clicked();
    void on_enrollStudentButton_clicked();
    void on_viewStudentsButton_clicked();
    void on_viewTeachersButton_clicked();
    void on_viewCoursesButton_clicked();
    void on_logoutButton_clicked();

private:
    Ui::AdminDashboard *ui;
    LMS *system;
    void refreshStudentList();
    void refreshTeacherList();
    void refreshCourseList();
};

#endif // ADMINDASHBOARD_H
