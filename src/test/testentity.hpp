// floason (C) 2026
// Licensed under the MIT License.

#pragma once

#include <forward_list>

#include "nuke.hpp"

// TODO: use std::vector<T> for final sdk due to cache locality
typedef std::forward_list<nuke::IEntity*> entity_list;

class PhysicsDescriptor : public nuke::IPhysicsDescriptor
{
public:
    // Set the entity of this descriptor and set default properties.
    PhysicsDescriptor(nuke::IEntity* entity);

public:
    // Get the described entity.
    virtual nuke::IEntity* GetEntity() override;

    // Get a reference to the the entity's world origin.
    virtual nuke::Vector2& GetOrigin() override;

    // Get a reference to the entity's minimum bounding box vector.
    virtual nuke::Vector2& GetMins() override;

    // Get a reference to the entity's maximum bounding box vector.
    virtual nuke::Vector2& GetMaxs() override;

    // Get a reference to the entity's rotation attribute.
    virtual float& GetRotation() override;

private:
    nuke::IEntity* entity_;
    nuke::Vector2 origin_;
    nuke::Vector2 mins_;
    nuke::Vector2 maxs_;
    float rotation_             = 0.f;
};

class TextureDescriptor : public nuke::ITextureDescriptor
{
public:
    // Set the entity of this descriptor and set default properties.
    TextureDescriptor(nuke::IEntity* entity);

    // Manage texture instance upon destruction.
    virtual ~TextureDescriptor();

public:
    // Get the described entity.
    virtual nuke::IEntity* GetEntity() override;

    // Get the entity's texture.
    virtual nuke::ITexture* GetTexture() override;

    // Set the entity's texture.
    virtual void SetTexture(nuke::ITexture* texture, bool set_bounds = false) override;

    // Get a reference to the render size vector of the texture.
    virtual nuke::Vector2& GetRenderSize() override;

    // Get a reference to the crop offset vector when drawing the texture.
    virtual nuke::Vector2& GetCropOffset() override;

    // Get a reference to the color struct of this texture.
    virtual nuke::Color& GetColor() override;

    // Get a reference to the scale property for deciding whether to scale
    // the drawn texture to fit its render size.
    virtual bool& GetScale() override;

    // Set ownership of the loaded texture to this entity, if applicable.
    virtual void OwnTexture() override;

private:
    nuke::IEntity* entity_;
    nuke::ITexture* texture_    = nullptr;
    nuke::Vector2 render_size_;
    nuke::Vector2 crop_offset_;
    nuke::Color color_          = { 255, 255, 255, 255 };
    bool scale_                 = false;
    bool owns_texture_          = false;
};

class TestEntityManager : public nuke::IEntityManager
{
public:
    // Create a new entity by classname.
    virtual nuke::IEntity* CreateEntity(const char* name) override;

    // Get the next entity. The first entity listed is returned if first is NULL.
    // Returns NULL when all entities have been traversed.
    virtual nuke::IEntity* GetEntity(nuke::IEntity* first = nullptr) override;

private:
    entity_list entities_;
};

class TestEntity : public nuke::IEntity
{
    friend TestEntityManager;

public:
    // Set entity default properties.
    TestEntity();

public:
    // Get the entity's physics descriptor.
    virtual nuke::IPhysicsDescriptor* GetPhysicsDescriptor() override;

    // Get the entity's texture descriptor.
    virtual nuke::ITextureDescriptor* GetTextureDescriptor() override;

private:
    PhysicsDescriptor physics_;
    TextureDescriptor texture_;
    entity_list::iterator it_;
};