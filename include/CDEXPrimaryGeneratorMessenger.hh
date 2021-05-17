#ifndef CDEXPrimaryGeneratorMessenger_h
#define CDEXPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class CDEXPrimaryGeneratorAction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CDEXPrimaryGeneratorMessenger : public G4UImessenger
{
public:
  CDEXPrimaryGeneratorMessenger(CDEXPrimaryGeneratorAction *);
  virtual ~CDEXPrimaryGeneratorMessenger();

  virtual void SetNewValue(G4UIcommand *, G4String);

private:
  CDEXPrimaryGeneratorAction *fAction;
  G4UIdirectory *fSrcDir;

  G4UIcmdWithAString *cmdSetSrcType;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
