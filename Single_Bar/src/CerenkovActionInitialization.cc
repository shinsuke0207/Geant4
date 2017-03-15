#include "CerenkovActionInitialization.hh"
#include "CerenkovPrimaryGeneratorAction.hh"
#include "CerenkovRunAction.hh"
#include "CerenkovEventAction.hh"
#include "CerenkovSteppingAction.hh"
#include "SteppingVerbose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovActionInitialization::CerenkovActionInitialization()
  : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovActionInitialization::~CerenkovActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovActionInitialization::BuildForMaster() const
{
  CerenkovEventAction* eventAction = new CerenkovEventAction;
  SetUserAction(new CerenkovRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovActionInitialization::Build() const
{
  SetUserAction(new CerenkovPrimaryGeneratorAction);
  CerenkovEventAction* eventAction = new CerenkovEventAction;
  SetUserAction(new CerenkovRunAction(eventAction));
  SetUserAction(new CerenkovSteppingAction);  
  SetUserAction(eventAction);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
