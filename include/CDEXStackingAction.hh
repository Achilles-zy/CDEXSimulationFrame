#pragma once

#include "G4UserStackingAction.hh"
#include "globals.hh"

//class SAGEDataObject;


class CDEXStackingAction : public G4UserStackingAction {

public:
    CDEXStackingAction();
    ~CDEXStackingAction();

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
    inline void SetTimeWindow(G4double tWin) { fTimeWindow = tWin; }

private:
    //SAGEDataObject* fDataObject;
    G4bool   fFullChain;
    G4int    fCurrentParent;
    G4double fTimeWindow;
};