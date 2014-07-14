#ifndef IPC_HPP
#define IPC_HPP

#include <definitions.hpp>

#include <cstdint>
#include <cstddef>

#include <list.hpp>

#include "../hal/hal.hpp"

#include "thread.hpp"
#include "process.hpp"

namespace kernel
{
	/**
	  * @brief	Namespace containing functions and structures used for Interprocess Communication
	  */
	namespace ipc
	{	
		/**
		  * @brief	The maximum length of a service name
		  */
		static const size_t	max_service_name = 64;

	        /**
		  * @brief	The error code returned if ne error occurred
		  */
		static const int	noerror		= 0;

		/**
		  * @brief	The error code returned if the there is no service associated with the given name
		  */
		static const int	notfound	= -254;

		/**
		  * @brief	The error code returned if the data field is unaligned
		  */
		static const int	unaligned	= -255;

		/**
		  * @brief	The error code returned if the data field is located in kernel space
		  */
		static const int	illmem		= -256;

		/**
		  * @brief	The error code returned if the data field is unmapped
		  */
		static const int	unmapped	= -257;

		/**
		  * @brief	The error code returned if the system is out of memory
		  */
		static const int	outofmem	= -258;

		/**
		  * @brief	The error code returned if the data field is too big
		  */
		static const int	toobig		= -259;


		/**
		  * @brief	Structure describing an ipc message packet
		  */
		struct packet_t
		{
			/**
			  * @brief	Pointer to data field
			  */
			void		*data;
			
			/**
			  * @brief	Size of the data field
			  */
			size_t		data_size;
		};

		/**
		  * @brief	Structure describing a whole ipc request (question + answer)
		  */
		struct message_t
		{
			/**
			  * @brief	Question 1
			  */
			packet_t	question1;

			/**
			  * @brief	Question 2
			  */
			packet_t	question2;

			/**
			  * @brief	Answer 1
			  */
			packet_t	answer1;

			/**
			  * @brief	Answer 2
			  */
			packet_t	answer2;

			/**
			  * @brief	The id of the calling thread
			  */
			thread::id_t	sender;

			/**
			  * @brief	Return code indicating errors
			  */
			int 		error;
		};
		
		/**
		  * @brief	Shares a block of memory between two processes
		  *
		  *		Do not use to create 'real' shared memory; only used for massage passing
		  *
		  * @param	dest		Virtual memory context to map the block of memory in
		  * @param	src		Virtual memory context containig the block of memory
		  * @param	addr		Address within the source process
		  * @param	n		Number of pages to map
		  */
		uintptr_t shm_map( vmm *dest, vmm *src, uintptr_t addr, size_t n );

		/**
		  * @brief	Class describing a service
		  */
		class service
		{
			friend class servicemgr;

			friend cpu::cpu_state *syscall::handle( cpu::cpu_state *cpu ); 

			public:
				/**
				  * @brief	The contructor
				  *
				  * @param	name		Name of the new service
				  * @param	proc		The process this service is associated with
				  * @param	entry		Entry point of the new service
				  * @param	max_threads	The maximum number of threads allowed to run concurrently
				  * @param	max_data	The maximum size of data handled
				  */
				service( char name[max_service_name], process *proc, uintptr_t entry, size_t max_threads, size_t max_data );
			
				/**
				  * @brief	The destructor
				  */
				~service(  );

				/**
				  * @brief	Fetches a message from the services message queue
				  *
				  * @param	message		Pointer to a memory region to be filled with the mesages information
				  */
				size_t fetch( message_t *message );

				/**
				  * @brief	Finishes a ipc request (returns to the caller)
				  *
				  * @param	message		Pointer to a memory region containing a message object
				  */
				void finish( message_t *message );

				/**
				  * @brief	Function doing a ipc request to the server
				  *
				  * @param	sender			The thread sent the message to the service
				  * @param	message			The message
				  */
				int _do_( thread *sender, message_t *message );

			private:
				/**
				  * @brief	The name of the service
				  */
				char			name[max_service_name];

				/**
				  * @brief	The process this service is associated with
				  */
				process			*proc;
	
				/**
				  * @brief	The entry point of the services ipc handler
				  */
				uintptr_t		entry;

				/**
				  * @brief	The maximum number of threads allowed to run concurrently
				  */
				size_t			max_threads;

				/**
				  * @brief	The maximum size of data handled
				  */
				size_t			max_data;

				/**
				  * @brief	Currently running threads
				  */
				size_t			running_threads;

				/**
				  * @brief	The services mesage queue
				  */
				tools::list<message_t>	message_queue;
		};

		/**
		  * @brief	The service manager
		  */
		class servicemgr
		{
			SINGLETON(servicemgr)

			public:
				/**
				  * @brief	Adds a service to the managers list
				  *
				  * @param	srvc		The service to be added
				  */
				void add( service *srvc );

				/**
				  * @brief	Removes a service from the managers list
				  *
				  * @param	srvc		The service to be removes
				  */
				void remove( service *srvc );

				/**
				  * @brief	Gets a service from the managers list
				  *
				  * @param	name		The name of the service
				  *
				  * @todo			Problem if multiple services has got the same name
				  */
				service *get( const char name[max_service_name] );

				/**
				  * @brief	Gets a service from the managers list
				  *
				  * @param	srvc		The service to get
				  */	
				service *get( service *srvc );

			private:
				/**
				  * @brief	List containing all services managed by the service manager
				  */
				tools::list<service> 	srvcs;
		};
	}
}

#endif
