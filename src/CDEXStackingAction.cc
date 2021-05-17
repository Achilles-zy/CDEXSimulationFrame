#pragma once
#include "CDEXStackingAction.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4IonTable.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4HadronicProcessType.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXStackingAction::CDEXStackingAction()
    :G4UserStackingAction(),
    fFullChain(false),
    fCurrentParent(0)
{
    // for HPGe detector, 10us time window
    fTimeWindow = 10 * us;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXStackingAction::~CDEXStackingAction()
{}


G4ClassificationOfNewTrack CDEXStackingAction::ClassifyNewTrack(const G4Track* track)
{
    G4double charge = track->GetDefinition()->GetPDGCharge();
    G4bool stable = track->GetDefinition()->GetPDGStable();
    G4int ID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    G4double trackTime = track->GetGlobalTime();

    // reset fCurrentParent to 0 for primary (ID=1)
    if (ID == 1) {
        fCurrentParent = 0;
        return fUrgent;
    }

    // secondaries not from radioactivedecay
    G4int processType = track->GetCreatorProcess()->GetProcessSubType();

    if (processType != fRadioactiveDecay) {
        return fUrgent;
    }

    // reset global time after decay that exceeds detector time window
    if (trackTime > fTimeWindow) {
        // G4cout << "current track time (ns): " << trackTime << G4endl;
        const_cast<G4Track*>(track)->SetGlobalTime(0.0);
        // reload energy variables for new stages in the chain
        if (parentID != fCurrentParent) {
            //fDataObject->FillData();
            //fDataObject->ClearData();
            fCurrentParent = parentID;
        }
    }
    // put radioactive secondaries into stacks
    //
    
    if (charge > 2. && track->GetParticleDefinition()->GetPDGEncoding() % 10 == 0) {
        // if no fullchain kill track else put ions to waiting stack      
        if (!fFullChain) {
            return fKill;
        }
        else {
            return fWaiting;
        }
    }
    else {
        // all other secondaries goes to normal stack
        return fUrgent;
    }
}