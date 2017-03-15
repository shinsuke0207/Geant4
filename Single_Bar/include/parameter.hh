/*
 * parameter.hh
 *
 *  Created on: 2016/04/15
 *      Author: katsumata
 */

#ifndef INCLUDE_PARAMETER_HH_
#define INCLUDE_PARAMETER_HH_


// const G4double WorldLength_X = 0.75*m;
// const G4double WorldLength_Y = 0.75*m;
// const G4double WorldLength_Z = 0.75*m;

const G4double WorldLength_X = 0.75*m;
const G4double WorldLength_Y = 0.75*m;
const G4double WorldLength_Z = 0.75*m;

const G4double AcrylLength_X = 5*mm;
const G4double AcrylLength_Y = 0.5*m;
const G4double AcrylLength_Z = 5*mm;

const G4double absBox_X = AcrylLength_X;
const G4double absBox_Y = 0.1*mm;
const G4double absBox_Z = AcrylLength_Z;

const G4String fileName = "result"; //作成するrootファイルの名前 （拡張子省略）

#endif /* INCLUDE_PARAMETER_HH_ */
