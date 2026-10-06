#pragma once
#include "BoxComponent.h"

class PlayerBoxComponent : public BoxComponent
{
public:
    PlayerBoxComponent(class Actor *owner);

    void OnCollision(BoxComponent *otherBC) override;
};