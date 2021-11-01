
#pragma once
#include "G4VPhysicsConstructor.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;
class G4OpWLS;

class CDEXOpticalPhysics : public G4VPhysicsConstructor
{
public:
    CDEXOpticalPhysics();
    ~CDEXOpticalPhysics();

    virtual void ConstructProcess();
    virtual void ConstructParticle();

private:
    G4int OpVerbLevel;

    //end of adds
    static G4ThreadLocal G4int fVerboseLevel;
    static G4ThreadLocal G4int fMaxNumPhotonStep;

    static G4ThreadLocal G4Cerenkov* fCerenkovProcess;
    static G4ThreadLocal G4Scintillation* fScintillationProcess;
    static G4ThreadLocal G4OpAbsorption* fAbsorptionProcess;
    static G4ThreadLocal G4OpRayleigh* fRayleighScatteringProcess;
    static G4ThreadLocal G4OpMieHG* fMieHGScatteringProcess;
    static G4ThreadLocal G4OpBoundaryProcess* fBoundaryProcess;
    static G4ThreadLocal G4OpWLS* fWLSProcess;
}; 
