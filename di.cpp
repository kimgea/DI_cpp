#include "stdafx.h"
#include <memory>
#include <string>


// Dependency injection for c++


////////////////////////////////////////////////////////
//
//
//		GetData components
//
class IGetData
{
public:
	virtual double get(const std::string & case_no) = 0;
	IGetData() {};
	virtual ~IGetData() {};
};

class GetData : public IGetData
{
public:
	double get(const std::string & case_no)				// Set some status messages here thath Worker needs to access
	{	
		// do db query
		return 2;
	}
	GetData() : IGetData() {};
	~GetData() {};
};

// Mocked object for the GetData component - used by unit-tests to avoid accessing external data
class GetDataMock : public IGetData
{
public:
	double get(const std::string & case_no)				// Set some status messages here thath Worker needs to access
	{
		// create a mock object. DB is not accessed. Used in unit-tests
		return 4;
	}
	GetDataMock() : IGetData() {};
	~GetDataMock() {};
};


////////////////////////////////////////////////////////
//
//
//		Calculation comonents
//

// Man caclulation method
class IDoCalculationOnData
{
public:
	virtual double calculate(double x, double y) = 0;
	IDoCalculationOnData() {};
	virtual ~IDoCalculationOnData() {};
};

// Specialiced calculation class for some few special places thath need this caclulation instead
class Calculation1 : public IDoCalculationOnData
{
public:
	double calculate(double x, double y)				// Set some status messages here thath Worker needs to access
	{
		return x + y;
	}
	Calculation1() : IDoCalculationOnData() {};
	~Calculation1() {};
};
class Calculation2 : public IDoCalculationOnData
{
public:
	double calculate(double x, double y)				// Set some status messages here thath Worker needs to access
	{
		// create a mock object. DB is not accessed. Used in unit-tests
		return x * y;
	}
	Calculation2() : IDoCalculationOnData() {};
	~Calculation2() {};
};


////////////////////////////////////////////////////////
//
//
//		Composition class using the different components
//
class Composition final
{
private:
	std::unique_ptr<IGetData> _get_data;
	std::unique_ptr<IDoCalculationOnData> _calculation;

public:
	double work(const std::string & case_no="")
	{
		double data = 0;

		if (_get_data)
			data = _get_data->get(case_no);

		if (_calculation)
			return _calculation->calculate(data, 1);

		return 0;
	};

	// Setters to inject custom classes
	void set_get_data(std::unique_ptr<IGetData> component)
	{
		_get_data = std::move(component);
	}

	void set_calculation(std::unique_ptr<IDoCalculationOnData> component)
	{
		_calculation = std::move(component);
	}

	// Factory/makers for custom dependency injections
	static Composition make_empty()
	{
		return Composition(nullptr, nullptr);
	}
	static Composition make_default()
	{
		return Composition();
	}
	static Composition make_method_one()
	{
		return Composition(std::make_unique<GetData>(), std::make_unique<Calculation2>());
	}

	// Constructor. with default dependecy injectons
	Composition(
		std::unique_ptr<IGetData> get_data = std::make_unique<GetData>(),
		std::unique_ptr<IDoCalculationOnData> calculation = std::make_unique<Calculation1>())
		: _get_data(std::move(get_data)), _calculation(std::move(calculation))
	{};
	~Composition() {};
	Composition(Composition&&) = default;	// TODO: find out if this is safe
};



///////////////////////////////////////

int main()
{
	Composition worker;
	double ret = worker.work();	// 2 + 1 = 3


	Composition worker_test = Composition::make_empty();	
	worker_test.set_get_data(std::make_unique<GetDataMock>());	// can be used by unit-tests to inject a mock object
	worker_test.set_calculation(std::make_unique<Calculation2>());
	double ret2 = worker_test.work();	// 4 + 1 = 5	-- Mock object returns 4, not 2
	

	Composition worker2 = Composition::make_method_one();
	double ret3 = worker2.work();	// 2 * 1 = 2	--- Used calculation2 method. multiplication instead of addition
	
	return 0;
}
