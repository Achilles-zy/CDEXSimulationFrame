//CDEXRunAction.cc

#include "CDEXRunAction.hh"
#include "CDEXRunMessenger.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <time.h>
#include "g4root.hh"
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXRunAction::CDEXRunAction(CDEXPrimaryGeneratorAction *gen, CDEXDetectorConstruction *det) : SiPMEventCount(0),
																							   FiberPhotonCount(0),
																							   BulkEventCount(0),
																							   ROIEventCount(0),
																							   VetoEventCount(0),
																							   ROIVetoEventCount(0),
																							   VetoPossibleEvtCount(0),
																							   ROIVetoPossibleEvtCount(0),
																							   runID(0)
{
	fRunMessenger = new CDEXRunMessenger(this);
	auto analysisManager = G4AnalysisManager::Instance();
	auto accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->RegisterAccumulable(SiPMEventCount);
	accumulableManager->RegisterAccumulable(FiberPhotonCount);
	accumulableManager->RegisterAccumulable(BulkEventCount);
	accumulableManager->RegisterAccumulable(ROIEventCount);
	accumulableManager->RegisterAccumulable(VetoEventCount);
	accumulableManager->RegisterAccumulable(ROIVetoEventCount);
	accumulableManager->RegisterAccumulable(VetoPossibleEvtCount);
	accumulableManager->RegisterAccumulable(ROIVetoPossibleEvtCount);
	analysisManager->SetNtupleMerging(true);
	analysisManager->SetVerboseLevel(1);
	//analysisManager->CreateH1("edepBulk", "Edep in Bulk", 200, 0 * keV, 20 * keV);

	fDetCons = det;
	fPrimaryGenerator = gen;
	filename = "Simulation Result";
	txtname = "Simulation Result";

	if (fDetCons->GetMode() == "CDEX300")
	{
		analysisManager->CreateNtuple("RunSum", "Run Summary");
		analysisManager->CreateNtupleIColumn(0, "BulkEventCount");
		analysisManager->CreateNtupleIColumn(0, "ROIEventCount");
		analysisManager->FinishNtuple(0);

		analysisManager->CreateNtuple("EdepArgon", "Edep in Argon");
		analysisManager->CreateNtupleIColumn(1, "ParticleType");
		analysisManager->CreateNtupleIColumn(1, "CreatorProcess");
		analysisManager->CreateNtupleDColumn(1, "PosX");
		analysisManager->CreateNtupleDColumn(1, "PosY");
		analysisManager->CreateNtupleDColumn(1, "PosZ");
		analysisManager->CreateNtupleDColumn(1, "Edep");
		analysisManager->CreateNtupleIColumn(1, "EventID");
		analysisManager->FinishNtuple(1);

		analysisManager->CreateNtuple("EdepArgonVeto", "Veto Edep in Argon"); //Bulk events that deposit energy in Ar
		analysisManager->CreateNtupleIColumn(2, "ParticleType");
		analysisManager->CreateNtupleIColumn(2, "CreatorProcess");
		analysisManager->CreateNtupleDColumn(2, "PosX");
		analysisManager->CreateNtupleDColumn(2, "PosY");
		analysisManager->CreateNtupleDColumn(2, "PosZ");
		analysisManager->CreateNtupleDColumn(2, "Edep");
		analysisManager->CreateNtupleIColumn(2, "EventID");
		analysisManager->FinishNtuple(2);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXRunAction::~CDEXRunAction()
{
	delete G4AnalysisManager::Instance();
	delete G4AccumulableManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXRunAction::BeginOfRunAction(const G4Run *aRun)
{
	auto analysisManager = G4AnalysisManager::Instance();
	auto accumulableManager = G4AccumulableManager::Instance();
	G4int RunID = aRun->GetRunID();

	if (fDetCons->GetMode() == "CDEX300")
	{
		G4int thickness = std::floor(fDetCons->GetCuShieldThickness() / (1 * cm));
		filename = "CDEX300_" + fPrimaryGenerator->GetSrcType() + "_" + std::to_string(thickness) + "cm";
		txtname = "CDEX300_" + fPrimaryGenerator->GetSrcType() + "_" + std::to_string(thickness) + "cm";
		analysisManager->OpenFile(filename);
		accumulableManager->Reset();
	}

	if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
	{
		G4cout << "Run started." << G4endl;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXRunAction::EndOfRunAction(const G4Run *aRun)
{
	auto analysisManager = G4AnalysisManager::Instance();
	auto accumulableManager = G4AccumulableManager::Instance();
	if (fDetCons->GetMode() == "CDEX300")
	{
		accumulableManager->Merge();
		if (G4RunManager::GetRunManager()->GetRunManagerType() != 1)
		{
			analysisManager->FillNtupleIColumn(0, 0, BulkEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 1, ROIEventCount.GetValue());
			analysisManager->AddNtupleRow(0);
		}
		CDEX300Output(aRun);
		G4cout << "Output Done" << G4endl;
		G4int IntWL = std::floor(fPrimaryGenerator->GetWavelength() / (1 * nm));
		G4int IntTheta = std::floor(fPrimaryGenerator->GetBeamTheta() / (1 * degree));
		G4int IntPhi = std::floor(fPrimaryGenerator->GetBeamPhi() / (1 * degree));
		G4String fileName0 = filename + ".root";
		G4String fileName1 = filename + "_" + fPrimaryGenerator->GetPrimaryName() + ".root";

		analysisManager->Write();
		analysisManager->CloseFile();
		if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
		{
			std::rename(fileName0, fileName1);
		}
	}
	else
	{
		G4cout << "ERROR! Mode does not exsist, nothing to output!" << G4endl;
	}

	G4cout << "Simulation End" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXRunAction::CDEX300Output(const G4Run *aRun)
{
	if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
	{
		G4cout << "End Running ..." << G4endl;
		G4cout << "==========================Run Summary==========================" << G4endl;
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "Run" << aRun->GetRunID() << " Finished" << G4endl;
		G4cout << "TotalEvent =" << aRun->GetNumberOfEvent() << G4endl;
		G4cout << "BulkEventCount =" << BulkEventCount.GetValue() << G4endl;
		G4cout << "ROIEventCount =" << fDetCons->GetCuShieldThickness() << G4endl;
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "===============================================================" << G4endl;

		std::ofstream output;
		if (aRun->GetRunID() == 0)
		{
			output.open(txtname + ".txt", std::ios::ate);
			//output.open("FiberTest_Point.txt", std::ios::app);
			output
				<< "Simulation Result" << G4endl
				<< "Source Type: " << fPrimaryGenerator->GetSrcType() << G4endl
				<< "Cu Shield Thickness: " << fDetCons->GetCuShieldThickness() / (1 * cm) << " cm" << G4endl;
		}
		else
		{
			output.open(txtname + ".txt", std::ios::app);
		}
		G4double eff;

		if (fPrimaryGenerator->GetSrcType() == "SSWall")
		{
			if (aRun->GetRunID() == 0)
			{
				output
					<< std::setw(10) << std::left << "Run ID" << '\t'
					<< std::setw(40) << std::left << "Number of Event" << '\t'
					<< std::setw(40) << std::left << "Primary Particle" << '\t'
					<< std::setw(40) << std::left << "Primary Energy(eV)" << '\t'
					<< std::setw(40) << std::left << "Bulk Event Count" << '\t'
					<< std::setw(40) << std::left << "ROI Event Count" << G4endl;
			}

			output
				<< std::setw(10) << std::left << aRun->GetRunID() << '\t'
				<< std::setw(40) << std::left << aRun->GetNumberOfEvent() << '\t'
				<< std::setw(40) << std::left << fPrimaryGenerator->GetPrimaryName() << '\t'
				<< std::setw(40) << std::left << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() / (1 * eV)<< '\t'
				<< std::setw(40) << std::left << BulkEventCount.GetValue() << '\t'
				<< std::setw(40) << std::left << ROIEventCount.GetValue() << G4endl;
			output.close();
		}
		else if (fPrimaryGenerator->GetSrcType() == "CuShield")
		{
			if (aRun->GetRunID() == 0)
			{
				output
					<< std::setw(10) << std::left << "Run ID" << '\t'
					<< std::setw(40) << std::left << "Number of Event" << '\t'
					<< std::setw(40) << std::left << "Primary Particle" << '\t'
					<< std::setw(40) << std::left << "Primary Energy(eV)" << '\t'
					<< std::setw(40) << std::left << "Bulk Event Count" << '\t'
					<< std::setw(40) << std::left << "ROI Event Count" << G4endl;
			}
			output
				<< std::setw(10) << std::left << aRun->GetRunID() << '\t'
				<< std::setw(40) << std::left << aRun->GetNumberOfEvent() << '\t'
				<< std::setw(40) << std::left << fPrimaryGenerator->GetPrimaryName() << '\t'
				<< std::setw(40) << std::left << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() / (1 * eV)<< '\t'
				<< std::setw(40) << std::left << BulkEventCount.GetValue() << '\t'
				<< std::setw(40) << std::left << ROIEventCount.GetValue() << G4endl;
			output.close();
			//std::DecimalFormat df1 = new DecimalFormat("0.0");
		}
	}
}