#include "target_component.h"

#include "actor.h"
#include "game.h"
#include "hud.h"

TargetComponent::TargetComponent(Actor* owner) : Component(owner) {
    owner->getGame()->getHUD()->addTarget(this);
}

TargetComponent::~TargetComponent() {
    owner->getGame()->getHUD()->removeTarget(this);
}

Component::TypeID TargetComponent::getType() const {
    return Component::TypeID::TargetComponent;
}