#include "UpdateCommand.h"

UpdateCommand::UpdateCommand(MovieService &mv, Movie old_mov, Movie new_mov, int ID): movieServ{&mv}, oldMovie{old_mov}, newMovie{new_mov}, id{ID} {
}

void UpdateCommand::execute() {
    switch (this->id) {
        case 1:
            this->movieServ->updateTitle(this->oldMovie, this->newMovie.getTitle());
            break;
        case 2:
            this->movieServ->updateGenre(this->oldMovie, this->newMovie.getGenre());
            break;
        case 3:
            this->movieServ->updateTrailer(this->oldMovie, this->newMovie.getTrailer());
            break;
        case 4:
            this->movieServ->updateLikes(this->oldMovie, this->newMovie.getLikesNumber());
            break;
        case 5:
            this->movieServ->updateReleaseYear(this->oldMovie, this->newMovie.getReleaseYear());
            break;

    }
}

void UpdateCommand::undo() {
    switch (this->id) {
        case 1:
            this->movieServ->updateTitle(this->oldMovie, this->oldMovie.getTitle());
            break;
        case 2:
            this->movieServ->updateGenre(this->oldMovie, this->oldMovie.getGenre());
            break;
        case 3:
            this->movieServ->updateTrailer(this->oldMovie, this->oldMovie.getTrailer());
            break;
        case 4:
            this->movieServ->updateLikes(this->oldMovie, this->oldMovie.getLikesNumber());
            break;
        case 5:
            this->movieServ->updateReleaseYear(this->oldMovie, this->oldMovie.getReleaseYear());
            break;

    }

}

void UpdateCommand::redo() {
    switch (this->id) {
        case 1:
            this->movieServ->updateTitle(this->oldMovie, this->newMovie.getTitle());
            break;
        case 2:
            this->movieServ->updateGenre(this->oldMovie, this->newMovie.getGenre());
            break;
        case 3:
            this->movieServ->updateTrailer(this->oldMovie, this->newMovie.getTrailer());
            break;
        case 4:
            this->movieServ->updateLikes(this->oldMovie, this->newMovie.getLikesNumber());
            break;
        case 5:
            this->movieServ->updateReleaseYear(this->oldMovie, this->newMovie.getReleaseYear());
            break;

    }

}
