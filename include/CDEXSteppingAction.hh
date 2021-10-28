// CDEXSteppingAction.hh

#ifndef CDEXSteppingAction_h
#define CDEXSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "CDEXRunAction.hh"

#include "G4Types.hh"
#include "G4String.hh"

class CDEXDetectorConstruction;
class CDEXEventAction;
class CDEXRunAction;
class CDEXTrackingAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CDEXSteppingAction : public G4UserSteppingAction
{
public:
  CDEXSteppingAction(CDEXTrackingAction *, CDEXEventAction *, CDEXRunAction *, CDEXDetectorConstruction *);
  ~CDEXSteppingAction(){};

  void UserSteppingAction(const G4Step *);
  inline G4double GetEfficiency(G4double wavelength);
  inline G4double GetPMTEfficiency(G4double wavelength);
  inline G4double GetSiPMEfficiency(G4double wavelength);


  G4int GetParticleIntType(G4String name)
  {
    G4int particletype;
    if (name == "opticalphoton")
    {
      particletype = 0;
    }
    else if (name == "gamma")
    {
      particletype = 1;
    }
    else if (name == "e-")
    {
      particletype = 2;
    }
    else if (name == "e+")
    {
      particletype = 3;
    }
    else if (name == "alpha")
    {
      particletype = 4;
    }
    else
    {
      particletype = 5;
    }
    return particletype;
  }

  G4int GetCreatorProcessIntType(G4String name)
  {
    G4int creatorprocesstype;
    if (name == "RadioactiveDecay" || name == "RadioactiveDecayBase")
    {
      creatorprocesstype = 0;
    }
    else if (name == "conv")
    {
      creatorprocesstype = 1;
    }
    else if (name == "phot")
    {
      creatorprocesstype = 2;
    }
    else if (name == "compt")
    {
      creatorprocesstype = 3;
    }
    else
    {
      creatorprocesstype = 4;
    }
    return creatorprocesstype;
  }

    G4double SiPMWavelength[89];
    G4double SiPMEffciency[89];
    G4double PMTWavelength[108];
    G4double PMTEffciency[108];
private:
  CDEXTrackingAction *CDEXTrack;
  CDEXEventAction *CDEXEvent;
  CDEXRunAction *CDEXRun;
  CDEXDetectorConstruction *CDEXCons;
  G4int ContainerSignalSiPMCount;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
