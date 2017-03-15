#ifndef CerenkovEventAction_h
#define CerenkovEventAction_h 1

#include "G4UserEventAction.hh"

#include "G4ThreeVector.hh"
#include "globals.hh"
#include <vector>

/// Event action class

class CerenkovEventAction : public G4UserEventAction
{
public:
  CerenkovEventAction();
  virtual ~CerenkovEventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);

  G4int eventID;
private:
  //水だけ、アクリル用-----------------------------------
  G4int fDHC1ID;
  //-----------------------------------------------------

  std::vector<G4int> electronTrackID;
  std::vector<G4int> eleTrackID;
  std::vector<G4int> muTrackID;
  std::vector<G4int> TrackID;

  G4int nOfelectron;
  
  G4int nOfbottom;
  G4int elenOfbottom;
  G4int munOfbottom;  

  G4int nOfPhoton;
  G4int elenOfPhoton;
  G4int munOfPhoton;



  G4int nOftopRefle;
  G4int elenOftopRefle;
  G4int munOftopRefle;  
  
  G4double StLength;
  std::vector<G4ThreeVector> Pos_mu;
  G4ThreeVector Pos_esc_mu;

  G4double Pos_esc_mu_x;
  G4double Pos_esc_mu_y;
  G4double Pos_esc_mu_z;


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
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
