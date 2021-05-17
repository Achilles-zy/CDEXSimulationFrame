// CDEXSteppingAction.cc

#include "CDEXSteppingAction.hh"

#include "CDEXDetectorConstruction.hh"
#include "CDEXEventAction.hh"
#include "CDEXRunAction.hh"
#include "CDEXTrackingAction.hh"

#include "G4Track.hh"
#include "G4SteppingManager.hh"

#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4HadronicProcessType.hh"
#include "G4EmProcessSubType.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXSteppingAction::CDEXSteppingAction(
	CDEXTrackingAction *track, CDEXEventAction *evt, CDEXRunAction *run, CDEXDetectorConstruction *cons)
	: CDEXTrack(track), CDEXEvent(evt), CDEXRun(run), CDEXCons(cons)
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXSteppingAction::UserSteppingAction(const G4Step *aStep)
{
	if (CDEXCons->GetMode() == "CDEX300")
	{
		//G4cout << "EnableAcc: " << CDEXRun->GetAccelerate() << G4endl;
		auto volume = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
		G4ThreeVector PostStepPos = aStep->GetPostStepPoint()->GetPosition();
		G4ThreeVector PreStepPos = aStep->GetPreStepPoint()->GetPosition();
		G4LogicalVolume *logicvolume = nullptr;
		if (volume)
		{
			logicvolume = volume->GetLogicalVolume();
		}

		auto touchable = aStep->GetPostStepPoint()->GetTouchableHandle();
		auto ParticleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
		auto edep = aStep->GetTotalEnergyDeposit();
		auto KineticE = aStep->GetPreStepPoint()->GetKineticEnergy();
		auto CurrentWL = 1240 / (KineticE / (1 * eV));
		auto DeltaE = aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy();
		CDEXTrack->AddEdepTrack(edep);
		G4int TrackID = aStep->GetTrack()->GetTrackID();
		G4int ParentTrackID = aStep->GetTrack()->GetParentID();

		const CDEXDetectorConstruction *detectorConstruction = static_cast<const CDEXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
				
		if (volume == detectorConstruction->GetBulk())
		{
			CDEXEvent->AddBulkEnergy(edep);
		}
		
		G4int ParticleType = -1;
		ParticleType = GetParticleIntType(ParticleName);
		G4String CreatorProcessName = "Default";
		if (aStep->GetTrack()->GetTrackID() > 1)
		{
			CreatorProcessName = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();
		}
		G4int CreatorProcessType = -1;
		CreatorProcessType = GetCreatorProcessIntType(CreatorProcessName);
		G4String Mode = CDEXCons->GetMode();
		if (volume && logicvolume == detectorConstruction->GetArgonVolume(Mode) && edep > 1 * eV && ParticleType != 0)
		{
			CDEXEvent->DetectableTrue();
		}
		if (CreatorProcessType == 0 || TrackID == 1)
		{
			if (volume && logicvolume == detectorConstruction->GetArgonVolume(Mode) && DeltaE > 1 * eV && ParticleType != 0)
			{
				CDEXEvent->RecordEdepInfoInScintillator(ParticleType, CreatorProcessType, PostStepPos.getX(), PostStepPos.getY(), PostStepPos.getZ(), DeltaE);
			}
		}

	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double CDEXSteppingAction::GetEfficiency(G4double wavelength)
{
	G4double eff;
	if (wavelength > 400 && wavelength <= 900)
	{
		eff = 0.5 - (wavelength - 400) / 1000;
	}
	else if (wavelength > 100 && wavelength <= 400)
	{
		eff = (wavelength - 100) * 5 / 3000;
	}
	else
	{
		eff = 0;
	}
	return eff;
}