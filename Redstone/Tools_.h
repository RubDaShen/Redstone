#pragma once
#include <vector>



namespace tls
{
	//	Erase-an-element-in-vector function! Note that this function works for
	//	non-pointer vectors. At least...
	template<typename Ty_>
	inline void		EraseElement(std::vector<Ty_>& _MyVec, int& _Where)
	{
		delete			_MyVec[_Where];
		_MyVec.erase(_MyVec.begin() + _Where);
		--_Where;
	};

	//	Deletes an object pointed by a pointer. (Pretty obvious)
	template<typename Ty_>
	inline void		Delete(Ty_ _Ptr)
	{
		if (_Ptr != nullptr)
		{
			delete		_Ptr;
		}
	};
}