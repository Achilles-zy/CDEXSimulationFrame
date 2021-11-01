#include "CDEXActionInitialization.hh"
#include "CDEXPrimaryGeneratorAction.hh"
#include "CDEXRunAction.hh"
#include "CDEXEventAction.hh"
#include "CDEXSteppingAction.hh"
#include "CDEXTrackingAction.hh"
#include "CDEXStackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXActionInitialization::CDEXActionInitialization(CDEXDetectorConstruction *det)
	: G4VUserActionInitialization()
{
	fDetCons = det;
	fPrimaryGen = new CDEXPrimaryGeneratorAction(fDetCons);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXActionInitialization::~CDEXActionInitialization()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXActionInitialization::BuildForMaster() const
{
	CDEXPrimaryGeneratorAction *gen = new CDEXPrimaryGeneratorAction(fDetCons);
	CDEXRunAction *runAction = new CDEXRunAction(fPrimaryGen, fDetCons);
	SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXActionInitialization::Build() const
{
	CDEXPrimaryGeneratorAction *gen = new CDEXPrimaryGeneratorAction(fDetCons);
	//fPrimaryGen = gen;
	SetUserAction(fPrimaryGen);

	CDEXRunAction *runAction = new CDEXRunAction(gen, fDetCons);
	SetUserAction(runAction);

	CDEXEventAction *eventAction = new CDEXEventAction(runAction, fDetCons);
	SetUserAction(eventAction);

	CDEXTrackingAction *trackAction = new CDEXTrackingAction(eventAction, fDetCons);
	SetUserAction(trackAction);

	SetUserAction(new CDEXSteppingAction(trackAction, eventAction, runAction, fDetCons));

	SetUserAction(new CDEXStackingAction);
}