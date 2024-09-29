#include "RemoveWatchlistCommand.h"

RemoveWatchlistCommand::RemoveWatchlistCommand(MovieService &mv, Movie &m, bool fl): movieServ{&mv}, mov{m}, isRated{fl}{

}

void RemoveWatchlistCommand::execute() {
    this->movieServ->removefromWatchList(this->mov);

}

void RemoveWatchlistCommand::undo() {
    this->movieServ->addtoWatchList(this->mov,0);
    if(isRated)
        this->movieServ->unrate_movie(this->mov);
}

void RemoveWatchlistCommand::redo() {
    this->movieServ->removefromWatchList(this->mov);
    if(isRated)
        this->movieServ->rate_movie(this->mov);

}
