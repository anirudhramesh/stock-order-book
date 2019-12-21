#include "Order.h"
#include <ctime>
#include <chrono>
#include <iomanip>

Order::Order(string ticker, unsigned int shares, double price, bool bidOrAsk, bool marketOrLimit, time_t creation=0)
{
	this->ticker=ticker;
	this->k_shares = shares;
	this->bidOrAsk = bidOrAsk;
	
	this->price = price;
	this->marketOrLimit = marketOrLimit;
	if(creation!=0)
		this->creation=creation;
	else
		this->creation = chrono::system_clock::to_time_t(chrono::system_clock::now());
	
	this->orderNumber = ticker + to_string(this->creation).erase(0, 7);
}

bool Order::operator<(const Order comparedOrder) const //if compared order's price and this order's price are equal, check for creation time to establish precedence. Else, just use price.
{
	if (this->marketOrLimit==1 && comparedOrder.marketOrLimit==0) //market order takes precedence
		return false;
	else if(this->marketOrLimit==0 && comparedOrder.marketOrLimit==1)
		return true;
	else
		return (this->price==comparedOrder.price)?(this->creation<comparedOrder.creation):(this->price<comparedOrder.price);
}

bool Order::operator>(const Order comparedOrder) const
{
	if (this->marketOrLimit==1 && comparedOrder.marketOrLimit==0)
		return false;
	else if(this->marketOrLimit==0 && comparedOrder.marketOrLimit==1)
		return true;
	else
		return (this->price==comparedOrder.price)?(this->creation<comparedOrder.creation):(this->price>comparedOrder.price);
}

ostream& operator<<(ostream & out, const Order order)
{
	if (order.getBidOrAsk()==0)
		if(order.getMarketOrLimit()==1)
			out << setw(20) << "Market Order" << setw(20) << "";
		else
			out << setw(20) << order.getPrice() << setw(20) << "";
	else
		if(order.getMarketOrLimit()==1)
			out << setw(20) << "" << setw(20) << "Market Order";
		else
			out << setw(20) << "" << setw(20) << order.getPrice();
	
	out << setw(20) << order.getK_Shares() << setw(20) << order.getOrderNumber() << setw(20)<< order.getTime() <<endl;
	return out;
}

void Order::setKontracts(unsigned int k_shares)
{
	this->k_shares = k_shares;
}
void Order::setPrice(double price)
{
	this->price = price;
}
void Order::setBidORAsk(bool bidOrAsk)
{
	this->bidOrAsk = bidOrAsk;
}

unsigned int Order::getK_Shares() const
{
	return k_shares;
}
string Order::getTicker() const
{
	return ticker;
}
string Order::getOrderNumber() const
{
	return orderNumber;
}
double Order::getPrice() const
{
	return price;
}
bool Order::getBidOrAsk() const
{
	return bidOrAsk;
}
bool Order::getMarketOrLimit() const
{
	return marketOrLimit;
}
string Order::getTime() const
{
	tm creationTime = *localtime(&creation);
	string s = to_string(creationTime.tm_mon)+"/"+to_string(creationTime.tm_mday)+"/"+to_string(creationTime.tm_year+1900)+" "
		  +to_string(creationTime.tm_hour)+":"+to_string(creationTime.tm_min)+":"+to_string(creationTime.tm_sec);
	return s;
}
time_t Order::getCreation() const
{
	return creation;
}


bool sortOnTime(const Order&l, const Order&r)
{
	return l.getCreation() < r.getCreation();

}
bool sortOnPrice(const Order&l, const Order&r)
{
	return l.getPrice() < r.getPrice();
}
bool sortOnKontracts(const Order&l, const Order&r)
{
	return l.getK_Shares() < r.getK_Shares();
}
