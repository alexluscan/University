#include "RemoveWatchlistCommand.h"

RemoveWatchlistCommand::RemoveWatchlistCommand(Service &mv, Movie &m, bool fl): movieServ{&mv}, mov{m}, isRated{fl}{

}

void RemoveWatchlistCommand::execute() {
    this->movieServ->removeWatchedMovieFromWatchList(1, true);

}

void RemoveWatchlistCommand::undo() {
    this->movieServ->addMovieToWatchList(mov);
}

void RemoveWatchlistCommand::redo() {
    this->movieServ->removeWatchedMovieFromWatchList(1, true);
}
