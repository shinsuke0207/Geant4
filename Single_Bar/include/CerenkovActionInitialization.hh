#ifndef CerenkovActionInitialization_h
#define CerenkovActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class CerenkovDetectorConstruction;
class G4VSteppingVerbose;


class CerenkovActionInitialization : public G4VUserActionInitialization
{
public:
  CerenkovActionInitialization();
  virtual ~CerenkovActionInitialization();
  
  virtual void BuildForMaster() const;
  virtual void Build() const;
  
  virtual G4VSteppingVerbose* InitializeSteppingVerbose() const;
};

#endif

    
