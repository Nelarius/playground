#include "app/Command.h"

namespace pg {

Command::Command(std::function<void()> execute, std::function<void()> undo)
    : execute_(execute),
    undo_(undo)
{}

void Command::execute() const {
    execute_();
}

void Command::undo() const {
    undo_();
}

}