#include <cmath>
#include <iostream>

using namespace std;
double bw(double x, double mH, double w_fr){
	    // Double_t width = w_fr * mH;

    double mH_cmplx = mH * sqrt(1+w_fr*w_fr);
    // double wH_cmplx = mH_cmplx * w_fr;

    double A = 1;
    double B = std::pow((x*x-mH_cmplx*mH_cmplx),2) + std::pow((x*x*w_fr),2);
    double Propag = A/B;

    double Lgg = pow((1-pow((x/13000),(1./3))),8.95) * pow((x/13000),-4.1) * (-2.95e-10 + 1.32e-7*(x/13000) -1.7e-7*pow((x/13000), 2));


    double G_gg = std::pow(x,3);
    double tau = std::pow(x/(2*173.34),2);
    double f=0;
    double A_f = 0;
	const double PI = 3.1415926535897;
    if (tau<=1) {
        // f = TMath::ASin(sqrt(tau));
        f = asin(sqrt(tau));
		
        f = std::pow(f,2);
        A_f = 2*(tau + (tau-1)*f)/tau/tau;
        G_gg *= A_f*A_f;
    }
    else {
        f = 0.5*(log((1+sqrt(1-1/tau)) / (1-sqrt(1-1/tau))));
        double l_2 = std::pow(log((1+sqrt(1-1/tau)) / (1-sqrt(1-1/tau))),2);
        double pi_2 = PI*PI;
        double tau_1 = tau - 1;
        G_gg *= 4*(tau*tau + tau_1*tau_1*(std::pow(l_2-pi_2,2)+4*l_2*pi_2)/16 - 0.5*tau*tau_1*(l_2-pi_2));
        G_gg = G_gg/std::pow(tau,4);
    }

    double G_zz = 1;
    G_zz *= std::pow(x,3) * std::pow((1-std::pow(2*91.2/x,2)),1/2) * (1-std::pow(2*91.2/x,2)+0.75*std::pow(2*91.2/x,4)) * (x>2*91.2);

    double result(1);
    result *=  Lgg * std::pow(x,1) * G_gg * Propag * G_zz;

    // result *=  (a0acc + a1acc*x + a2acc*x*x + a3acc*x*x*x);

    if (x<0 || result<0 || std::isnan(result)) return 0.;

    return result;
}

int main(){
	cout << bw(815, 850, 8.5/850) << endl;	
	return 0;
}
