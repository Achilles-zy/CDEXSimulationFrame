#pragma once
#ifndef CDEXRunMessenger_h
#define CDEXRunMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class CDEXRunAction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CDEXRunMessenger : public G4UImessenger
{
public:
    CDEXRunMessenger(CDEXRunAction*);
    virtual ~CDEXRunMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:
    CDEXRunAction* fAction;
    G4UIdirectory* fSrcDir;
    G4UIcmdWithABool* cmdSetRefresh;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
