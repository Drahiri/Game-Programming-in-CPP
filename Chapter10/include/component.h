#ifndef COMPONENT_H
#define COMPONENT_H

class Actor;

class Component {
public:
    // Contructor
    // (the lower the update order, the ealier the component updates)
    Component(Actor* owner, int updateOrder = 100);
    // Destructor
    virtual ~Component();

    virtual void processInput(const bool* keyState);
    virtual void onUpdateWorldTransform();

    // Update this component by delta time
    virtual void update(float deltaTime);
    int getUpdateOrder() const;

    Actor* getOwner();

protected:
    // Owning Actor
    Actor* owner;
    // Update order of component
    int updateOrder;
};

#endif
