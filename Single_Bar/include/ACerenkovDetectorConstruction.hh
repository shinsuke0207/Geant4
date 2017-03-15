#ifndef ACerenkovDetectorConstruction_h
#define ACerenkovDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4GlobalMagFieldMessenger;
class UltraFresnelLens;


class ACerenkovDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ACerenkovDetectorConstruction();
    virtual ~ACerenkovDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
  
  virtual void ConstructSDandField();

  private:
    // methods
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
    UltraFresnelLens   *FresnelLens ;
  
    // data members     // pointer to the logical Target
  static G4ThreadLocal G4GlobalMagFieldMessenger* fMagFieldMessenger;

  //アクリル用---------------------------------
  G4LogicalVolume* fLogicAcrylD;
  G4VPhysicalVolume* acrylPV;
  G4LogicalVolume* fLogicAirBox;
  G4LogicalVolume* fLogicabsBox;
  G4LogicalVolume* fLogicabsBox_top;    
  G4VPhysicalVolume  *ConstructMirror(G4VPhysicalVolume *);

  G4LogicalVolume* logMirror;
  //-------------------------------------------
  G4double lambda_min ;
  G4double lambda_max ;

  G4bool  fCheckOverlaps;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
