
#include "CDEXOpticalPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4RadioactiveDecay.hh"
#include "G4SystemOfUnits.hh"
#include "G4NuclideTable.hh"

#include "G4Threading.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"


#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

G4ThreadLocal G4int CDEXOpticalPhysics::fVerboseLevel = 0;
G4ThreadLocal G4int CDEXOpticalPhysics::fMaxNumPhotonStep = 20;
G4ThreadLocal G4Cerenkov* CDEXOpticalPhysics::fCerenkovProcess = 0;
G4ThreadLocal G4Scintillation* CDEXOpticalPhysics::fScintillationProcess = 0;
G4ThreadLocal G4OpAbsorption* CDEXOpticalPhysics::fAbsorptionProcess = 0;
G4ThreadLocal G4OpRayleigh* CDEXOpticalPhysics::fRayleighScatteringProcess = 0;
G4ThreadLocal G4OpMieHG* CDEXOpticalPhysics::fMieHGScatteringProcess = 0;
G4ThreadLocal G4OpBoundaryProcess* CDEXOpticalPhysics::fBoundaryProcess = 0;
G4ThreadLocal G4OpWLS* CDEXOpticalPhysics::fWLSProcess = 0;

CDEXOpticalPhysics::CDEXOpticalPhysics() : G4VPhysicsConstructor()
{
    // mandatory for G4NuclideTable
    OpVerbLevel = 0;
    G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1 * picosecond);
    G4NuclideTable::GetInstance()->SetLevelTolerance(1.0 * eV);
    std::cout << "INFO: Construct the Optical Physics !" << std::endl;
}

CDEXOpticalPhysics::~CDEXOpticalPhysics()
{
    std::cout << "INFO: Deconstruct the Optical Physics !" << std::endl;
}

void CDEXOpticalPhysics::ConstructProcess()
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
    if (G4Threading::IsMasterThread())
    {
        G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
        fScintillationProcess->AddSaturation(emSaturation);
    }
    
    auto particleIterator = GetParticleIterator();
    particleIterator->reset();

    while ((*particleIterator)()) {
        G4ParticleDefinition* particle = particleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        if (fCerenkovProcess->IsApplicable(*particle)) {
            pmanager->AddProcess(fCerenkovProcess);
            pmanager->SetProcessOrdering(fCerenkovProcess, idxPostStep);
        }
        //if (fScintillationProcess->IsApplicable(*particle) && particle->GetParticleType() != "nucleus") {
        if (fScintillationProcess->IsApplicable(*particle)) {
            if (particle->GetParticleName() == "GenericIon") {
                pmanager->AddProcess(theScintProcessNuc); // AtRestDiscrete
                //pmanager->SetProcessOrderingToLast(theScintProcessNuc, idxAtRest);
                //pmanager->SetProcessOrderingToLast(theScintProcessNuc, idxPostStep);
            }
            else if (particle->GetParticleName() == "alpha") {
                pmanager->AddProcess(theScintProcessAlpha);
                //pmanager->SetProcessOrderingToLast(theScintProcessAlpha, idxAtRest);
                //pmanager->SetProcessOrderingToLast(theScintProcessAlpha, idxPostStep);
            }
            else {
                pmanager->AddProcess(fScintillationProcess);
                //pmanager->SetProcessOrderingToLast(fScintillationProcess, idxAtRest);
                //pmanager->SetProcessOrderingToLast(fScintillationProcess, idxPostStep);
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



void CDEXOpticalPhysics::ConstructParticle()
{
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
