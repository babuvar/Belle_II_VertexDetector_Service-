//This is to find the relative angle between the coordinate systems of the kokeshi pins and the reference holes
//Steps to Follow
//1) Get angle of misalignment of Kok-Pin (subtract 90 degrees) sys w.r.t. CMM coordinates by providing X,Y coordinates of Kok-Pin points to vec2 A & B
//2) Convert into radians
//3) Input angle of step 2 in radians to get the correct transformed Tilt/ Normal Angles. Make sure to give the correct data text file as input




#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;
float pi=acos(-1.0);
class vec2{
public:
float X,Y;
vec2();
vec2(float a,float b);
~vec2();
void set(float a,float b);
void print();
float distance(vec2 A);
void translate(vec2 A);
void rotate(float theta);
void Rotate(float theta);
void copY(vec2 A);
void add(vec2 A);
vec2 sub(vec2 A);
void divide(float d);
void XYAngles();
float mag();
float CrossAngle(vec2 A);
vec2 GetUnit();
float dot(vec2 A);
};

float vec2::dot(vec2 A)
{float dt;
dt=(X*A.X)+(Y*A.Y);
return dt;
}


vec2 vec2::GetUnit()
{
vec2 B;
float r,x,y;
r=sqrt((X*X)+(Y*Y));
x=X/r;y=Y/r;
B.set(x,y);
return B;
}

float vec2::CrossAngle(vec2 A)
{
float d,m1,m2,t;
d=(X*A.X)+(Y*A.Y);
m1=sqrt((X*X)+(Y*Y));
m2=sqrt((A.X*A.X)+(A.Y*A.Y));
t=d/(m1*m2);
t=acos(t);
t=t*180/pi;
return t;
}


vec2::vec2()
{
X=0;Y=0;
}

float vec2::mag()
{
float r;
r=sqrt((X*X)+(Y*Y));
return r;
}

void vec2::add(vec2 A)
{vec2 B;
X=X+A.X;
Y=Y+A.Y;
}

vec2 vec2::sub(vec2 A)
{vec2 B;
B.X=X-A.X;
B.Y=Y-A.Y;
return B;
}

void vec2::divide(float d)
{
X=X/d;
Y=Y/d;
}

void vec2::XYAngles()
{
float r,theta1,theta2;
r=sqrt((X*X)+(Y*Y));
theta1=acos(X/r);
theta2=asin(Y/r);
cout<<"theta1 = "<<(theta1*180/pi)<<"    theta2 = "<<(theta2*180/pi)<<endl;
//cout<<"theta1 = "<<theta1-(pi/2)<<"    theta2 = "<<theta2<<endl;
}

vec2::vec2(float a,float b)
{
X=a;Y=b;
}

void vec2::set(float a,float b)
{
X=a;Y=b;
}

void vec2::copY(vec2 A)
{
X=A.X;
Y=A.Y;
}

vec2::~vec2(float a,float b){}

void vec2::print()
{
cout<<X<<"\t"<<Y<<endl;
}

float vec2::distance(vec2 A){
float dist,Xdif,Ydif;
Xdif=X-A.X;
Ydif=Y-A.Y;
dist=sqrt((Xdif*Xdif)+(Ydif*Ydif));
return dist;
}

void vec2::translate(vec2 A){
X=X+A.X;
Y=Y+A.Y;
}

void vec2::rotate(float theta){
float X1,Y1;
X1=X;Y1=Y;
X=(cos(theta)*X1)-(sin(theta)*Y1);
Y=(sin(theta)*X1)+(cos(theta)*Y1);
}

vec2 vec2::Rotate(float theta){
float X1,Y1; vec2 A;
X1=X;Y1=Y;
A.X=(cos(theta)*X1)-(sin(theta)*Y1);
A.Y=(sin(theta)*X1)+(cos(theta)*Y1);
return A;
}












void Identify()
{

//Ref pts angle w.r.t. CMM coordinates
vec2 A,B,C,UnitX,UnitY;//,D(1,-0.1);


A.set(408.899,221.008); 
B.set(554.906,221.184);
//A.set(432.65559,200.82595);
//B.set(578.67881,201.60715);


C=B.sub(A);
//C.XYAngles();
//D.XYAngles();
UnitX=C.GetUnit();
UnitY=UnitX.Rotate(pi/2);
//UnitX.print();
//UnitY.print();
//cout<<UnitX.mag()<<endl;
//cout<<UnitY.mag()<<endl;



/*
//Kok sys angle w.r.t. CMM coordinates
vec2 A,B,C;//,D(1,-0.1);
A.set(506.55523,63.52049); //X & Y coordinates of 1st Kokeshi Pin are set to 2-D vector A
B.set(504.19663,519.45358); //X & Y coordinates of 2nd Kokeshi Pin are set to 2-D vector B
C=B.sub(A);
C.XYAngles();
//D.XYAngles();
*/



 gStyle->SetOptStat(0);
   gStyle->SetOptFit();
double x[1000],y[1000],z[1000];
int c=-1;


//Reading Data
ifstream fin;
//fin.open("slant1");
//fin.open("slant2");
//fin.open("slant3");
//fin.open("slant4");
//fin.open("slantF");
//fin.open("slantF1");
//fin.open("slantF2");
//fin.open("slantF3");
//fin.open("fwf.txt");//*******************
//fin.open("cef.txt");
fin.open("bwf.txt");


//fin.open("A");
//fin.open("slantF8");




//fin.open("slant5");//***********
//fin.open("FWRandom_1");
//fin.open("FWRandom_2");
//fin.open("slantpoints");
//fin.open("slantpoints_2");
while(!fin.eof())
{c++;
fin>>x[c]>>y[c]>>z[c];
}
fin.close();


//Modify Each Point************************************************************************************
vec2 V1,V2; float x1,y1;
for(int i=0;i<=c-1;i++){
x1=x[i]; y1=y[i];
V1.set(x1,y1);
V2=V1.sub(A);
x[i]=V2.dot(UnitX);
y[i]=V2.dot(UnitY);
cout<<x[i]<<"  "<<y[i]<<endl;
}



float tempmaxz=-1000000,tempminz=1000000,tempminy=1000000;
for(int i=0;i<=c-1;i++){
if(z[i]>=tempmaxz){tempmaxz=z[i];}
if(z[i]<=tempminz){tempminz=z[i];}
if(y[i]<=tempminy){tempminy=y[i];}
//cout<<x[i]<<"\t"<<y[i]<<"\t"<<z[i]<<endl;
}


//Fitting
TGraph2D *g = new TGraph2D();
for(int i=0;i<=c-1;i++){g->SetPoint(i,x[i],y[i],z[i]);}
TF2 *f = new TF2("f", "[0] + [1] * x + [2] * y"); // Equation of Plane
  g->Fit(f);
  g->Draw("TRI1");
  g->Draw("SAME P0");
  g->GetZaxis()->SetRangeUser(tempminz,tempmaxz);

/*
//Fit Parameters & ram/incorrect values of Tilt and Normal angles
cout<<endl<<"Fit parameters:"<<endl;
cout<<"Tilt angle(degrees) ="<<atan(f->GetParameter(2))*180/pi<<endl;
cout<<"Normal angle(degrees) ="<<atan(f->GetParameter(1))*180/pi<<endl;
cout<<"Error in Tilt angle(degrees) ="<<atan(f->GetParError(2))*180/pi<<endl;
cout<<"Error in Normal angle(degrees) ="<<atan(f->GetParError(1))*180/pi<<endl;


//To transform the raw Tilt/Normal angles to correct values in the Kokeshi pin ref frame
float T1,T2,phi=0.00517309; //Angle Phi of kok sys w.r.t. CMM sys in radians
T1=f->GetParameter(1);
T2=f->GetParameter(2);

vec2 Th(T1,T2);
Th.rotate(-phi);

T1=(atan(Th.X))*180/pi;
T2=(atan(Th.Y))*180/pi;

cout<<endl<<"Correct Transformed values"<<endl;
cout<<"Tilt angle(degrees) ="<<T2<<endl;
cout<<"Normal angle(degrees) ="<<T1<<endl;
*/
cout<<"***********\t"<<tempminy<<"\t**********"<<endl;
}









