// CDEXEventAction.cc
#include <string.h>
#include "CDEXEventAction.hh"
#include "CDEXRunAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXEventAction::CDEXEventAction(CDEXRunAction *runaction, CDEXDetectorConstruction *detcons)
	: EdepBulk(0.),
	  EdepBulkDet{0},
	  SiPMPhotonCount{0},
	  SiPMSignalCount{0},
	  Total(0),
	  ID(0),
	  ifSiPM(false),
	  ifFiber(false),
	  ifBulk(false),
	  ifROI(false),
	  run(runaction),
	  CDEXCons(detcons)
// ResultFile("Distribution_Results_NTuple.root","RECREATE"),
// Distribution_Results("Distribution_Results","Distribution_Results")
{
	SignalSiPMCount = 0;
	SiPMVetoThreshold = 1;
	SiPMVetoThreshold1 = 2;
	SiPMVetoThreshold2 = 3;
	SiPMVetoThreshold3 = 4;
	SiPMVetoThreshold4 = 5;
	EnergyThreshold = 160 * eV;
	DepositeID = 0;
	fEventID = 0;
	RowNb = sizeof(SiPMPhotonCount) / sizeof(SiPMPhotonCount[0]);
	ColumnNb = sizeof(SiPMPhotonCount[0]) / sizeof(SiPMPhotonCount[0][0]);
	DetNumber = DETNUMBER;
	MinSignalSiPMCount = 1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXEventAction::~CDEXEventAction()
{
	// Distribution_Results.Write();
	// ResultFile.Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXEventAction::BeginOfEventAction(const G4Event *evt)
{
	fEventID = evt->GetEventID();
	if (CDEXCons->GetMode() == "CDEX300")
	{
		EdepBulk = 0;
		memset(EdepBulkDet, 0, sizeof(EdepBulkDet));
		memset(EdepBulkString, 0, sizeof(EdepBulkString));
		memset(SiPMPhotonCount, 0, sizeof(SiPMPhotonCount));
		memset(SiPMSignalCount, 0, sizeof(SiPMPhotonCount));

		Total = 0;
		TotalSiPMPhotonCount = 0;
		SignalSiPMCount = 0;

		ifBulk = false;
		ifROI = false;
		ifDetectable = false;
		ifSiPM = false;
		ifFiber = false;
		DepositeInfoInScintillator.clear();
		EdepInfoInScintillator.clear();
		TempPosList.clear();
		TempPosListInScintillator.clear();
	}
	else if (CDEXCons->GetMode() == "CDEXFiberBucketSetup" || CDEXCons->GetMode() == "CDEXLightGuideBucketSetup" || CDEXCons->GetMode() == "CDEXArParametersTest")
	{
		EdepBulk = 0;
		memset(EdepBulkDet, 0, sizeof(EdepBulkDet));
		memset(SiPMPhotonCount, 0, sizeof(SiPMPhotonCount));
		memset(SiPMSignalCount, 0, sizeof(SiPMPhotonCount));

		TempSiPMList.clear();
		BulkEdepInfo.clear();
		Total = 0;
		TotalSiPMPhotonCount = 0;
		SiPMSignalCnt = 0;
		SignalSiPMCount = 0;

		ifBulk = false;
		ifROI = false;
		ifDetectable = false;
		ifSiPM = false;
		ifFiber = false;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXEventAction::EndOfEventAction(const G4Event *evt)
{
	if (fEventID % 5000 == 0)
	{
		G4cout << fEventID << G4endl;
	}
	// G4cout<<"ID:"<<fEventID<<" E:"<<EdepBulk<<G4endl;
	if (CDEXCons->GetMode() == "CDEX300")
	{
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->FillNtupleDColumn(3, 0, EdepBulk);
		analysisManager->AddNtupleRow(3);

		// Accurate but slow
		// GetEdepStatus();
		// Not Accurate
		if (EdepBulk > 160 * eV)
		{
			ifBulk = true;
		}
		if (EdepBulk > 2000 * eV && EdepBulk < 2100 * eV)
		{
			ifROI = true;
		}

		if (ifBulk == true)
		{
			run->CountBulkEvent();
			if (ifDetectable == true)
			{
				run->CountVetoPossibleEvent();
			}
		}

		if (ifROI == true)
		{
			run->CountROIEvent();
			if (ifDetectable == true)
			{
				run->CountROIVetoPossibleEvent();
			}
		}

		if (EdepInfoInScintillator.empty() == false)
		{
			for (G4int i = 0; i < EdepInfoInScintillator.size(); i++)
			{
				// analysisManager->FillNtupleIColumn(1, 0, EdepInfoInScintillator[i][0]);
				// analysisManager->FillNtupleIColumn(1, 1, EdepInfoInScintillator[i][1]);
				// analysisManager->FillNtupleDColumn(1, 2, EdepInfoInScintillator[i][2]);
				// analysisManager->FillNtupleDColumn(1, 3, EdepInfoInScintillator[i][3]);
				// analysisManager->FillNtupleDColumn(1, 4, EdepInfoInScintillator[i][4]);
				// analysisManager->FillNtupleDColumn(1, 5, EdepInfoInScintillator[i][5]);
				// analysisManager->FillNtupleIColumn(1, 6, EventID);
				// analysisManager->AddNtupleRow(1);

				if (EdepBulk > 160 * eV)
				{
					analysisManager->FillNtupleIColumn(2, 0, EdepInfoInScintillator[i][0]);
					analysisManager->FillNtupleIColumn(2, 1, EdepInfoInScintillator[i][1]);
					analysisManager->FillNtupleDColumn(2, 2, EdepInfoInScintillator[i][2]);
					analysisManager->FillNtupleDColumn(2, 3, EdepInfoInScintillator[i][3]);
					analysisManager->FillNtupleDColumn(2, 4, EdepInfoInScintillator[i][4]);
					analysisManager->FillNtupleDColumn(2, 5, EdepInfoInScintillator[i][5]);
					analysisManager->FillNtupleIColumn(2, 6, fEventID);
					analysisManager->AddNtupleRow(2);
				}
			}
		}
	}
	if (CDEXCons->GetMode() == "CDEXFiberBucketSetup" || CDEXCons->GetMode() == "CDEXLightGuideBucketSetup" || CDEXCons->GetMode() == "CDEXArParametersTest")
	{
		auto analysisManager = G4AnalysisManager::Instance();

		// Accurate but slow method:
		// GetEdepStatus();

		// Not accurate:
		if (EdepBulk > 160 * eV)
		{
			ifBulk = true;
		}
		if (EdepBulk > 10 * eV)
		{
			if (SignalSiPMCount >= SiPMVetoThreshold)
			{
				analysisManager->FillNtupleIColumn(2, 0, 1);
			}
			else
			{
				analysisManager->FillNtupleIColumn(2, 0, 0);
			}

			if (SignalSiPMCount >= SiPMVetoThreshold1)
			{
				analysisManager->FillNtupleIColumn(2, 1, 1);
			}
			else
			{
				analysisManager->FillNtupleIColumn(2, 1, 0);
			}

			if (SignalSiPMCount >= SiPMVetoThreshold2)
			{
				analysisManager->FillNtupleIColumn(2, 2, 1);
			}
			else
			{
				analysisManager->FillNtupleIColumn(2, 2, 0);
			}

			if (SignalSiPMCount >= SiPMVetoThreshold3)
			{
				analysisManager->FillNtupleIColumn(2, 3, 1);
			}
			else
			{
				analysisManager->FillNtupleIColumn(2, 3, 0);
			}

			if (IfMultiSiteEvent(BulkEdepInfo, 1 * mm, 0.9) == true)
			{
				analysisManager->FillNtupleIColumn(2, 4, 1);
			}
			else
			{
				analysisManager->FillNtupleIColumn(2, 4, 0);
			}

			if (IfMultiSiteEvent(BulkEdepInfo, 1 * mm, 0.8) == true)
			{
				analysisManager->FillNtupleIColumn(2, 5, 1);
			}
			else
			{
				analysisManager->FillNtupleIColumn(2, 5, 0);
			}

			if (IfMultiSiteEvent(BulkEdepInfo, 1 * mm, 0.7) == true)
			{
				analysisManager->FillNtupleIColumn(2, 6, 1);
			}
			else
			{
				analysisManager->FillNtupleIColumn(2, 6, 0);
			}

			if (IfMultiSiteEvent(BulkEdepInfo, 2 * mm, 0.7) == true)
			{
				analysisManager->FillNtupleIColumn(2, 7, 1);
			}
			else
			{
				analysisManager->FillNtupleIColumn(2, 7, 0);
			}

			if (IfInterDet(1 * keV, EdepBulkDet) == true)
			{
				analysisManager->FillNtupleIColumn(2, 8, 1);
			}
			else
			{
				analysisManager->FillNtupleIColumn(2, 8, 0);
			}

			analysisManager->FillNtupleDColumn(2, 9, EdepBulk);
			for (G4int i = 0; i < STRINGNUMBER; i++)
			{
				analysisManager->FillNtupleDColumn(2, i + 10, EdepBulkString[i]);
			}

			analysisManager->AddNtupleRow(2);
		}

		if (EdepBulk > 2000 * eV && EdepBulk < 2100 * eV)
		{
			ifROI = true;
		}

		if (ifSiPM == true)
		{
			run->CountSiPMEvent();
		}
		if (ifDetectable == true)
		{
			run->CountDetectableEvent();
		}
		if (ifBulk == true)
		{
			run->CountBulkEvent();
			if (ifDetectable == true)
			{
				run->CountVetoPossibleEvent();
			}
			if (SignalSiPMCount >= SiPMVetoThreshold)
			{
				run->CountVetoEvent(0);
			}
			if (SignalSiPMCount >= SiPMVetoThreshold1)
			{
				run->CountVetoEvent(1);
			}
			if (SignalSiPMCount >= SiPMVetoThreshold2)
			{
				run->CountVetoEvent(2);
			}
			if (SignalSiPMCount >= SiPMVetoThreshold3)
			{
				run->CountVetoEvent(3);
			}
			if (SignalSiPMCount >= SiPMVetoThreshold4)
			{
				run->CountVetoEvent(4);
			}
		}

		if (ifROI == true)
		{
			run->CountROIEvent();
			if (ifDetectable == true)
			{
				run->CountROIVetoPossibleEvent();
			}
			if (SignalSiPMCount >= SiPMVetoThreshold)
			{
				run->CountROIVetoEvent(0);
			}
			if (SignalSiPMCount >= SiPMVetoThreshold1)
			{
				run->CountROIVetoEvent(1);
			}
			if (SignalSiPMCount >= SiPMVetoThreshold2)
			{
				run->CountROIVetoEvent(2);
			}
			if (SignalSiPMCount >= SiPMVetoThreshold3)
			{
				run->CountROIVetoEvent(3);
			}
			if (SignalSiPMCount >= SiPMVetoThreshold4)
			{
				run->CountROIVetoEvent(4);
			}
		}
		// for(G4int i=0;i<DETNUMBER;i++){
		// 	G4cout<<EdepBulkDet[i]<<" ";
		// }
		// G4cout<<G4endl;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXEventAction::AddBulkEnergy(G4double de)
{
	EdepBulk += de;
}

void CDEXEventAction::AddBulkEnergyDet(G4double de, G4int DetID)
{
	if (DetID < DETNUMBER)
	{
		EdepBulkDet[DetID] += de;
	}
}

void CDEXEventAction::AddBulkEnergyString(G4double de, G4int StrID)
{
	if (StrID < STRINGNUMBER)
	{
		EdepBulkString[StrID] += de;
	}
}

void CDEXEventAction::AddToSiPM(G4int i, G4int j)
{
	Total++;
	SiPMPhotonCount[i][j]++;
}

void CDEXEventAction::AddToSiPMSignal(G4int i, G4int j)
{
	SiPMSignalCount[i][j]++;
}

G4double CDEXEventAction::GetDistance(G4double x0, G4double y0, G4double z0, G4double x1, G4double y1, G4double z1)
{
	G4double res;
	res = std::sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) + (z0 - z1) * (z0 - z1));
	return res;
}

void CDEXEventAction::RecordStepInfoInScintillator(G4int particletype, G4int creatorprocess, G4double posx, G4double posy, G4double posz, G4double edep)
{
	std::vector<G4double> StepInfo;
	StepInfo.push_back(particletype);
	StepInfo.push_back(creatorprocess);
	StepInfo.push_back(posx);
	StepInfo.push_back(posy);
	StepInfo.push_back(posz);
	StepInfo.push_back(edep);

	DepositeInfoInScintillator.push_back(StepInfo);
	StepInfo.clear();
}

void CDEXEventAction::RecordEdepInfoInScintillator(G4int particletype, G4int creatorprocess, G4double posx, G4double posy, G4double posz, G4double edep)
{

	std::vector<G4double> StepInfo;
	StepInfo.push_back(particletype);
	StepInfo.push_back(creatorprocess);
	StepInfo.push_back(posx);
	StepInfo.push_back(posy);
	StepInfo.push_back(posz);
	StepInfo.push_back(edep);
	std::vector<G4double> Pos;
	Pos.push_back(posx);
	Pos.push_back(posy);
	Pos.push_back(posz);

	if (TempPosListInScintillator.empty())
	{
		TempPosListInScintillator.push_back(Pos);
		EdepInfoInScintillator.push_back(StepInfo);
	}
	else
	{
		G4bool ifNewPoint = true;
		G4int PointID;
		for (G4int i = 0; i < TempPosListInScintillator.size(); i++)
		{
			G4double dist = GetDistance(TempPosListInScintillator[i][0], TempPosListInScintillator[i][1], TempPosListInScintillator[i][2], Pos[0], Pos[1], Pos[2]);
			if (dist < 5 * mm)
			{
				ifNewPoint = false;
				PointID = i;
			}
		}
		if (ifNewPoint == true)
		{
			TempPosListInScintillator.push_back(Pos);
			EdepInfoInScintillator.push_back(StepInfo);
		}
		else
		{
			EdepInfoInScintillator[PointID][5] += edep;
			if (particletype == 1)
			{
				EdepInfoInScintillator[PointID][0] == 1;
			}
		}
	}

	StepInfo.clear();
	Pos.clear();
}

G4bool CDEXEventAction::IfMultiSiteEvent(std::vector<std::vector<G4double>> edepinfo, G4double distcut, G4double Ecut)
{
	G4bool ifMultiSite = true;
	std::vector<std::vector<G4double>> TempBulkEdepInfoList;
	G4double TotalEdep = 0;
	for (G4int j = 0; j < edepinfo.size(); j++)
	{
		std::vector<G4double> OneEdepInfo;
		OneEdepInfo = edepinfo[j];
		TotalEdep += OneEdepInfo[3];
		if (TempBulkEdepInfoList.empty())
		{
			TempBulkEdepInfoList.push_back(OneEdepInfo);
		}
		else
		{
			G4bool ifNewPoint = true;
			G4int PointID;
			G4double MinDist = distcut;
			for (G4int i = 0; i < TempBulkEdepInfoList.size(); i++)
			{
				G4double dist = GetDistance(TempBulkEdepInfoList[i][0], TempBulkEdepInfoList[i][1], TempBulkEdepInfoList[i][2], OneEdepInfo[0], OneEdepInfo[1], OneEdepInfo[2]);
				if (dist < MinDist)
				{
					MinDist = dist;
					PointID = i;
				}
			}
			if (MinDist < distcut)
			{
				ifNewPoint = false;
			}
			if (ifNewPoint == true)
			{
				TempBulkEdepInfoList.push_back(OneEdepInfo);
			}
			else
			{
				TempBulkEdepInfoList[PointID][3] += OneEdepInfo[3];
			}
		}
	}
	for (G4int i = 0; i < TempBulkEdepInfoList.size(); i++)
	{
		if (TempBulkEdepInfoList[i][3] / TotalEdep > Ecut)
		{
			ifMultiSite = false;
			break;
		}
	}
	// if(TempBulkEdepInfoList.size()==1){
	// 	ifSingleSite = true;
	// }
	// G4cout<<"F:"<<ifSingleSite<<G4endl;
	return ifMultiSite;
}

G4bool CDEXEventAction::IfInterDet(G4double Ecut, G4double EdepList[])
{
	G4int SignalCount = 0;
	G4bool ifInterDet = false;
	G4int listlen = sizeof(EdepList) / sizeof(EdepList[0]);
	for (G4int i = 0; i < listlen; i++)
	{
		if (EdepList[i] > Ecut)
		{
			SignalCount += 1;
		}
	}
	if (SignalCount > 1)
	{
		ifInterDet = true;
	}
	return ifInterDet;
}

void CDEXEventAction::GetEdepStatus()
{
	for (G4int i = 0; i < DETNUMBER; i++)
	{
		if (EdepBulkDet[i] > 160 * eV)
		{
			ifBulk = true;
		}
		if (EdepBulkDet[i] > 2000 * eV && EdepBulkDet[i] < 2100 * eV)
		{
			ifROI = true;
		}
	}
}

void CDEXEventAction::CountSignalSiPM(G4int sipmtype, G4int sipmid)
{
	G4bool ifNewSiPM = true;
	std::vector<G4int> SiPMName;
	SiPMName.push_back(sipmtype);
	SiPMName.push_back(sipmid);
	if (TempSiPMList.empty())
	{
		TempSiPMList.push_back(SiPMName);
		SignalSiPMCount += 1;
	}
	else
	{
		for (G4int i = 0; i < TempSiPMList.size(); i++)
		{
			if (sipmtype == TempSiPMList[i][0] && sipmid == TempSiPMList[i][1])
			{
				ifNewSiPM = false;
			}
		}
		if (ifNewSiPM == true)
		{
			SignalSiPMCount += 1;
			TempSiPMList.push_back(SiPMName);
		}
	}
}