#include "TVector3.h"
#include <vector>
#include "TVector3.h"
#include "TMath.h"
#include <cstring>
using namespace std;
using namespace ROOT;
void test()
{
TVector3 A(1,3,-4),B(303.781,0,12.812),C(426.365,0,-13.021);
//TVector3 A(1,3,-4),B(303.781,0,0),C(426.365,0,0);

cout<<"Mag2 = "<<((B-C).Mag())/2<<endl;

}
