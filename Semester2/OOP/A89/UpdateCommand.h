
#include "ICommand.h"
#include "Service.h"
#include <iostream>

class UpdateCommand : public ICommand {
private:
    Service* movieServ;
    Movie oldMovie, newMovie;
    int id;

public:
    UpdateCommand(Service& mv, Movie old_mov, Movie new_mov, int ID);
    void execute() override;
    void undo() override;
    void redo() override;


};
