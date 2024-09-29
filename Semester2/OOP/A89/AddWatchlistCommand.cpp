
#include "AddWatchlistCommand.h"

AddWatchlistCommand::AddWatchlistCommand(Service &mv, Movie m): movieServ{&mv}, mov{m} {
}

void AddWatchlistCommand::execute() {
    this->movieServ->addMovieToWatchList(mov);

}

void AddWatchlistCommand::undo() {
    this->movieServ->removeWatchedMovieFromWatchList(1, true);

}

void AddWatchlistCommand::redo() {
    this->movieServ->addMovieToWatchList(this->mov);

}
