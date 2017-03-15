#include "CerenkovSteppingAction.hh"
#include "CerenkovEventAction.hh"
#include "ACerenkovDetectorConstruction.hh"
#include "CerenkovAnalysis.hh"
#include "CerenkovRunAction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "G4ParticleTypes.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include "G4OpBoundaryProcess.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovSteppingAction::CerenkovSteppingAction(/*CerenkovEventAction* eventAction*/)
  : G4UserSteppingAction()
{
  fOpProcess = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovSteppingAction::~CerenkovSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4Track* track = aStep->GetTrack();
  G4StepPoint* point2 = aStep->GetPostStepPoint();
  G4ThreeVector Position = point2->GetPosition();

  //ミューオン、反ミューオン、電子、陽電子、光子以外の粒子は無視する
  if (track->GetDefinition() != G4MuonPlus::MuonPlusDefinition() &&
      track->GetDefinition() != G4MuonMinus::MuonMinusDefinition() &&
      track->GetDefinition() != G4Electron::ElectronDefinition() &&
      track->GetDefinition() != G4Positron::PositronDefinition() &&
      track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()
      ) track->SetTrackStatus(fKillTrackAndSecondaries);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
