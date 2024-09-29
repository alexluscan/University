
#include "AddWatchlistCommand.h"

AddWatchlistCommand::AddWatchlistCommand(MovieService &mv, Movie m): movieServ{&mv}, mov{m} {
}

void AddWatchlistCommand::execute() {
    this->movieServ->addtoWatchList(this->mov, 0);

}

void AddWatchlistCommand::undo() {
    this->movieServ->removefromWatchList(this->mov);

}

void AddWatchlistCommand::redo() {
    this->movieServ->addtoWatchList(this->mov, 0);

}
