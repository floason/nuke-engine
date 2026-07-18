// floason (C) 2026
// Licensed under the MIT License.

#pragma once

#include <forward_list>

#include "nuke.hpp"

// TODO: use std::vector<T> for final sdk due to cache locality
using entity_list = std::forward_list<nuke::IEntity*>;

class TestEntityManager
{
public:
    // Create a new entity by classname.
    nuke::IEntity* CreateEntity(const char* name);

private:
    entity_list entities_;
};

class TestEntity : public nuke::IEntity, public nuke::IRenderable, public nuke::ICollideable
{
    friend TestEntityManager;

public:
    // Set entity default properties.
    TestEntity();

    // Remove this entity from the renderer's list and delete its texture,
    // if owned.
    virtual ~TestEntity() override;

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

// nuke::ICollideable
public:
    // Get a reference to the collideable's physics context struct.
    nuke::PhysicsContext& GetPhysicsContext() override;

public:
    // Set the entity's texture.
    void SetTexture(nuke::ITexture* texture, bool set_bounds = true);

    // Adjust the bounds of the entity to match that of the loaded texture.
    void AdjustBounds();

    // Adjust the centre origin of render context.
    void AdjustRenderOrigin();

public:
    nuke::PhysicsContext collision;
    nuke::RenderContext render_context;

private:
    nuke::RenderHandle render_handle_   = nuke::InvalidRenderHandle;
    bool owns_texture_                  = false;
};