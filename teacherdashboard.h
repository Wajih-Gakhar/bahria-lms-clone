#ifndef TEACHERDASHBOARD_H
#define TEACHERDASHBOARD_H

#include <QMainWindow>
#include "lms.h"

namespace Ui {
class TeacherDashboard;
}

class TeacherDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeacherDashboard(LMS *lms, QString teacherID, QWidget *parent = nullptr);
    ~TeacherDashboard();

private slots:
    void on_addAssignmentButton_clicked();
    void on_viewSubmissionsButton_clicked();
    void on_gradeButton_clicked();
    void on_viewStudentsButton_clicked();
    void on_logoutButton_clicked();
    void on_courseCombo_currentIndexChanged(int index);
    void on_assignmentCombo_currentIndexChanged(int index);

private:
    Ui::TeacherDashboard *ui;
    LMS *system;
    QString teacherID;
    void refreshCourseList();
    void refreshAssignmentList();
    void loadSubmissions();
};

#endif // TEACHERDASHBOARD_H
