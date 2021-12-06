// CDEXSteppingAction.cc

#include "CDEXSteppingAction.hh"

#include "CDEXDetectorConstruction.hh"
#include "CDEXEventAction.hh"
#include "CDEXRunAction.hh"
#include "CDEXTrackingAction.hh"

#include "G4Track.hh"
#include "G4SteppingManager.hh"

#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4HadronicProcessType.hh"
#include "G4EmProcessSubType.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXSteppingAction::CDEXSteppingAction(
	CDEXTrackingAction *track, CDEXEventAction *evt, CDEXRunAction *run, CDEXDetectorConstruction *cons)
	: CDEXTrack(track), CDEXEvent(evt), CDEXRun(run), CDEXCons(cons)
{
	//PMT PDE
	G4String PMT_file="../properties/PMT.txt";
	std::ifstream Read_PMTPDE;
    Read_PMTPDE.open(PMT_file);
    if (Read_PMTPDE.is_open())
    {
		G4int PointID=0;
        while (!Read_PMTPDE.eof())
        {
            G4double wavelength, efficiency;

            Read_PMTPDE >> wavelength >> efficiency;
            if (Read_PMTPDE.eof())
            {
                break;
            }
            PMTWavelength[PointID] = wavelength;
			PMTEffciency[PointID] = efficiency;
            PointID++;
            if (PointID > (sizeof(PMTWavelength) / sizeof(PMTWavelength[0])- 1))
            {
                break;
            }
        }
    }
    else
    {
        G4cout << "Error opening file: " << PMT_file << G4endl;
    }
    Read_PMTPDE.close();

	//SiPM PDE
	G4String SiPM_file="../properties/SiPM.txt";
	std::ifstream Read_SiPMPDE;
    Read_SiPMPDE.open(SiPM_file);
    if (Read_SiPMPDE.is_open())
    {
		G4int PointID=0;
        while (!Read_SiPMPDE.eof())
        {
            G4double wavelength, efficiency;

            Read_SiPMPDE >> wavelength >> efficiency;
            if (Read_SiPMPDE.eof())
            {
                break;
            }
            SiPMWavelength[PointID] = wavelength;
			SiPMEffciency[PointID] = efficiency;
            PointID++;
            if (PointID > (sizeof(SiPMWavelength) / sizeof(SiPMWavelength[0])- 1))
            {
                break;
            }
        }
    }
    else
    {
        G4cout << "Error opening file: " << SiPM_file << G4endl;
    }
    Read_SiPMPDE.close();

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXSteppingAction::UserSteppingAction(const G4Step *aStep)
{
	if (CDEXCons->GetMode() == "CDEX300")
	{
		// G4cout << "EnableAcc: " << CDEXRun->GetAccelerate() << G4endl;
		auto volume = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
		G4ThreeVector PostStepPos = aStep->GetPostStepPoint()->GetPosition();
		G4ThreeVector PreStepPos = aStep->GetPreStepPoint()->GetPosition();
		G4LogicalVolume *logicvolume = nullptr;
		if (volume)
		{
			logicvolume = volume->GetLogicalVolume();
		}

		auto touchable = aStep->GetPostStepPoint()->GetTouchableHandle();
		auto ParticleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
		auto edep = aStep->GetTotalEnergyDeposit();
		auto KineticE = aStep->GetPreStepPoint()->GetKineticEnergy();
		auto CurrentWL = 1240 / (KineticE / (1 * eV));
		auto DeltaE = aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy();
		CDEXTrack->AddEdepTrack(edep);
		G4int TrackID = aStep->GetTrack()->GetTrackID();
		G4int ParentTrackID = aStep->GetTrack()->GetParentID();

		const CDEXDetectorConstruction *detectorConstruction = static_cast<const CDEXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

		if (volume == detectorConstruction->GetBulk())
		{
			CDEXEvent->AddBulkEnergy(edep);
		}

		G4int ParticleType = -1;
		ParticleType = GetParticleIntType(ParticleName);
		G4String CreatorProcessName = "Default";
		if (aStep->GetTrack()->GetTrackID() > 1)
		{
			CreatorProcessName = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();
		}
		G4int CreatorProcessType = -1;
		CreatorProcessType = GetCreatorProcessIntType(CreatorProcessName);
		G4String Mode = CDEXCons->GetMode();
		if (volume && logicvolume == detectorConstruction->GetArgonVolume(Mode) && edep > 1 * eV && ParticleType != 0)
		{
			CDEXEvent->DetectableTrue();
		}
		if (CreatorProcessType == 0 || TrackID == 1)
		{
			if (volume && logicvolume == detectorConstruction->GetArgonVolume(Mode) && DeltaE > 1 * eV && ParticleType != 0)
			{
				CDEXEvent->RecordEdepInfoInScintillator(ParticleType, CreatorProcessType, PostStepPos.getX(), PostStepPos.getY(), PostStepPos.getZ(), DeltaE);
			}
		}
	}

	if (CDEXCons->GetMode() == "CDEXFiberBucketSetup" || CDEXCons->GetMode() == "CDEXLightGuideBucketSetup" || CDEXCons->GetMode() == "CDEXArParametersTest")
	{

		// G4cout << "EnableAcc: " << CDEXRun->GetAccelerate() << G4endl;
		G4String Mode = CDEXCons->GetMode();

		auto analysisManager = G4AnalysisManager::Instance();
		auto volume = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
		G4ThreeVector PostStepPos = aStep->GetPostStepPoint()->GetPosition();
		G4ThreeVector PreStepPos = aStep->GetPreStepPoint()->GetPosition();
		G4LogicalVolume *logicvolume = nullptr;
		if (volume)
		{
			logicvolume = volume->GetLogicalVolume();
		}
		auto touchable = aStep->GetPostStepPoint()->GetTouchableHandle();
		auto ParticleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
		auto edep = aStep->GetTotalEnergyDeposit();
		auto KineticE = aStep->GetPreStepPoint()->GetKineticEnergy();

		auto DeltaE = aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy();
		CDEXTrack->AddEdepTrack(edep);
		G4int ParticleType = GetParticleIntType(ParticleName);
		G4int TrackID = aStep->GetTrack()->GetTrackID();
		G4int ParentTrackID = aStep->GetTrack()->GetParentID();

		const CDEXDetectorConstruction *detectorConstruction = static_cast<const CDEXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

		if (volume == detectorConstruction->GetBulk())
		{
			G4int DetID = touchable->GetCopyNumber(2);
			G4int UnitNumber=CDEXCons->GetUnitNumber();
			G4int DetStrID=std::floor(DetID/(double)UnitNumber);
			CDEXEvent->AddBulkEnergy(edep);
			CDEXEvent->AddBulkEnergyDet(edep, DetID);
			CDEXEvent->AddBulkEnergyString(edep, DetStrID);
			CDEXEvent->RecordBulkEdep(PostStepPos.getX(),PostStepPos.getY(),PostStepPos.getZ(),edep);
		}

		if (volume && logicvolume == detectorConstruction->GetArgonVolume(Mode) && edep > 1 * eV && ParticleType != 0)
		{
			CDEXEvent->DetectableTrue();
		}

		// G4cout<<Mode<<G4endl;
		// G4cout<<detectorConstruction->GetArgonVolume("2")->GetName()<<G4endl;
// For Acceleraiton
#ifdef ACCTRUE
		G4int SiPMSignalCnt = CDEXEvent->GetSiPMSignalCnt();
		G4int SignalSiPMCount = CDEXEvent->GetSignalSiPMCount();
		// if (SiPMSignalCnt>1&&ParticleName=="opticalphoton"){
		if (SignalSiPMCount >= 4 && ParticleName == "opticalphoton")
		{
			aStep->GetTrack()->SetTrackStatus(fStopAndKill);
		}
#endif

		if (ParticleName == "opticalphoton" && logicvolume != nullptr)
		{
			G4int SiPMType = 0;
			G4int SiPMID;
			G4int EvtID = CDEXEvent->GetEventID();
			auto CurrentWL = 1240 / (KineticE / (1 * eV));
			if (logicvolume == detectorConstruction->GetLogicFiberSiPM())
			{
				SiPMID = touchable->GetCopyNumber(1);
				SiPMType = 1;
				CDEXEvent->CountTotalSiPMPhoton(1);
				CDEXEvent->SiPMTrue();
				analysisManager->FillNtupleIColumn(1, 0, EvtID);
				analysisManager->FillNtupleIColumn(1, 1, SiPMType);
				analysisManager->FillNtupleIColumn(1, 2, SiPMID);
				analysisManager->FillNtupleDColumn(1, 3, PostStepPos.getX());
				analysisManager->FillNtupleDColumn(1, 4, PostStepPos.getY());
				analysisManager->FillNtupleDColumn(1, 5, PostStepPos.getZ());
				analysisManager->FillNtupleDColumn(1, 6, CurrentWL);
				analysisManager->AddNtupleRow(1);
				G4double SiPMEff = GetSiPMEfficiency(CurrentWL);
				//G4cout<<"WL:"<<CurrentWL<<"Eff:"<< SiPMEff<<G4endl;
				G4double rnd = G4UniformRand();
				if (rnd < SiPMEff)
				{
					CDEXEvent->CountSiPMSignal();
					CDEXEvent->CountSignalSiPM(SiPMType, SiPMID);
				}
				aStep->GetTrack()->SetTrackStatus(fStopAndKill);
			}
			else if (logicvolume == detectorConstruction->GetLogicArVolumeSiPM())
			{
				SiPMID = touchable->GetCopyNumber(1);
				SiPMType = 2;
				CDEXEvent->CountTotalSiPMPhoton(1);
				CDEXEvent->SiPMTrue();
				analysisManager->FillNtupleIColumn(1, 0, EvtID);
				analysisManager->FillNtupleIColumn(1, 1, SiPMType);
				analysisManager->FillNtupleIColumn(1, 2, SiPMID);
				analysisManager->FillNtupleDColumn(1, 3, PostStepPos.getX());
				analysisManager->FillNtupleDColumn(1, 4, PostStepPos.getY());
				analysisManager->FillNtupleDColumn(1, 5, PostStepPos.getZ());
				analysisManager->FillNtupleDColumn(1, 6, CurrentWL);
				analysisManager->AddNtupleRow(1);
				G4double SiPMEff = GetPMTEfficiency(CurrentWL);
				G4double rnd = G4UniformRand();
				if (rnd < SiPMEff)
				{
					CDEXEvent->CountSiPMSignal();
					CDEXEvent->CountSignalSiPM(SiPMType, SiPMID);
				}
				aStep->GetTrack()->SetTrackStatus(fStopAndKill);
			}
		}
	}

	if (CDEXCons->GetMode() == "CDEXArExpSetup")
	{
		// G4cout << "EnableAcc: " << CDEXRun->GetAccelerate() << G4endl;
		G4String Mode = CDEXCons->GetMode();

		auto analysisManager = G4AnalysisManager::Instance();
		auto volume = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
		G4LogicalVolume *logicvolume = nullptr;
		if (volume)
		{
			logicvolume = volume->GetLogicalVolume();
		}

		auto touchable = aStep->GetPostStepPoint()->GetTouchableHandle();
		auto ParticleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();

		const CDEXDetectorConstruction *detectorConstruction = static_cast<const CDEXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

		if (ParticleName == "opticalphoton" && logicvolume != nullptr)
		{
			G4int SiPMType = 0;
			G4int SiPMID;
			G4int EvtID = CDEXEvent->GetEventID();
			if (logicvolume == detectorConstruction->GetImaginaryPMT())
			{

				SiPMID = touchable->GetCopyNumber(0);
				// G4cout<<"True"<<G4endl;
				// G4cout<<SiPMID<<G4endl;
				CDEXRun->CountImaginaryPMTPhoton(SiPMID);
			}
		}
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double CDEXSteppingAction::GetEfficiency(G4double wavelength)
{
	G4double eff;
	if (wavelength > 400 && wavelength <= 900)
	{
		eff = 0.5 - (wavelength - 400) / 1000;
	}
	else if (wavelength > 100 && wavelength <= 400)
	{
		eff = (wavelength - 100) * 5 / 3000;
	}
	else
	{
		eff = 0;
	}
	G4double SiPMEffFactor = 0.2;
	eff = eff * SiPMEffFactor;
	return eff;
}


G4double CDEXSteppingAction::GetPMTEfficiency(G4double wavelength)
{
	G4double eff;
	G4int PointNumber=sizeof(PMTWavelength) / sizeof(PMTWavelength[0]);
	if(wavelength<PMTWavelength[0]||wavelength>PMTWavelength[PointNumber-1]){
		eff=0;
	}
	else{
		G4int i=0;
		while(i<PointNumber){
			i++;
			if (PMTWavelength[i]>=wavelength)
            {
                break;
            }
		}
		eff=(PMTEffciency[i]-PMTEffciency[i-1])/(PMTWavelength[i]-PMTWavelength[i-1])*(wavelength-PMTWavelength[i-1])+PMTEffciency[i-1];
	}
	//G4cout<<"Ar:"<<eff<<G4endl;
	return eff;
}

G4double CDEXSteppingAction::GetSiPMEfficiency(G4double wavelength)
{
	G4double eff;
	G4int SiPMPointNumber=sizeof(SiPMWavelength) / sizeof(SiPMWavelength[0]);
	if(wavelength<SiPMWavelength[0]||wavelength>SiPMWavelength[SiPMPointNumber-1]){
		eff=0;
	}
	else{
		G4int i=0;
		while(i<SiPMPointNumber){
			i++;
			if (SiPMWavelength[i]>=wavelength)
            {
                break;
            }
		}
		eff=(SiPMEffciency[i]-SiPMEffciency[i-1])/(SiPMWavelength[i]-SiPMWavelength[i-1])*(wavelength-SiPMWavelength[i-1])+SiPMEffciency[i-1];
	}
	return eff;
}