#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class Order
{

private:
	string ticker;
	unsigned int k_shares;
	double price;
	bool bidOrAsk; //bid is 0, ask is 1
	bool marketOrLimit; //limit is 0, market is 1
	
	string orderNumber;
	time_t creation;
	static int numberOfOrders;

public:
	Order(string, unsigned int, double, bool, bool, time_t);
	Order() { }

	string getTicker() const;
	unsigned int getK_Shares() const;
	double getPrice() const;
	bool getBidOrAsk() const;
	bool getMarketOrLimit() const;
	string getOrderNumber() const;
	time_t getCreation() const;
	string getTime() const;

	void setTicker(const string);
	void setKontracts(unsigned int);
	void setPrice(double);
	void setBidORAsk(bool);

	bool operator>(const Order) const;
	bool operator<(const Order) const;

};

#endif
