// floason (C) 2026
// Licensed under the MIT License.

#pragma once

#include <forward_list>

#include "nuke.hpp"

// TODO: use std::vector<T> for final sdk due to cache locality
using entity_list = std::forward_list<nuke::IEntity*>;

class TestEntity;

class TestEntityManager
{
public:
    // Create a new entity by classname.
    TestEntity* CreateEntity(const char* name);

private:
    entity_list entities_;
};

class TestCollideable : public nuke::ICollideable
{
public:
    // Signal the deletion of this collideable.
    virtual ~TestCollideable();

// nuke::ICollideable
public:
    // Get a reference to the collideable's physics context struct.
    virtual nuke::PhysicsContext& GetPhysicsContext() override;

public:
    nuke::PhysicsContext context;
};

class TestRenderable : public nuke::IRenderable
{
public:
    // Add this renderable to the renderable list.
    TestRenderable();

    // Remove this renderable from the renderer's list and delete its 
    // texture, if owned.
    virtual ~TestRenderable();

// nuke::IRenderable
public:
    // Propagate this renderable to the renderer.
    virtual void AddToRenderList(nuke::RenderContext& context) override;

    // Tell the renderer to stop tracking this renderer.
    virtual void RemoveFromRenderList() override;

    // Set ownership of the loaded texture to this renderable, if applicable.
    virtual void OwnTexture() override;

    // Get a reference to the renderable's handle.
    virtual nuke::RenderHandle& GetRenderHandle() override;

public:
    // Does the renderable own its texture?
    inline bool IsTextureOwned()
    {
        return owns_texture_;
    }

public:
    nuke::RenderContext context;

private:
    nuke::RenderHandle render_handle_   = nuke::InvalidRenderHandle;
    bool owns_texture_                  = false;
};

class TestEntity : public nuke::IEntity
{
public:
    // Set entity default properties.
    TestEntity();
    
public:
    // Set the entity's texture.
    void SetTexture(nuke::ITexture* texture, bool set_bounds = true);

    // Adjust the bounds of the entity to match that of the loaded texture.
    void AdjustBounds();

    // Adjust the centre origin of render context.
    void AdjustRenderOrigin();

public:
    TestCollideable collision;
    TestRenderable render;
};