// floason (C) 2026
// Licensed under the MIT License.

#include <forward_list>
#include <cstring>

#include "nuke.hpp"
#include "game_interface.hpp"
#include "testentity.hpp"

// Set the entity of this descriptor and set default properties.
PhysicsDescriptor::PhysicsDescriptor(nuke::IEntity* entity) :
    entity_(entity)
{
}

// Get the described entity.
nuke::IEntity* PhysicsDescriptor::GetEntity()
{
    return entity_;
}

// Get a reference to the the entity's world origin.
nuke::Vector2& PhysicsDescriptor::GetOrigin() 
{
    return origin_;
}

// Get a reference to the entity's minimum bounding box vector.
nuke::Vector2& PhysicsDescriptor::GetMins()
{
    return mins_;
}

// Get a reference to the entity's maximum bounding box vector.
nuke::Vector2& PhysicsDescriptor::GetMaxs()
{
    return maxs_;
}

// Get a reference to the entity's rotation attribute.
float& PhysicsDescriptor::GetRotation()
{
    return rotation_;
}

// Set the entity of this descriptor and set default properties.
TextureDescriptor::TextureDescriptor(nuke::IEntity* entity) :
    entity_(entity)
{
}

// Manage texture instance upon destruction.
TextureDescriptor::~TextureDescriptor()
{
    if (owns_texture_)
        delete texture_;
}

// Get the described entity.
nuke::IEntity* TextureDescriptor::GetEntity()
{
    return entity_;
}

// Get the entity's texture.
nuke::ITexture* TextureDescriptor::GetTexture()
{
    return texture_;
}

// Set the entity's texture.
void TextureDescriptor::SetTexture(nuke::ITexture* texture, bool set_bounds)
{
    // The old texture must be deleted if the entity owns it.
    if (texture_ != nullptr && owns_texture_)
        delete texture_;

    texture_ = texture;
    owns_texture_ = false;

    // If set_bounds is true, the entity's size should be reset to the texture's
    // size, with the centre of mass set to the centre of the texture.
    if (set_bounds)
    {
        nuke::IPhysicsDescriptor* physics = entity_->GetPhysicsDescriptor();
        if (physics != nullptr)
        {
            nuke::Vector2 maxs = texture->GetSize() / 2.f;
            physics->GetMaxs() = maxs;
            physics->GetMins() = -maxs;
        }
    }
}

// Get a reference to the render size vector of the texture.
nuke::Vector2& TextureDescriptor::GetRenderSize()
{
    return render_size_;
}

// Get a reference to the crop offset vector when drawing the texture.
nuke::Vector2& TextureDescriptor::GetCropOffset()
{
    return crop_offset_;
}

// Get a reference to the color struct of this texture.
nuke::Color& TextureDescriptor::GetColor()
{
    return color_;
}

// Get a reference to the scale property for deciding whether to scale
// the drawn texture to fit its render size.
bool& TextureDescriptor::GetScale()
{
    return scale_;
}

// Set ownership of the loaded texture to this entity, if applicable.
void TextureDescriptor::OwnTexture()
{
    owns_texture_ = true;
}

// Create a new entity by classname.
nuke::IEntity* TestEntityManager::CreateEntity(const char* name)
{
    // TODO: in sdk use factory
    TestEntity* entity = nullptr;
    if (std::strcmp(name, "test_entity") == 0)
    {
        entity = new TestEntity();

        nuke::ITextureDescriptor* texture = entity->GetTextureDescriptor();
        texture->SetTexture(engine->CreateRawTexture("texture_rect"));
        texture->OwnTexture();
        
        entities_.emplace_front(entity);
        entity->it_ = entities_.begin();
    }

    return entity;
}

// Get the next entity. The first entity listed is returned if first is NULL.
// Returns NULL when all entities have been traversed.
nuke::IEntity* TestEntityManager::GetEntity(nuke::IEntity* first)
{
    if (first == NULL)
        return !entities_.empty() ? entities_.front() : nullptr;
    auto next = std::next(static_cast<TestEntity*>(first)->it_);
    return (next != entities_.end()) ? *next : nullptr;
}

// Set entity default properties.
TestEntity::TestEntity() :
    physics_(this),
    texture_(this)
{
}

// Get the entity's physics descriptor.
nuke::IPhysicsDescriptor* TestEntity::GetPhysicsDescriptor()
{
    return &physics_;
}

// Get the entity's texture descriptor.
nuke::ITextureDescriptor* TestEntity::GetTextureDescriptor()
{
    return &texture_;
}