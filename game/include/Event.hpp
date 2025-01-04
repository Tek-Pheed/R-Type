/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** Event
*/

#ifndef EVENTCLASS_HPP_
#define EVENTCLASS_HPP_

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <vector>

class Event {
    public:
        Event() = default;

        void mouseClicked(std::vector<ecs::Entity> &entities);

};

#endif /* !EVENTCLASS_HPP_ */
