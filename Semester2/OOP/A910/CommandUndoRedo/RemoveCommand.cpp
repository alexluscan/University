
#include "RemoveCommand.h"

RemoveCommand::RemoveCommand(MovieService &mv, Movie m): movieServ{&mv}, mov{m} {
}

void RemoveCommand::execute() {
    this->movieServ->removeMovie(this->mov.getTitle(), this->mov.getGenre(), this->mov.getTrailer(), this->mov.getReleaseYear(), this->mov.getLikesNumber());


}

void RemoveCommand::undo() {
    this->movieServ->addMovie(this->mov.getTitle(), this->mov.getGenre(), this->mov.getTrailer(), this->mov.getReleaseYear(), this->mov.getLikesNumber(), 2);


}

void RemoveCommand::redo() {
    this->movieServ->removeMovie(this->mov.getTitle(), this->mov.getGenre(), this->mov.getTrailer(), this->mov.getReleaseYear(), this->mov.getLikesNumber());


}
