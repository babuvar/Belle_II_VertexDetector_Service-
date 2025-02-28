//******************************************
//*Original Author: Varghese Babu***********
//*Tata Institute of Fundamental Research***
//******************************************

#include "TVector3.h"
#include <vector>
#include "TVector3.h"
#include "TMath.h"
#include <cstring>
using namespace std;
using namespace ROOT;


vector<TVector3> A1; Int_t count=-1,KOK_OneTime_Corr=0;



//****************************---------------------XXXXXXXXXXXX---------------------****************************//
class data{
public:
vector<TVector3> A;
Double_t ParA,ParB;
Int_t Num_Entries, Data_Status, FW_Flag; char name[10];
data();
~data();
void Set(vector<TVector3>& B, Int_t Start_Num, Int_t End_Num);
void Print();
};

data::data()
{
Data_Status=0; FW_Flag=0; 
Num_Entries=0; //Signifies Empty instance of data
}
data::~data(){}

void data::Set(vector<TVector3>& B, Int_t Start_Num, Int_t End_Num)
{
Num_Entries=End_Num-Start_Num+1;
if(Num_Entries>=1){
for(Int_t i=0;i<Num_Entries;i++){A.push_back(B.at(Start_Num+i));}
}
else if(Num_Entries==0){cout<<"Trying to input a null array. Action aborted."<<endl;}
else{cout<<"Starting number is greater than ending number. Please enter numbers correctly."<<endl;}

}

void data::Print(){
cout<<"Num entries is "<<Num_Entries<<endl;
if (Num_Entries>=1){
for(Int_t i=0;i<Num_Entries;i++){cout<<i<<") "; A.at(i).Print();}
}
}

//****************************---------------------XXXXXXXXXXXX---------------------****************************//


void Read(char filename[], data & DATUM, Int_t need, Int_t option);
void Copy(data & DATUM1, data & DATUM2);
void MakeBasis(data & DATUM);
void LocalAxes(data & DATUM);
void FitPlane(data & DATUM);
void FindShifts(data & DATUM, Int_t option);
void FullSensorShift(data & DATUM, Int_t option);
void Rotate3D(TVector3 & V, TVector3 Rot);
TVector3 UnitX,UnitY,UnitZ,Origin;
//Carriers of data
data KOK,FWRand,CERand,BWRand,FW_F,CE_F,BW_F;
data KOK_copy,FWRand_copy,CERand_copy,BWRand_copy,FW_F_copy,CE_F_copy,BW_F_copy;


void cmm_3pt_BWCE()
{
// Reading Data Points
Read("KOK",KOK,1,0);//KOK.Print();
//Read("FWRand",FWRand,0,1);//FWRand.Print();
//Read("CERand",CERand,0,0);//CERand.Print();
//Read("BWRand",BWRand,0,0);//BWRand.Print();
//Read("FW_F",FW_F,0,1);//FW_F.Print();
Read("CE_F",CE_F,0,0);//CE_F.Print();
Read("BW_F",BW_F,0,0);//BW_F.Print();


//Making copies
//if(FWRand.Data_Status==1){Copy(FWRand,FWRand_copy);}
//if(CERand.Data_Status==1){Copy(CERand,CERand_copy);}
//if(BWRand.Data_Status==1){Copy(BWRand,BWRand_copy);}
//if(FW_F.Data_Status==1){Copy(FW_F,FW_F_copy);}
//if(CE_F.Data_Status==1){Copy(CE_F,CE_F_copy);}
//if(BW_F.Data_Status==1){Copy(BW_F,BW_F_copy);}


//Constructing the basis vectors
MakeBasis(KOK);


//Getting Local Coordinates
//if(FWRand.Data_Status==1){LocalAxes(FWRand);}
//if(CERand.Data_Status==1){LocalAxes(CERand);}
//if(BWRand.Data_Status==1){LocalAxes(BWRand);}
if(FW_F.Data_Status==1){LocalAxes(FW_F);}
//if(CE_F.Data_Status==1){LocalAxes(CE_F);}
//if(BW_F.Data_Status==1){LocalAxes(BW_F);}


//Slant, Tilt Angles
//if(FWRand.Data_Status==1){FitPlane(FWRand);} else{if(FW_F.Data_Status==1){FitPlane(FW_F);}else{cout<<"No data on FW sensor found"<<endl;}}
//if(CERand.Data_Status==1){FitPlane(CERand);} else{if(CE_F.Data_Status==1){FitPlane(CE_F);}else{cout<<"No data on CE sensor found"<<endl;}}
//if(BWRand.Data_Status==1){FitPlane(BWRand);} else{if(BW_F.Data_Status==1){FitPlane(BW_F);}else{cout<<"No data on BW sensor found"<<endl;}}

//'F' mark shifts
//if(FW_F.Data_Status==1){FindShifts(FW_F,1);}
//if(CE_F.Data_Status==1){FindShifts(CE_F,2);}
//if(BW_F.Data_Status==1){FindShifts(BW_F,3);}

//Whole-Sensor shifts
if(FW_F.Data_Status==1){FullSensorShift(FW_F,1);}
if(CE_F.Data_Status==1){FullSensorShift(CE_F,2);}
if(BW_F.Data_Status==1){FullSensorShift(BW_F,3);}



}//end of cmm4()




void Read(char filename[], data & DATUM, Int_t need, Int_t option )
{
strcpy(DATUM.name,filename);
Int_t Start_Num, End_Num;
Double_t x,y,z; ifstream fin; TVector3 temp;

if(option==1){DATUM.FW_Flag=1;}

fin.open(filename); 
if(!fin.is_open()){cout<<"'"<<filename<<"' does not exist."<<endl;
if(need==1){cout<<"Please create this file with the correct data as location of Kokeshi Pin is absolutely essential."<<endl; exit();}
else{cout<<"The program will proceed and try to do calculations without this information"<<endl;}
}

else{DATUM.Data_Status=1;
cout<<"Reading file '"<<filename<<"'"<<endl;
Start_Num=count+1;
while(!fin.eof())
{count++;
fin>>x>>y>>z; temp.SetXYZ(x,y,z);
A1.push_back(temp);
}
End_Num=count-1;
fin.close();
DATUM.Set(A1,Start_Num,End_Num);}

}

void Copy(data & DATUM1, data & DATUM2)
{
DATUM2.Num_Entries=DATUM1.Num_Entries;
DATUM2.Data_Status=DATUM1.Data_Status;
DATUM2.FW_Flag=DATUM1.FW_Flag;
strcpy( DATUM2.name, DATUM1.name ); 
//Clear DATUM2 first
DATUM2.A.clear();
for(Int_t i=0;i<DATUM2.Num_Entries;i++){
DATUM2.A.push_back(DATUM1.A.at(i));
}//for

}



void MakeBasis(data & DATUM)
{
TVector2 unitx,unity; Double_t x,y,z,ztemp;
//Auto-correcting for change in FW and BW MT Block height interchange
if(DATUM.A.at(0).Z() > DATUM.A.at(1).Z()){z=DATUM.A.at(0).Z(); ztemp=DATUM.A.at(1).Z(); DATUM.A.at(0).SetZ(ztemp); DATUM.A.at(1).SetZ(z);
cout<<"Auto-correcting for change in FW and BW MT Block height interchange"<<endl; }

if(KOK_OneTime_Corr==0){z=DATUM.A.at(0).Z()+18.3;DATUM.A.at(0).SetZ(z); KOK_OneTime_Corr=1;}

UnitX=DATUM.A.at(0)-DATUM.A.at(1); UnitX=UnitX.Unit(); //UnitX.Print();
x=UnitX.X(); y=UnitX.Y(); unitx.Set(x,y); unitx=unitx.Unit(); 
unity=unitx.Rotate(TMath::Pi()/2); 
x=unity.X(); y=unity.Y(); UnitY.SetXYZ(x,y,0); //UnitY.Print();
UnitZ=UnitX; UnitZ.Rotate(TMath::Pi()/2,-UnitY); //UnitZ.Print();
Origin=DATUM.A.at(1);

}
//****************************---------------------XXXXXXXXXXXX---------------------****************************//


void LocalAxes(data & DATUM)
{
Double_t x,y,z;
for(Int_t  i=0;i<DATUM.Num_Entries;i++){
DATUM.A.at(i)=DATUM.A.at(i)-Origin;
x=DATUM.A.at(i)*UnitX;y=DATUM.A.at(i)*UnitY;z=DATUM.A.at(i)*UnitZ; DATUM.A.at(i).SetXYZ(x,y,z);
}

}

void FitPlane(data & DATUM)
{
cout<<"****************************************"<<endl;
cout<<"Fitting of Plane performed on file: "<<DATUM.name<<endl;

//Plotting Range
Double_t tempmaxz=-1000000,tempminz=1000000;
for(Int_t  i=0;i<DATUM.Num_Entries;i++){
if(DATUM.A.at(i).Z()>=tempmaxz){tempmaxz=DATUM.A.at(i).Z();}
if(DATUM.A.at(i).Z()<=tempminz){tempminz=DATUM.A.at(i).Z();}
}



Double_t x,y,z,xtemp,ytemp;
TGraph2D *g = new TGraph2D();
for(i=0;i<DATUM.Num_Entries;i++)
{x=DATUM.A.at(i).X(); y=DATUM.A.at(i).Y(); z=DATUM.A.at(i).Z(); 
g->SetPoint(i,x,y,z);}
TF2 *f = new TF2("f", "[0] + [1] * x + [2] * y"); // Equation of Plane
  g->Fit(f);


//Auto-Correcting for FW -BW KOK pin interchange
if( DATUM.FW_Flag == 1 && f->GetParameter(1) > 0){
cout<<"Auto-Correcting for FW -BW KOK pin interchange"<<endl;
x=KOK.A.at(0).X(); xtemp=KOK.A.at(1).X(); KOK.A.at(0).SetX(xtemp); KOK.A.at(1).SetX(x); 
y=KOK.A.at(0).Y(); ytemp=KOK.A.at(1).Y(); KOK.A.at(0).SetY(ytemp); KOK.A.at(1).SetY(y);
//Copying data back
if(FWRand.Data_Status==1){Copy(FWRand_copy,FWRand);}
if(CERand.Data_Status==1){Copy(CERand_copy,CERand);}
if(BWRand.Data_Status==1){Copy(BWRand_copy,BWRand);}
if(FW_F.Data_Status==1){Copy(FW_F_copy,FW_F);}
if(CE_F.Data_Status==1){Copy(CE_F_copy,CE_F);}
if(BW_F.Data_Status==1){Copy(BW_F_copy,BW_F);}
//Reconstructing the Basis
MakeBasis(KOK);


//Getting the Local coordinates again
if(FWRand.Data_Status==1){LocalAxes(FWRand);}
if(CERand.Data_Status==1){LocalAxes(CERand);}
if(BWRand.Data_Status==1){LocalAxes(BWRand);}
if(FW_F.Data_Status==1){LocalAxes(FW_F);}
if(CE_F.Data_Status==1){LocalAxes(CE_F);}
if(BW_F.Data_Status==1){LocalAxes(BW_F);}

//Redoing the fit
if(FWRand.Data_Status==1){FitPlane(FWRand);} else{FitPlane(FW_F);}
}//if
else{
//Fit Parameters & ram/incorrect values of Tilt and Normal angles
cout<<endl<<"Sensor Angles"<<endl;
cout<<"Slant angle(degrees) ="<<atan(f->GetParameter(1))*180/TMath::Pi()<<"+/- "<<atan(f->GetParError(1))*180/TMath::Pi()<<endl;
cout<<"Tilt angle(degrees) ="<<atan(f->GetParameter(2))*180/TMath::Pi()<<"+/- "<<atan(f->GetParError(2))*180/TMath::Pi()<<endl;
cout<<"-----------------------------------------"<<endl;
TCanvas * c1 =new TCanvas(DATUM.name,DATUM.name);
  g->Draw("TRI1");
  g->Draw("SAME P0");
  g->GetZaxis()->SetRangeUser(tempminz,tempmaxz);
DATUM.ParA=f->GetParError(1); DATUM.ParB=f->GetParError(2);
}//else

}


void FindShifts(data & DATUM, Int_t option)
{
if(option != 1 && option != 2 && option != 3){cout<<"No such option exists for FindShifts(). Please enter correct option."<<endl;}
else{

Int_t i,j; Double_t x,y,z;
vector<TVector3> VM(4), VD(4), Shift(4);
char pnt[4]="efhg";

if(option==1){
VD.at(0).SetXYZ(426.365,-20.219,-13.021);VD.at(1).SetXYZ(303.781,-30.007,12.812);VD.at(2).SetXYZ(303.781,30.307,12.812);VD.at(3).SetXYZ(426.365,20.519,-13.021);}
else if(option==2){
VD.at(0).SetXYZ(302.810,-29.250,13.020);VD.at(1).SetXYZ(178.710,-29.250,13.020);VD.at(2).SetXYZ(178.710,29.550,13.020);VD.at(3).SetXYZ(302.810,29.550,13.020);}
else if(option==3){
VD.at(0).SetXYZ(177.810,-29.250,13.020);VD.at(1).SetXYZ(53.710,-29.250,13.020);VD.at(2).SetXYZ(53.710,29.550,13.020);VD.at(3).SetXYZ(177.810,29.550,13.020);}


cout<<"'F' mark shifts calculated for the file : "<<DATUM.name<<endl<< "\t\t    (Δx\t\t    Δy\t\t    Δz)"<<endl;
for(i=0;i<4;i++)
{j=i*4; VM.at(i)=(DATUM.A.at(j)+DATUM.A.at(j+1)+DATUM.A.at(j+2)+DATUM.A.at(j+3))*(0.25);
Shift.at(i)=(VM.at(i)-VD.at(i))*1000;
cout<<"Point "<<i+1<<"("<<pnt[i]<<"):\t"<<Shift.at(i).X()<<"μm\t"<<Shift.at(i).Y()<<"μm\t"<<Shift.at(i).Z()<<"μm"<<endl;
}
cout<<"$@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@ $@"<<endl;


}//else
}


void FullSensorShift(data & DATUM, Int_t option)
{
if(option != 1 && option != 2 && option != 3){cout<<"No such option exists for FindShifts(). Please enter correct option."<<endl;}
else{
TVector3 CGM(0.0,0.0,0.0), CGD, Shift; Int_t i; Double_t x,y,z;
vector<TVector3> VM(16),VD(16),VD1(16); 

if(option==1){CGD.SetXYZ(365.073,0.15,-0.1045);}
else if(option==2){CGD.SetXYZ(240.76,0.15,13.02);}
else if(option==3){CGD.SetXYZ(115.76,0.15,13.02);}
//cout<<"Full Sensor shift corresponding to the file : "<<DATUM.name<<endl<< "    (Δx\t\t    Δy\t\t    Δz)"<<endl;
//for(i=0;i<16;i++){CGM=CGM+DATUM.A.at(i);}
//CGM=CGM*0.0625;
TVector3 unitn(0,0,0),v4(0,0,0),v3(0,0,0),v2(0,0,0),v1(0,0,0),vect1; Double_t con;
for(i=0;i<4;i++){v1=v1+DATUM.A.at(i);} v1=v1*0.25;
for(i=4;i<8;i++){v2=v2+DATUM.A.at(i);} v2=v2*0.25;
for(i=8;i<12;i++){v3=v3+DATUM.A.at(i);} v3=v3*0.25;
for(i=12;i<16;i++){v4=v4+DATUM.A.at(i);} v4=v4*0.25;
unitn=v3-v2;
vect1=v1-v2;
unitn=unitn.Unit();
con=unitn*vect1;
TVector3 v4calc;
//v4calc=v3+v1-v2-(2*con*unitn);
v4calc=v3+v1-v2;

//CGM=(v1+v2+v3+v4)*0.25;
Shift=(v4-v4calc)*1000;


//In planar coordinates
//TVector3 xplane=v1-v2-(con*unitn);
TVector3 xplane=v1-v2;
xplane=xplane.Unit();
//xplane.Print();
TVector3 yplane=v3-v2;
yplane=yplane.Unit();
//yplane.Print();
TVector3 zplane=xplane;
zplane.Rotate(-(TMath::Pi()/2), yplane);
//zplane.Print();


//cout<<"Point - 4(g) deformation in Kokeshi coordinates"<<endl;
//cout<<Shift.X()<<"\t"<<Shift.Y()<<"\t"<<Shift.Z()<<endl;

cout<<"Point - 4(g) deformation w.r.t. sensor coordinates"<<endl;
cout<<Shift*xplane<<"\t"<<Shift*yplane<<"\t"<<Shift*zplane<<endl;

//cout<<Shift.X()<<"μm\t"<<Shift.Y()<<"μm\t"<<Shift.Z()<<"μm"<<endl;

//cout<<"v3 = "<<v3.X()<<"\t"<<v3.Y()<<"\t"<<v3.Z()<<endl;
/*
//Getting Rotation about plane's normal 
TVector3 L(62.05,0,0),M(0.1,0,0),N1(0,29.25,0),N2(0,29.55,0),O(0,0.1,0);
TVector3 NS1(0,20.219,0),NS2(0,20.519,0),NL1(0,30.007,0),NL2(0,30.307,0);



if(option==1){
VD.at(0)=L+M-NS1-O;
VD.at(1)=L-M-NS1-O;
VD.at(2)=L-M-NS1+O;
VD.at(3)=L+M-NS1+O;
VD.at(4)=-L+M-NL1-O;
VD.at(5)=-L-M-NL1-O;
VD.at(6)=-L-M-NL1+O;
VD.at(7)=-L+M-NL1+O;
VD.at(8)=-L+M+NL2-O;
VD.at(9)=-L-M+NL2-O;
VD.at(10)=-L-M+NL2+O;
VD.at(11)=-L+M+NL2+O;
VD.at(12)=L+M+NS2-O;
VD.at(13)=L-M+NS2-O;
VD.at(14)=L-M+NS2+O;
VD.at(15)=L+M+NS2+O;}
else if(option==2 || option==3)
{
VD.at(0)=L+M-N1-O;
VD.at(1)=L-M-N1-O;
VD.at(2)=L-M-N1+O;
VD.at(3)=L+M-N1+O;
VD.at(4)=-L+M-N1-O;
VD.at(5)=-L-M-N1-O;
VD.at(6)=-L-M-N1+O;
VD.at(7)=-L+M-N1+O;
VD.at(8)=-L+M+N2-O;
VD.at(9)=-L-M+N2-O;
VD.at(10)=-L-M+N2+O;
VD.at(11)=-L+M+N2+O;
VD.at(12)=L+M+N2-O;
VD.at(13)=L-M+N2-O;
VD.at(14)=L-M+N2+O;
VD.at(15)=L+M+N2+O;
}

Double_t theta,BestTheta=0,ErrDist,BestErrDist=100000000,a=DATUM.ParA,b=DATUM.ParB;
TVector3 Rot(-a,-b,1),Diff;
for(i=0;i<16;i++){VM.at(i)=DATUM.A.at(i);}

//for(theta=-(TMath::Pi()/20);theta <= (TMath::Pi()/20);theta=theta+0.0002){
for(theta=-0.1;theta <=0.1;theta=theta+0.0002){
ErrDist=0;
for(i=0;i<8;i++){
VD1.at(i)=VD.at(i);
//VD1.at(i).Print();
//VD.at(i).Print();cout<<endl;

VD1.at(i).RotateZ(theta);
Rotate3D(VD1.at(i),Rot);
VD1.at(i)=VD1.at(i)+CGM;
Diff=VD1.at(i)-VM.at(i);
ErrDist=ErrDist+Diff.Mag2();
}//i
for(i=12;i<16;i++){
VD1.at(i)=VD.at(i);
//VD1.at(i).Print();
//VD.at(i).Print();cout<<endl;

VD1.at(i).RotateZ(theta);
Rotate3D(VD1.at(i),Rot);
VD1.at(i)=VD1.at(i)+CGM;
Diff=VD1.at(i)-VM.at(i);
ErrDist=ErrDist+Diff.Mag2();
}//i





if(ErrDist < BestErrDist){BestErrDist=ErrDist; BestTheta=theta;}
cout<<"theta = "<<theta<<"\tErrDist = "<<ErrDist<<endl;
}//theta
cout<<"Rotation of sensor about its normal"<<endl;
cout<<"Gamma(degrees) = "<<(BestTheta*180)/TMath::Pi()<<" +/- 0.01"<<endl;*/
}//else
}


void Rotate3D(TVector3 & V, TVector3 Rot)
{
Double_t a,b,c,theta,x,y,r; TVector3 Rot_Axis;
a=Rot.X();b=Rot.Y();c=Rot.Z();

r=sqrt((a*a)+(b*b));
theta=r/fabs(c);
theta=atan(theta);

x=-b/r;
y=a/r;

Rot_Axis.SetXYZ(x,y,0);
V.Rotate(theta,Rot_Axis);
}



















