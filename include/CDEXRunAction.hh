//CDEXRunAction.hh

#ifndef CDEXRunAction_h
#define CDEXRunAction_h

#define DETNUMBER 50
#define ACCTRUE
#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4AccumulableManager.hh"
#include "G4Accumulable.hh"
#include "CDEXPrimaryGeneratorAction.hh"
#include "CDEXDetectorConstruction.hh"

class G4Run;
class CDEXPrimaryGeneratorAction;
class CDEXDetectorConstruction;
class CDEXRunMessenger;

class CDEXRunAction : public G4UserRunAction
{
public:
	CDEXRunAction(CDEXPrimaryGeneratorAction *, CDEXDetectorConstruction *);
	~CDEXRunAction();

	void BeginOfRunAction(const G4Run *);
	void EndOfRunAction(const G4Run *);
	void CDEX300Output(const G4Run *aRun);
	void CDEXFiberBucketSetupOutput(const G4Run *aRun);
	void CountTransmittedPhoton(G4int paraid);

	void CountSiPMEvent()
	{
		SiPMEventCount += 1;
	}

	void CountFiberPhoton()
	{
		FiberPhotonCount += 1;
	}

	void CountBulkEvent()
	{
		BulkEventCount += 1;
	}

	void CountVetoEvent()
	{
		VetoEventCount += 1;
	}

	void CountROIVetoEvent()
	{
		ROIVetoEventCount += 1;
	}

	void CountVetoPossibleEvent()
	{
		VetoPossibleEvtCount += 1;
	}

	void CountROIVetoPossibleEvent()
	{
		ROIVetoPossibleEvtCount += 1;
	}

	void CountROIEvent()
	{
		ROIEventCount += 1;
	}

	void CountDetectableEvent()
	{
		DetectableEventCount += 1;
	}

private:
	//number of events that generate signals in bulk
	//G4int BulkEventCount;
	//number of events that generate signals in SiPMs
	//G4int SiPMEventCount;
	//G4int VetoEventCount;
	CDEXPrimaryGeneratorAction *fPrimaryGenerator;
	CDEXDetectorConstruction *fDetCons;

	G4Accumulable<G4int> DetectableEventCount;
	G4Accumulable<G4int> SiPMEventCount;
	G4Accumulable<G4int> FiberPhotonCount;
	G4Accumulable<G4int> BulkEventCount;
	G4Accumulable<G4int> ROIEventCount;

	G4Accumulable<G4int> VetoEventCount;
	G4Accumulable<G4int> ROIVetoEventCount;
	
	G4Accumulable<G4int> VetoPossibleEvtCount;
	G4Accumulable<G4int> ROIVetoPossibleEvtCount;

	CDEXRunMessenger *fRunMessenger;
	G4String filename;
	G4String txtname;
	G4int runID;
};

#endif