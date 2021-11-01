#ifndef CDEXTrackingAction_h
#define CDEXTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
class G4Track;
class CDEXDetectorConstruction;
class CDEXEventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CDEXTrackingAction : public G4UserTrackingAction
{
  public:
    CDEXTrackingAction(CDEXEventAction*, CDEXDetectorConstruction* cons);
   ~CDEXTrackingAction();

  public:
    void PreUserTrackingAction(const G4Track*);
    void PostUserTrackingAction(const G4Track*);

    void RecordTrackPos(G4ThreeVector trackpos);
    void AddEdepTrack(G4double edepstep);
	
  private:
      G4double EdepTrack;
      G4ThreeVector TrackPos;
      CDEXEventAction* CDEXEvent;
      CDEXDetectorConstruction* CDEXCons;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
