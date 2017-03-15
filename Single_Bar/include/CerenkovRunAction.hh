#ifndef CerenkovRunAction_h
#define CerenkovRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class CerenkovEventAction;

class G4Run;

/// Run action class

class CerenkovRunAction : public G4UserRunAction
{
public:
  CerenkovRunAction(CerenkovEventAction* eventAction);
  virtual ~CerenkovRunAction();
  
  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);

  static G4int nbEventInRun;
private:

  
  CerenkovEventAction* fEventAction;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
