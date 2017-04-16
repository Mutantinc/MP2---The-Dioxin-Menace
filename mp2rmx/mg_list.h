//-------------------------------------------------------------------
//MG_LIST.H - LIST MANAGEMENT TEMPLATE
//this list-lib is NOT <vector>-like. It copies the element when adding, but it only gets a pointer when reading !
//-------------------------------------------------------------------
#ifndef _MG_LIST_H_
#define _MG_LIST_H_




template<class T> class mgList
{
public:
	uint _size;
	uint _capa;
	uint _incr;
	T**	 _elts;

/*
	inline mgList( uint initial_capa, uint capa_incr )
	{
		_size = 0;
		_capa = initial_capa;
		if( capa_incr > 0 )
			_incr = capa_incr;
		else
			_incr = 5;
		_elts = new void*[_capa];

	}

	inline mgList( uint initial_capa )
	{
		_size = 0;
		_incr = 5;
		_capa = initial_capa;
		_elts = new void*[_capa];
	}
*/
	inline mgList( const mgList<T>& L )
	{
		_size = L._size;
		_incr = L._incr;
		_capa = L._capa;
		_elts = new T*[_capa];
		for( uint i=0; i< _size; i++ )
			_elts[i] = new T(*L._elts[i]);
	}

	inline mgList()
	{
		_size = 0;
		_incr = 5;
		_capa = 0;
		_elts = NULL;
	}

	inline ~mgList()
	{
		for( uint i=0; i<_size; i++ )
			delete _elts[i];
		delete[] _elts;
	}

//---------------------------------------

	inline uint size()
	{
		return _size;
	}

	inline capacity()
	{
		return _capa;
	}

//---------------------------------------

	inline T* front()
	{
		return at(0);
	}

	inline T* back()
	{
		return at(_size-1);
	}

	inline T* at( uint idx )
	{
		if( idx < _size )
			return _elts[idx];
		else
			return NULL;
	}

//---------------------------------------
	
	inline void add( const T& elt )
	{
		insert( _size, elt);
	}

	inline void insert( uint idx, const T& elt )
	{
		uint i;
		if( _size == _capa )
		{
			_capa += _incr;
			T** newelts = new T*[_capa];
			for( i=0; i< _size; i++ )
				newelts[i] = _elts[i];
			delete[] _elts;
			_elts = newelts;
		}

		T* cpy = new T(elt);
		for( i=idx; i<_size; i++ )
			_elts[i+1] = _elts[i];

		_elts[idx] = cpy;
		_size++;
	}

//---------------------------------------

	inline void erase( uint idx )
	{
		erase( idx, idx );
	}

	inline void erase( uint idx1, uint idx2 )
	{
		uint i;

		if( idx1<_size  && idx2<_size )
		{
			if( idx1 > idx2 )
			{
				uint idxs = idx1;
				idx1 = idx2;
				idx2 = idxs;
			}

			for( i=idx1; i<=idx2; i++ )
			{
				delete _elts[i];
				_size--;
			}

			for( i=idx1; i<_size; i++ )
			{
				_elts[i] = _elts[i+idx2-idx1+1];
			}
		}
	}



};


//END
#endif