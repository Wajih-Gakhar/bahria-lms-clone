# 🎓 Bahria University LMS Clone

A complete Learning Management System clone of Bahria University's LMS, built in **C++** with **Qt**, featuring role-based access for Admin, Teacher, and Student.

---

## 🎥 Video

> https://github.com/user-attachments/assets/f9c81c69-b1c5-413b-8ccc-a85b16bf453e

---

## 🚀 Features

### 👥 Role-Based Access Control
- **Admin** — Full system control, user management
- **Teacher** — Course management, grade entry, student tracking
- **Student** — Course enrollment, grade viewing, multi-service access

### 📚 Course Management
- Create, update, and delete courses
- Assign teachers to courses

### 🎓 Student Enrollment
- Enroll students into courses
- Track enrollment history

### 📊 Grade Tracking
- Teachers can enter and update grades
- Students can view their grades per course

### 💾 Persistent File-Based Storage
- All data saved across sessions via file handling

---

## 🛠️ Tech Stack

| Technology | Purpose |
|---|---|
| C++ | Core programming language |
| OOP | System architecture & design |
| Qt Framework | GUI development |
| File Handling | Data persistence |

---

## 📂 Project Structure

```text
LMS/
├── CMakeLists.txt
├── Forms/
│   ├── admindashboard.ui
│   ├── mainwindow.ui
│   ├── studentdashboard.ui
│   └── teacherdashboard.ui
├── Header Files/
│   ├── admindashboard.h
│   ├── lms.h
│   ├── mainwindow.h
│   ├── studentdashboard.h
│   └── teacherdashboard.h
├── Resources/
│   ├── resources.qrc
│   └── fonts/
│       └── Poppins-*.ttf (All font variations)
├── Source Files/
│   ├── admindashboard.cpp
│   ├── lms.cpp
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── studentdashboard.cpp
│   └── teacherdashboard.cpp
└── style.qss

```

---

## ⚙️ How to Run

### Prerequisites
- Qt Creator (6.x or later)
- C++ compiler (MinGW 64-bit)

### Steps
1. Clone the repository
```bash
   git clone https://github.com/Wajih-Gakhar/bahria-lms-clone.git
```
2. Open `CMakeLists.txt` in **Qt Creator**
3. Select the **Desktop Qt 6.9.0 MinGW 64-bit** kit
4. Build & Run ▶️

---

## 👨‍💻 Author

**Muhammad Wajih Ul Hassan**  
[![LinkedIn](https://img.shields.io/badge/LinkedIn-0A66C2?style=flat&logo=linkedin&logoColor=white)](https://linkedin.com/in/wajih2206)
[![GitHub](https://img.shields.io/badge/GitHub-181717?style=flat&logo=github&logoColor=white)](https://github.com/Wajih-Gakhar)
