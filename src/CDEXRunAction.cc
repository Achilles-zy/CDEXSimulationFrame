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
																							   VetoEventCount1(0),
																							   ROIVetoEventCount1(0),
																							   VetoEventCount2(0),
																							   ROIVetoEventCount2(0),
																							   VetoEventCount3(0),
																							   ROIVetoEventCount3(0),
																							   VetoEventCount4(0),
																							   ROIVetoEventCount4(0),
																							   VetoPossibleEvtCount(0),
																							   ROIVetoPossibleEvtCount(0),
																							   DetectableEventCount(0),
																							   //ImaginaryPMTPhotonCount{ImaginaryPMTPhotonCount0},
																							   runID(0),
																							   ImaginaryPMTPhotonCount0(0),
																							   ImaginaryPMTPhotonCount1(0),
																							   ImaginaryPMTPhotonCount2(0),
																							   ImaginaryPMTPhotonCount3(0),
																							   ImaginaryPMTPhotonCount4(0),
																							   ImaginaryPMTPhotonCount5(0),
																							   ImaginaryPMTPhotonCount6(0),
																							   ImaginaryPMTPhotonCount7(0),
																							   ImaginaryPMTPhotonCount8(0),
																							   ImaginaryPMTPhotonCount9(0),
																							   ImaginaryPMTPhotonCount10(0),
																							   ImaginaryPMTPhotonCount11(0),
																							   ImaginaryPMTPhotonCount12(0),
																							   ImaginaryPMTPhotonCount13(0),
																							   ImaginaryPMTPhotonCount14(0),
																							   ImaginaryPMTPhotonCount15(0),
																							   ImaginaryPMTPhotonCount16(0),
																							   ImaginaryPMTPhotonCount17(0),
																							   ImaginaryPMTPhotonCount18(0),
																							   ImaginaryPMTPhotonCount19(0)
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
	accumulableManager->RegisterAccumulable(VetoEventCount1);
	accumulableManager->RegisterAccumulable(ROIVetoEventCount1);
	accumulableManager->RegisterAccumulable(VetoEventCount2);
	accumulableManager->RegisterAccumulable(ROIVetoEventCount2);
	accumulableManager->RegisterAccumulable(VetoEventCount3);
	accumulableManager->RegisterAccumulable(ROIVetoEventCount3);
	accumulableManager->RegisterAccumulable(VetoEventCount4);
	accumulableManager->RegisterAccumulable(ROIVetoEventCount4);
	accumulableManager->RegisterAccumulable(VetoPossibleEvtCount);
	accumulableManager->RegisterAccumulable(ROIVetoPossibleEvtCount);
	accumulableManager->RegisterAccumulable(DetectableEventCount);

	analysisManager->SetNtupleMerging(true);
	analysisManager->SetVerboseLevel(1);
	//analysisManager->CreateH1("edepBulk", "Edep in Bulk", 200, 0 * keV, 20 * keV);
	fDetCons = det;
	fPrimaryGenerator = gen;
	filename = "Simulation Result";
	txtname = "Simulation Result";
	ifRefresh = false;
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

		analysisManager->CreateNtuple("Edep", "Edep in Bulk"); //Bulk events that deposit energy in Ar
		analysisManager->CreateNtupleDColumn(3, "Edep");
		analysisManager->FinishNtuple(3);
	}

	else if (fDetCons->GetMode() == "CDEXFiberBucketSetup" || fDetCons->GetMode() == "CDEXLightGuideBucketSetup" || fDetCons->GetMode() == "CDEXArParametersTest")
	{
		analysisManager->CreateNtuple("RunSum", "Run Summary");
		analysisManager->CreateNtupleIColumn(0, "TotalEventCount");
		analysisManager->CreateNtupleIColumn(0, "SiPMEventCount");
		analysisManager->CreateNtupleIColumn(0, "BulkEventCount");
		analysisManager->CreateNtupleIColumn(0, "ROIEventCount");
		analysisManager->CreateNtupleIColumn(0, "VetoEventCount");
		analysisManager->CreateNtupleIColumn(0, "VetoEventCount1");
		analysisManager->CreateNtupleIColumn(0, "VetoEventCount2");
		analysisManager->CreateNtupleIColumn(0, "VetoEventCount3");
		analysisManager->CreateNtupleIColumn(0, "VetoEventCount4");
		analysisManager->CreateNtupleIColumn(0, "ROIVetoEventCount");
		analysisManager->CreateNtupleIColumn(0, "ROIVetoEventCount1");
		analysisManager->CreateNtupleIColumn(0, "ROIVetoEventCount2");
		analysisManager->CreateNtupleIColumn(0, "ROIVetoEventCount3");
		analysisManager->CreateNtupleIColumn(0, "ROIVetoEventCount4");
		analysisManager->CreateNtupleIColumn(0, "VetoPossibleEventCount");
		analysisManager->CreateNtupleIColumn(0, "ROIVetoPossibleEventCount");
		analysisManager->CreateNtupleIColumn(0, "DetectableEventCount");
		analysisManager->FinishNtuple(0);

		analysisManager->CreateNtuple("PhotonInfo", "Recorded Photon Info");
		analysisManager->CreateNtupleIColumn(1, "EventID");
		analysisManager->CreateNtupleIColumn(1, "SiPMType");
		analysisManager->CreateNtupleIColumn(1, "SiPMID");
		analysisManager->CreateNtupleDColumn(1, "PosX");
		analysisManager->CreateNtupleDColumn(1, "PosY");
		analysisManager->CreateNtupleDColumn(1, "PosZ");
		analysisManager->CreateNtupleDColumn(1, "Wavelength");
		analysisManager->FinishNtuple(1);

		analysisManager->CreateNtuple("Edep", "Edep in Bulk"); //Bulk events that deposit energy in Ar
		analysisManager->CreateNtupleIColumn(2, "IfVetoDetected");
		for (G4int i = 0; i <= DETNUMBER; i++)
		{
			analysisManager->CreateNtupleDColumn(2, "Det" + std::to_string(i));
		}

		analysisManager->FinishNtuple(2);
	}
	else if (fDetCons->GetMode() == "CDEXArExpSetup")
	{
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount0);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount1);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount2);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount3);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount4);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount5);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount6);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount7);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount8);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount9);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount10);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount11);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount12);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount13);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount14);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount15);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount16);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount17);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount18);
		accumulableManager->RegisterAccumulable(ImaginaryPMTPhotonCount19);
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
	else if (fDetCons->GetMode() == "CDEXFiberBucketSetup" || fDetCons->GetMode() == "CDEXLightGuideBucketSetup")
	{
		filename = fDetCons->GetMode() + "_" + fPrimaryGenerator->GetSrcType();
		txtname = fDetCons->GetMode() + "_" + fPrimaryGenerator->GetSrcType();
		analysisManager->OpenFile(filename);
		accumulableManager->Reset();
	}
	else if (fDetCons->GetMode() == "CDEXArParametersTest")
	{
		//filename = fDetCons->GetMode() + "_" + fPrimaryGenerator->GetSrcType() + "_Abs_" + std::to_string(G4int (fDetCons->GetArAbsLength() / cm)) + "_LYRatio_" + std::to_string(G4int( fDetCons->GetArYieldRatio() * 10));
		//txtname = fDetCons->GetMode() + "_Abs_" + std::to_string(G4int (fDetCons->GetArAbsLength() / cm)) + "_LYRatio_" + std::to_string(G4int(fDetCons->GetArYieldRatio() * 10));
		filename = fDetCons->GetMode() + "_" + fPrimaryGenerator->GetSrcType() + "_Abs_" + std::to_string(G4int(fDetCons->GetArAbsLength() / cm)) + "_LYRatio_" + std::to_string(G4int(fDetCons->GetArYieldRatio() * 10))+"_LGRadius_" + std::to_string(G4int(fDetCons->GetLightGuideRadius()/cm))+"_LGLength_" + std::to_string(G4int(fDetCons->GetLightGuideLength()/cm));
		txtname = "Res_"+fDetCons->GetMode()+"_" + fPrimaryGenerator->GetSrcType();
		analysisManager->OpenFile(filename);
		accumulableManager->Reset();
	}
	else if (fDetCons->GetMode() == "CDEXArExpSetup")
	{
		//filename = fDetCons->GetMode() + "_" + fPrimaryGenerator->GetSrcType() + "_Abs_" + std::to_string(G4int (fDetCons->GetArAbsLength() / cm)) + "_LYRatio_" + std::to_string(G4int( fDetCons->GetArYieldRatio() * 10));
		txtname = fDetCons->GetMode() + "_" + fPrimaryGenerator->GetSrcType();
		//analysisManager->OpenFile(filename);
		accumulableManager->Reset();
		//memset(ImaginaryPMTPhotonCount, 0, sizeof(ImaginaryPMTPhotonCount));
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

	else if (fDetCons->GetMode() == "CDEXFiberBucketSetup" || fDetCons->GetMode() == "CDEXLightGuideBucketSetup")
	{
		accumulableManager->Merge();
		if (G4RunManager::GetRunManager()->GetRunManagerType() != 1)
		{
			analysisManager->FillNtupleIColumn(0, 0, aRun->GetNumberOfEvent());
			analysisManager->FillNtupleIColumn(0, 1, SiPMEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 2, BulkEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 3, ROIEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 4, VetoEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 5, VetoEventCount1.GetValue());
			analysisManager->FillNtupleIColumn(0, 6, VetoEventCount2.GetValue());
			analysisManager->FillNtupleIColumn(0, 7, VetoEventCount3.GetValue());
			analysisManager->FillNtupleIColumn(0, 8, VetoEventCount4.GetValue());
			analysisManager->FillNtupleIColumn(0, 9, ROIVetoEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 10, ROIVetoEventCount1.GetValue());
			analysisManager->FillNtupleIColumn(0, 11, ROIVetoEventCount2.GetValue());
			analysisManager->FillNtupleIColumn(0, 12, ROIVetoEventCount3.GetValue());
			analysisManager->FillNtupleIColumn(0, 13, ROIVetoEventCount4.GetValue());
			analysisManager->FillNtupleIColumn(0, 14, VetoPossibleEvtCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 15, ROIVetoPossibleEvtCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 16, DetectableEventCount.GetValue());
			analysisManager->AddNtupleRow(0);
		}
		CDEXFiberBucketSetupOutput(aRun);
		G4cout << "Output Done" << G4endl;
		G4String fileName0 = filename + ".root";
		G4String fileName1 = filename + "_" + fPrimaryGenerator->GetPrimaryName() + ".root";

		analysisManager->Write();
		analysisManager->CloseFile();
		if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
		{
			std::rename(fileName0, fileName1);
		}
	}

	else if (fDetCons->GetMode() == "CDEXArParametersTest")
	{
		accumulableManager->Merge();
		if (G4RunManager::GetRunManager()->GetRunManagerType() != 1)
		{
			analysisManager->FillNtupleIColumn(0, 0, aRun->GetNumberOfEvent());
			analysisManager->FillNtupleIColumn(0, 1, SiPMEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 2, BulkEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 3, ROIEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 4, VetoEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 5, VetoEventCount1.GetValue());
			analysisManager->FillNtupleIColumn(0, 6, VetoEventCount2.GetValue());
			analysisManager->FillNtupleIColumn(0, 7, VetoEventCount3.GetValue());
			analysisManager->FillNtupleIColumn(0, 8, VetoEventCount4.GetValue());
			analysisManager->FillNtupleIColumn(0, 9, ROIVetoEventCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 10, ROIVetoEventCount1.GetValue());
			analysisManager->FillNtupleIColumn(0, 11, ROIVetoEventCount2.GetValue());
			analysisManager->FillNtupleIColumn(0, 12, ROIVetoEventCount3.GetValue());
			analysisManager->FillNtupleIColumn(0, 13, ROIVetoEventCount4.GetValue());
			analysisManager->FillNtupleIColumn(0, 14, VetoPossibleEvtCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 15, ROIVetoPossibleEvtCount.GetValue());
			analysisManager->FillNtupleIColumn(0, 16, DetectableEventCount.GetValue());
			analysisManager->AddNtupleRow(0);
		}
		CDEXArParametersTestOutput(aRun);
		G4cout << "Output Done" << G4endl;
		G4String fileName0 = filename + ".root";
		G4String fileName1 = filename + "_" + fPrimaryGenerator->GetPrimaryName() + ".root";

		analysisManager->Write();
		analysisManager->CloseFile();
		if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
		{
			std::rename(fileName0, fileName1);
			G4cout << fileName1 << G4endl;
		}
	}
	else if (fDetCons->GetMode() == "CDEXArExpSetup")
	{
		accumulableManager->Merge();
		// for (G4int i = 0; i < 20; i++)
		// {
		// 	G4cout << ImaginaryPMTPhotonCount[i].GetValue() << " ";
		// }
		// G4cout << G4endl;
		CDEXArExpOutput(aRun);
	}

	else
	{
		G4cout << "ERROR! Mode does not exsist, nothing to output!" << G4endl;
	}

	ifRefresh = false;
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
		G4cout << "ROIEventCount =" << ROIEventCount.GetValue() << G4endl;
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
				<< std::setw(40) << std::left << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() / (1 * eV) << '\t'
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
				<< std::setw(40) << std::left << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() / (1 * eV) << '\t'
				<< std::setw(40) << std::left << BulkEventCount.GetValue() << '\t'
				<< std::setw(40) << std::left << ROIEventCount.GetValue() << G4endl;
			output.close();
			//std::DecimalFormat df1 = new DecimalFormat("0.0");
		}
	}
}

void CDEXRunAction::CDEXFiberBucketSetupOutput(const G4Run *aRun)
{
	if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
	{
		G4cout << "End Running ..." << G4endl;
		G4cout << "==========================Run Summary==========================" << G4endl;
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "Run" << aRun->GetRunID() << " Finished" << G4endl;
		G4cout << "TotalEvent =" << aRun->GetNumberOfEvent() << G4endl;
		G4cout << "DetectableEvent =" << DetectableEventCount.GetValue() << G4endl;
		G4cout << "SiPMEvent =" << SiPMEventCount.GetValue() << G4endl;
		G4cout << "BulkEventCount =" << BulkEventCount.GetValue() << G4endl;
		G4cout << "VetoEventCount =" << VetoEventCount.GetValue() << G4endl;
		G4cout << "VetoEventCount1 =" << VetoEventCount1.GetValue() << G4endl;
		G4cout << "VetoEventCount2 =" << VetoEventCount2.GetValue() << G4endl;
		G4cout << "VetoEventCount3 =" << VetoEventCount3.GetValue() << G4endl;
		G4cout << "VetoEventCount4 =" << VetoEventCount4.GetValue() << G4endl;
		G4cout << "ROIEventCount =" << ROIEventCount.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount =" << ROIVetoEventCount.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount1 =" << ROIVetoEventCount1.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount2 =" << ROIVetoEventCount2.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount3 =" << ROIVetoEventCount3.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount4 =" << ROIVetoEventCount4.GetValue() << G4endl;
		G4cout << "VetoPossibleEventCount =" << VetoPossibleEvtCount.GetValue() << G4endl;
		G4cout << "ROIVetoPossibleEventCount =" << ROIVetoPossibleEvtCount.GetValue() << G4endl;
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
				<< "Source Type: " << fPrimaryGenerator->GetSrcType() << G4endl;
		}
		else
		{
			output.open(txtname + ".txt", std::ios::app);
		}
		G4double eff;

		if (aRun->GetRunID() == 0)
		{
			output
				<< std::setw(10) << std::left << "Run ID" << '\t'
				<< std::setw(40) << std::left << "Number of Event" << '\t'
				<< std::setw(40) << std::left << "Primary Particle" << '\t'
				<< std::setw(40) << std::left << "Primary Energy(MeV)" << '\t'
				<< std::setw(40) << std::left << "Reflector Type" << '\t'
				<< std::setw(40) << std::left << "DetectableEvent" << '\t'
				<< std::setw(40) << std::left << "SiPMEvent" << '\t'
				<< std::setw(40) << std::left << "BulkEvent" << '\t'
				<< std::setw(40) << std::left << "VetoEvent" << '\t'
				<< std::setw(40) << std::left << "VetoEvent1" << '\t'
				<< std::setw(40) << std::left << "VetoEvent2" << '\t'
				<< std::setw(40) << std::left << "VetoEvent3" << '\t'
				<< std::setw(40) << std::left << "VetoEvent4" << '\t'
				<< std::setw(40) << std::left << "VetoPossibleEvent" << '\t'
				<< std::setw(40) << std::left << "ROIEvent" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent1" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent2" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent3" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent4" << '\t'
				<< std::setw(40) << std::left << "ROIVetoPossibleEvent" << G4endl;
		}

		output
			<< std::setw(10) << std::left << aRun->GetRunID() << '\t'
			<< std::setw(40) << std::left << aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << fPrimaryGenerator->GetPrimaryName() << '\t'
			<< std::setw(40) << std::left << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() << '\t'
			<< std::setw(40) << std::left << fDetCons->GetReflectorType() << '\t'
			<< std::setw(40) << std::left << DetectableEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << SiPMEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << BulkEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount1.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount2.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount3.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount4.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount1.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount2.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount3.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount4.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoPossibleEvtCount.GetValue() << G4endl;
		output.close();
		//std::DecimalFormat df1 = new DecimalFormat("0.0");
	}
}

void CDEXRunAction::CDEXArParametersTestOutput(const G4Run *aRun)
{
	if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
	{
		G4cout << "End Running ..." << G4endl;
		G4cout << "==========================Run Summary==========================" << G4endl;
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "Run" << aRun->GetRunID() << " Finished" << G4endl;
		G4cout << "TotalEvent =" << aRun->GetNumberOfEvent() << G4endl;
		G4cout << "DetectableEvent =" << DetectableEventCount.GetValue() << G4endl;
		G4cout << "SiPMEvent =" << SiPMEventCount.GetValue() << G4endl;
		G4cout << "BulkEventCount =" << BulkEventCount.GetValue() << G4endl;
		G4cout << "VetoEventCount =" << VetoEventCount.GetValue() << G4endl;
		G4cout << "VetoEventCount1 =" << VetoEventCount1.GetValue() << G4endl;
		G4cout << "VetoEventCount2 =" << VetoEventCount2.GetValue() << G4endl;
		G4cout << "VetoEventCount3 =" << VetoEventCount3.GetValue() << G4endl;
		G4cout << "VetoEventCount4 =" << VetoEventCount4.GetValue() << G4endl;
		G4cout << "ROIEventCount =" << ROIEventCount.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount =" << ROIVetoEventCount.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount1 =" << ROIVetoEventCount1.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount2 =" << ROIVetoEventCount2.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount3 =" << ROIVetoEventCount3.GetValue() << G4endl;
		G4cout << "ROIVetoEventCount4 =" << ROIVetoEventCount4.GetValue() << G4endl;
		G4cout << "VetoPossibleEventCount =" << VetoPossibleEvtCount.GetValue() << G4endl;
		G4cout << "ROIVetoPossibleEventCount =" << ROIVetoPossibleEvtCount.GetValue() << G4endl;
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "===============================================================" << G4endl;

		std::ofstream output;
		//if (aRun->GetRunID() == 0)
		if (GetRefresh() == true)
		{
			output.open(txtname + ".txt", std::ios::ate);
			output
				<< "Simulation Result" << G4endl;
		}
		else
		{
			output.open(txtname + ".txt", std::ios::app);
		}
		G4double eff;

		//if (aRun->GetRunID() == 0)
		if (GetRefresh() == true)
		{
			output
				<< std::setw(10) << std::left << "Run ID" << '\t'
				<< std::setw(40) << std::left << "Number of Event" << '\t'
				<< std::setw(40) << std::left << "Source Type" << '\t'
				<< std::setw(40) << std::left << "Primary Particle" << '\t'
				<< std::setw(40) << std::left << "Primary Energy(MeV)" << '\t'
				<< std::setw(40) << std::left << "Reflector Type" << '\t'
				<< std::setw(40) << std::left << "Ar Abs Length(cm)" << '\t'
				<< std::setw(40) << std::left << "Ar LY Ratio" << '\t'
				<< std::setw(40) << std::left << "Light Guide Length" << '\t'
				<< std::setw(40) << std::left << "Light Guide Radius" << '\t'
				<< std::setw(40) << std::left << "DetectableEvent" << '\t'
				<< std::setw(40) << std::left << "SiPMEvent" << '\t'
				<< std::setw(40) << std::left << "BulkEvent" << '\t'
				<< std::setw(40) << std::left << "VetoEvent" << '\t'
				<< std::setw(40) << std::left << "VetoEvent1" << '\t'
				<< std::setw(40) << std::left << "VetoEvent2" << '\t'
				<< std::setw(40) << std::left << "VetoEvent3" << '\t'
				<< std::setw(40) << std::left << "VetoEvent4" << '\t'
				<< std::setw(40) << std::left << "VetoPossibleEvent" << '\t'
				<< std::setw(40) << std::left << "ROIEvent" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent1" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent2" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent3" << '\t'
				<< std::setw(40) << std::left << "ROIVetoEvent4" << '\t'
				<< std::setw(40) << std::left << "ROIVetoPossibleEvent" << G4endl;
		}

		output
			<< std::setw(10) << std::left << aRun->GetRunID() << '\t'
			<< std::setw(40) << std::left << aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << fPrimaryGenerator->GetSrcType() << '\t'
			<< std::setw(40) << std::left << fPrimaryGenerator->GetPrimaryName() << '\t'
			<< std::setw(40) << std::left << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() << '\t'
			<< std::setw(40) << std::left << fDetCons->GetReflectorType() << '\t'
			<< std::setw(40) << std::left << fDetCons->GetArAbsLength() / cm << '\t'
			<< std::setw(40) << std::left << fDetCons->GetArYieldRatio() << '\t'
			<< std::setw(40) << std::left << fDetCons->GetLightGuideLength() << '\t'
			<< std::setw(40) << std::left << fDetCons->GetLightGuideRadius() << '\t'
			<< std::setw(40) << std::left << DetectableEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << SiPMEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << BulkEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount1.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount2.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount3.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoEventCount4.GetValue() << '\t'
			<< std::setw(40) << std::left << VetoPossibleEvtCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount1.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount2.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount3.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoEventCount4.GetValue() << '\t'
			<< std::setw(40) << std::left << ROIVetoPossibleEvtCount.GetValue() << G4endl;
		output.close();
		//std::DecimalFormat df1 = new DecimalFormat("0.0");
	}
}

void CDEXRunAction::CDEXArExpOutput(const G4Run *aRun)
{

	if (G4RunManager::GetRunManager()->GetRunManagerType() == 1)
	{
		G4cout << "End Running ..." << G4endl;
		G4cout << "==========================Run Summary==========================" << G4endl;
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "Run" << aRun->GetRunID() << " Finished" << G4endl;
		G4cout << "TotalEvent =" << aRun->GetNumberOfEvent() << G4endl;
		G4cout << "AbsLength =" << fDetCons->GetArAbsLength() / cm << G4endl;
		G4cout << "LYRatio =" << fDetCons->GetArYieldRatio() << G4endl;
		// for (G4int i = 0; i < 20; i++)
		// {
		// 	G4cout << "ImaginaryPMTPhotonCount" + std::to_string(i) + "=" << ImaginaryPMTPhotonCount[i].GetValue() << G4endl;
		// }
		G4cout << G4endl;
		G4cout << G4endl;
		G4cout << "===============================================================" << G4endl;

		std::ofstream output;
		if (aRun->GetRunID() == 0)
		{
			output.open(txtname + ".txt", std::ios::ate);
			output
				<< "Simulation Result" << G4endl
				<< "Source Type: " << fPrimaryGenerator->GetSrcType() << G4endl;
		}
		else
		{
			output.open(txtname + ".txt", std::ios::app);
		}
		G4double eff;

		if (aRun->GetRunID() == 0)
		{
			output
				<< std::setw(10) << std::left << "Run ID" << '\t'
				<< std::setw(40) << std::left << "Number of Event" << '\t'
				<< std::setw(40) << std::left << "Primary Particle" << '\t'
				<< std::setw(40) << std::left << "Primary Energy(MeV)" << '\t'
				<< std::setw(40) << std::left << "Ar Abs Length(cm)" << '\t'
				<< std::setw(40) << std::left << "Ar LY Ratio" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount0" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount1" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount2" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount3" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount4" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount5" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount6" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount7" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount8" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount9" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount10" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount11" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount12" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount13" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount14" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount15" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount16" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount17" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount18" << '\t'
				<< std::setw(40) << std::left << "ImaginaryPMTPhotonCount19" << G4endl;
		}

		output
			<< std::setw(10) << std::left << aRun->GetRunID() << '\t'
			<< std::setw(40) << std::left << aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << fPrimaryGenerator->GetPrimaryName() << '\t'
			<< std::setw(40) << std::left << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fPrimaryGenerator->GetPrimaryE() << '\t'
			<< std::setw(40) << std::left << fDetCons->GetArAbsLength() / cm << '\t'
			<< std::setw(40) << std::left << fDetCons->GetArYieldRatio() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount0.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount1.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount2.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount3.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount4.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount5.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount6.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount7.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount8.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount9.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount10.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount11.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount12.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount13.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount14.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount15.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount16.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount17.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount18.GetValue() / aRun->GetNumberOfEvent() << '\t'
			<< std::setw(40) << std::left << ImaginaryPMTPhotonCount19.GetValue() / aRun->GetNumberOfEvent() << G4endl;
		output.close();
		//std::DecimalFormat df1 = new DecimalFormat("0.0");
	}
}