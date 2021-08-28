
//LEGEND Version

/*
#include "globals.hh"
#include "CDEXPhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4ProcessManager.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "G4UnitsTable.hh"
#include "G4ParticleTypes.hh"
#include "G4PhysicsListHelper.hh"
#include "G4RadioactiveDecay.hh"
#include "G4SystemOfUnits.hh"
#include "G4NuclideTable.hh"
#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4NuclearLevelData.hh"
#include "G4DeexPrecoParameters.hh"

G4ThreadLocal G4int CDEXPhysicsList::fVerboseLevel = 0;
G4ThreadLocal G4int CDEXPhysicsList::fMaxNumPhotonStep = 20;
G4ThreadLocal G4Cerenkov* CDEXPhysicsList::fCerenkovProcess = 0;
G4ThreadLocal G4Scintillation* CDEXPhysicsList::fScintillationProcess = 0;
G4ThreadLocal G4OpAbsorption* CDEXPhysicsList::fAbsorptionProcess = 0;
G4ThreadLocal G4OpRayleigh* CDEXPhysicsList::fRayleighScatteringProcess = 0;
G4ThreadLocal G4OpMieHG* CDEXPhysicsList::fMieHGScatteringProcess = 0;
G4ThreadLocal G4OpBoundaryProcess* CDEXPhysicsList::fBoundaryProcess = 0;
G4ThreadLocal G4OpWLS* CDEXPhysicsList::fWLSProcess = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXPhysicsList::CDEXPhysicsList()
 : G4VUserPhysicsList() {

	defaultCutValue = 1.0 * micrometer; //
	cutForGamma = defaultCutValue;
	//cutForElectron = 1.0 * nanometer;
    cutForElectron = defaultCutValue / 10;
	cutForPositron = defaultCutValue;

  	VerboseLevel = 0;
  	OpVerbLevel = 0;

  	SetVerboseLevel(VerboseLevel);
	//end modifs
	//read new PhotonEvaporation data set 
    // mandatory for G4NuclideTable
    //
	G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1 * picosecond);
	G4NuclideTable::GetInstance()->SetLevelTolerance(1.0 * eV);
	//
	G4DeexPrecoParameters* deex =
		G4NuclearLevelData::GetInstance()->GetParameters();
	deex->SetCorrelatedGamma(false);
	deex->SetStoreAllLevels(true);
	deex->SetMaxLifeTime(G4NuclideTable::GetInstance()->GetThresholdOfHalfLife() / std::log(2.));
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CDEXPhysicsList::~CDEXPhysicsList()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void CDEXPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program.

  G4BosonConstructor bConstructor;
  bConstructor.ConstructParticle();

  G4LeptonConstructor lConstructor;
  lConstructor.ConstructParticle();

  G4MesonConstructor mConstructor;
  mConstructor.ConstructParticle();

  G4BaryonConstructor rConstructor;
  rConstructor.ConstructParticle();

  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXPhysicsList::ConstructProcess()
{

  AddTransportation();
  ConstructDecay();
  ConstructEM();
  ConstructOp();

  SetVerbose(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Decay.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXPhysicsList::ConstructDecay()
{

  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }

  G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();
  
  radioactiveDecay->SetARM(false);               //Atomic Rearangement
  //radioactiveDecay->SetARM(true);               //Atomic Rearangement

  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());

  // Need to initialize atomic deexcitation outside of radioactive decay
  G4LossTableManager* theManager = G4LossTableManager::Instance();
  G4VAtomDeexcitation* p = theManager->AtomDeexcitation();
  if (!p) {
     G4UAtomicDeexcitation* atomDeex = new G4UAtomicDeexcitation();
     theManager->SetAtomDeexcitation(atomDeex);
     atomDeex->InitialiseAtomicDeexcitation();
  }
  //
  // mandatory for G4NuclideTable
  //
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1*picosecond);
  //G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(1.1*picosecond);
  G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXPhysicsList::ConstructEM()
{
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
    // gamma
      // Construct processes for gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-") {
    //electron
      //Modified by Luis
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,-1, 1, -1);

      // Ionisation
      G4eIonisation* eIonisation = new G4eIonisation();
      eIonisation->SetEmModel(new G4LivermoreIonisationModel());
      eIonisation->SetStepFunction(0.2, 100*um); //improved precision in tracking 
      pmanager->AddProcess(eIonisation,-1, 2, 2);

      // Bremsstrahlung
      G4eBremsstrahlung* eBremsstrahlung = new G4eBremsstrahlung();
      eBremsstrahlung->SetEmModel(new G4LivermoreBremsstrahlungModel());
      pmanager->AddProcess(eBremsstrahlung, -1,-3, 3);
      //End of modifs by Luis
      //Construct processes for electron
      //pmanager->AddProcess(new G4eMultipleScattering(),-1, 1, 1);
      //pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
      //pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);

    } else if (particleName == "e+") {
    //positron
      // Construct processes for positron
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation(),  0,-1, 4);

    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
    //muon
     // Construct processes for muon
     pmanager->AddProcess(new G4MuMultipleScattering(),-1, 1, 1);
     pmanager->AddProcess(new G4MuIonisation(),      -1, 2, 2);
     pmanager->AddProcess(new G4MuBremsstrahlung(),  -1, 3, 3);
     pmanager->AddProcess(new G4MuPairProduction(),  -1, 4, 4);

    } else {
      if ((particle->GetPDGCharge() != 0.0) &&
          (particle->GetParticleName() != "chargedgeantino") &&
          !particle->IsShortLived()) {
       // all others charged particles except geantino
       pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
       pmanager->AddProcess(new G4hIonisation(),       -1,2,2);
     }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4Threading.hh"

void CDEXPhysicsList::ConstructOp()
{
  fCerenkovProcess = new G4Cerenkov("Cerenkov");
  fCerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
  fCerenkovProcess->SetMaxBetaChangePerStep(10.0);
  fCerenkovProcess->SetTrackSecondariesFirst(true);

  fWLSProcess = new G4OpWLS("OpWLS");
  //fWLSProcess->UseTimeProfile();

  fScintillationProcess = new G4Scintillation("Scintillation");
  fScintillationProcess->SetScintillationYieldFactor(1.0);
  fScintillationProcess->SetScintillationExcitationRatio(0.0);//Line added by Luis
  fScintillationProcess->SetTrackSecondariesFirst(true);
  fScintillationProcess->SetVerboseLevel(OpVerbLevel);

  // scintillation process for alpha:
  G4Scintillation* theScintProcessAlpha = new G4Scintillation("Scintillation");
  // theScintProcessNuc->DumpPhysicsTable();
  theScintProcessAlpha->SetTrackSecondariesFirst(true);
  theScintProcessAlpha->SetScintillationYieldFactor(1.1);
  theScintProcessAlpha->SetScintillationExcitationRatio(1.0);
  theScintProcessAlpha->SetVerboseLevel(OpVerbLevel);

  // scintillation process for heavy nuclei
  G4Scintillation* theScintProcessNuc = new G4Scintillation("Scintillation");
  // theScintProcessNuc->DumpPhysicsTable();
  theScintProcessNuc->SetTrackSecondariesFirst(true);
  theScintProcessNuc->SetScintillationYieldFactor(0.2);
  theScintProcessNuc->SetScintillationExcitationRatio(1.0);
  theScintProcessNuc->SetVerboseLevel(OpVerbLevel);

  fAbsorptionProcess = new G4OpAbsorption();
  fRayleighScatteringProcess = new G4OpRayleigh();
  fMieHGScatteringProcess = new G4OpMieHG();
  fBoundaryProcess = new G4OpBoundaryProcess();

  fCerenkovProcess->SetVerboseLevel(fVerboseLevel);
  fScintillationProcess->SetVerboseLevel(OpVerbLevel);
  fAbsorptionProcess->SetVerboseLevel(OpVerbLevel);
  fRayleighScatteringProcess->SetVerboseLevel(fVerboseLevel);
  fMieHGScatteringProcess->SetVerboseLevel(fVerboseLevel);
  fBoundaryProcess->SetVerboseLevel(fVerboseLevel);

  // Use Birks Correction in the Scintillation process
  if(G4Threading::IsMasterThread())
  {
    G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
       fScintillationProcess->AddSaturation(emSaturation);
  }

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (fCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(fCerenkovProcess);
      pmanager->SetProcessOrdering(fCerenkovProcess,idxPostStep);
    }
    //if (fScintillationProcess->IsApplicable(*particle) && particle->GetParticleType() != "nucleus") {
    if (fScintillationProcess->IsApplicable(*particle)) {
      if(particle->GetParticleName() == "GenericIon") {
          pmanager->AddProcess(theScintProcessNuc); // AtRestDiscrete
          pmanager->SetProcessOrderingToLast(theScintProcessNuc,idxAtRest);
          pmanager->SetProcessOrderingToLast(theScintProcessNuc,idxPostStep);
        }
        else if(particle->GetParticleName() == "alpha") {
          pmanager->AddProcess(theScintProcessAlpha);
          pmanager->SetProcessOrderingToLast(theScintProcessAlpha,idxAtRest);
          pmanager->SetProcessOrderingToLast(theScintProcessAlpha,idxPostStep);
        }
        else {
          pmanager->AddProcess(fScintillationProcess);
          pmanager->SetProcessOrderingToLast(fScintillationProcess, idxAtRest);
          pmanager->SetProcessOrderingToLast(fScintillationProcess, idxPostStep);
       }
    }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(fAbsorptionProcess);
      pmanager->AddDiscreteProcess(fRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(fMieHGScatteringProcess);
      pmanager->AddDiscreteProcess(fBoundaryProcess);
      pmanager->AddDiscreteProcess(fWLSProcess);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXPhysicsList::SetVerbose(G4int verbose)
{
  fVerboseLevel = verbose;

  fCerenkovProcess->SetVerboseLevel(fVerboseLevel);
  fScintillationProcess->SetVerboseLevel(OpVerbLevel);
  fAbsorptionProcess->SetVerboseLevel(fVerboseLevel);
  fRayleighScatteringProcess->SetVerboseLevel(fVerboseLevel);
  fMieHGScatteringProcess->SetVerboseLevel(fVerboseLevel);
  fBoundaryProcess->SetVerboseLevel(fVerboseLevel);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXPhysicsList::SetNbOfPhotonsCerenkov(G4int MaxNumber)
{
  fMaxNumPhotonStep = MaxNumber;

  fCerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CDEXPhysicsList::SetCuts()
{
	//  " G4VUserPhysicsList::SetCutsWithDefault" method sets
	//   the default cut value for all particle types
	//
	//SetCutsWithDefault();
	//special for low energy physics
    //G4double lowlimit = 250 * eV;
	G4double LowerLimit = 25 * eV;
	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(LowerLimit, 100. * GeV);

	// set cut values for gamma at first and for e- second and next for e+,
	// because some processes for e+/e- need cut values for gamma 
	SetCutValue(cutForGamma, "gamma");
	SetCutValue(cutForElectron, "e-");
	SetCutValue(cutForPositron, "e+");


//  if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
*/


//SAGe Version
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmExtraPhysics.hh"
#include "G4GenericBiasingPhysics.hh"

#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4RadioactiveDecay.hh"
#include "G4SystemOfUnits.hh"
#include "Shielding.hh"


#include "CDEXPhysicsList.hh"
#include "CDEXTritiumPhysics.hh"
#include "CDEXOpticalPhysics.hh"
#include "G4OpticalPhysics.hh"

// guide for different physics lists
// http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/TrackingAndPhysics/physicsProcess.html

CDEXPhysicsList::CDEXPhysicsList() : G4VModularPhysicsList()
{
    SetVerboseLevel(1);
    // Default physics
    RegisterPhysics(new G4DecayPhysics());
    // EM physics (low energy option 3)
    RegisterPhysics(new G4EmStandardPhysics_option3());
    // Synchroton Radiation & GN Physics
    RegisterPhysics(new G4EmExtraPhysics());
    // Hadronic physics
    RegisterPhysics(new G4HadronPhysicsFTFP_BERT_HP());
    // Hadronic Elastic
    RegisterPhysics(new G4HadronElasticPhysics());
    // Stopping and ion physics
    RegisterPhysics(new G4StoppingPhysics());
    RegisterPhysics(new G4IonPhysics());
    // // Radio active decay physics and add user defined data base
    // auto radioactiveDecayContainer = new G4RadioactiveDecay();
    // G4int Z = 1;
    // G4int A = 3;
    // G4String file_name = "../data/Z1.A3";
    // // const char* nv = (const char*)file_name;
    // radioactiveDecayContainer->AddUserDecayDataFile(Z, A, file_name);
    RegisterPhysics(new G4RadioactiveDecayPhysics());


    //Customized Physics
    RegisterPhysics(new CDEXTritiumPhysics());
    RegisterPhysics(new CDEXOpticalPhysics());
    //RegisterPhysics(new G4OpticalPhysics());

    // Add biasing to physical list
    G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
    biasingPhysics->NonPhysicsBias("gamma");
    // biasingPhysics->NonPhysicsBias("neutron");
    // biasingPhysics->Bias("alpha");
    RegisterPhysics(biasingPhysics);
    SetCutValue(100 * micrometer, "alpha");
    SetCutValue(100 * micrometer, "gamma");
    SetCutValue(100 * micrometer, "e-");
    SetCutValue(100 * micrometer, "e+");
    std::cout << "INFO: Construct the Physical List register !" << std::endl;
}

CDEXPhysicsList::~CDEXPhysicsList()
{
    std::cout << "INFO: Deconstruct the Physical List register !" << std::endl;
}
