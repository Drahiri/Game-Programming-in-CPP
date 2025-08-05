#ifndef COMPONENT_H
#define COMPONENT_H

#include <rapidjson/document.h>

class Actor;

class Component {
public:
    enum class TypeID {
        Component = 0,
        AudioComponent,
        BallMove,
        BoxComponent,
        CameraComponent,
        FollowCamera,
        MeshComponent,
        MoveComponent,
        SkeletalMeshComponent,
        SpriteComponent,
        MirrorCamera,
        PointLightComponent,
        TargetComponent,

        NUM_COMPONENT_TYPES
    };

    static const char* typeNames[static_cast<int>(TypeID::NUM_COMPONENT_TYPES)];

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

    virtual TypeID getType() const = 0;

    virtual void loadProperties(const rapidjson::Value& inObject);
    virtual void saveProperties(
          rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject) const;

    template<typename T>
    static Component* create(Actor* owner, const rapidjson::Value& inObject) {
        // Dynamically allocate new component of type
        T* t = new T(owner);
        // Call loadProperties on new component
        t->loadProperties(inObject);
        return t;
    }

protected:
    // Owning Actor
    Actor* owner;
    // Update order of component
    int updateOrder;
};

#endif
