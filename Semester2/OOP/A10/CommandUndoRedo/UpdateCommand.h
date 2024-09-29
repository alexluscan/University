
#include "CommandUndoRedo/ICommand.h"
#include "Service/MovieService.h"
#include <iostream>

class UpdateCommand : public ICommand {
private:
    MovieService* movieServ;
    Movie oldMovie, newMovie;
    int id;

public:
    UpdateCommand(MovieService& mv, Movie old_mov, Movie new_mov, int ID);
    void execute() override;
    void undo() override;
    void redo() override;


};
