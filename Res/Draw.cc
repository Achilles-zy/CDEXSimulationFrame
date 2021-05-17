#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
//#include ""
//#ifndef ROOTHEADERS
//#define ROOTHEADERS
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TChain.h"
//#endif
//#include "misc.h"
//#include "tanh_fit.h"
using namespace std;
//Char_t filename[100] = "./6/CDEXBucket_BucketCrystal_K42.root";
Char_t filename[100] = "./6/CDEXBucket_Wire_A1_Tl208.root";

void Draw() {
	TH2* XYPlane = new TH2D("XYPlane", "Position Result", 150, -750, 750, 150, -750, 750);
    TH2* XYPlaneElectron = new TH2D("XYPlaneElectron", "Electorn Result", 150, -750, 750, 150, -750, 750);
    TH2* XYPlaneGamma = new TH2D("XYPlaneGamma", " Gamma Result", 150, -750, 750, 150, -750, 750);
    TH2* XYPlaneElectronZoomed = new TH2D("XYPlaneElectron", "Electorn Result", 150, 44, 48, 150, -2, 2);
    TH2* XYPlaneWeighted = new TH2D("XYPlaneWeighted", "Edep Result", 150, -750, 750, 150, -750, 750);
    TH2* XYPlaneElectronWeighted = new TH2D("XYPlaneElectronWeighted", "Electorn Result", 150, -750, 750, 150, -750, 750);
    TH2* XYPlaneGammaWeighted = new TH2D("XYPlaneGammaWeighted", "Gamma Result", 150, -750, 750, 150, -750, 750);
    TH2* XYPlaneElectronWeightedZoomed = new TH2D("XYPlaneElectronWeighted", "Electorn Result", 150, 44, 48, 150, -2, 2);
	TH3* XYZVolume = new TH3D("XYZVolume", "Result", 150, -750, 750, 150, -750, 750, 400, -2000, 2000);
    TH3* XYZVolumeElectron = new TH3D("XYZVolumeElectron", "Electron Result", 150, -750, 750, 150, -750, 750, 400, -2000, 2000);
    TH3* XYZVolumeGamma = new TH3D("XYZVolumeGamma", "Gamma Result", 150, -750, 750, 150, -750, 750, 400, -2000, 2000);

    TFile* file;
    file = new TFile(filename);

	//   TTree* EdepEnv = (TTree*)file->Get("EdepEnv");
    //   TTree* EdepEnvVeto = (TTree*)file->Get("EdepEnvVeto");
    //   TTree* EdepArgon = (TTree*)file->Get("EdepArgon");
    //   TTree* EdepArgonVeto = (TTree*)file->Get("EdepArgonVeto");

    TNtuple* EdepEnv = (TNtuple*)file->Get("EdepEnv");
    TNtuple* EdepEnvVeto = (TNtuple*)file->Get("EdepEnvVeto");
    TNtuple* EdepArgon = (TNtuple*)file->Get("EdepArgon");
    TNtuple* EdepArgonVeto = (TNtuple*)file->Get("EdepArgonVeto");

	Int_t NEdepEnv = (Int_t)EdepEnv->GetEntries();
    Int_t NEdepEnvVeto = (Int_t)EdepEnvVeto->GetEntries();
    Int_t NEdepArgon = (Int_t)EdepArgon->GetEntries();
    Int_t NEdepArgonVeto = (Int_t)EdepArgonVeto->GetEntries();

    Double_t posx, posy, posz, edep;
    Int_t creatorprocess, particletype;

    TTree* WorkingTree = EdepArgonVeto;
    Int_t NEntries = (Int_t)WorkingTree->GetEntries();

    WorkingTree->SetBranchAddress("ParticleType", &particletype);
    WorkingTree->SetBranchAddress("CreatorProcess", &creatorprocess);
    WorkingTree->SetBranchAddress("PosX", &posx);
    WorkingTree->SetBranchAddress("PosY", &posy);
    WorkingTree->SetBranchAddress("PosZ", &posz);
    WorkingTree->SetBranchAddress("Edep", &edep);

    Double_t SliceThickness = 2000;//mm

    Double_t EdepTotal = 0;//mm
    Double_t ReadoutRadius = 400;//mm
    Double_t EdepRecorded = 0;//mm

    Double_t ReadoutRadiusGroup[6] = { 100,200,300,400,500,600 };
    Double_t EdepRecordedGroup[7] = {};

    for (Int_t i = 0; i < NEntries; i++) {
        WorkingTree->GetEntry(i);

        Double_t CentX = 0;
        Double_t CentY = 0;
        Double_t radius = sqrt((posx - CentX) * (posx - CentX) + (posy - CentY) * (posy - CentY));

        if (abs(posz - 40) < SliceThickness / 2) {
            XYPlane->Fill(posx, posy);
            EdepTotal += edep;
            if (radius < ReadoutRadius) {
                EdepRecorded += edep;
            }
            if (radius < ReadoutRadiusGroup[0]) {
                EdepRecordedGroup[0] += edep;
            }
            if (radius < ReadoutRadiusGroup[1]) {
                EdepRecordedGroup[1] += edep;
            }
            if (radius < ReadoutRadiusGroup[2]) {
                EdepRecordedGroup[2] += edep;
            }
            if (radius < ReadoutRadiusGroup[3]) {
                EdepRecordedGroup[3] += edep;
            }
            if (radius < ReadoutRadiusGroup[4]) {
                EdepRecordedGroup[4] += edep;
            }
            if (radius < ReadoutRadiusGroup[5]) {
                EdepRecordedGroup[5] += edep;
            }

        }
        if (particletype == 1 && abs(posz - 40) < SliceThickness / 2) {
			XYPlaneGamma->Fill(posx, posy);
		}
		else if (particletype == 2 && abs(posz - 40) < SliceThickness / 2) {
			XYPlaneElectron->Fill(posx, posy);
            XYPlaneElectronZoomed->Fill(posx, posy);
        }

        if (abs(posz - 40) < SliceThickness / 2) {
            XYPlaneWeighted->Fill(posx, posy, edep);
        }
        if (particletype == 1 && abs(posz - 40) < SliceThickness / 2) {
            XYPlaneGammaWeighted->Fill(posx, posy, edep);
        }
        else if (particletype == 2 && abs(posz - 40) < SliceThickness / 2) {
            XYPlaneElectronWeighted->Fill(posx, posy, edep);
            XYPlaneElectronWeightedZoomed->Fill(posx, posy, edep);
        }

        XYZVolume->Fill(posx, posy, posz);
        if (particletype == 1) {
            XYZVolumeGamma->Fill(posx, posy, posz);
        }
        else if (particletype == 2) {
            XYZVolumeElectron->Fill(posx, posy, posz);
        }
    }

    cout << "RecordingEff =" << EdepRecorded / EdepTotal << endl;

    Char_t drawoption[100] = "";
    Float_t xMin = 0.67, yMin = 0.875, xMax = 0.85, yMax = 0.95; //设置标签绘制位置
    Int_t CanColor = 17;
    auto Canvas1 = new TCanvas("PosDist", "Position Distribution", 10, 10, 700, 700); //创建画板
    Canvas1->Divide(2, 2); //将画板横纵分别分为2块
    Canvas1->SetFillColor(CanColor);
    Canvas1->cd(1);
    XYPlane->Draw(drawoption);
    Canvas1->cd(2);
    XYPlaneGamma->Draw(drawoption);
    Canvas1->cd(3);
    XYPlaneElectron->Draw(drawoption);
    Canvas1->cd(4);
    XYPlaneElectronZoomed->Draw(drawoption);

    auto Canvas2 = new TCanvas("EDepDist", "Energy Deposition Distribution", 10, 10, 700, 700); //创建画板
    Canvas2->Divide(2, 2); //将画板横纵分别分为2块
    Canvas2->SetFillColor(CanColor);
    Canvas2->cd(1);
    //XYPlaneWeighted->Draw("contz");
    XYPlaneWeighted->Draw(drawoption);
    Canvas2->cd(2);
    XYPlaneGammaWeighted->Draw(drawoption);
    Canvas2->cd(3);
    XYPlaneElectronWeighted->Draw(drawoption);
    Canvas2->cd(4);
    XYPlaneElectronWeightedZoomed->Draw(drawoption);

    auto Canvas3 = new TCanvas("Canvas3", "Canvas3", 10, 10, 700, 700); //创建画板
    Canvas3->Divide(2, 2); //将画板横纵分别分为2块
    Canvas3->SetFillColor(CanColor);
    Canvas3->cd(1);
    XYZVolume->Draw();
    Canvas3->cd(2);
    XYZVolumeGamma->Draw();
    Canvas3->cd(3);
    XYZVolumeElectron->Draw();

    //auto Canvas4 = new TCanvas("Canvas4", "Energy Deposition Distribution", 10, 10, 700, 700); //创建画板
    //XYPlaneGamma->Draw(drawoption);


	auto CanvasLine = new TCanvas("CanvasLine", "Energy Coverage", 200, 10, 500, 300);
	Int_t n = 6;
	auto gr = new TGraph(n, ReadoutRadiusGroup, EdepRecordedGroup);
	gr->Draw("AC*");
    
}
