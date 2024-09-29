
#include "CommandUndoRedo/ICommand.h"
#include "Service/MovieService.h"

class AddWatchlistCommand : public ICommand{
private:
    MovieService* movieServ;
    Movie mov;

public:
    AddWatchlistCommand(MovieService& mv, Movie m);
    void execute() override;
    void undo() override;
    void redo() override;

};


