typedef void (*dtor)(  );

extern "C" 
{
	dtor start_dtors;
	dtor end_dtors;

	void dtors(  )
	{
		for(dtor *i = &start_dtors; i != &end_dtors; i++)
		{
			(*i)();
		}
	}
}
