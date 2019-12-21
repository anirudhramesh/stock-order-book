
#include "StockBook.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
using namespace std;

Stockbook::Stockbook(const string& ticker)
{
	this->ticker = ticker;
}

void Stockbook::listQueue()
{
	cout << endl << "TICKER: " << ticker << endl << endl;
	
	if(bid.empty() && ask.empty())
	{
		cout << endl << "No orders for this ticker!" << endl;
		return;
	}		
		
	vector<Order> tempOrders;
	
	cout << setw(20) << "BID" << setw(20) << "ASK" << setw(20) << "QUANTITY" << setw(20) << "ORDER NUMBER" << setw(20)<<"TIME OF CREATION" << endl;
	
	while (!ask.empty())
	{
		tempOrders.push_back(ask.top());
		ask.pop();
	}
	
	while (tempOrders.size()>0)
	{
		cout << tempOrders.back();
		ask.push(tempOrders.back());
		tempOrders.pop_back();
	}
		
	
	while (!bid.empty())
	{
		cout << bid.top();
		tempOrders.push_back(bid.top());
		bid.pop();
	}
	
	while (tempOrders.size()>0)
	{
		bid.push(tempOrders.back());
		tempOrders.pop_back();
	}
}

string Stockbook::getTicker() const
{
	return ticker;
}

void Stockbook::cancelOrder()
{
	int cmenu, found=0;
	string orderNumberCancel;
	
	if(ask.empty() && bid.empty())
	{
		cout << endl << "Order book for this ticker is empty!" << endl;
		return;
	}
	
	cout << "Look for the ORDER NUMBER of the order you would like to cancel: \n";
	
	listQueue();
	
	do{
		cout << "\nEnter 0 to cancel BID order and 1 to cancel ASK order: ";
		cin >> cmenu; cout << endl;
		if(cmenu!=0 && cmenu!=1) cout << "Wrong option. Enter again!" << endl;
	}while(cmenu!=0 && cmenu!=1);
	
	cout << "Enter ORDER NUMBER: ";
	cin >> orderNumberCancel;

	if (cmenu == 1)
	{
		priority_queue<Order> new_ask;
		while (!ask.empty() && !found)
		{
			if (ask.top().getOrderNumber() != orderNumberCancel)
			{
				new_ask.push(ask.top());
				ask.pop();
			}
			else
			{
				ask.pop();
				found=1;
			}
		}
		
		if(!found) cout << endl << "This order number does not exist in the ASK queue! Reselect cancel from main menu and start again." << endl;
		else cout << endl << "Order cancelled!" << endl;
		
		while (!new_ask.empty())
		{
			ask.push(new_ask.top());
			new_ask.pop();
		}
	}
	else
	{
		priority_queue<Order> new_bid;
		while (!bid.empty() && !found)
		{
			if (bid.top().getOrderNumber() != orderNumberCancel)
			{
				new_bid.push(bid.top());
				bid.pop();
			}
			else
			{
				bid.pop();
				found=1;
			}
		}
		
		if(!found) cout << endl << "This order number does not exist in the BID queue! Reselect cancel from main menu and start again." << endl;
		else cout << endl << "Order cancelled!" << endl;
		
		while (!new_bid.empty())
		{
			bid.push(new_bid.top());
			new_bid.pop();
		}
	}
}

bool Stockbook::editOrder()
{
	int found=0;
	string orderNumber;
	bool bidOrAsk;
	unsigned int k_shares;
	double price;
	vector<Order> tempOrders;
	
	cout << "Look for the ORDER NUMBER of the order you would like to change: \n";
	
	this->listQueue();
	
	do{
		cout << endl << "Would you like to edit a (0) Bid or (1) Ask order?: ";
		cin >> bidOrAsk;
		if(bidOrAsk!=0 && bidOrAsk!=1) cout << endl << "Invalid option! Enter again." << endl;
	} while(bidOrAsk!=0 && bidOrAsk!=1);
	
	cout << "\nEnter target ORDER NUMBER: ";
	cin >> orderNumber;

	if (bidOrAsk == 1)
	{
		if(this->ask.empty())
		{
			cout << endl << "There are no Ask orders to edit in this order book! Please select a different option and try again!" << endl;
			return false;
		}
		
		while(!this->ask.empty() && this->ask.top().getOrderNumber() != orderNumber)
		{
			tempOrders.push_back(this->ask.top());
			this->ask.pop();
		}
		
		if(this->ask.empty()){
			cout << endl << "Order Number not found!" << endl;
			return false;
		}
		
		do{
			cout << endl << "Enter new amount of shares: ";
			cin >> k_shares;
			if(k_shares<0) cout << endl << "Quantity of shares cannot be negative! Enter again." << endl;
		} while(k_shares<0);
		
		do{
			cout << "Enter new price per share (enter 0 for market order): ";
			cin >> price;
			if(price<0) cout << endl << "Price cannot be negative! Enter again." << endl;
		} while(price<0);
		
		Order replacedOrder = Order(this->ticker, k_shares, price, bidOrAsk, price==0);
		
		this->ask.pop();
		this->ask.push(replacedOrder);
		while(tempOrders.size()>0)
		{
			this->ask.push(tempOrders.back());
			tempOrders.pop_back();
		}
	}
	else
	{
		if(this->bid.empty())
		{
			cout << endl << "There are no Bid orders to edit in this order book! Please select a different option and try again!" << endl;
			return false;
		}
		
		while(!this->bid.empty() && this->bid.top().getOrderNumber() != orderNumber)
		{
			tempOrders.push_back(this->bid.top());
			this->bid.pop();
		}
		
		if(this->bid.empty()){
			cout << endl << "Order Number not found!" << endl;
			return false;
		}
		
		do{
			cout << endl << "Enter new amount of shares: ";
			cin >> k_shares;
			if(k_shares<0) cout << endl << "Quantity of shares cannot be negative! Enter again." << endl;
		} while(k_shares<0);
		
		do{
			cout << "Enter new price per share (enter 0 for market order): ";
			cin >> price;
			if(price<0) cout << endl << "Price cannot be negative! Enter again." << endl;
		} while(price<0);
		
		Order replacedOrder = Order(this->ticker, k_shares, price, bidOrAsk, price==0);
		
		this->bid.pop();
		this->bid.push(replacedOrder);
		while(tempOrders.size()>0)
		{
			this->bid.push(tempOrders.back());
			tempOrders.pop_back();
		}
	}
	
	return true;
}

void Stockbook::newOrder()
{
	unsigned int shares;
	double price=0;
	bool marketOrLimit, bidOrAsk;
				
	cout << "Enter number of shares: ";
	cin >> shares;
	if (shares <= 0)
	{
		throw invalid_argument("\nShares must be greater than 0!\n");
		return;
	}
	
	do {
		cout << endl << "What type of order?" << endl << "0 - Limit order" << endl << "1 - Market order" << endl << "Enter option: ";
		cin >> marketOrLimit;
		if(marketOrLimit!=0 && marketOrLimit!=1) cout << endl << "Enter either 0 or 1 for the order type" << endl;
	} while(marketOrLimit!=0 && marketOrLimit!=1);
	
	if(marketOrLimit==0){
		cout << "\Enter Order Price: ";
		cin >> price;
		if (price <= 0)
		{
			cerr << "\nPrice must be positive! Please select the option to enter an order again!\n";
			return;
		}
	}
	else
		price==0;
		
	do{
		cout << endl << "What type of order?: \n0 Bid (buy) Order\n1 Ask (sell) Order\nEnter option: ";
		cin >> bidOrAsk;
		if(bidOrAsk!=0 && bidOrAsk!=1) cout << endl << "Enter either 0 or 1 for the order type" << endl;
	} while(bidOrAsk!=0 && bidOrAsk!=1);

	Order thisOrder(ticker, shares, price, bidOrAsk, marketOrLimit);
	
	if (bidOrAsk == 1)//add an order to that Stockbook's ASK priority queue
		this->ask.emplace(thisOrder);
		
	else //add an order to that Stockbook's BID prioirity queue
		this->bid.emplace(thisOrder);
	
}

vector<Order> Stockbook::searchOrder(int N, int shouldSort, int sortBy, int bidOrAsk)
{
	vector<Order> resultOrdersBid, resultOrdersAsk;
	int N1=N;
	
	if(bidOrAsk==1 || bidOrAsk==2)
	{		
		while(!this->bid.empty()){
			resultOrdersBid.push_back(this->bid.top());
			this->bid.pop();
		}
		
		for(int i=0; i<resultOrdersBid.size(); i++)
			this->bid.push(resultOrdersBid[i]);
	}
	
	if(bidOrAsk==1 || bidOrAsk==3)
	{		
		while(!this->ask.empty()){
			resultOrdersAsk.push_back(this->ask.top());
			this->ask.pop();
		}
		
		for(int i=0; i<resultOrdersAsk.size(); i++)
			this->ask.push(resultOrdersAsk[i]);
	}
	
	if(shouldSort!=1)
	{
		switch(sortBy)
		{
			case 1: sort(resultOrdersBid.begin(), resultOrdersBid.end(), sortOnKontracts);
					sort(resultOrdersAsk.begin(), resultOrdersAsk.end(), sortOnKontracts); break;
			
			case 2: sort(resultOrdersBid.begin(), resultOrdersBid.end(), sortOnPrice);
					sort(resultOrdersAsk.begin(), resultOrdersAsk.end(), sortOnPrice); break;
			
			case 3: sort(resultOrdersBid.begin(), resultOrdersBid.end(), sortOnTime);
					sort(resultOrdersAsk.begin(), resultOrdersAsk.end(), sortOnTime); break;
		}
	}
	
	if(shouldSort==2){
		reverse(resultOrdersAsk.begin(), resultOrdersAsk.end());
		reverse(resultOrdersBid.begin(), resultOrdersBid.end());
	}
	
	if(N!=0)
	{
		if(N<resultOrdersAsk.size()) resultOrdersAsk.erase(resultOrdersAsk.end()-N, resultOrdersAsk.end());
		if(N<resultOrdersBid.size()) resultOrdersBid.erase(resultOrdersBid.end()-N, resultOrdersBid.end());
	}
	
	resultOrdersAsk.insert(resultOrdersAsk.end(), resultOrdersBid.begin(), resultOrdersBid.end());
	
	return resultOrdersAsk;
}

void Stockbook::matchOrders()
{
	vector<Order> ordersExecuted[2], marketOrdersBid, marketOrdersAsk;
	Order tempOrder;
	
	//Logic:
	//1) if there is a market order on either side of the book, exhaust it against all limit orders on the opposite side
	//2) if there are limit orders on either side of the book that are greater / lesser than the top order on the opposite side, execute at the price of the oldest order available at the lowest / highest price on the opposite side
	
	//pop out all the market orders, and match them with the remaining limit orders as prioritised.
	while(!this->ask.empty() && this->ask.top().getMarketOrLimit()==1)
	{
		marketOrdersAsk.push_back(this->ask.top());
		this->ask.pop();
	}
	
	while(!this->bid.empty() && this->bid.top().getMarketOrLimit()==1)
	{
		marketOrdersBid.push_back(this->bid.top());
		this->bid.pop();
	}
	
	int marketOrdersExecuted=0;
	while(!marketOrdersExecuted)
	{
		if(marketOrdersBid.size()>0 && !this->ask.empty())
		{
			if(this->ask.top().getK_Shares() < marketOrdersBid.back().getK_Shares())
			{
				marketOrdersBid.back().setKontracts(marketOrdersBid.back().getK_Shares() - this->ask.top().getK_Shares());
				ordersExecuted[0].push_back(Order(this->getTicker(), this->ask.top().getK_Shares(), this->ask.top().getPrice(), 0, 0, this->ask.top().getCreation()));
				ordersExecuted[1].push_back(Order(this->getTicker(), this->ask.top().getK_Shares(), this->ask.top().getPrice(), 0, 0, marketOrdersBid.back().getCreation()));
				this->ask.pop();
			}
			else
			{
				ordersExecuted[0].push_back(Order(this->getTicker(), marketOrdersBid.back().getK_Shares(), this->ask.top().getPrice(), 0, 0, this->ask.top().getCreation()));
				ordersExecuted[1].push_back(Order(this->getTicker(), marketOrdersBid.back().getK_Shares(), this->ask.top().getPrice(), 0, 0, marketOrdersBid.back().getCreation()));
				if(this->ask.top().getK_Shares() == marketOrdersBid.back().getK_Shares())
					this->ask.pop();
				else
				{
					tempOrder = this->ask.top();
					this->ask.pop();
					tempOrder.setKontracts(tempOrder.getK_Shares() - marketOrdersBid.back().getK_Shares());
					this->ask.push(tempOrder);
				}
				marketOrdersBid.pop_back();
			}
		}
		else if(marketOrdersAsk.size()>0 && !this->bid.empty())
		{
			if(this->bid.top().getK_Shares() < marketOrdersAsk.back().getK_Shares())
			{
				marketOrdersAsk.back().setKontracts(marketOrdersAsk.back().getK_Shares() - this->bid.top().getK_Shares());
				ordersExecuted[0].push_back(Order(this->getTicker(), this->bid.top().getK_Shares(), this->bid.top().getPrice(), 0, 0, this->bid.top().getCreation()));
				ordersExecuted[1].push_back(Order(this->getTicker(), this->bid.top().getK_Shares(), this->bid.top().getPrice(), 0, 0, marketOrdersAsk.back().getCreation()));
				this->bid.pop();
			}
			else
			{
				ordersExecuted[0].push_back(Order(this->getTicker(), marketOrdersAsk.back().getK_Shares(), this->bid.top().getPrice(), 0, 0, this->bid.top().getCreation()));
				ordersExecuted[1].push_back(Order(this->getTicker(), marketOrdersAsk.back().getK_Shares(), this->bid.top().getPrice(), 0, 0, this->bid.top().getCreation()));
				if(this->bid.top().getK_Shares() == marketOrdersAsk.back().getK_Shares())
					this->bid.pop();
				else
				{
					tempOrder=this->bid.top();
					this->bid.pop();
					tempOrder.setKontracts(tempOrder.getK_Shares() - marketOrdersAsk.back().getK_Shares());
					this->bid.push(tempOrder);
				}
				marketOrdersAsk.pop_back();
			}
		}
		else
			marketOrdersExecuted=1;
	}
	
	while(marketOrdersAsk.size()>0)
	{
		this->ask.push(marketOrdersAsk.back());
		marketOrdersAsk.pop_back();
	}
	
	while(marketOrdersBid.size()>0)
	{
		this->bid.push(marketOrdersBid.back());
		marketOrdersBid.pop_back();
	}
	
	//check if either queue is empty, if not check if any of them have market orders unfulfilled (which should be matched by above loops, if not check if highest bid is higher than lowest ask
	while(!this->bid.empty() && !this->ask.empty() && this->bid.top().getMarketOrLimit()!=1 && this->ask.top().getMarketOrLimit()!=1 && this->bid.top().getPrice() >= this->ask.top().getPrice())
	{
		double price;
		time_t creation;
		if(this->bid.top().getK_Shares() < this->ask.top().getK_Shares())
		{
			tempOrder=this->ask.top();
			tempOrder.setKontracts(this->ask.top().getK_Shares() - this->bid.top().getK_Shares());
			this->ask.pop(); this->ask.push(tempOrder);
			
			price = (this->bid.top().getCreation() < this->ask.top().getCreation())?this->bid.top().getPrice():this->ask.top().getPrice();
			ordersExecuted[0].push_back(Order(this->getTicker(), this->bid.top().getK_Shares(), price, 0, 0, this->bid.top().getCreation()));
			ordersExecuted[1].push_back(Order(this->getTicker(), this->bid.top().getK_Shares(), price, 0, 0, this->ask.top().getCreation()));
			this->bid.pop();
		}
		else
		{
			price = (this->bid.top().getCreation() < this->ask.top().getCreation())?this->bid.top().getPrice():this->ask.top().getPrice();
			ordersExecuted[0].push_back(Order(this->getTicker(), this->ask.top().getK_Shares(), price, 0, 0, this->bid.top().getCreation()));
			ordersExecuted[1].push_back(Order(this->getTicker(), this->ask.top().getK_Shares(), price, 0, 0, this->ask.top().getCreation()));
			
			if(this->bid.top().getK_Shares() == this->ask.top().getK_Shares())
				this->bid.pop();
			else
			{
				tempOrder=this->bid.top();
				this->bid.pop();
				tempOrder.setKontracts(tempOrder.getK_Shares() - this->ask.top().getK_Shares());
				this->bid.push(tempOrder);
			}
			this->ask.pop();
		}
	}
	
	if(ordersExecuted[0].size()==0)
	{
		cout << endl << "No matching orders found!" << endl;
		return;
	}
	
	cout << endl << "The following orders were executed: " << endl;
	cout << endl << setw(20) << "ORDER NUMBER" << setw(20) << "TICKER" << setw(20) << "PRICE" << setw(20) << "QUANTITY" << endl;
	for(int i=0; i<ordersExecuted[0].size(); i++)
	{
		cout << setw(20) << ordersExecuted[0][i].getOrderNumber() << setw(20) << ordersExecuted[0][i].getTicker() << setw(20) << ordersExecuted[0][i].getPrice() << setw(20) << ordersExecuted[0][i].getK_Shares() << endl;
		cout << "was matched with" << endl;
		cout << setw(20) << ordersExecuted[1][i].getOrderNumber() << setw(20) << ordersExecuted[1][i].getTicker() << setw(20) << ordersExecuted[1][i].getPrice() << setw(20) << ordersExecuted[1][i].getK_Shares() << endl;
	}
}

