/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** ErrorClass
*/

#ifndef ERRORCLASS_HPP_
#define ERRORCLASS_HPP_

#include <stdexcept>
#include <string>

class ErrorClass : public std::runtime_error {

    public:
        explicit ErrorClass(const std::string &message);

};

#endif /* !ERRORCLASS_HPP_ */
