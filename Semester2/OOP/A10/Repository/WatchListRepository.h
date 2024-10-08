#include "Domain\Movie.h"
#include "Exception\RepositoryException.h"
#include "Repository\RepositoryValidator.h"
#include <vector>

class WatchList {
protected:
    std::vector<Movie> watchlist;
    std::string _file_name;

public:
    //default constructor
    WatchList(const std::string& file_name);

    const std::string& getFileName() const;
    void setFileName(const std::string& fileName);


    /**
     * Add a movie to the watchlist
     * Precondition: check if the movie has been added beforehand in the watchlist
     * @param mov
     */
    virtual void addMovie(Movie mov, int flag);

    /**
     * Remove a movie from the watch list
     * Precondition: check if the movie exists in the watchlist
     * @param mov
     */
    virtual void deleteMovie(Movie mov);

    /**
     * Look for a movie in the watchlist and return its position
     * @param mov
     * @return -1 if the movie does not exist in the watchlist, else its position
     */
    virtual int searchMovie(Movie mov);

    /**
     * Return the current size of the watchlist
     * @return
     */
    int list_size();

    Movie getMovie(int position);

    std::vector<Movie> getWatchList();

    virtual void clear_watchlist();

    virtual void readFromFile();
    virtual void writeToFile();

    virtual ~WatchList();

};
