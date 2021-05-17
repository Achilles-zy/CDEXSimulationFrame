#include "CDEXPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"

//#include "TMath.h"
#include "Randomize.hh"

CDEXPrimaryGeneratorAction::CDEXPrimaryGeneratorAction(CDEXDetectorConstruction *det) : G4VUserPrimaryGeneratorAction(),
																						fPrimaryE(0),
																						fInitialE(1 * keV),
																						fPrimaryName(""),
																						fSrcType("Point")
{
	fCDEXGPS = new G4GeneralParticleSource();
	fPrimaryMessenger = new CDEXPrimaryGeneratorMessenger(this);
	fDetCons = det;
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName = "e-";
	fCDEXGPS->SetParticleDefinition(particleTable->FindParticle(particleName));
	fBeamPhi = 0 * degree;
	fBeamTheta = 0 * degree;
	fWavelength = 128 * nm;
	//fWavelength = 420 * nm;//TPB Peak

	/*
	G4PhysicalVolumeStore* PVStore = G4PhysicalVolumeStore::GetInstance();
	G4int i = 0;
	G4VPhysicalVolume* tempPV = NULL;
	while (i < G4int(PVStore->size())) {
		tempPV = (*PVStore)[i];
		G4cout << i << " " << " " << tempPV->GetName() << " " << G4endl;
		i++;
	}
	*/
}

CDEXPrimaryGeneratorAction::~CDEXPrimaryGeneratorAction()
{
	delete fCDEXGPS;
}

void CDEXPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{	

	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	//G4String particleName = "e-";
	//G4double particleEnergy = 0 * MeV;
	G4String mode = fDetCons->GetMode();
	G4int EvtID = anEvent->GetEventID();
	if (mode == "CDEX300")
	{
		if (fSrcType == "SSWall")
		{
			G4double SampleRadius = 0.75*m;
			G4double SampleHeight = 5*m;
			if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
			{
				G4cout << "==========================Primary Info==========================" << G4endl;
				G4cout << "Radius: " << SampleRadius << G4endl;
				G4cout << "Height: " << SampleHeight << G4endl;
				G4cout << "================================================================" << G4endl;
			}
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0*eV);
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(SampleRadius);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(SampleHeight/2);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0, 0, 0));
			fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("Wall");
		}
		else if (fSrcType == "CuShield")
		{
			G4double SampleRadius = 0.75*m;
			G4double SampleHeight = 5*m;
			if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
			{
				G4cout << "==========================Primary Info==========================" << G4endl;
				G4cout << "Radius: " << SampleRadius << G4endl;
				G4cout << "Height: " << SampleHeight << G4endl;
				G4cout << "================================================================" << G4endl;
			}
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0*eV);
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(SampleRadius);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(SampleHeight/2);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0, 0, 0));
			fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("CuShield");
		}
		else
		{
			G4cout << "Error: Src type not found! Using Geant4 default settings." << G4endl;
		}
	}

	fCDEXGPS->GeneratePrimaryVertex(anEvent);

	if (anEvent->GetEventID() == 0)
	{
		fPrimaryE = fCDEXGPS->GetCurrentSource()->GetParticleEnergy();
		fPrimaryName = fCDEXGPS->GetCurrentSource()->GetParticleDefinition()->GetParticleName();
	}
}