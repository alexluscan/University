
#include "CommandUndoRedo/ICommand.h"
#include "Service/MovieService.h"

class RemoveWatchlistCommand : public ICommand{
private:
    MovieService* movieServ;
    Movie mov;
    bool isRated;

public:
    RemoveWatchlistCommand(MovieService& mv, Movie& m, bool fl);
    void execute() override;
    void undo() override;
    void redo() override;

};


