#include "Domain\Movie.h"
#include "Exception\RepositoryException.h"
#include "Repository\MovieRepository.h"
#include <vector>

class RepositoryValidator {
public:
    static void validate_add(std::vector<Movie> list, Movie mov);
    static void validate_remove(std::vector<Movie> list, Movie mov);
    static void validate_update(std::vector<Movie> list, Movie mov, Movie new_mov, int flag);
};


