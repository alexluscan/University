#include <QApplication>
#include <QLabel>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include "Service\MovieService.h"
#include "Exception\RepositoryException.h"
#include "Repository\FileMovieRepo.h"
#include "Repository\CSVWatchListRepo.h"
#include "Repository\HTMLWatchListRepo.h"
#include "Repository\WatchListRepository.h"
#include "CommandUndoRedo/UndoRedo.h"
#include "GUI/WatchListTableModel.h"
#include <crtdbg.h>
#include <fstream>
#include <QtWidgets>
#include <QMessageBox>
#include "GUI/GUI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMessageBox::StandardButton csv = QMessageBox::question(nullptr, "Choose input", "CSV", QMessageBox::Yes | QMessageBox::No);


    QPalette palette;
    palette.setColor(QPalette::Background, Qt::lightGray);
    //csv.setPalette(palette);

    if(csv == QMessageBox::Yes){
        UndoRedo undoredo;
        FileMovieRepo movie_repo{"University//Semester2//OOP//A910//cmake-build-debug//movies.txt"};
        CSVWatchListRepo watch_list{"University//Semester2//OOP//A910//cmake-build-debug//watchlist.txt","University//Semester2//OOP//A910//cmake-build-debug//cmake-build-debug//watch.csv"};
        MovieService service{movie_repo, watch_list};
        WatchListTableModel tableView{watch_list};
        //tableView.show()
        GUI gui{service, undoredo, tableView};
        gui.show();
        a.exec();

    }
    else{
        UndoRedo undoredo;
        FileMovieRepo movie_repo{"University//Semester2//OOP//A910//cmake-build-debug//movies.txt"};
        HTMLWatchListRepo watch_list{"University//Semester2//OOP//A910//cmake-build-debug//watchlist.txt","University//Semester2//OOP//A910//cmake-build-debug//watch.html"};
        MovieService service{movie_repo, watch_list};
        WatchListTableModel tableView{watch_list};
        GUI gui{service, undoredo, tableView};
        gui.show();
        a.exec();
    }
    //
    return 0;

}
