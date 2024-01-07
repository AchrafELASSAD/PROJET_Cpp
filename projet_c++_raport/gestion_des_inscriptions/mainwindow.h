#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:



    void on_go_Etudiant_clicked();

    void on_go_Cours_clicked();

    void on_go_Inscription_clicked();

    void on_go_Affichage_clicked();

    void on_go_Home_clicked();

    void on_go_Home_2_clicked();

    void on_go_Home_3_clicked();

    void on_go_Home_4_clicked();

    void on_ajouter_Etudiant_clicked();


    void on_go_next_clicked();

    void on_go_next_2_clicked();

    void on_go_next_3_clicked();

    void on_go_previous_clicked();

    void on_go_previous_2_clicked();

    void on_go_previous_3_clicked();



    void on_ajouter_Cours_clicked();

    void on_insert_into_inscription_clicked();



    void on_update_etudiant_clicked();
    void on_update_cours_clicked();



    void on_update_Button_clicked();

    void on_Update_button_2_clicked();

    void on_delete_etudiant_clicked();

    void on_delete_cours_clicked();

signals:
    void dataChanged();

private:
Ui::MainWindow *ui;
QSqlDatabase db;
int currentYear;

void displayStudents();
void displayCourses();
void displayStudentCourses();
bool performAdminLogin();
bool isValidAdminCredentials(const QString &username, const QString &password);

};

#endif // MAINWINDOW_H

