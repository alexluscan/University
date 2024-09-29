//
// Created by amina on 5/29/2021.
//

#ifndef A10_913_DIACONU_ANA_1_UNDOREDOEXCEPTION_H
#define A10_913_DIACONU_ANA_1_UNDOREDOEXCEPTION_H
#include <exception>
#include <string>

class UndoRedoException : public std::exception {
private:
    std::string _msg;
public:
    UndoRedoException(const std::string& msg);
    const std::string& get_message() const;

};



#endif //A10_913_DIACONU_ANA_1_UNDOREDOEXCEPTION_H
