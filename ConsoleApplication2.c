// including necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//defining macros
#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50
#define MAX_GRADES 5

// defining student struct
struct Student {
    int id;
    char name[MAX_NAME_LENGTH];
    int grades[MAX_GRADES];
    float averageGrade;
};

// defining functions
void addStudent(struct Student students[], int* count);
void displayRecords(struct Student students[], int count);
void updateRecord(struct Student students[], int count);
void deleteRecord(struct Student students[], int* count);
void calculateStatistics(struct Student students[], int count);
void saveToFile(struct Student students[], int count);
void readFromFile(struct Student students[], int* count);
void calculateAverage(struct Student* student);
int generateUniqueID();

//main function
int main() {
    //reading and counting students from file
    struct Student students[MAX_STUDENTS];
    int count = 0;

    readFromFile(students, &count);

    //displaying menu for the user
    int choice;
    do {
        printf("\nStudent Record Management System\n");
        printf("1. Add Student Record\n");
        printf("2. Display Records\n");
        printf("3. Update Record\n");
        printf("4. Delete Record\n");
        printf("5. Calculate Statistics\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf_s("%d", &choice, sizeof(choice));

        switch (choice) {
        case 1:
            addStudent(students, &count);
            break;
        case 2:
            displayRecords(students, count);
            break;
        case 3:
            updateRecord(students, count);
            break;
        case 4:
            deleteRecord(students, &count);
            break;
        case 5:
            calculateStatistics(students, count);
            break;
        case 6:
            printf("Exiting program.\n");
            saveToFile(students, count);
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 6.\n");
            break;
        }
    } while (choice != 6);

    return 0;
}

//defining student adding function
void addStudent(struct Student students[], int* count) {
    //displaying the warning for limit of max student number
    if (*count >= MAX_STUDENTS) {
        printf("Maximum number of students reached.\n");
        return;
    }

    //creating a new student
    struct Student newStudent;

    do {
        //getting name of student
        printf("Enter student name: ");
        scanf_s("%s", newStudent.name, (unsigned)_countof(newStudent.name));
        if (strlen(newStudent.name) == 0) {
            printf("Invalid name. Please enter a valid name.\n");
        }
    } while (strlen(newStudent.name) == 0);

    //getting grades of the student
    printf("Enter grades (5 grades separated by space): ");
    for (int i = 0; i < MAX_GRADES; i++) {
        scanf_s("%d", &newStudent.grades[i], sizeof(newStudent.grades[i]));
    }
    //calculating mean of grades
    calculateAverage(&newStudent);
    //assigning a random id to student through the function
    newStudent.id = generateUniqueID();

    students[*count] = newStudent;
    (*count)++;
    printf("Student added successfully with ID: %d\n", newStudent.id);
}

//defining display function
void displayRecords(struct Student students[], int count) {
    if (count == 0) {
        printf("No records found.\n");
        return;
    }

    //displaying names grades and means of students
    printf("Student Records:\n");
    printf("ID\tName\tGrades\tAverage Grade\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t", students[i].id, students[i].name);
        for (int j = 0; j < MAX_GRADES; j++) {
            printf("%d ", students[i].grades[j]);
        }
        printf("\t%.2f\n", students[i].averageGrade);
    }
}

//defining the update function
void updateRecord(struct Student students[], int count) {
    if (count == 0) {
        printf("No records found.\n");
        return;
    }

    int id;
    printf("Enter student ID to update: ");
    scanf_s("%d", &id, sizeof(id));

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            printf("Enter updated student name: ");
            scanf_s("%s", students[i].name, (unsigned)_countof(students[i].name));
            printf("Enter updated grades (5 grades separated by space): ");
            for (int j = 0; j < MAX_GRADES; j++) {
                scanf_s("%d", &students[i].grades[j], sizeof(students[i].grades[j]));
            }
            calculateAverage(&students[i]);
            printf("Record updated successfully.\n");
            return;
        }
    }

    printf("Student with ID %d not found.\n", id);
}

//defining the delete function
void deleteRecord(struct Student students[], int* count) {
    if (*count == 0) {
        printf("No records found.\n");
        return;
    }

    int id;
    printf("Enter student ID to delete: ");
    scanf_s("%d", &id, sizeof(id));

    for (int i = 0; i < *count; i++) {
        if (students[i].id == id) {
            for (int j = i; j < *count - 1; j++) {
                students[j] = students[j + 1];
            }
            (*count)--;
            printf("Record deleted successfully.\n");
            return;
        }
    }

    printf("Student with ID %d not found.\n", id);
}

//defining the function to calculate stats of students
void calculateStatistics(struct Student students[], int count) {
    if (count == 0) {
        printf("No records found.\n");
        return;
    }

    float sum = 0;
    float min = students[0].averageGrade;
    float max = students[0].averageGrade;
    for (int i = 0; i < count; i++) {
        sum += students[i].averageGrade;
        if (students[i].averageGrade < min) {
            min = students[i].averageGrade;
        }
        if (students[i].averageGrade > max) {
            max = students[i].averageGrade;
        }
    }
    float averageOfAverages = sum / count;
    printf("Average of Averages: %.2f\n", averageOfAverages);
    printf("Highest Average: %.2f\n", max);
    printf("Lowest Average: %.2f\n", min);
}

//defining the function to save changes to file
void saveToFile(struct Student students[], int count) {
    FILE* file;
    if (fopen_s(&file, "student_records.txt", "w") != 0) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s ", students[i].id, students[i].name);
        for (int j = 0; j < MAX_GRADES; j++) {
            fprintf(file, "%d ", students[i].grades[j]);
        }
        fprintf(file, "%.2f\n", students[i].averageGrade);
    }

    fclose(file);
}

//defining the function to read from the file
void readFromFile(struct Student students[], int* count) {
    FILE* file;
    if (fopen_s(&file, "student_records.txt", "r") != 0) {
        printf("No existing records found.\n");
        return;
    }

    while (fscanf_s(file, "%d", &students[*count].id) != EOF) {
        fscanf_s(file, "%s", students[*count].name, (unsigned)_countof(students[*count].name));
        for (int j = 0; j < MAX_GRADES; j++) {
            fscanf_s(file, "%d", &students[*count].grades[j]);
        }
        fscanf_s(file, "%f", &students[*count].averageGrade);
        (*count)++;
    }

    fclose(file);
}

//defining the function to calculate averages
void calculateAverage(struct Student* student) {
    int sum = 0;
    for (int i = 0; i < MAX_GRADES; i++) {
        sum += student->grades[i];
    }
    student->averageGrade = (float)sum / MAX_GRADES;
}

//defining a function to create random ids for students
int generateUniqueID() {
    srand((unsigned)time(NULL));
    return rand() % 1000 + 1;
}