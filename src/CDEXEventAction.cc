//CDEXEventAction.cc
#include <string.h>
#include "CDEXEventAction.hh"

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
	: edepBulk(0.),
	  SiPMPhotonCount{0},
	  SiPMSignalCount{0},
	  Total(0),
	  ID(0),
	  ifSiPM(false),
	  ifFiber(false),
	  run(runaction),
	  CDEXCons(detcons)
//ResultFile("Distribution_Results_NTuple.root","RECREATE"),
//Distribution_Results("Distribution_Results","Distribution_Results")
{
	SignalSiPMCount = 0;
	EnergyThreshold = 160 * eV;
	DepositeID = 0;
	RowNb = sizeof(SiPMPhotonCount) / sizeof(SiPMPhotonCount[0]);
	ColumnNb = sizeof(SiPMPhotonCount[0]) / sizeof(SiPMPhotonCount[0][0]);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXEventAction::~CDEXEventAction()
{
	//Distribution_Results.Write();
	//ResultFile.Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXEventAction::BeginOfEventAction(const G4Event *evt)
{
	if (CDEXCons->GetMode() == "CDEX300")
	{
		edepBulk = 0;
		memset(SiPMPhotonCount, 0, sizeof(SiPMPhotonCount));
		memset(SiPMSignalCount, 0, sizeof(SiPMPhotonCount));

		Total = 0;
		TotalSiPMPhotonCount = 0;
		SignalSiPMCount = 0;
		MinSignalSiPMCount = 1;

		ifSiPM = false;
		ifFiber = false;
		DepositeInfoInScintillator.clear();
		EdepInfoInScintillator.clear();
		TempPosList.clear();
		TempPosListInScintillator.clear();
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXEventAction::EndOfEventAction(const G4Event *evt)
{
	G4int EventID = evt->GetEventID();
	if (EventID % 5000 == 0)
	{
		G4cout << EventID << G4endl;
	}

	if (CDEXCons->GetMode() == "CDEX300")
	{
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->FillNtupleDColumn(3, 0, edepBulk);
		analysisManager->AddNtupleRow(3);
		if (edepBulk > 160 * eV)
		{
			run->CountBulkEvent();
			if (ifDetectable == true)
			{
				run->CountVetoPossibleEvent();
			}
		}

		if (edepBulk > 2000 * keV && edepBulk < 2100 * keV)
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

				if (edepBulk > 160 * eV)
				{
					analysisManager->FillNtupleIColumn(2, 0, EdepInfoInScintillator[i][0]);
					analysisManager->FillNtupleIColumn(2, 1, EdepInfoInScintillator[i][1]);
					analysisManager->FillNtupleDColumn(2, 2, EdepInfoInScintillator[i][2]);
					analysisManager->FillNtupleDColumn(2, 3, EdepInfoInScintillator[i][3]);
					analysisManager->FillNtupleDColumn(2, 4, EdepInfoInScintillator[i][4]);
					analysisManager->FillNtupleDColumn(2, 5, EdepInfoInScintillator[i][5]);
					analysisManager->FillNtupleIColumn(2, 6, EventID);
					analysisManager->AddNtupleRow(2);
				}
			}
		}
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXEventAction::AddBulkEnergy(G4double de)
{
	edepBulk += de;
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