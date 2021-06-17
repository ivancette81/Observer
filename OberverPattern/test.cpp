#include "pch.h"
#include <list>

class ISubscriber
{
public:
	virtual void update(double * price) = 0;
};

class Subscriber1 : public ISubscriber
{
private:
	std::list<double> price;

public:
	Subscriber1()
	{
		price.push_back(0.0);
	}
	
	double getLastPrice()
	{
		return price.back();
	}

	void update(double * newPrice)
	{
		price.push_back(*newPrice);
	}
};

class Subscriber2 : public ISubscriber
{
private:
	double price;

public:
	Subscriber2()
	{
		price = 0.0;
	}

	double getPrice()
	{
		return price;
	}

	void update(double *newPrice)
	{
		price = *newPrice;
	}
};
class IPublisher
{
	virtual void attach(ISubscriber *)=0;
	virtual void detach(ISubscriber *)=0;
	virtual void notify(void *)=0;
};
class Publisher : public IPublisher {
	std::list<ISubscriber*> subscribers;
	int numSubscribers = 0;

public:
	void attach(ISubscriber *subscriber)
	{
		subscribers.push_back(subscriber);
	}
	void detach(ISubscriber *subscriber)
	{
		subscribers.remove(subscriber);
	}

	void notify(void * price) {
		for (std::list<ISubscriber*>::iterator it = subscribers.begin(); it !=subscribers.end();it++)
		{
				(*it)->update((double *)price);
		}
	}
};

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(TestHappyPath, HappyPath) {
	ISubscriber *Metamarket = new Subscriber1();
	ISubscriber *APIApp = new Subscriber2();
	Publisher adapterBLB;
	
	adapterBLB.attach(Metamarket);
	adapterBLB.attach(APIApp);

	double price = 100.12;
	adapterBLB.notify((void *)&price);

	EXPECT_DOUBLE_EQ(((Subscriber1*)Metamarket)->getLastPrice(), 100.12);
	EXPECT_DOUBLE_EQ(((Subscriber2*)APIApp)->getPrice(), 100.12);

	adapterBLB.detach(Metamarket);

	price = 100.15;
	adapterBLB.notify((void *)&price);

	EXPECT_DOUBLE_EQ(((Subscriber1*)Metamarket)->getLastPrice(), 100.12);
	EXPECT_DOUBLE_EQ(((Subscriber2*)APIApp)->getPrice(), 100.15);

}