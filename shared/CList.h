#pragma once

template <class T>
class CList
{
public:
					  CList( T _obj );
					 ~CList();

	void			  Release();

	void			  Add( T _obj );
	CList<T>		* Del( T _obj );

	int				  Size();
	T				  Get( int i );

public:
	CList			* prev;
	CList			* next;

	T				  obj;
};

template <class T>
CList<T>::CList( T _obj )
{
	prev	= NULL;
	next	= NULL;

	obj		= _obj;
}

template <class T>
CList<T>::~CList()
{
	if( prev != NULL )
		prev->next = next;
	if( next != NULL )
		next->prev = prev;

	prev	= NULL;
	next	= NULL;
}

template <class T>
void CList<T>::Release()
{
	if( next != NULL )
	{
		next->Release();
		next = NULL;
	}

	if( prev != NULL )
		prev->next = NULL;

	delete this;
}

template <class T>
void CList<T>::Add( T _obj )
{
	if( obj == NULL )
	{
		obj = _obj;
		return;
	}

	if( next != NULL )
	{
		next->Add( _obj );
		return;
	}

	next = new CList( _obj );
}

template <class T>
CList<T> * CList<T>::Del( T _obj )
{
	if( obj == _obj )
	{
		obj = NULL;
		return this;
	}

	if( next != NULL )
	{
		//Next ?
		return next->Del( _obj );
	}

	return NULL;
}

template <class T>
int CList<T>::Size()
{
	if( !this )
		return 0;

	int iSize = 1;

	if( next != NULL )
		iSize += next->Size();

	return iSize;
}

template <class T>
T CList<T>::Get( int i )
{
	if( i == 0 )
		return obj;

	int j = 1;

	CList<T> * item = next;
	while( item )
	{
		if( j == i )
			return item->obj;

		item = item->next;

		j++;
	}

	return NULL;
}