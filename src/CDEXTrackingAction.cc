//CDEXTrackingAction.cc

#include "CDEXTrackingAction.hh"
#include "CDEXDetectorConstruction.hh"
#include "CDEXEventAction.hh"

#include "G4Event.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
CDEXTrackingAction::CDEXTrackingAction(CDEXEventAction* evt, CDEXDetectorConstruction* cons)
    :CDEXEvent(evt),CDEXCons(cons)
{
    EdepTrack = 0;
    TrackPos = G4ThreeVector(0, 0, 0);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
CDEXTrackingAction::~CDEXTrackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void CDEXTrackingAction::PreUserTrackingAction(const G4Track* track)
{
    EdepTrack = 0;
    TrackPos = G4ThreeVector(0, 0, 0);
    G4double trackTime = track->GetGlobalTime();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void CDEXTrackingAction::PostUserTrackingAction(const G4Track* trk)
{
    const CDEXDetectorConstruction* detectorConstruction
        = static_cast<const CDEXDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	G4double Ekin = trk->GetKineticEnergy();
	auto volume = trk->GetVolume();
	G4LogicalVolume* logicvolume;
	if (volume) {
		logicvolume = volume->GetLogicalVolume();
		//G4cout << volume << volume->GetName() << G4endl;
		//G4cout << logicvolume << logicvolume->GetName() << G4endl;
		//G4cout << G4endl;
	}


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXTrackingAction::AddEdepTrack(G4double edepstep) {
    EdepTrack += edepstep;
}

void CDEXTrackingAction::RecordTrackPos(G4ThreeVector trackpos) {
    TrackPos = trackpos;
}