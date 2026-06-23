// floason (C) 2026
// Licensed under the MIT License.

// Interface class for an entity manager for Nuke entities. This is a component of
// IEngine.

#pragma once

namespace nuke
{

class IEntity;

class IEntityManager
{
public:
    // Create a new entity by classname.
    virtual IEntity* CreateEntity(const char* name) = 0;

    // Get the next entity. The first entity listed is returned if first is NULL.
    // Returns NULL when all entities have been traversed.
    virtual IEntity* GetEntity(IEntity* first = nullptr) = 0;
};

}   // namespace nuke