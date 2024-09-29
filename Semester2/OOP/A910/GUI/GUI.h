
#pragma once
#include <qwidget.h>
#include <qlistwidget.h>
#include "Service/MovieService.h"
#include <QPushButton>
#include <QTextBrowser>
#include <QLineEdit>

#include "CommandUndoRedo/UndoRedo.h"
#include "CommandUndoRedo/AddCommand.h"
#include "CommandUndoRedo/RemoveCommand.h"
#include "CommandUndoRedo/UpdateCommand.h"
#include "CommandUndoRedo/AddWatchlistCommand.h"
#include "CommandUndoRedo/RemoveWatchlistCommand.h"
#include "GUI/WatchListTableModel.h"
#include <QTableView>

class GUI : public QWidget  {

Q_OBJECT

private:
    MovieService& _service;
    Movie _working_mov;
    UndoRedo& undoredo;
    WatchListTableModel *tablemodel;


    //graphical elements
    QTableView movieTable;

    QPushButton *UserButton, *AdminButton;
    QPushButton *BarChart;
    QPushButton *UndoButton, *RedoButton;
    QPushButton *exit, *exitAdminMode, *exitUserMode;
    QPushButton *updateTitle, *updateGenre, *updateTrailer, *updateYear, *updateLikes;
    QPushButton *seeWatchList, *removeMovieUser, *seeDatabase, *seeCSVHTML;
    //QListWidget *movieListWidget;
    QPushButton *addMovie, *deleteMovie, *updateMovie,  *listMovie;
    QPushButton  *addButton;
    // QPushButton *updateTitle, *updateGenre, *updateTrailer, *updateLikes, *updateYear;
    QWidget *UserWidget, *AdministratorWidget;
    QWidget *updateWidget;

//
    // QWidget* MovieWidget;
    //  QVBoxLayout* MovieVL;
    //  QFormLayout* MovieLayout;
//
    //  QLineEdit* title, *genre, *trailer, *year, *likes;

public:
    GUI(MovieService& s, UndoRedo& und, WatchListTableModel& wl);

private:
    void initGUI();
    void connectGUI();
    void adminMenuGUI();
    void userMenuGUI();
    void listMovies();
    void showMovieFormLayout();
    static std::string toString(Movie& mov);

    void addMovieF();

    void addMovieAux();
    void deleteMovieAux();
    void updateMenu();

    void updateTitleAux();
    void updateGenreAux();
    void updateTrailerAux();
    void updateYearAux();
    void updateLikesAux();

    void seeWatchListView();
    void seeWatchListAux();
    void seeDatabaseAux();
    void removeMovieUserAux();
    void seeCSVHTMLAux();

    void viewBarChart();
    void undo();
    void redo();



signals:
    void exitAdmin();
    void addMovieSignal(Movie mov );
};


