#include <TFile.h>   // ROOTファイルを使用
#include <TH1D.h>
#include <TH1.h>     // ヒストグラムを使用
#include <TRandom.h> // 乱数を使用
#include <TCanvas.h> // キャンバス
#include <TApplication.h> // アプリケーションオブジェクト
#include <TTree.h>
#include <numeric>

#include <iostream>

using namespace std;

int count_unique(const vector<int> &vec);
  
int main(int argc, char* argv [])
{
  TFile *f = TFile::Open("result.root");
  vector<double> mu_length;
  double length = 0;
  vector<double> lengths;

  vector<int> eventIDs;
  vector<int> TrackIDs;
  vector<int> ParentIDs;
  vector<int> ParentCodes;
  vector<int> PDGCodes;
  vector<double> Pos_xs;
  vector<double> Pos_ys;
  vector<double> Pos_zs;      
  vector<double> WaveLengths;
  vector<double> Energys;
  vector<double> StLengths;

  vector<int> eleTrackIDs;
  vector<int> numele;
  
  Int_t eventID=0;    //ntupleからデータをとってくる変数を定義    
  Int_t TrackID=0;
  Int_t ParentID=0;
  Int_t ParentCode=0;
  Int_t PDGCode=0;
  Double_t Pos_x=0;    //ntupleからデータをとってくる変数を定義
  Double_t Pos_y=0;    //ntupleからデータをとってくる変数を定義
  Double_t Pos_z=0;    //ntupleからデータをとってくる変数を定義
  Double_t WaveLength=0;
  Double_t Energy=0;
  Double_t StLength=0;
  TTree *tr = (TTree*)f->Get("nt1");
  tr->SetBranchAddress("eventID",&eventID);
  tr->SetBranchAddress("TrackID",&TrackID);
  tr->SetBranchAddress("ParentID",&ParentID);
  tr->SetBranchAddress("ParentCode",&ParentCode);
  tr->SetBranchAddress("PDGCode",&PDGCode);
  tr->SetBranchAddress("Pos_x",&Pos_x);
  tr->SetBranchAddress("Pos_y",&Pos_y);
  tr->SetBranchAddress("Pos_z",&Pos_z);
  tr->SetBranchAddress("WaveLength",&WaveLength);
  tr->SetBranchAddress("Energy",&Energy);
  tr->SetBranchAddress("StLength",&StLength);
  
  for(Int_t i = 0;i < tr->GetEntries();i++){
    tr->GetEntry(i);
    eventIDs.push_back(eventID);
    TrackIDs.push_back(TrackID);
    ParentIDs.push_back(ParentID);
    ParentCodes.push_back(ParentCode);
    PDGCodes.push_back(PDGCode);
    Pos_xs.push_back(Pos_x);
    Pos_ys.push_back(Pos_y);
    Pos_zs.push_back(Pos_z);
    WaveLengths.push_back(WaveLength);
    Energys.push_back(Energy);
    StLengths.push_back(StLength);
    cout << 100.*i/tr->GetEntries() << "[%] " << "input done" <<  endl;
  }
  sort(eventIDs.begin(), eventIDs.end());
  //  eventIDs.erase(unique(eventIDs.begin(), eventIDs.end()), eventIDs.end());
  
  // for(Int_t j = 0;j < eventIDs.size();j++){
  //   for(Int_t i = j;i < tr->GetEntries();i++){
  //     tr->GetEntry(i);
  //     if(i == j){
  // 	lengths.clear();
  //     }
  //     else if(eventIDs[i] != eventIDs[i-1]){
  // 	break;
  //     }
  //     if(PDGCode == 13) length += StLengths[i];
  //   }
  //   lengths.push_back(length/10);
  //   cout << lengths[j] << endl;
  // }
  Int_t i;
  // for(j = 0;j < tr->GetEntries();j++){
  //   for(i = j;i < tr->GetEntries();i++){
  //     if(eventIDs[i] ==  j){
  //     	length = 0.;
  //     }
  //     if(eventIDs[i] != eventIDs[i-1]){
  // 	break;
  //     }
  //     if(PDGCodes[i] == 13) length += StLengths[i];
  //   }
  //   eventIDs.erase(eventIDs.begin() + 1, eventIDs.begin() + i+1);
  //   lengths.push_back(length/10);
  //   cout << lengths[j] << endl;
  //   if(j == back) break;
  // }
  
  if(PDGCodes[0] == 13){
    length = StLengths[0];
  }
  
  for(i = 1;i <= tr->GetEntries();i++){    
    if(eventIDs[i] != eventIDs[i-1] ){
      lengths.push_back(length/10);
      numele.push_back(count_unique(eleTrackIDs));
		       //      cout << "[" << i << "] " << length/10  << " " << count_unique(eleTrackIDs) << endl;
      length = 0.;
      eleTrackIDs.clear();
    }
    if(PDGCodes[i] == 13){
      length += StLengths[i];
    }
    else if(PDGCodes[i] == 11){
      eleTrackIDs.push_back(TrackIDs[i]);
    }
  }
  for(i=0;i<numele.size();i++){
    cout << "[" << i << "] " << lengths[i]  << " " << numele[i] << endl;
  }

  cout << "avg = " << accumulate(lengths.begin(), lengths.end(), 0.) / lengths.size()  << "[cm]," << accumulate(numele.begin(), numele.end(), 0.) / numele.size() << " particles" << endl;
  
  //  TCanvas *c1 = new TCanvas("CerenkovDistribution","",600,600);
  //nt1->Draw("Pos_y:Pos_x >> h1(40,-500,500,40,-500,500)","ParentCode == 13 && WaveLength <= 650.*10**(-6) && WaveLength >= 300.*10**(-6)","colz");
  //h1->GetXaxis()->SetTitle("x[mm]");
  //h1->GetYaxis()->SetTitle("y[mm]");
  // h1->GetYaxis()->SetTitleOffset(1.4);//Y軸タイトルの位置調整
  // gStyle->SetOptTitle(0);
  // gStyle->SetOptStat(0);
  // gPad->Update();

  // TCanvas *c1 = new TCanvas("CerenkovDistribution","",600,600);
  // nt1->Draw("Photon_y:Photon_x >> h1(40,-500,500,40,-500,500)","ParentID == 1 &&  WaveLength <= 650.*10**(-6) && WaveLength >= 300.*10**(-6) && Photon_z == -100","colz");
  // h1->GetXaxis()->SetTitle("x[mm]");
  // h1->GetYaxis()->SetTitle("y[mm]");
  // h1->GetYaxis()->SetTitleOffset(1.4);//Y軸タイトルの位置調整
  // gStyle->SetOptTitle(0);
  // gStyle->SetOptStat(0);
  // gPad->Update();
  // TPaletteAxis *palette = (TPaletteAxis*)h1->GetListOfFunctions()->FindObject("palette");
  // palette->SetX1NDC(1);
  // palette->SetX2NDC(10); //位置を変えたり、縮めたり
  return 0;
};

int count_unique(const vector<int> &vec){
    vector<int> v;
    v = vec;
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return v.size();
  }

