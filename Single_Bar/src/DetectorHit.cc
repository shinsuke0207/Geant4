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
// $Id: DetectorHit.cc 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file DetectorHit.cc
/// \brief Implementation of the DetectorHit class

#include "DetectorHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::DetectorHit()
  : G4VHit(),
    fTrackID(0)/*, fParentID(0), fPos(0), fWL(0),
		 fPDGCode(0)*/
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::~DetectorHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::DetectorHit(const DetectorHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  //fPos       = right.fPos;
  // fWL      = right.fWL;
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const DetectorHit& DetectorHit::operator=(const DetectorHit& right)
{
  fTrackID   = right.fTrackID;
  //fPos       = right.fPos;
  // fWL      = right.fWL;
  // fParentID      = right.fParentID;
  // fPDGCode      = right.fPDGCode;
  

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int DetectorHit::operator==(const DetectorHit &/*right*/) const
{
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorHit::Draw()
{
  /*  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4Circle circle(fPos);
      circle.SetScreenSize(4.);
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(1.,0.,0.);
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
      }*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* DetectorHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("DetectorHit",isNew);

  if (isNew) {
    (*store)["HitType"]
      = G4AttDef("HitType","Hit Type","Physics","","G4String");

    // (*store)["Pos"]
    //   = G4AttDef("Pos","Position","Physics","G4BestUnit","G4ThreeVector");

    (*store)["Track"]
      = G4AttDef("Track","Track ID","Physics","","G4int");
    // (*store)["Parent"]
    //   = G4AttDef("Parent","ParentID","Physics","G4BestUnit","G4double");
    // (*store)["WL"]
    //   = G4AttDef("WL","WL","Physics","G4BestUnit","G4double");
    // (*store)["PDGCode"]
    //   = G4AttDef("PDGCode","PDGCode","Physics","G4BestUnit","G4double");   
    
  }
  return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* DetectorHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

    values
      ->push_back(G4AttValue("HitType","DetectorHit",""));
    //values
    //   ->push_back(G4AttValue("Pos",G4BestUnit(fPos,"Length"),""));
    values
      ->push_back(G4AttValue("Track",fTrackID,""));
    // values
    //   ->push_back(G4AttValue("Parent",G4BestUnit(fParentID,"Parent"),""));
    // values
    //   ->push_back(G4AttValue("PDGCode",G4BestUnit(fPDGCode,"PDGCode"),""));
    // values
    //   ->push_back(G4AttValue("WL",G4BestUnit(fWL,"WL"),""));  
    

      return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorHit::Print()
{
    G4cout
    << "  trackID: " << fTrackID
    // << " Position: "
    // << std::setw(7) << G4BestUnit( fPos,"Length")
    // << " Parent: "
    // << std::setw(7) << G4BestUnit( fParentID,"Parent" )
    << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
