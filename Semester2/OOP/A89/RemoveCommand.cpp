
#include "RemoveCommand.h"

RemoveCommand::RemoveCommand(Service &mv, Movie m, int id): movieServ{&mv}, mov{m}, id{id} {
}

void RemoveCommand::execute() {
    this->movieServ->removeMovieByPosition(id);


}

void RemoveCommand::undo() {
    this->movieServ->addMovie(Movie(this->mov.getTitle(), this->mov.getGenre(), this->mov.getTrailer(), this->mov.getYearOfRelease()>mov.getLikesCount()));


}

void RemoveCommand::redo() {
    this->movieServ->removeMovieByPosition(id);

}
