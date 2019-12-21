#ifndef STOCKBOOK_H
#define STOCKBOOK_H

#include<string>
#include<queue>
#include"Order.cpp"
using namespace std;

class Stockbook 
{
private: 
	std::string ticker;
	priority_queue<Order, vector<Order>, greater<Order>> ask; 
	priority_queue<Order, vector<Order>, less<Order>> bid; 

public:
	Stockbook() { }
	Stockbook(const string&);
	
	void listQueue();
	bool editOrder();
	void cancelOrder();
	void newOrder();
	vector<Order> searchOrder(int,int,int,int);
	void matchOrders();
	
	string getTicker() const;
};

#endif
