#include "CDEXTritiumPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4Triton.hh"
#include "G4VProcess.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4RadioactiveDecay.hh"
#include "G4SystemOfUnits.hh"
#include "G4NuclideTable.hh"

CDEXTritiumPhysics::CDEXTritiumPhysics() : G4VPhysicsConstructor()
{
    // mandatory for G4NuclideTable
    G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1 * picosecond);
    G4NuclideTable::GetInstance()->SetLevelTolerance(1.0 * eV);
    std::cout << "INFO: Construct the Tritium Decay Physics !" << std::endl;
}

CDEXTritiumPhysics::~CDEXTritiumPhysics()
{
    std::cout << "INFO: Deconstruct the Tritium Decay Physics !" << std::endl;
}

void CDEXTritiumPhysics::ConstructProcess()
{

    // Make tritium unstable
    G4ParticleDefinition* H3 = G4Triton::Definition();
    H3->SetPDGStable(false);

    // Remove G4Decay process, which requires a registered decay table
    G4VProcess* decay = 0;
    G4ProcessManager* processMan = H3->GetProcessManager();
    G4ProcessVector* processVec = processMan->GetAtRestProcessVector();
    for (unsigned int i = 0; i < processVec->size() && decay == 0; i++)
    {
        if ((*processVec)[i]->GetProcessName() == "Decay")
            decay = (*processVec)[i];
    }
    if (decay)
        processMan->RemoveProcess(decay);

    // Attach RDM, which is a rest-discrete process
    H3->GetProcessManager()->AddProcess(new G4RadioactiveDecay(), 1000, -1, 1000);
}

void CDEXTritiumPhysics::ConstructParticle()
{
    G4Triton::Definition();
}