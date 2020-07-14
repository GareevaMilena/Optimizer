#include"IVector.h"
#include"ILogger.h"
#include<string.h>
#include<new>


namespace
{
	class MiVector : public IVector {
	protected:
		size_t dim{ 0 };
		double *vector{nullptr};
	public:

		MiVector(size_t dim, double *pData) :
			dim(dim),
			vector(pData)
		{};

		~MiVector() {
			delete[] this->vector;
			this->vector = nullptr;
		};

		MiVector* clone() const;
		double norm(NORM norm) const;
		double getCoord(size_t index)const;
		RESULT_CODE setCoord(size_t index, double value);
		size_t getDim()const;
	};
}


double MiVector::getCoord(size_t index) const
{
	if (index >= this->dim)
		return NAN;
	return this->vector[index];
}

RESULT_CODE MiVector::setCoord(size_t index, double value)
{
	if (index >= this->dim)
		return RESULT_CODE::OUT_OF_BOUNDS;
	if (isnan(value))
		return RESULT_CODE::NAN_VALUE;
	this->vector[index] = value;
	return RESULT_CODE::SUCCESS;
}

size_t MiVector::getDim() const
{
	return this->dim;
}

IVector * IVector::createVector(size_t dim, double * pData, ILogger* pLogger)
{
	if (dim == 0)
	{
		if (pLogger)
			pLogger->log("VECTOR: The size of the vector can only be > 0", RESULT_CODE::WRONG_DIM);
		return nullptr;
	}
	if (!pData)
	{
		if (pLogger)
			pLogger->log("VECTOR: You must enter the coordinates of the vector", RESULT_CODE::WRONG_ARGUMENT);
		return nullptr;
	}
	for (size_t i = 0; i < dim; i++)
	{
		if (isnan(pData[i]))
		{
			if (pLogger)
				pLogger->log("VECTOR: You must enter the coordinates of the vector", RESULT_CODE::NAN_VALUE);
			return nullptr;
		}
	}
	double *vector = new(std::nothrow) double[dim];
	if (!vector)
	{
		if (pLogger)
			pLogger->log("VECTOR: Not enough memory", RESULT_CODE::OUT_OF_MEMORY);
		return nullptr;
	}
	memcpy(vector, pData, dim * sizeof(double));
	return new(std::nothrow) MiVector(dim, vector);
}

MiVector * MiVector::clone() const
{
	double *vec = new(std::nothrow) double[this->dim];
	if (!vec)
		return nullptr;
	memcpy(vec, this->vector, this->dim * sizeof(double));
	return new(std::nothrow) MiVector(this->dim, vec);
}

IVector * IVector::add(IVector const * pOperand1, IVector const * pOperand2, ILogger* pLogger)
{
	if (pOperand1 != nullptr && pOperand2 != nullptr)
	{
		if (pOperand1->getDim() == pOperand2->getDim())
		{
			IVector *vector = pOperand1->clone();
			double n;
			for (size_t i = 0; i < pOperand1->getDim(); i++)
			{
				n = pOperand1->getCoord(i) + pOperand2->getCoord(i);
				if (isnan(n))
				{
					delete vector;
					if (pLogger)
						pLogger->log("VECTOR: NAN", RESULT_CODE::CALCULATION_ERROR);
					return nullptr;
				}
				vector->setCoord(i, n);
			}
			return vector;
		}
		else
		{
			if (pLogger)
				pLogger->log("VECTOR: It is impossible to add vectors of different dimensions", RESULT_CODE::WRONG_DIM);
			return nullptr;
		}
	}
	else
	{
		if (pLogger)
			pLogger->log("VECTOR: Vector / vectors not created", RESULT_CODE::WRONG_ARGUMENT);
		return nullptr;
	}
}

IVector *IVector::sub(IVector const * pOperand1, IVector const * pOperand2, ILogger* pLogger)
{
	if (pOperand1 != nullptr && pOperand2 != nullptr)
	{
		if (pOperand1->getDim() == pOperand2->getDim())
		{
			IVector *vector = pOperand1->clone();
			double n;
			for (size_t i = 0; i < pOperand1->getDim(); i++) {
				n = pOperand1->getCoord(i) - pOperand2->getCoord(i);
				if (isnan(n))
				{
					delete vector;
					if (pLogger)
						pLogger->log("VECTOR: NAN", RESULT_CODE::CALCULATION_ERROR);
					return nullptr;
				}
				vector->setCoord(i, n);
			}
			return vector;
		}
		else
		{
			if (pLogger)
				pLogger->log("VECTOR: It is impossible to substruct vectors of different dimensions", RESULT_CODE::WRONG_DIM);
			return nullptr;
		}
	}
	else
	{
		if (pLogger)
			pLogger->log("VECTOR: Vector / vectors not created", RESULT_CODE::WRONG_ARGUMENT);
		return nullptr;
	}
}

IVector * IVector::mul(IVector const * pOperand1, double scaleParam, ILogger* pLogger)
{
	if (pOperand1 != nullptr && !isnan(scaleParam))
	{
		IVector *vector = pOperand1->clone();
		double n;
		for (size_t i = 0; i < pOperand1->getDim(); i++) {
			n = pOperand1->getCoord(i) * scaleParam;
			if (isnan(n))
			{
				delete vector;
				if (pLogger)
					pLogger->log("VECTOR: NAN", RESULT_CODE::CALCULATION_ERROR);
				return nullptr;
			}
			vector->setCoord(i, n);
		}
		return vector;
	}
	else
	{
		if (pLogger)
			pLogger->log("VECTOR: Vector not created or parameter not defined", RESULT_CODE::WRONG_ARGUMENT);
		return nullptr;
	}
}

double IVector::mul(IVector const * pOperand1, IVector const * pOperand2, ILogger* pLogger)
{
	if (pOperand1 != nullptr && pOperand2 != nullptr)
	{
		if (pOperand1->getDim() == pOperand2->getDim())
		{
			double s = 0;
			for (size_t i = 0; i < pOperand1->getDim(); i++)
				s += (pOperand1->getCoord(i) * pOperand2->getCoord(i));
			if (isnan(s))
			{
				if (pLogger)
					pLogger->log("VECTOR: NAN", RESULT_CODE::CALCULATION_ERROR);
				return NAN;
			}
			return s;
		}
		else
		{
			if (pLogger)
				pLogger->log("VECTOR: It is impossible to multiply vectors of different dimensions", RESULT_CODE::WRONG_DIM);
			return NAN;
		}
	}
	else
	{
		if (pLogger)
			pLogger->log("VECTOR: Vector / vectors not created", RESULT_CODE::WRONG_ARGUMENT);
		return NAN;
	}
}

RESULT_CODE IVector::equals(IVector const * pOperand1, IVector const * pOperand2, NORM norm, double tolerance, bool * result, ILogger * pLogger)
{
	if (isnan(tolerance) || tolerance < 0)
	{
		if (pLogger)
			pLogger->log("VECTOR: NUN", RESULT_CODE::NAN_VALUE);
		return RESULT_CODE::NAN_VALUE;
	}
	if (pOperand1 != nullptr && pOperand2 != nullptr)
	{
		if (pOperand1->getDim() == pOperand2->getDim())
		{
			double n1 = pOperand1->norm(norm);
			double n2 = pOperand2->norm(norm);
			if (isnan(n1) || isnan(n2))
			{
				if (pLogger)
					pLogger->log("VECTOR: NUN", RESULT_CODE::NAN_VALUE);
				return RESULT_CODE::NAN_VALUE;
			}
			double s = abs(n1 - n2);
			if (isnan(s))
			{
				if (pLogger)
					pLogger->log("VECTOR: NUN", RESULT_CODE::NAN_VALUE);
				return RESULT_CODE::NAN_VALUE;
			}
			if (s <= tolerance)
				*result = true;
			else
				*result = false;
			return RESULT_CODE::SUCCESS;
		}
		else
		{
			if (pLogger)
				pLogger->log("VECTOR: It is impossible to equals vectors of different dimensions", RESULT_CODE::WRONG_DIM);
			return RESULT_CODE::WRONG_DIM;
		}
	}
	else
	{
		if (pLogger)
			pLogger->log("VECTOR: Vector / vectors not created", RESULT_CODE::WRONG_ARGUMENT);
		return RESULT_CODE::WRONG_ARGUMENT;
	}
}

/*double IVector::norm(IVector const * pVector, NORM norm, ILogger * pLogger)
{
	if (pVector != nullptr)
	{ 
		double n = 0;
		if (norm == NORM::NORM_1)
		{
			for (size_t i = 0; i < pVector->getDim(); i++)
				n += abs(pVector->getCoord(i));
			return n;
		}
		if (norm == NORM::NORM_2)
		{
			for (size_t i = 0; i < pVector->getDim(); i++)
				n += abs(pVector->getCoord(i)) * abs(pVector->getCoord(i));
			n = sqrt(n);
			return n;
		}
		if (norm == NORM::NORM_INF)
		{
			for (size_t i = 0; i < pVector->getDim(); i++)
			{
				if (fabs(pVector->getCoord(i)) > n)
					n = fabs(pVector->getCoord(i));
			}
			return n;
		}
	}
	else
	{
		pLogger->log("Vector not created", RESULT_CODE::WRONG_ARGUMENT);
		return NAN;
	}
	pLogger->log("Wrong type of norm entered", RESULT_CODE::WRONG_ARGUMENT);
	return NAN;
}*/
double MiVector::norm(NORM norm)const
{
	double n = 0;
	if (norm == NORM::NORM_1)		
	{
		for (size_t i = 0; i < dim; i++)
			n += abs(vector[i]);
		if (isnan(n))
			return NAN;
		return n;
	}
	if (norm == NORM::NORM_2)
	{
		for (size_t i = 0; i < dim; i++)
			n += abs(vector[i]) * abs(vector[i]);
		n = sqrt(n);
		if (isnan(n))
			return NAN;
		return n;
	}
	if (norm == NORM::NORM_INF)
	{
		for (size_t i = 0; i < dim; i++)
		{
			if (fabs(vector[i]) > n)
				n = fabs(vector[i]);
		}
		if (isnan(n))
			return NAN;
		return n;
	}
	return NAN;
}

IVector::~IVector() {};
