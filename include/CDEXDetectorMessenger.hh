#ifndef CDEXDetectorMessenger_h
#define CDEXDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class CDEXDetectorConstruction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CDEXDetectorMessenger : public G4UImessenger
{
public:

    CDEXDetectorMessenger(CDEXDetectorConstruction*);
    ~CDEXDetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:

    CDEXDetectorConstruction* fDetCons;

    G4UIdirectory* fCDEXDir;
    G4UIdirectory* fDetDir;
    G4UIdirectory* fMatDir;

    G4UIcmdWithAString* cmdSetMode;
    G4UIcmdWithAnInteger* cmdSetPENPropertiesID;
    G4UIcmdWithADoubleAndUnit* cmdCuShieldThickness;
    G4UIcmdWithADoubleAndUnit* cmdSetArAbsLength;
    G4UIcmdWithADouble* cmdSetArYieldRatio;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
