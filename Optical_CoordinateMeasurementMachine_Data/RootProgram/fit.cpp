#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;
void fit()
{
const Double_t pi = acos(-1.0);

 gStyle->SetOptStat(0);
   gStyle->SetOptFit();
double x[100],y[100],z[100];
int c=-1;
ifstream fin;
//fin.open("work1.txt");
//fin.open("work2.txt");
//fin.open("work3.txt");
fin.open("slant-points");
//fin.open("Sheet1.tsv");

while(!fin.eof())
{c++;
fin>>x[c]>>y[c]>>z[c];
}
fin.close();


float tempmaxz=-1000000,tempminz=1000000;
for(int i=0;i<=c-1;i++){
if(z[i]>=tempmaxz){tempmaxz=z[i];}
if(z[i]<=tempminz){tempminz=z[i];}
//cout<<x[i]<<"\t"<<y[i]<<"\t"<<z[i]<<endl;
}


TGraph2D *g = new TGraph2D();
for(int i=0;i<=c-1;i++){g->SetPoint(i,x[i],y[i],z[i]);}

TF2 *f = new TF2("f", "[0] + [1] * x + [2] * y");


  g->Fit(f);


  g->Draw("TRI1");
  g->Draw("SAME P0");
  g->GetZaxis()->SetRangeUser(tempminz,tempmaxz);


cout<<endl<<"Fit parameters:"<<endl;
cout<<"Tilt angle(degrees) ="<<atan(f->GetParameter(2))*180/pi<<endl;
cout<<"Normal angle(degrees) ="<<atan(f->GetParameter(1))*180/pi<<endl;
}
