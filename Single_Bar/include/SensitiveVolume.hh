#ifndef SensitiveVolume_h
#define SensitiveVolume_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

#include "DetectorHit.hh"

#include <vector>

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4OpBoundaryProcess.hh"


class G4Step;
class G4HCofThisEvent;

//------------------------------------------------------------------------------
class SensitiveVolume : public G4VSensitiveDetector
//------------------------------------------------------------------------------
{
public:
  SensitiveVolume(G4String name);
  virtual ~SensitiveVolume();

  virtual void Initialize(G4HCofThisEvent* HCE);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history);
  virtual void EndOfEvent(G4HCofThisEvent* HCE);

  template <class FwdIter>
  inline size_t unique_count(FwdIter first, FwdIter last)
  {
    std::vector<typename FwdIter::value_type> uni;
    
    std::unique_copy(first, last, std::back_inserter(uni));
    
    return uni.size();
  }
  
  template <class FwdIter, class Pred>
  inline size_t unique_count(FwdIter first, FwdIter last, Pred pred)
  {
    std::vector<typename FwdIter::value_type> uni;
    
    std::unique_copy(first, last, std::back_inserter(uni), pred);
    
    return uni.size();
  }
  
private:
  DetectorHitsCollection* fHitsCollection;
  G4int fHCID;
  G4double  TrackID;
  G4double  ParentID;
  G4ThreeVector Position;
  G4double WL;
  G4double StLength;
};
#endif
