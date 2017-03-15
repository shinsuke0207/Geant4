
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: DetectorHit.hh 69505 2013-05-07 01:57:27Z asaim $
//
/// \file DetectorHit.hh
/// \brief Definition of the DetectorHit class

#ifndef DetectorHit_h
#define DetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class G4AttDef;
class G4AttValue;

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

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

      
  // G4int GetID() const { return fId; }
  
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
  
  // void SetParentID  (G4int parent)      { fParentID = parent; };  
  // void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
  // void SetWL  (G4double wl)    { fWL = wl; };
  // void SetPDGCode  (G4double pdgcode)    { fPDGCode = pdgcode; };
  // //  void SetPos (G4ThreeVector pos_from_electron)    { fPos_from_electron = pos_from_electron; };
  // void SetPos_from_mu (G4ThreeVector pos_from_mu)    { fPos_from_mu = pos_from_mu; };
  // void SetTime (G4double val)    { fTime = val; };    
  // void SetParentPDG (G4int parentpdg) {fParentPDG = parentpdg;};
  // void SetEnergy (G4double energy) {fEnergy = energy;};
  // // void SetStLength (G4double stlength) {fstlength = stlength;};
  // void SetBounday (G4int boundary) {fboundary = boundary;};


  // //void SetProcess (G4String process) {fprocess = process;};      
  // // Get methods

  // G4int GetParentID() const     { return fParentID; };  
  // G4ThreeVector GetPos() const { return fPos; };
  // G4double GetWL() const  { return fWL; };
  // G4int GetPDGCode() const { return fPDGCode;};
  // G4ThreeVector GetPos_from_electron() const { return fPos_from_electron;};
  // G4ThreeVector GetPos_from_mu() const { return fPos_from_mu;};
  // G4double GetEnergy() const { return fEnergy;};
  // // G4double GetStLength() const { return fstlength;};
  // G4double GetTime() const { return fTime;};    
  
  // G4int GetParentPDG() const { return fParentPDG;};
  // G4int GetBoundary() const { return fboundary;};
  //G4String GetProcess() const { return fprocess;};


private:
  G4int fTrackID;
  G4String fProcessName;
  G4ThreeVector fPosition;
  G4double fStlength;
  G4String fpName;
  G4int fboundarystatus;  
  G4int         fParentID;  
  // G4int fId;

  // G4int         fParentID;  
  // G4ThreeVector fPos;
  // G4double      fWL;
  // G4double fEnergy;

  // G4int fPDGCode;
  // // G4String fpName;
  // G4ThreeVector fPos_from_electron;
  // G4ThreeVector fPos_from_mu;


  // G4int fParentPDG;
  // G4double fTime;
  // G4int fboundary;
  //G4String fprocess;

  
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
