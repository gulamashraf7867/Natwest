//
//
//                          MJmatrices.cpp
//
//

#include <MJMatrices.h>
#include <algorithm>


#ifdef _DEBUG
void MJMatrix::UpdateViews()const{

	if(Rows*Columns)for(unsigned long i(0);i<std::min<unsigned long>(NUMBER_OF_MATRIX_ROWS_TO_VIEW,Rows);i++)
		MatrixView[i]=(DoubleDebugView*)RowStarts[i];
}
#endif

MJMatrix& 
MJMatrix::resize(unsigned long rows, unsigned long columns)
{
  if(rows==Rows && columns == Columns)
    return *this;
  else
  {
    if(Start>0)
    {
      delete [] Start;
      delete [] RowStarts;
    }
    Create(rows,columns);
    return *this;
  }

}



void MJMatrix::Create()
{
    if (Rows*Columns>0)
    {
        Start = new double[Rows*Columns];
        RowStarts = new double*[Rows];

        for (unsigned long i=0; i < Rows; i++)
            RowStarts[i] = Start+i*Columns;
    }
    else
        Start=0;

	for (unsigned long i=0; i < Rows; i++)
		for (unsigned long j=0; j < Columns; j++)
			RowStarts[i][j]=0.0;
#ifdef _DEBUG
	UpdateViews();
#endif
}


void MJMatrix::Create(unsigned long rows, unsigned long cols)
{
  Rows = rows;
  Columns = cols;
  Create();
}


MJMatrix::MJMatrix(unsigned long Rows_, unsigned long Columns_) 
                :       Rows(Rows_), 
                        Columns(Columns_)
{
    Create();
}

MJMatrix::MJMatrix(const MJMatrix& original)
                :       Rows(original.Rows), 
                        Columns(original.Columns)
{
    Create();
    if (Rows*Columns > 0) std::copy(original.Start, original.Start+Rows*Columns, Start);    

}

MJMatrix& MJMatrix::operator=(const MJMatrix& original)
{
    if (this != &original)
    {
        if (Rows != original.Rows || Columns != original.Columns)
        {
            if (Start>0)
            {
                delete[] Start;
                delete[] RowStarts;
            }

            Rows = original.Rows;
            Columns = original.Columns;

            Create();
        }

	if (Rows*Columns > 0) std::copy(original.Start, original.Start+Rows*Columns, Start);    

    }

    return *this;
}


MJMatrix::~MJMatrix()
{
    if (Start >0)
    {
        delete[] Start;
        delete[] RowStarts;
    }
}

MJMatrix* MJMatrix::clone() const
{
    return new MJMatrix(*this);   
}
