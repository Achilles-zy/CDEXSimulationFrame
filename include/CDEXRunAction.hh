//CDEXRunAction.hh

#ifndef CDEXRunAction_h
#define CDEXRunAction_h

#define DETNUMBER 304
#define STRINGNUMBER 19
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
	void CDEXArParametersTestOutput(const G4Run *aRun);
	void CDEXArExpOutput(const G4Run *aRun);
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

	void CountVetoEvent(G4int i)
	{
		switch (i)
		{
		case 0:
			VetoEventCount += 1;
			break;
		case 1:
			VetoEventCount1 += 1;
			break;
		case 2:
			VetoEventCount2 += 1;
			break;
		case 3:
			VetoEventCount3 += 1;
			break;
		case 4:
			VetoEventCount4 += 1;
			break;
		default:
			break;
		}
	}

	void CountROIVetoEvent(G4int i)
	{
		switch (i)
		{
		case 0:
			ROIVetoEventCount += 1;
			break;
		case 1:
			ROIVetoEventCount1 += 1;
			break;
		case 2:
			ROIVetoEventCount2 += 1;
			break;
		case 3:
			ROIVetoEventCount3 += 1;
			break;
		case 4:
			ROIVetoEventCount4 += 1;
			break;
		default:
			break;
		}
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

	// void CountImaginaryPMTPhoton(G4int id)
	// {
	// 	ImaginaryPMTPhotonCount[id] += 1;
	// }
	void CountImaginaryPMTPhoton(G4int id)
	{
		switch (id)
		{
		case 0:
			ImaginaryPMTPhotonCount0 += 1;
			break;
		case 1:
			ImaginaryPMTPhotonCount1 += 1;
			break;
		case 2:
			ImaginaryPMTPhotonCount2 += 1;
			break;
		case 3:
			ImaginaryPMTPhotonCount3 += 1;
			break;
		case 4:
			ImaginaryPMTPhotonCount4 += 1;
			break;
		case 5:
			ImaginaryPMTPhotonCount5 += 1;
			break;
		case 6:
			ImaginaryPMTPhotonCount6 += 1;
			break;
		case 7:
			ImaginaryPMTPhotonCount7 += 1;
			break;
		case 8:
			ImaginaryPMTPhotonCount8 += 1;
			break;
		case 9:
			ImaginaryPMTPhotonCount9 += 1;
			break;
		case 10:
			ImaginaryPMTPhotonCount10 += 1;
			break;
		case 11:
			ImaginaryPMTPhotonCount11 += 1;
			break;
		case 12:
			ImaginaryPMTPhotonCount12 += 1;
			break;
		case 13:
			ImaginaryPMTPhotonCount13 += 1;
			break;
		case 14:
			ImaginaryPMTPhotonCount14 += 1;
			break;
		case 15:
			ImaginaryPMTPhotonCount15 += 1;
			break;
		case 16:
			ImaginaryPMTPhotonCount16 += 1;
			break;
		case 17:
			ImaginaryPMTPhotonCount17 += 1;
			break;
		case 18:
			ImaginaryPMTPhotonCount18 += 1;
			break;
		case 19:
			ImaginaryPMTPhotonCount19 += 1;
			break;
		default:
			break;
		}
	}
	
	void SetRefresh(G4bool b){
		ifRefresh=b;
	}

	G4bool GetRefresh(){
		return ifRefresh;
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

	G4Accumulable<G4int> VetoEventCount1;
	G4Accumulable<G4int> ROIVetoEventCount1;
	G4Accumulable<G4int> VetoEventCount2;
	G4Accumulable<G4int> ROIVetoEventCount2;
	G4Accumulable<G4int> VetoEventCount3;
	G4Accumulable<G4int> ROIVetoEventCount3;
	G4Accumulable<G4int> VetoEventCount4;
	G4Accumulable<G4int> ROIVetoEventCount4;

	G4Accumulable<G4int> VetoPossibleEvtCount;
	G4Accumulable<G4int> ROIVetoPossibleEvtCount;

	//std::array<G4Accumulable<G4int>,20> ImaginaryPMTPhotonCount;
	//G4int ImaginaryPMTPhotonCount[20];
	//G4Accumulable<G4int> ImaginaryPMTPhotonCount1;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount0;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount1;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount2;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount3;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount4;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount5;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount6;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount7;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount8;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount9;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount10;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount11;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount12;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount13;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount14;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount15;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount16;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount17;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount18;
	G4Accumulable<G4int> ImaginaryPMTPhotonCount19;

	//G4Accumulable<G4int> ImaginaryPMTPhotonCount1;
	// G4Accumulable<G4int> ImaginaryPMTPhotonCount[20] = {
	// 	ImaginaryPMTPhotonCount0,
	// 	ImaginaryPMTPhotonCount1,
	// 	ImaginaryPMTPhotonCount2,
	// 	ImaginaryPMTPhotonCount3,
	// 	ImaginaryPMTPhotonCount4,
	// 	ImaginaryPMTPhotonCount5,
	// 	ImaginaryPMTPhotonCount6,
	// 	ImaginaryPMTPhotonCount7,
	// 	ImaginaryPMTPhotonCount8,
	// 	ImaginaryPMTPhotonCount9,
	// 	ImaginaryPMTPhotonCount10,
	// 	ImaginaryPMTPhotonCount11,
	// 	ImaginaryPMTPhotonCount12,
	// 	ImaginaryPMTPhotonCount13,
	// 	ImaginaryPMTPhotonCount14,
	// 	ImaginaryPMTPhotonCount15,
	// 	ImaginaryPMTPhotonCount16,
	// 	ImaginaryPMTPhotonCount17,
	// 	ImaginaryPMTPhotonCount18,
	// 	ImaginaryPMTPhotonCount19};

	CDEXRunMessenger *fRunMessenger;
	G4String filename;
	G4String txtname;
	G4int runID;
	G4bool ifRefresh;
};

#endif