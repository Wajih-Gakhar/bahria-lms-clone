#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QMainWindow>
#include "lms.h"

namespace Ui {
class StudentDashboard;
}

class StudentDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentDashboard(LMS *lms, QString studentID, QWidget *parent = nullptr);
    ~StudentDashboard();

private slots:
    void on_viewCoursesButton_clicked();
    void on_viewAssignmentsButton_clicked();
    void on_submitButton_clicked();
    void on_viewSubmissionsButton_clicked();
    void on_browseButton_clicked();
    void on_logoutButton_clicked();
    void on_courseCombo_currentIndexChanged(int index);

private:
    Ui::StudentDashboard *ui;
    LMS *system;
    QString studentID;
    void refreshCourseList();
    void refreshAssignmentList();
};

#endif // STUDENTDASHBOARD_H
