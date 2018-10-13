#define merged_cxx
#include "merged.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

void merged::Loop(Int_t pdg)
{
   if (fChain == 0) return;
   TProfile* profile[12];
   // y vs v1, mc
   for(int i=0;i<3;i++)
   {
       Int_t b = i%3;
       profile[i] = new TProfile(Form("y vs v1, mc, centrality %i",b),Form("y vs v1, for %i, mc, centrality %i",pdg,b),8, -1, 1);
   }
   // y vs v1, mpd
   for(int i=3;i<6;i++)
   {
       Int_t b = i%3;
       profile[i] = new TProfile(Form("y vs v1, mpd, centrality %i",b),Form("y vs v1, for %i, mpd, centrality %i",pdg,b),8, -1, 1);
   }
   // pt vs v1, mc
   for(int i=6;i<9;i++)
   {
       Int_t b = i%3;
       profile[i] = new TProfile(Form("pt vs v1, mc, centrality %i",b),Form("pt vs v1, for %i, mc, centrality %i",pdg,b),8, 0.2, 2);
   }
   // pt vs v1, mpd
   for(int i=9;i<12;i++)
   {
       Int_t b = i%3;
       profile[i] = new TProfile(Form("pt vs v1, mpd, centrality %i",b),Form("pt vs v1, for %i, mpd, centrality %i",pdg,b),8, 0.2, 2);
   }
   
   Int_t N = fChain->GetEntries();
   for(int i=0;i<N;i++)
   {
       fChain->GetEntry(i);
       Float_t PsiEP = this->GetPsiEP();
       for(Int_t j=0; j<n_tracks_mc;j++) // mc-loop
       {
            if( !this->Selector(pdg,j,0) )
                continue;
            Float_t y = this->GetY(j,0);
            Float_t v1 = this->GetV1( atan2(py_mc[j],px_mc[j]), phiEP_mc );
            Float_t w=0;
            if(abs(y)>1.5)
                continue;
            if(y>0)
                w=1;
            if(y<0)
                w=-1;
            if(b_mc<5)
            {
                if(abs(y) > 0.2 )
                    profile[6]->Fill(pt_mc[j],w*v1);
                profile[0]->Fill(y,v1);
            }
            if( (b_mc>5)&&(b_mc<9) )
            {
                if(abs(y) > 0.2 )
                    profile[7]->Fill(pt_mc[j],w*v1);
                profile[1]->Fill(y,v1);
            }
            if( (b_mc>9)&&(b_mc<17) )
            {
                if(abs(y) > 0.2 )
                    profile[8]->Fill(pt_mc[j],w*v1);
                profile[2]->Fill(y,v1);
            }
       }
        for(Int_t j=0; j<n_tracks_mpd;j++) // mpd-loop
        {
            if( !this->Selector(pdg,j,1) )
                continue;
            Float_t y = this->GetY(j,1);
            Float_t v1 = this->GetV1( phi_mpd[j], PsiEP );
            Float_t w=0;
            if(abs(y)>1.5)
                continue;
            if(y>0)
                w=1;
            if(y<0)
                w=-1;
            if(b_mc<5)
            {
                if(abs(y) > 0.2 )
                    profile[9]->Fill(pt_mc[j],w*v1);
                profile[3]->Fill(y,v1);
            }
            if( (b_mc>5)&&(b_mc<9) )
            {
                if(abs(y) > 0.2 )
                    profile[10]->Fill(pt_mc[j],w*v1);
                profile[4]->Fill(y,v1);
            }
            if( (b_mc>9)&&(b_mc<17) )
            {
                if(abs(y) > 0.2 )
                    profile[11]->Fill(pt_mc[j],w*v1);
                profile[5]->Fill(y,v1);
            }
        }
       this->ShowProgress(i,N);
   }
   TFile* out_file = new TFile(Form("flow_%i.root",pdg),"recreate");
   out_file->cd();
   gStyle->SetOptStat(0);
   for(int i=0; i<12; i++)
       profile[i]->Write();
   out_file->Close();


}
Float_t merged::GetAngleZDC(Int_t j)
{
    Float_t y = 0, x = 0;    
    if ((j>=0) && (j<=4))
    {
        y = 45;
        x = (j-2)*15;
        return atan2(y,x);
    }
    else if ((j>=5) && (j<=39))
    {
        y = (3-(j+2)/7)*15; 
        x = (3-(j+2)%7)*15;
        return atan2(y,x);
    }
    else if ((j>=40) && (j<=44))
    {
        y = -45; 
        x = (j-42)*15;
        return atan2(y,x);
    }
    return -1000;
}
Bool_t merged::Selector(Int_t pdg, Int_t j, Bool_t is_mpd)
{
    if ( is_mpd )
    {
        if( PDG_code_mc[ id_from_mc_mpd[j] ] != pdg )
            return 0;
        if( eta_mpd[j]>1.2 )
            return 0;
        if( ( abs(signed_pt_mpd[j])<0.2 ) && ( abs(signed_pt_mpd[j])>2.0) )
            return 0;
        if( mother_ID_mc[ id_from_mc_mpd[j] ]!=-1 )
            return 0;
        if( n_hits_mpd[j]<32 )
            return 0;
        return 1;
    }
    if ( !is_mpd )
    {
        if( PDG_code_mc[j] != pdg )
            return 0;
        if( eta_mc[j]>1.2 )
            return 0;
        if( (pt_mc[j]<0.2) && (pt_mc[j]>2.0) )
            return 0;
        if( mother_ID_mc[j]!=-1 )
            return 0;
        return 1;
    }
    return 0;
    
}
Float_t merged::GetPsiEP()
{
    Float_t phi=0; // angle of central module
    Float_t Qx = 0, Qy = 0; // proections of Q-vector
    for(int j=0;j<45;j++) // Gettitng event angle
    {
        if (j==22)
        {
            continue;
        }
        phi=this->GetAngleZDC(j);
        Qx+= ZDC_energy_mpd[j]*cos(phi);
        Qy+= ZDC_energy_mpd[j]*sin(phi);
    }
    return atan2(Qy,Qx);
}
Float_t merged::GetY(Int_t j, Bool_t is_mpd)
{
    if( is_mpd )
    {
        Float_t m = mass_mc[ id_from_mc_mpd[j] ];
        Float_t pt = abs(signed_pt_mpd[j]);
        Float_t coshn = cosh(eta_mpd[j]), sinhn = sinh(eta_mpd[j]);
        Float_t y = log( sqrt(m*m+pt*pt*coshn*coshn) + pt*sinhn ) - 0.5*log( m*m+pt*pt );
        return y;
    }
    if( !is_mpd )
    {
        Float_t y = 0.5*( log(energy_mc[j]+pz_mc[j]) - log(energy_mc[j]-pz_mc[j]) );
        return y;
    }
    return 0;
}
Float_t merged::GetV1(Float_t phi, Float_t phiEP)
{
    return cos(phi-phiEP);
}
void merged::ShowProgress(Int_t i, Int_t N)
{
    Float_t barsize=50;
    Int_t progress = barsize*i/N;
    if(i == 0)
    {
        cout << "[" << flush;
        for(int j=0;j<barsize-2;j++)
            cout << "." << flush;
        cout << "]" << flush;
    }
    if( (i!=0)&&(i!=N) )
    {
        for(int j=0;j<barsize;j++)
            cout<<"\r"<<flush;
        cout << "[" <<flush;
        for(int j=0;j<=progress;j++)
            cout<<"#"<<flush;
        for(int j=0;j<barsize-progress-2;j++)
            cout<<"."<<flush;
        cout<<"]"<<flush;
    }
    if( i==N )
    {
        for(int j=0;j<barsize;j++)
            cout<<"\r"<<flush;
        cout << "[" << flush;
        for(int j=0;j<barsize-2;j++)
            cout<<"#"<<flush;
        cout<<"]"<<endl;
    }
}
