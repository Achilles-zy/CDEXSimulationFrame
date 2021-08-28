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
																						fSrcType("Default")
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
	ImprintID = 1;
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
			G4double SampleRadius = 0.75 * m;
			G4double SampleHeight = 5 * m;
			if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
			{
				G4cout << "==========================Primary Info==========================" << G4endl;
				G4cout << "Radius: " << SampleRadius << G4endl;
				G4cout << "Height: " << SampleHeight << G4endl;
				G4cout << "================================================================" << G4endl;
			}
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0 * eV);
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(SampleRadius);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(SampleHeight / 2);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0, 0, 0));
			fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("Wall");
		}
		else if (fSrcType == "CuShield")
		{
			G4double SampleRadius = 0.75 * m;
			G4double SampleHeight = 5 * m;
			if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
			{
				G4cout << "==========================Primary Info==========================" << G4endl;
				G4cout << "Radius: " << SampleRadius << G4endl;
				G4cout << "Height: " << SampleHeight << G4endl;
				G4cout << "================================================================" << G4endl;
			}
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0 * eV);
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(SampleRadius);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(SampleHeight / 2);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0, 0, 0));
			fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("CuShield");
		}
		else
		{
			G4cout << "Error: Src type not found! Using Geant4 default settings." << G4endl;
		}
	}

	if (mode == "CDEXFiberBucketSetup" || mode == "CDEXLightGuideBucketSetup" || mode == "CDEXArParametersTest")
	{
		if (fSrcType == "Bucket")
		{
			G4ThreeVector CentCoord;
			G4double zCoord;
			G4double BucketHeight = fDetCons->GetBucketHeight();
			G4double BucketThickness = fDetCons->GetBucketThickness();
			G4double BucketRadius = fDetCons->GetBucketOuterRadius();

			if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
			{
				G4cout << "==========================Primary Info==========================" << G4endl;
				G4cout << "Sample Region Radius: " << BucketRadius << G4endl;
				G4cout << "Sample Region Height: " << BucketHeight << G4endl;
				G4cout << "================================================================" << G4endl;
			}

			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			//fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(BucketRadius * 1.1);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(BucketRadius * 1.1);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(BucketHeight / 2);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("Bucket");
		}

		else if (fSrcType == "ArVolume")
		{
			G4ThreeVector CentCoord;
			G4double zCoord;
			G4double BucketHeight = fDetCons->GetBucketHeight();
			G4double BucketThickness = fDetCons->GetBucketThickness();
			G4double BucketRadius = fDetCons->GetBucketOuterRadius();
			G4double LGRadius = fDetCons->GetLightGuideRadius();

			if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
			{
				G4cout << "==========================Primary Info==========================" << G4endl;
				G4cout << "Sample Region Radius: " << BucketRadius << G4endl;
				G4cout << "Sample Region Height: " << BucketHeight << G4endl;
				G4cout << "================================================================" << G4endl;
			}

			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			//fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(BucketRadius * 1.1);
			// if (mode == "CDEXArParametersTest")
			// {
			// 	fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(LGRadius);
			// }
			// else
			// {
			// 	fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(BucketRadius);
			// }
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(BucketRadius);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(BucketHeight / 2);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("ArVolume");
		}

		else if (fSrcType == "Wire")
		{
			G4double Radius = fDetCons->GetWireRadius();
			G4double Length = fDetCons->GetSmallestUnitHeight() * fDetCons->GetUnitNumber();
			G4ThreeVector WirePos = fDetCons->GetWirePos();
			if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
			{
				G4cout << "==========================Primary Info==========================" << G4endl;
				G4cout << "Sample Region Radius: " << Radius << G4endl;
				G4cout << "Sample Region Length: " << Length << G4endl;
				G4cout << "================================================================" << G4endl;
			}
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(Radius);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(Length / 2);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(WirePos);
			//fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume();
		}

		else if (fSrcType == "Fiber")
		{
			G4double Radius = fDetCons->GetFiberRadius();
			G4double Length = 4 * m;
			G4ThreeVector FiberPos = fDetCons->GetFiberPlacementCenter() + G4ThreeVector(fDetCons->GetFiberPlacementRadius(), 0, 0);
			if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
			{
				G4cout << "==========================Primary Info==========================" << G4endl;
				G4cout << "Sample Region Radius: " << Radius << G4endl;
				G4cout << "Sample Region Length: " << Length << G4endl;
				G4cout << "================================================================" << G4endl;
			}
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(Radius);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(Length / 2);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(FiberPos);
			//fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("Fiber");
		}

		else if (fSrcType == "SingleASIC")
		{
			G4ThreeVector CentCoord;
			G4double zCoord;
			if (ImprintID % 2 == 0)
			{
				zCoord = -(ImprintID / 2 - 1) * fDetCons->GetSmallestUnitHeight() - fDetCons->GetSmallestUnitHeight() / 2;
				CentCoord = G4ThreeVector(0, 0, zCoord * mm);
			}
			if (ImprintID % 2 == 1)
			{
				zCoord = (ImprintID - 1) / 2 * fDetCons->GetSmallestUnitHeight() + fDetCons->GetSmallestUnitHeight() / 2;
				CentCoord = G4ThreeVector(0, 0, zCoord * mm);
			}
			G4double Radius = 3 * cm;
			G4double Length = fDetCons->GetSmallestUnitHeight();

			if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
			{
				G4cout << "==========================Primary Info==========================" << G4endl;
				G4cout << "Sample Region Radius: " << Radius << G4endl;
				G4cout << "Sample Region Length: " << Length << G4endl;
				G4cout << "================================================================" << G4endl;
			}

			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(Radius);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(Length / 2);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(CentCoord);
			fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("ASIC");
		}
	}

	if (mode == "CDEXArExpSetup")
	{
		if (fSrcType == "Point")
		{
			//fCDEXGPS->GetCurrentSource()->SetParticleDefinition(particleTable->FindParticle("opticalphoton"));
			//fCDEXGPS->GetCurrentSource()->SetParticlePolarization(G4ThreeVector(1,1,1));
			fCDEXGPS->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
			//fCDEXGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(PhotonEnergy);
			//fCDEXGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(1*MeV);
			fCDEXGPS->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
			//fCDEXGPS->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
			fCDEXGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0, 0, 0));
			// fCDEXGPS->GetCurrentSource()->GetAngDist()->SetMinPhi(-90 * degree);
			// fCDEXGPS->GetCurrentSource()->GetAngDist()->SetMaxPhi(90 * degree);
			// fCDEXGPS->GetCurrentSource()->GetAngDist()->SetMinTheta(0 * degree);
			// fCDEXGPS->GetCurrentSource()->GetAngDist()->SetMaxTheta(180 * degree);
			// fCDEXGPS->GetCurrentSource()->GetPosDist()->SetRadius(Radius);
			// fCDEXGPS->GetCurrentSource()->GetPosDist()->SetHalfZ(Length / 2);
			// fCDEXGPS->GetCurrentSource()->GetPosDist()->ConfineSourceToVolume("Wire");
		}
	}
	fCDEXGPS->GeneratePrimaryVertex(anEvent);
	if (anEvent->GetEventID() == 0)
	{
		fPrimaryE = fCDEXGPS->GetCurrentSource()->GetParticleEnergy();
		fPrimaryName = fCDEXGPS->GetCurrentSource()->GetParticleDefinition()->GetParticleName();
	}
}