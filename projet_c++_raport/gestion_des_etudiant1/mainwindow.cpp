#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QListWidget>
#include <QVBoxLayout>
#include <QSqlRecord>
#include <QDate>
#include <QMessageBox>
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(QSqlDatabase::addDatabase("QSQLITE")),
    currentYear(QDate::currentDate().year())
{
    ui->setupUi(this);
    // Perform admin login
    if (!performAdminLogin()) {
        // Close the application if login fails
        QApplication::quit();
        return;}

    // Connect to SQLite database
 db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("C:/Users/Zakaria/Desktop/projetc++/projetc++.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Database connection failed. Please check the database.");
        QApplication::quit();

        return;
    } else {
        qDebug() << "Database connected successfully.";
        for (int i = currentYear - 5; i <= currentYear + 5; ++i) {
            ui->anneeScolaireComboBox->addItem(QString::number(i));
        }
         ui->anneeScolaireComboBox->setCurrentText(QString::number(currentYear));


        db.transaction();
        displayStudents();
        displayCourses();
        displayStudentCourses();
    }

}


MainWindow::~MainWindow()
{ if (!db.commit()) {
        qDebug() << "Unable to commit transaction.";
    }
    //db.close();
    delete ui;
}
bool MainWindow::performAdminLogin()
{
    bool ok;
    QString username = QInputDialog::getText(this, "Login", "Enter username:", QLineEdit::Normal, "", &ok);

    if (ok && !username.isEmpty()) {
        QString password = QInputDialog::getText(this, "Login", "Enter password:", QLineEdit::Password, "", &ok);

        if (ok && !password.isEmpty()) {
            // Check if the entered username and password are correct
            if (isValidAdminCredentials(username, password)) {
                ui->stackedWidget->setCurrentIndex(0);
                // Admin login successful
                return true;
            } else {
                // Invalid credentials, show an error message and retry
                QMessageBox::critical(this, "Login Failed", "Invalid username or password. Please try again.");
                return performAdminLogin(); // Recursive call to retry login
            }
        } else {
            // User canceled password input, close the application
            QApplication::quit();
            return false;
        }
    } else {
        // User canceled username input, close the application
        QApplication::quit();
        return false;
    }
}

bool MainWindow::isValidAdminCredentials(const QString &username, const QString &password)
{
    // Replace this function with your logic to validate admin credentials
    // For example, you might compare against a predefined admin username and password
    return (username == "admin" && password == "admin123");
}



void MainWindow::on_go_Home_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_go_Home_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_go_Home_3_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_go_Home_4_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_go_Etudiant_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_go_Cours_clicked()
{
     ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_go_Inscription_clicked()
{
     ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_go_Affichage_clicked()
{
     ui->stackedWidget->setCurrentIndex(4);
}
void MainWindow::on_go_next_clicked()
{
     ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_go_next_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_go_previous_clicked()
{
   ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_go_next_3_clicked()
{
     ui->stackedWidget->setCurrentIndex(4);
}
void MainWindow::on_go_previous_2_clicked()
{
      ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_go_previous_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}





void MainWindow::on_ajouter_Etudiant_clicked()
{  QString Nom = ui->nomEtudiantInput->text().trimmed();
    QString Prenom = ui->prenomEtudiantInput->text().trimmed();

    if (Nom.isEmpty() || Prenom.isEmpty()) {
        qDebug() << "Please fill in all fields.";
        QMessageBox::critical(this, "Error", "Please fill in all fields.");
        return;
    }


    QSqlQuery query;
    query.prepare("INSERT INTO etudiant (Nom, Prenom) VALUES (:name, :last_name)");
    query.bindValue(":name", Nom);
    query.bindValue(":last_name", Prenom);

    if (!query.exec()) {
        qDebug() << "Error adding student. Error details:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to add student. Check the details and try again.");
    } else {
        qDebug() << "Student added successfully.";
        QMessageBox::information(this, "Success", "Student added successfully.");
        displayStudents(); // Refresh the student list after adding a new student
    }

    // Add a transaction and commit

    emit dataChanged();
    ui->nomEtudiantInput->clear();
    ui->prenomEtudiantInput->clear();

    qDebug() << "Input fields reset.";
}


void MainWindow::on_ajouter_Cours_clicked()
{
    QString nomCours = ui->nomCoursInput->text().trimmed();
    QString seances = ui->seancesInput->text().trimmed();

    if (nomCours.isEmpty() || seances.isEmpty()) {
        qDebug() << "Please fill in all fields.";
        QMessageBox::critical(this, "Error", "Please fill in all fields.");
        return;
    }
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid() || !db.isOpen()) {
        qDebug() << "Database is not open or not valid!";
        db.open();
        return;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO cours (nom_cours, seances) VALUES (:nom_cours, :seances)");
    query.bindValue(":nom_cours", nomCours);
    query.bindValue(":seances", seances.toInt());

    if (!query.exec()) {
        qDebug() << "Error adding course. Error details:" << query.lastError().text();
         QMessageBox::critical(this, "Error", "Failed to add course. Check the details and try again.");
    } else {
        qDebug() << "Course added successfully.";
        QMessageBox::information(this, "Success", "Course added successfully.");
        displayCourses(); // Refresh the course list after adding a new course
    }

    // Add a transaction and commit

    ui->nomCoursInput->clear();
    ui->seancesInput->clear();

    qDebug() << "Input fields reset.";
}

void MainWindow::displayStudents() {
    QSqlQuery query("SELECT * FROM etudiant");

    // Check for errors
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Clear the existing items in the list widget
    ui->studentListWidget->clear();

    // Iterate over the query results and add each student to the list widget
    while (query.next()) {
        QString matricule = query.value("matricule").toString();
        QString nom = query.value("Nom").toString();
        QString prenom = query.value("Prenom").toString();

        // Create a custom list item that hides the matricule
        QListWidgetItem *studentItem = new QListWidgetItem();
        studentItem->setData(Qt::UserRole, matricule);  // Store matricule in UserRole
        studentItem->setText("Nom: " + nom + " | Prenom: " + prenom);

        ui->studentListWidget->addItem(studentItem);
    }
}



void MainWindow::displayCourses() {
    QSqlQuery query("SELECT * FROM cours");

    // Check for errors
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Clear the existing items in the list widget
    ui->courseListWidget->clear();

    // Iterate over the query results and add each course to the list widget
    while (query.next()) {
        QString code = query.value("code").toString();
        QString nomCours = query.value("nom_cours").toString();
        int seances = query.value("seances").toInt();

        // Create a custom list item that hides the code
        QListWidgetItem *courseItem = new QListWidgetItem();
        courseItem->setData(Qt::UserRole, code);  // Store code in UserRole
        courseItem->setText("Nom du cours: " + nomCours + " | Seances: " + QString::number(seances));

        ui->courseListWidget->addItem(courseItem);
    }
}


void MainWindow::on_insert_into_inscription_clicked()
{
    // Retrieve the existing database connection
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid() || !db.isOpen()) {
        qDebug() << "Database is not open or not valid!";
        // Handle the error as needed
        return;
    }

    // Commit any open transaction before starting a new one
    if (!db.commit()) {
        qDebug() << "Unable to commit transaction.";
        // Rollback the transaction if commit fails
        db.rollback();
        return;
    }

    // Begin a transaction
    if (!db.transaction()) {
        qDebug() << "Unable to start a transaction.";
        return;
    }

    QStringList selectedStudents;
    QStringList selectedCourses;

    QList<QListWidgetItem *> selectedStudentItems = ui->studentListWidget->selectedItems();
    for (const QListWidgetItem *item : selectedStudentItems) {
        selectedStudents.append(item->data(Qt::UserRole).toString()); // Use Qt::UserRole to store matricule
    }

    QList<QListWidgetItem *> selectedCourseItems = ui->courseListWidget->selectedItems();
    for (const QListWidgetItem *item : selectedCourseItems) {
        selectedCourses.append(item->data(Qt::UserRole).toString()); // Use Qt::UserRole to store course code
    }

    int anneeScolaire = ui->anneeScolaireComboBox->currentText().toInt();

    // Perform the insertion into the "inscription" table
    QSqlQuery query(db); // Use QSqlQuery with the transaction object

    for (const QString &studentMatricule : selectedStudents) {
        for (const QString &courseCode : selectedCourses) {
            // Insert into the "inscription" table
            query.prepare("INSERT INTO inscription (id_etudiant, id_cours, anneeScolaire) VALUES (:id_etudiant, :id_cours, :anneeScolaire)");

            query.bindValue(":id_etudiant", studentMatricule.toInt());
            query.bindValue(":id_cours", courseCode.toInt());
            query.bindValue(":anneeScolaire", anneeScolaire);

            if (!query.exec()) {
                qDebug() << "Error inserting data into inscription table:" << query.lastError().text();
                // Rollback the transaction in case of an error
                db.rollback();
                return;
            } else {
                qDebug() << "Data inserted into inscription table successfully.";
            }

            qDebug() << "Matricule:" << studentMatricule << "Code:" << courseCode << "Annee Scolaire:" << anneeScolaire;

        }
    }
    emit dataChanged();

    // Finish the query to make it inactive
    query.finish();

    // Commit the transaction
    if (!db.commit()) {
        qDebug() << "Unable to commit transaction.";
        // Rollback the transaction if commit fails
        db.rollback();
    }


    // Close the database connection
    db.close();
}


void MainWindow::displayStudentCourses() {
    if (!ui || !ui->studentCoursesTable) {
        qDebug() << "UI elements not initialized properly.";
        return;
    }

    // Clear the existing items in the table
    ui->studentCoursesTable->clearContents();

    // Set the headers for the table
    ui->studentCoursesTable->setRowCount(0);  // Clear all rows
    ui->studentCoursesTable->setColumnCount(3);
    ui->studentCoursesTable->setHorizontalHeaderLabels(QStringList() << "Student" << "Courses" << "School Year");
    connect(this, &MainWindow::dataChanged, this, &MainWindow::displayStudentCourses);
    // SQL query to retrieve student courses
    QSqlQuery query("SELECT e.Nom, e.Prenom, GROUP_CONCAT(c.nom_cours, ', ') AS courses, i.anneeScolaire "
                    "FROM etudiant e "
                    "JOIN inscription i ON e.matricule = i.id_etudiant "
                    "JOIN cours c ON i.id_cours = c.code "
                    "GROUP BY e.matricule, i.anneeScolaire");

    // Populate the table with the query results
    int row = 0;
    while (query.next()) {
        QString studentName = query.value("Nom").toString() + " " + query.value("Prenom").toString();
        QString courses = query.value("courses").toString();
        QString schoolYear = query.value("anneeScolaire").toString();

        // If the row is out of bounds, insert a new row
        if (row >= ui->studentCoursesTable->rowCount()) {
            ui->studentCoursesTable->insertRow(row);
        }

        // Set data in the table
        ui->studentCoursesTable->setItem(row, 0, new QTableWidgetItem(studentName));
        ui->studentCoursesTable->setItem(row, 1, new QTableWidgetItem(courses));
        ui->studentCoursesTable->setItem(row, 2, new QTableWidgetItem(schoolYear));

        row++;
    }
    // Automatically resize columns to fit content
    for (int i = 0; i < ui->studentCoursesTable->columnCount(); ++i) {
        ui->studentCoursesTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }
}





void MainWindow::on_update_etudiant_clicked() {

    // Get the selected student's matricule
    QList<QListWidgetItem *> selectedStudentItems = ui->studentListWidget->selectedItems();
    if (selectedStudentItems.isEmpty()) {
        qDebug() << "No student selected for update.";
        QMessageBox::warning(this, "Warning", "No student selected for update. Please select a student.");
        return;
    }

    QString matricule = selectedStudentItems.first()->data(Qt::UserRole).toString();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        db.open();
        return;
    }

    // Query the database to get the current information about the selected student
    QSqlQuery query;
    query.prepare("SELECT * FROM etudiant WHERE matricule = :matricule");
    query.bindValue(":matricule", matricule.toInt());

    if (query.exec() && query.next()) {
        // Retrieve information from the query result
        QString nom = query.value("Nom").toString();
        QString prenom = query.value("Prenom").toString();

        // Open the update page and set the initial values
        ui->stackedWidget->setCurrentIndex(5);  // Assuming index 5 corresponds to the update page
        ui->nomEtudiantInputUpdate->setText(nom);
        ui->prenomEtudiantInputUpdate->setText(prenom);

        // Connect the update button to the slot for handling the update action
        connect(ui->update_Button, &QPushButton::clicked, this, &MainWindow::on_update_Button_clicked);
    } else {
        qDebug() << "Error retrieving information about the selected student:" << query.lastError().text();
    }
}

void MainWindow::on_update_Button_clicked() {
    // Perform the update action using the entered values
    QString updatedNom = ui->nomEtudiantInputUpdate->text();
    QString updatedPrenom = ui->prenomEtudiantInputUpdate->text();

    // Retrieve the matricule from the selected student
    QList<QListWidgetItem *> selectedStudentItems = ui->studentListWidget->selectedItems();
    if (selectedStudentItems.isEmpty()) {
        qDebug() << " student selected  updated.";
        QMessageBox::information(this, "Success", "student selected is updated");
        return;
    }

    QString matricule = selectedStudentItems.first()->data(Qt::UserRole).toString();

    // Execute an SQL query to update the student information
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE etudiant SET Nom = :nom, Prenom = :prenom WHERE matricule = :matricule");
    updateQuery.bindValue(":nom", updatedNom);
    updateQuery.bindValue(":prenom", updatedPrenom);
    updateQuery.bindValue(":matricule", matricule.toInt());

    if (updateQuery.exec()) {
        qDebug() << "Student information updated successfully.";
    } else {
        qDebug() << "Error updating student information:" << updateQuery.lastError().text();
    }
     displayStudents();
     emit dataChanged();

    // Clear the input fields and switch back to the original page with the student list
    ui->nomEtudiantInputUpdate->clear();
    ui->prenomEtudiantInputUpdate->clear();
    ui->stackedWidget->setCurrentIndex(3);  // Assuming index 3 corresponds to the student list page
}
void MainWindow::on_update_cours_clicked() {
    // Get the selected course's code
    QList<QListWidgetItem *> selectedCourseItems = ui->courseListWidget->selectedItems();
    if (selectedCourseItems.isEmpty()) {
        qDebug() << "No course selected for update.";
          QMessageBox::warning(this, "Warning", "No course selected for update . Please select a course.");
        return;
    }

    QString courseCode = selectedCourseItems.first()->data(Qt::UserRole).toString();

    // Query the database to get the current information about the selected course
    QSqlQuery query;
    query.prepare("SELECT * FROM cours WHERE code = :code");
    query.bindValue(":code", courseCode.toInt());

    if (query.exec() && query.next()) {
        // Retrieve information from the query result
        QString nomCours = query.value("nom_cours").toString();
        int seances = query.value("seances").toInt();

        // Open the update page and set the initial values
        ui->stackedWidget->setCurrentIndex(6);  // Assuming index 6 corresponds to the course update page
        ui->nomCoursInputUpdate->setText(nomCours);
        ui->seancesInputUpdate->setText(QString::number(seances));

        // Connect the update button to the new slot for handling the update action
        connect(ui->update_cours, &QPushButton::clicked, this, &MainWindow::on_Update_button_2_clicked);
    } else {
        qDebug() << "Error retrieving information about the selected course:" << query.lastError().text();
    }
}

void MainWindow::on_Update_button_2_clicked() {
    // Perform the update action using the entered values
    QString updatedNomCours = ui->nomCoursInputUpdate->text();
    int updatedSeances = ui->seancesInputUpdate->text().toInt();

    // Retrieve the course code from the selected course
    QList<QListWidgetItem *> selectedCourseItems = ui->courseListWidget->selectedItems();
    if (selectedCourseItems.isEmpty()) {
        qDebug() << "course selected updated";
        QMessageBox::information(this, "Success", "course selected is updated");
        return;
    }

    QString courseCode = selectedCourseItems.first()->data(Qt::UserRole).toString();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        db.open();
        return;
    }

    // Execute an SQL query to update the course information
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE cours SET nom_cours = :nom_cours, seances = :seances WHERE code = :code");
    updateQuery.bindValue(":nom_cours", updatedNomCours);
    updateQuery.bindValue(":seances", updatedSeances);
    updateQuery.bindValue(":code", courseCode.toInt());

    if (updateQuery.exec()) {
        qDebug() << "Course information updated successfully.";
    } else {
        qDebug() << "Error updating course information:" << updateQuery.lastError().text();
    }
    displayCourses();
    emit dataChanged();

    // Clear the input fields and switch back to the original page with the course list
    ui->nomCoursInputUpdate->clear();
    ui->seancesInputUpdate->clear();
    ui->stackedWidget->setCurrentIndex(3);  // Assuming index 2 corresponds to the course list page
}

void MainWindow::on_delete_etudiant_clicked() {
    // Get the selected student's matricule
    QList<QListWidgetItem *> selectedStudentItems = ui->studentListWidget->selectedItems();
    if (selectedStudentItems.isEmpty()) {
        qDebug() << "No student selected for deletion.";
        QMessageBox::warning(this, "Warning", "No student selected for deletion. Please select a student.");
        return;
    }

    QString matricule = selectedStudentItems.first()->data(Qt::UserRole).toString();

    // Display a confirmation message box
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this student?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        // User chose not to delete, return without further action
        return;
    }
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        db.open();
        return;
    }

    // Execute an SQL query to delete the student
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM etudiant WHERE matricule = :matricule");
    deleteQuery.bindValue(":matricule", matricule.toInt());

    if (deleteQuery.exec()) {
        qDebug() << "Data deleted successfully.";
         QMessageBox::information(this, "Success", "Student deleted successfully.");
        if (!db.commit()) {
            qDebug() << "Unable to commit transaction.";
            // Handle the error as needed
        }
    } else {
        qDebug() << "Error deleting data:" << deleteQuery.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to delete student. Check the details and try again.");


    }
     displayStudents();
    emit dataChanged();
}

void MainWindow::on_delete_cours_clicked() {
    // Get the selected course's code
    QList<QListWidgetItem *> selectedCourseItems = ui->courseListWidget->selectedItems();
    if (selectedCourseItems.isEmpty()) {
        qDebug() << "No course selected for deletion.";
        QMessageBox::warning(this, "Warning", "No course selected for deletion. Please select a course.");
        return;
    }

    QString courseCode = selectedCourseItems.first()->data(Qt::UserRole).toString();

    // Display a confirmation message box
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this course?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        // User chose not to delete, return without further action
        return;
    }
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        db.open();
        return;
    }


    // Execute an SQL query to delete the course
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM cours WHERE code = :code");
    deleteQuery.bindValue(":code", courseCode.toInt());

    if (deleteQuery.exec()) {
        qDebug() << "Data deleted successfully.";
         QMessageBox::information(this, "Success", "course deleted successfully.");
        if (!db.commit()) {
            qDebug() << "Unable to commit transaction.";
            // Handle the error as needed
        }
    } else {
        qDebug() << "Error deleting data:" << deleteQuery.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to delete course. Check the details and try again.");

    }
    displayCourses();
    emit dataChanged();
}


