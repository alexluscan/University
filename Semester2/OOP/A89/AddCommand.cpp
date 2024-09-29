#include "AddCommand.h"

AddCommand::AddCommand(Service &mv, Movie m, int id): movieServ{&mv}, mov{m}, id{id} {
}

void AddCommand::execute() {
    this->movieServ->addMovie(Movie(this->mov.getTitle(), this->mov.getGenre(), this->mov.getTrailer(), this->mov.getYearOfRelease()>mov.getLikesCount()));
}

void AddCommand::undo() {
    this->movieServ->removeMovieByPosition(id);
}

void AddCommand::redo() {
    this->movieServ->addMovie(Movie(this->mov.getTitle(), this->mov.getGenre(), this->mov.getTrailer(), this->mov.getYearOfRelease()>mov.getLikesCount()));
}



