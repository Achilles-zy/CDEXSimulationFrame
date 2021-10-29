#include "CDEXDetectorConstruction.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Hype.hh"
#include "G4ExtrudedSolid.hh"
#include "G4EllipticalCone.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4String.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4NistManager.hh"
#include "G4Cerenkov.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4MultiUnion.hh"

#include "CDEXMaterials.hh"
#include <G4VisAttributes.hh>
#include <iostream>
#include <fstream>
#include <iterator>

#include "CADMesh.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

//#include "TMath.h"

using namespace std;
CDEXDetectorConstruction::CDEXDetectorConstruction() : G4VUserDetectorConstruction(),
													   physEnv(nullptr),
													   physBulk(nullptr),
													   physPENShell(nullptr),
													   physInnerShell(nullptr),
													   physOuterShell(nullptr),
													   physSiPM0(nullptr),
													   physSiPM1(nullptr),
													   physSiPM2(nullptr),
													   physSiPM3(nullptr),
													   physSiPM4(nullptr),
													   physSiPM5(nullptr),
													   physContainerSiPM0(nullptr),
													   physContainerSiPM1(nullptr),
													   physContainerSiPM2(nullptr),
													   physContainerSiPM3(nullptr),
													   physContainerSiPM4(nullptr),
													   physContainerSiPM5(nullptr),
													   physSiPMArray0(nullptr),
													   physSiPMArray1(nullptr),
													   physSiPMArray2(nullptr),
													   physSiPMArray3(nullptr),
													   physOuterReflector(nullptr),
													   physInnerReflector(nullptr),
													   // physWire(nullptr),
													   physContainerBrick(nullptr),	  // ContainerBrick
													   physContainerCrystal(nullptr), // Crystal in Container
													   physStringBoxBrick(nullptr),
													   physStringBoxCrystal(nullptr), // Crystal in String Box
													   physASICPlate(nullptr),
													   physFiberSiPM(nullptr),
													   physArVolumeSiPM(nullptr),
													   logicBEGe(nullptr),
													   logicBulk(nullptr),
													   // logicPENShell(nullptr),
													   logicContainerCrystal(nullptr),
													   logicStringBoxCrystal(nullptr),
													   logicASICPlate(nullptr),
													   logicWire(nullptr),
													   logicArVolume_FiberBucket(nullptr),
													   logicArVolume_SiPMBucket(nullptr),
													   logicShourdVoid(nullptr),
													   logicArVolume_CDEX300(nullptr),
													   logicCuShield(nullptr),
													   logicSSWall(nullptr),
													   logicFiberSiPM(nullptr),
													   logicArVolumeSiPM(nullptr),
													   logicArVolume(nullptr),
													   logicArContainer(nullptr),
													   logicImaginaryPMT(nullptr),
													   LambdaE(twopi * 1.973269602e-16 * m * GeV)
{
	fDetectorMessenger = new CDEXDetectorMessenger(this);
	matconstructor = new CDEXMaterials();
	MPT_PEN = new G4MaterialPropertiesTable();
	MPT_Ar = new G4MaterialPropertiesTable();
	AbsorptionLength = 1.5; // value at 400 nm
	fRES = 1.0;
	fLY = 3500. / MeV;
	fABSFile = "PEN_ABS";
	fConfine = "PENShell";
	fWireType = "A1";
	fReflectorType = "PolisherESR_LUT";
	// fMode = "CDEX300";
	fMode = "CDEXArParametersTest";
	fFiberShape = "Rec"; // Round & Rec
	// Unit
	// SArUnit
	// Array-1
	// CDEXSiPMBucketSetup
	// CDEXFiberBucketSetup
	// FiberTest
	// CDEX300
	// CDEXLightGuideBucketSetup
	// CDEXArParametersTest
	// CDEXArExpSetup

	fWirePos = G4ThreeVector();
	fWireRadius = 0.7 * mm;
	fWireLength = 4 * cm;
	fWireCentDist = 5 * cm;
	pmtReflectivity = 0.50;
	fPENShellLength = 6.5 * cm;
	fPENShellRadius = 20 * cm;
	fPENPropertiesID = 1;
	fBEGeRadius = 40 * mm;
	fBEGeHeight = 40 * mm;
	fSArBrickHeight = 150 * cm;
	fSArBrickRadius = 40 * cm;
	fArContainerHeight = 4 * m;
	fArContainerRadius = 0.7 * m;
	fArContainerThickness = 5 * cm;
	fArVolumeHeight = fArContainerHeight;
	fArVolumeRadius = fArContainerRadius - fArContainerThickness;
	fASICLength = 2 * cm;
	fASICWidth = 1 * cm;
	fASICThickness = 1 * mm;
	absFactor = 1.5;
	fDetMat = matEnGe;
	fOuterShellLength = 33 * cm;
	fOuterShellHeight = 60 * mm;
	fOuterShellWidth = 98 * mm;
	fBucketRadius = 0.75 * m;
	fBucketHeight = 4 * m;
	fBucketThickness = 10 * cm;
	fSmallestUnitHeight = 8 * cm;
	fUnitNb = 16;
	fFiberPlacementRadius = 0.3 * m;
	// fFiberPlacementRadius = 0.295 * m;//LEGEND
	fFiberPlacementCenter = G4ThreeVector(0, 0, 0);
	fFiberLength = 2.4 * m;
	fLightGuideLength = 2.4 * m;

	fFiberRadius = 0.5 * mm;
	fFiberTPBThickness = 0.6 * micrometer;
	fFiberInnerCladdingThickness = 0.04 * mm;
	fFiberOuterCladdingThickness = 0.02 * mm;

	fLightGuideRadius = 12 * cm;
	fLightGuideInnerRadius = 29.5 * cm;

	fShellThickness = 5 * mm;

	fArYieldRatio = 1;
	fArAbsLength = 60 * cm;

	fCuShieldThickness = 5 * cm;
	G4cout << "Start Construction" << G4endl;
	DefineMat();
	fTargetMaterial = G4Material::GetMaterial("PVT_structure");
	fGlassMaterialPMT = G4Material::GetMaterial("BorosilicateGlass");
	ifOuterReflector = false;
	ifInnerReflector = false;
	ifReflector = false;
	ifFiberTPB = true;
	CheckOverlaps = true;

	G4ThreeVector List[19] =
		{
			G4ThreeVector(0, 0, 0),
			G4ThreeVector(0, 1, 0),
			G4ThreeVector(0, -1, 0),
			G4ThreeVector(0.5 * sqrt(3), 0.5, 0),
			G4ThreeVector(0.5 * sqrt(3), -0.5, 0),
			G4ThreeVector(-0.5 * sqrt(3), 0.5, 0),
			G4ThreeVector(-0.5 * sqrt(3), -0.5, 0),
			G4ThreeVector(0, 2, 0),
			G4ThreeVector(0, -2, 0),
			G4ThreeVector(0.5 * sqrt(3), 1.5, 0),
			G4ThreeVector(0.5 * sqrt(3), -1.5, 0),
			G4ThreeVector(sqrt(3), 0, 0),
			G4ThreeVector(sqrt(3), 1, 0),
			G4ThreeVector(sqrt(3), -1, 0),
			G4ThreeVector(-0.5 * sqrt(3), 1.5, 0),
			G4ThreeVector(-0.5 * sqrt(3), -1.5, 0),
			G4ThreeVector(-sqrt(3), 0, 0),
			G4ThreeVector(-sqrt(3), 1, 0),
			G4ThreeVector(-sqrt(3), -1, 0)};
	memcpy(StringPosList, List, sizeof(List));
}

CDEXDetectorConstruction::~CDEXDetectorConstruction()
{
}

G4VPhysicalVolume *CDEXDetectorConstruction::GetPhysicalVolumeByName(const G4String &name)
{
	// access the store of physical volumes
	G4PhysicalVolumeStore *pvs = G4PhysicalVolumeStore::GetInstance();
	G4VPhysicalVolume *pv;
	G4int npv = pvs->size();
	G4int ipv;
	G4cout << "Volume Name List:" << G4endl;
	for (ipv = 0; ipv < npv; ipv++)
	{
		pv = (*pvs)[ipv];
		if (!pv)
			break;
		if (pv->GetName() == name)
			return pv;
		G4String VolumeName = pv->GetName();
		G4double VolumeMass = pv->GetLogicalVolume()->GetMass();
	}
	G4cout << "PV" << name << " not found!" << G4endl;
	return NULL;
}

void CDEXDetectorConstruction::GetPhysicalVolumeProperties()
{
	// access the store of physical volumes
	VolumeLUT.clear();
	VolumeNameLUT.clear();
	G4PhysicalVolumeStore *pvs = G4PhysicalVolumeStore::GetInstance();
	G4VPhysicalVolume *pv;
	G4int npv = pvs->size();
	G4int ipv;
	G4cout << "Volume Name List:" << G4endl;
	std::ofstream output;
	output.open("PhysicalVolumeProperties" + GetMode() + ".txt", std::ios::ate);
	output << "Volume ID" << '\t' << "Volume Name" << '\t' << "Volume Mass" << G4endl;
	for (ipv = 0; ipv < npv; ipv++)
	{
		pv = (*pvs)[ipv];
		if (!pv)
			break;
		VolumeLUT.insert(pair<G4VPhysicalVolume *, G4int>(pv, ipv));
		VolumeNameLUT.insert(pair<G4VPhysicalVolume *, G4String>(pv, pv->GetName()));
		G4String VolumeName = pv->GetName();
		G4double VolumeMass = pv->GetLogicalVolume()->GetMass() / (1 * kg);
		G4cout << "Volume ID:" << ipv << " VolumeName:" << VolumeName << " VolumeMass" << VolumeMass << G4endl;
		output << ipv << '\t' << VolumeName << '\t' << VolumeMass << G4endl;
	}
	output.close();
}

void CDEXDetectorConstruction::ResetPhysicalVolumeNames()
{
	// access the store of physical volumes
	G4PhysicalVolumeStore *pvs = G4PhysicalVolumeStore::GetInstance();
	G4VPhysicalVolume *pv;
	G4int npv = pvs->size();
	G4int ipv;

	for (ipv = 0; ipv < npv; ipv++)
	{
		pv = (*pvs)[ipv];
		if (!pv)
			break;
		std::map<G4VPhysicalVolume *, G4String>::iterator iter;
		iter = VolumeNameLUT.find(pv);
		if (iter != VolumeNameLUT.end())
		{
			pv->SetName(iter->second);
		}
	}
	G4cout << "Reset physical volume names done!" << G4endl;
}

void CDEXDetectorConstruction::SetWireType(G4String type)
{
	fWireType = type;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetConfine(G4String confine)
{
	fConfine = confine;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetRunInfo(G4String runinfo)
{
	fRunInfo = runinfo;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetMode(G4String mode)
{
	fMode = mode;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetPENPropertiesID(G4int nb)
{
	fPENPropertiesID = nb;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetOuterReflector(G4bool ref)
{
	ifOuterReflector = ref;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetInnerReflector(G4bool ref)
{
	ifInnerReflector = ref;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetReflectorType(G4String type)
{
	fReflectorType = type;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetCuShieldThickness(G4double thickness)
{
	fCuShieldThickness = thickness;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetArYieldRatio(G4double yieldratio)
{
	fArYieldRatio = yieldratio;
	G4double scint_yield = 23.6 * eV; // Nominal energy to produce a photon (measured)
	G4double yield_factor = fArYieldRatio;
	G4double photon_yield = 1.0 * MeV / scint_yield;
	matLAr->GetMaterialPropertiesTable()->AddConstProperty("SCINTILLATIONYIELD", photon_yield * yield_factor);
}

void CDEXDetectorConstruction::SetArAbsLength(G4double abslength)
{
	fArAbsLength = abslength;

	const G4int NUMENTRIES = 69;
	G4int ji;
	G4double e;

	G4double PPCKOVHighE = LambdaE / (115 * nanometer);
	G4double PPCKOVLowE = LambdaE / (650 * nanometer);
	G4double de = ((PPCKOVHighE - PPCKOVLowE) / ((G4double)(NUMENTRIES - 1)));

	// liquid argon (LAr)
	G4double LAr_PPCK[(NUMENTRIES)] = {0};
	G4double LAr_ABSL[(NUMENTRIES)] = {0};

	// G4double LAr_ABSL_xuv = 60 * cm;
	G4double LAr_ABSL_xuv = fArAbsLength;
	// G4double LAr_ABSL_xuv = 110*cm;
	G4double LAr_ABSL_vis = 1000 * m;

	for (ji = 0; ji < NUMENTRIES; ji++)
	{
		e = PPCKOVLowE + ((G4double)ji) * de;
		LAr_PPCK[ji] = e;

		if (((LambdaE / e) / nm) < 200.0)
		{
			LAr_ABSL[ji] = LAr_ABSL_xuv;
		}
		else
		{
			LAr_ABSL[ji] = LAr_ABSL_vis;
		}
	}

	matLAr->GetMaterialPropertiesTable()->AddProperty("ABSLENGTH", LAr_PPCK, LAr_ABSL, NUMENTRIES);
}

void CDEXDetectorConstruction::DefineMat()
{
	matconstructor->Construct();
	// ============================================================= Materials =============================================================
	// materialConstruction = new PenMaterials;
	matAir = G4Material::GetMaterial("Air");
	matBialkali = G4Material::GetMaterial("Bialkali");
	fGlass = G4Material::GetMaterial("BorosilicateGlass");
	fPOM = G4Material::GetMaterial("POM");
	fABS = G4Material::GetMaterial("ABS");
	matPEN = G4Material::GetMaterial("PEN");
	matSi = G4Material::GetMaterial("G4_Si");
	matCu = G4Material::GetMaterial("G4_Cu");
	matTriggerFoilEJ212 = G4Material::GetMaterial("EJ212");
	matPolystyrene = G4Material::GetMaterial("Polystyrene");
	matPMMA = G4Material::GetMaterial("PMMA");
	fVacuum = G4Material::GetMaterial("Vacuum");
	matGreaseEJ550 = G4Material::GetMaterial("Grease");
	matTeflon = G4Material::GetMaterial("G4_TEFLON");
	matVikuiti = G4Material::GetMaterial("Vikuiti");
	matTitanium = G4Material::GetMaterial("Titanium");
	matPolyethylene = G4Material::GetMaterial("G4_POLYETHYLENE");
	matEnGe = G4Material::GetMaterial("EnGe");
	matNaGe = G4Material::GetMaterial("NaGe");
	matLN2 = G4Material::GetMaterial("G4_lN2");
	matGN2 = G4Material::GetMaterial("G4_N");
	matLAr = G4Material::GetMaterial("G4_lAr");
	// matLAr = G4Material::GetMaterial("LAr");
	matGAGG = G4Material::GetMaterial("GAGG");
	matPTFE = G4Material::GetMaterial("PTFE");
	matNylon = G4Material::GetMaterial("Nylon");
	matTPB = G4Material::GetMaterial("TPB");
	matFiber = G4Material::GetMaterial("PolystyreneFiber");
	matSS = G4Material::GetMaterial("G4_STAINLESS-STEEL");
	matFluorAcrylic = G4Material::GetMaterial("Fluor-acrylic");
	G4cout << " materials ok " << G4endl;

	G4double wavelength;
	char filler;
	G4double varAbsorLength;
	G4double emission;
	G4double rindex;

	G4double wlPhotonEnergy[102] = {0};
	G4double ABSORPTION_PEN[102] = {0};
	G4double RINDEX_PEN[102] = {0};

	G4int absEntries = 0;

	ifstream ReadAbs;

	G4String absFile = "../input_files/" + fABSFile + ".csv";
	ReadAbs.open(absFile);
	if (ReadAbs.is_open())
	{
		while (!ReadAbs.eof())
		{
			ReadAbs >> wavelength >> filler >> varAbsorLength >> filler >> emission >> filler >> rindex;
			if (ReadAbs.eof())
			{
				break;
			}
			wlPhotonEnergy[absEntries] = (1240. / wavelength) * eV;
			ABSORPTION_PEN[absEntries] = (varAbsorLength)*mm;
			RINDEX_PEN[absEntries] = rindex;
			absEntries++;
		}
	}

	else
		G4cout << "Error opening file: " << absFile << G4endl;
	ReadAbs.close();
	absEntries--;

	const G4int nEntries1 = sizeof(wlPhotonEnergy) / sizeof(G4double);
	assert(sizeof(RINDEX_PEN) == sizeof(wlPhotonEnergy));
	assert(sizeof(ABSORPTION_PEN) == sizeof(wlPhotonEnergy));
	// assert(sizeof(EMISSION_PEN) == sizeof(wlPhotonEnergy));

	MPT_PEN = new G4MaterialPropertiesTable();

	/////////////////////////////////
	// const G4int NUMENTRIES_LN2 = 11;
	// const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
	// G4double LN2_PP[NUMENTRIES_LN2] = { hc / 800. * eV, hc / 520. * eV, hc / 495. * eV, hc / 480. * eV, hc / 467. * eV, hc / 450. * eV, hc / 431. * eV, hc / 425. * eV, hc / 419. * eV,hc / 412. * eV,hc / 40. * eV };
	// G4double LN2_RefractiveIndex[NUMENTRIES_LN2] = { 1.638, 1.638, 1.638, 1.638, 1.638, 1.638, 1.638, 1.638, 1.638, 1.638, 1.638 };

	// MPT_PEN->AddProperty("RINDEX", LN2_PP, LN2_RefractiveIndex, NUMENTRIES_LN2);
	//////////////////////////////////

	MPT_PEN->AddProperty("RINDEX", wlPhotonEnergy, RINDEX_PEN, nEntries1)->SetSpline(true);
	MPT_PEN->AddProperty("ABSLENGTH", wlPhotonEnergy, ABSORPTION_PEN, nEntries1)->SetSpline(true); // *

	
	// Read primary emission spectrum from PEN
	// Measurements from MPP Munich
	G4double pWavelength;
	G4String Scint_file = "../properties/PEN_EM_SPECTRUM.dat";
	std::ifstream ReadScint2(Scint_file), ReadScintPEN;
	// count number of entries
	ReadScint2.unsetf(std::ios_base::skipws);
	// unsigned line_count = std::count(
	int line_count = std::count(
		std::istream_iterator<char>(ReadScint2),
		std::istream_iterator<char>(),
		'\n');
	std::cout << "Lines: " << line_count << "\n";
	ReadScint2.close();
	G4double PEN_EMISSION[500];
	G4double PEN_WL_ENERGY[500];
	G4int nEntriesPEN = 0;
	ReadScintPEN.open(Scint_file);
	if (ReadScintPEN.is_open())
	{
		while (!ReadScintPEN.eof())
		{

			ReadScintPEN >> pWavelength >> PEN_EMISSION[nEntriesPEN];
			if (ReadScintPEN.eof())
			{
				break;
			}
			PEN_WL_ENERGY[nEntriesPEN] = (1240. / pWavelength) * eV; // convert wavelength to eV
																	 // G4cout<<nEntriesPEN<<" wl "<<PEN_WL_ENERGY[nEntriesPEN]<<" "<<PEN_EMISSION[nEntriesPEN]<<G4endl;
			nEntriesPEN++;
			if (nEntriesPEN > (line_count - 1))
			{
				G4cout << " entries completed " << G4endl;
				break;
			}
		}
	}
	else
		G4cout << "Error opening file: " << Scint_file << G4endl;
	ReadScintPEN.close();
	G4cout << " nEntriesPEN " << nEntriesPEN << G4endl;

	MPT_PEN->AddProperty("FASTCOMPONENT", PEN_WL_ENERGY, PEN_EMISSION, line_count)->SetSpline(true);
	MPT_PEN->AddProperty("SLOWCOMPONENT", PEN_WL_ENERGY, PEN_EMISSION, line_count)->SetSpline(true);

	MPT_PEN->AddConstProperty("SCINTILLATIONYIELD", fLY); // * 2.5 * PEN = PS, 10*PEN=PS
	MPT_PEN->AddConstProperty("RESOLUTIONSCALE", fRES);	  // * 1, 4, 8
	MPT_PEN->AddConstProperty("FASTTIMECONSTANT", 5.198 * ns);
	MPT_PEN->AddConstProperty("SLOWTIMECONSTANT", 24.336 * ns);
	MPT_PEN->AddConstProperty("YIELDRATIO", 1.);

	G4cout << "PEN Properties:" << G4endl;
	G4cout << "AbsFactor =" << absFactor << G4endl;
	G4cout << "LY =" << fLY << G4endl;
	matPEN->SetMaterialPropertiesTable(MPT_PEN);
	// pvt_structure->SetMaterialPropertiesTable(MPT_PEN);
	G4cout << " pen ok " << G4endl;

	G4double rindexEnergy[500] = {0};
	G4double scintIndex[500] = {0};

	G4int rindexEntries = 0;
	ifstream ReadRindex;

	G4String rindex_file = "../input_files/rindexScint.txt";
	ReadRindex.open(rindex_file);

	if (ReadRindex.is_open())
	{
		while (!ReadRindex.eof())
		{

			ReadRindex >> wavelength >> filler >> scintIndex[rindexEntries];
			if (ReadRindex.eof())
			{
				break;
			}
			rindexEnergy[rindexEntries] = (1240. / wavelength) * eV;
			rindexEntries++;
		}
	}
	else
		G4cout << "Error opening file: " << rindex_file << G4endl;
	ReadRindex.close();
	rindexEntries--;

	G4double scintEnergy[501] = {0};
	G4double scintEmit[501] = {0};
	G4double scintEmitSlow[501] = {0};

	G4int scintEntries = 0;
	ifstream ReadScint;

	Scint_file = "../input_files/pTP_emission.txt";
	ReadScint.open(Scint_file);

	if (ReadScint.is_open())
	{
		while (!ReadScint.eof())
		{

			ReadScint >> wavelength >> filler >> scintEmit[scintEntries];
			if (ReadScint.eof())
			{
				break;
			}
			// convert wavelength to eV:
			scintEnergy[scintEntries] = (1240. / wavelength) * eV;
			scintEmitSlow[scintEntries] = scintEmit[scintEntries];
			scintEntries++;
		}
	}
	else
		G4cout << "Error opening file: " << Scint_file << G4endl;
	ReadScint.close();
	scintEntries--;

	G4int absorbEntries = 0;
	G4double varAbsorbLength;
	G4double absorbEnergy[501] = {0};
	G4double Absorb[501] = {0};

	ifstream ReadAbsorb;
	G4String ReadAbsorbLength = "../input_files/PlasticBulkAbsorb2.cfg";

	ReadAbsorb.open(ReadAbsorbLength);
	if (ReadAbsorb.is_open())
	{
		while (!ReadAbsorb.eof())
		{

			ReadAbsorb >> wavelength >> filler >> varAbsorbLength;
			if (ReadAbsorb.eof())
			{
				break;
			}
			absorbEnergy[absorbEntries] = (1240 / wavelength) * eV;
			Absorb[absorbEntries] = (varAbsorbLength)*m;
			absorbEntries++;
		}
	}
	else
		G4cout << "Error opening file: " << ReadAbsorbLength << G4endl;
	ReadAbsorb.close();
	absorbEntries--;

	G4double wlsEnergy[501] = {0};
	G4double wlsEmit[501] = {0};

	G4int wlsScintEntries = 0;
	ifstream ReadWLSScint;

	G4String wls_Scint_file = "../input_files/full_popop_emission.cfg";
	ReadWLSScint.open(wls_Scint_file);

	if (ReadWLSScint.is_open())
	{
		while (!ReadWLSScint.eof())
		{

			ReadWLSScint >> wavelength >> filler >> wlsEmit[500 - wlsScintEntries];
			if (ReadWLSScint.eof())
			{
				break;
			}
			// convert wavelength to eV:
			wlsEnergy[500 - wlsScintEntries] = (1240 / wavelength) * eV;
			wlsScintEntries++;
		}
	}
	else
		G4cout << "Error opening file: " << wls_Scint_file << G4endl;
	ReadWLSScint.close();
	wlsScintEntries--;

	G4int wlsAbsorbEntries = 0;
	G4double wlsAbsorbEnergy[501] = {0};
	G4double wlsAbsorb[501] = {0};

	ifstream ReadWLSAbsorb;
	G4String ReadWLSAbsorbLength = "../input_files/scintAbsLen.txt";

	ReadWLSAbsorb.open(ReadWLSAbsorbLength);
	if (ReadWLSAbsorb.is_open())
	{
		while (!ReadWLSAbsorb.eof())
		{
			ReadWLSAbsorb >> wavelength >> filler >> varAbsorbLength;
			if (ReadWLSAbsorb.eof())
			{
				break;
			}
			wlsAbsorbEnergy[wlsAbsorbEntries] = (1240. / wavelength) * eV;
			wlsAbsorb[wlsAbsorbEntries] = varAbsorbLength * mm;
			wlsAbsorbEntries++;
		}
	}
	else
		G4cout << "Error opening file: " << ReadWLSAbsorbLength << G4endl;
	ReadWLSAbsorb.close();
	wlsAbsorbEntries--;

	G4MaterialPropertiesTable *MPT_FoilEJ212 = new G4MaterialPropertiesTable();

	MPT_FoilEJ212->AddProperty("WLSABSLENGTH", wlsAbsorbEnergy, wlsAbsorb, wlsAbsorbEntries);
	MPT_FoilEJ212->AddProperty("WLSCOMPONENT", wlsEnergy, wlsEmit, wlsScintEntries);
	MPT_FoilEJ212->AddConstProperty("WLSTIMECONSTANT", 12 * ns);

	MPT_FoilEJ212->AddProperty("RINDEX", rindexEnergy, scintIndex, rindexEntries);
	MPT_FoilEJ212->AddProperty("ABSLENGTH", absorbEnergy, Absorb, absorbEntries);
	MPT_FoilEJ212->AddProperty("FASTCOMPONENT", scintEnergy, scintEmit, scintEntries);
	MPT_FoilEJ212->AddProperty("SLOWCOMPONENT", scintEnergy, scintEmitSlow, scintEntries);

	// MPT_FoilEJ212->AddConstProperty("SCINTILLATIONYIELD",11520./MeV);
	MPT_FoilEJ212->AddConstProperty("SCINTILLATIONYIELD", 10. / MeV); // set low LY to make it faster, intead use Edep for coincidences
	MPT_FoilEJ212->AddConstProperty("RESOLUTIONSCALE", 4.0);
	MPT_FoilEJ212->AddConstProperty("FASTTIMECONSTANT", 2.1 * ns);
	MPT_FoilEJ212->AddConstProperty("SLOWTIMECONSTANT", 14.2 * ns);
	MPT_FoilEJ212->AddConstProperty("YIELDRATIO", 1.0);

	matTriggerFoilEJ212->SetMaterialPropertiesTable(MPT_FoilEJ212);

	G4cout << " EJ212 ok " << G4endl;

	G4double refractive_index[] = {1.49, 1.49, 1.49, 1.49, 1.49, 1.49};
	G4double absPMMA[] = {5 * m, 5 * m, 5 * m, 5 * m, 5 * m, 5 * m};
	G4double reflPMMA[] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
	G4double energyPMMA[] = {2.18 * eV, 2.48 * eV, 2.58 * eV, 2.68 * eV, 2.78 * eV, 4.1 * eV};
	const G4int nEntries3 = sizeof(energyPMMA) / sizeof(G4double);

	G4MaterialPropertiesTable *MPT_PMMA = new G4MaterialPropertiesTable();
	MPT_PMMA->AddProperty("RINDEX", energyPMMA, refractive_index, nEntries3);
	MPT_PMMA->AddProperty("ABSLENGTH", energyPMMA, absPMMA, nEntries3)->SetSpline(true);
	MPT_PMMA->AddProperty("REFLECTIVITY", energyPMMA, reflPMMA, nEntries3)->SetSpline(true);
	matPMMA->SetMaterialPropertiesTable(MPT_PMMA);

	G4int ji;
	G4double e;
	G4double ee;
	G4int NUMENTRIES = 69;
	G4double PPCKOVHighE = LambdaE / (115 * nanometer);
	G4double PPCKOVLowE = LambdaE / (650 * nanometer);
	G4double de = ((PPCKOVHighE - PPCKOVLowE) / ((G4double)(NUMENTRIES - 1)));

	// liquid argon (LAr)
	G4double LAr_PPCK[(NUMENTRIES)] = {0};
	G4double LAr_RAYL[(NUMENTRIES)] = {0};

	for (ji = 0; ji < NUMENTRIES; ji++)
	{
		e = PPCKOVLowE + ((G4double)ji) * de;
		LAr_PPCK[ji] = e;
		// G4double b= LArRayLength((LambdaE / e), temp);
		// G4double T = temp;
		LAr_RAYL[ji] = 66 * cm;
	}

	// matLAr->GetMaterialPropertiesTable()->AddProperty("RAYLEIGH", LAr_PPCK, LAr_RAYL, NUMENTRIES);
}

void CDEXDetectorConstruction::SetPENABS(G4double value)
{
	AbsorptionLength = value;
	// read file and add the value given by the user
	G4double wavelength;
	char filler;
	G4double varAbsorLength;
	G4double emission;
	G4double rindex;

	G4double wlPhotonEnergy[102] = {0};
	G4double ABSORPTION_PEN[102] = {0};

	G4int absEntries = 0;
	ifstream ReadAbs;

	G4String absFile = "../input_files/" + fABSFile + ".csv";
	ReadAbs.open(absFile);
	if (ReadAbs.is_open())
	{
		while (!ReadAbs.eof())
		{
			ReadAbs >> wavelength >> filler >> varAbsorLength >> filler >> emission >> filler >> rindex;
			if (ReadAbs.eof())
			{
				break;
			}
			wlPhotonEnergy[absEntries] = (1240. / wavelength) * eV;
			ABSORPTION_PEN[absEntries] = (varAbsorLength * AbsorptionLength) * mm; // use measured value of attenuation to constrain curve and then change values multiplying the curve for a given factor
			absEntries++;
		}
	}

	else
		G4cout << "Error opening file: " << absFile << G4endl;
	ReadAbs.close();
	absEntries--;

	const G4int nEntries1 = sizeof(wlPhotonEnergy) / sizeof(G4double);
	assert(sizeof(ABSORPTION_PEN) == sizeof(wlPhotonEnergy));
	matPEN->GetMaterialPropertiesTable()->AddProperty("ABSLENGTH", wlPhotonEnergy, ABSORPTION_PEN, nEntries1)->SetSpline(true);
	// 	MPT_PEN->AddProperty("ABSLENGTH", wlPhotonEnergy, ABSORPTION_PEN, nEntries1)->SetSpline(true); // *
	// 																								   //G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	// #ifdef G4MULTITHREADED
	// 	G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
	// #else
	// 	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	// #endif
}

void CDEXDetectorConstruction::SetPENLY(G4double ly)
{
	matPEN->GetMaterialPropertiesTable()->AddConstProperty("SCINTILLATIONYIELD", ly);
	// 	MPT_PEN->AddConstProperty("SCINTILLATIONYIELD", ly); // * 2.5 * PEN = PS, 10*PEN=PS
	// //G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	// #ifdef G4MULTITHREADED
	// 	G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
	// #else
	// 	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	// #endif
}

void CDEXDetectorConstruction::SetLightGuideRadius(G4double r)
{
	fLightGuideRadius = r;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void CDEXDetectorConstruction::SetLightGuideLength(G4double l)
{
	fLightGuideLength = l;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
	// #ifdef G4MULTITHREADED
	// 	G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
	// #else
	// 	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	// #endif
}
////////
//
////////
G4VPhysicalVolume *CDEXDetectorConstruction::Construct()
{
	// if (fPENPropertiesID == 0)
	// {
	// 	fLY = 6000. / MeV;
	// 	absFactor = 1.5;
	// }
	// else if (fPENPropertiesID == 1)
	// {
	// 	fLY = 3500. / MeV;
	// 	absFactor = 2.58;
	// }
	// else if (fPENPropertiesID == 2)
	// {
	// 	fLY = 6000. / MeV;
	// 	absFactor = 6.44;
	// }

	// //SetABS(absFactor);
	// //SetLY(fLY);

	if (fMode == "Unit")
	{
		return ConstructUnit();
	}
	else if (fMode == "SArUnit")
	{
		return ConstructSArUnit();
	}
	else if (fMode == "Array-1")
	{
		return ConstructArray_1();
	}
	else if (fMode == "CDEXSiPMBucketSetup")
	{
		return ConstructBucketSiPMSystem();
	}
	else if (fMode == "CDEXFiberBucketSetup")
	{
		return ConstructBucketFiberSystem();
	}
	else if (fMode == "FiberTest")
	{
		// physWorld=ConstructFiberTestSetup();
		return ConstructFiberTestSetup();
	}
	else if (fMode == "CDEX300")
	{
		// physWorld=ConstructFiberTestSetup();
		return ConstructCDEX300();
	}
	else if (fMode == "CDEXLightGuideBucketSetup")
	{
		// physWorld=ConstructFiberTestSetup();
		return ConstructBucketLightGuideSystem();
	}
	else if (fMode == "CDEXArParametersTest")
	{
		return ConstructBucketLightGuideSystem();
	}
	else if (fMode == "CDEXArExpSetup")
	{
		return ConstructArExpSetup();
	}
	else
	{
		G4cout << "Error: Mode not fount!" << G4endl;
	}
}

// Common Construct Functions
// Place at (0,0,-1mm) to devide the detector evenly in Z direction
G4LogicalVolume *CDEXDetectorConstruction::ConstructBEGe()
{
	// flatBEGe
	G4bool checkOverlaps = true;
	G4double GeChamfer = 2. * mm;
	// double outerGeRadius = 31.35 * mm;
	G4double outerGeRadius = fBEGeRadius;
	G4double innerGeRadius = 1.50 * mm;
	// double GeHeight1 = 60.80 * mm;
	G4double GeHeight1 = fBEGeHeight - GeChamfer;
	G4double SrcThickness = 0.01 * mm;
	G4double lSmallValue = 0.01 * mm;
	G4double groovedepth = 1.5 * mm;
	G4double grooveradius = 9 * mm;
	G4double groovethickness = 0.001 * mm;
	G4double outerplayerthickness = 1 * um;
	G4double orbradius = 1.89 * mm;
	G4double deadlayerthickness = 1 * mm;

	// G4Height2 is the depth of the hole for pin contact
	double GeHeight2 = 4.0 * mm;
	double GeHeight3 = GeHeight2 + deadlayerthickness;

	G4ThreeVector zTransGe0(0., 0., -GeHeight1 / 2);
	G4ThreeVector zTransGe1(0., 0., -GeHeight1 / 2 + deadlayerthickness);
	G4ThreeVector zTransGe2(0., 0., GeHeight1 / 2 + lSmallValue);
	G4ThreeVector zTransGroove(0., 0., -GeHeight1 / 2);

	auto GeP1 = new G4Tubs("GeP1", 0., outerGeRadius, GeHeight1 / 2, 0., twopi);
	auto GeP2 = new G4Tubs("GeP2", 0., outerGeRadius - GeChamfer, GeChamfer, 0., twopi);
	auto GeP3 = new G4Torus("GeP3", 0., GeChamfer, outerGeRadius - GeChamfer, 0., twopi);
	auto GeGroove = new G4Torus("solidgroove", 0., groovedepth, grooveradius, 0., twopi);

	// total germanium crystal
	auto GeTemp1 = new G4UnionSolid("GeTemp1", GeP2, GeP3);
	auto GeTemp2 = new G4UnionSolid("GeTemp2", GeP1, GeTemp1, 0, zTransGe2);
	//?
	// auto solidtempTotalCrystal = new G4SubtractionSolid("totaltempCrystal", GeTemp2, GeGroove, 0, zTransGroove);
	auto solidTotalCrystal = new G4SubtractionSolid("totalCrystal", GeTemp2, GeGroove, 0, zTransGroove);
	auto logicTotalCrystal = new G4LogicalVolume(solidTotalCrystal, matEnGe, "logicTotalCrystal");

	// bulk
	auto GeP1In = new G4Tubs("GeP1In", 0., outerGeRadius - deadlayerthickness, (GeHeight1 - deadlayerthickness * 2) / 2, 0., twopi);
	auto GeP2In = new G4Tubs("GeP2In", 0., outerGeRadius - deadlayerthickness - GeChamfer, GeChamfer, 0., twopi);
	auto GeP3In = new G4Torus("GeP3In", 0., GeChamfer, outerGeRadius - deadlayerthickness - GeChamfer, 0., twopi);
	auto GeP4In = new G4Tubs("GeP4In", 0., grooveradius, deadlayerthickness / 2, 0., twopi);

	auto GeLargerGroove = new G4Torus("solidlargergroove", 0., groovedepth + groovethickness, grooveradius, 0., twopi);
	auto GeOuterpLayer = new G4Tubs("solidouterplayer", 0., grooveradius - groovedepth, outerplayerthickness / 2, 0., twopi);

	auto GeInTemp1 = new G4UnionSolid("GeInTemp1", GeP2In, GeP3In);
	auto GeInTemp2 = new G4UnionSolid("GeInTemp2", GeP1In, GeInTemp1, 0, G4ThreeVector(0., 0., (GeHeight1 - deadlayerthickness * 2) / 2.0 + lSmallValue));
	auto GeInTemp3 = new G4UnionSolid("GeInTemp3", GeInTemp2, GeP4In, 0, G4ThreeVector(0., 0., -(GeHeight1 - deadlayerthickness) / 2));
	G4ThreeVector zbulkTrans(0., 0., -(GeHeight1 - deadlayerthickness * 2 - (GeHeight2 - deadlayerthickness)) / 2);
	G4ThreeVector zbulkTransOuterp(0., 0., -(GeHeight1 - outerplayerthickness) / 2);

	auto GeInTemp4 = new G4SubtractionSolid("GeInTemp4", GeInTemp3, GeLargerGroove, 0, zTransGroove);
	auto solidBulk = new G4SubtractionSolid("Bulk", GeInTemp4, GeOuterpLayer, 0, zbulkTransOuterp);
	logicBulk = new G4LogicalVolume(solidBulk, matEnGe, "Bulk");

	// deadlayer
	auto tempdeadlayer = new G4SubtractionSolid("tempdeadlayer", solidTotalCrystal, GeInTemp3, 0, G4ThreeVector(0., 0., 0.));
	auto solidOuterDeadlayer = new G4SubtractionSolid("OuterDeadlayer", tempdeadlayer, GeLargerGroove, 0, zTransGroove);
	auto logicOuterDeadlayer = new G4LogicalVolume(solidOuterDeadlayer, matEnGe, "OuterDeadlayer");

	// groove Layer
	auto GrooveTorus = new G4Torus("solidpLayer", groovedepth, groovedepth + groovethickness, grooveradius, 0., twopi);
	auto GrooveCut1 = new G4Tubs("groovecut1", 0, grooveradius + groovedepth + groovethickness, groovedepth + groovethickness, 0, twopi);
	auto GrooveCut2 = new G4Tubs("groovecut2", 0, grooveradius - groovedepth, outerplayerthickness, 0, twopi);
	auto tempGroove1 = new G4SubtractionSolid("tempgroove1", GrooveTorus, GrooveCut1, 0, G4ThreeVector(0., 0., -(groovedepth + groovethickness)));
	auto GrooveLayer = new G4SubtractionSolid("GrooveLayer", tempGroove1, GrooveCut2, 0, G4ThreeVector(0., 0., 0.));
	auto logicGrooveLayer = new G4LogicalVolume(GrooveLayer, matEnGe, "GrooveLayer");

	// outer pLayer
	auto OuterpLayer = new G4Tubs("solidOuterpLayer", 0, grooveradius - groovedepth, outerplayerthickness / 2, 0, twopi);
	auto logicOuterpLayer = new G4LogicalVolume(OuterpLayer, matEnGe, "OuterpLayer");
	physBulk = new G4PVPlacement(0, G4ThreeVector(), logicBulk, "Bulk", logicTotalCrystal, false, 0, checkOverlaps);
	// new G4PVPlacement(0, G4ThreeVector(), logicOuterDeadlayer, "OuterDeadlayer", logicTotalCrystal, false, 0, checkOverlaps);
	// new G4PVPlacement(0, zbulkTransOuterp, logicOuterpLayer, "OuterpLayer", logicTotalCrystal, false, 0, checkOverlaps);
	// new G4PVPlacement(0, zTransGroove, logicGrooveLayer, "GrooveLayer", logicTotalCrystal, false, 0, checkOverlaps);
	return logicTotalCrystal;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructA1(G4double WireLength)
{
	//=======================================
	// A1 rho = 2.8457 g/m, 2.80 g/m from ref.
	//=======================================

	G4double ConductorRadius = 0.0762 * mm / 2;
	G4double JacketRadius = 0.254 * mm / 2;
	G4double BraidThickness = 0.02 * mm;
	G4double BraidRadius = JacketRadius + BraidThickness;
	G4double SubWireRadius = 0.5 * mm / 2;
	G4double WireRadius = 1.25 * mm / 2;
	fWireRadius = WireRadius;
	G4double WireJacketThickness = WireRadius - (1 + sqrt(2)) * SubWireRadius;
	G4Material *JacketMat = matPTFE;

	G4Tubs *solidWire = new G4Tubs("solidWire", 0 * mm, WireRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicWire = new G4LogicalVolume(solidWire, JacketMat, "logicWire");

	G4Tubs *solidWireJacket = new G4Tubs("solidWireJacket", WireRadius - WireJacketThickness, WireRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicWireJacket = new G4LogicalVolume(solidWireJacket, JacketMat, "logicWireJacket");
	G4PVPlacement *physWireJacket = new G4PVPlacement(0, G4ThreeVector(), logicWireJacket, "WireJacket", logicWire, false, 0, CheckOverlaps);

	G4Tubs *solidSubWire = new G4Tubs("solidWire", 0 * mm, SubWireRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicSubWire = new G4LogicalVolume(solidSubWire, JacketMat, "logicWire");
	G4PVPlacement *physSubWire_0 = new G4PVPlacement(0, G4ThreeVector(sqrt(2) * SubWireRadius, 0, 0), logicSubWire, "SubWire_0", logicWire, false, 0, CheckOverlaps);
	G4PVPlacement *physSubWire_1 = new G4PVPlacement(0, G4ThreeVector(0, sqrt(2) * SubWireRadius, 0), logicSubWire, "SubWire_1", logicWire, false, 1, CheckOverlaps);
	G4PVPlacement *physSubWire_2 = new G4PVPlacement(0, G4ThreeVector(-sqrt(2) * SubWireRadius, 0, 0), logicSubWire, "SubWire_2", logicWire, false, 2, CheckOverlaps);
	G4PVPlacement *physSubWire_3 = new G4PVPlacement(0, G4ThreeVector(0, -sqrt(2) * SubWireRadius, 0), logicSubWire, "SubWire_3", logicWire, false, 3, CheckOverlaps);

	G4Tubs *solidConductor = new G4Tubs("solidConductor", 0 * mm, ConductorRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicConductor = new G4LogicalVolume(solidConductor, matCu, "logicConductor");
	G4PVPlacement *physConductor = new G4PVPlacement(0, G4ThreeVector(), logicConductor, "Conductor", logicSubWire, false, 0, CheckOverlaps);

	G4Tubs *solidJacket = new G4Tubs("solidJacket", ConductorRadius, JacketRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicJacket = new G4LogicalVolume(solidJacket, JacketMat, "logicJacket");
	G4PVPlacement *physJacket = new G4PVPlacement(0, G4ThreeVector(), logicJacket, "Jacket", logicSubWire, false, 0, CheckOverlaps);

	G4Tubs *solidBraid = new G4Tubs("solidBraid", JacketRadius, BraidRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicBraid = new G4LogicalVolume(solidBraid, matCu, "logicBraid");
	G4PVPlacement *physBraid = new G4PVPlacement(0, G4ThreeVector(), logicBraid, "Braid", logicSubWire, false, 0, CheckOverlaps);

	G4Tubs *solidOuterJacket = new G4Tubs("solidOuterJacket", BraidRadius, SubWireRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicOuterJacket = new G4LogicalVolume(solidOuterJacket, JacketMat, "logicOuterJacket");
	G4PVPlacement *physOuterJacket = new G4PVPlacement(0, G4ThreeVector(), logicOuterJacket, "OuterJacket", logicSubWire, false, 0, CheckOverlaps);
	return logicWire;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructA2(G4double WireLength)
{
	//=======================================
	// A2 rho = 2.0531 g/m, 2 g/m from ref.
	//=======================================

	G4double ConductorRadius = 0.0762 * mm / 2;
	G4double BraidThickness = 0.04 * mm;
	G4double JacketRadius = 0.68 * mm / 2;
	G4double BraidRadius = JacketRadius + BraidThickness;
	G4double WireRadius = 1 * mm / 2;
	fWireRadius = WireRadius;
	G4Material *JacketMat = matPTFE;

	G4Tubs *solidWire = new G4Tubs("solidJacket", 0 * mm, WireRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicWire = new G4LogicalVolume(solidWire, JacketMat, "logicWire");

	G4Tubs *solidConductor = new G4Tubs("solidConductor", 0 * mm, ConductorRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicConductor = new G4LogicalVolume(solidConductor, matCu, "logicConductor");

	G4Tubs *solidJacket = new G4Tubs("solidJacket", ConductorRadius, JacketRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicJacket = new G4LogicalVolume(solidJacket, JacketMat, "logicJacket");

	G4Tubs *solidBraid = new G4Tubs("solidBraid", JacketRadius, BraidRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicBraid = new G4LogicalVolume(solidBraid, matCu, "logicBraid");

	G4Tubs *solidOuterJacket = new G4Tubs("solidOuterJacket", BraidRadius, WireRadius, WireLength / 2, 0, twopi);
	G4LogicalVolume *logicOuterJacket = new G4LogicalVolume(solidOuterJacket, JacketMat, "logicOuterJacket");

	G4PVPlacement *physConductor = new G4PVPlacement(0, G4ThreeVector(), logicConductor, "Conductor", logicWire, false, 0, CheckOverlaps);
	G4PVPlacement *physJacket = new G4PVPlacement(0, G4ThreeVector(), logicJacket, "Jacket", logicWire, false, 0, CheckOverlaps);
	G4PVPlacement *physBraid = new G4PVPlacement(0, G4ThreeVector(), logicBraid, "Braid", logicWire, false, 0, CheckOverlaps);
	G4PVPlacement *physOuterJacket = new G4PVPlacement(0, G4ThreeVector(), logicOuterJacket, "OuterJacket", logicWire, false, 0, CheckOverlaps);

	return logicWire;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructASICPlate()
{
	G4Box *solidAsicPlate = new G4Box("solidASICPlate", fASICLength / 2, fASICWidth / 2, fASICThickness / 2);
	G4LogicalVolume *logicAsicPlate = new G4LogicalVolume(solidAsicPlate, matSi, "logicASICPlate");

	return logicAsicPlate;
}

// PEN Composite System Design
G4LogicalVolume *CDEXDetectorConstruction::ConstructPENShell()
{
	G4double ISOuterRadius = 44 * mm;
	G4double ISOuterHeight = 49 * mm;
	G4double ISChamfer = 3 * mm;
	G4double asmallvalue = 0.01 * mm;
	auto ISTorus = new G4Torus("ISTorus", 0., ISChamfer, ISOuterRadius - ISChamfer, 0., twopi);
	auto ISInnerTop = new G4Tubs("ISInnerTop", 0., ISOuterRadius - ISChamfer, ISChamfer, 0., twopi);
	auto ISTop = new G4UnionSolid("GeInTemp1", ISInnerTop, ISTorus);
	auto ISCylinder = new G4Tubs("solidInnerShell", 0., ISOuterRadius, ISOuterHeight / 2 - ISChamfer, 0., twopi);

	// auto solidInnerShellSolid = new G4MultiUnion("solidInnerShellSolid");
	G4ThreeVector ISpos0 = G4ThreeVector();
	G4ThreeVector ISpos1 = G4ThreeVector(0., 0., ISOuterHeight / 2 - ISChamfer + asmallvalue);
	G4ThreeVector ISpos2 = G4ThreeVector(0., 0., -ISOuterHeight / 2 + ISChamfer - asmallvalue);
	G4RotationMatrix ISrot1 = G4RotationMatrix();
	G4Transform3D IStr0 = G4Transform3D(ISrot1, ISpos0);
	G4Transform3D IStr1 = G4Transform3D(ISrot1, ISpos1);
	G4Transform3D IStr2 = G4Transform3D(ISrot1, ISpos2);

	auto Temp1 = new G4UnionSolid("Temp1", ISCylinder, ISTop, IStr1);
	auto solidInnerShellSolid = new G4UnionSolid("solidInnerShellSolid", Temp1, ISTop, IStr2);

	// solidInnerShellSolid->AddNode(*ISCylinder, IStr0);
	// solidInnerShellSolid->AddNode(*ISTop, IStr1);
	// solidInnerShellSolid->AddNode(*ISTop, IStr2);
	// solidInnerShellSolid->Voxelize();

	G4ThreeVector pos1 = G4ThreeVector(0., 0., 0.);
	G4RotationMatrix *rot1 = new G4RotationMatrix();
	// rot1->rotateX(90 * degree);
	G4Transform3D OSTr0 = G4Transform3D(*rot1, pos1);

	G4String Ver = "IV";
	auto outermesh = CADMesh::TessellatedMesh::FromSTL("../models/OuterShell-" + Ver + ".stl");
	G4VSolid *solidOuterShell = outermesh->GetSolid();
	// auto innermesh = CADMesh::TessellatedMesh::FromSTL("../models/InnerShellSolid-" + Ver + ".stl");
	// G4VSolid* solidInnerShellSolid = innermesh->GetSolid();

	// auto solidInnerShellVoid = new G4Tubs("solidInnerShell", 0., 40.1 * mm, 20.1 * mm, 0., twopi);
	// auto solidInnerShell = new G4SubtractionSolid("solidPENShell", solidInnerShellSolid, solidInnerShellVoid, 0, G4ThreeVector());

	auto solidPENShell = new G4UnionSolid("solidPENShell", solidInnerShellSolid, solidOuterShell, rot1, pos1);

	auto logicPENShell = new G4LogicalVolume(solidPENShell, matPEN, "logicPENShell");

	return logicPENShell;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructCSGPENShell()
{
	G4double ISOuterRadius = 44.9 * mm;
	G4double ISOuterHeight = 50 * mm;
	G4double ISChamfer = 3 * mm;
	G4double asmallvalue = 0.01 * mm;
	auto ISTorus = new G4Torus("ISTorus", 0., ISChamfer, ISOuterRadius - ISChamfer, 0., twopi);
	auto ISInnerTop = new G4Tubs("ISInnerTop", 0., ISOuterRadius - ISChamfer, ISChamfer, 0., twopi);
	auto ISTop = new G4UnionSolid("GeInTemp1", ISInnerTop, ISTorus);
	auto ISCylinder = new G4Tubs("solidInnerShell", 0., ISOuterRadius, ISOuterHeight / 2 - ISChamfer, 0., twopi);

	auto solidInnerShellSolid = new G4MultiUnion("solidInnerShellSolid");
	G4ThreeVector ISpos0 = G4ThreeVector();
	G4ThreeVector ISpos1 = G4ThreeVector(0., 0., ISOuterHeight / 2 - ISChamfer + asmallvalue);
	G4ThreeVector ISpos2 = G4ThreeVector(0., 0., -ISOuterHeight / 2 + ISChamfer - asmallvalue);
	G4RotationMatrix ISrot1 = G4RotationMatrix();
	G4Transform3D IStr0 = G4Transform3D(ISrot1, ISpos0);
	G4Transform3D IStr1 = G4Transform3D(ISrot1, ISpos1);
	G4Transform3D IStr2 = G4Transform3D(ISrot1, ISpos2);
	solidInnerShellSolid->AddNode(*ISCylinder, IStr0);
	solidInnerShellSolid->AddNode(*ISTop, IStr1);
	solidInnerShellSolid->AddNode(*ISTop, IStr2);
	solidInnerShellSolid->Voxelize();

	auto solidInnerShellVoid = new G4Tubs("solidInnerShell", 0., 40.1 * mm, 20.1 * mm, 0., twopi);
	auto solidInnerShell = new G4SubtractionSolid("solidPENShell", solidInnerShellSolid, solidInnerShellVoid, 0, G4ThreeVector());

	// Part1 Solid
	G4double OSConeHeight0 = 37.313 * mm;
	G4double OSRadius0 = 250 * mm;
	auto OSConeUp0 = new G4Cons("OSConeUp0", 0, OSRadius0, 0, 0, OSConeHeight0 / 2, 0, twopi);
	auto OSConeDown0 = new G4Cons("OSConeDown0", 0, 0, 0, OSRadius0, OSConeHeight0 / 2, 0, twopi);
	// auto OSCone=G4UnionSolid("GeInTemp2", OSCone1, GeInTemp1, 0, G4ThreeVector(0., 0., (GeHeight1 - deadlayerthickness * 2) / 2.0 + lSmallValue));
	auto OSCone0 = new G4MultiUnion("OSCone0");
	G4ThreeVector OSConepos00 = G4ThreeVector(0., 0., OSConeHeight0 / 2);
	G4ThreeVector OSConepos01 = G4ThreeVector(0., 0., -OSConeHeight0 / 2);
	G4RotationMatrix OSConerot0 = G4RotationMatrix();
	G4Transform3D OStr00 = G4Transform3D(OSConerot0, OSConepos00);
	G4Transform3D OStr01 = G4Transform3D(OSConerot0, OSConepos01);
	OSCone0->AddNode(*OSConeUp0, OStr00);
	OSCone0->AddNode(*OSConeDown0, OStr01);
	OSCone0->Voxelize();

	G4double OSOffset0 = 9.98 * mm;
	G4double TrdHeight0 = 155.02 * mm;
	G4double OSHeight0 = 60 * mm;
	auto OSTrd = new G4Trd("OSTrd", OSHeight0 / 2, OSHeight0 / 2, 48.05 * mm, 16.99 * mm, TrdHeight0 / 2); //	12.555 * mm,16.99 * mm,
	auto OSP3 = new G4Tubs("OSP3", 0., 49 * mm, 30 * mm, 0., twopi);
	auto OSCut0 = new G4MultiUnion("OSCut0");
	G4ThreeVector OSCutpos0 = G4ThreeVector(TrdHeight0 / 2 + OSOffset0, 0., 0.);
	G4ThreeVector OSCutpos1 = G4ThreeVector(-TrdHeight0 / 2 - OSOffset0, 0., 0.);
	G4ThreeVector OSCutpos2 = G4ThreeVector(0., 0., 0);
	auto OSCutRot0 = new G4RotationMatrix();
	OSCutRot0->rotateY(90 * degree);
	auto OSCutRot1 = new G4RotationMatrix();
	OSCutRot1->rotateY(-90 * degree);
	auto OSCutRot2 = new G4RotationMatrix();
	G4Transform3D OSCutTr0 = G4Transform3D(*OSCutRot0, OSCutpos0);
	G4Transform3D OSCutTr1 = G4Transform3D(*OSCutRot1, OSCutpos1);
	G4Transform3D OSCutTr2 = G4Transform3D(*OSCutRot2, OSCutpos2);
	OSCut0->AddNode(*OSTrd, OSCutTr0);
	OSCut0->AddNode(*OSTrd, OSCutTr1);
	OSCut0->AddNode(*OSP3, OSCutTr2);
	OSCut0->Voxelize();

	auto solidOuterShellSolid = new G4IntersectionSolid("solidOuterShellSolid", OSCone0, OSCut0);

	///////////////

	// Part2 Void
	G4double OSConeHeight1 = 33.2687 * mm;
	G4double OSRadius1 = 222.903 * mm;
	auto OSConeUp1 = new G4Cons("OSConeUp1", 0, OSRadius1, 0, 0, OSConeHeight1 / 2, 0, twopi);
	auto OSConeDown1 = new G4Cons("OSConeUp1", 0, 0, 0, OSRadius1, OSConeHeight1 / 2, 0, twopi);

	auto OSCone1 = new G4MultiUnion("OSCone1");
	G4ThreeVector OSConepos10 = G4ThreeVector(0., 0., OSConeHeight1 / 2);
	G4ThreeVector OSConepos11 = G4ThreeVector(0., 0., -OSConeHeight1 / 2);
	G4RotationMatrix OSConerot1 = G4RotationMatrix();
	G4Transform3D OStr10 = G4Transform3D(OSConerot1, OSConepos10);
	G4Transform3D OStr11 = G4Transform3D(OSConerot1, OSConepos11);
	OSCone1->AddNode(*OSConeUp1, OStr10);
	OSCone1->AddNode(*OSConeDown1, OStr11);
	OSCone1->Voxelize();

	G4double OSOffset1 = 9.12 * mm;
	G4double TrdHeight1 = 155.88 * mm;
	G4double OSHeight1 = 52 * mm;
	auto OSTrdVoid = new G4Trd("OSTrdVoid", OSHeight1 / 2, OSHeight1 / 2, 43.925 * mm, 12.985 * mm, TrdHeight1 / 2); //	12.555 * mm,16.99 * mm,
	auto OSP3Void = new G4Tubs("OSP3", 0., 45 * mm, 26 * mm, 0., twopi);
	auto OSCut1 = new G4MultiUnion("OSCut1");
	G4ThreeVector OSCutVoidpos0 = G4ThreeVector(TrdHeight1 / 2 + OSOffset1, 0., 0.);
	G4ThreeVector OSCutVoidpos1 = G4ThreeVector(-TrdHeight1 / 2 - OSOffset1, 0., 0.);
	G4ThreeVector OSCutVoidpos2 = G4ThreeVector(0., 0., 0);
	auto OSCutVoidRot0 = new G4RotationMatrix();
	OSCutRot0->rotateY(90 * degree);
	auto OSCutVoidRot1 = new G4RotationMatrix();
	OSCutVoidRot1->rotateY(-90 * degree);
	auto OSCutVoidRot2 = new G4RotationMatrix();
	G4Transform3D OSCutVoidTr0 = G4Transform3D(*OSCutVoidRot0, OSCutVoidpos0);
	G4Transform3D OSCutVoidTr1 = G4Transform3D(*OSCutVoidRot1, OSCutVoidpos0);
	G4Transform3D OSCutVoidTr2 = G4Transform3D(*OSCutVoidRot2, OSCutVoidpos0);
	OSCut1->AddNode(*OSTrdVoid, OSCutVoidTr0);
	OSCut1->AddNode(*OSTrdVoid, OSCutVoidTr1);
	OSCut1->AddNode(*OSP3Void, OSCutVoidTr1);
	OSCut1->Voxelize();

	auto solidOuterShellVoid = new G4IntersectionSolid("solidOuterShellSolid", OSCone1, OSCut1);
	auto solidOuterShell = new G4SubtractionSolid("solidOuterShell", solidOuterShellSolid, solidOuterShellVoid, 0, G4ThreeVector());

	///////////////
	G4ThreeVector position1 = G4ThreeVector(0., 0., 0.);
	auto solidPENShell = new G4UnionSolid("solidPENShell", solidInnerShell, solidOuterShell, 0, position1);

	auto logicPENShell = new G4LogicalVolume(solidPENShell, matPEN, "logicPENShell");
	auto logicOuterShell = new G4LogicalVolume(solidOuterShell, matPEN, "logicOuterShell");
	auto logicInnerShell = new G4LogicalVolume(solidInnerShell, matPEN, "logicInnerShell");
	return logicPENShell;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructInnerShell()
{
	G4String Ver = "IV";
	auto innermesh = CADMesh::TessellatedMesh::FromSTL("../models/InnerShell-" + Ver + "-Standard.stl");
	G4VSolid *solidInnerShell = innermesh->GetSolid();

	auto logicInnerShell = new G4LogicalVolume(solidInnerShell, matPEN, "logicPENShell");
	return logicInnerShell;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructCSGInnerShell()
{
	G4double ISOuterRadius = 44.9 * mm;
	G4double ISOuterHeight = 50 * mm;
	G4double ISChamfer = 3 * mm;
	G4double asmallvalue = 0.01 * mm;
	auto ISTorus = new G4Torus("ISTorus", 0., ISChamfer, ISOuterRadius - ISChamfer, 0., twopi);
	auto ISInnerTop = new G4Tubs("ISInnerTop", 0., ISOuterRadius - ISChamfer, ISChamfer, 0., twopi);
	G4ThreeVector ISpos0 = G4ThreeVector();
	G4ThreeVector ISpos1 = G4ThreeVector(0., 0., ISOuterHeight / 2 - ISChamfer + asmallvalue);
	G4ThreeVector ISpos2 = G4ThreeVector(0., 0., -ISOuterHeight / 2 + ISChamfer - asmallvalue);

	auto ISTop = new G4UnionSolid("ISTop", ISInnerTop, ISTorus);
	auto ISCylinder = new G4Tubs("solidInnerShell", 0., ISOuterRadius, ISOuterHeight / 2 - ISChamfer, 0., twopi);

	// auto Temp1 = new G4UnionSolid("Temp1", ISCylinder, ISTop, 0, ISpos1);
	// auto solidInnerShellSolid = new G4UnionSolid("solidInnerShellVoid", Temp1, ISTop, 0, ISpos2);

	auto solidInnerShellSolid = new G4MultiUnion("solidInnerShellSolid");
	G4RotationMatrix ISrot1 = G4RotationMatrix();
	G4Transform3D IStr0 = G4Transform3D(ISrot1, ISpos0);
	G4Transform3D IStr1 = G4Transform3D(ISrot1, ISpos1);
	G4Transform3D IStr2 = G4Transform3D(ISrot1, ISpos2);
	solidInnerShellSolid->AddNode(*ISCylinder, IStr0);
	solidInnerShellSolid->AddNode(*ISTop, IStr1);
	solidInnerShellSolid->AddNode(*ISTop, IStr2);
	solidInnerShellSolid->Voxelize();

	auto solidInnerShellVoid = new G4Tubs("solidInnerShell", 0., 40.5 * mm, 20.5 * mm, 0., twopi);
	auto solidInnerShell = new G4SubtractionSolid("solidPENShell", solidInnerShellSolid, solidInnerShellVoid, 0, G4ThreeVector());

	auto logicInnerShell = new G4LogicalVolume(solidInnerShellSolid, matPEN, "logicInnerShell");
	return logicInnerShell;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructOuterShell()
{
	G4String Ver = "IV";
	auto outermesh = CADMesh::TessellatedMesh::FromSTL("../models/OuterShell-" + Ver + ".stl");
	G4VSolid *solidOuterShell = outermesh->GetSolid();

	auto logicOuterShell = new G4LogicalVolume(solidOuterShell, matPEN, "logicOuterShell");
	return logicOuterShell;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructCSGOuterShell()
{

	// Part1 Solid
	G4double OSConeHeight0 = 37.313 * mm;
	G4double OSRadius0 = 250 * mm;
	G4double ASmallValue = 0.1 * mm;
	auto OSConeUp0 = new G4Cons("OSConeUp0", 0, OSRadius0, 0, 0, OSConeHeight0 / 2, 0, twopi);
	auto OSConeDown0 = new G4Cons("OSConeDown0", 0, 0, 0, OSRadius0, OSConeHeight0 / 2, 0, twopi);
	// auto OSCone=G4UnionSolid("GeInTemp2", OSCone1, GeInTemp1, 0, G4ThreeVector(0., 0., (GeHeight1 - deadlayerthickness * 2) / 2.0 + lSmallValue));
	G4ThreeVector OSConepos00 = G4ThreeVector(0., 0., OSConeHeight0 / 2);
	G4ThreeVector OSConepos01 = G4ThreeVector(0., 0., -OSConeHeight0 / 2);
	auto OSCone0 = new G4MultiUnion("OSCone0");
	G4RotationMatrix OSConerot0 = G4RotationMatrix();
	G4Transform3D OStr00 = G4Transform3D(OSConerot0, OSConepos00);
	G4Transform3D OStr01 = G4Transform3D(OSConerot0, OSConepos01);
	OSCone0->AddNode(*OSConeUp0, OStr00);
	OSCone0->AddNode(*OSConeDown0, OStr01);
	OSCone0->Voxelize();
	// auto OSCone0 = new G4IntersectionSolid("Temp1", OSConeUp0, OSConeDown0, 0, OSConepos01);
	// auto solidInnerShellSolid = new G4UnionSolid("solidInnerShellVoid", Temp1, ISTop, 0, ISpos2);
	G4double OSOffset0 = 9.98 * mm;
	G4double TrdHeight0 = 155.02 * mm;
	G4double OSHeight0 = 60 * mm;
	auto OSTrd = new G4Trd("OSTrd", OSHeight0 / 2, OSHeight0 / 2, 48.05 * mm, 16.99 * mm, TrdHeight0 / 2); //	12.555 * mm,16.99 * mm,
	auto OSP3 = new G4Tubs("OSP3", 0., 49 * mm, 30 * mm, 0., twopi);
	auto OSCut0 = new G4MultiUnion("OSCut0");
	G4ThreeVector OSCutpos0 = G4ThreeVector(TrdHeight0 / 2 + OSOffset0, 0., 0.);
	G4ThreeVector OSCutpos1 = G4ThreeVector(-TrdHeight0 / 2 - OSOffset0, 0., 0.);
	G4ThreeVector OSCutpos2 = G4ThreeVector(0., 0., 0);
	auto OSCutRot0 = new G4RotationMatrix();
	OSCutRot0->rotateY(90 * degree);
	auto OSCutRot1 = new G4RotationMatrix();
	OSCutRot1->rotateY(-90 * degree);
	auto OSCutRot2 = new G4RotationMatrix();
	G4Transform3D OSCutTr0 = G4Transform3D(*OSCutRot0, OSCutpos0);
	G4Transform3D OSCutTr1 = G4Transform3D(*OSCutRot1, OSCutpos1);
	G4Transform3D OSCutTr2 = G4Transform3D(*OSCutRot2, OSCutpos2);
	OSCut0->AddNode(*OSTrd, OSCutTr0);
	OSCut0->AddNode(*OSTrd, OSCutTr1);
	OSCut0->AddNode(*OSP3, OSCutTr2);
	OSCut0->Voxelize();

	auto solidOuterShellSolidP1 = new G4IntersectionSolid("solidOuterShellSolid", OSCut0, OSConeUp0);
	auto solidOuterShellSolidP2 = new G4IntersectionSolid("solidOuterShellSolid", OSCut0, OSConeDown0);

	///////////////

	// Part2 Void
	G4double OSConeHeight1 = 33.2687 * mm;
	G4double OSRadius1 = 222.903 * mm;
	auto OSConeUp1 = new G4Cons("OSConeUp1", 0, OSRadius1, 0, 0, OSConeHeight1 / 2, 0, twopi);
	auto OSConeDown1 = new G4Cons("OSConeUp1", 0, 0, 0, OSRadius1, OSConeHeight1 / 2, 0, twopi);
	// auto OSCone=G4UnionSolid("GeInTemp2", OSCone1, GeInTemp1, 0, G4ThreeVector(0., 0., (GeHeight1 - deadlayerthickness * 2) / 2.0 + lSmallValue));
	auto OSCone1 = new G4MultiUnion("OSCone1");
	G4ThreeVector OSConepos10 = G4ThreeVector(0., 0., OSConeHeight1 / 2 - ASmallValue);
	G4ThreeVector OSConepos11 = G4ThreeVector(0., 0., -OSConeHeight1 / 2 + ASmallValue);
	G4RotationMatrix OSConerot1 = G4RotationMatrix();
	G4Transform3D OStr10 = G4Transform3D(OSConerot1, OSConepos10);
	G4Transform3D OStr11 = G4Transform3D(OSConerot1, OSConepos11);
	OSCone1->AddNode(*OSConeUp1, OStr10);
	OSCone1->AddNode(*OSConeDown1, OStr11);
	OSCone1->Voxelize();

	G4double OSOffset1 = 9.12 * mm;
	G4double TrdHeight1 = 155.88 * mm;
	G4double OSHeight1 = 52 * mm;
	auto OSTrdVoid = new G4Trd("OSTrdVoid", OSHeight1 / 2, OSHeight1 / 2, 43.925 * mm, 12.985 * mm, TrdHeight1 / 2); //	12.555 * mm,16.99 * mm,
	auto OSP3Void = new G4Tubs("OSP3", 0., 45 * mm, OSHeight1 / 2, 0., twopi);
	auto OSCut1 = new G4MultiUnion("OSCut1");
	G4ThreeVector OSCutVoidpos0 = G4ThreeVector(TrdHeight1 / 2 + OSOffset1 + ASmallValue, 0., 0.);
	G4ThreeVector OSCutVoidpos1 = G4ThreeVector(-TrdHeight1 / 2 - OSOffset1 - ASmallValue, 0., 0.);
	G4ThreeVector OSCutVoidpos2 = G4ThreeVector(0., 0., 0);
	auto OSCutVoidRot0 = new G4RotationMatrix();
	OSCutVoidRot0->rotateY(90 * degree);
	auto OSCutVoidRot1 = new G4RotationMatrix();
	OSCutVoidRot1->rotateY(-90 * degree);
	auto OSCutVoidRot2 = new G4RotationMatrix();
	G4Transform3D OSCutVoidTr0 = G4Transform3D(*OSCutVoidRot0, OSCutVoidpos0);
	G4Transform3D OSCutVoidTr1 = G4Transform3D(*OSCutVoidRot1, OSCutVoidpos1);
	G4Transform3D OSCutVoidTr2 = G4Transform3D(*OSCutVoidRot2, OSCutVoidpos2);
	OSCut1->AddNode(*OSTrdVoid, OSCutVoidTr0);
	OSCut1->AddNode(*OSTrdVoid, OSCutVoidTr1);
	OSCut1->AddNode(*OSP3Void, OSCutVoidTr2);
	OSCut1->Voxelize();

	auto solidOuterShellVoidP1 = new G4IntersectionSolid("solidOuterShellSolid", OSCut1, OSConeUp1);
	auto solidOuterShellVoidP2 = new G4IntersectionSolid("solidOuterShellSolid", OSCut1, OSConeDown1);

	auto solidOuterShellP1 = new G4SubtractionSolid("solidOuterShell", solidOuterShellSolidP1, solidOuterShellVoidP1, 0, G4ThreeVector(0., 0., -(OSConeHeight0 - OSConeHeight1) / 2));
	auto solidOuterShellP2 = new G4SubtractionSolid("solidOuterShell", solidOuterShellSolidP2, solidOuterShellVoidP2, 0, G4ThreeVector(0., 0., (OSConeHeight0 - OSConeHeight1) / 2));

	auto solidOuterShell = new G4UnionSolid("solidOuterShell", solidOuterShellP1, solidOuterShellP2, 0, G4ThreeVector(0., 0., -OSConeHeight0));

	auto logicOuterShell = new G4LogicalVolume(solidOuterShell, matPEN, "logicPENShell");
	return logicOuterShell;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructOuterReflector()
{
	auto outermesh = CADMesh::TessellatedMesh::FromSTL("../models/OuterShell-IV.stl");
	G4VSolid *solidOuterShell = outermesh->GetSolid();
	// G4ThreeVector position1 = G4ThreeVector(0., -4 * mm, 0.);
	// auto solidPENShell = new G4UnionSolid("solidPENShell", solidInnerShell, solidOuterShell, 0, position1);

	auto reflectormesh = CADMesh::TessellatedMesh::FromSTL("../models/OuterReflector-IV.stl");
	G4VSolid *solidTemp = reflectormesh->GetSolid();
	auto solidReflector = new G4SubtractionSolid("solidReflector", solidTemp, solidOuterShell, 0, G4ThreeVector());
	auto logicReflector = new G4LogicalVolume(solidReflector, matLN2, "logicReflector");
	return logicReflector;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructInnerReflector()
{
	auto outermesh = CADMesh::TessellatedMesh::FromSTL("../models/OuterShell-IV.stl");
	G4VSolid *solidOuterShell = outermesh->GetSolid();
	// G4ThreeVector position1 = G4ThreeVector(0., -4 * mm, 0.);
	// auto solidPENShell = new G4UnionSolid("solidPENShell", solidInnerShell, solidOuterShell, 0, position1);

	auto reflectormesh = CADMesh::TessellatedMesh::FromSTL("../models/InnerReflector-IV.stl");
	G4VSolid *solidTemp = reflectormesh->GetSolid();
	auto solidReflector = new G4SubtractionSolid("solidReflector", solidTemp, solidOuterShell, 0, G4ThreeVector());
	auto logicReflector = new G4LogicalVolume(solidReflector, matLN2, "logicReflector");
	return logicReflector;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructReflector()
{
	auto reflectormesh = CADMesh::TessellatedMesh::FromSTL("../models/Reflector-IV.stl");
	G4VSolid *solidReflector = reflectormesh->GetSolid();
	auto logicReflector = new G4LogicalVolume(solidReflector, matLN2, "logicReflector");
	return logicReflector;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructContainerBrick()
{
	G4String Ver = "IV";
	auto ContainerCrystalmesh = CADMesh::TessellatedMesh::FromSTL("../models/ContainerCrystal-" + Ver + ".stl");
	auto ContainerBrickmesh = CADMesh::TessellatedMesh::FromSTL("../models/ContainerSolid-" + Ver + ".stl");
	G4VSolid *solidContainerCrystal = ContainerCrystalmesh->GetSolid();
	G4VSolid *solidContainerBrick = ContainerBrickmesh->GetSolid();

	auto solidContainer = new G4SubtractionSolid("solidContainer", solidContainerBrick, solidContainerCrystal, 0, G4ThreeVector(0, 1 * cm, 0));
	auto logicContainerBrick = new G4LogicalVolume(solidContainerBrick, matPEN, "logicContainerBrick");
	logicContainerCrystal = new G4LogicalVolume(solidContainerCrystal, matLAr, "logicContainerCrystal");
	// auto logicSArContainer = new G4LogicalVolume(solidSArCrystal, matPEN, "logicSArContainer");

	physContainerCrystal = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicContainerCrystal, "ContainerCrystal", logicContainerBrick, false, 0, CheckOverlaps);
	// auto physSArContainer = new G4PVPlacement(0, G4ThreeVector(), logicSArContainer, "SArContainer", logicSArBrick, false, 0, CheckOverlaps);

	return logicContainerBrick;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructStringBoxBrick()
{
	G4String Ver = "IV";
	auto StringBoxCrystalmesh = CADMesh::TessellatedMesh::FromSTL("../models/StringBoxCrystal-" + Ver + ".stl");
	auto StringBoxBrickmesh = CADMesh::TessellatedMesh::FromSTL("../models/StringBoxSolid-" + Ver + ".stl");
	G4VSolid *solidStringBoxCrystal = StringBoxCrystalmesh->GetSolid();
	G4VSolid *solidStringBoxBrick = StringBoxBrickmesh->GetSolid();

	auto solidStringBox = new G4SubtractionSolid("solidStringBox", solidStringBoxBrick, solidStringBoxCrystal, 0, G4ThreeVector(0, 1 * cm, 0));
	auto logicStringBoxBrick = new G4LogicalVolume(solidStringBoxBrick, matPEN, "logicStringBoxBrick");
	logicStringBoxCrystal = new G4LogicalVolume(solidStringBoxCrystal, matLAr, "logicStringBoxCrystal");
	// auto logicSArContainer = new G4LogicalVolume(solidSArCrystal, matPEN, "logicSArContainer");

	physStringBoxCrystal = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicStringBoxCrystal, "StringBoxCrystal", logicStringBoxBrick, false, 0, CheckOverlaps);
	// auto physSArContainer = new G4PVPlacement(0, G4ThreeVector(), logicSArContainer, "SArContainer", logicSArBrick, false, 0, CheckOverlaps);

	G4OpticalSurface *PEN_LN2_Ref = new G4OpticalSurface("PEN_LN2_Ref");
	PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
	PEN_LN2_Ref->SetModel(LUT);
	PEN_LN2_Ref->SetFinish(PolishedESR_LUT);

	G4OpticalSurface *PEN_SAr = new G4OpticalSurface("PEN_SAr");
	PEN_SAr->SetType(dielectric_LUTDAVIS);
	PEN_SAr->SetModel(LUT);
	PEN_SAr->SetFinish(Polished_LUT);

	G4LogicalSkinSurface *StringBoxCrystal_LSS = new G4LogicalSkinSurface("SiPM_SAr_LBS_0", logicStringBoxCrystal, PEN_SAr);
	G4LogicalSkinSurface *StringBoxBrick_LSS = new G4LogicalSkinSurface("SiPM_SAr_LBS_0", logicStringBoxBrick, PEN_LN2_Ref);

	return logicStringBoxBrick;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructStringBox()
{
	G4String Ver = "IV";
	auto StringBoxCrystalmesh = CADMesh::TessellatedMesh::FromSTL("../models/StringBoxCrystal-" + Ver + ".stl");
	auto StringBoxBrickmesh = CADMesh::TessellatedMesh::FromSTL("../models/StringBoxSolid-" + Ver + ".stl");
	G4VSolid *solidStringBoxCrystal = StringBoxCrystalmesh->GetSolid();
	G4VSolid *solidStringBoxBrick = StringBoxBrickmesh->GetSolid();

	auto solidStringBox = new G4SubtractionSolid("solidStringBox", solidStringBoxBrick, solidStringBoxCrystal, 0, G4ThreeVector(0, 1 * cm, 0));
	auto logicStringBox = new G4LogicalVolume(solidStringBox, matPEN, "logicStringBox");

	G4OpticalSurface *PEN_LN2_Ref = new G4OpticalSurface("PEN_LN2_Ref");
	PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
	PEN_LN2_Ref->SetModel(LUT);
	PEN_LN2_Ref->SetFinish(PolishedESR_LUT);

	G4OpticalSurface *PEN_SAr = new G4OpticalSurface("PEN_SAr");
	PEN_SAr->SetType(dielectric_LUTDAVIS);
	PEN_SAr->SetModel(LUT);
	PEN_SAr->SetFinish(Polished_LUT);

	G4LogicalSkinSurface *StringBox_LSS = new G4LogicalSkinSurface("StringBox_LSS", logicStringBox, PEN_SAr);

	return logicStringBox;
}

// Construct Light Readout
G4AssemblyVolume *CDEXDetectorConstruction::ConstructSiPMArray()
{
	G4double SiPMThickness = 1 * mm;

	G4double SiPMWidth = 1.2 * cm;
	G4double SiPMLength = 1.5 * cm;
	G4Box *solidSiPM = new G4Box("solidSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2);
	G4LogicalVolume *logicSiPM = new G4LogicalVolume(solidSiPM, matSi, "logicSiPM");
	G4double offset1 = SiPMWidth / 2 + 0.3 * mm;
	G4double offset2 = SiPMLength / 2 + 0.3 * mm;

	G4AssemblyVolume *logicSiPMArray = new G4AssemblyVolume();

	// Rotation and translation of a plate inside the assembly
	G4RotationMatrix *RotSiPM = new G4RotationMatrix();
	G4ThreeVector PosSiPM0 = G4ThreeVector(offset1, offset2, 0);
	G4ThreeVector PosSiPM1 = G4ThreeVector(offset1, offset2, 0);
	G4ThreeVector PosSiPM2 = G4ThreeVector(offset1, offset2, 0);
	G4ThreeVector PosSiPM3 = G4ThreeVector(offset1, offset2, 0);
	G4Transform3D TrSiPM0 = G4Transform3D(*RotSiPM, PosSiPM0);
	G4Transform3D TrSiPM1 = G4Transform3D(*RotSiPM, PosSiPM1);
	G4Transform3D TrSiPM2 = G4Transform3D(*RotSiPM, PosSiPM2);
	G4Transform3D TrSiPM3 = G4Transform3D(*RotSiPM, PosSiPM3);

	logicSiPMArray->AddPlacedVolume(logicSiPM, TrSiPM0);
	logicSiPMArray->AddPlacedVolume(logicSiPM, TrSiPM1);
	logicSiPMArray->AddPlacedVolume(logicSiPM, TrSiPM2);
	logicSiPMArray->AddPlacedVolume(logicSiPM, TrSiPM3);
	G4OpticalSurface *SiPM_LN2 = new G4OpticalSurface("SiPM_LN2");
	SiPM_LN2->SetType(dielectric_LUTDAVIS);
	SiPM_LN2->SetModel(DAVIS);
	SiPM_LN2->SetFinish(Detector_LUT);

	const G4int NUMENTRIES_CHIP = 11;
	const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
	G4double sipm_pp[NUMENTRIES_CHIP] = {hc / 600. * eV, hc / 590. * eV, hc / 580. * eV, hc / 570. * eV, hc / 560. * eV, hc / 550. * eV, hc / 540. * eV, hc / 530. * eV, hc / 520. * eV, hc / 510. * eV, hc / 500. * eV};
	G4double sipm_sl[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_ss[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_bs[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_rindex[NUMENTRIES_CHIP] = {1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406};
	G4double sipm_reflectivity[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// G4double sipm_efficiency[NUMENTRIES_CHIP] = {0.20,0.21,0.23,0.25,0.26,0.28,0.30,0.32,0.34,0.36,0.38};
	G4double sipm_efficiency[NUMENTRIES_CHIP] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	G4MaterialPropertiesTable *SIPM_MPT_Surf = new G4MaterialPropertiesTable();
	// SIPM_MPT_Surf->AddProperty("SPECULARLOBECONSTANT",sipm_pp,sipm_sl,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("SPECULARSPIKECONSTANT",sipm_pp,sipm_ss,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("BACKSCATTERCONSTANT",sipm_pp,sipm_bs,NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("REFLECTIVITY", sipm_pp, sipm_reflectivity, NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("EFFICIENCY", sipm_pp, sipm_efficiency, NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("RINDEX",sipm_pp,sipm_rindex,NUMENTRIES_CHIP);

	SiPM_LN2->SetMaterialPropertiesTable(SIPM_MPT_Surf);

	G4LogicalSkinSurface *SiPM_SAr_LBS_0 = new G4LogicalSkinSurface("SiPM_SAr_LBS_0", logicSiPM, SiPM_LN2);

	return logicSiPMArray;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructSiPMArrayLV()
{
	G4double SiPMThickness = 0.5 * mm;
	G4double TPBThickness = 2 * micrometer;
	G4Box *solidSiPMArray = new G4Box("solidSiPMArray", 3 * cm, 3 * cm, SiPMThickness / 2 + 0.5 * mm);
	G4LogicalVolume *logicSiPMArray = new G4LogicalVolume(solidSiPMArray, matLAr, "logicSiPMArray");

	G4double SiPMWidth = 1.2 * cm;
	G4double SiPMLength = 1.5 * cm;

	G4Box *solidSiPM = new G4Box("solidSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2);
	G4LogicalVolume *logicSiPM = new G4LogicalVolume(solidSiPM, matSi, "logicSiPM");
	G4double offset1 = SiPMWidth / 2 + 0.3 * mm;
	G4double offset2 = SiPMLength / 2 + 0.3 * mm;

	physSiPM0 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, 0), logicSiPM, "physSiPM0", logicSiPMArray, false, 0, CheckOverlaps);
	physSiPM1 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, 0), logicSiPM, "physSiPM1", logicSiPMArray, false, 1, CheckOverlaps);
	physSiPM2 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, 0), logicSiPM, "physSiPM2", logicSiPMArray, false, 2, CheckOverlaps);
	physSiPM3 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, 0), logicSiPM, "physSiPM3", logicSiPMArray, false, 3, CheckOverlaps);

	G4OpticalSurface *SiPM_LN2 = new G4OpticalSurface("SiPM_LN2");
	SiPM_LN2->SetType(dielectric_LUTDAVIS);
	SiPM_LN2->SetModel(DAVIS);
	SiPM_LN2->SetFinish(Detector_LUT);

	G4OpticalSurface *SAr_SAr = new G4OpticalSurface("SAr_SAr");
	SAr_SAr->SetType(dielectric_dielectric);
	SAr_SAr->SetModel(unified);
	SAr_SAr->SetFinish(polished);

	const G4int NUMENTRIES_CHIP = 11;
	const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
	G4double sipm_pp[NUMENTRIES_CHIP] = {hc / 600. * eV, hc / 590. * eV, hc / 580. * eV, hc / 570. * eV, hc / 560. * eV, hc / 550. * eV, hc / 540. * eV, hc / 530. * eV, hc / 520. * eV, hc / 510. * eV, hc / 500. * eV};
	G4double sipm_sl[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_ss[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_bs[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_rindex[NUMENTRIES_CHIP] = {1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406};
	G4double sipm_reflectivity[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// G4double sipm_efficiency[NUMENTRIES_CHIP] = {0.20,0.21,0.23,0.25,0.26,0.28,0.30,0.32,0.34,0.36,0.38};
	G4double sipm_efficiency[NUMENTRIES_CHIP] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	G4MaterialPropertiesTable *SIPM_MPT_Surf = new G4MaterialPropertiesTable();
	// SIPM_MPT_Surf->AddProperty("SPECULARLOBECONSTANT",sipm_pp,sipm_sl,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("SPECULARSPIKECONSTANT",sipm_pp,sipm_ss,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("BACKSCATTERCONSTANT",sipm_pp,sipm_bs,NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("REFLECTIVITY", sipm_pp, sipm_reflectivity, NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("EFFICIENCY", sipm_pp, sipm_efficiency, NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("RINDEX",sipm_pp,sipm_rindex,NUMENTRIES_CHIP);

	SiPM_LN2->SetMaterialPropertiesTable(SIPM_MPT_Surf);

	G4LogicalSkinSurface *SiPM_LSS = new G4LogicalSkinSurface("SiPM_LSS", logicSiPM, SiPM_LN2);
	G4LogicalSkinSurface *SiPMArray_LSS = new G4LogicalSkinSurface("SiPMArray_LSS", logicSiPMArray, SAr_SAr);
	return logicSiPMArray;
}

G4AssemblyVolume *CDEXDetectorConstruction::ConstructContainerSiPMArray()
{
	G4double SiPMThickness = 0.5 * mm;
	G4double TPBThickness = 2 * micrometer;
	G4Box *solidSiPMArray = new G4Box("solidSiPMArray", 3 * cm, 3 * cm, SiPMThickness / 2 + 0.5 * mm);
	// G4LogicalVolume* logicSiPMArray = new G4LogicalVolume(solidSiPMArray, matLAr, "logicSiPMArray");

	G4double SiPMWidth = 1.2 * cm;
	G4double SiPMLength = 1.5 * cm;

	G4Box *solidContainerSiPM = new G4Box("solidSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2);
	G4LogicalVolume *logicContainerSiPM = new G4LogicalVolume(solidContainerSiPM, matSi, "logicContainerSiPM");
	G4Box *solidTPB = new G4Box("solidTPB", SiPMWidth / 2, SiPMLength / 2, TPBThickness / 2 + SiPMThickness / 2);
	G4LogicalVolume *logicTPB = new G4LogicalVolume(solidTPB, matTPB, "logicTPB");
	G4double offset1 = SiPMWidth / 2 + 0.3 * mm;
	G4double offset2 = SiPMLength / 2 + 0.3 * mm;

	physContainerSiPM0 = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicContainerSiPM, "ContainerSiPM0", logicTPB, false, 0, CheckOverlaps);

	G4AssemblyVolume *logicContainerSiPMArray = new G4AssemblyVolume();

	// Rotation and translation of a plate inside the assembly
	G4RotationMatrix *RotSiPM = new G4RotationMatrix();
	G4ThreeVector PosSiPM0 = G4ThreeVector(offset1, offset2, 0);
	G4ThreeVector PosSiPM1 = G4ThreeVector(offset1, -offset2, 0);
	G4ThreeVector PosSiPM2 = G4ThreeVector(-offset1, offset2, 0);
	G4ThreeVector PosSiPM3 = G4ThreeVector(-offset1, -offset2, 0);
	G4Transform3D TrSiPM0 = G4Transform3D(*RotSiPM, PosSiPM0);
	G4Transform3D TrSiPM1 = G4Transform3D(*RotSiPM, PosSiPM1);
	G4Transform3D TrSiPM2 = G4Transform3D(*RotSiPM, PosSiPM2);
	G4Transform3D TrSiPM3 = G4Transform3D(*RotSiPM, PosSiPM3);

	logicContainerSiPMArray->AddPlacedVolume(logicTPB, TrSiPM0);
	logicContainerSiPMArray->AddPlacedVolume(logicTPB, TrSiPM1);
	logicContainerSiPMArray->AddPlacedVolume(logicTPB, TrSiPM2);
	logicContainerSiPMArray->AddPlacedVolume(logicTPB, TrSiPM3);

	G4OpticalSurface *SiPM_SAr = new G4OpticalSurface("SiPM_SAr");
	SiPM_SAr->SetType(dielectric_LUTDAVIS);
	SiPM_SAr->SetModel(DAVIS);
	SiPM_SAr->SetFinish(Detector_LUT);

	const G4int NUMENTRIES_CHIP = 11;
	const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
	G4double sipm_pp[NUMENTRIES_CHIP] = {hc / 600. * eV, hc / 590. * eV, hc / 580. * eV, hc / 570. * eV, hc / 560. * eV, hc / 550. * eV, hc / 540. * eV, hc / 530. * eV, hc / 520. * eV, hc / 510. * eV, hc / 500. * eV};
	G4double sipm_sl[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_ss[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_bs[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_rindex[NUMENTRIES_CHIP] = {1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406};
	G4double sipm_reflectivity[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// G4double sipm_efficiency[NUMENTRIES_CHIP] = {0.20,0.21,0.23,0.25,0.26,0.28,0.30,0.32,0.34,0.36,0.38};
	G4double sipm_efficiency[NUMENTRIES_CHIP] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	G4MaterialPropertiesTable *SIPM_MPT_Surf = new G4MaterialPropertiesTable();
	// SIPM_MPT_Surf->AddProperty("SPECULARLOBECONSTANT",sipm_pp,sipm_sl,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("SPECULARSPIKECONSTANT",sipm_pp,sipm_ss,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("BACKSCATTERCONSTANT",sipm_pp,sipm_bs,NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("REFLECTIVITY", sipm_pp, sipm_reflectivity, NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("EFFICIENCY", sipm_pp, sipm_efficiency, NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("RINDEX",sipm_pp,sipm_rindex,NUMENTRIES_CHIP);

	SiPM_SAr->SetMaterialPropertiesTable(SIPM_MPT_Surf);

	G4OpticalSurface *TPB_SAr = new G4OpticalSurface("SiPM_SAr");
	TPB_SAr->SetType(dielectric_LUTDAVIS);
	TPB_SAr->SetModel(DAVIS);
	TPB_SAr->SetFinish(Detector_LUT);

	G4LogicalSkinSurface *SiPM_SAr_LBS_0 = new G4LogicalSkinSurface("SiPM_SAr_LBS_0", logicContainerSiPM, SiPM_SAr);
	G4LogicalSkinSurface *TPB_SAr_LBS_0 = new G4LogicalSkinSurface("TPB_SAr_LBS_0", logicTPB, TPB_SAr);

	return logicContainerSiPMArray;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructContainerSiPMArrayLV()
{
	G4double SiPMThickness = 0.5 * mm;
	G4double TPBThickness = 2 * micrometer;
	G4Box *solidSiPMArray = new G4Box("solidSiPMArray", 3 * cm, 3 * cm, SiPMThickness / 2 + 0.5 * mm);
	G4LogicalVolume *logicContainerSiPMArray = new G4LogicalVolume(solidSiPMArray, matLAr, "logicSiPMArray");

	G4double SiPMWidth = 1.2 * cm;
	G4double SiPMLength = 1.5 * cm;

	G4Box *solidSiPM = new G4Box("solidSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2);
	G4LogicalVolume *logicContainerSiPM = new G4LogicalVolume(solidSiPM, matSi, "logicSiPM");
	G4Box *solidTPB = new G4Box("solidTPB", SiPMWidth / 2, SiPMLength / 2, TPBThickness / 2);
	G4LogicalVolume *logicTPB = new G4LogicalVolume(solidTPB, matTPB, "logicTPB");
	G4double offset1 = SiPMWidth / 2 + 0.3 * mm;
	G4double offset2 = SiPMLength / 2 + 0.3 * mm;

	physContainerSiPM0 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, 0), logicContainerSiPM, "physSiPM0", logicContainerSiPMArray, false, 0, CheckOverlaps);
	physContainerSiPM1 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, 0), logicContainerSiPM, "physSiPM1", logicContainerSiPMArray, false, 1, CheckOverlaps);
	physContainerSiPM2 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, 0), logicContainerSiPM, "physSiPM2", logicContainerSiPMArray, false, 2, CheckOverlaps);
	physContainerSiPM3 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, 0), logicContainerSiPM, "physSiPM3", logicContainerSiPMArray, false, 3, CheckOverlaps);

	auto physTPB0 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB0", logicContainerSiPMArray, false, 0, CheckOverlaps);
	auto physTPB1 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB1", logicContainerSiPMArray, false, 1, CheckOverlaps);
	auto physTPB2 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB2", logicContainerSiPMArray, false, 2, CheckOverlaps);
	auto physTPB3 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB3", logicContainerSiPMArray, false, 3, CheckOverlaps);
	auto physTPB4 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB4", logicContainerSiPMArray, false, 4, CheckOverlaps);
	auto physTPB5 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB5", logicContainerSiPMArray, false, 5, CheckOverlaps);
	auto physTPB6 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB6", logicContainerSiPMArray, false, 6, CheckOverlaps);
	auto physTPB7 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB7", logicContainerSiPMArray, false, 7, CheckOverlaps);

	G4OpticalSurface *SiPM_SAr = new G4OpticalSurface("SiPM_SAr");
	SiPM_SAr->SetType(dielectric_LUTDAVIS);
	SiPM_SAr->SetModel(DAVIS);
	SiPM_SAr->SetFinish(Detector_LUT);

	const G4int NUMENTRIES_CHIP = 11;
	const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
	G4double sipm_pp[NUMENTRIES_CHIP] = {hc / 600. * eV, hc / 590. * eV, hc / 580. * eV, hc / 570. * eV, hc / 560. * eV, hc / 550. * eV, hc / 540. * eV, hc / 530. * eV, hc / 520. * eV, hc / 510. * eV, hc / 500. * eV};
	G4double sipm_sl[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_ss[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_bs[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_rindex[NUMENTRIES_CHIP] = {1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406};
	G4double sipm_reflectivity[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// G4double sipm_efficiency[NUMENTRIES_CHIP] = {0.20,0.21,0.23,0.25,0.26,0.28,0.30,0.32,0.34,0.36,0.38};
	G4double sipm_efficiency[NUMENTRIES_CHIP] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	G4MaterialPropertiesTable *SIPM_MPT_Surf = new G4MaterialPropertiesTable();
	// SIPM_MPT_Surf->AddProperty("SPECULARLOBECONSTANT",sipm_pp,sipm_sl,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("SPECULARSPIKECONSTANT",sipm_pp,sipm_ss,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("BACKSCATTERCONSTANT",sipm_pp,sipm_bs,NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("REFLECTIVITY", sipm_pp, sipm_reflectivity, NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("EFFICIENCY", sipm_pp, sipm_efficiency, NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("RINDEX",sipm_pp,sipm_rindex,NUMENTRIES_CHIP);

	SiPM_SAr->SetMaterialPropertiesTable(SIPM_MPT_Surf);

	G4OpticalSurface *TPB_SAr = new G4OpticalSurface("SiPM_SAr");
	TPB_SAr->SetType(dielectric_dielectric);
	TPB_SAr->SetModel(unified);
	TPB_SAr->SetFinish(polished);

	G4OpticalSurface *SAr_SAr = new G4OpticalSurface("SAr_SAr");
	SAr_SAr->SetType(dielectric_dielectric);
	SAr_SAr->SetModel(unified);
	SAr_SAr->SetFinish(polished);

	G4LogicalSkinSurface *SiPM_LSS = new G4LogicalSkinSurface("SiPM_LSS", logicContainerSiPM, SiPM_SAr);
	G4LogicalSkinSurface *TPB_LSS = new G4LogicalSkinSurface("TPB_LSS", logicTPB, TPB_SAr);
	G4LogicalSkinSurface *SiOMArray_LSS = new G4LogicalSkinSurface("SiOMArray_LSS", logicContainerSiPMArray, SAr_SAr);

	return logicContainerSiPMArray;
}

G4AssemblyVolume *CDEXDetectorConstruction::ConstructSArSiPMArray()
{
	G4double SiPMThickness = 0.5 * mm;
	G4double TPBThickness = 2 * micrometer;
	G4Box *solidSiPMArray = new G4Box("solidSiPMArray", 3 * cm, 3 * cm, SiPMThickness / 2 + 0.5 * mm);
	// G4LogicalVolume* logicSiPMArray = new G4LogicalVolume(solidSiPMArray, matLAr, "logicSiPMArray");

	G4double SiPMWidth = 1.2 * cm;
	G4double SiPMLength = 1.5 * cm;

	G4Box *solidSiPM = new G4Box("solidSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2);
	G4LogicalVolume *logicSiPM = new G4LogicalVolume(solidSiPM, matSi, "logicSiPM");
	G4Box *solidTPB = new G4Box("solidTPB", SiPMWidth / 2, SiPMLength / 2, TPBThickness / 2 + SiPMThickness / 2);
	G4LogicalVolume *logicTPB = new G4LogicalVolume(solidTPB, matTPB, "logicTPB");
	G4double offset1 = SiPMWidth / 2 + 0.3 * mm;
	G4double offset2 = SiPMLength / 2 + 0.3 * mm;

	physSiPM0 = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicSiPM, "physSiPM0", logicTPB, false, 0, CheckOverlaps);

	// physSiPM0 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, 0), logicSiPM, "physSiPM0", logicSiPMArray, false, 0, CheckOverlaps);
	// physSiPM1 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, 0), logicSiPM, "physSiPM1", logicSiPMArray, false, 1, CheckOverlaps);
	// physSiPM2 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, 0), logicSiPM, "physSiPM2", logicSiPMArray, false, 2, CheckOverlaps);
	// physSiPM3 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, 0), logicSiPM, "physSiPM3", logicSiPMArray, false, 3, CheckOverlaps);

	// auto physTPB0 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, 0), logicTPB, "physSiPM0", logicSiPMArray, false, 0, CheckOverlaps);
	// auto physTPB1 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, 0), logicTPB, "physSiPM1", logicSiPMArray, false, 1, CheckOverlaps);
	// auto physTPB2 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, 0), logicTPB, "physSiPM2", logicSiPMArray, false, 2, CheckOverlaps);
	// auto physTPB3 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, 0), logicTPB, "physSiPM3", logicSiPMArray, false, 3, CheckOverlaps);

	// auto physTPB0 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB0", logicSiPMArray, false, 0, CheckOverlaps);
	// auto physTPB1 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB1", logicSiPMArray, false, 1, CheckOverlaps);
	// auto physTPB2 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB2", logicSiPMArray, false, 2, CheckOverlaps);
	// auto physTPB3 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB3", logicSiPMArray, false, 3, CheckOverlaps);
	// auto physTPB4 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB4", logicSiPMArray, false, 4, CheckOverlaps);
	// auto physTPB5 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB5", logicSiPMArray, false, 5, CheckOverlaps);
	// auto physTPB6 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB6", logicSiPMArray, false, 6, CheckOverlaps);
	// auto physTPB7 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB7", logicSiPMArray, false, 7, CheckOverlaps);

	G4AssemblyVolume *logicSiPMArray = new G4AssemblyVolume();

	// Rotation and translation of a plate inside the assembly
	G4RotationMatrix *RotSiPM = new G4RotationMatrix();
	G4ThreeVector PosSiPM0 = G4ThreeVector(offset1, offset2, 0);
	G4ThreeVector PosSiPM1 = G4ThreeVector(offset1, -offset2, 0);
	G4ThreeVector PosSiPM2 = G4ThreeVector(-offset1, offset2, 0);
	G4ThreeVector PosSiPM3 = G4ThreeVector(-offset1, -offset2, 0);
	G4Transform3D TrSiPM0 = G4Transform3D(*RotSiPM, PosSiPM0);
	G4Transform3D TrSiPM1 = G4Transform3D(*RotSiPM, PosSiPM1);
	G4Transform3D TrSiPM2 = G4Transform3D(*RotSiPM, PosSiPM2);
	G4Transform3D TrSiPM3 = G4Transform3D(*RotSiPM, PosSiPM3);

	logicSiPMArray->AddPlacedVolume(logicTPB, TrSiPM0);
	logicSiPMArray->AddPlacedVolume(logicTPB, TrSiPM1);
	logicSiPMArray->AddPlacedVolume(logicTPB, TrSiPM2);
	logicSiPMArray->AddPlacedVolume(logicTPB, TrSiPM3);

	G4OpticalSurface *SiPM_SAr = new G4OpticalSurface("SiPM_SAr");
	SiPM_SAr->SetType(dielectric_LUTDAVIS);
	SiPM_SAr->SetModel(DAVIS);
	SiPM_SAr->SetFinish(Detector_LUT);

	const G4int NUMENTRIES_CHIP = 11;
	const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
	G4double sipm_pp[NUMENTRIES_CHIP] = {hc / 600. * eV, hc / 590. * eV, hc / 580. * eV, hc / 570. * eV, hc / 560. * eV, hc / 550. * eV, hc / 540. * eV, hc / 530. * eV, hc / 520. * eV, hc / 510. * eV, hc / 500. * eV};
	G4double sipm_sl[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_ss[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_bs[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_rindex[NUMENTRIES_CHIP] = {1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406};
	G4double sipm_reflectivity[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// G4double sipm_efficiency[NUMENTRIES_CHIP] = {0.20,0.21,0.23,0.25,0.26,0.28,0.30,0.32,0.34,0.36,0.38};
	G4double sipm_efficiency[NUMENTRIES_CHIP] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	G4MaterialPropertiesTable *SIPM_MPT_Surf = new G4MaterialPropertiesTable();
	// SIPM_MPT_Surf->AddProperty("SPECULARLOBECONSTANT",sipm_pp,sipm_sl,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("SPECULARSPIKECONSTANT",sipm_pp,sipm_ss,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("BACKSCATTERCONSTANT",sipm_pp,sipm_bs,NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("REFLECTIVITY", sipm_pp, sipm_reflectivity, NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("EFFICIENCY", sipm_pp, sipm_efficiency, NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("RINDEX",sipm_pp,sipm_rindex,NUMENTRIES_CHIP);

	SiPM_SAr->SetMaterialPropertiesTable(SIPM_MPT_Surf);

	G4OpticalSurface *TPB_SAr = new G4OpticalSurface("SiPM_SAr");
	TPB_SAr->SetType(dielectric_dielectric);
	TPB_SAr->SetModel(unified);
	TPB_SAr->SetFinish(polished);

	G4LogicalSkinSurface *SiPM_LSS = new G4LogicalSkinSurface("SiPM_LSS", logicSiPM, SiPM_SAr);
	G4LogicalSkinSurface *TPB_LSS = new G4LogicalSkinSurface("TPB_LSS", logicTPB, TPB_SAr);

	return logicSiPMArray;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructSArSiPMArrayLV()
{
	G4double SiPMThickness = 0.5 * mm;
	G4double TPBThickness = 2 * micrometer;
	G4Box *solidSiPMArray = new G4Box("solidSiPMArray", 3 * cm, 3 * cm, SiPMThickness / 2 + 0.5 * mm);
	G4LogicalVolume *logicSiPMArray = new G4LogicalVolume(solidSiPMArray, matLAr, "logicSiPMArray");

	G4double SiPMWidth = 1.2 * cm;
	G4double SiPMLength = 1.5 * cm;

	G4Box *solidSiPM = new G4Box("solidSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2);
	G4LogicalVolume *logicSiPM = new G4LogicalVolume(solidSiPM, matSi, "logicSiPM");
	G4Box *solidTPB = new G4Box("solidTPB", SiPMWidth / 2, SiPMLength / 2, TPBThickness / 2);
	G4LogicalVolume *logicTPB = new G4LogicalVolume(solidTPB, matTPB, "logicTPB");
	G4double offset1 = SiPMWidth / 2 + 0.3 * mm;
	G4double offset2 = SiPMLength / 2 + 0.3 * mm;

	physSiPM0 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, 0), logicSiPM, "physSiPM0", logicSiPMArray, false, 0, CheckOverlaps);
	physSiPM1 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, 0), logicSiPM, "physSiPM1", logicSiPMArray, false, 1, CheckOverlaps);
	physSiPM2 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, 0), logicSiPM, "physSiPM2", logicSiPMArray, false, 2, CheckOverlaps);
	physSiPM3 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, 0), logicSiPM, "physSiPM3", logicSiPMArray, false, 3, CheckOverlaps);

	auto physTPB0 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB0", logicSiPMArray, false, 0, CheckOverlaps);
	auto physTPB1 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB1", logicSiPMArray, false, 1, CheckOverlaps);
	auto physTPB2 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB2", logicSiPMArray, false, 2, CheckOverlaps);
	auto physTPB3 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, SiPMThickness / 2 + TPBThickness / 2), logicTPB, "physTPB3", logicSiPMArray, false, 3, CheckOverlaps);
	auto physTPB4 = new G4PVPlacement(0, G4ThreeVector(offset1, offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB4", logicSiPMArray, false, 4, CheckOverlaps);
	auto physTPB5 = new G4PVPlacement(0, G4ThreeVector(-offset1, offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB5", logicSiPMArray, false, 5, CheckOverlaps);
	auto physTPB6 = new G4PVPlacement(0, G4ThreeVector(offset1, -offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB6", logicSiPMArray, false, 6, CheckOverlaps);
	auto physTPB7 = new G4PVPlacement(0, G4ThreeVector(-offset1, -offset2, -SiPMThickness / 2 - TPBThickness / 2), logicTPB, "physTPB7", logicSiPMArray, false, 7, CheckOverlaps);

	G4OpticalSurface *SiPM_SAr = new G4OpticalSurface("SiPM_SAr");
	SiPM_SAr->SetType(dielectric_LUTDAVIS);
	SiPM_SAr->SetModel(DAVIS);
	SiPM_SAr->SetFinish(Detector_LUT);

	const G4int NUMENTRIES_CHIP = 11;
	const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
	G4double sipm_pp[NUMENTRIES_CHIP] = {hc / 600. * eV, hc / 590. * eV, hc / 580. * eV, hc / 570. * eV, hc / 560. * eV, hc / 550. * eV, hc / 540. * eV, hc / 530. * eV, hc / 520. * eV, hc / 510. * eV, hc / 500. * eV};
	G4double sipm_sl[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_ss[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_bs[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_rindex[NUMENTRIES_CHIP] = {1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406};
	G4double sipm_reflectivity[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// G4double sipm_efficiency[NUMENTRIES_CHIP] = {0.20,0.21,0.23,0.25,0.26,0.28,0.30,0.32,0.34,0.36,0.38};
	G4double sipm_efficiency[NUMENTRIES_CHIP] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	G4MaterialPropertiesTable *SIPM_MPT_Surf = new G4MaterialPropertiesTable();
	// SIPM_MPT_Surf->AddProperty("SPECULARLOBECONSTANT",sipm_pp,sipm_sl,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("SPECULARSPIKECONSTANT",sipm_pp,sipm_ss,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("BACKSCATTERCONSTANT",sipm_pp,sipm_bs,NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("REFLECTIVITY", sipm_pp, sipm_reflectivity, NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("EFFICIENCY", sipm_pp, sipm_efficiency, NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("RINDEX",sipm_pp,sipm_rindex,NUMENTRIES_CHIP);

	SiPM_SAr->SetMaterialPropertiesTable(SIPM_MPT_Surf);

	G4OpticalSurface *TPB_SAr = new G4OpticalSurface("SiPM_SAr");
	TPB_SAr->SetType(dielectric_dielectric);
	TPB_SAr->SetModel(unified);
	TPB_SAr->SetFinish(polished);

	G4OpticalSurface *SAr_SAr = new G4OpticalSurface("SAr_SAr");
	SAr_SAr->SetType(dielectric_dielectric);
	SAr_SAr->SetModel(unified);
	SAr_SAr->SetFinish(polished);

	G4LogicalSkinSurface *SiPM_LSS = new G4LogicalSkinSurface("SiPM_LSS", logicSiPM, SiPM_SAr);
	G4LogicalSkinSurface *TPB_LSS = new G4LogicalSkinSurface("TPB_LSS", logicTPB, TPB_SAr);
	G4LogicalSkinSurface *SiPMArray_LSS = new G4LogicalSkinSurface("SiPMArray_LSS", logicSiPMArray, SAr_SAr);

	return logicSiPMArray;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructSiPM()
{
	G4double SiPMThickness = 0.5 * mm;
	G4double SiPMWidth = 3 * cm;
	G4double SiPMLength = 3 * cm;
	G4double TPBThickness = 2 * micrometer;
	G4Box *solidCoatedSiPM = new G4Box("solidCoatedSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2 + TPBThickness);
	G4LogicalVolume *logicCoatedSiPM = new G4LogicalVolume(solidCoatedSiPM, matTPB, "logicCoatedSiPM");
	G4Box *solidSiPM = new G4Box("solidSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2);
	G4LogicalVolume *logicSiPMChip = new G4LogicalVolume(solidSiPM, matSi, "logicSiPMChip");
	physSiPM0 = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicSiPMChip, "physSiPM0", logicCoatedSiPM, false, 0, CheckOverlaps);

	G4OpticalSurface *TPB_Surf = new G4OpticalSurface("TPB_Surf");
	TPB_Surf->SetType(dielectric_dielectric);
	TPB_Surf->SetModel(unified);
	TPB_Surf->SetFinish(polished);

	G4OpticalSurface *SiPM_Surf = new G4OpticalSurface("SiPM_Surf");
	SiPM_Surf->SetType(dielectric_dielectric);
	SiPM_Surf->SetModel(unified);
	SiPM_Surf->SetFinish(polished);

	const G4int NUMENTRIES_CHIP = 11;
	const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
	G4double sipm_pp[NUMENTRIES_CHIP] = {hc / 600. * eV, hc / 590. * eV, hc / 580. * eV, hc / 570. * eV, hc / 560. * eV, hc / 550. * eV, hc / 540. * eV, hc / 530. * eV, hc / 520. * eV, hc / 510. * eV, hc / 500. * eV};
	G4double sipm_sl[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_ss[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_bs[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double sipm_rindex[NUMENTRIES_CHIP] = {1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406, 1.406};
	G4double sipm_reflectivity[NUMENTRIES_CHIP] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// G4double sipm_efficiency[NUMENTRIES_CHIP] = {0.20,0.21,0.23,0.25,0.26,0.28,0.30,0.32,0.34,0.36,0.38};
	G4double sipm_efficiency[NUMENTRIES_CHIP] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

	G4MaterialPropertiesTable *SiPM_MPT_Surf = new G4MaterialPropertiesTable();
	// SiPM_MPT_Surf->AddProperty("SPECULARLOBECONSTANT",sipm_pp,sipm_sl,NUMENTRIES_CHIP);
	// SiPM_MPT_Surf->AddProperty("SPECULARSPIKECONSTANT",sipm_pp,sipm_ss,NUMENTRIES_CHIP);
	// SiPM_MPT_Surf->AddProperty("BACKSCATTERCONSTANT",sipm_pp,sipm_bs,NUMENTRIES_CHIP);
	// SiPM_MPT_Surf->AddProperty("REFLECTIVITY", sipm_pp, sipm_reflectivity, NUMENTRIES_CHIP);
	// SiPM_MPT_Surf->AddProperty("EFFICIENCY", sipm_pp, sipm_efficiency, NUMENTRIES_CHIP);
	SiPM_MPT_Surf->AddProperty("RINDEX", sipm_pp, sipm_rindex, NUMENTRIES_CHIP);

	SiPM_Surf->SetMaterialPropertiesTable(SiPM_MPT_Surf);

	G4LogicalSkinSurface *SiPM_LSS = new G4LogicalSkinSurface("SiPM_LSS", logicSiPMChip, SiPM_Surf);
	G4LogicalSkinSurface *TPB_LSS = new G4LogicalSkinSurface("TPB_LSS", logicCoatedSiPM, TPB_Surf);

	return logicCoatedSiPM;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructFiberSiPM()
{
	G4double SiPMThickness = 0.5 * mm;
	G4double SiPMRadius = fFiberRadius;
	G4double TPBThickness = 2 * micrometer;
	// G4Box* solidCoatedSiPM = new G4Box("solidCoatedSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2 + TPBThickness);
	G4LogicalVolume *logicCoatedFiberSiPM;
	if (fFiberShape == "Round")
	{
		G4Tubs *solidCoatedSiPM = new G4Tubs("solidCoatedSiPM", 0, SiPMRadius, SiPMThickness / 2 + TPBThickness, 0, twopi);
		logicCoatedFiberSiPM = new G4LogicalVolume(solidCoatedSiPM, matTPB, "logicCoatedFiberSiPM");
		G4Tubs *solidSiPMChip = new G4Tubs("solidSiPM", 0, SiPMRadius, SiPMThickness / 2, 0, twopi);
		logicFiberSiPM = new G4LogicalVolume(solidSiPMChip, matSi, "logicFiberSiPM");
		physFiberSiPM = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicFiberSiPM, "physFiberSiPM", logicCoatedFiberSiPM, false, 0, CheckOverlaps);
	}
	else if (fFiberShape == "Rec")
	{
		G4Box *solidCoatedSiPM = new G4Box("solidCoatedSiPM", SiPMRadius, SiPMRadius, SiPMThickness / 2 + TPBThickness);
		logicCoatedFiberSiPM = new G4LogicalVolume(solidCoatedSiPM, matTPB, "logicCoatedFiberSiPM");
		G4Box *solidSiPMChip = new G4Box("solidSiPM", SiPMRadius, SiPMRadius, SiPMThickness / 2);
		logicFiberSiPM = new G4LogicalVolume(solidSiPMChip, matSi, "logicFiberSiPM");
		physFiberSiPM = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicFiberSiPM, "physFiberSiPM", logicCoatedFiberSiPM, false, 0, CheckOverlaps);
	}

	const G4int NUMENTRIES_CHIP = 10;
	G4double SiPMPhotonEnergy[NUMENTRIES_CHIP];
	G4double SiPMEfficiencyWl[NUMENTRIES_CHIP] = {100., 280., 310., 350., 400., 435., 505., 525., 595., 670.};
	// 100 mm wafer reference. 120 um Cell @ dV = 7.5 volts
	// G4double SiPMEfficiencyTbl[npoints_eff] = {0.0,0.19,0.30,0.32,0.33,0.32,0.27,0.19,0.12,0.07};
	//  200 mm Wafer reference. 50 um cell @ dV = 5.0 volts
	// G4double SiPMEfficiencyWl[npoints_eff] = {100.,280.,310.,350.,400.,435.,470.,525.,595.,670.};
	// G4double SiPMEfficiencyTbl[npoints_eff] = {0.0,0.19,.30,0.34,0.44,0.41,0.40,0.30,0.18,.07};
	// If the PDE is not know, make it 100% efficient and correct for it off-line
	G4double SiPMEfficiencyTbl[NUMENTRIES_CHIP] = {0.0, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
	G4double SiPMEfficiency[NUMENTRIES_CHIP];
	G4double SiPMReflectivity[NUMENTRIES_CHIP];

	/*
	** not sure what this is for, but taked from GEGeometryLArInstHybrid
	G4double LAr_Fiber_coverage = fDetectorDB->GetLArInstFiberCoverage();
	MGLog(routine) << "Fiber coverage: " << LAr_Fiber_coverage << endlog;
	//fiber efficiency scaling relative to PMTs
	G4double SiPM_bonding_scale = 0.7 * LAr_Fiber_coverage;
	*/

	for (G4int ji = 0; ji < NUMENTRIES_CHIP; ji++)
	{
		// Zero reflectivity and 1.0 efficiency means that the
		// photons are all "absorbed and identified as hits"
		SiPMPhotonEnergy[ji] = LambdaE / (SiPMEfficiencyWl[(NUMENTRIES_CHIP - 1) - ji] * nm);
		SiPMReflectivity[ji] = 0.0;											// Set the reflectivity in the fibers to zero, otherwise 99.999 % is reflected due to index of refraction
		SiPMEfficiency[ji] = SiPMEfficiencyTbl[(NUMENTRIES_CHIP - 1) - ji]; // Quantum efficiency of the SiPM.
	}
	G4MaterialPropertiesTable *SiPM_Surf_MPT = new G4MaterialPropertiesTable();
	SiPM_Surf_MPT->AddProperty("EFFICIENCY", SiPMPhotonEnergy, SiPMEfficiency, NUMENTRIES_CHIP);
	SiPM_Surf_MPT->AddProperty("REFLECTIVITY", SiPMPhotonEnergy, SiPMReflectivity, NUMENTRIES_CHIP);

	G4OpticalSurface *SiPM_Surf = new G4OpticalSurface("SiPM_Surf", glisur, ground, dielectric_metal);
	SiPM_Surf->SetType(dielectric_dielectric);
	SiPM_Surf->SetModel(glisur);
	SiPM_Surf->SetFinish(polished);
	// SiPM_Surf->SetMaterialPropertiesTable(SiPM_Surf_MPT);

	G4OpticalSurface *TPB_Surf = new G4OpticalSurface("TPB_Surf");
	TPB_Surf->SetType(dielectric_dielectric);
	TPB_Surf->SetModel(glisur);
	TPB_Surf->SetFinish(polished);

	G4LogicalSkinSurface *SiPM_LSS = new G4LogicalSkinSurface("SiPM_LSS", logicFiberSiPM, SiPM_Surf);
	G4LogicalSkinSurface *TPB_LSS = new G4LogicalSkinSurface("TPB_LSS", logicCoatedFiberSiPM, TPB_Surf);

	return logicCoatedFiberSiPM;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructArVolumeSiPM(G4double radius)
{
	G4double SiPMThickness = 1 * mm;
	G4double SiPMRadius = radius;
	G4double TPBThickness = 2 * micrometer;
	// G4Box* solidCoatedSiPM = new G4Box("solidCoatedSiPM", SiPMWidth / 2, SiPMLength / 2, SiPMThickness / 2 + TPBThickness);
	G4Tubs *solidCoatedSiPM = new G4Tubs("solidCoatedSiPM", 0, SiPMRadius, SiPMThickness / 2 + TPBThickness, 0, twopi);
	G4LogicalVolume *logicCoatedArVolumeSiPM = new G4LogicalVolume(solidCoatedSiPM, matTPB, "logicCoatedArVolumeSiPM");
	G4Tubs *solidSiPMChip = new G4Tubs("solidSiPM", 0, SiPMRadius, SiPMThickness / 2, 0, twopi);
	logicArVolumeSiPM = new G4LogicalVolume(solidSiPMChip, matSi, "logicArVolumeSiPM");
	physArVolumeSiPM = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicArVolumeSiPM, "physArVolumeSiPM", logicCoatedArVolumeSiPM, false, 0, CheckOverlaps);

	const G4int NUMENTRIES_CHIP = 10;
	G4double SiPMPhotonEnergy[NUMENTRIES_CHIP];
	G4double SiPMEfficiencyWl[NUMENTRIES_CHIP] = {100., 280., 310., 350., 400., 435., 505., 525., 595., 670.};
	// 100 mm wafer reference. 120 um Cell @ dV = 7.5 volts
	// G4double SiPMEfficiencyTbl[npoints_eff] = {0.0,0.19,0.30,0.32,0.33,0.32,0.27,0.19,0.12,0.07};
	//  200 mm Wafer reference. 50 um cell @ dV = 5.0 volts
	// G4double SiPMEfficiencyWl[npoints_eff] = {100.,280.,310.,350.,400.,435.,470.,525.,595.,670.};
	// G4double SiPMEfficiencyTbl[npoints_eff] = {0.0,0.19,.30,0.34,0.44,0.41,0.40,0.30,0.18,.07};
	// If the PDE is not know, make it 100% efficient and correct for it off-line
	G4double SiPMEfficiencyTbl[NUMENTRIES_CHIP] = {0.0, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
	G4double SiPMEfficiency[NUMENTRIES_CHIP];
	G4double SiPMReflectivity[NUMENTRIES_CHIP];

	for (G4int ji = 0; ji < NUMENTRIES_CHIP; ji++)
	{
		// Zero reflectivity and 1.0 efficiency means that the
		// photons are all "absorbed and identified as hits"
		SiPMPhotonEnergy[ji] = LambdaE / (SiPMEfficiencyWl[(NUMENTRIES_CHIP - 1) - ji] * nm);
		SiPMReflectivity[ji] = 0.0;											// Set the reflectivity in the fibers to zero, otherwise 99.999 % is reflected due to index of refraction
		SiPMEfficiency[ji] = SiPMEfficiencyTbl[(NUMENTRIES_CHIP - 1) - ji]; // Quantum efficiency of the SiPM.
	}
	G4MaterialPropertiesTable *SiPM_Surf_MPT = new G4MaterialPropertiesTable();
	SiPM_Surf_MPT->AddProperty("EFFICIENCY", SiPMPhotonEnergy, SiPMEfficiency, NUMENTRIES_CHIP);
	SiPM_Surf_MPT->AddProperty("REFLECTIVITY", SiPMPhotonEnergy, SiPMReflectivity, NUMENTRIES_CHIP);

	G4OpticalSurface *SiPM_Surf = new G4OpticalSurface("SiPM_Surf", glisur, ground, dielectric_metal);
	SiPM_Surf->SetType(dielectric_dielectric);
	SiPM_Surf->SetModel(glisur);
	SiPM_Surf->SetFinish(polished);
	// SiPM_Surf->SetMaterialPropertiesTable(SiPM_Surf_MPT);

	G4OpticalSurface *TPB_Surf = new G4OpticalSurface("TPB_Surf");
	TPB_Surf->SetType(dielectric_dielectric);
	TPB_Surf->SetModel(glisur);
	TPB_Surf->SetFinish(polished);

	G4LogicalSkinSurface *SiPM_LSS = new G4LogicalSkinSurface("SiPM_LSS", logicArVolumeSiPM, SiPM_Surf);
	G4LogicalSkinSurface *TPB_LSS = new G4LogicalSkinSurface("TPB_LSS", logicCoatedArVolumeSiPM, TPB_Surf);

	return logicCoatedArVolumeSiPM;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructArVolumeHexSiPM(G4double circumradius)
{
	G4double SiPMThickness = 1 * mm;
	G4double SiPMRadius = circumradius;
	G4double TPBThickness = 2 * micrometer;

	const G4int nsect = 6;
	std::vector<G4TwoVector> Polygon(nsect);
	G4double ang = twopi / nsect;
	for (G4int i = 0; i < nsect; ++i)
	{
		G4double phi = i * ang;
		G4double cosphi = std::cos(phi);
		G4double sinphi = std::sin(phi);
		Polygon[i].set(circumradius * cosphi, circumradius * sinphi);
	}

	G4TwoVector offsetA(0, 0), offsetB(0, 0);
	G4double scaleA = 1, scaleB = 1;
	G4VSolid *solidCoatedSiPM = new G4ExtrudedSolid("solidCoatedSiPM", Polygon, SiPMThickness / 2 + TPBThickness, offsetA, scaleA, offsetB, scaleB);
	G4VSolid *solidSiPMChip = new G4ExtrudedSolid("solidCoatedSiPM", Polygon, SiPMThickness / 2, offsetA, scaleA, offsetB, scaleB);
	G4LogicalVolume *logicCoatedArVolumeSiPM = new G4LogicalVolume(solidCoatedSiPM, matTPB, "logicCoatedArVolumeSiPM");
	logicArVolumeSiPM = new G4LogicalVolume(solidSiPMChip, matSi, "logicArVolumeSiPM");
	physArVolumeSiPM = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicArVolumeSiPM, "physArVolumeSiPM", logicCoatedArVolumeSiPM, false, 0, CheckOverlaps);

	const G4int NUMENTRIES_CHIP = 10;
	G4double SiPMPhotonEnergy[NUMENTRIES_CHIP];
	G4double SiPMEfficiencyWl[NUMENTRIES_CHIP] = {100., 280., 310., 350., 400., 435., 505., 525., 595., 670.};
	// 100 mm wafer reference. 120 um Cell @ dV = 7.5 volts
	// G4double SiPMEfficiencyTbl[npoints_eff] = {0.0,0.19,0.30,0.32,0.33,0.32,0.27,0.19,0.12,0.07};
	//  200 mm Wafer reference. 50 um cell @ dV = 5.0 volts
	// G4double SiPMEfficiencyWl[npoints_eff] = {100.,280.,310.,350.,400.,435.,470.,525.,595.,670.};
	// G4double SiPMEfficiencyTbl[npoints_eff] = {0.0,0.19,.30,0.34,0.44,0.41,0.40,0.30,0.18,.07};
	// If the PDE is not know, make it 100% efficient and correct for it off-line
	G4double SiPMEfficiencyTbl[NUMENTRIES_CHIP] = {0.0, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
	G4double SiPMEfficiency[NUMENTRIES_CHIP];
	G4double SiPMReflectivity[NUMENTRIES_CHIP];

	for (G4int ji = 0; ji < NUMENTRIES_CHIP; ji++)
	{
		// Zero reflectivity and 1.0 efficiency means that the
		// photons are all "absorbed and identified as hits"
		SiPMPhotonEnergy[ji] = LambdaE / (SiPMEfficiencyWl[(NUMENTRIES_CHIP - 1) - ji] * nm);
		SiPMReflectivity[ji] = 0.0;											// Set the reflectivity in the fibers to zero, otherwise 99.999 % is reflected due to index of refraction
		SiPMEfficiency[ji] = SiPMEfficiencyTbl[(NUMENTRIES_CHIP - 1) - ji]; // Quantum efficiency of the SiPM.
	}
	G4MaterialPropertiesTable *SiPM_Surf_MPT = new G4MaterialPropertiesTable();
	SiPM_Surf_MPT->AddProperty("EFFICIENCY", SiPMPhotonEnergy, SiPMEfficiency, NUMENTRIES_CHIP);
	SiPM_Surf_MPT->AddProperty("REFLECTIVITY", SiPMPhotonEnergy, SiPMReflectivity, NUMENTRIES_CHIP);

	G4OpticalSurface *SiPM_Surf = new G4OpticalSurface("SiPM_Surf", glisur, ground, dielectric_metal);
	SiPM_Surf->SetType(dielectric_dielectric);
	SiPM_Surf->SetModel(glisur);
	SiPM_Surf->SetFinish(polished);
	// SiPM_Surf->SetMaterialPropertiesTable(SiPM_Surf_MPT);

	G4OpticalSurface *TPB_Surf = new G4OpticalSurface("TPB_Surf");
	TPB_Surf->SetType(dielectric_dielectric);
	TPB_Surf->SetModel(glisur);
	TPB_Surf->SetFinish(polished);

	G4LogicalSkinSurface *SiPM_LSS = new G4LogicalSkinSurface("SiPM_LSS", logicArVolumeSiPM, SiPM_Surf);
	G4LogicalSkinSurface *TPB_LSS = new G4LogicalSkinSurface("TPB_LSS", logicCoatedArVolumeSiPM, TPB_Surf);

	return logicCoatedArVolumeSiPM;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructArVolumeReflector(G4double radius)
{
	G4double ReflectorThickness = 1 * mm;
	G4double ReflectorRadius = radius;
	G4double TPBThickness = 2 * micrometer;
	// G4Box* solidCoatedReflector = new G4Box("solidCoatedReflector", ReflectorWidth / 2, ReflectorLength / 2, ReflectorThickness / 2 + TPBThickness);
	G4Tubs *solidCoatedReflector = new G4Tubs("solidCoatedReflector", 0, ReflectorRadius, ReflectorThickness / 2 + TPBThickness, 0, twopi);
	G4LogicalVolume *logicCoatedArVolumeReflector = new G4LogicalVolume(solidCoatedReflector, matTPB, "logicCoatedArVolumeReflector");
	G4Tubs *solidReflectorChip = new G4Tubs("solidReflector", 0, ReflectorRadius, ReflectorThickness / 2, 0, twopi);
	G4LogicalVolume *logicArVolumeReflector = new G4LogicalVolume(solidReflectorChip, matSi, "logicArVolumeReflector");
	G4PVPlacement *physArVolumeReflector = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicArVolumeReflector, "physArVolumeReflector", logicCoatedArVolumeReflector, false, 0, CheckOverlaps);

	G4OpticalSurface *Reflector_Surf = new G4OpticalSurface("Reflector_Surf", DAVIS, PolishedTeflon_LUT, dielectric_LUTDAVIS);
	Reflector_Surf->SetType(dielectric_dielectric);
	Reflector_Surf->SetModel(glisur);
	Reflector_Surf->SetFinish(polished);
	// Reflector_Surf->SetMaterialPropertiesTable(Reflector_Surf_MPT);

	G4OpticalSurface *TPB_Surf = new G4OpticalSurface("TPB_Surf");
	TPB_Surf->SetType(dielectric_dielectric);
	TPB_Surf->SetModel(glisur);
	TPB_Surf->SetFinish(polished);

	G4LogicalSkinSurface *Reflector_LSS = new G4LogicalSkinSurface("Reflector_LSS", logicArVolumeReflector, Reflector_Surf);
	G4LogicalSkinSurface *TPB_LSS = new G4LogicalSkinSurface("TPB_LSS", logicCoatedArVolumeReflector, TPB_Surf);

	return logicCoatedArVolumeReflector;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructArVolumeHexReflector(G4double circumradius)
{
	G4double ReflectorThickness = 0.5 * mm;
	G4double ReflectorRadius = circumradius;
	G4double TPBThickness = 2 * micrometer;

	const G4int nsect = 6;
	std::vector<G4TwoVector> Polygon(nsect);
	G4double ang = twopi / nsect;
	for (G4int i = 0; i < nsect; ++i)
	{
		G4double phi = i * ang;
		G4double cosphi = std::cos(phi);
		G4double sinphi = std::sin(phi);
		Polygon[i].set(circumradius * cosphi, circumradius * sinphi);
	}

	G4TwoVector offsetA(0, 0), offsetB(0, 0);
	G4double scaleA = 1, scaleB = 1;
	G4VSolid *solidCoatedReflector = new G4ExtrudedSolid("solidCoatedReflector", Polygon, ReflectorThickness / 2 + TPBThickness, offsetA, scaleA, offsetB, scaleB);
	G4VSolid *solidReflector = new G4ExtrudedSolid("solidCoatedReflector", Polygon, ReflectorThickness / 2, offsetA, scaleA, offsetB, scaleB);
	G4LogicalVolume *logicCoatedArVolumeReflector = new G4LogicalVolume(solidCoatedReflector, matTPB, "logicCoatedArVolumeReflector");
	G4LogicalVolume *logicArVolumeReflector = new G4LogicalVolume(solidReflector, matTeflon, "logicArVolumeReflector");
	G4PVPlacement *physArVolumeReflector = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicArVolumeReflector, "physArVolumeReflector", logicCoatedArVolumeReflector, false, 0, CheckOverlaps);

	G4OpticalSurface *Reflector_Surf = new G4OpticalSurface("Reflector_Surf", DAVIS, PolishedTeflon_LUT, dielectric_LUTDAVIS);
	Reflector_Surf->SetType(dielectric_dielectric);
	Reflector_Surf->SetModel(glisur);
	Reflector_Surf->SetFinish(polished);
	// Reflector_Surf->SetMaterialPropertiesTable(Reflector_Surf_MPT);

	G4OpticalSurface *TPB_Surf = new G4OpticalSurface("TPB_Surf");
	TPB_Surf->SetType(dielectric_dielectric);
	TPB_Surf->SetModel(glisur);
	TPB_Surf->SetFinish(polished);

	G4LogicalSkinSurface *Reflector_LSS = new G4LogicalSkinSurface("Reflector_LSS", logicArVolumeReflector, Reflector_Surf);
	G4LogicalSkinSurface *TPB_LSS = new G4LogicalSkinSurface("TPB_LSS", logicCoatedArVolumeReflector, TPB_Surf);

	return logicCoatedArVolumeReflector;
}
// CDEX Bucket Veto SiPM System Design
G4LogicalVolume *CDEXDetectorConstruction::ConstructSiPMBucket()
{
	G4double BucketRadius = fBucketRadius;
	G4double BucketHeight = fBucketHeight;
	G4double BucketThickness = fBucketThickness;
	auto solidBucket = new G4Tubs("solidBucket", 0., BucketRadius, BucketHeight / 2, 0., twopi);
	auto logicBucket = new G4LogicalVolume(solidBucket, matPMMA, "logicBucket");

	auto solidBucketCrystal = new G4Tubs("solidBucketCrystal", 0, BucketRadius - BucketThickness, BucketHeight / 2 - BucketThickness, 0, twopi);
	logicArVolume = new G4LogicalVolume(solidBucketCrystal, matLAr, "logicBucket");
	auto physBucketCrystal = new G4PVPlacement(0, G4ThreeVector(), logicArVolume, "BucketCrystal", logicBucket, false, 0, CheckOverlaps);
	return logicBucket;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructShroud()
{
	G4double ShroudHeight = 1.5 * m;
	G4double ShroudThickness = 1 * cm;
	G4double ShroudRadius = fBEGeRadius + 2 * mm + ShroudThickness;
	auto solidShroud = new G4Tubs("solidShroud", 0., ShroudRadius, ShroudHeight / 2, 0., twopi);
	auto logicShroud = new G4LogicalVolume(solidShroud, matPMMA, "logicShroud");

	auto solidShroudVoid = new G4Tubs("solidBucketCrystal", 0, ShroudRadius - ShroudThickness, ShroudHeight / 2 - ShroudThickness, 0, twopi);
	logicShourdVoid = new G4LogicalVolume(solidShroudVoid, matGN2, "logicShroudVoid");
	auto physShroudVoid = new G4PVPlacement(0, G4ThreeVector(), logicShourdVoid, "ShroudVoid", logicShroud, false, 0, CheckOverlaps);
	return logicShroud;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructArVolume()
{
	G4double ArVolumeRadius = fArVolumeRadius;
	G4double ArVolumeHeight = fArVolumeHeight;
	auto solidArVolumeCons = new G4Tubs("solidArVolume", 0., ArVolumeRadius, ArVolumeHeight / 2, 0., twopi);
	auto logicArVolumeCons = new G4LogicalVolume(solidArVolumeCons, matLAr, "logicArVolume");

	return logicArVolumeCons;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructArContainer()
{
	G4double ArContainerRadius = fArContainerRadius;
	G4double ArContainerHeight = fArContainerHeight;
	G4double ArContainerThickness = fArContainerThickness;
	auto solidArContainerCons = new G4Tubs("solidArVolume", ArContainerRadius - fArContainerThickness, ArContainerRadius, ArContainerHeight / 2, 0., twopi);
	auto logicArContainerCons = new G4LogicalVolume(solidArContainerCons, matPMMA, "logicArContainer");

	return logicArContainerCons;
}

// CDEX Bucket-Fiber Veto System Design

G4LogicalVolume *CDEXDetectorConstruction::ConstructFiberBucket()
{
	G4double BucketRadius = fBucketRadius;
	G4double BucketHeight = fBucketHeight;
	G4double BucketThickness = fBucketThickness;
	auto solidBucket = new G4Tubs("solidBucket", 0., BucketRadius, BucketHeight / 2, 0., twopi);
	auto logicBucket = new G4LogicalVolume(solidBucket, matPMMA, "logicBucket");

	auto solidArVolume = new G4Tubs("solidBucketCrystal", 0, BucketRadius - BucketThickness, BucketHeight / 2 - BucketThickness, 0, twopi);
	logicArVolume = new G4LogicalVolume(solidArVolume, matLAr, "logicArVolume");
	// logicBucketFiberCrystal = new G4LogicalVolume(solidBucketCrystal, matLAr, "logicBucket");
	// logicArVolume = LV;
	auto physArVolume = new G4PVPlacement(0, G4ThreeVector(), logicArVolume, "ArVolume", logicBucket, false, 0, CheckOverlaps);
	return logicBucket;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructLightGuideBucket()
{
	G4double BucketRadius = fBucketRadius;
	G4double BucketHeight = fBucketHeight;
	G4double BucketThickness = fBucketThickness;
	auto solidBucket = new G4Tubs("solidBucket", 0., BucketRadius, BucketHeight / 2, 0., twopi);
	auto logicBucket = new G4LogicalVolume(solidBucket, matPMMA, "logicBucket");

	auto solidArVolume = new G4Tubs("solidBucketCrystal", 0, BucketRadius - BucketThickness, BucketHeight / 2 - BucketThickness, 0, twopi);
	logicArVolume = new G4LogicalVolume(solidArVolume, matLAr, "logicArVolume");
	// logicBucketFiberCrystal = new G4LogicalVolume(solidBucketCrystal, matLAr, "logicBucket");
	// logicArVolume = LV;
	auto physArVolume = new G4PVPlacement(0, G4ThreeVector(), logicArVolume, "ArVolume", logicBucket, false, 0, CheckOverlaps);
	return logicBucket;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructCDEX300Bucket(G4double shieldthickness)
{
	G4double BucketRadius = fBucketRadius;
	G4double BucketHeight = fBucketHeight;
	G4double BucketThickness = fBucketThickness;

	G4double SSWallThickness = 5 * mm;
	G4double CuShieldThickness = shieldthickness;
	G4double VacuumThickness = 4 * cm;
	auto solidBucket = new G4Tubs("solidBucket", 0., BucketRadius, BucketHeight / 2, 0., twopi);
	auto logicBucket = new G4LogicalVolume(solidBucket, matLAr, "logicBucket");

	auto solidSSOuterWall = new G4Tubs("solidBucketSSOuterWall", BucketRadius - SSWallThickness, BucketRadius, BucketHeight / 2, 0., twopi);
	auto logicSSOuterWall = new G4LogicalVolume(solidSSOuterWall, matSS, "logicBucketSSWall");

	auto solidSSInnerWall = new G4Tubs("solidBucketSSInnerWall", BucketRadius - SSWallThickness * 2 - VacuumThickness, BucketRadius - SSWallThickness - VacuumThickness, BucketHeight / 2, 0., twopi);
	auto logicSSInnerWall = new G4LogicalVolume(solidSSInnerWall, matSS, "logicBucketSSWall");

	auto solidVacuumLayer = new G4Tubs("solidBucketVacuumLayer", BucketRadius - SSWallThickness - VacuumThickness, BucketRadius - SSWallThickness, BucketHeight / 2, 0., twopi);
	auto logicVacuumLayer = new G4LogicalVolume(solidSSOuterWall, fVacuum, "logicBucketVacuumLayer");

	auto solidCuShield = new G4Tubs("solidCuShield", BucketRadius - SSWallThickness * 2 - VacuumThickness - CuShieldThickness, BucketRadius - SSWallThickness * 2 - VacuumThickness, BucketHeight / 2, 0., twopi);
	auto logicCuShield = new G4LogicalVolume(solidCuShield, matCu, "logicCuShield");

	auto solidArVolume = new G4Tubs("solidArVolume", 0, BucketRadius - 2 * SSWallThickness - VacuumThickness - CuShieldThickness, BucketHeight / 2, 0, twopi);
	logicArVolume = new G4LogicalVolume(solidArVolume, matLAr, "logicArVolume");
	// logicBucketFiberCrystal = new G4LogicalVolume(solidBucketCrystal, matLAr, "logicBucket");
	auto physBucketCrystal = new G4PVPlacement(0, G4ThreeVector(), logicArVolume, "BucketCrystal", logicBucket, false, 0, CheckOverlaps);
	auto physCuShield = new G4PVPlacement(0, G4ThreeVector(), logicCuShield, "CuShield", logicBucket, false, 0, CheckOverlaps);
	auto physVacuumLayer = new G4PVPlacement(0, G4ThreeVector(), logicVacuumLayer, "VacuumLayer", logicBucket, false, 0, CheckOverlaps);
	auto physSSOuterWall = new G4PVPlacement(0, G4ThreeVector(), logicSSOuterWall, "Wall", logicBucket, false, 0, CheckOverlaps);
	auto physSSInnerWall = new G4PVPlacement(0, G4ThreeVector(), logicSSInnerWall, "Wall", logicBucket, false, 0, CheckOverlaps);

	return logicBucket;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructCDEX300LArContainer()
{
	G4double ContainerRadius = fArContainerRadius;
	G4double ContainerHeight = fArContainerHeight;

	G4double OuterWallThickness = 20 * mm;
	G4double InnerWallThickness = 15 * mm;
	G4double VacuumThickness = 4 * cm;
	auto solidContainer = new G4Tubs("solidBucket", 0., ContainerRadius, ContainerHeight / 2, 0., twopi);
	auto logicContainer = new G4LogicalVolume(solidContainer, matLAr, "logicArContainer");

	auto solidOuterWall = new G4Tubs("solidOuterWall", ContainerRadius - OuterWallThickness, ContainerRadius, ContainerHeight / 2, 0., twopi);
	auto logicOuterWall = new G4LogicalVolume(solidOuterWall, matCu, "logicOuterWall");

	auto solidInnerWall = new G4Tubs("solidInnerWall", ContainerRadius - OuterWallThickness - VacuumThickness - InnerWallThickness, ContainerRadius - OuterWallThickness - VacuumThickness, ContainerHeight / 2, 0., twopi);
	auto logicInnerWall = new G4LogicalVolume(solidInnerWall, matCu, "logicInnerWall");

	auto solidVacuumLayer = new G4Tubs("solidVacuumLayer", ContainerRadius - OuterWallThickness - VacuumThickness, ContainerRadius - OuterWallThickness, ContainerHeight / 2, 0., twopi);
	auto logicVacuumLayer = new G4LogicalVolume(solidVacuumLayer, fVacuum, "logicBucketVacuumLayer");

	auto solidArVolume = new G4Tubs("solidArVolume", 0, ContainerRadius - OuterWallThickness - VacuumThickness - InnerWallThickness, ContainerHeight / 2, 0, twopi);
	logicArVolume = new G4LogicalVolume(solidArVolume, matLAr, "logicArVolume");
	// logicBucketFiberCrystal = new G4LogicalVolume(solidBucketCrystal, matLAr, "logicBucket");
	auto physBucketCrystal = new G4PVPlacement(0, G4ThreeVector(), logicArVolume, "ArVolume", logicContainer, false, 0, CheckOverlaps);
	auto physVacuumLayer = new G4PVPlacement(0, G4ThreeVector(), logicVacuumLayer, "VacuumLayer", logicContainer, false, 0, CheckOverlaps);
	auto physOuterWall = new G4PVPlacement(0, G4ThreeVector(), logicOuterWall, "Wall", logicContainer, false, 0, CheckOverlaps);
	auto physInnerWall = new G4PVPlacement(0, G4ThreeVector(), logicInnerWall, "Wall", logicContainer, false, 0, CheckOverlaps);
	G4OpticalSurface *Cu_Dielectric = new G4OpticalSurface("Cu_Dielectric");
	Cu_Dielectric->SetType(dielectric_metal);
	Cu_Dielectric->SetModel(glisur);
	Cu_Dielectric->SetFinish(polished);
	G4LogicalSkinSurface *Wall_LSS = new G4LogicalSkinSurface("Bucket_LSS", logicInnerWall, Cu_Dielectric);
	return logicContainer;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructShell()
{
	G4double OuterRadius = fBEGeRadius + fShellThickness;
	G4double OuterHeight = fBEGeHeight + 2 * fShellThickness;
	G4double Chamfer = 3 * mm;
	G4double asmallvalue = 0.01 * mm;
	auto Torus = new G4Torus("Torus", 0., Chamfer, OuterRadius - Chamfer, 0., twopi);
	auto TopCylinder = new G4Tubs("Top", 0., OuterRadius - Chamfer, Chamfer, 0., twopi);
	G4ThreeVector pos0 = G4ThreeVector();
	G4ThreeVector pos1 = G4ThreeVector(0., 0., OuterHeight / 2 - Chamfer + asmallvalue);
	G4ThreeVector pos2 = G4ThreeVector(0., 0., -OuterHeight / 2 + Chamfer - asmallvalue);

	auto Top = new G4UnionSolid("Top", TopCylinder, Torus);
	auto Cylinder = new G4Tubs("solidShell", 0., OuterRadius, OuterHeight / 2 - Chamfer, 0., twopi);

	// auto Temp1 = new G4UnionSolid("Temp1", Cylinder, Top, 0, pos1);
	// auto solidShellSolid = new G4UnionSolid("solidShellVoid", Temp1, Top, 0, pos2);

	auto solidShellSolid = new G4MultiUnion("solidShellSolid");
	G4RotationMatrix rot1 = G4RotationMatrix();
	G4Transform3D tr0 = G4Transform3D(rot1, pos0);
	G4Transform3D tr1 = G4Transform3D(rot1, pos1);
	G4Transform3D tr2 = G4Transform3D(rot1, pos2);
	solidShellSolid->AddNode(*Cylinder, tr0);
	solidShellSolid->AddNode(*Top, tr1);
	solidShellSolid->AddNode(*Top, tr2);
	solidShellSolid->Voxelize();

	auto solidShellVoid = new G4Tubs("solidShell", 0., 40.1 * mm, 20.1 * mm, 0., twopi);
	auto solidShell = new G4SubtractionSolid("solidPENShell", solidShellSolid, solidShellVoid, 0, G4ThreeVector());

	auto logicShell = new G4LogicalVolume(solidShellSolid, matPMMA, "logicShell");
	return logicShell;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructLightFiber(G4double length)
{
	G4LogicalVolume *logicFiber;
	G4LogicalVolume *logicFiberCore;
	G4LogicalVolume *logicFiberInnerCladding;
	G4LogicalVolume *logicFiberOuterCladding;
	G4LogicalVolume *logicFiberWLSLayer;
	if (fFiberShape == "Round")
	{
		if (ifFiberTPB == true)
		{
			auto solidFiber = new G4Tubs("solidFiber", 0, fFiberRadius + fFiberTPBThickness, length / 2, 0, twopi);
			auto solidFiberCore = new G4Tubs("solidFiberCore", 0, fFiberRadius - fFiberInnerCladdingThickness - fFiberOuterCladdingThickness, length / 2, 0, twopi);
			auto solidFiberInnerCladding = new G4Tubs("solidInnerCladding", fFiberRadius - fFiberInnerCladdingThickness - fFiberOuterCladdingThickness, fFiberRadius - fFiberOuterCladdingThickness, length / 2, 0, twopi);
			auto solidFiberOuterCladding = new G4Tubs("solidFiberOuterCladding", fFiberRadius - fFiberOuterCladdingThickness, fFiberRadius, length / 2, 0, twopi);
			auto solidFiberWLSLayer = new G4Tubs("solidFiberTPB", fFiberRadius, fFiberRadius + fFiberTPBThickness, length / 2, 0, twopi);

			G4Material *CoreMaterial = matPolystyrene;
			logicFiber = new G4LogicalVolume(solidFiber, CoreMaterial, "logicFiber");
			logicFiberCore = new G4LogicalVolume(solidFiberCore, CoreMaterial, "logicFiberCore");
			logicFiberInnerCladding = new G4LogicalVolume(solidFiberInnerCladding, matPMMA, "logicFiberInnerCladding");
			logicFiberOuterCladding = new G4LogicalVolume(solidFiberOuterCladding, matFluorAcrylic, "logicFiberOuterCladding");

			logicFiberWLSLayer = new G4LogicalVolume(solidFiberWLSLayer, matTPB, "logicFiberWLSLayer");

			auto physFiberCore = new G4PVPlacement(0, G4ThreeVector(), logicFiberCore, "FiberCore", logicFiber, false, 0, CheckOverlaps);
			auto physFiberInnerCladding = new G4PVPlacement(0, G4ThreeVector(), logicFiberInnerCladding, "FiberInnerCladding", logicFiber, false, 0, CheckOverlaps);
			auto physFiberOuterCladding = new G4PVPlacement(0, G4ThreeVector(), logicFiberOuterCladding, "FiberOuterCladding", logicFiber, false, 0, CheckOverlaps);
			auto physFiberWLSLayer = new G4PVPlacement(0, G4ThreeVector(), logicFiberWLSLayer, "FiberWLSLayer", logicFiber, false, 0, CheckOverlaps);
		}
		else
		{
			auto solidFiber = new G4Tubs("solidFiber", 0, fFiberRadius, length / 2, 0, twopi);
			auto solidFiberCore = new G4Tubs("solidFiberCore", 0, fFiberRadius - fFiberInnerCladdingThickness - fFiberOuterCladdingThickness, length / 2, 0, twopi);
			auto solidFiberInnerCladding = new G4Tubs("solidInnerCladding", fFiberRadius - fFiberInnerCladdingThickness - fFiberOuterCladdingThickness, fFiberRadius - fFiberOuterCladdingThickness, length / 2, 0, twopi);
			auto solidFiberOuterCladding = new G4Tubs("solidFiberOuterCladding", fFiberRadius - fFiberOuterCladdingThickness, fFiberRadius, length / 2, 0, twopi);

			G4Material *CoreMaterial = matPolystyrene;
			logicFiber = new G4LogicalVolume(solidFiber, CoreMaterial, "logicFiber");
			logicFiberCore = new G4LogicalVolume(solidFiberCore, CoreMaterial, "logicFiberCore");
			logicFiberInnerCladding = new G4LogicalVolume(solidFiberInnerCladding, matPMMA, "logicFiberInnerCladding");
			logicFiberOuterCladding = new G4LogicalVolume(solidFiberOuterCladding, matFluorAcrylic, "logicFiberOuterCladding");

			auto physFiberCore = new G4PVPlacement(0, G4ThreeVector(), logicFiberCore, "FiberCore", logicFiber, false, 0, CheckOverlaps);
			auto physFiberInnerCladding = new G4PVPlacement(0, G4ThreeVector(), logicFiberInnerCladding, "FiberInnerCladding", logicFiber, false, 0, CheckOverlaps);
			auto physFiberOuterCladding = new G4PVPlacement(0, G4ThreeVector(), logicFiberOuterCladding, "FiberOuterCladding", logicFiber, false, 0, CheckOverlaps);
		}
	}
	else if (fFiberShape == "Rec")
	{
		if (ifFiberTPB == true)
		{
			auto solidFiber = new G4Box("solidFiber", fFiberRadius + fFiberTPBThickness, fFiberRadius + fFiberTPBThickness, length / 2);
			auto solidFiberCore = new G4Box("solidFiberCore", fFiberRadius - fFiberInnerCladdingThickness - fFiberOuterCladdingThickness, fFiberRadius - fFiberInnerCladdingThickness - fFiberOuterCladdingThickness, length / 2);
			auto solidToFiberInnerCladding = new G4Box("solidInnerCladding", fFiberRadius - fFiberOuterCladdingThickness, fFiberRadius - fFiberOuterCladdingThickness, length / 2);
			auto solidToFiberOuterCladding = new G4Box("solidFiberOuterCladding", fFiberRadius, fFiberRadius, length / 2);

			auto solidFiberInnerCladding = new G4SubtractionSolid("solidFiberInnerCladding", solidToFiberInnerCladding, solidFiberCore);
			auto solidFiberOuterCladding = new G4SubtractionSolid("solidFiberOuterCladding", solidToFiberOuterCladding, solidToFiberInnerCladding);
			auto solidFiberWLSLayer = new G4SubtractionSolid("solidFiberWLSLayer", solidFiber, solidToFiberOuterCladding);
			G4Material *CoreMaterial = matPolystyrene;
			logicFiber = new G4LogicalVolume(solidFiber, CoreMaterial, "logicFiber");
			logicFiberCore = new G4LogicalVolume(solidFiberCore, CoreMaterial, "logicFiberCore");
			logicFiberInnerCladding = new G4LogicalVolume(solidFiberInnerCladding, matPMMA, "logicFiberInnerCladding");
			logicFiberOuterCladding = new G4LogicalVolume(solidFiberOuterCladding, matFluorAcrylic, "logicFiberOuterCladding");

			logicFiberWLSLayer = new G4LogicalVolume(solidFiberWLSLayer, matTPB, "logicFiberWLSLayer");

			auto physFiberCore = new G4PVPlacement(0, G4ThreeVector(), logicFiberCore, "FiberCore", logicFiber, false, 0, CheckOverlaps);
			auto physFiberInnerCladding = new G4PVPlacement(0, G4ThreeVector(), logicFiberInnerCladding, "FiberInnerCladding", logicFiber, false, 0, CheckOverlaps);
			auto physFiberOuterCladding = new G4PVPlacement(0, G4ThreeVector(), logicFiberOuterCladding, "FiberOuterCladding", logicFiber, false, 0, CheckOverlaps);
			auto physFiberWLSLayer = new G4PVPlacement(0, G4ThreeVector(), logicFiberWLSLayer, "FiberWLSLayer", logicFiber, false, 0, CheckOverlaps);
		}
		else
		{
			auto solidFiber = new G4Box("solidFiber", fFiberRadius, fFiberRadius, length / 2);
			auto solidFiberCore = new G4Box("solidFiberCore", fFiberRadius - fFiberInnerCladdingThickness - fFiberOuterCladdingThickness, fFiberRadius - fFiberInnerCladdingThickness - fFiberOuterCladdingThickness, length / 2);
			auto solidToFiberInnerCladding = new G4Box("solidInnerCladding", fFiberRadius - fFiberOuterCladdingThickness, fFiberRadius - fFiberOuterCladdingThickness, length / 2);

			auto solidFiberInnerCladding = new G4SubtractionSolid("solidFiberInnerCladding", solidToFiberInnerCladding, solidFiberCore);
			auto solidFiberOuterCladding = new G4SubtractionSolid("solidFiberOuterCladding", solidFiber, solidToFiberInnerCladding);
			G4Material *CoreMaterial = matPolystyrene;
			logicFiber = new G4LogicalVolume(solidFiber, CoreMaterial, "logicFiber");
			logicFiberCore = new G4LogicalVolume(solidFiberCore, CoreMaterial, "logicFiberCore");
			logicFiberInnerCladding = new G4LogicalVolume(solidFiberInnerCladding, matPMMA, "logicFiberInnerCladding");
			logicFiberOuterCladding = new G4LogicalVolume(solidFiberOuterCladding, matFluorAcrylic, "logicFiberOuterCladding");
			auto physFiberCore = new G4PVPlacement(0, G4ThreeVector(), logicFiberCore, "FiberCore", logicFiber, false, 0, CheckOverlaps);
			auto physFiberInnerCladding = new G4PVPlacement(0, G4ThreeVector(), logicFiberInnerCladding, "FiberInnerCladding", logicFiber, false, 0, CheckOverlaps);
			auto physFiberOuterCladding = new G4PVPlacement(0, G4ThreeVector(), logicFiberOuterCladding, "FiberOuterCladding", logicFiber, false, 0, CheckOverlaps);
		}
	}
	const G4int NUMENTRIES_FIBER = 4;
	G4double Wavelength[NUMENTRIES_FIBER] = {100., 200., 301., 650.};

	G4double RefFiberPhotonEnergy[NUMENTRIES_FIBER];
	G4double RefFiberReflectivity[NUMENTRIES_FIBER];
	G4double RefFiberEfficiency[NUMENTRIES_FIBER];

	for (G4int ji = 0; ji < NUMENTRIES_FIBER; ji++)
	{
		RefFiberPhotonEnergy[ji] = LambdaE / (Wavelength[(NUMENTRIES_FIBER - 1) - ji] * nm);
		RefFiberReflectivity[ji] = 0.98;
		RefFiberEfficiency[ji] = 1.0;
	}
	G4MaterialPropertiesTable *FiberRef_Surf_MPT = new G4MaterialPropertiesTable();
	FiberRef_Surf_MPT->AddProperty("REFLECTIVITY", RefFiberPhotonEnergy, RefFiberReflectivity, NUMENTRIES_FIBER);
	FiberRef_Surf_MPT->AddProperty("EFFICIENCY", RefFiberPhotonEnergy, RefFiberEfficiency, NUMENTRIES_FIBER);

	G4OpticalSurface *FiberRef_Surf = new G4OpticalSurface("Fiber reflective surface", glisur, polished, dielectric_metal);
	FiberRef_Surf->SetMaterialPropertiesTable(FiberRef_Surf_MPT);

	G4OpticalSurface *FiberCladding_Surf = new G4OpticalSurface("FiberCladding_Surf", glisur, polished, dielectric_dielectric);
	G4OpticalSurface *FiberCoating_Surf = new G4OpticalSurface("FiberCoating_Surf", unified, polished, dielectric_dielectric);
	G4OpticalSurface *FiberCore_Surf = new G4OpticalSurface("FiberCoreSurface", glisur, polished, dielectric_dielectric);

	G4LogicalSkinSurface *InnerCladding_LSS = new G4LogicalSkinSurface("InnerCladding_LSS", logicFiberInnerCladding, FiberCladding_Surf);
	G4LogicalSkinSurface *OuterCladding_LSS = new G4LogicalSkinSurface("OuterCladding_LSS", logicFiberOuterCladding, FiberCladding_Surf);
	G4LogicalSkinSurface *FiberCore_LSS = new G4LogicalSkinSurface("FiberCore_LSS", logicFiberCore, FiberCore_Surf);
	G4LogicalSkinSurface *Fiber_LSS = new G4LogicalSkinSurface("Fiber_LSS", logicFiber, FiberCore_Surf);
	if (ifFiberTPB == true)
	{
		G4LogicalSkinSurface *WLSLayer_LSS = new G4LogicalSkinSurface("WLSLayer_LSS", logicFiberWLSLayer, FiberCoating_Surf);
	}

	return logicFiber;
}

void CDEXDetectorConstruction::ConstructLightFiberArray(G4LogicalVolume *motherLV, G4ThreeVector pos, G4double radius)
{
	G4double FiberLength = fFiberLength;
	G4double FiberSiPMThickness = 0.5 * mm;
	G4double FiberSiPMTPBThickness = 2 * micrometer;
	G4double PlacementRadius = radius;
	G4ThreeVector PlacementCenter = pos;
	G4LogicalVolume *logicFiber = ConstructLightFiber(FiberLength);
	G4LogicalVolume *logicFiberSiPMChip = ConstructFiberSiPM();
	G4LogicalVolume *MotherLV = motherLV;

	G4LogicalVolume *logicSiBoard;
	G4double BoardThickness = 1 * mm;
	if (fFiberShape == "Round")
	{
		G4Tubs *solidSiBoard = new G4Tubs("solidSiBoard", 0, fFiberRadius, BoardThickness / 2, 0, twopi);
		logicSiBoard = new G4LogicalVolume(solidSiBoard, matSi, "logicSiBoard");
	}
	else if (fFiberShape == "Rec")
	{
		G4Box *solidSiBoard = new G4Box("solidSiBoard", fFiberRadius, fFiberRadius, BoardThickness / 2);
		logicSiBoard = new G4LogicalVolume(solidSiBoard, matSi, "logicSiBoard");
	}
	G4double DeltaAngle = 2 * asin((fFiberRadius + fFiberTPBThickness) / PlacementRadius);
	G4int FiberAmount = std::floor(twopi / DeltaAngle);
	G4double RealDeltaAngle = twopi / FiberAmount;

	G4PVPlacement *physLightFiberSet[4000] = {nullptr};
	G4PVPlacement *physFiberSiPMSetUp[4000] = {nullptr};
	G4PVPlacement *physSiBoardSetUp[4000] = {nullptr};
	G4PVPlacement *physFiberSiPMSetDown[4000] = {nullptr};
	G4PVPlacement *physSiBoardSetDown[4000] = {nullptr};
	for (G4int FiberNb = 0; FiberNb < FiberAmount; FiberNb++)
	{
		G4double PosAngle = FiberNb * RealDeltaAngle;
		G4ThreeVector posFiber = G4ThreeVector((PlacementRadius+fFiberRadius) * cos(PosAngle), (PlacementRadius+fFiberRadius)  * sin(PosAngle), 0);
		G4ThreeVector posFiberSiPMUp = G4ThreeVector(0, 0, FiberLength / 2 + FiberSiPMThickness / 2 + FiberSiPMTPBThickness);
		G4ThreeVector posFiberSiPMDown = G4ThreeVector(0, 0, -FiberLength / 2 - FiberSiPMThickness / 2 - FiberSiPMTPBThickness);
		G4RotationMatrix *rotFiber = new G4RotationMatrix();
		rotFiber->rotateZ(-PosAngle);
		physLightFiberSet[FiberNb] = new G4PVPlacement(rotFiber, posFiber + PlacementCenter, logicFiber, "Fiber", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
		physFiberSiPMSetUp[FiberNb] = new G4PVPlacement(rotFiber, posFiberSiPMUp + posFiber + PlacementCenter, logicFiberSiPMChip, "FiberSiPM", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
		physSiBoardSetUp[FiberNb] = new G4PVPlacement(rotFiber, posFiberSiPMUp + posFiber + PlacementCenter + G4ThreeVector(0, 0, FiberSiPMThickness / 2 + FiberSiPMTPBThickness + BoardThickness / 2), logicSiBoard, "SiBoard", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
		physFiberSiPMSetDown[FiberNb] = new G4PVPlacement(rotFiber, posFiberSiPMDown + posFiber + PlacementCenter, logicFiberSiPMChip, "FiberSiPM", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
		physSiBoardSetDown[FiberNb] = new G4PVPlacement(rotFiber, posFiberSiPMDown + posFiber + PlacementCenter + G4ThreeVector(0, 0, -FiberSiPMThickness / 2 - FiberSiPMTPBThickness - BoardThickness / 2), logicSiBoard, "SiBoard", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
	}
}

void CDEXDetectorConstruction::ConstructHexLightFiberArray(G4LogicalVolume *motherLV, G4ThreeVector pos, G4double circumradius)
{
	G4double FiberLength = fFiberLength;
	G4double FiberSiPMThickness = 0.5 * mm;
	G4double FiberSiPMTPBThickness = 2 * micrometer;
	G4double PlacementRadius = circumradius;
	G4ThreeVector PlacementCenter = pos;
	G4LogicalVolume *logicFiber = ConstructLightFiber(FiberLength);
	G4LogicalVolume *logicFiberSiPMChip = ConstructFiberSiPM();
	G4LogicalVolume *MotherLV = motherLV;
	G4int EdgeNb = std::floor(circumradius / fFiberRadius / 2)-1;
	G4ThreeVector PosFiberList[EdgeNb * 6 - 6];
	G4double RotFiberList[EdgeNb * 6 - 6];

	for (G4int FiberNb = 0; FiberNb < EdgeNb - 1; FiberNb++)
	{
		G4ThreeVector posFiberFront = G4ThreeVector(circumradius * (-1.0 + 1.0 / 2.0 / EdgeNb * (FiberNb + 1)), circumradius * (sqrt(3) / 2.0 / EdgeNb * (FiberNb + 1)), 0);
		G4ThreeVector posFiberBack = G4ThreeVector(circumradius * (-1.0 + 1.0 / 2.0 / EdgeNb * (FiberNb + 1)), -circumradius * (sqrt(3) / 2.0 / EdgeNb * (FiberNb + 1)), 0);
		PosFiberList[FiberNb * 2] = posFiberFront;
		PosFiberList[FiberNb * 2 + 1] = posFiberBack;
		RotFiberList[FiberNb * 2] = -pi / 3;
		RotFiberList[FiberNb * 2 + 1] = pi / 3;
	}

	for (G4int FiberNb = 0; FiberNb < EdgeNb - 1; FiberNb++)
	{
		G4ThreeVector posFiberFront = G4ThreeVector(circumradius * (-1.0 / 2.0 + 1.0 / EdgeNb * (FiberNb + 1)), circumradius * (sqrt(3) / 2.0), 0);
		G4ThreeVector posFiberBack = G4ThreeVector(circumradius * (-1.0 / 2.0 + 1.0 / EdgeNb * (FiberNb + 1)), -circumradius * (sqrt(3) / 2.0), 0);
		PosFiberList[FiberNb * 2 + 2 * EdgeNb - 2] = posFiberFront;
		PosFiberList[FiberNb * 2 + 2 * EdgeNb - 1] = posFiberBack;
		RotFiberList[FiberNb * 2 + 2 * EdgeNb - 2] = 0;
		RotFiberList[FiberNb * 2 + 2 * EdgeNb - 1] = 0;
	}

	for (G4int FiberNb = 0; FiberNb < EdgeNb - 1; FiberNb++)
	{
		G4ThreeVector posFiberFront = G4ThreeVector(circumradius * (1.0 / 2.0 + 1.0 / 2.0 / EdgeNb * (FiberNb + 1)), circumradius * (sqrt(3) / 2.0 - sqrt(3) / 2.0 / EdgeNb * (FiberNb + 1)), 0);
		G4ThreeVector posFiberBack = G4ThreeVector(circumradius * (1.0 / 2.0 + 1.0 / 2.0 / EdgeNb * (FiberNb + 1)), -circumradius * (sqrt(3) / 2.0 - sqrt(3) / 2.0 / EdgeNb * (FiberNb + 1)), 0);
		PosFiberList[FiberNb * 2 + 4 * EdgeNb - 4] = posFiberFront;
		PosFiberList[FiberNb * 2 + 4 * EdgeNb - 3] = posFiberBack;
		RotFiberList[FiberNb * 2 + 4 * EdgeNb - 4] = pi / 3;
		RotFiberList[FiberNb * 2 + 4 * EdgeNb - 3] = -pi / 3;
	}

	G4double BoardThickness = 1 * mm;
	G4LogicalVolume *logicSiBoard;
	if (fFiberShape == "Round")
	{
		G4Tubs *solidSiBoard = new G4Tubs("solidSiBoard", 0, fFiberRadius, BoardThickness / 2, 0, twopi);
		logicSiBoard = new G4LogicalVolume(solidSiBoard, matSi, "logicSiBoard");
	}
	else if (fFiberShape == "Rec")
	{
		G4Box *solidSiBoard = new G4Box("solidSiBoard", fFiberRadius, fFiberRadius, BoardThickness / 2);
		logicSiBoard = new G4LogicalVolume(solidSiBoard, matSi, "logicSiBoard");
	}

	G4PVPlacement *physLightFiberSet[4000] = {nullptr};
	G4PVPlacement *physFiberSiPMSetUp[4000] = {nullptr};
	G4PVPlacement *physSiBoardSetUp[4000] = {nullptr};
	G4PVPlacement *physFiberSiPMSetDown[4000] = {nullptr};
	G4PVPlacement *physSiBoardSetDown[4000] = {nullptr};
	for (G4int FiberNb = 0; FiberNb < 6 * EdgeNb - 6; FiberNb++)
	{
		G4ThreeVector posFiber = PosFiberList[FiberNb];

		G4ThreeVector posFiberSiPMUp = G4ThreeVector(0, 0, FiberLength / 2 + FiberSiPMThickness / 2 + FiberSiPMTPBThickness);
		G4ThreeVector posFiberSiPMDown = G4ThreeVector(0, 0, -FiberLength / 2 - FiberSiPMThickness / 2 - FiberSiPMTPBThickness);
		G4RotationMatrix *rotFiber = new G4RotationMatrix();
		rotFiber->rotateZ(RotFiberList[FiberNb]);
		physLightFiberSet[FiberNb] = new G4PVPlacement(rotFiber, posFiber + PlacementCenter, logicFiber, "Fiber", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
		physFiberSiPMSetUp[FiberNb] = new G4PVPlacement(rotFiber, posFiberSiPMUp + posFiber + PlacementCenter, logicFiberSiPMChip, "FiberSiPM", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
		physSiBoardSetUp[FiberNb] = new G4PVPlacement(rotFiber, posFiberSiPMUp + posFiber + PlacementCenter + G4ThreeVector(0, 0, FiberSiPMThickness / 2 + FiberSiPMTPBThickness + BoardThickness / 2), logicSiBoard, "SiBoard", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
		physFiberSiPMSetDown[FiberNb] = new G4PVPlacement(rotFiber, posFiberSiPMDown + posFiber + PlacementCenter, logicFiberSiPMChip, "FiberSiPM", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
		physSiBoardSetDown[FiberNb] = new G4PVPlacement(rotFiber, posFiberSiPMDown + posFiber + PlacementCenter + G4ThreeVector(0, 0, -FiberSiPMThickness / 2 - FiberSiPMTPBThickness - BoardThickness / 2), logicSiBoard, "SiBoard", MotherLV, false, std::floor(FiberNb / 30), CheckOverlaps);
	}
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructLightGuide(G4double length, G4double radius, G4Material *Coremat)
{
	G4LogicalVolume *logicLightGuide;
	G4LogicalVolume *logicLightGuideInnerTPB;
	G4LogicalVolume *logicLightGuideOuterTPB;
	// G4LogicalVolume *logicLightGuideCoreVolume;
	G4LogicalVolume *logicLightGuideReflector;
	G4LogicalVolume *logicLightGuideInnerLayer;

	G4double LightGuideTPBCoatingThickness = 2 * micrometer;
	G4double LightGuideReflectorThickness = 0.5 * mm;
	G4double LightGuideOuterRadius = radius;
	G4double LightGuideInnerRadius = LightGuideOuterRadius - 2 * LightGuideTPBCoatingThickness - LightGuideReflectorThickness;
	fLightGuideInnerRadius = LightGuideInnerRadius;

	auto solidLightGuideSolid = new G4Tubs("solidLightGuideSolid", 0, LightGuideOuterRadius, length / 2, 0, twopi);
	auto solidLightGuideCoreVolume = new G4Tubs("solidLightCoreVolume", 0, LightGuideInnerRadius, length / 2, 0, twopi);
	auto solidLightGuide = new G4SubtractionSolid("solidLightGuide", solidLightGuideSolid, solidLightGuideCoreVolume);
	// logicLightGuideCoreVolume = new G4LogicalVolume(solidLightGuideCoreVolume, Coremat, "logicLightGuideCoreVolume");
	// G4OpticalSurface *LightGuideCore_Surf = new G4OpticalSurface("LightGuideCore_Surf", glisur, polished, dielectric_dielectric);
	// G4LogicalSkinSurface *LightGuideCore_Surf_LSS = new G4LogicalSkinSurface("LightGuideCore_Surf_LSS", logicLightGuideCoreVolume, LightGuideCore_Surf);

	logicLightGuide = new G4LogicalVolume(solidLightGuide, Coremat, "logicLightGuide");
	G4OpticalSurface *LightGuide_Surf = new G4OpticalSurface("LightGuideSurf", glisur, polished, dielectric_dielectric);
	G4LogicalSkinSurface *LightGuide_Surf_LSS = new G4LogicalSkinSurface("LightGuide_Surf_LSS", logicLightGuide, LightGuide_Surf);

	auto solidLightGuideOuterTPB = new G4Tubs("solidLightGuideOuterTPB", LightGuideOuterRadius - LightGuideTPBCoatingThickness, LightGuideOuterRadius, length / 2, 0, twopi);
	logicLightGuideOuterTPB = new G4LogicalVolume(solidLightGuideOuterTPB, matTPB, "logicLightGuideOuterTPB");
	auto solidLightGuideInnerTPB = new G4Tubs("solidLightGuideInnerTPB", LightGuideInnerRadius, LightGuideInnerRadius + LightGuideTPBCoatingThickness, length / 2, 0, twopi);
	logicLightGuideInnerTPB = new G4LogicalVolume(solidLightGuideInnerTPB, matTPB, "logicLightGuideOuterTPB");
	G4OpticalSurface *LightGuideTPB_Surf = new G4OpticalSurface("LightGuideTPBSurf", glisur, polished, dielectric_dielectric);
	G4LogicalSkinSurface *LightGuideOuterTPB_Surf_LSS = new G4LogicalSkinSurface("LightGuideOuterTPB_Surf_LSS", logicLightGuideOuterTPB, LightGuideTPB_Surf);
	G4LogicalSkinSurface *LightGuideInnerTPB_Surf_LSS = new G4LogicalSkinSurface("LightGuideInnerTPB_Surf_LSS", logicLightGuideInnerTPB, LightGuideTPB_Surf);

	auto solidLightGuideReflector = new G4Tubs("solidLightGuideWall", LightGuideInnerRadius + LightGuideTPBCoatingThickness, LightGuideOuterRadius - LightGuideTPBCoatingThickness, length / 2, 0, twopi);
	logicLightGuideReflector = new G4LogicalVolume(solidLightGuideReflector, matTeflon, "logicLightGuideReflector");

	const G4int NUMENTRIES_FIBER = 4;
	G4double Wavelength[NUMENTRIES_FIBER] = {100., 200., 301., 650.};

	G4double RefFiberPhotonEnergy[NUMENTRIES_FIBER];
	G4double RefFiberReflectivity[NUMENTRIES_FIBER];
	G4double RefFiberEfficiency[NUMENTRIES_FIBER];

	for (G4int ji = 0; ji < NUMENTRIES_FIBER; ji++)
	{
		RefFiberPhotonEnergy[ji] = LambdaE / (Wavelength[(NUMENTRIES_FIBER - 1) - ji] * nm);
		RefFiberReflectivity[ji] = 0.98;
		RefFiberEfficiency[ji] = 1.0;
	}
	G4MaterialPropertiesTable *FiberRef_Surf_MPT = new G4MaterialPropertiesTable();
	FiberRef_Surf_MPT->AddProperty("REFLECTIVITY", RefFiberPhotonEnergy, RefFiberReflectivity, NUMENTRIES_FIBER);
	FiberRef_Surf_MPT->AddProperty("EFFICIENCY", RefFiberPhotonEnergy, RefFiberEfficiency, NUMENTRIES_FIBER);

	G4OpticalSurface *LightGuideReflector_Surf = new G4OpticalSurface("LightGuideReflectorSurf", DAVIS, PolishedTeflon_LUT, dielectric_LUTDAVIS);
	// G4OpticalSurface *LightGuideReflector_Surf = new G4OpticalSurface("LightGuideReflectorSurf", unified, polished, dielectric_metal);
	// LightGuideReflector_Surf->SetMaterialPropertiesTable(FiberRef_Surf_MPT);
	G4LogicalSkinSurface *LightGuideReflector_Surf_LSS = new G4LogicalSkinSurface("LightGuideReflector_Surf_LSS", logicLightGuideReflector, LightGuideReflector_Surf);

	auto physLightGuideInnerTPB = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideInnerTPB, "LightGuideInnerTPB", logicLightGuide, false, 0, CheckOverlaps);
	auto physLightGuideOuterTPB = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideOuterTPB, "LightGuideOuterTPB", logicLightGuide, false, 0, CheckOverlaps);
	auto physLightGuideReflector = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideReflector, "LightGuideReflector", logicLightGuide, false, 0, CheckOverlaps);
	// auto physLightGuideCoreVolume = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideCoreVolume, "LightGuideCoreVolume", logicLightGuide, false, 0, CheckOverlaps);
	G4LogicalBorderSurface *LightGuideCore_Surf1_LBS = new G4LogicalBorderSurface("LightGuideCore_Surf_LBS", physLightGuideInnerTPB, physLightGuideReflector, LightGuideReflector_Surf);
	G4LogicalBorderSurface *LightGuideCore_Surf2_LBS = new G4LogicalBorderSurface("LightGuideCore_Surf_LBS", physLightGuideReflector, physLightGuideInnerTPB, LightGuideReflector_Surf);
	G4LogicalBorderSurface *LightGuideCore_Surf3_LBS = new G4LogicalBorderSurface("LightGuideCore_Surf_LBS", physLightGuideOuterTPB, physLightGuideReflector, LightGuideReflector_Surf);
	G4LogicalBorderSurface *LightGuideCore_Surf4_LBS = new G4LogicalBorderSurface("LightGuideCore_Surf_LBS", physLightGuideReflector, physLightGuideOuterTPB, LightGuideReflector_Surf);

	return logicLightGuide;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructRecLightGuide(G4double length, G4Material *Coremat)
{
	G4LogicalVolume *logicLightGuide;
	G4LogicalVolume *logicLightGuideInnerTPB;
	G4LogicalVolume *logicLightGuideOuterTPB;
	G4LogicalVolume *logicLightGuideCoreVolume;
	G4LogicalVolume *logicLightGuideReflector;
	G4LogicalVolume *logicLightGuideInnerLayer;

	G4double LightGuideTPBCoatingThickness = 2 * micrometer;
	G4double LightGuideReflectorThickness = 0.5 * mm;
	G4double LightGuideOuterRadius = fLightGuideRadius;
	G4double LightGuideInnerRadius = LightGuideOuterRadius - 2 * LightGuideTPBCoatingThickness - LightGuideReflectorThickness;
	fLightGuideInnerRadius = LightGuideInnerRadius;

	auto solidLightGuide = new G4Box("solidLightGuide", LightGuideOuterRadius / 2, LightGuideOuterRadius / 2, length / 2);
	logicLightGuide = new G4LogicalVolume(solidLightGuide, matTeflon, "logicLightGuide");
	G4OpticalSurface *LightGuide_Surf = new G4OpticalSurface("LightGuideSurf", glisur, polished, dielectric_dielectric);
	G4LogicalSkinSurface *LightGuide_Surf_LSS = new G4LogicalSkinSurface("LightGuide_Surf_LSS", logicLightGuide, LightGuide_Surf);

	auto solidLightGuideInnerTPBP1 = new G4Box("solidLightGuideInnerTPB", LightGuideOuterRadius / 2 - LightGuideReflectorThickness, LightGuideOuterRadius / 2 - LightGuideReflectorThickness, length / 2);
	auto solidLightGuideInnerTPBP2 = new G4Box("solidLightGuideInnerTPB", LightGuideOuterRadius / 2 - LightGuideReflectorThickness - LightGuideTPBCoatingThickness, LightGuideOuterRadius / 2 - LightGuideReflectorThickness - LightGuideTPBCoatingThickness, length / 2);
	auto solidLightGuideInnerTPB = new G4SubtractionSolid("solidLightGuideInnerTPB", solidLightGuideInnerTPBP1, solidLightGuideInnerTPBP2);
	logicLightGuideInnerTPB = new G4LogicalVolume(solidLightGuideInnerTPB, matTPB, "logicLightGuideOuterTPB");
	G4OpticalSurface *LightGuideTPB_Surf = new G4OpticalSurface("LightGuideTPBSurf", glisur, polished, dielectric_dielectric);
	G4LogicalSkinSurface *LightGuideInnerTPB_Surf_LSS = new G4LogicalSkinSurface("LightGuideInnerTPB_Surf_LSS", logicLightGuideInnerTPB, LightGuideTPB_Surf);

	const G4int NUMENTRIES_FIBER = 4;
	G4double Wavelength[NUMENTRIES_FIBER] = {100., 200., 301., 650.};

	G4double RefFiberPhotonEnergy[NUMENTRIES_FIBER];
	G4double RefFiberReflectivity[NUMENTRIES_FIBER];
	G4double RefFiberEfficiency[NUMENTRIES_FIBER];

	for (G4int ji = 0; ji < NUMENTRIES_FIBER; ji++)
	{
		RefFiberPhotonEnergy[ji] = LambdaE / (Wavelength[(NUMENTRIES_FIBER - 1) - ji] * nm);
		RefFiberReflectivity[ji] = 0.98;
		RefFiberEfficiency[ji] = 1.0;
	}
	G4MaterialPropertiesTable *FiberRef_Surf_MPT = new G4MaterialPropertiesTable();
	FiberRef_Surf_MPT->AddProperty("REFLECTIVITY", RefFiberPhotonEnergy, RefFiberReflectivity, NUMENTRIES_FIBER);
	FiberRef_Surf_MPT->AddProperty("EFFICIENCY", RefFiberPhotonEnergy, RefFiberEfficiency, NUMENTRIES_FIBER);

	auto solidLightGuideReflectorP1 = new G4Box("solidLightGuide", LightGuideOuterRadius / 2, LightGuideOuterRadius / 2, length / 2);
	auto solidLightGuideReflectorP2 = new G4Box("solidLightGuide", LightGuideOuterRadius / 2 - LightGuideReflectorThickness, LightGuideOuterRadius / 2 - LightGuideReflectorThickness, length / 2);
	auto solidLightGuideReflector = new G4SubtractionSolid("solidLightGuide", solidLightGuideReflectorP1, solidLightGuideReflectorP2);

	logicLightGuide = new G4LogicalVolume(solidLightGuide, matTeflon, "logicLightGuide");

	G4OpticalSurface *LightGuideReflector_Surf = new G4OpticalSurface("LightGuideReflectorSurf", DAVIS, PolishedTeflon_LUT, dielectric_LUTDAVIS);
	// G4OpticalSurface *LightGuideReflector_Surf = new G4OpticalSurface("LightGuideReflectorSurf", glisur, polished, dielectric_metal);
	LightGuideReflector_Surf->SetMaterialPropertiesTable(FiberRef_Surf_MPT);
	G4LogicalSkinSurface *LightGuideReflector_Surf_LSS = new G4LogicalSkinSurface("LightGuideReflector_Surf_LSS", logicLightGuideReflector, LightGuideReflector_Surf);

	auto solidLightGuideCoreVolume = new G4Box("solidLightCoreVolume", LightGuideOuterRadius / 2 - LightGuideReflectorThickness - LightGuideTPBCoatingThickness, LightGuideOuterRadius / 2 - LightGuideReflectorThickness - LightGuideTPBCoatingThickness, length / 2);
	logicLightGuideCoreVolume = new G4LogicalVolume(solidLightGuideCoreVolume, Coremat, "logicLightGuideCoreVolume");
	G4OpticalSurface *LightGuideCore_Surf = new G4OpticalSurface("LightGuideCore_Surf", glisur, polished, dielectric_dielectric);
	G4LogicalSkinSurface *LightGuideCore_Surf_LSS = new G4LogicalSkinSurface("LightGuideCore_Surf_LSS", logicLightGuideCoreVolume, LightGuideCore_Surf);

	auto physLightGuideInnerTPB = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideInnerTPB, "LightGuideInnerTPB", logicLightGuide, false, 0, CheckOverlaps);
	// G4PVPlacement *physLightGuideInnerTPBArray[400] = {nullptr};
	// for(G4int i=0;i<100;i++){
	// 	physLightGuideInnerTPBArray[i*2] = new G4PVPlacement(0, G4ThreeVector(0,0,3*i*cm), logicLightGuideInnerTPB, "LightGuideInnerTPB", logicLightGuide, false, 0, CheckOverlaps);
	// 	physLightGuideInnerTPBArray[i*2+1] = new G4PVPlacement(0, G4ThreeVector(0,0,-3*i*cm), logicLightGuideInnerTPB, "LightGuideInnerTPB", logicLightGuide, false, 0, CheckOverlaps);
	// }
	// auto physLightGuideOuterTPB = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideOuterTPB, "LightGuideOuterTPB", logicLightGuide, false, 0, CheckOverlaps);
	auto physLightGuideReflector = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideReflector, "LightGuideReflector", logicLightGuide, false, 0, CheckOverlaps);
	auto physLightGuideCoreVolume = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideCoreVolume, "LightGuideCoreVolume", logicLightGuide, false, 0, CheckOverlaps);
	// auto physLightGuideInnerLayer = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideInnerLayer, "LightGuideInnerLayer", logicLightGuide, false, 0, CheckOverlaps);
	//  G4PVPlacement *physLightGuideInnerLayerArray[400] = {nullptr};
	//  for(G4int i=0;i<100;i++){
	//  	physLightGuideInnerLayerArray[i*2] = new G4PVPlacement(0, G4ThreeVector(0,0,3*i*cm), logicLightGuideInnerLayer, "LightGuideInnerLayer", logicLightGuide, false, 0, CheckOverlaps);
	//  	physLightGuideInnerLayerArray[i*2+1] = new G4PVPlacement(0, G4ThreeVector(0,0,-3*i*cm), logicLightGuideInnerLayer, "LightGuideInnerLayer", logicLightGuide, false, 0, CheckOverlaps);
	//  }
	return logicLightGuide;
}

G4LogicalVolume *CDEXDetectorConstruction::ConstructHexLightGuide(G4double length, G4double circumradius, G4Material *Coremat)
{
	G4LogicalVolume *logicLightGuide;
	G4LogicalVolume *logicLightGuideInnerTPB;
	G4LogicalVolume *logicLightGuideOuterTPB;
	G4LogicalVolume *logicLightGuideCoreVolume;
	G4LogicalVolume *logicLightGuideReflector;
	G4LogicalVolume *logicLightGuideInnerLayer;

	G4double LightGuideTPBCoatingThickness = 2 * micrometer;
	G4double LightGuideReflectorThickness = 0.5 * mm;
	G4double LightGuideOuterRadius = fLightGuideRadius;
	G4double LightGuideInnerRadius = LightGuideOuterRadius - 2 * LightGuideTPBCoatingThickness - LightGuideReflectorThickness;
	fLightGuideInnerRadius = LightGuideInnerRadius;
	const G4int nsect = 6;
	std::vector<G4TwoVector> Polygon0(nsect);
	std::vector<G4TwoVector> Polygon1(nsect);
	std::vector<G4TwoVector> Polygon2(nsect);
	std::vector<G4TwoVector> Polygon3(nsect);
	G4double ang = twopi / nsect;
	for (G4int i = 0; i < nsect; ++i)
	{
		G4double phi = i * ang;
		G4double cosphi = std::cos(phi);
		G4double sinphi = std::sin(phi);
		Polygon0[i].set(circumradius * cosphi, circumradius * sinphi);
		Polygon1[i].set((circumradius - LightGuideTPBCoatingThickness / std::sqrt(3) * 2) * cosphi, (circumradius - LightGuideTPBCoatingThickness / std::sqrt(3) * 2) * sinphi);
		Polygon2[i].set((circumradius - (LightGuideTPBCoatingThickness + LightGuideReflectorThickness) / std::sqrt(3) * 2) * cosphi, (circumradius - (LightGuideTPBCoatingThickness + LightGuideReflectorThickness) / std::sqrt(3) * 2) * sinphi);
		Polygon3[i].set((circumradius - (2 * LightGuideTPBCoatingThickness + LightGuideReflectorThickness) / std::sqrt(3) * 2) * cosphi, (circumradius - (2 * LightGuideTPBCoatingThickness + LightGuideReflectorThickness) / std::sqrt(3) * 2) * sinphi);
	}

	G4TwoVector offsetA(0, 0), offsetB(0, 0);
	G4double scaleA = 1, scaleB = 1;
	G4VSolid *Extruded0 = new G4ExtrudedSolid("Extruded0", Polygon0, length / 2, offsetA, scaleA, offsetB, scaleB);
	G4VSolid *Extruded1 = new G4ExtrudedSolid("Extruded1", Polygon1, length / 2 + 0.1 * mm, offsetA, scaleA, offsetB, scaleB);
	G4VSolid *Extruded2 = new G4ExtrudedSolid("Extruded2", Polygon2, length / 2 + 0.2 * mm, offsetA, scaleA, offsetB, scaleB);
	G4VSolid *Extruded3 = new G4ExtrudedSolid("Extruded3", Polygon3, length / 2 + 0.3 * mm, offsetA, scaleA, offsetB, scaleB); // Small values make sure the tube is open

	auto solidLightGuide = new G4SubtractionSolid("solidLightGuide", Extruded0, Extruded3);
	auto solidLightGuideOuterTPB = new G4SubtractionSolid("solidLightGuide", Extruded0, Extruded1);
	auto solidLightGuideReflector = new G4SubtractionSolid("solidLightGuide", Extruded1, Extruded2);
	auto solidLightGuideInnerTPB = new G4SubtractionSolid("solidLightGuide", Extruded2, Extruded3);

	logicLightGuide = new G4LogicalVolume(solidLightGuide, Coremat, "logicLightGuide");
	G4OpticalSurface *LightGuide_Surf = new G4OpticalSurface("LightGuideSurf", glisur, polished, dielectric_dielectric);
	G4LogicalSkinSurface *LightGuide_Surf_LSS = new G4LogicalSkinSurface("LightGuide_Surf_LSS", logicLightGuide, LightGuide_Surf);

	logicLightGuideOuterTPB = new G4LogicalVolume(solidLightGuideOuterTPB, matTPB, "logicLightGuideOuterTPB");

	logicLightGuideInnerTPB = new G4LogicalVolume(solidLightGuideInnerTPB, matTPB, "logicLightGuideOuterTPB");
	G4OpticalSurface *LightGuideTPB_Surf = new G4OpticalSurface("LightGuideTPBSurf", glisur, polished, dielectric_dielectric);
	G4LogicalSkinSurface *LightGuideOuterTPB_Surf_LSS = new G4LogicalSkinSurface("LightGuideOuterTPB_Surf_LSS", logicLightGuideOuterTPB, LightGuideTPB_Surf);
	G4LogicalSkinSurface *LightGuideInnerTPB_Surf_LSS = new G4LogicalSkinSurface("LightGuideInnerTPB_Surf_LSS", logicLightGuideInnerTPB, LightGuideTPB_Surf);

	logicLightGuideReflector = new G4LogicalVolume(solidLightGuideReflector, matTeflon, "logicLightGuideReflector");

	const G4int NUMENTRIES_FIBER = 4;
	G4double Wavelength[NUMENTRIES_FIBER] = {100., 200., 301., 650.};

	G4double RefFiberPhotonEnergy[NUMENTRIES_FIBER];
	G4double RefFiberReflectivity[NUMENTRIES_FIBER];
	G4double RefFiberEfficiency[NUMENTRIES_FIBER];

	for (G4int ji = 0; ji < NUMENTRIES_FIBER; ji++)
	{
		RefFiberPhotonEnergy[ji] = LambdaE / (Wavelength[(NUMENTRIES_FIBER - 1) - ji] * nm);
		RefFiberReflectivity[ji] = 0.98;
		RefFiberEfficiency[ji] = 1.0;
	}
	G4MaterialPropertiesTable *FiberRef_Surf_MPT = new G4MaterialPropertiesTable();
	FiberRef_Surf_MPT->AddProperty("REFLECTIVITY", RefFiberPhotonEnergy, RefFiberReflectivity, NUMENTRIES_FIBER);
	FiberRef_Surf_MPT->AddProperty("EFFICIENCY", RefFiberPhotonEnergy, RefFiberEfficiency, NUMENTRIES_FIBER);

	G4OpticalSurface *LightGuideReflector_Surf = new G4OpticalSurface("LightGuideReflectorSurf", DAVIS, PolishedTeflon_LUT, dielectric_LUTDAVIS);
	// G4OpticalSurface *LightGuideReflector_Surf = new G4OpticalSurface("LightGuideReflectorSurf", unified, polished, dielectric_metal);
	// LightGuideReflector_Surf->SetMaterialPropertiesTable(FiberRef_Surf_MPT);
	G4LogicalSkinSurface *LightGuideReflector_Surf_LSS = new G4LogicalSkinSurface("LightGuideReflector_Surf_LSS", logicLightGuideReflector, LightGuideReflector_Surf);

	auto physLightGuideInnerTPB = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideInnerTPB, "LightGuideInnerTPB", logicLightGuide, false, 0, CheckOverlaps);
	auto physLightGuideOuterTPB = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideOuterTPB, "LightGuideOuterTPB", logicLightGuide, false, 0, CheckOverlaps);
	auto physLightGuideReflector = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideReflector, "LightGuideReflector", logicLightGuide, false, 0, CheckOverlaps);
	// auto physLightGuideCoreVolume = new G4PVPlacement(0, G4ThreeVector(), logicLightGuideCoreVolume, "LightGuideCoreVolume", logicLightGuide, false, 0, CheckOverlaps);
	G4LogicalBorderSurface *LightGuideCore_Surf1_LBS = new G4LogicalBorderSurface("LightGuideCore_Surf_LBS", physLightGuideInnerTPB, physLightGuideReflector, LightGuideReflector_Surf);
	G4LogicalBorderSurface *LightGuideCore_Surf2_LBS = new G4LogicalBorderSurface("LightGuideCore_Surf_LBS", physLightGuideReflector, physLightGuideInnerTPB, LightGuideReflector_Surf);
	G4LogicalBorderSurface *LightGuideCore_Surf3_LBS = new G4LogicalBorderSurface("LightGuideCore_Surf_LBS", physLightGuideOuterTPB, physLightGuideReflector, LightGuideReflector_Surf);
	G4LogicalBorderSurface *LightGuideCore_Surf4_LBS = new G4LogicalBorderSurface("LightGuideCore_Surf_LBS", physLightGuideReflector, physLightGuideOuterTPB, LightGuideReflector_Surf);

	return logicLightGuide;
}
// Construct Systems
G4VPhysicalVolume *CDEXDetectorConstruction::ConstructUnit()
{
	G4NistManager *nist = G4NistManager::Instance();
	G4bool checkOverlaps = true;

	// Vaccum for world
	// G4Material* vacuum=new G4Material("Galactic",z=1.,a=1.01*g/mole,density=universe_mean_density,kStateGas,2.73*kelvin,3.e-18*pascal);

	//------------------------------------------------------ volumes
	//
	G4Material *world_mat = fVacuum;
	G4Material *env_mat = matLN2;
	G4Material *det_mat = matEnGe;

	//
	// World&Envelope
	//
	G4double world_size = 200 * cm;
	G4double env_size = 180 * cm;
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "logicWorld");
	G4VPhysicalVolume *physWorld =
		new G4PVPlacement(0,			   // no rotation
						  G4ThreeVector(), // at (0,0,0)
						  logicWorld,	   // its logical volume
						  "World",		   // its name
						  0,			   // its mother  volume
						  false,		   // no boolean operation
						  0,			   // copy number
						  checkOverlaps);  // overlaps checking

	G4Box *solidEnv = new G4Box("solidEnvelope", 0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "logicEnvelope");
	physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

	//========================PEN shell and wire paremeters========================//

	G4String WireType = fWireType;
	G4double wireradius = 0.7 * mm;
	G4double LN2Gap = 0.5 * mm;
	G4double ShellThickness = 1 * cm;
	G4double WireLength = 71 * mm;
	G4double outerGeRadius = 40 * mm;

	//=============================================================================//

	fWireLength = WireLength;
	G4double WireCentDist;
	G4ThreeVector WirePlacement;

	if (WireType == "A1")
	{
		G4LogicalVolume *logicWire = ConstructA1(WireLength);
		fWireCentDist = outerGeRadius + LN2Gap + ShellThickness + fWireRadius;
		WirePlacement = G4ThreeVector(fWireCentDist, 0, 0);
		fWirePos = WirePlacement;
		// physWire = new G4PVPlacement(0, WirePlacement, logicWire, "Wire", logicEnv, false, 0, checkOverlaps);
	}

	else if (WireType == "A2")
	{
		G4LogicalVolume *logicWire = ConstructA2(WireLength);
		fWireCentDist = outerGeRadius + LN2Gap + ShellThickness + fWireRadius;
		WirePlacement = G4ThreeVector(fWireCentDist, 0, 0);
		fWirePos = WirePlacement;
		// physWire = new G4PVPlacement(0, WirePlacement, logicWire, "Wire", logicEnv, false, 0, checkOverlaps);
	}

	else
	{
		G4cout << "Type does not exist!" << G4endl;
	}

	//=============================================================//
	//                          PENShell                           //
	//=============================================================//
	auto rotPENShell = new G4RotationMatrix();
	rotPENShell->rotateX(90 * degree);
	auto rotPENShell1 = new G4RotationMatrix();
	// G4ThreeVector PENShellPlacement =G4ThreeVector(0, 0, 34.1 * mm);
	G4double OSConeHeight0 = 37.313 * mm;
	G4ThreeVector PENShellPlacement = G4ThreeVector(0, 0, 0);
	G4ThreeVector PENShellPlacement1 = G4ThreeVector(0, 0, OSConeHeight0 / 2);
	// G4LogicalVolume* logicPENShell = ConstructCSGPENShell();
	// physPENShell = new G4PVPlacement(rotPENShell, PENShellPlacement, logicPENShell, "PENShell", logicEnv, false, 0, checkOverlaps);

	G4LogicalVolume *logicOuterShell = ConstructOuterShell();
	physOuterShell = new G4PVPlacement(rotPENShell, PENShellPlacement, logicOuterShell, "PENShell", logicEnv, false, 0, checkOverlaps);

	G4LogicalVolume *logicInnerShell = ConstructCSGInnerShell();
	physInnerShell = new G4PVPlacement(rotPENShell1, PENShellPlacement, logicInnerShell, "PENShell", logicEnv, false, 0, checkOverlaps);

	//=============================================================//
	//                            BEGe                             //
	//=============================================================//

	G4LogicalVolume *logicTotalCrystal = ConstructBEGe();
	auto physDet = new G4PVPlacement(0, G4ThreeVector(0, 0, -1 * mm), logicTotalCrystal, "PhysDet", logicInnerShell, false, 0, checkOverlaps);

	//=============================================================//
	//                       Optical Surfaces                      //
	//=============================================================//

	G4OpticalSurface *scintWrap = new G4OpticalSurface("ScintWrap");
	scintWrap->SetModel(unified);
	scintWrap->SetType(dielectric_dielectric);
	scintWrap->SetFinish(polished);
	G4double energyReflector[] = {2.0 * eV, 3.0 * eV, 4.0 * eV, 5.0 * eV, 6.0 * eV, 7.0 * eV};
	const G4int num = sizeof(energyReflector) / sizeof(G4double);
	G4double reflectivity[] = {0.97, 0.97, 0.97, 0.97, 0.97, 0.97};
	assert(sizeof(reflectivity) == sizeof(energyReflector));
	G4MaterialPropertiesTable *scintWrapProperty = matVikuiti->GetMaterialPropertiesTable();
	scintWrapProperty->AddProperty("REFLECTIVITY", energyReflector, reflectivity, num);
	scintWrap->SetMaterialPropertiesTable(scintWrapProperty);

	G4OpticalSurface *PEN_LN2_Ref = new G4OpticalSurface("PEN_LN2_Ref");
	if (fReflectorType == "polishedvm2000air")
	{
		PEN_LN2_Ref->SetType(dielectric_LUT);
		PEN_LN2_Ref->SetModel(LUT);
		PEN_LN2_Ref->SetFinish(polishedvm2000air);
	}
	else if (fReflectorType == "polishedvm2000glue")
	{
		PEN_LN2_Ref->SetType(dielectric_LUT);
		PEN_LN2_Ref->SetModel(LUT);
		PEN_LN2_Ref->SetFinish(polishedvm2000glue);
	}
	else if (fReflectorType == "polishedtyvekair")
	{
		PEN_LN2_Ref->SetType(dielectric_LUT);
		PEN_LN2_Ref->SetModel(LUT);
		PEN_LN2_Ref->SetFinish(polishedtyvekair);
	}
	else if (fReflectorType == "polishedlumirrorair")
	{
		PEN_LN2_Ref->SetType(dielectric_LUT);
		PEN_LN2_Ref->SetModel(LUT);
		PEN_LN2_Ref->SetFinish(polishedlumirrorair);
	}
	else if (fReflectorType == "PolishedESR_LUT")
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(PolishedESR_LUT);
	}
	else if (fReflectorType == "PolishedESRGrease_LUT")
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(PolishedESRGrease_LUT);
	}
	else if (fReflectorType == "PolishedTeflon_LUT")
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(PolishedTeflon_LUT);
	}
	else if (fReflectorType == "RoughESR_LUT")
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(RoughESR_LUT);
	}
	else
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(Polished_LUT);
	}

	G4OpticalSurface *PEN_LN2_Ref1 = new G4OpticalSurface("PEN_LN2_Ref1");
	PEN_LN2_Ref1->SetType(dielectric_LUTDAVIS);
	PEN_LN2_Ref1->SetModel(DAVIS);
	PEN_LN2_Ref1->SetFinish(PolishedESR_LUT);
	G4OpticalSurface *PEN_LN2 = new G4OpticalSurface("PEN_LN2");
	PEN_LN2->SetType(dielectric_LUTDAVIS);
	PEN_LN2->SetModel(DAVIS);
	PEN_LN2->SetFinish(Polished_LUT);

	G4OpticalSurface *Ge_LN2 = new G4OpticalSurface("Ge_LN2");
	Ge_LN2->SetType(dielectric_metal);
	Ge_LN2->SetModel(glisur);
	Ge_LN2->SetFinish(polished);
	G4OpticalSurface *Ge_PEN = new G4OpticalSurface("Ge_PEN");
	Ge_PEN->SetType(dielectric_metal);
	Ge_PEN->SetModel(glisur);
	Ge_PEN->SetFinish(polished);
	G4OpticalSurface *Ge_Skin = new G4OpticalSurface("Ge_Skin");
	Ge_Skin->SetType(dielectric_metal);
	Ge_Skin->SetModel(glisur);
	Ge_Skin->SetFinish(polished);

	G4OpticalSurface *Wire_LN2 = new G4OpticalSurface("Wire_LN2");
	Wire_LN2->SetType(dielectric_LUTDAVIS);
	Wire_LN2->SetModel(DAVIS);
	Wire_LN2->SetFinish(Polished_LUT);

	// G4LogicalBorderSurface* Wire_LN2_LBS = new G4LogicalBorderSurface("Wire_LN2_LBS", physEnv, physWire, Wire_LN2);
	G4LogicalBorderSurface *OuterShell_LN2_LBS = new G4LogicalBorderSurface("OuterShell_LN2_LBS", physEnv, physOuterShell, PEN_LN2_Ref);
	// G4LogicalBorderSurface* LN2_OuterShell_LBS = new G4LogicalBorderSurface("OuterShell_LN2_LBS", physOuterShell, physEnv,  PEN_LN2_Ref);
	G4LogicalBorderSurface *InnerShell_LN2_LBS = new G4LogicalBorderSurface("InnerShell_LN2_LBS", physEnv, physInnerShell, PEN_LN2);
	G4LogicalBorderSurface *Ge_LN2_LBS = new G4LogicalBorderSurface("Ge_LN2_LBS", physEnv, physDet, Ge_LN2);
	G4LogicalBorderSurface *Ge_PEN_LBS = new G4LogicalBorderSurface("Ge_PEN_LBS", physInnerShell, physDet, Ge_PEN);
	G4LogicalSkinSurface *Ge_LSS = new G4LogicalSkinSurface("Ge_LSS", logicTotalCrystal, Ge_Skin);

	//=============================================================//
	//                          REflector                          //
	//=============================================================//

	if (ifOuterReflector == true)
	{
		G4LogicalVolume *logicReflector = ConstructOuterReflector();
		physOuterReflector = new G4PVPlacement(rotPENShell, PENShellPlacement, logicReflector, "Reflector", logicEnv, false, 0, checkOverlaps);

		// Reflector
		G4LogicalBorderSurface *PEN_OuterReflector_1_LBS = new G4LogicalBorderSurface("PEN_OuterReflector_LBS", physOuterReflector, physOuterShell, PEN_LN2_Ref);
		G4LogicalBorderSurface *PEN_OuterReflector_2_LBS = new G4LogicalBorderSurface("PEN_OuterReflector_LBS", physOuterShell, physOuterReflector, PEN_LN2_Ref);
	}

	if (ifInnerReflector == true)
	{
		G4LogicalVolume *logicReflector = ConstructInnerReflector();
		physInnerReflector = new G4PVPlacement(rotPENShell, PENShellPlacement, logicReflector, "Reflector", logicEnv, false, 0, checkOverlaps);

		// Reflector
		G4OpticalSurface *PEN_InnerReflector = new G4OpticalSurface("PEN_Reflector");
		G4LogicalBorderSurface *PEN_InnerReflector_LBS = new G4LogicalBorderSurface("PEN_InnerReflector_LBS", physInnerReflector, physPENShell, PEN_InnerReflector);
		PEN_InnerReflector = dynamic_cast<G4OpticalSurface *>(PEN_InnerReflector_LBS->GetSurface(physInnerReflector, physPENShell)->GetSurfaceProperty());
		PEN_InnerReflector->SetType(dielectric_LUTDAVIS);
		PEN_InnerReflector->SetModel(DAVIS);
		PEN_InnerReflector->SetFinish(PolishedESR_LUT);
		if (fReflectorType == "ESR")
		{
			PEN_InnerReflector->SetFinish(PolishedESR_LUT);
		}
		else if (fReflectorType == "ESRGrease")
		{
			PEN_InnerReflector->SetFinish(PolishedESRGrease_LUT);
		}
		else if (fReflectorType == "Teflon")
		{
			PEN_InnerReflector->SetFinish(PolishedTeflon_LUT);
		}
		else
		{
			G4cout << "Reflector type not found! Using default ESR LUT." << G4endl;
			PEN_InnerReflector->SetFinish(PolishedESR_LUT);
		}
		//
	}

	if (ifReflector == true)
	{
		G4LogicalVolume *logicReflector = ConstructReflector();
		physOuterReflector = new G4PVPlacement(rotPENShell, PENShellPlacement, logicReflector, "Reflector", logicEnv, false, 0, checkOverlaps);

		// Reflector
		G4OpticalSurface *PEN_OuterReflector = new G4OpticalSurface("PEN_Reflector");
		G4LogicalBorderSurface *PEN_OuterReflector_LBS = new G4LogicalBorderSurface("PEN_OuterReflector_LBS", physOuterReflector, physPENShell, PEN_OuterReflector);
		PEN_OuterReflector = dynamic_cast<G4OpticalSurface *>(PEN_OuterReflector_LBS->GetSurface(physOuterReflector, physPENShell)->GetSurfaceProperty());
		PEN_OuterReflector->SetType(dielectric_LUTDAVIS);
		PEN_OuterReflector->SetModel(DAVIS);
		if (fReflectorType == "ESR")
		{
			PEN_OuterReflector->SetFinish(PolishedESR_LUT);
		}
		else if (fReflectorType == "ESRGrease")
		{
			PEN_OuterReflector->SetFinish(PolishedESRGrease_LUT);
		}
		else if (fReflectorType == "Teflon")
		{
			PEN_OuterReflector->SetFinish(PolishedTeflon_LUT);
		}
		else
		{
			G4cout << "Reflector type not found! Using default ESR LUT." << G4endl;
			PEN_OuterReflector->SetFinish(PolishedESR_LUT);
		}
	}

	//=============================================================//
	//                            SiPMs                            //
	//=============================================================//

	G4AssemblyVolume *logicSiPMArray = ConstructSiPMArray();

	auto rotSiPM0 = new G4RotationMatrix();
	rotSiPM0->rotateY(90 * degree);
	G4ThreeVector PosSiPM0 = G4ThreeVector(-166 * mm, 0, 0 * mm);
	G4ThreeVector PosSiPM1 = G4ThreeVector(166 * mm, 0, 0 * mm);

	G4Transform3D TrPosSiPM0 = G4Transform3D(*rotSiPM0, PosSiPM0);
	G4Transform3D TrPosSiPM1 = G4Transform3D(*rotSiPM0, PosSiPM1);
	logicSiPMArray->MakeImprint(logicEnv, TrPosSiPM0);
	logicSiPMArray->MakeImprint(logicEnv, TrPosSiPM1);
	return physWorld;

	/*
	G4OpticalSurface* SiPM_LN2 = new G4OpticalSurface("SiPM_LN2");
	SiPM_LN2->SetType(dielectric_LUTDAVIS);
	SiPM_LN2->SetModel(DAVIS);
	SiPM_LN2->SetFinish(Detector_LUT);

	const G4int NUMENTRIES_CHIP = 11;
	const double hc = 6.62606876 * 2.99792458 * 100. / 1.602176462;
	G4double sipm_pp[NUMENTRIES_CHIP] = { hc / 600. * eV, hc / 590. * eV, hc / 580. * eV, hc / 570. * eV, hc / 560. * eV, hc / 550. * eV, hc / 540. * eV, hc / 530. * eV, hc / 520. * eV,hc / 510. * eV,hc / 500. * eV };
	G4double sipm_sl[NUMENTRIES_CHIP] = { 0,0,0,0,0,0,0,0,0,0,0 };
	G4double sipm_ss[NUMENTRIES_CHIP] = { 0,0,0,0,0,0,0,0,0,0,0 };
	G4double sipm_bs[NUMENTRIES_CHIP] = { 0,0,0,0,0,0,0,0,0,0,0 };
	G4double sipm_rindex[NUMENTRIES_CHIP] = { 1.406,1.406,1.406,1.406,1.406,1.406,1.406,1.406,1.406,1.406,1.406 };
	G4double sipm_reflectivity[NUMENTRIES_CHIP] = { 0,0,0,0,0,0,0,0,0,0,0 };
	// G4double sipm_efficiency[NUMENTRIES_CHIP] = {0.20,0.21,0.23,0.25,0.26,0.28,0.30,0.32,0.34,0.36,0.38};
	G4double sipm_efficiency[NUMENTRIES_CHIP] = { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 };

	G4MaterialPropertiesTable* SIPM_MPT_Surf = new G4MaterialPropertiesTable();
	// SIPM_MPT_Surf->AddProperty("SPECULARLOBECONSTANT",sipm_pp,sipm_sl,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("SPECULARSPIKECONSTANT",sipm_pp,sipm_ss,NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("BACKSCATTERCONSTANT",sipm_pp,sipm_bs,NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("REFLECTIVITY", sipm_pp, sipm_reflectivity, NUMENTRIES_CHIP);
	SIPM_MPT_Surf->AddProperty("EFFICIENCY", sipm_pp, sipm_efficiency, NUMENTRIES_CHIP);
	// SIPM_MPT_Surf->AddProperty("RINDEX",sipm_pp,sipm_rindex,NUMENTRIES_CHIP);

	SiPM_LN2->SetMaterialPropertiesTable(SIPM_MPT_Surf);

	G4LogicalBorderSurface* SiPM_LN2_LBS_0 = new G4LogicalBorderSurface("SiPM_LN2_LBS_0", physEnv, physSiPM0, SiPM_LN2);
	G4LogicalBorderSurface* SiPM_LN2_LBS_1 = new G4LogicalBorderSurface("SiPM_LN2_LBS_1", physEnv, physSiPM1, SiPM_LN2);
	G4LogicalBorderSurface* SiPM_LN2_LBS_2 = new G4LogicalBorderSurface("SiPM_LN2_LBS_2", physEnv, physSiPM2, SiPM_LN2);
	G4LogicalBorderSurface* SiPM_LN2_LBS_3 = new G4LogicalBorderSurface("SiPM_LN2_LBS_3", physEnv, physSiPM3, SiPM_LN2);
	G4LogicalBorderSurface* SiPM_LN2_LBS_4 = new G4LogicalBorderSurface("SiPM_LN2_LBS_4", physEnv, physSiPM4, SiPM_LN2);
	G4LogicalBorderSurface* SiPM_LN2_LBS_5 = new G4LogicalBorderSurface("SiPM_LN2_LBS_5", physEnv, physSiPM5, SiPM_LN2);
	// G4LogicalBorderSurface* SiPM_LN2_LBS_P = new G4LogicalBorderSurface("SiPM_LN2_LBS_P", physEnv, physSiPMP, SiPM_LN2_P);
	*/
}

G4VPhysicalVolume *CDEXDetectorConstruction::ConstructArray_1()
{

	G4NistManager *nist = G4NistManager::Instance();
	G4bool checkOverlaps = true;

	//=============================================================//
	//                        World&Envelope                       //
	//=============================================================//

	G4Material *world_mat = fVacuum;
	G4Material *env_mat = matLN2;
	G4Material *det_mat = matEnGe;

	G4double world_size = 300 * cm;
	G4double env_size = 250 * cm;
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "logicWorld");
	G4VPhysicalVolume *physWorld =
		new G4PVPlacement(0,			   // no rotation
						  G4ThreeVector(), // at (0,0,0)
						  logicWorld,	   // its logical volume
						  "World",		   // its name
						  0,			   // its mother  volume
						  false,		   // no boolean operation
						  0,			   // copy number
						  checkOverlaps);  // overlaps checking

	G4Box *solidEnv = new G4Box("solidEnvelope", 0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "logicEnvelope");
	physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

	//=============================================================//
	//                          StringBox                          //
	//=============================================================//
	auto rotStringBox = new G4RotationMatrix();
	rotStringBox->rotateX(90 * degree);
	auto logicStringBoxBrick = ConstructStringBoxBrick();
	physStringBoxBrick = new G4PVPlacement(rotStringBox, G4ThreeVector(), logicStringBoxBrick, "StringBox", logicEnv, false, 0, checkOverlaps);

	// auto logicStringBox = ConstructStringBox();
	////physStringBoxCrystal = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicStringBoxCrystal, "StringBoxCrystal", logicStringBoxBrick, false, 0, CheckOverlaps);
	// auto physStringBox = new G4PVPlacement(rotStringBox, G4ThreeVector(), logicStringBox, "StringBox", logicEnv, false, 0, checkOverlaps);

	//=============================================================//
	//                           PENShell                          //
	//=============================================================//

	// G4LogicalVolume* logicPENShell = ConstructPENShell();

	// G4double OSConeHeight0 = 37.313 * mm;
	// G4ThreeVector PENShellPlacement = G4ThreeVector(0, 0, 0);
	// auto rotInnerPENShell = new G4RotationMatrix();
	// rotInnerPENShell->rotateX(90 * degree);
	// auto rotOuterPENShell = new G4RotationMatrix();

	// G4ThreeVector PENShellPlacement =G4ThreeVector(0, 0, 34.1 * mm);

	// G4ThreeVector PENShellPlacement1 = G4ThreeVector(0, 0, OSConeHeight0 / 2);
	// G4LogicalVolume* logicPENShell = ConstructCSGPENShell();
	// physPENShell = new G4PVPlacement(rotPENShell, PENShellPlacement, logicPENShell, "PENShell", logicEnv, false, 0, checkOverlaps);
	G4LogicalVolume *logicOuterShell = ConstructOuterShell();
	G4LogicalVolume *logicInnerShell = ConstructCSGInnerShell();

	// G4String Ver = "IV";
	// auto solidVirtualUnitBoxmesh = CADMesh::TessellatedMesh::FromSTL("../models/VirtualUnitBox-" + Ver + ".stl");
	// G4VSolid* solidVirtualUnitBox = solidVirtualUnitBoxmesh->GetSolid();
	// G4LogicalVolume* logicVirtualUnitBox = new G4LogicalVolume(solidVirtualUnitBox, matLAr, "logicVirtualUnitBox");

	// physOuterShell = new G4PVPlacement(rotOuterPENShell, PENShellPlacement, logicOuterShell, "PENShell", logicVirtualUnitBox, false, 0, checkOverlaps);
	// physInnerShell = new G4PVPlacement(rotInnerPENShell, PENShellPlacement, logicInnerShell, "PENShell", logicVirtualUnitBox, false, 0, checkOverlaps);

	//=============================================================//
	//                             BEGe                            //
	//=============================================================//
	auto rotBEGe = new G4RotationMatrix();
	G4LogicalVolume *logicTotalCrystal = ConstructBEGe();
	// auto physDet = new G4PVPlacement(rotBEGe, G4ThreeVector(0, 0, -1 * mm), logicTotalCrystal, "PhysDet", logicInnerShell, false, 0, checkOverlaps);
	auto physDet = new G4PVPlacement(rotBEGe, G4ThreeVector(0, 0, -1 * mm), logicTotalCrystal, "PhysDet", logicInnerShell, false, 0, checkOverlaps);

	//=============================================================//
	//                            SiPMs                            //
	//=============================================================//

	// G4AssemblyVolume* logicSArSiPMArray = ConstructSArSiPMArray();
	G4LogicalVolume *logicSArSiPMArray = ConstructSArSiPMArrayLV();

	// auto rotSiPM0 = new G4RotationMatrix();
	// rotSiPM0->rotateX(90 * degree);

	// physSiPMArray0 = new G4PVPlacement(rotSiPM0, G4ThreeVector(-166 * mm, 0, 0 * mm), logicSiPMArray, "SiPMArray0", logicStringBoxCrystal, false, 0, checkOverlaps);
	// physSiPMArray1 = new G4PVPlacement(rotSiPM0, G4ThreeVector(166 * mm, 0, 0 * mm), logicSiPMArray, "SiPMArray1", logicStringBoxCrystal, false, 1, checkOverlaps);

	//=============================================================//
	//                             Unit                            //
	//=============================================================//
	G4AssemblyVolume *logicUnit = new G4AssemblyVolume();

	// Rotation and translation of a plate inside the assembly
	G4RotationMatrix *RotInnerShell = new G4RotationMatrix();
	G4ThreeVector PosInnerShell = G4ThreeVector(0, 0, 0);
	G4Transform3D TrInnerShell = G4Transform3D(*RotInnerShell, PosInnerShell);

	G4RotationMatrix *RotOuterShell = new G4RotationMatrix();
	RotOuterShell->rotateX(90 * degree);
	G4ThreeVector PosOuterShell = G4ThreeVector(0, 0, 0);
	G4Transform3D TrOuterShell = G4Transform3D(*RotOuterShell, PosOuterShell);

	// G4RotationMatrix* RotVirtualBox = new G4RotationMatrix();
	// RotVirtualBox->rotateX(-90 * degree);
	// G4ThreeVector PosVirtualBox = G4ThreeVector(0, 0, 0);
	// G4Transform3D TrVirtualBox = G4Transform3D(*RotVirtualBox, PosVirtualBox);

	G4RotationMatrix *RotPENShell = new G4RotationMatrix();
	RotPENShell->rotateX(90 * degree);
	G4ThreeVector PosPENShell = G4ThreeVector(0, 0, 0);
	G4Transform3D TrPENShell = G4Transform3D(*RotPENShell, PosPENShell);

	G4RotationMatrix *RotSArSiPM = new G4RotationMatrix();
	RotSArSiPM->rotateY(90 * degree);
	G4ThreeVector PosSArSiPM0 = G4ThreeVector(-166 * mm, 0, 0);
	G4ThreeVector PosSArSiPM1 = G4ThreeVector(166 * mm, 0, 0);
	G4ThreeVector PosSArSiPM2 = G4ThreeVector(179 * mm, 0, 37.5 * mm);
	G4ThreeVector PosSArSiPM3 = G4ThreeVector(-179 * mm, 0, 37.5 * mm);

	G4Transform3D TrPosSArSiPM0 = G4Transform3D(*RotSArSiPM, PosSArSiPM0);
	G4Transform3D TrPosSArSiPM1 = G4Transform3D(*RotSArSiPM, PosSArSiPM1);
	G4Transform3D TrPosSArSiPM2 = G4Transform3D(*RotSArSiPM, PosSArSiPM2);
	G4Transform3D TrPosSArSiPM3 = G4Transform3D(*RotSArSiPM, PosSArSiPM3);

	// logicUnit->AddPlacedVolume(logicVirtualUnitBox, TrVirtualBox);
	// logicUnit->AddPlacedAssembly(logicSArSiPMArray, TrPosSArSiPM0);
	// logicUnit->AddPlacedAssembly(logicSArSiPMArray, TrPosSArSiPM1);
	// logicUnit->AddPlacedAssembly(logicSArSiPMArray, TrPosSArSiPM2);
	// logicUnit->AddPlacedAssembly(logicSArSiPMArray, TrPosSArSiPM3);
	// logicUnit->AddPlacedVolume(logicPENShell, TrPENShell);
	logicUnit->AddPlacedVolume(logicOuterShell, TrOuterShell);
	logicUnit->AddPlacedVolume(logicInnerShell, TrInnerShell);
	logicUnit->AddPlacedVolume(logicSArSiPMArray, TrPosSArSiPM0);
	logicUnit->AddPlacedVolume(logicSArSiPMArray, TrPosSArSiPM1);
	logicUnit->AddPlacedVolume(logicSArSiPMArray, TrPosSArSiPM2);
	logicUnit->AddPlacedVolume(logicSArSiPMArray, TrPosSArSiPM3);

	G4RotationMatrix *RotUnit = new G4RotationMatrix();
	RotUnit->rotateX(90 * degree);
	// Rotation of the assembly inside the world
	for (G4int i = 0; i < 9; i++)
	{
		G4ThreeVector PosUnit1 = G4ThreeVector(0, (32.5 + 65 * i) * mm, 0);
		G4ThreeVector PosUnit2 = G4ThreeVector(0, (-32.5 - 65 * i) * mm, 0);
		G4Transform3D TrUnit1 = G4Transform3D(*RotUnit, PosUnit1);
		G4Transform3D TrUnit2 = G4Transform3D(*RotUnit, PosUnit2);
		logicUnit->MakeImprint(logicStringBoxCrystal, TrUnit1);
		logicUnit->MakeImprint(logicStringBoxCrystal, TrUnit2);
	}

	//=============================================================//
	//                            SArUnit                          //
	//=============================================================//
	G4LogicalVolume *logicContainerBrick = ConstructContainerBrick();

	G4RotationMatrix RotSArUnit;
	auto rotContainerBrick = new G4RotationMatrix();
	rotContainerBrick->rotateX(-90 * degree);
	G4ThreeVector ContainerPos0 = G4ThreeVector(0, (346.4 + 32) * mm, 0);
	G4ThreeVector ContainerPos1 = G4ThreeVector(0, -(346.4 + 32) * mm, 0);
	G4ThreeVector ContainerPos2 = G4ThreeVector((600 + 55.43) * mm, 0, 0);
	G4ThreeVector ContainerPos3 = G4ThreeVector(-(600 + 55.43) * mm, 0, 0);
	auto physContainerBrick0 = new G4PVPlacement(rotContainerBrick, ContainerPos0, logicContainerBrick, "SArContainerUnit", logicEnv, false, 0, checkOverlaps);
	auto physContainerBrick1 = new G4PVPlacement(rotContainerBrick, ContainerPos1, logicContainerBrick, "SArContainerUnit", logicEnv, false, 0, checkOverlaps);
	auto physContainerBrick2 = new G4PVPlacement(rotContainerBrick, ContainerPos2, logicContainerBrick, "SArContainerUnit", logicEnv, false, 0, checkOverlaps);
	auto physContainerBrick3 = new G4PVPlacement(rotContainerBrick, ContainerPos3, logicContainerBrick, "SArContainerUnit", logicEnv, false, 0, checkOverlaps);

	G4ThreeVector SArUnitSiPMPosUp0 = G4ThreeVector(0, 648 * mm, 0);
	G4ThreeVector SArUnitSiPMPosUp1 = G4ThreeVector(200 * mm, 648 * mm, 0);
	G4ThreeVector SArUnitSiPMPosUp2 = G4ThreeVector(-200 * mm, 648 * mm, 0);
	G4ThreeVector SArUnitSiPMPosUp3 = G4ThreeVector(100 * mm, 648 * mm, 173.2 * mm);
	G4ThreeVector SArUnitSiPMPosUp4 = G4ThreeVector(100 * mm, 648 * mm, -173.2 * mm);
	G4ThreeVector SArUnitSiPMPosUp5 = G4ThreeVector(-100 * mm, 648 * mm, 173.2 * mm);
	G4ThreeVector SArUnitSiPMPosUp6 = G4ThreeVector(-100 * mm, 648 * mm, -173.2 * mm);

	G4ThreeVector SArUnitSiPMPosDown0 = G4ThreeVector(0, -648 * mm, 0);
	G4ThreeVector SArUnitSiPMPosDown1 = G4ThreeVector(200 * mm, -648 * mm, 0);
	G4ThreeVector SArUnitSiPMPosDown2 = G4ThreeVector(-200 * mm, -648 * mm, 0);
	G4ThreeVector SArUnitSiPMPosDown3 = G4ThreeVector(100 * mm, -648 * mm, 173.2 * mm);
	G4ThreeVector SArUnitSiPMPosDown4 = G4ThreeVector(100 * mm, -648 * mm, -173.2 * mm);
	G4ThreeVector SArUnitSiPMPosDown5 = G4ThreeVector(-100 * mm, -648 * mm, 173.2 * mm);
	G4ThreeVector SArUnitSiPMPosDown6 = G4ThreeVector(-100 * mm, -648 * mm, -173.2 * mm);

	auto RotContainerSiPM = new G4RotationMatrix();
	RotContainerSiPM->rotateX(-90 * degree);

	G4LogicalVolume *logicContainerSiPMArray = ConstructContainerSiPMArrayLV();
	G4Transform3D TrSArUnitSiPMUp0 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosUp0);
	G4Transform3D TrSArUnitSiPMUp1 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosUp1);
	G4Transform3D TrSArUnitSiPMUp2 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosUp2);
	G4Transform3D TrSArUnitSiPMUp3 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosUp3);
	G4Transform3D TrSArUnitSiPMUp4 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosUp4);
	G4Transform3D TrSArUnitSiPMUp5 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosUp5);
	G4Transform3D TrSArUnitSiPMUp6 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosUp6);

	G4Transform3D TrSArUnitSiPMDown0 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosDown0);
	G4Transform3D TrSArUnitSiPMDown1 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosDown1);
	G4Transform3D TrSArUnitSiPMDown2 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosDown2);
	G4Transform3D TrSArUnitSiPMDown3 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosDown3);
	G4Transform3D TrSArUnitSiPMDown4 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosDown4);
	G4Transform3D TrSArUnitSiPMDown5 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosDown5);
	G4Transform3D TrSArUnitSiPMDown6 = G4Transform3D(*RotContainerSiPM, SArUnitSiPMPosDown6);

	auto physContainerSiPMArrayUp0 = new G4PVPlacement(TrSArUnitSiPMUp0, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayUp1 = new G4PVPlacement(TrSArUnitSiPMUp1, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayUp2 = new G4PVPlacement(TrSArUnitSiPMUp2, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayUp3 = new G4PVPlacement(TrSArUnitSiPMUp3, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayUp4 = new G4PVPlacement(TrSArUnitSiPMUp4, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayUp5 = new G4PVPlacement(TrSArUnitSiPMUp5, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayUp6 = new G4PVPlacement(TrSArUnitSiPMUp6, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);

	auto physContainerSiPMArrayDown0 = new G4PVPlacement(TrSArUnitSiPMDown0, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayDown1 = new G4PVPlacement(TrSArUnitSiPMDown1, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayDown2 = new G4PVPlacement(TrSArUnitSiPMDown2, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayDown3 = new G4PVPlacement(TrSArUnitSiPMDown3, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayDown4 = new G4PVPlacement(TrSArUnitSiPMDown4, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayDown5 = new G4PVPlacement(TrSArUnitSiPMDown5, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);
	auto physContainerSiPMArrayDown6 = new G4PVPlacement(TrSArUnitSiPMDown6, logicContainerSiPMArray, "ContainerSiPMArray", logicContainerCrystal, false, 0, checkOverlaps);

	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMUp0);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMUp1);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMUp2);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMUp3);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMUp4);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMUp5);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMUp6);

	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMDown0);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMDown1);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMDown2);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMDown3);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMDown4);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMDown5);
	// logicContainerSiPMArray->MakeImprint(logicContainerCrystal, TrSArUnitSiPMDown6);

	//========================PEN shell and wire paremeters========================//

	G4String WireType = fWireType;

	G4double LN2Gap = 0.5 * mm;
	G4double ShellThickness = 4.9 * mm;
	G4double WireLength = 40 * mm;
	G4double outerGeRadius = 40 * mm;

	//=============================================================================//

	//=============================================================//
	//                             Wire                            //
	//=============================================================//

	fWireLength = WireLength;
	G4double WireCentDist;
	G4ThreeVector WirePlacement;

	if (WireType == "A1")
	{
		G4double WireRadius = 1.25 * mm / 2;
		fWireRadius = WireRadius;
		logicWire = ConstructA1(WireLength);
		fWireCentDist = outerGeRadius + ShellThickness - WireRadius;
		WirePlacement = G4ThreeVector(0, fWireCentDist, 0);
		fWirePos = WirePlacement;
		auto physWire = new G4PVPlacement(0, WirePlacement, logicWire, "Wire", logicInnerShell, false, 0, checkOverlaps);
		// physWire = new G4PVPlacement(0, WirePlacement, logicWire, "Wire", logicInnerShell, false, 0, checkOverlaps);
	}
	else if (WireType == "A2")
	{
		G4double WireRadius = 1 * mm / 2;
		fWireRadius = WireRadius;
		logicWire = ConstructA2(WireLength);
		fWireCentDist = outerGeRadius + LN2Gap + ShellThickness - WireRadius;
		WirePlacement = G4ThreeVector(0, fWireCentDist, 0);
		fWirePos = WirePlacement;
		auto physWire = new G4PVPlacement(0, WirePlacement, logicWire, "Wire", logicInnerShell, false, 0, checkOverlaps);
	}
	else
	{
		G4cout << "Type does not exist!" << G4endl;
	}

	//=============================================================//
	//                         ASIC Plate                          //
	//=============================================================//

	G4ThreeVector ASICPlacement = G4ThreeVector(0, 0, -fBEGeHeight / 2 - ShellThickness + fASICThickness / 2);

	logicASICPlate = ConstructASICPlate();
	// physASICPlate = new G4PVPlacement(0, ASICPlacement, logicASICPlate, "ASIC", logicInnerShell, false, 0, checkOverlaps);
	physASICPlate = new G4PVPlacement(0, ASICPlacement, logicASICPlate, "ASIC", logicInnerShell, false, 0, checkOverlaps);
	//=============================================================//
	//                        Optical Surfaces                     //
	//=============================================================//

	G4OpticalSurface *Ge_SAr = new G4OpticalSurface("Ge_SAr");
	Ge_SAr->SetType(dielectric_metal);
	Ge_SAr->SetModel(glisur);
	Ge_SAr->SetFinish(polished);

	G4OpticalSurface *scintWrap = new G4OpticalSurface("ScintWrap");
	scintWrap->SetModel(unified);
	scintWrap->SetType(dielectric_dielectric);
	scintWrap->SetFinish(polished);
	G4double energyReflector[] = {2.0 * eV, 3.0 * eV, 4.0 * eV, 5.0 * eV, 6.0 * eV, 7.0 * eV};
	const G4int num = sizeof(energyReflector) / sizeof(G4double);
	G4double reflectivity[] = {0.97, 0.97, 0.97, 0.97, 0.97, 0.97};
	assert(sizeof(reflectivity) == sizeof(energyReflector));
	G4MaterialPropertiesTable *scintWrapProperty = matVikuiti->GetMaterialPropertiesTable();
	scintWrapProperty->AddProperty("REFLECTIVITY", energyReflector, reflectivity, num);
	scintWrap->SetMaterialPropertiesTable(scintWrapProperty);

	G4OpticalSurface *PEN_SAr_Ref = new G4OpticalSurface("PEN_SAr_Ref");
	if (fReflectorType == "polishedvm2000air")
	{
		PEN_SAr_Ref->SetType(dielectric_LUT);
		PEN_SAr_Ref->SetModel(LUT);
		PEN_SAr_Ref->SetFinish(polishedvm2000air);
	}
	else if (fReflectorType == "polishedvm2000glue")
	{
		PEN_SAr_Ref->SetType(dielectric_LUT);
		PEN_SAr_Ref->SetModel(LUT);
		PEN_SAr_Ref->SetFinish(polishedvm2000glue);
	}
	else if (fReflectorType == "polishedtyvekair")
	{
		PEN_SAr_Ref->SetType(dielectric_LUT);
		PEN_SAr_Ref->SetModel(LUT);
		PEN_SAr_Ref->SetFinish(polishedtyvekair);
	}
	else if (fReflectorType == "polishedlumirrorair")
	{
		PEN_SAr_Ref->SetType(dielectric_LUT);
		PEN_SAr_Ref->SetModel(LUT);
		PEN_SAr_Ref->SetFinish(polishedlumirrorair);
	}
	else if (fReflectorType == "PolishedESR_LUT")
	{
		PEN_SAr_Ref->SetType(dielectric_LUTDAVIS);
		PEN_SAr_Ref->SetModel(DAVIS);
		PEN_SAr_Ref->SetFinish(PolishedESR_LUT);
	}
	else if (fReflectorType == "PolishedESRGrease_LUT")
	{
		PEN_SAr_Ref->SetType(dielectric_LUTDAVIS);
		PEN_SAr_Ref->SetModel(DAVIS);
		PEN_SAr_Ref->SetFinish(PolishedESRGrease_LUT);
	}
	else if (fReflectorType == "PolishedTeflon_LUT")
	{
		PEN_SAr_Ref->SetType(dielectric_LUTDAVIS);
		PEN_SAr_Ref->SetModel(DAVIS);
		PEN_SAr_Ref->SetFinish(PolishedTeflon_LUT);
	}
	else if (fReflectorType == "RoughESR_LUT")
	{
		PEN_SAr_Ref->SetType(dielectric_LUTDAVIS);
		PEN_SAr_Ref->SetModel(DAVIS);
		PEN_SAr_Ref->SetFinish(RoughESR_LUT);
	}
	else
	{
		PEN_SAr_Ref->SetType(dielectric_LUTDAVIS);
		PEN_SAr_Ref->SetModel(DAVIS);
		PEN_SAr_Ref->SetFinish(Polished_LUT);
	}

	G4OpticalSurface *PEN_LN2_Ref = new G4OpticalSurface("PEN_LN2_Ref");
	if (fReflectorType == "polishedvm2000air")
	{
		PEN_LN2_Ref->SetType(dielectric_LUT);
		PEN_LN2_Ref->SetModel(LUT);
		PEN_LN2_Ref->SetFinish(polishedvm2000air);
	}
	else if (fReflectorType == "polishedvm2000glue")
	{
		PEN_LN2_Ref->SetType(dielectric_LUT);
		PEN_LN2_Ref->SetModel(LUT);
		PEN_LN2_Ref->SetFinish(polishedvm2000glue);
	}
	else if (fReflectorType == "polishedtyvekair")
	{
		PEN_LN2_Ref->SetType(dielectric_LUT);
		PEN_LN2_Ref->SetModel(LUT);
		PEN_LN2_Ref->SetFinish(polishedtyvekair);
	}
	else if (fReflectorType == "polishedlumirrorair")
	{
		PEN_LN2_Ref->SetType(dielectric_LUT);
		PEN_LN2_Ref->SetModel(LUT);
		PEN_LN2_Ref->SetFinish(polishedlumirrorair);
	}
	else if (fReflectorType == "PolishedESR_LUT")
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(PolishedESR_LUT);
	}
	else if (fReflectorType == "PolishedESRGrease_LUT")
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(PolishedESRGrease_LUT);
	}
	else if (fReflectorType == "PolishedTeflon_LUT")
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(PolishedTeflon_LUT);
	}
	else if (fReflectorType == "RoughESR_LUT")
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(RoughESR_LUT);
	}
	else
	{
		PEN_LN2_Ref->SetType(dielectric_LUTDAVIS);
		PEN_LN2_Ref->SetModel(DAVIS);
		PEN_LN2_Ref->SetFinish(Polished_LUT);
	}

	G4OpticalSurface *SAr_SAr = new G4OpticalSurface("SAr_SAr");
	SAr_SAr->SetType(dielectric_dielectric);
	SAr_SAr->SetModel(unified);
	SAr_SAr->SetFinish(polished);

	G4OpticalSurface *PEN_LN2_Ref1 = new G4OpticalSurface("PEN_LN2_Ref1");
	PEN_LN2_Ref1->SetType(dielectric_LUTDAVIS);
	PEN_LN2_Ref1->SetModel(DAVIS);
	PEN_LN2_Ref1->SetFinish(PolishedESR_LUT);

	G4OpticalSurface *PEN_LN2 = new G4OpticalSurface("PEN_LN2");
	PEN_LN2->SetType(dielectric_LUTDAVIS);
	PEN_LN2->SetModel(LUT);
	PEN_LN2->SetFinish(Polished_LUT);

	G4OpticalSurface *PEN_SAr_Ref1 = new G4OpticalSurface("PEN_SAr_Ref1");
	PEN_SAr_Ref1->SetType(dielectric_LUTDAVIS);
	PEN_SAr_Ref1->SetModel(DAVIS);
	PEN_SAr_Ref1->SetFinish(PolishedESR_LUT);

	G4OpticalSurface *PEN_SAr = new G4OpticalSurface("PEN_SAr");
	PEN_SAr->SetType(dielectric_LUTDAVIS);
	PEN_SAr->SetModel(LUT);
	PEN_SAr->SetFinish(Polished_LUT);

	G4OpticalSurface *Wire_SAr = new G4OpticalSurface("Wire_SAr");
	Wire_SAr->SetType(dielectric_dielectric);
	Wire_SAr->SetModel(DAVIS);
	Wire_SAr->SetFinish(Polished_LUT);

	G4OpticalSurface *Ge_LN2 = new G4OpticalSurface("Ge_LN2");
	Ge_LN2->SetType(dielectric_metal);
	Ge_LN2->SetModel(glisur);
	Ge_LN2->SetFinish(polished);
	G4OpticalSurface *Ge_PEN = new G4OpticalSurface("Ge_PEN");
	Ge_PEN->SetType(dielectric_metal);
	Ge_PEN->SetModel(glisur);
	Ge_PEN->SetFinish(polished);
	G4OpticalSurface *Ge_Skin = new G4OpticalSurface("Ge_Skin");
	Ge_Skin->SetType(dielectric_metal);
	Ge_Skin->SetModel(glisur);
	Ge_Skin->SetFinish(polished);

	G4OpticalSurface *Wire_Skin = new G4OpticalSurface("Wire_Skin");
	Wire_Skin->SetType(dielectric_dielectric);
	Wire_Skin->SetModel(unified);
	Wire_Skin->SetFinish(polished);

	G4OpticalSurface *ASIC_Skin = new G4OpticalSurface("ASIC_Skin");
	ASIC_Skin->SetType(dielectric_dielectric);
	ASIC_Skin->SetModel(unified);
	ASIC_Skin->SetFinish(polished);

	// G4LogicalSkinSurface* VirtualBox_LSS = new G4LogicalSkinSurface("VirtualBox_LSS", logicVirtualUnitBox, SAr_SAr);
	G4LogicalSkinSurface *Box_LN2_LSS = new G4LogicalSkinSurface("Box_LN2_LSS", logicStringBoxBrick, PEN_LN2_Ref);
	G4LogicalSkinSurface *Ge_LSS = new G4LogicalSkinSurface("Ge_LSS", logicTotalCrystal, Ge_Skin);
	G4LogicalSkinSurface *ASIC_LSS = new G4LogicalSkinSurface("ASIC_LSS", logicASICPlate, ASIC_Skin);
	G4LogicalSkinSurface *Wire_LSS = new G4LogicalSkinSurface("Wire_LSS", logicWire, Wire_Skin);

	// G4LogicalBorderSurface* Ge_PEN_LBS = new G4LogicalBorderSurface("Ge_PEN_LBS", physInnerShell, physDet, Ge_PEN);
	// G4LogicalBorderSurface* Ge_SAr_LBS = new G4LogicalBorderSurface("Ge_SAr_LBS", physStringBoxCrystal, physDet, Ge_SAr);

	G4LogicalBorderSurface *Box_SAr_LBS = new G4LogicalBorderSurface("Box_SAr_LBS", physStringBoxCrystal, physStringBoxBrick, PEN_SAr_Ref);
	// for (int i = 1; i <= 18; i++) {
	//	G4String PVName = "av_1_impr_" + std::to_string(i) + "_logicVirtualUnitBox_pv_0";
	//	G4String InnerShellLBSName = "InnerShell_SAr_LBS";
	//	new G4LogicalBorderSurface("InnerShell_SAr_LBS", GetPhysicalVolumeByName(PVName), physInnerShell, PEN_SAr_Ref);
	//	new G4LogicalBorderSurface("OuterShell_SAr_LBS", GetPhysicalVolumeByName(PVName), physOuterShell, PEN_SAr);
	// }
	for (int i = 1; i <= 18; i++)
	{
		G4String PVName0 = "av_1_impr_" + std::to_string(i) + "_logicOuterShell_pv_0";
		G4String PVName1 = "av_1_impr_" + std::to_string(i) + "_logicInnerShell_pv_1";
		G4String InnerShellLBSName = "InnerShell_SAr_LBS";
		new G4LogicalBorderSurface("OuterShell_SAr_LBS", physStringBoxCrystal, GetPhysicalVolumeByName(PVName0), PEN_SAr_Ref);
		new G4LogicalBorderSurface("InnerShell_SAr_LBS", physStringBoxCrystal, GetPhysicalVolumeByName(PVName1), PEN_SAr);
	}
	G4LogicalBorderSurface *InnerShell_SAr_LBS = new G4LogicalBorderSurface("InnerShell_SAr_LBS", physStringBoxCrystal, physInnerShell, PEN_SAr_Ref);
	G4LogicalBorderSurface *OuterShell_SAr_LBS = new G4LogicalBorderSurface("OuterShell_SAr_LBS", physStringBoxCrystal, physOuterShell, PEN_SAr_Ref);

	GetPhysicalVolumeProperties();
	return physWorld;
}

G4VPhysicalVolume *CDEXDetectorConstruction::ConstructSArUnit()
{

	G4NistManager *nist = G4NistManager::Instance();
	G4bool checkOverlaps = true;

	G4Material *world_mat = fVacuum;
	G4Material *env_mat = matLN2;
	G4Material *det_mat = matEnGe;

	// World&Envelope
	G4double world_size = 450 * cm;
	G4double env_size = 400 * cm;
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "logicWorld");
	G4VPhysicalVolume *physWorld =
		new G4PVPlacement(0,			   // no rotation
						  G4ThreeVector(), // at (0,0,0)
						  logicWorld,	   // its logical volume
						  "World",		   // its name
						  0,			   // its mother  volume
						  false,		   // no boolean operation
						  0,			   // copy number
						  checkOverlaps);  // overlaps checking

	G4Box *solidEnv = new G4Box("solidEnvelope", 0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "logicEnvelope");
	physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

	G4LogicalVolume *logicSArBrick = ConstructContainerBrick();
	auto rotSArBrick = new G4RotationMatrix();
	rotSArBrick->rotateX(-90 * degree);
	physContainerBrick = new G4PVPlacement(rotSArBrick, G4ThreeVector(), logicSArBrick, "SArBrick", logicEnv, false, 0, checkOverlaps);

	G4OpticalSurface *Container_LN2 = new G4OpticalSurface("Container_LN2");
	G4LogicalBorderSurface *Container_LN2_LBS = new G4LogicalBorderSurface("Container_LN2_LBS", physContainerBrick, physEnv, Container_LN2);
	Container_LN2 = dynamic_cast<G4OpticalSurface *>(Container_LN2_LBS->GetSurface(physContainerBrick, physEnv)->GetSurfaceProperty());
	Container_LN2->SetType(dielectric_LUTDAVIS);
	Container_LN2->SetModel(DAVIS);
	Container_LN2->SetFinish(Polished_LUT);

	G4OpticalSurface *Container_SAr = new G4OpticalSurface("Container_SAr");
	G4LogicalBorderSurface *Container_SAr_LBS = new G4LogicalBorderSurface("Container_SAr_LBS", physContainerCrystal, physContainerBrick, Container_SAr);
	Container_SAr = dynamic_cast<G4OpticalSurface *>(Container_SAr_LBS->GetSurface(physContainerBrick, physEnv)->GetSurfaceProperty());
	Container_SAr->SetType(dielectric_LUTDAVIS);
	Container_SAr->SetModel(DAVIS);
	Container_SAr->SetFinish(Polished_LUT);

	return physWorld;
}

G4VPhysicalVolume *CDEXDetectorConstruction::ConstructBucketSiPMSystem()
{

	G4NistManager *nist = G4NistManager::Instance();
	G4bool checkOverlaps = true;

	G4double SmallestUnitHeight = fSmallestUnitHeight;
	//=============================================================//
	//                        World&Envelope                       //
	//=============================================================//

	G4Material *world_mat = fVacuum;
	G4Material *env_mat = matLN2;
	G4Material *det_mat = matEnGe;

	G4double world_size = 300 * cm;
	G4double env_size = 250 * cm;
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "logicWorld");
	G4VPhysicalVolume *physWorld =
		new G4PVPlacement(0,			   // no rotation
						  G4ThreeVector(), // at (0,0,0)
						  logicWorld,	   // its logical volume
						  "World",		   // its name
						  0,			   // its mother  volume
						  false,		   // no boolean operation
						  0,			   // copy number
						  checkOverlaps);  // overlaps checking

	G4Box *solidEnv = new G4Box("solidEnvelope", 0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "logicEnvelope");
	physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

	//=============================================================//
	//                            Bucket                           //
	//=============================================================//
	auto rotBucket = new G4RotationMatrix();
	auto logicBucket = ConstructSiPMBucket();
	auto physBucket = new G4PVPlacement(rotBucket, G4ThreeVector(), logicBucket, "Bucket", logicEnv, false, 0, checkOverlaps);

	//=============================================================//
	//                            Shroud                           //
	//=============================================================//

	auto logicShroud = ConstructShroud();
	auto physShroud = new G4PVPlacement(0, G4ThreeVector(), logicShroud, "Shroud", logicArVolume, false, 0, checkOverlaps);

	//=============================================================//
	//                             BEGe                            //
	//=============================================================//
	auto rotBEGe = new G4RotationMatrix();
	// auto logicBEGe = ConstructBEGe();
	logicBEGe = ConstructBEGe();

	//=============================================================//
	//                             Wire                            //
	//=============================================================//
	G4double WireLength = SmallestUnitHeight;
	fWireLength = WireLength;
	if (fWireType == "A1")
	{
		logicWire = ConstructA1(WireLength);
	}
	else if (fWireType == "A2")
	{
		logicWire = ConstructA2(WireLength);
	}
	else
	{
		G4cout << "Type does not exist!" << G4endl;
	}

	//=============================================================//
	//                             ASIC                            //
	//=============================================================//
	logicASICPlate = ConstructASICPlate();

	//=============================================================//
	//                            SiPMs                            //
	//=============================================================//

	G4LogicalVolume *logicSiPM = ConstructSiPM();
	// G4LogicalVolume * logicSiPM = ConstructSiPMArrayLV();
	G4ThreeVector posSiPMUp;
	G4ThreeVector posSiPMDown;
	G4PVPlacement *physCoatedSiPM[60] = {nullptr};
	for (G4int i = 0; i < 10; i++)
	{
		for (G4int j = 0; j < 6; j++)
		{
			auto rotSiPM = new G4RotationMatrix();
			rotSiPM->rotateZ(60 * j * degree);
			rotSiPM->rotateY(90 * degree);
			G4double SiPMCentDist = fBucketRadius - fBucketThickness - 1 * cm;
			G4double SiPMAng = 60 * j * degree;
			posSiPMUp = G4ThreeVector(SiPMCentDist * cos(-SiPMAng), SiPMCentDist * sin(-SiPMAng), SmallestUnitHeight / 2 + i * SmallestUnitHeight);
			posSiPMDown = G4ThreeVector(SiPMCentDist * cos(-SiPMAng), SiPMCentDist * sin(-SiPMAng), -SmallestUnitHeight / 2 - i * SmallestUnitHeight);
			G4int SiPMID1 = 12 * i + 2 * j;
			G4int SiPMID2 = 12 * i + 2 * j + 1;
			physCoatedSiPM[SiPMID1] = new G4PVPlacement(rotSiPM, posSiPMUp, logicSiPM, "SiPM", logicArVolume, false, SiPMID1, checkOverlaps);
			physCoatedSiPM[SiPMID2] = new G4PVPlacement(rotSiPM, posSiPMDown, logicSiPM, "SiPM", logicArVolume, false, SiPMID2, checkOverlaps);
		}
	}

	//=============================================================//
	//                           String                            //
	//=============================================================//

	// Rotation and translation of a plate inside the assembly
	G4RotationMatrix *RotBEGe = new G4RotationMatrix();
	G4ThreeVector PosBEGe = G4ThreeVector(0, 0, 0);
	G4Transform3D TrBEGe = G4Transform3D(*RotBEGe, PosBEGe);

	G4RotationMatrix *RotASIC = new G4RotationMatrix();
	G4ThreeVector PosASIC = G4ThreeVector(0, 0, -fBEGeHeight / 2 - fASICThickness / 2 - 1 * mm);
	G4Transform3D TrASIC = G4Transform3D(*RotASIC, PosASIC);

	G4RotationMatrix *RotWire = new G4RotationMatrix();
	fWirePos = G4ThreeVector(fWireRadius + fBEGeRadius, 0, 0);
	G4ThreeVector PosWire = fWirePos;
	G4Transform3D TrWire = G4Transform3D(*RotWire, PosWire);
	G4PVPlacement *physBEGe[20] = {nullptr};
	G4PVPlacement *physWire[20] = {nullptr};
	G4PVPlacement *physASIC[20] = {nullptr};
	for (G4int i = 0; i < fUnitNb / 2; i++)
	{
		G4ThreeVector PosUnit1 = G4ThreeVector(0, 0, (SmallestUnitHeight / 2 + SmallestUnitHeight * i));
		G4ThreeVector PosUnit2 = G4ThreeVector(0, 0, (-SmallestUnitHeight / 2 - SmallestUnitHeight * i));
		physBEGe[2 * i] = new G4PVPlacement(RotBEGe, PosUnit1 + PosBEGe, logicBEGe, "BEGe", logicShourdVoid, false, 0, checkOverlaps);
		physBEGe[2 * i + 1] = new G4PVPlacement(RotBEGe, PosUnit2 + PosBEGe, logicBEGe, "BEGe", logicShourdVoid, false, 0, checkOverlaps);
		physWire[2 * i] = new G4PVPlacement(RotWire, PosUnit1 + PosWire, logicWire, "Wire", logicShourdVoid, false, 0, checkOverlaps);
		physWire[2 * i] = new G4PVPlacement(RotWire, PosUnit2 + PosWire, logicWire, "Wire", logicShourdVoid, false, 0, checkOverlaps);
		physASIC[2 * i] = new G4PVPlacement(RotASIC, PosUnit1 + PosASIC, logicASICPlate, "ASIC", logicShourdVoid, false, 0, checkOverlaps);
		physASIC[2 * i] = new G4PVPlacement(RotASIC, PosUnit2 + PosASIC, logicASICPlate, "ASIC", logicShourdVoid, false, 0, checkOverlaps);
	}

	// G4AssemblyVolume* logicString = new G4AssemblyVolume();
	// logicString->AddPlacedVolume(logicBEGe, TrBEGe);
	// logicString->AddPlacedVolume(logicASICPlate, TrASIC);
	// logicString->AddPlacedVolume(logicWire, TrWire);

	// G4RotationMatrix* RotUnit = new G4RotationMatrix();
	// RotUnit->rotateX(0);
	//// Rotation of the assembly inside the world
	// for (G4int i = 0; i < fUnitNb/2; i++) {
	//	G4ThreeVector PosUnit1 = G4ThreeVector(0, 0, (SmallestUnitHeight / 2 + SmallestUnitHeight * i));
	//	G4ThreeVector PosUnit2 = G4ThreeVector(0, 0, (-SmallestUnitHeight / 2 - SmallestUnitHeight * i));
	//	G4Transform3D TrUnit1 = G4Transform3D(*RotUnit, PosUnit1);
	//	G4Transform3D TrUnit2 = G4Transform3D(*RotUnit, PosUnit2);
	//	logicString->MakeImprint(logicShourdVoid, TrUnit1);
	//	logicString->MakeImprint(logicShourdVoid, TrUnit2);
	// }

	//=============================================================//
	//                        Optical Surfaces                     //
	//=============================================================//

	G4OpticalSurface *Ge_SAr = new G4OpticalSurface("Ge_SAr");
	Ge_SAr->SetType(dielectric_metal);
	Ge_SAr->SetModel(glisur);
	Ge_SAr->SetFinish(polished);

	G4OpticalSurface *scintWrap = new G4OpticalSurface("ScintWrap");
	scintWrap->SetModel(unified);
	scintWrap->SetType(dielectric_dielectric);
	scintWrap->SetFinish(polished);
	G4double energyReflector[] = {2.0 * eV, 3.0 * eV, 4.0 * eV, 5.0 * eV, 6.0 * eV, 7.0 * eV};
	const G4int num = sizeof(energyReflector) / sizeof(G4double);
	G4double reflectivity[] = {0.97, 0.97, 0.97, 0.97, 0.97, 0.97};
	assert(sizeof(reflectivity) == sizeof(energyReflector));
	G4MaterialPropertiesTable *scintWrapProperty = matVikuiti->GetMaterialPropertiesTable();
	scintWrapProperty->AddProperty("REFLECTIVITY", energyReflector, reflectivity, num);
	scintWrap->SetMaterialPropertiesTable(scintWrapProperty);

	G4OpticalSurface *PMMA_Dielctric_Ref = new G4OpticalSurface("PMMA_Dielctric_Ref");
	if (fReflectorType == "polishedvm2000air")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUT);
		PMMA_Dielctric_Ref->SetModel(LUT);
		PMMA_Dielctric_Ref->SetFinish(polishedvm2000air);
	}
	else if (fReflectorType == "polishedvm2000glue")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUT);
		PMMA_Dielctric_Ref->SetModel(LUT);
		PMMA_Dielctric_Ref->SetFinish(polishedvm2000glue);
	}
	else if (fReflectorType == "polishedtyvekair")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUT);
		PMMA_Dielctric_Ref->SetModel(LUT);
		PMMA_Dielctric_Ref->SetFinish(polishedtyvekair);
	}
	else if (fReflectorType == "polishedlumirrorair")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUT);
		PMMA_Dielctric_Ref->SetModel(LUT);
		PMMA_Dielctric_Ref->SetFinish(polishedlumirrorair);
	}
	else if (fReflectorType == "PolishedESR_LUT")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(PolishedESR_LUT);
	}
	else if (fReflectorType == "PolishedESRGrease_LUT")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(PolishedESRGrease_LUT);
	}
	else if (fReflectorType == "PolishedTeflon_LUT")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(PolishedTeflon_LUT);
	}
	else if (fReflectorType == "RoughESR_LUT")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(RoughESR_LUT);
	}
	else
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(Polished_LUT);
	}

	G4OpticalSurface *SAr_SAr = new G4OpticalSurface("SAr_SAr");
	SAr_SAr->SetType(dielectric_dielectric);
	SAr_SAr->SetModel(unified);
	SAr_SAr->SetFinish(polished);

	G4OpticalSurface *PMMA_Dielectric = new G4OpticalSurface("PMMA_Dielectric");
	PMMA_Dielectric->SetType(dielectric_LUTDAVIS);
	PMMA_Dielectric->SetModel(LUT);
	PMMA_Dielectric->SetFinish(Polished_LUT);

	G4OpticalSurface *Ge_Dielectric = new G4OpticalSurface("Ge_Dielectric");
	Ge_Dielectric->SetType(dielectric_metal);
	Ge_Dielectric->SetModel(glisur);
	Ge_Dielectric->SetFinish(polished);

	G4OpticalSurface *Wire_Dielectric = new G4OpticalSurface("Wire_Dielectric");
	Wire_Dielectric->SetType(dielectric_dielectric);
	Wire_Dielectric->SetModel(unified);
	Wire_Dielectric->SetFinish(polished);

	G4OpticalSurface *ASIC_Dielectric = new G4OpticalSurface("ASIC_Dielectric");
	ASIC_Dielectric->SetType(dielectric_dielectric);
	ASIC_Dielectric->SetModel(unified);
	ASIC_Dielectric->SetFinish(polished);

	G4LogicalSkinSurface *ASIC_LSS = new G4LogicalSkinSurface("ASIC_LSS", logicASICPlate, ASIC_Dielectric);
	G4LogicalSkinSurface *Wire_LSS = new G4LogicalSkinSurface("Wire_LSS", logicWire, Wire_Dielectric);
	G4LogicalSkinSurface *Bucket_LSS = new G4LogicalSkinSurface("Bucket_LSS", logicBucket, PMMA_Dielectric);
	G4LogicalSkinSurface *Shroud_LSS = new G4LogicalSkinSurface("Shroud_LSS", logicShroud, PMMA_Dielectric);
	G4LogicalSkinSurface *BEGe_LSS = new G4LogicalSkinSurface("BEGe_LSS", logicBEGe, Ge_Dielectric);

	GetPhysicalVolumeProperties();
	return physWorld;
}

G4VPhysicalVolume *CDEXDetectorConstruction::ConstructBucketFiberSystem()
{
	G4NistManager *nist = G4NistManager::Instance();
	G4bool checkOverlaps = true;
	G4double SmallestUnitHeight = fSmallestUnitHeight;

	//=============================================================//
	//                        World&Envelope                       //
	//=============================================================//

	G4Material *world_mat = fVacuum;
	G4Material *env_mat = matLN2;
	G4Material *det_mat = matEnGe;

	G4double world_size = 600 * cm;
	G4double env_size = 500 * cm;
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "logicWorld");
	G4VPhysicalVolume *physWorld =
		new G4PVPlacement(0,			   // no rotation
						  G4ThreeVector(), // at (0,0,0)
						  logicWorld,	   // its logical volume
						  "World",		   // its name
						  0,			   // its mother  volume
						  false,		   // no boolean operation
						  0,			   // copy number
						  checkOverlaps);  // overlaps checking

	G4Box *solidEnv = new G4Box("solidEnvelope", 0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "logicEnvelope");
	physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

	//=============================================================//
	//                            Bucket                           //
	//=============================================================//
	auto rotBucket = new G4RotationMatrix();
	auto logicBucket = ConstructFiberBucket();
	auto physBucket = new G4PVPlacement(rotBucket, G4ThreeVector(), logicBucket, "Bucket", logicEnv, false, 0, checkOverlaps);

	// auto rotContainer = new G4RotationMatrix();
	// logicArVolume=ConstructArVolume();
	// auto physArvolume= new G4PVPlacement(rotContainer, G4ThreeVector(), logicArVolume, "ArVolume", logicEnv, false, 0, checkOverlaps);

	// logicArContainer=ConstructArContainer();
	// auto physArContainer= new G4PVPlacement(rotContainer, G4ThreeVector(), logicArContainer, "ArContainer", logicEnv, false, 0, checkOverlaps);

	//=============================================================//
	//                            Shell                            //
	//=============================================================//
	auto logicShell = ConstructShell();

	//=============================================================//
	//                             BEGe                            //
	//=============================================================//
	auto rotBEGe = new G4RotationMatrix();
	G4LogicalVolume *logicBEGe = ConstructBEGe();
	auto physBEGe = new G4PVPlacement(0, G4ThreeVector(0, 0, -1 * mm), logicBEGe, "BEGe", logicShell, false, 0, checkOverlaps);

	//=============================================================//
	//                             Wire                            //
	//=============================================================//
	G4double WireLength = SmallestUnitHeight;
	fWireLength = WireLength;
	if (fWireType == "A1")
	{
		logicWire = ConstructA1(WireLength);
	}
	else if (fWireType == "A2")
	{
		logicWire = ConstructA2(WireLength);
	}
	else
	{
		G4cout << "Type does not exist!" << G4endl;
	}

	//=============================================================//
	//                             ASIC                            //
	//=============================================================//
	logicASICPlate = ConstructASICPlate();

	//=============================================================//
	//                      Light Fiber Array                      //
	//=============================================================//
	ConstructLightFiberArray(logicArVolume, fFiberPlacementCenter, fFiberPlacementRadius);

	//=============================================================//
	//                           String                            //
	//=============================================================//

	// Rotation and translation of a plate inside the assembly
	G4RotationMatrix *RotBEGe = new G4RotationMatrix();
	G4ThreeVector PosBEGe = G4ThreeVector(0, 0, 0);
	G4Transform3D TrBEGe = G4Transform3D(*RotBEGe, PosBEGe);

	G4double Gap = 0.1 * mm;
	G4RotationMatrix *RotASIC = new G4RotationMatrix();
	G4ThreeVector PosASIC = G4ThreeVector(0, 0, -fBEGeHeight / 2 - fASICThickness / 2 - fShellThickness - Gap);
	G4Transform3D TrASIC = G4Transform3D(*RotASIC, PosASIC);

	G4RotationMatrix *RotWire = new G4RotationMatrix();
	fWirePos = G4ThreeVector(fWireRadius + fBEGeRadius + fShellThickness + Gap, 0, 0);
	G4ThreeVector PosWire = fWirePos;
	G4Transform3D TrWire = G4Transform3D(*RotWire, PosWire);
	G4PVPlacement *physPackedBEGe[40] = {nullptr};
	G4PVPlacement *physWire[40] = {nullptr};
	G4PVPlacement *physASIC[40] = {nullptr};
	for (G4int i = 0; i < fUnitNb / 2; i++)
	{
		G4ThreeVector PosUnit1 = G4ThreeVector(0, 0, (SmallestUnitHeight / 2 + SmallestUnitHeight * i));
		G4ThreeVector PosUnit2 = G4ThreeVector(0, 0, (-SmallestUnitHeight / 2 - SmallestUnitHeight * i));
		physPackedBEGe[2 * i] = new G4PVPlacement(RotBEGe, PosUnit1 + PosBEGe, logicShell, "PackedBEGe", logicArVolume, false, 2 * i, checkOverlaps);
		physPackedBEGe[2 * i + 1] = new G4PVPlacement(RotBEGe, PosUnit2 + PosBEGe, logicShell, "PackedBEGe", logicArVolume, false, 2 * i + 1, checkOverlaps);
		physWire[2 * i] = new G4PVPlacement(RotWire, PosUnit1 + PosWire, logicWire, "Wire", logicArVolume, false, 2 * i, checkOverlaps);
		physWire[2 * i + 1] = new G4PVPlacement(RotWire, PosUnit2 + PosWire, logicWire, "Wire", logicArVolume, false, 2 * i + 1, checkOverlaps);
		physASIC[2 * i] = new G4PVPlacement(RotASIC, PosUnit1 + PosASIC, logicASICPlate, "ASIC", logicArVolume, false, 2 * i, checkOverlaps);
		physASIC[2 * i + 1] = new G4PVPlacement(RotASIC, PosUnit2 + PosASIC, logicASICPlate, "ASIC", logicArVolume, false, 2 * i + 1, checkOverlaps);
	}

	G4PVPlacement *physArVolumeSiPMSet[2] = {nullptr};
	auto logicArVolumeSiPMChip = ConstructArVolumeSiPM(fFiberPlacementRadius);
	physArVolumeSiPMSet[0] = new G4PVPlacement(0, G4ThreeVector(0, 0, fFiberLength / 2 + 3 * mm), logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 0, checkOverlaps);
	physArVolumeSiPMSet[1] = new G4PVPlacement(0, G4ThreeVector(0, 0, -fFiberLength / 2 - 3 * mm), logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 1, checkOverlaps);
	// physArVolumeSiPMSet[2] = new G4PVPlacement(0, G4ThreeVector(15 * cm, 0, fFiberLength/2 + 3 * mm), logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 0, checkOverlaps);
	// physArVolumeSiPMSet[3] = new G4PVPlacement(0, G4ThreeVector(-15 * cm, 0, fFiberLength/2 + 3 * mm), logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 0, checkOverlaps);
	// physArVolumeSiPMSet[4] = new G4PVPlacement(0, G4ThreeVector(0, 15 * cm, -fFiberLength/2 - 3 * mm), logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 0, checkOverlaps);
	// physArVolumeSiPMSet[5] = new G4PVPlacement(0, G4ThreeVector(0, -15 * cm, -fFiberLength/2 - 3 * mm), logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 0, checkOverlaps);
	// physArVolumeSiPMSet[6] = new G4PVPlacement(0, G4ThreeVector(15 * cm, 0, -fFiberLength/2 - 3 * mm), logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 0, checkOverlaps);
	// physArVolumeSiPMSet[7] = new G4PVPlacement(0, G4ThreeVector(-15 * cm, 0, -fFiberLength/2 - 3 * mm), logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 0, checkOverlaps);

	//=============================================================//
	//                        Optical Surfaces                     //
	//=============================================================//

	G4OpticalSurface *Ge_SAr = new G4OpticalSurface("Ge_SAr");
	Ge_SAr->SetType(dielectric_metal);
	Ge_SAr->SetModel(glisur);
	Ge_SAr->SetFinish(polished);

	G4OpticalSurface *scintWrap = new G4OpticalSurface("ScintWrap");
	scintWrap->SetModel(unified);
	scintWrap->SetType(dielectric_dielectric);
	scintWrap->SetFinish(polished);
	G4double energyReflector[] = {2.0 * eV, 3.0 * eV, 4.0 * eV, 5.0 * eV, 6.0 * eV, 7.0 * eV};
	const G4int num = sizeof(energyReflector) / sizeof(G4double);
	G4double reflectivity[] = {0.97, 0.97, 0.97, 0.97, 0.97, 0.97};
	assert(sizeof(reflectivity) == sizeof(energyReflector));
	G4MaterialPropertiesTable *scintWrapProperty = matVikuiti->GetMaterialPropertiesTable();
	scintWrapProperty->AddProperty("REFLECTIVITY", energyReflector, reflectivity, num);
	scintWrap->SetMaterialPropertiesTable(scintWrapProperty);

	// G4OpticalSurface *PMMA_Dielctric_Ref = new G4OpticalSurface("PMMA_Dielctric_Ref");
	// if (fReflectorType == "polishedvm2000air")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUT);
	// 	PMMA_Dielctric_Ref->SetModel(LUT);
	// 	PMMA_Dielctric_Ref->SetFinish(polishedvm2000air);
	// }
	// else if (fReflectorType == "polishedvm2000glue")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUT);
	// 	PMMA_Dielctric_Ref->SetModel(LUT);
	// 	PMMA_Dielctric_Ref->SetFinish(polishedvm2000glue);
	// }
	// else if (fReflectorType == "polishedtyvekair")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUT);
	// 	PMMA_Dielctric_Ref->SetModel(LUT);
	// 	PMMA_Dielctric_Ref->SetFinish(polishedtyvekair);
	// }
	// else if (fReflectorType == "polishedlumirrorair")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUT);
	// 	PMMA_Dielctric_Ref->SetModel(LUT);
	// 	PMMA_Dielctric_Ref->SetFinish(polishedlumirrorair);
	// }
	// else if (fReflectorType == "PolishedESR_LUT")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(PolishedESR_LUT);
	// }
	// else if (fReflectorType == "PolishedESRGrease_LUT")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(PolishedESRGrease_LUT);
	// }
	// else if (fReflectorType == "PolishedTeflon_LUT")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(PolishedTeflon_LUT);
	// }
	// else if (fReflectorType == "RoughESR_LUT")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(RoughESR_LUT);
	// }
	// else
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(Polished_LUT);
	// }

	G4OpticalSurface *SAr_SAr = new G4OpticalSurface("SAr_SAr");
	SAr_SAr->SetType(dielectric_dielectric);
	SAr_SAr->SetModel(unified);
	SAr_SAr->SetFinish(polished);

	G4OpticalSurface *PMMA_Dielectric = new G4OpticalSurface("PMMA_Dielectric");
	PMMA_Dielectric->SetType(dielectric_LUTDAVIS);
	PMMA_Dielectric->SetModel(LUT);
	PMMA_Dielectric->SetFinish(Polished_LUT);

	G4OpticalSurface *Ge_Dielectric = new G4OpticalSurface("Ge_Dielectric");
	Ge_Dielectric->SetType(dielectric_metal);
	Ge_Dielectric->SetModel(glisur);
	Ge_Dielectric->SetFinish(polished);

	G4OpticalSurface *Wire_Dielectric = new G4OpticalSurface("Wire_Dielectric");
	Wire_Dielectric->SetType(dielectric_dielectric);
	Wire_Dielectric->SetModel(unified);
	Wire_Dielectric->SetFinish(polished);

	G4OpticalSurface *ASIC_Dielectric = new G4OpticalSurface("ASIC_Dielectric");
	ASIC_Dielectric->SetType(dielectric_dielectric);
	ASIC_Dielectric->SetModel(unified);
	ASIC_Dielectric->SetFinish(polished);

	G4LogicalSkinSurface *ASIC_LSS = new G4LogicalSkinSurface("ASIC_LSS", logicASICPlate, ASIC_Dielectric);
	G4LogicalSkinSurface *Wire_LSS = new G4LogicalSkinSurface("Wire_LSS", logicWire, Wire_Dielectric);
	G4LogicalSkinSurface *ArVolume_LSS = new G4LogicalSkinSurface("ArVolume_LSS", logicArVolume, PMMA_Dielectric);
	G4LogicalSkinSurface *Bucket_LSS = new G4LogicalSkinSurface("Bucket_LSS", logicBucket, PMMA_Dielectric);
	G4LogicalSkinSurface *BEGe_LSS = new G4LogicalSkinSurface("BEGe_LSS", logicBEGe, Ge_Dielectric);
	G4LogicalSkinSurface *Shell_LSS = new G4LogicalSkinSurface("Shell_LSS", logicBEGe, PMMA_Dielectric);

	GetPhysicalVolumeProperties();

	return physWorld;
}

G4VPhysicalVolume *CDEXDetectorConstruction::ConstructFiberTestSetup()
{

	G4NistManager *nist = G4NistManager::Instance();
	G4bool checkOverlaps = true;

	//=============================================================//
	//                        World&Envelope                       //
	//=============================================================//

	G4Material *world_mat = matLN2;
	G4Material *env_mat = matLAr;
	G4Material *det_mat = matEnGe;

	G4double world_size = 600 * cm;
	G4double env_size = 500 * cm;
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "logicWorld");
	G4VPhysicalVolume *physWorld =
		new G4PVPlacement(0,			   // no rotation
						  G4ThreeVector(), // at (0,0,0)
						  logicWorld,	   // its logical volume
						  "World",		   // its name
						  0,			   // its mother  volume
						  false,		   // no boolean operation
						  0,			   // copy number
						  checkOverlaps);  // overlaps checking

	G4Box *solidEnv = new G4Box("solidEnvelope", 0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "logicEnvelope");
	physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);
	G4double FiberLength = fFiberLength;

	logicFiber = ConstructLightFiber(fFiberLength);
	G4VPhysicalVolume *physFiber = new G4PVPlacement(0, G4ThreeVector(), logicFiber, "Fiber", logicEnv, false, 0, checkOverlaps);
	return physWorld;
}

G4VPhysicalVolume *CDEXDetectorConstruction::ConstructCDEX300()
{

	G4NistManager *nist = G4NistManager::Instance();
	G4bool checkOverlaps = true;
	G4double SmallestUnitHeight = fSmallestUnitHeight;

	//=============================================================//
	//                        World&Envelope                       //
	//=============================================================//

	G4Material *world_mat = fVacuum;
	G4Material *env_mat = matLN2;
	G4Material *det_mat = matEnGe;

	G4double world_size = 600 * cm;
	G4double env_size = 500 * cm;
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "logicWorld");
	G4VPhysicalVolume *physWorld =
		new G4PVPlacement(0,			   // no rotation
						  G4ThreeVector(), // at (0,0,0)
						  logicWorld,	   // its logical volume
						  "World",		   // its name
						  0,			   // its mother  volume
						  false,		   // no boolean operation
						  0,			   // copy number
						  checkOverlaps);  // overlaps checking

	G4Box *solidEnv = new G4Box("solidEnvelope", 0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "logicEnvelope");
	physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

	//=============================================================//
	//                          Container                          //
	//=============================================================//
	auto rotLArContainer = new G4RotationMatrix();
	auto logicLArContainer = ConstructCDEX300LArContainer();

	auto physLArContainer = new G4PVPlacement(rotLArContainer, G4ThreeVector(), logicLArContainer, "Container", logicEnv, false, 0, checkOverlaps);

	//=============================================================//
	//                            Shell                            //
	//=============================================================//
	auto logicShell = ConstructShell();

	//=============================================================//
	//                             BEGe                            //
	//=============================================================//
	auto rotBEGe = new G4RotationMatrix();
	G4LogicalVolume *logicBEGe = ConstructBEGe();
	auto physBEGe = new G4PVPlacement(0, G4ThreeVector(0, 0, -1 * mm), logicBEGe, "BEGe", logicShell, false, 0, checkOverlaps);

	//=============================================================//
	//                             Wire                            //
	//=============================================================//
	G4double WireLength = SmallestUnitHeight;
	fWireLength = WireLength;
	if (fWireType == "A1")
	{
		logicWire = ConstructA1(WireLength);
	}
	else if (fWireType == "A2")
	{
		logicWire = ConstructA2(WireLength);
	}
	else
	{
		G4cout << "Type does not exist!" << G4endl;
	}

	//=============================================================//
	//                             ASIC                            //
	//=============================================================//
	logicASICPlate = ConstructASICPlate();

	//=============================================================//
	//                              LG                             //
	//=============================================================//
	// auto logicLightGuide = ConstructLightGuide(LightGuideLength, LightGuideRadius, matLAr);
	auto logicLightGuide = ConstructHexLightGuide(fLightGuideLength, fLightGuideRadius, matLAr);

	//=============================================================//
	//                        LightReadout                         //
	//=============================================================//
	// auto logicArVolumeSiPMChip = ConstructArVolumeSiPM(fLightGuideRadius);
	auto logicArVolumeSiPMChip = ConstructArVolumeHexSiPM(fLightGuideRadius);

	//=============================================================//
	//                           String                            //
	//=============================================================//

	// Rotation and translation of a plate inside the assembly
	G4RotationMatrix *RotBEGe = new G4RotationMatrix();
	G4ThreeVector PosBEGe = G4ThreeVector(0, 0, 0);
	G4Transform3D TrBEGe = G4Transform3D(*RotBEGe, PosBEGe);

	G4double Gap = 0.1 * mm;
	G4RotationMatrix *RotASIC = new G4RotationMatrix();
	G4ThreeVector PosASIC = G4ThreeVector(0, 0, -fBEGeHeight / 2 - fASICThickness / 2 - fShellThickness - Gap);
	G4Transform3D TrASIC = G4Transform3D(*RotASIC, PosASIC);

	G4RotationMatrix *RotWire = new G4RotationMatrix();
	fWirePos = G4ThreeVector(fWireRadius + fBEGeRadius + fShellThickness + Gap, 0, 0);
	G4ThreeVector PosWire = fWirePos;
	G4Transform3D TrWire = G4Transform3D(*RotWire, PosWire);
	G4PVPlacement *physPackedBEGe[40][19] = {nullptr};
	G4PVPlacement *physWire[40][19] = {nullptr};
	G4PVPlacement *physASIC[40][19] = {nullptr};
	G4PVPlacement *physArVolumeSiPMSet[2][19] = {nullptr};
	G4PVPlacement *physArLightGuideSet[19] = {nullptr};
	G4double StringDist = fLightGuideRadius * sqrt(3) + 5 * mm; // distance between two strings

	for (G4int strid = 0; strid < 19; strid++)
	{
		G4ThreeVector XYPos = StringPosList[strid] * StringDist;
		for (G4int i = 0; i < fUnitNb; i++)
		{
			G4ThreeVector PosUnit = G4ThreeVector(0, 0, (SmallestUnitHeight / 2 + SmallestUnitHeight * ceil(i / 2)) * pow(-1, i)) + XYPos;
			physPackedBEGe[i][strid] = new G4PVPlacement(RotBEGe, PosUnit + PosBEGe, logicShell, "PackedBEGe", logicArVolume, false, 0, checkOverlaps);
			physWire[i][strid] = new G4PVPlacement(RotWire, PosUnit + PosWire, logicWire, "Wire", logicArVolume, false, 0, checkOverlaps);
			physASIC[i][strid] = new G4PVPlacement(RotASIC, PosUnit + PosASIC, logicASICPlate, "ASIC", logicArVolume, false, 0, checkOverlaps);
		}
		physArVolumeSiPMSet[0][strid] = new G4PVPlacement(0, G4ThreeVector(0, 0, fLightGuideLength / 2 + 3 * mm) + XYPos, logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 0, checkOverlaps);
		physArVolumeSiPMSet[1][strid] = new G4PVPlacement(0, G4ThreeVector(0, 0, -fLightGuideLength / 2 - 3 * mm) + XYPos, logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 1, checkOverlaps);
		physArLightGuideSet[strid] = new G4PVPlacement(0, XYPos, logicLightGuide, "LightGuide", logicArVolume, false, 0, checkOverlaps);
		//ConstructHexLightFiberArray(logicArVolume, XYPos, fLightGuideRadius);
		//ConstructLightFiberArray(logicArVolume, XYPos, fLightGuideRadius);
	}

	//=============================================================//
	//                        Optical Surfaces                     //
	//=============================================================//

	G4OpticalSurface *Ge_SAr = new G4OpticalSurface("Ge_SAr");
	Ge_SAr->SetType(dielectric_metal);
	Ge_SAr->SetModel(glisur);
	Ge_SAr->SetFinish(polished);

	G4OpticalSurface *scintWrap = new G4OpticalSurface("ScintWrap");
	scintWrap->SetModel(unified);
	scintWrap->SetType(dielectric_dielectric);
	scintWrap->SetFinish(polished);
	G4double energyReflector[] = {2.0 * eV, 3.0 * eV, 4.0 * eV, 5.0 * eV, 6.0 * eV, 7.0 * eV};
	const G4int num = sizeof(energyReflector) / sizeof(G4double);
	G4double reflectivity[] = {0.97, 0.97, 0.97, 0.97, 0.97, 0.97};
	assert(sizeof(reflectivity) == sizeof(energyReflector));
	G4MaterialPropertiesTable *scintWrapProperty = matVikuiti->GetMaterialPropertiesTable();
	scintWrapProperty->AddProperty("REFLECTIVITY", energyReflector, reflectivity, num);
	scintWrap->SetMaterialPropertiesTable(scintWrapProperty);

	G4OpticalSurface *PMMA_Dielctric_Ref = new G4OpticalSurface("PMMA_Dielctric_Ref");
	if (fReflectorType == "polishedvm2000air")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUT);
		PMMA_Dielctric_Ref->SetModel(LUT);
		PMMA_Dielctric_Ref->SetFinish(polishedvm2000air);
	}
	else if (fReflectorType == "polishedvm2000glue")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUT);
		PMMA_Dielctric_Ref->SetModel(LUT);
		PMMA_Dielctric_Ref->SetFinish(polishedvm2000glue);
	}
	else if (fReflectorType == "polishedtyvekair")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUT);
		PMMA_Dielctric_Ref->SetModel(LUT);
		PMMA_Dielctric_Ref->SetFinish(polishedtyvekair);
	}
	else if (fReflectorType == "polishedlumirrorair")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUT);
		PMMA_Dielctric_Ref->SetModel(LUT);
		PMMA_Dielctric_Ref->SetFinish(polishedlumirrorair);
	}
	else if (fReflectorType == "PolishedESR_LUT")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(PolishedESR_LUT);
	}
	else if (fReflectorType == "PolishedESRGrease_LUT")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(PolishedESRGrease_LUT);
	}
	else if (fReflectorType == "PolishedTeflon_LUT")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(PolishedTeflon_LUT);
	}
	else if (fReflectorType == "RoughESR_LUT")
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(RoughESR_LUT);
	}
	else
	{
		PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
		PMMA_Dielctric_Ref->SetModel(DAVIS);
		PMMA_Dielctric_Ref->SetFinish(Polished_LUT);
	}

	G4OpticalSurface *SAr_SAr = new G4OpticalSurface("SAr_SAr");
	SAr_SAr->SetType(dielectric_dielectric);
	SAr_SAr->SetModel(unified);
	SAr_SAr->SetFinish(polished);

	G4OpticalSurface *PMMA_Dielectric = new G4OpticalSurface("PMMA_Dielectric");
	PMMA_Dielectric->SetType(dielectric_LUTDAVIS);
	PMMA_Dielectric->SetModel(LUT);
	PMMA_Dielectric->SetFinish(Polished_LUT);

	G4OpticalSurface *Ge_Dielectric = new G4OpticalSurface("Ge_Dielectric");
	Ge_Dielectric->SetType(dielectric_metal);
	Ge_Dielectric->SetModel(glisur);
	Ge_Dielectric->SetFinish(polished);

	G4OpticalSurface *Wire_Dielectric = new G4OpticalSurface("Wire_Dielectric");
	Wire_Dielectric->SetType(dielectric_dielectric);
	Wire_Dielectric->SetModel(unified);
	Wire_Dielectric->SetFinish(polished);

	G4OpticalSurface *ASIC_Dielectric = new G4OpticalSurface("ASIC_Dielectric");
	ASIC_Dielectric->SetType(dielectric_dielectric);
	ASIC_Dielectric->SetModel(unified);
	ASIC_Dielectric->SetFinish(polished);

	G4LogicalSkinSurface *ASIC_LSS = new G4LogicalSkinSurface("ASIC_LSS", logicASICPlate, ASIC_Dielectric);
	G4LogicalSkinSurface *Wire_LSS = new G4LogicalSkinSurface("Wire_LSS", logicWire, Wire_Dielectric);
	G4LogicalSkinSurface *Bucket_LSS = new G4LogicalSkinSurface("Bucket_LSS", logicLArContainer, PMMA_Dielectric);
	G4LogicalSkinSurface *BEGe_LSS = new G4LogicalSkinSurface("BEGe_LSS", logicBEGe, Ge_Dielectric);
	G4LogicalSkinSurface *Shell_LSS = new G4LogicalSkinSurface("Shell_LSS", logicBEGe, PMMA_Dielectric);

	GetPhysicalVolumeProperties();

	return physWorld;
}

G4VPhysicalVolume *CDEXDetectorConstruction::ConstructBucketLightGuideSystem()
{

	G4NistManager *nist = G4NistManager::Instance();
	G4bool checkOverlaps = true;
	G4double SmallestUnitHeight = fSmallestUnitHeight;

	//=============================================================//
	//                        World&Envelope                       //
	//=============================================================//

	G4Material *world_mat = fVacuum;
	G4Material *env_mat = matLN2;
	G4Material *det_mat = matEnGe;

	G4double world_size = 600 * cm;
	G4double env_size = 500 * cm;
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "logicWorld");
	G4VPhysicalVolume *physWorld =
		new G4PVPlacement(0,			   // no rotation
						  G4ThreeVector(), // at (0,0,0)
						  logicWorld,	   // its logical volume
						  "World",		   // its name
						  0,			   // its mother  volume
						  false,		   // no boolean operation
						  0,			   // copy number
						  checkOverlaps);  // overlaps checking

	G4Box *solidEnv = new G4Box("solidEnvelope", 0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "logicEnvelope");
	physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

	//=============================================================//
	//                            Bucket                           //
	//=============================================================//
	// auto rotBucket = new G4RotationMatrix();
	// auto logicBucket = ConstructLightGuideBucket();
	// auto physBucket = new G4PVPlacement(rotBucket, G4ThreeVector(), logicBucket, "Bucket", logicEnv, false, 0, checkOverlaps);
	
	//=============================================================//
	//                          Container                          //
	//=============================================================//
	auto rotLArContainer = new G4RotationMatrix();
	auto logicLArContainer = ConstructCDEX300LArContainer();

	auto physLArContainer = new G4PVPlacement(rotLArContainer, G4ThreeVector(), logicLArContainer, "Container", logicEnv, false, 0, checkOverlaps);

	//=============================================================//
	//                            Shell                            //
	//=============================================================//
	auto logicShell = ConstructShell();

	//=============================================================//
	//                             BEGe                            //
	//=============================================================//
	auto rotBEGe = new G4RotationMatrix();
	G4LogicalVolume *logicBEGe = ConstructBEGe();
	auto physBEGe = new G4PVPlacement(0, G4ThreeVector(0, 0, -1 * mm), logicBEGe, "BEGe", logicShell, false, 0, checkOverlaps);

	//=============================================================//
	//                             Wire                            //
	//=============================================================//
	G4double WireLength = SmallestUnitHeight;
	fWireLength = WireLength;
	if (fWireType == "A1")
	{
		logicWire = ConstructA1(WireLength);
	}
	else if (fWireType == "A2")
	{
		logicWire = ConstructA2(WireLength);
	}
	else
	{
		G4cout << "Type does not exist!" << G4endl;
	}

	//=============================================================//
	//                             ASIC                            //
	//=============================================================//
	logicASICPlate = ConstructASICPlate();

	//=============================================================//
	//                         Light Guide                         //
	//=============================================================//
	G4double LightGuideRadius = fLightGuideRadius;
	G4double LightGuideLength = fLightGuideLength;
	// auto logicLightGuide = ConstructLightGuide(LightGuideLength, LightGuideRadius, matLAr);
	auto logicLightGuide = ConstructHexLightGuide(LightGuideLength, LightGuideRadius, matLAr);

	//=============================================================//
	//                        Light Readout                        //
	//=============================================================//
	//auto logicArVolumeSiPMChip = ConstructArVolumeSiPM(LightGuideRadius);
	auto logicArVolumeSiPMChip = ConstructArVolumeHexSiPM(LightGuideRadius);

	//=============================================================//
	//                          Reflector                          //
	//=============================================================//
	auto logicArVolumeReflector=ConstructArVolumeHexReflector(LightGuideRadius);

	//=============================================================//
	//                            String                           //
	//=============================================================//

	// Rotation and translation of a plate inside the assembly
	G4RotationMatrix *RotBEGe = new G4RotationMatrix();
	G4ThreeVector PosBEGe = G4ThreeVector(0, 0, 0);
	G4Transform3D TrBEGe = G4Transform3D(*RotBEGe, PosBEGe);

	G4double Gap = 0.1 * mm;
	G4RotationMatrix *RotASIC = new G4RotationMatrix();
	G4ThreeVector PosASIC = G4ThreeVector(0, 0, -fBEGeHeight / 2 - fASICThickness / 2 - fShellThickness - Gap);
	G4Transform3D TrASIC = G4Transform3D(*RotASIC, PosASIC);

	G4RotationMatrix *RotWire = new G4RotationMatrix();
	fWirePos = G4ThreeVector(fWireRadius + fBEGeRadius + fShellThickness + Gap, 0, 0);
	G4ThreeVector PosWire = fWirePos;
	G4Transform3D TrWire = G4Transform3D(*RotWire, PosWire);
	G4PVPlacement *physPackedBEGe[40][19] = {nullptr};
	G4PVPlacement *physWire[40][19] = {nullptr};
	G4PVPlacement *physASIC[40][19] = {nullptr};
	G4PVPlacement *physArVolumeSiPMSet[2][19] = {nullptr};
	G4PVPlacement *physArVolumeReflectorSet[2][19] = {nullptr};
	G4PVPlacement *physArLightGuideSet[19] = {nullptr};
	G4double StringDist = fLightGuideRadius * sqrt(3) + 5 * mm; // distance between two strings

	for (G4int strid = 0; strid < 19; strid++)
	{
		G4ThreeVector XYPos = StringPosList[strid] * StringDist;
		for (G4int i = 0; i < fUnitNb; i++)
		{
			G4ThreeVector PosUnit = G4ThreeVector(0, 0, (SmallestUnitHeight / 2 + SmallestUnitHeight * ceil(i / 2)) * pow(-1, i)) + XYPos;
			physPackedBEGe[i][strid] = new G4PVPlacement(RotBEGe, PosUnit + PosBEGe, logicShell, "PackedBEGe", logicArVolume, false, 0, checkOverlaps);
			physWire[i][strid] = new G4PVPlacement(RotWire, PosUnit + PosWire, logicWire, "Wire", logicArVolume, false, 0, checkOverlaps);
			physASIC[i][strid] = new G4PVPlacement(RotASIC, PosUnit + PosASIC, logicASICPlate, "ASIC", logicArVolume, false, 0, checkOverlaps);
		}
		//physArVolumeSiPMSet[0][strid] = new G4PVPlacement(0, G4ThreeVector(0, 0, fLightGuideLength / 2 + 3 * mm) + XYPos, logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 0, checkOverlaps);
		//physArVolumeSiPMSet[1][strid] = new G4PVPlacement(0, G4ThreeVector(0, 0, -fLightGuideLength / 2 - 3 * mm) + XYPos, logicArVolumeSiPMChip, "ArVolumeSiPM", logicArVolume, false, 1, checkOverlaps);
		physArVolumeReflectorSet[0][strid]=new G4PVPlacement(0, G4ThreeVector(0, 0, -fLightGuideLength / 2 - 3 * mm) + XYPos, logicArVolumeReflector, "ArVolumeReflector", logicArVolume, false, 1, checkOverlaps);
		//physArLightGuideSet[strid] = new G4PVPlacement(0, XYPos, logicLightGuide, "LightGuide", logicArVolume, false, 0, checkOverlaps);
		ConstructHexLightFiberArray(logicArVolume, XYPos, LightGuideRadius);
		//ConstructLightFiberArray(logicArVolume, XYPos, LightGuideRadius);
	}

	//=============================================================//
	//                        Optical Surfaces                     //
	//=============================================================//

	G4OpticalSurface *Ge_SAr = new G4OpticalSurface("Ge_SAr");
	Ge_SAr->SetType(dielectric_metal);
	Ge_SAr->SetModel(glisur);
	Ge_SAr->SetFinish(polished);

	G4OpticalSurface *scintWrap = new G4OpticalSurface("ScintWrap");
	scintWrap->SetModel(unified);
	scintWrap->SetType(dielectric_dielectric);
	scintWrap->SetFinish(polished);
	G4double energyReflector[] = {2.0 * eV, 3.0 * eV, 4.0 * eV, 5.0 * eV, 6.0 * eV, 7.0 * eV};
	const G4int num = sizeof(energyReflector) / sizeof(G4double);
	G4double reflectivity[] = {0.97, 0.97, 0.97, 0.97, 0.97, 0.97};
	assert(sizeof(reflectivity) == sizeof(energyReflector));
	G4MaterialPropertiesTable *scintWrapProperty = matVikuiti->GetMaterialPropertiesTable();
	scintWrapProperty->AddProperty("REFLECTIVITY", energyReflector, reflectivity, num);
	scintWrap->SetMaterialPropertiesTable(scintWrapProperty);

	// G4OpticalSurface *PMMA_Dielctric_Ref = new G4OpticalSurface("PMMA_Dielctric_Ref");
	// if (fReflectorType == "polishedvm2000air")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUT);
	// 	PMMA_Dielctric_Ref->SetModel(LUT);
	// 	PMMA_Dielctric_Ref->SetFinish(polishedvm2000air);
	// }
	// else if (fReflectorType == "polishedvm2000glue")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUT);
	// 	PMMA_Dielctric_Ref->SetModel(LUT);
	// 	PMMA_Dielctric_Ref->SetFinish(polishedvm2000glue);
	// }
	// else if (fReflectorType == "polishedtyvekair")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUT);
	// 	PMMA_Dielctric_Ref->SetModel(LUT);
	// 	PMMA_Dielctric_Ref->SetFinish(polishedtyvekair);
	// }
	// else if (fReflectorType == "polishedlumirrorair")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUT);
	// 	PMMA_Dielctric_Ref->SetModel(LUT);
	// 	PMMA_Dielctric_Ref->SetFinish(polishedlumirrorair);
	// }
	// else if (fReflectorType == "PolishedESR_LUT")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(PolishedESR_LUT);
	// }
	// else if (fReflectorType == "PolishedESRGrease_LUT")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(PolishedESRGrease_LUT);
	// }
	// else if (fReflectorType == "PolishedTeflon_LUT")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(PolishedTeflon_LUT);
	// }
	// else if (fReflectorType == "RoughESR_LUT")
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(RoughESR_LUT);
	// }
	// else
	// {
	// 	PMMA_Dielctric_Ref->SetType(dielectric_LUTDAVIS);
	// 	PMMA_Dielctric_Ref->SetModel(DAVIS);
	// 	PMMA_Dielctric_Ref->SetFinish(Polished_LUT);
	// }

	G4OpticalSurface *SAr_SAr = new G4OpticalSurface("SAr_SAr");
	SAr_SAr->SetType(dielectric_dielectric);
	SAr_SAr->SetModel(unified);
	SAr_SAr->SetFinish(polished);

	G4OpticalSurface *PMMA_Dielectric = new G4OpticalSurface("PMMA_Dielectric");
	PMMA_Dielectric->SetType(dielectric_LUTDAVIS);
	PMMA_Dielectric->SetModel(LUT);
	PMMA_Dielectric->SetFinish(Polished_LUT);

	G4OpticalSurface *Ge_Dielectric = new G4OpticalSurface("Ge_Dielectric");
	Ge_Dielectric->SetType(dielectric_metal);
	Ge_Dielectric->SetModel(glisur);
	Ge_Dielectric->SetFinish(polished);

	G4OpticalSurface *Wire_Dielectric = new G4OpticalSurface("Wire_Dielectric");
	Wire_Dielectric->SetType(dielectric_dielectric);
	Wire_Dielectric->SetModel(unified);
	Wire_Dielectric->SetFinish(polished);

	G4OpticalSurface *ASIC_Dielectric = new G4OpticalSurface("ASIC_Dielectric");
	ASIC_Dielectric->SetType(dielectric_dielectric);
	ASIC_Dielectric->SetModel(unified);
	ASIC_Dielectric->SetFinish(polished);

	G4LogicalSkinSurface *ASIC_LSS = new G4LogicalSkinSurface("ASIC_LSS", logicASICPlate, ASIC_Dielectric);
	G4LogicalSkinSurface *Wire_LSS = new G4LogicalSkinSurface("Wire_LSS", logicWire, Wire_Dielectric);
	//G4LogicalSkinSurface *Bucket_LSS = new G4LogicalSkinSurface("Bucket_LSS", logicBucket, PMMA_Dielectric);
	G4LogicalSkinSurface *BEGe_LSS = new G4LogicalSkinSurface("BEGe_LSS", logicBEGe, Ge_Dielectric);
	G4LogicalSkinSurface *Shell_LSS = new G4LogicalSkinSurface("Shell_LSS", logicBEGe, PMMA_Dielectric);

	GetPhysicalVolumeProperties();

	return physWorld;
}

G4VPhysicalVolume *CDEXDetectorConstruction::ConstructArExpSetup()
{

	G4NistManager *nist = G4NistManager::Instance();
	G4bool checkOverlaps = true;

	//=============================================================//
	//                        World&Envelope                       //
	//=============================================================//

	G4Material *world_mat = matLN2;
	G4Material *env_mat = matLAr;
	G4Material *det_mat = matEnGe;

	G4double world_size = 600 * cm;
	G4double env_size = 500 * cm;
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, world_mat, "logicWorld");
	G4VPhysicalVolume *physWorld =
		new G4PVPlacement(0,			   // no rotation
						  G4ThreeVector(), // at (0,0,0)
						  logicWorld,	   // its logical volume
						  "World",		   // its name
						  0,			   // its mother  volume
						  false,		   // no boolean operation
						  0,			   // copy number
						  checkOverlaps);  // overlaps checking

	G4Box *solidEnv = new G4Box("solidEnvelope", 0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
	G4LogicalVolume *logicEnv = new G4LogicalVolume(solidEnv, env_mat, "logicEnvelope");
	physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

	auto solidArExpContainer = new G4Tubs("solidArExpContainer", 0 * cm, 41 * cm, 3 * m, 0, twopi);
	auto logicArExpContainer = new G4LogicalVolume(solidArExpContainer, matSS, "logicArExpContainer");

	auto solidArVolume = new G4Tubs("solidArVolume", 0 * cm, 40 * cm, 3 * m, 0, twopi);
	logicArVolume = new G4LogicalVolume(solidArVolume, matLAr, "logicEnvelope");
	auto physArVolume = new G4PVPlacement(0, G4ThreeVector(), logicArVolume, "ArVolume", logicArExpContainer, false, 0, checkOverlaps);

	auto solidImaginaryPMT = new G4Tubs("solidImaginaryPMT", 0 * cm, 20 * cm, 1 * mm, 0, twopi);
	logicImaginaryPMT = new G4LogicalVolume(solidImaginaryPMT, matLAr, "logicImaginaryPMT");

	auto physArExpContainer = new G4PVPlacement(0, G4ThreeVector(), logicArExpContainer, "ArExpContainer", logicEnv, false, 0, checkOverlaps);

	G4PVPlacement *physImaginaryPMT[40] = {nullptr};
	for (G4int i = 0; i < 20; i++)
	{
		physImaginaryPMT[2 * i] = new G4PVPlacement(0, G4ThreeVector(0, 0, 10 * cm * i + 10 * cm), logicImaginaryPMT, "ImaginaryPMT", logicArVolume, false, i, checkOverlaps);
		physImaginaryPMT[2 * i + 1] = new G4PVPlacement(0, G4ThreeVector(0, 0, -10 * cm * i - 10 * cm), logicImaginaryPMT, "ImaginaryPMT", logicArVolume, false, i, checkOverlaps);
	}

	const G4int NUMENTRIES_Surf = 4;
	G4double Wavelength[NUMENTRIES_Surf] = {100., 200., 301., 650.};
	G4double RefPhotonEnergy[NUMENTRIES_Surf];
	G4double RefReflectivity[NUMENTRIES_Surf];

	for (G4int ji = 0; ji < NUMENTRIES_Surf; ji++)
	{
		RefPhotonEnergy[ji] = LambdaE / (Wavelength[(NUMENTRIES_Surf - 1) - ji] * nm);
		RefReflectivity[ji] = 0;
	}
	G4MaterialPropertiesTable *PerfectAbs_Surf_MPT = new G4MaterialPropertiesTable();
	PerfectAbs_Surf_MPT->AddProperty("REFLECTIVITY", RefPhotonEnergy, RefReflectivity, NUMENTRIES_Surf);
	G4OpticalSurface *PerfectAbs_Surf = new G4OpticalSurface("PerfectAbs_Surf", unified, polished, dielectric_metal);
	PerfectAbs_Surf->SetMaterialPropertiesTable(PerfectAbs_Surf_MPT);
	G4LogicalBorderSurface *SSContainer_Surf_LBS1 = new G4LogicalBorderSurface("SSContainer_Surf_LBS", physArVolume, physArExpContainer, PerfectAbs_Surf);
	G4LogicalBorderSurface *SSContainer_Surf_LBS2 = new G4LogicalBorderSurface("SSContainer_Surf_LBS", physArExpContainer, physArVolume, PerfectAbs_Surf);

	return physWorld;
}