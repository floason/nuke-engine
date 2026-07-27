// floason (C) 2026
// Licensed under the MIT License.

#include <forward_list>
#include <cstring>

#include "nuke.hpp"
#include "game_interface.hpp"
#include "testentity.hpp"

// Create a new entity by classname.
TestEntity* TestEntityManager::CreateEntity(const char* name)
{
    // TODO: in sdk use factory
    TestEntity* entity = nullptr;
    if (std::strcmp(name, "test_entity") == 0)
    {
        entity = new TestEntity();
        entity->SetTexture(engine->CreateRawTexture("texture_rect"));
        entity->render.OwnTexture();
        
        entities_.emplace_front(entity);
    }

    return entity;
}

// Signal the deletion of this collideable.
TestCollideable::~TestCollideable()
{
    nuke::IEvent* event = engine->CreateEvent("collideable_removed");
    event->SetPointer("physics_context", &context);
    engine->FireEvent(event);
}

// Get a reference to the collideable's physics context struct.
nuke::PhysicsContext& TestCollideable::GetPhysicsContext()
{
    return context;
}

// Add this renderable to the renderable list.
TestRenderable::TestRenderable()
{
    AddToRenderList(context);
}

// Remove this renderable from the renderer's list and delete its 
// texture, if owned.
TestRenderable::~TestRenderable()
{
    RemoveFromRenderList();
    if (owns_texture_)
        delete context.texture;
}

// Propagate this renderable to the renderer.
void TestRenderable::AddToRenderList(nuke::RenderContext& context)
{
    render_handle_ = renderer->AddRenderable(this, context);
}

// Tell the renderer to stop tracking this renderer.
void TestRenderable::RemoveFromRenderList()
{
    renderer->RemoveRenderable(render_handle_);
}

// Set ownership of the loaded texture to this renderable, if applicable.
void TestRenderable::OwnTexture()
{
    owns_texture_ = true;
}

// Get a reference to the renderable's handle.
nuke::RenderHandle& TestRenderable::GetRenderHandle()
{
    return render_handle_;
}

// Set entity default properties.
TestEntity::TestEntity()
{
}

// Set the entity's texture.
void TestEntity::SetTexture(nuke::ITexture* texture, bool set_bounds)
{
    // The old texture must be deleted if the entity owns it.
    if (render.context.texture != nullptr && render.IsTextureOwned())
        delete render.context.texture;

    render.context.texture = texture;
    render.OwnTexture();

    // If set_bounds is true, the entity's size should be reset to the texture's
    // size, with the centre of mass set to the centre of the texture.
    if (set_bounds)
        AdjustBounds();
}

// Adjust the bounds of the entity to match that of the loaded texture.
void TestEntity::AdjustBounds()
{
    if (render.context.texture == nullptr)
        return;

    collision.context.maxs = render.context.texture->GetSize() / 2.f;
    collision.context.maxs.y = -collision.context.maxs.y;
    collision.context.mins = -collision.context.maxs;
}

// Adjust the centre origin of render context.
void TestEntity::AdjustRenderOrigin()
{
    render.context.origin = collision.context.origin 
                          - (collision.context.maxs - collision.context.mins) / 2.f;
}