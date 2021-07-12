#include "CDEXDetectorMessenger.hh"

#include "CDEXDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"

#include "G4RunManager.hh"

class CDEXDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

CDEXDetectorMessenger::CDEXDetectorMessenger(CDEXDetectorConstruction* Det)
	:G4UImessenger(),
	fDetCons(Det),
	fCDEXDir(0),
	fDetDir(0),
	fMatDir(0),
	cmdSetPENPropertiesID(0),
	cmdCuShieldThickness(0),
	cmdSetArAbsLength(0),
	cmdSetArYieldRatio(0)
{
	fDetDir = new G4UIdirectory("/CDEX/cons/set/");
	fDetDir->SetGuidance("Set construction parameters");

	fCDEXDir = new G4UIdirectory("/CDEX/sim/set/");
	fCDEXDir->SetGuidance("Set simulation parameters");

	fMatDir = new G4UIdirectory("/CDEX/mat/set/");
	fMatDir->SetGuidance("Set material parameters");

	cmdSetMode = new G4UIcmdWithAString("/CDEX/sim/set/mode", this);
	cmdSetMode->SetGuidance("Select simulation mode.");
	cmdSetMode->SetParameterName("SimulationMode", false);
	cmdSetMode->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetMode->SetToBeBroadcasted(false);

	cmdSetPENPropertiesID = new G4UIcmdWithAnInteger("/CDEX/mat/set/PENpropertiesID", this);
	cmdSetPENPropertiesID->SetGuidance("Set PEN properties ID");
	cmdSetPENPropertiesID->SetParameterName("ID", false);
	cmdSetPENPropertiesID->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetPENPropertiesID->SetToBeBroadcasted(false);

	cmdCuShieldThickness = new G4UIcmdWithADoubleAndUnit("/CDEX/cons/set/cushieldthickness", this);
	cmdCuShieldThickness->SetGuidance("Set Cu shield Thickness");
	cmdCuShieldThickness->SetParameterName("CuShieldThickness", false);
	cmdCuShieldThickness->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdCuShieldThickness->SetToBeBroadcasted(false);
	cmdCuShieldThickness->SetUnitCategory("Length");

	cmdSetArAbsLength = new G4UIcmdWithADoubleAndUnit("/CDEX/cons/set/arabslength", this);
	cmdSetArAbsLength->SetGuidance("Set Ar Absorption Length");
	cmdSetArAbsLength->SetParameterName("ArAbsLength", false);
	cmdSetArAbsLength->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetArAbsLength->SetToBeBroadcasted(false);
	cmdSetArAbsLength->SetUnitCategory("Length");

	cmdSetArYieldRatio = new G4UIcmdWithADouble("/CDEX/cons/set/arabsyieldratio", this);
	cmdSetArYieldRatio->SetGuidance("Set Ar Yield Ratio");
	cmdSetArYieldRatio->SetParameterName("ArYieldRatio", false);
	cmdSetArYieldRatio->AvailableForStates(G4State_PreInit, G4State_Idle);
	cmdSetArYieldRatio->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXDetectorMessenger::~CDEXDetectorMessenger()
{
	delete fDetDir;
	delete fCDEXDir;
	delete fMatDir;
	delete cmdSetPENPropertiesID;
	delete cmdCuShieldThickness;
	delete cmdSetArYieldRatio;
	delete cmdSetArAbsLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

	if (command == cmdSetMode) {
		fDetCons->SetMode(newValue);
	}

	if (command == cmdSetPENPropertiesID) {
		fDetCons->SetPENPropertiesID(cmdSetPENPropertiesID->GetNewIntValue(newValue));
	}

	if (command==cmdCuShieldThickness){
		fDetCons->SetCuShieldThickness(cmdCuShieldThickness->GetNewDoubleValue(newValue));
	}

	if (command==cmdSetArAbsLength){
		fDetCons->SetArAbsLength(cmdSetArAbsLength->GetNewDoubleValue(newValue));
	}

	if (command==cmdSetArYieldRatio){
		fDetCons->SetArYieldRatio(cmdSetArYieldRatio->GetNewDoubleValue(newValue));
	}
}