#include "CDEXDetectorConstruction.hh"
#include "CDEXPhysicsList.hh"
#include "CDEXRunAction.hh"
#include "CDEXEventAction.hh"
#include "CDEXTrackingAction.hh"
#include "CDEXSteppingAction.hh"
#include "CDEXActionInitialization.hh"
#include "CDEXPrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#define G4MT_USE

#ifdef G4MT_USE
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "time.h"

//#define G4VIS_USE
//#define G4UI_USE


//#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
//#endif

//#ifdef G4UI_USE
#include "G4UIExecutive.hh"
//#endif

#include "Randomize.hh"
#include "CDEXMaterials.hh"
#include "QBBC.hh"
#include "Shielding.hh"

int main(int argc, char** argv)
{
    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4long seed = time(NULL);
    G4Random::setTheSeed(seed);
#ifdef G4MT_USE
	G4MTRunManager* CDEXRunManager = new G4MTRunManager;
#else
	G4RunManager* CDEXRunManager = new G4RunManager;
#endif

    CDEXDetectorConstruction* CDEXDetCons = new CDEXDetectorConstruction();
    CDEXRunManager -> SetUserInitialization(CDEXDetCons);

	G4PhysicalVolumeStore* PVStore = G4PhysicalVolumeStore::GetInstance();
	G4int i = 0;
	G4VPhysicalVolume* tempPV = NULL;
	while (i < G4int(PVStore->size())) {
		tempPV = (*PVStore)[i];
		G4cout << i << " " << " " << tempPV->GetName() << " " << G4endl;
		i++;
	}

	G4LogicalVolumeStore* LVStore = G4LogicalVolumeStore::GetInstance();
	i = 0;
	G4LogicalVolume* tempLV = NULL;
	while (i < G4int(PVStore->size())) {
		tempLV = (*LVStore)[i];
		G4cout << i << " " << " " << tempLV->GetName() << " " << G4endl;
		i++;
	}
	auto physicslist = new CDEXPhysicsList();
    G4VModularPhysicsList* qbbc = new QBBC;
    G4VModularPhysicsList* shielding = new Shielding;
	physicslist->SetVerboseLevel(0);
    CDEXRunManager -> SetUserInitialization(physicslist);
    //CDEXRunManager->SetUserInitialization(shielding);

    CDEXRunManager -> SetUserInitialization(new CDEXActionInitialization(CDEXDetCons));

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/verbose 0");
    UImanager->ApplyCommand("/event/verbose 0");
    UImanager->ApplyCommand("/tracking/verbose 0");
    UImanager->ApplyCommand("/process/em/fluo true");
    UImanager->ApplyCommand("/process/em/auger true");
    UImanager->ApplyCommand("/process/em/augerCascade true");
    UImanager->ApplyCommand("/process/em/pixe true");

    //#ifdef G4VIS_USE
    G4VisManager* CDEXVis = new G4VisExecutive;
    CDEXVis -> Initialize();
    //#endif


    if (argc != 1){
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager -> ApplyCommand(command+fileName);
    }
    else {
        //#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        //#ifdef G4VIS_USE
        //UImanager -> ApplyCommand("/control/execute gps_def_03.mac");
        UImanager -> ApplyCommand("/control/execute init_vis.mac");
        //#endif
        ui -> SessionStart();
        delete ui;
        //#endif
    }

    //#ifdef G4VIS_USE
    delete CDEXVis;
    //#endif
    
    delete CDEXRunManager;
    return 0;
}