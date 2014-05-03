typedef void (*ctor)(  );

extern "C" 
{
	ctor start_ctors;
	ctor end_ctors;

	void ctors(  )
	{
		for(ctor *i = &start_ctors; i != &end_ctors; i++)
		{
			(*i)();
		}
	}
}
