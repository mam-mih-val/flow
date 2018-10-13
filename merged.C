#define merged_cxx
#include "merged.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

void merged::Loop()
{
   if (fChain == 0) return;
   TProfile* profile[3];
   profile[0] = new TProfile("0<b_mc<5","0<b_mc<5, mpd",8,0.2,2,-1,1); // mpd
   profile[1] = new TProfile("5<b_mc<9","5<b_mc<9, mpd",8,0.2,2,-1,1); // mpd
   profile[2] = new TProfile("9<b_mc<17","9<b_mc<17, mpd",8,0.2,2,-1,1); // mpd

   Int_t N = fChain->GetEntries();
   Int_t progress = 0;
   for(int i=0;i<N;i++)
   {
       fChain->GetEntry(i);
       Float_t PsiEP = this->GetPsiEP();
       for(Int_t j=0; j<n_tracks_mc;j++)
       {
            if( !this->Selector(-321,j,0) )
                continue;
            Float_t m = 0.493;
            Float_t y = this->GetY(j,m,0);
            Float_t v1 = this->GetV1( atan2(py_mc[j],px_mc[j]), phiEP_mc );
            Float_t w=0;
            if( (abs(y)<0.2) || (abs(y)>1.5) )
                continue;
            if(y>0)
                w=1;
            if(y<0)
                w=-1;
            if(b_mc<5)
                profile[0]->Fill(pt_mc[j],w*cos(phi_mc-phiEP_mc));
            if( (b_mc>5)&&(b_mc<9) )
                profile[1]->Fill(pt_mc[j],w*cos(phi_mc-phiEP_mc));
            if( (b_mc>9)&&(b_mc<17) )
                profile[2]->Fill(pt_mc[j],w*cos(phi_mc-phiEP_mc));
       }
       if( i==0 )
           cout<<"<"<<flush;
       if( 20*i/N > progress )
       {
           cout<<"="<<flush;
       }
       if( i==N-1 )
           cout<<">"<<endl;
       progress = 20*i/N;
   }
   TFile* out_file = new TFile("histograms.root","recreate");
   out_file->cd();
   gStyle->SetOptStat(0);
   profile[0]->Write();
   profile[1]->Write();
   profile[2]->Write();
   out_file->Close();


}
Float_t merged::GetAngleZDC(Int_t j)
{
    Float_t y = 0, x = 0;    
    if ((j>=0) && (j<=4))
    {
        y = 45;
        x = (j-2)*15;
//        cout << j << " " << x << " " << y << " " << atan2(y,x)*180/3.14 <<endl;
        return atan2(y,x);
    }
    else if ((j>=5) && (j<=39))
    {
        y = (3-(j+2)/7)*15; 
        x = (3-(j+2)%7)*15;
//        cout << j << " " << x << " " << y << " " << atan2(y,x)*180/3.14 <<endl;
        return atan2(y,x);
    }
    else if ((j>=40) && (j<=44))
    {
        y = -45; 
        x = (j-42)*15;
//        cout << j << " " << x << " " << y << " " << atan2(y,x)*180/3.14 <<endl;
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
Float_t merged::GetY(Int_t j, Float_t m, Bool_t is_mpd)
{
    if( is_mpd )
    {
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