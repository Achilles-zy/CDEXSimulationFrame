#ifndef CDEXPrimaryGeneratorAction_h
#define CDEXPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "CDEXDetectorConstruction.hh"
#include "CDEXPrimaryGeneratorMessenger.hh"

class G4GeneralParticleSource;
class G4Event;
class CDEXDetectorConstruction;
class CDEXPrimaryGeneratorMessenger;

class CDEXPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:
    CDEXPrimaryGeneratorAction(CDEXDetectorConstruction *);
    ~CDEXPrimaryGeneratorAction();
    void GeneratePrimaries(G4Event *anEvent);
    G4double GetPrimaryE()
    {
        return fPrimaryE;
    }

    G4String GetPrimaryName()
    {
        return fPrimaryName;
    }

    G4double GetBeamTheta()
    {
        return fBeamTheta;
    }

    G4double GetBeamPhi()
    {
        return fBeamPhi;
    }

    G4double GetWavelength()
    {
        return fWavelength;
    }

    void SetSrcType(G4String type)
    {
        fSrcType = type;
    }

    void SetBeamTheta(G4double theta)
    {
        fBeamTheta = theta;
    }

    void SetBeamPhi(G4double phi)
    {
        fBeamPhi = phi;
    }

    void SetWavelength(G4double wavelength)
    {
        fWavelength = wavelength;
    }

    G4String GetSrcType()
    {
        return fSrcType;
    }

private:
    G4double fPrimaryE;
    G4double fInitialE;
    G4String fPrimaryName;
    G4String fSrcType;

    G4double fBeamPhi;
    G4double fBeamTheta;
    G4double fWavelength;

    G4int ImprintID=1;

    G4GeneralParticleSource *fCDEXGPS;
    CDEXDetectorConstruction *fDetCons;
    CDEXPrimaryGeneratorMessenger *fPrimaryMessenger;
};

#endif