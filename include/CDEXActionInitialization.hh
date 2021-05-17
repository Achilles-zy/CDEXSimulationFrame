#ifndef CDEXActionInitialization_h
#define CDEXActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

#include "CDEXPrimaryGeneratorAction.hh"
#include "CDEXDetectorConstruction.hh"

class CDEXPrimaryGeneratorAction;
class CDEXDetectorConstruction;
/// Action initialization class.

class CDEXActionInitialization : public G4VUserActionInitialization
{
public:
	CDEXActionInitialization(CDEXDetectorConstruction*);
	virtual ~CDEXActionInitialization();

	virtual void BuildForMaster() const;
	virtual void Build() const;

private:
	CDEXPrimaryGeneratorAction* fPrimaryGen;
	CDEXDetectorConstruction* fDetCons;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif