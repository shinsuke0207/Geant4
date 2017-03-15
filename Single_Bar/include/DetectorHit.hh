#ifndef DetectorHit_h
#define DetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class G4AttDef;
class G4AttValue;


class DetectorHit : public G4VHit
{
public:
  DetectorHit();
  DetectorHit(const DetectorHit &right);
  virtual ~DetectorHit();

  // operators
  const DetectorHit& operator=(const DetectorHit &right);
  G4int operator==(const DetectorHit &right) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*aHit);

  // methods from base class
  virtual void Draw();
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;
  virtual void Print();

  // Set methods
      
  void SetTrackID  (G4int track)      { fTrackID = track; };
  void SetProcessName  (G4String proName)      { fProcessName = proName; };
  void SetPosition  (G4ThreeVector position)      { fPosition = position; };
  void SetStLength  (G4double stlength)      { fStlength = stlength; };
  void SetpName  (G4String pName)      { fpName = pName; };
  void SetBoundaryStatus  (G4int boundarystatus)      { fboundarystatus = boundarystatus; };
  void SetParentID  (G4int parent)      { fParentID = parent; };  

  G4int GetTrackID() const     { return fTrackID; };
  G4String GetProcessName() const     { return fProcessName; };
  G4ThreeVector GetPosition() const     { return fPosition; };
  G4double GetStLength() const     { return fStlength; };
  G4String GetpName() const     { return fpName; };
  G4int GetBoundaryStatus() const { return fboundarystatus;};
  G4int GetParentID() const     { return fParentID; };  

private:
  G4int fTrackID;
  G4String fProcessName;
  G4ThreeVector fPosition;
  G4double fStlength;
  G4String fpName;
  G4int fboundarystatus;  
  G4int         fParentID;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

extern G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* DetectorHit::operator new(size_t)
{
  if(!DetectorHitAllocator){
    DetectorHitAllocator = new G4Allocator<DetectorHit>;
  }
  return (void *) DetectorHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorHit::operator delete(void* aHit)
{
  DetectorHitAllocator->FreeSingle((DetectorHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
