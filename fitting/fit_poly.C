#include <TSystem.h>
#include <TString.h>
#include "TFile.h"
#include "TTree.h"
#include <TNtuple.h>
#include "TCanvas.h"
#include <iostream>
#include <fstream>
#include "TMath.h"
#include "TH1F.h"
#include <TF1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TROOT.h>
#include <TMath.h>
#include <TLegend.h>
#include <TPaveLabel.h>
#include <TProfile.h>
#include <TPolyLine.h>
#include <TObjArray.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include<math.h>
#include <TRandom3.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompSVD.h>
using namespace std;

void fit_poly() {
 const static Int_t npar=3;
 const static Int_t nfit_max=10000;
 const static Int_t nv_max=10000;
  TVectorD B(npar);
  TMatrixD lambda(npar,nfit_max);
  TMatrixD C(npar,npar);
  TMatrixD V(npar,npar);
  TMatrixD U(npar,npar);
  TVectorD S(npar);
  TMatrixD Cnew(npar,npar);
  Double_t v_xval[nfit_max];
  Double_t v_yval[nfit_max];

  TF1 *fpoly = new TF1("fpoly","[0]+[1]*x+[2]*x*x",0,10);
  TF1 *fpoly2 = new TF1("fpoly2","[0]+[1]*x+[2]*x*x",0,10);

  TH2F *hpoly = new TH2F("hpoly"," ;Y;X",100,0,10,100,0,500);

  fpoly->SetParameters(3,5,7);
  TRandom *r3 = new TRandom3();
  TRandom *r2 = new TRandom3();
  Int_t nfit=0;
  for( Int_t nv=0; nv<nv_max; nv++ ){
    Double_t xval = r3->Uniform(0.,10.);
    Double_t yval = fpoly->Eval(xval) + 2.*r2->Gaus(0.,1.);
             if ( xval > 0 && xval < 10.1 && nfit < nfit_max) {
                 hpoly->Fill(xval,yval);
                 for( Int_t icoeff_fit=0; icoeff_fit<npar; icoeff_fit++ ){
		      Double_t etemp= pow( xval, icoeff_fit); 
                      lambda[icoeff_fit][nfit] = etemp;
	              B[icoeff_fit] += (yval) * etemp;
 	             } // for icoeff_fit loop
		 v_xval[nfit] = xval;
		 v_yval[nfit] = yval;
	          nfit++;
	     }
  }
  //
  hpoly->Draw();
  hpoly->Fit("fpoly");
  //
for(Int_t i=0; i<npar; i++){
    for(Int_t j=0; j<npar; j++){
      C[i][j] = 0.0;
    }
 }
//
   for( Int_t ifit=0; ifit<nfit; ifit++){
    for( Int_t ipar=0; ipar<npar; ipar++){
      for( Int_t jpar=0; jpar<npar; jpar++){
      	C[ipar][jpar] += lambda[ipar][ifit] * lambda[jpar][ifit];
       }
    }
  }
//
   TDecompSVD Ay_svd(C);
  bool ok;
  ok = Ay_svd.Solve( B );
  // After Solve the B matrix has the coefficents
  cout << "solution ok = " << ok << endl;
  cout << " solution" << endl;
  B.Print();
  U=Ay_svd.GetU();
  //cout << " U matrix" << endl;
  //U.Print();
  V=Ay_svd.GetV();
  //cout << " V matrix " << endl;
  //V.Print();
  S=Ay_svd.GetSig();
  //cout << " S matrix " << endl;
  // S.Print();
  fpoly2->SetParameters(B(0),B(1),B(2));
  fpoly2->Draw("same");
  fpoly2->SetLineColor(4);
  //
}
