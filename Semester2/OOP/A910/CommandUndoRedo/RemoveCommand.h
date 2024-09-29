
#include "CommandUndoRedo/ICommand.h"
#include "Service/MovieService.h"

class RemoveCommand : public ICommand {
private:
    MovieService* movieServ;
    Movie mov;

public:
    RemoveCommand(MovieService& mv, Movie m);
    void execute() override;
    void undo() override;
    void redo() override;


};


