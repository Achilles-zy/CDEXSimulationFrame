#include "CDEXRunMessenger.hh"

#include "CDEXRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

class CDEXPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

CDEXRunMessenger::CDEXRunMessenger(CDEXRunAction* run)
    : G4UImessenger()
{
    fSrcDir = new G4UIdirectory("/CDEX/run/");
    fSrcDir->SetGuidance("Run control");

    fAction = run;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXRunMessenger::~CDEXRunMessenger()
{
    delete fSrcDir;
    //delete fSourceEnergy;
    //delete fSourcePositionX;
    //delete fSourcePositionY;
    //delete fSourcePositionZ;
    //delete fAction;//debug
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXRunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
