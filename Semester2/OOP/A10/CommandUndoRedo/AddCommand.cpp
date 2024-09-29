#include "AddCommand.h"

AddCommand::AddCommand(MovieService &mv, Movie m): movieServ{&mv}, mov{m} {
}

void AddCommand::execute() {
    this->movieServ->addMovie(this->mov.getTitle(), this->mov.getGenre(), this->mov.getTrailer(), this->mov.getReleaseYear(), this->mov.getLikesNumber(), 2);

}

void AddCommand::undo() {
    this->movieServ->removeMovie(this->mov.getTitle(), this->mov.getGenre(), this->mov.getTrailer(), this->mov.getReleaseYear(), this->mov.getLikesNumber());

}

void AddCommand::redo() {
    this->movieServ->addMovie(this->mov.getTitle(), this->mov.getGenre(), this->mov.getTrailer(), this->mov.getReleaseYear(), this->mov.getLikesNumber(), 2);

}



