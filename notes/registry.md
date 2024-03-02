Registry
---
A Registry is responsible for creating, managing, and destroying all the entities, components and systems. 


```cpp

class Registry {
private:
    int numEntities = 0;

public:
    Registry() = default;
    Entity CreateEntity();
    void RemoveEntity(Entity entity);
    void AddComponent(Component component);
    void RemoveComponent(Component component);
    void AddSystem(System system);
    void RemoveSystem(System system);
}
```

Components will be stored as vector of pools. A pool is a list or a container of data where ith index belongs to entity of ith ID. Each component will have its own pool.

```cpp
class Registry {
private:
    int numEntities = 0;
    std::vector<Pool*> componentPools;
public:
    //...
}
```

A pool is nothing special, it's just a wrapper around a container. We will use std::vector for now.

```cpp
template<typename ComponentType>
class Pool {
private:
    std::vector<ComponentType> data;
public:
    Pool(int size) { data.resize(size); }
    ~Pool() = default;
    ComponentType operator[](unsigned int index) { return data[index];}
    size_t Size() { return entities.size(); }
    T& Get(int index) { return static_cast<T&>(data[index]); }
    void Set(int index, T object) { data[index] = object; };
    void Resize(int size = 100) { data.resize(size); }
    bool IsEmpty() const { return data.empty(); }
    void Clear() {
        data.clear();
    }
    void Add(T object) {
        data.push_back(object);
    }
};
```
The class Pool will inherit from IPool, so that the registry class can have a vector of pools without having to specify the type of each and every component.

```cpp
class IPool {
    public:
    virtual ~IPool() = default;
};
```
Virtual destructor means C++ will run the destructor of the class deriving IPool. 

```cpp 
template<typename CompenentType>
class Pool : public IPool {
public:
virtual ~Pool() = default;
};
```



```
struct Entity {
    // Player related fields like
    struct Combat{
    int hp = 0;
    int mana = 0;
    };
    struct Economy {
    unsigned int money = 0;
    }
    struct Transformation {
    float x = 0.0f,y = 0.0f,z = 0.0f;
    float scalx = 0.0f, scaly = 0.0f, scalz = 0.0f;
    }
    struct Movement {
    float vx = 0.0f, vy = 0.0f, vz = 0.0f;
    }
};



void updatePosition() {
    for(Entity & entity : getAllEntities()) {
        entity.xyz += entity.vxvyvz;
    }
}




```

### Animation

* 2D Sprite Based Animation
* The animation consists of frames which are grouped together in a single texture.
* The frames will be displayed at a certain rate which is the FPS.
* If there are 2 frames and a FPS of 30 has to be maintained then new frames will be displayed every 2/30 = 1/15 of a second.
* Some animations will be looped, some will not be looped.

```cpp
struct AnimationComponent {
    int numFrames = 1;
    int totalFrames = 1;
    int currentFrame = 1;
    float animationTimer = 0.0f
    bool looping = false;

    AnimationComponent() {}
    AnimationComponent(int numFrames, int totalFrames, bool looping = false, int currentFrame = 1) 
        :
        numFrames(numFrames),
        totalFrames(totalFrames),
        looping(looping),
        currentFrame(currentFrame),
        animationTimer(0.0f)
    {

    }
};


class AnimationControlSystem {
private:
    float currentDt = 0.0f;
public:
    AnimationControlSystem() {
        RequireComponent<AnimationComponent>();
    }

    void Update(float dt) {
        for(Entity entity : GetAllEntities()) {
            AnimationComponent & data = registry->GetComponent<AnimationComponent>(entity); 
            data.animationTimer += dt;
            if(currentDt > data.totalFrames/30.0f) {
                data.currentFrame += 1;
                data.animationTimer = 0.0f;
            }
        }
    }
}


class AnimationRenderSystem {

    void Update(float dt) {
        
    }

}


```