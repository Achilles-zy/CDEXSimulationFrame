
//LEGEND Version
/*

#ifndef CDEXPhysicsList_h
#define CDEXPhysicsList_h 1

#include "globals.hh"
#include "G4VUserPhysicsList.hh"

class PhysicsListMessenger;

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;
class G4OpWLS;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CDEXPhysicsList : public G4VUserPhysicsList
{
  public:

    CDEXPhysicsList();
    virtual ~CDEXPhysicsList();

  public:

    virtual void ConstructParticle();
    virtual void ConstructProcess();

    virtual void SetCuts();

    //these methods Construct physics processes and register them
    void ConstructDecay();
    void ConstructEM();
    void ConstructOp();

    //for the Messenger
    void SetVerbose(G4int);
    void SetNbOfPhotonsCerenkov(G4int);

  private:
    //added by Luis
    G4int VerboseLevel;
    G4int OpVerbLevel;

    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


 //physics model for simulations of background radiations
 //  Biasing model is included in this model
 //   + standard EM processes with livermore model for low energy interactions
 //   + hadronic process w/ high precision neutron model
 //   + radioactive decay of ions
 //   + radioactive decay of tritium
 //   + biasing for region of LN2 shield
 //   + default cut values
 */
 
//SAGe Version

#pragma once
#include "G4VModularPhysicsList.hh"


class CDEXPhysicsList : public G4VModularPhysicsList
{
public:
    CDEXPhysicsList();
    virtual ~CDEXPhysicsList();
};
