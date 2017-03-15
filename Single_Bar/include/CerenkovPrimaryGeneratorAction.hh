#ifndef CerenkovPrimaryGeneratorAction_h
#define CerenkovPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

#include "DetectorHit.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class CerenkovPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  CerenkovPrimaryGeneratorAction();
  ~CerenkovPrimaryGeneratorAction();

   void GeneratePrimaries(G4Event* );

    const G4ParticleGun* GetParticleGun() const {return fParticleGun;}
  
  
  // Set methods
  void SetRandomFlag(G4bool );

private:
  DetectorHitsCollection* fHitsCollection;
  G4ParticleGun*          fParticleGun; // G4 particle gun
  G4Box* fWorldBox;
  G4double E;
  G4int eventID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
