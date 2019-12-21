#include "StockBook.cpp"
#include <iomanip>
#include <algorithm>
#include <string>
#include <stdexcept>
using namespace std;

int newOrder(vector<Stockbook>&);
int editOrder(vector<Stockbook>&);
void cancelOrder(vector<Stockbook>&);
void displayAllOrders(vector<Stockbook>&);
void searchOrders(vector<Stockbook>&);

int main()
{
	vector<Stockbook> Book;
	int option, orderBookNumber;
	
	do{
		cout << endl << "Menu:" << endl << "1: PLACE ORDER\n2: EDIT ORDER\n3: CANCEL ORDER\n4: SEARCH ORDERS\n5: DISPLAY ALL ORDERS\n6: QUIT\nEnter option: ";
		cin >> option;
		
		orderBookNumber=-1;
		switch (option)
		{
			case 1: orderBookNumber=newOrder(Book); break;
			
			case 2: orderBookNumber=editOrder(Book); break;
			
			case 3: cancelOrder(Book); break;
			
			case 4: searchOrders(Book); break;
			
			case 5: displayAllOrders(Book); break;
			
			case 6: cout << endl << "Thank you!" << endl; break;
			
			default: cout << endl << "Invalid option! Please select again!" << endl; break;
		}
		
		if(orderBookNumber!=-1){
			cout << endl << "Order execution system called." << endl;
			Book[orderBookNumber].matchOrders();
		}
	} while(option!=6);
	
	return 0;
}

int newOrder(vector<Stockbook>& Book)
{
	string ticker;
	int j;
	
	cout << "Enter Stock Ticker: ";
	cin >> ticker;
	
	for (j = 0; j < Book.size() && Book[j].getTicker() != ticker; j++);
	
	if(j==Book.size())
		Book.push_back(Stockbook(ticker));
		
	Book[j].newOrder();
	
	return j;
}

int editOrder(vector<Stockbook>& Book)
{
	string ticker;
	int orderBookNumber;
	bool successfulEdit;
	
	cout << "Enter Ticker: "; cin >> ticker;
	
	for(orderBookNumber=0; orderBookNumber<Book.size() && Book[orderBookNumber].getTicker()!=ticker; orderBookNumber++);
	
	if(orderBookNumber==Book.size())
	{
		cout << endl << "No orders exist for this ticker!" << endl;
		return -1;
	}
	
	successfulEdit = Book[orderBookNumber].editOrder();
	
	if(successfulEdit)
		return orderBookNumber;
	else
		return -1;
}

void cancelOrder(vector<Stockbook>& Book)
{
	string ticker;
	cout << "\nENTER TICKER: ";
	cin >> ticker;
	
	int i;
	for(i=0; i<Book.size() && Book[i].getTicker()!=ticker; i++);
	
	if(i==Book.size())
	{
		cout << endl << "The order book for this ticker does not exist." << endl;
		return;
	}
	
	else
		Book[i].cancelOrder();
}

void displayAllOrders(vector<Stockbook>& Book)
{
	if(Book.size()==0)
	{
		cout << endl << "There are currently no order books initiated!" << endl;
		return;
	}
	
	for(int i=0; i<Book.size(); i++)
		Book[i].listQueue();
}

void searchOrders(vector<Stockbook>& Book)
{
	int options[5], N=0, orderBookNumber;
	string ticker;
	vector<Order> resultOrders;
	
	cout << endl << "Select options for search below." << endl << endl;
	
	do{
		cout << endl << "Options:" << endl
			 << "1) Search by ticker" << endl
			 << "2) Search all order books" << endl
			 << "Enter option: ";
		cin >> options[0];
		if(options[0]!=1 && options[0]!=2) cout << endl << "Invalid option! Enter again." << endl;
	}while (options[0]!=1 && options[0]!=2);
	
	if(options[0]==1)
	{
		cout << endl << "Enter Ticker to search in: ";
		cin >> ticker;
		
		for(orderBookNumber=0; orderBookNumber<Book.size() && Book[orderBookNumber].getTicker()!=ticker; orderBookNumber++);
		if(orderBookNumber==Book.size()){
			cout << endl << "Ticker does not exist! Reselect search options from beginning!" << endl;
			return;
		}
	}
	
	do{
		cout << endl << "Options: " << endl
			 << "1) List Bid and Ask orders" << endl
			 << "2) List only Bid orders" << endl
			 << "3) List only Ask orders" << endl
			 << "Enter option: ";
		cin >> options[4];
		if(options[4]!=1 && options[4]!=2 && options[4]!=3) cout << endl << "Invalid option! Enter again." << endl;
	}while (options[4]!=1 && options[4]!=2 && options[4]!=3);
	
	do{
		cout << endl << "Options: " << endl
			 << "1) Display all possible results" << endl
			 << "2) Display the top N results" << endl
			 << "Enter option: ";
		cin >> options[1];
		if(options[1]!=1 && options[1]!=2) cout << endl << "Invalid option! Enter again." << endl;
	}while (options[1]!=1 && options[1]!=2);
	
	if(options[1]==2)
	{
		cout << "Enter a value for N: ";
		cin >> N;
	}
	
	do{
		cout << endl << "Options: " << endl
			 << "1) See unsorted results" << endl
			 << "2) Sort results ascending by criteria (select on next option)" << endl
			 << "3) Sort results descending by criteria (select on next option)" << endl
			 << "Enter option: ";
		cin >> options[2];
		if(options[2]!=1 && options[2]!=2 && options[2]!=3) cout << endl << "Invalid option! Enter again." << endl;
	}while (options[2]!=1 && options[2]!=2 && options[2]!=3);
	
	if(options[2]==2 || options[2]==3)
	{
		do{
			cout << endl << "Options (Bid and Ask orders will still be listed separately but sorted within by the selected criteria): " << endl
				 << "1) Sort by shares / contract size" << endl
				 << "2) Sort by price" << endl
				 << "3) Sort by time" << endl
				 << "Enter option: ";
			cin >> options[3];
			if(options[3]!=1 && options[3]!=2 && options[3]!=3) cout << endl << "Invalid option! Enter again." << endl;
		}while (options[3]!=1 && options[3]!=2 && options[3]!=3);
	}
	else
		options[3]=0;
	
	if(options[0]==1)
		resultOrders=Book[orderBookNumber].searchOrder(N, options[2], options[3], options[4]);
	else
		for(int j=0; j<Book.size(); j++){
			vector<Order> tempOrders=Book[j].searchOrder(N, options[2], options[3], options[4]);
			resultOrders.insert(resultOrders.end(), tempOrders.begin(), tempOrders.end());
		}
	
	cout << endl << "Search Results: " << endl;
	cout << setw(20) << "TICKER" << setw(20) << "BID" << setw(20) << "ASK" << setw(20) << "QUANTITY " << setw(20) << "ORDERNUMBER " << setw(20)<<"TIME OF CREATION" << endl;
	for(int j=0; j<resultOrders.size(); j++)
		cout << setw(20) << resultOrders[j].getTicker() << resultOrders[j];
}
