#include "UpdateCommand.h"

UpdateCommand::UpdateCommand(Service &mv, Movie old_mov, Movie new_mov, int ID): movieServ{&mv}, oldMovie{old_mov}, newMovie{new_mov}, id{ID} {
}

void UpdateCommand::execute() {
    this->movieServ->updateMovie(id, this->oldMovie);
}

void UpdateCommand::undo() {
    this->movieServ->updateMovie(id, this->oldMovie);
}

void UpdateCommand::redo() {
    this->movieServ->updateMovie(id, this->newMovie);

}
