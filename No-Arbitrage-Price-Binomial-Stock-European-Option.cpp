#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <string>
#include <stdlib.h>
#include <algorithm>


double EuropeanOptionBinomialStock(double S_0, double u, double d, double r, int T, double K, std::string interestType, std::string optionType){
	
	// call-put based parity factor
	int parity;
	if (optionType == "c"){
		parity = 1;
	}
	else if (optionType == "p"){
		parity = -1;
	}
	else{
		throw std::invalid_argument("Option type must be call (c) or put (p)");
	}
	
	// Computing risk-free probabilities and discount factor depending on interest type 
	double p;
	double q;
	double discountFactor;
	
	if (interestType == "s"){
		p = (double) (1+r-d) / ((double) u-d);
		q = (double) 1-p;
		discountFactor = (double) 1/(1+r);
	}
	else if (interestType == "c"){
		p = (double) (std::exp(-r)-d)/((double)(u-d));
		q = 1-p;
		discountFactor = (double) std::exp(-r);
	}
	else{
		throw std::invalid_argument("Interest type must be simple (s) or compound (c)");
	}
	
	std::cout << std::endl;
	std::cout << "Risk-free probability: " << p << std::endl;
	std::cout << std::endl;
	
	// Computing T+1 number of European call option prices at maturity time T
	std::vector<double> optionPayoffs(T+1);
	
	double payoff = 0;
	
	std::cout << "Period " << T << " payoff(s):" << std::endl;
	for(int i = 0; i <= T; i++){
		payoff = parity * (((double) pow(u, T-i)) * ((double) pow(d, i)) * S_0 - K);
		optionPayoffs[i] = (double) std::max(payoff, 0.0);
		
		std::cout << optionPayoffs[i] << std::endl;
	}
	
	std::cout << std::endl;
	
	// Computing option prices period by period recursively
	
	for (int t = T-1; t >= 0; t--){
		std::cout << "Period " << t << " payoff(s):" << std::endl;
		for (int i = 0; i <= t; i++){
			// Overwriting prices on the same maturity time vector as optionPayoffs[i] does not depends on optionPayoffs[i-1]
			optionPayoffs[i] = discountFactor * (((double) p * optionPayoffs[i]) + (double) (q * optionPayoffs[i+1]));
			std::cout << optionPayoffs[i] << std::endl;
		}
		std::cout << std::endl;
	}
	
	return optionPayoffs[0];
}


int main(){
    
    double S_0;
    double u;
    double d;
    double r;
    int T;
    double K;
    std::string interestType;
    std::string optionType;
    
    std::cout << "Current stock price: ";
    std::cin >> S_0;
    
    std::cout << "Up factor: ";
    std::cin >> u;
    
    std::cout << "Down factor: ";
    std::cin >> d;
    
    std::cout << "Risk-free interest rate per period: ";
    std::cin >> r;
    
    std::cout << "Number of periods to maturity: ";
    std::cin >> T;
    
    std::cout << "Strike price: ";
    std::cin >> K;
    
    std::cout << "Interest type (\"s\" for simple, \"c\" for compound): ";
    std::cin >> interestType;
    
    std::cout << "Option type (\"c\" for call, \"p\" for put): ";
    std::cin >> optionType;
    
    double price = EuropeanOptionBinomialStock(S_0, u, d, r, T, K, interestType, optionType);
    std::cout << "No-arbitrage price: " << price << std::endl;
    std::cout << std::endl;
    return 0;
}