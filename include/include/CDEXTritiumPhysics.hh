#pragma once

#include "G4VPhysicsConstructor.hh"

class CDEXTritiumPhysics : public G4VPhysicsConstructor
{
public:
    CDEXTritiumPhysics();
    ~CDEXTritiumPhysics();

    virtual void ConstructProcess();
    virtual void ConstructParticle();
};