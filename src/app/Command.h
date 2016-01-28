#pragma once 

#include <functional>

namespace pg {

class Command {
public:
    Command(std::function<void()>, std::function<void()>);
    Command() = delete;
    ~Command() = default;

    void execute() const;
    void undo() const;

private:
    std::function< void() > execute_;
    std::function< void() > undo_;
};

}