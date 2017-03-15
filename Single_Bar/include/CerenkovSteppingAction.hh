#ifndef CerenkovSteppingAction_h
#define CerenkovSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class CerenkovEventAction;

class G4LogicalVolume;

class G4OpBoundaryProcess;
/// Stepping action class
/// 

class CerenkovSteppingAction : public G4UserSteppingAction
{
  public:
  CerenkovSteppingAction(/*CerenkovEventAction* eventAction*/);
    virtual ~CerenkovSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
  G4OpBoundaryProcess* fOpProcess;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
