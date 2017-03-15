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
// $Id: SensitiveVolume.hh 73722 2013-09-09 10:23:05Z gcosmo $
//
/// \file SensitiveVolume.hh
/// \brief Definition of the SensitiveVolume class
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SensitiveVolume.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

  // G4int return_parentPDGCode(G4int track){
  //   G4int p = 0;
  //   G4int PDG = 0;
  //   for(G4int i=0;i < (G4int)TrackIDs.size();i++){
  //     if(track == TrackIDs[i]){
  // 	p = ParentIDs[i];
  // 	break;
  //     }
  //   }
  //   for(G4int i=0;i < (G4int)TrackIDs.size();i++){
  //     if(p == TrackIDs[i])
  // 	{
  // 	  PDG = PDGCodes[i];
  // 	  break;
  // 	}
  //   }
  //   return PDG;
  // }

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
  //G4int nbOfbottom;
  
  DetectorHitsCollection* fHitsCollection;
  G4int fHCID;
  G4double  TrackID;
  G4double  ParentID;
  //G4double  PDGCode;
  // std::vector<G4String>  pNames;
  G4ThreeVector Position;
  G4double WL;
  G4double StLength;
  // G4double mu_length;
  // std::vector<G4int> Boundarys;  
  // std::vector<G4ThreeVector> pos_mu;
  // std::vector<G4ThreeVector> pos_ele;  
  // std::vector<G4int> PDGCodes;
  // std::vector<G4String> pNames;
  // std::vector<G4int> TrackIDs;
  // std::vector<G4int> ParentIDs;
  // std::vector<G4double> times;  
  // std::vector<G4double> Positions_x;
  // std::vector<G4double> Positions_y;
  // std::vector<G4double> Positions_z;
  // std::vector<G4double> wavelengths;
  // std::vector<G4ThreeVector> Positions;  
  // std::vector<G4ThreeVector> Positions_mu;
  // std::vector<G4ThreeVector> Positions_photon;  

  // std::vector<G4ThreeVector> Position_from_electron;
  // std::vector<G4ThreeVector> Position_from_muon;

  // std::vector<G4int> vec;

  // std::vector<G4ThreeVector> endOfPos;

  // std::vector<G4int>::iterator postitr;
  // std::vector<G4int>::iterator preitr;


  //G4OpBoundaryProcessStatus fExpectedNextStatus;

  //G4int nOfPhoton;


};
#endif
