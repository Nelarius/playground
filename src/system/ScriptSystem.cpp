#include "system/ScriptSystem.h"
#include "app/KeyboardManager.h"
#include "app/MouseEvents.h"
#include "app/Context.h"
#include "utils/Assert.h"
#include "utils/Log.h"

namespace pg {
namespace system {

void ScriptSystem::IdSet::add(std::size_t id) {
    auto it = ids_.find(id);
    if (it != ids_.end()) {
        it->second++;
    }
    else {
        ids_.insert(std::make_pair(id, 1));
    }
}

bool ScriptSystem::IdSet::contains(std::size_t id) const {
    auto it = ids_.find(id);
    if (it != ids_.end()) {
        return true;
    }
    return false;
}

void ScriptSystem::IdSet::remove(std::size_t id) {
    auto it = ids_.find(id);
    PG_ASSERT(it != ids_.end());
    it->second--;
    if (it->second == 0) {
        ids_.erase(it);
    }
}

ScriptSystem::ScriptSystem(Context& context, KeyboardManager& keyboard, MouseEvents& mouse)
    : context_{ context },
    keyboardManager_{ keyboard },
    mouseManager_{ mouse },
    containedScripts_{},
    updatedScripts_{} {}

void ScriptSystem::configure(ecs::EventManager& events) {
    events.subscribe< ecs::ComponentAssignedEvent< component::Script> >(*this);
    events.subscribe< ecs::ComponentRemovedEvent< component::Script > >(*this);
    events.subscribe< system::TextFileUpdated >(*this);
    keyboardManager_.setScriptHandler(*this);
    mouseManager_.setScriptHandler(*this);
}

void ScriptSystem::update(ecs::EntityManager& entities, ecs::EventManager& events, float dt) {
    if (!updatedScripts_.empty()) {
        for (auto entity : entities.join< component::Script>()) {
            for (auto id : updatedScripts_) {
                if (id != entity.component<component::Script>()->scriptId) {
                    continue;
                }

                wrenpp::VM vm;
                vm.executeString(
                    "import \"pg/entity\" for Entity\n"
                    "var entity = Entity.new()\n"
                    );
                {
                    auto set = vm.method("main", "entity", "set_(_)");
                    set(int(entity.id().index()));
                }
                wrenpp::Result res = vm.executeString(context_.textFileManager.get(id));
                if (res != wrenpp::Result::Success) {
                    continue;
                }
                entity.remove< component::Script >();
                entity.assign< component::Script >(
                    id,
                    std::move(vm)
                    );
            }
        }
        updatedScripts_.clear();
    }

    for (ecs::Entity entity : entities.join< component::Script >()) {
        entity.component< component::Script >()->update(dt);
    }
}

void ScriptSystem::receive(const ecs::ComponentAssignedEvent< component::Script >& event) {
    event.component->activate();
    containedScripts_.add(event.component->scriptId);
}

void ScriptSystem::receive(const ecs::ComponentRemovedEvent< component::Script >& event) {
    event.component->deactivate();
    containedScripts_.remove(event.component->scriptId);
}

void ScriptSystem::receive(const system::TextFileUpdated& event) {
    LOG_DEBUG << "Text file (id: " << event.id << ") updated";
    if (containedScripts_.contains(event.id)) {
        updatedScripts_.insert(event.id);
    }
}

void ScriptSystem::onKeyDown(const char* str, ecs::Entity* entity) {
    auto callback = entity->component<component::Script>()->vm.method("main", "onKeyDown", "call(_)");
    callback(str);
}

void ScriptSystem::onKeyPressed(const char* str, ecs::Entity* entity) {
    auto callback = entity->component<component::Script>()->vm.method("main", "onKeyPressed", "call(_)");
    callback(str);
}

void ScriptSystem::onKeyUp(const char* str, ecs::Entity* entity) {
    auto callback = entity->component<component::Script>()->vm.method("main", "onKeyUp", "call(_)");
    callback(str);
}

void ScriptSystem::onMouseDown(const char* str, ecs::Entity* entity) {
    auto callback = entity->component<component::Script>()->vm.method("main", "onMouseDown", "call(_)");
    callback(str);
}

void ScriptSystem::onMousePressed(const char* str, ecs::Entity* entity) {
    auto callback = entity->component<component::Script>()->vm.method("main", "onMousePressed", "call(_)");
    callback(str);
}

void ScriptSystem::onMouseUp(const char* str, ecs::Entity* entity) {
    auto callback = entity->component<component::Script>()->vm.method("main", "onMouseUp", "call(_)");
    callback(str);
}

void ScriptSystem::onMouseScroll(std::int32_t dx, std::int32_t dy, ecs::Entity* entity) {
    auto callback = entity->component<component::Script>()->vm.method("main", "onMouseScroll", "call(_,_)");
    callback(dx, dy);
}

void ScriptSystem::listenToKeyDown(std::string str, ecs::Entity* entity) {
    keyboardManager_.registerKeyDownScriptCallback(str, entity);
}

void ScriptSystem::listenToKeyPressed(std::string str, ecs::Entity* entity) {
    keyboardManager_.registerKeyPressedScriptCallback(str, entity);
}

void ScriptSystem::listenToKeyUp(std::string str, ecs::Entity* entity) {
    keyboardManager_.registerKeyUpScriptCallback(str, entity);
}

void ScriptSystem::listenToMouseDown(std::string str, ecs::Entity* entity) {
    mouseManager_.registerMouseDownScriptCallback(str, entity);
}

void ScriptSystem::listenToMousePressed(std::string str, ecs::Entity* entity) {
    mouseManager_.registerMousePressedScriptCallback(str, entity);
}

void ScriptSystem::listenToMouseUp(std::string str, ecs::Entity* entity) {
    mouseManager_.registerMouseUpScriptCallback(str, entity);
}

void ScriptSystem::listenToMouseScroll(ecs::Entity* entity) {
    mouseManager_.registerMouseScrollScriptCallback(entity);
}

}
}
