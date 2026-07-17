// floason (C) 2026
// Licensed under the MIT License.

#include <forward_list>
#include <cstring>

#include "nuke.hpp"
#include "game_interface.hpp"
#include "testentity.hpp"

// Create a new entity by classname.
nuke::IEntity* TestEntityManager::CreateEntity(const char* name)
{
    // TODO: in sdk use factory
    TestEntity* entity = nullptr;
    if (std::strcmp(name, "test_entity") == 0)
    {
        entity = new TestEntity();
        entity->SetTexture(engine->CreateRawTexture("texture_rect"));
        entity->OwnTexture();
        
        entities_.emplace_front(entity);
        entity->it_ = entities_.begin();
    }

    return entity;
}

// Get the next entity. The first entity listed is returned if first is NULL.
// Returns NULL when all entities have been traversed.
nuke::IEntity* TestEntityManager::GetEntity(nuke::IEntity* first)
{
    if (first == nullptr)
        return !entities_.empty() ? entities_.front() : nullptr;
    auto next = std::next(static_cast<TestEntity*>(first)->it_);
    return (next != entities_.end()) ? *next : nullptr;
}

// Set entity default properties.
TestEntity::TestEntity()
{
    AddToRenderList(render_context);
}

// Remove this entity from the renderer's list.
TestEntity::~TestEntity()
{
    RemoveFromRenderList();
}

// Propagate this renderable to the renderer.
void TestEntity::AddToRenderList(nuke::RenderContext& context)
{
    renderer->AddRenderable(this, context);
}

// Tell the renderer to stop tracking this renderer.
void TestEntity::RemoveFromRenderList()
{
    renderer->RemoveRenderable(render_handle_);
}

// Set ownership of the loaded texture to this renderable, if applicable.
void TestEntity::OwnTexture()
{
    owns_texture_ = true;
}

// Get a reference to the renderable's handle.
nuke::RenderHandle& TestEntity::GetRenderHandle()
{
    return render_handle_;
}

// Get a reference to the collideable's physics context struct.
nuke::PhysicsContext& TestEntity::GetPhysicsContext()
{
    return collision;
}

// Set the entity's texture.
void TestEntity::SetTexture(nuke::ITexture* texture, bool set_bounds)
{
    // The old texture must be deleted if the entity owns it.
    if (render_context.texture != nullptr && owns_texture_)
        delete render_context.texture;

    render_context.texture = texture;
    owns_texture_ = false;

    // If set_bounds is true, the entity's size should be reset to the texture's
    // size, with the centre of mass set to the centre of the texture.
    if (set_bounds)
        AdjustBounds();
}

// Adjust the bounds of the entity to match that of the loaded texture.
void TestEntity::AdjustBounds()
{
    if (render_context.texture == nullptr)
        return;

    collision.maxs = render_context.texture->GetSize() / 2.f;
    collision.maxs.y = -collision.maxs.y;
    collision.mins = -collision.maxs;
}

// Adjust the centre origin of render context.
void TestEntity::AdjustRenderOrigin()
{
    render_context.origin = collision.origin - (collision.maxs - collision.mins) / 2.f;
}