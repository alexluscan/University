#include <vector>
#include "Exception\RepositoryException.h"
#include "Domain\Movie.h"
#include "Repository\RepositoryValidator.h"
#include "Domain\MovieValidator.h"

class MovieRepository {
protected:
    std::vector<Movie> movie_repository;
    //std::string file_name;
public:
    /**
     * Constructor of the repository
     * @param movie_repository
     */

    MovieRepository();

    std::vector<Movie> getMovieList();

    /**
     * Return the number of elements currently stored in the repo
     * @return
     */
    int repo_size();


    /**
     * Add a movie to the repository
     * @param mov
     */
    virtual void add_movie(const Movie& mov, int flag);

    /**
     * Remove a movie from the repository
     * @param mov
     */
    virtual Movie remove_movie(Movie mov);

    /**
     * Search a movie in the list and return its position
     * @param mov
     * @return position, else -1 if not in the list
     */
    virtual int search_movie(Movie mov);

    /**
     * Update the title of a movie
     * @param mov
     * @param title
     */
    virtual Movie update(Movie mov, Movie new_movie);


    /**
     * Get a movie instance from a given position
     * @peturn
     */
    virtual Movie& getMovie(int position);

    virtual void clear_list();

    void setMovieList(std::vector<Movie> movies);

    Movie getMovieByTitle(const std::string& title);



};


