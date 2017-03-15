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
// $Id: ACerenkovDetectorConstruction.hh 73722 2013-09-09 10:23:05Z gcosmo $
//
/// \file ACerenkovDetectorConstruction.hh
/// \brief Definition of the ACerenkovDetectorConstruction class

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

/// Detector construction class to define materials, geometry
/// and global uniform magnetic field.

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
