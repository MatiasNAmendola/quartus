#ifndef TAR_HPP
#define TAR_HPP

#include <cstdint>
#include <cstddef>

namespace kernel
{
	/**
	  * @brief	Basic implementation of the  tar file format
	  */
	class tar
	{
		private:
			/**
			  * @brief	Data structure describing a tar-header
			  */
			struct header_t
			{
				/**
				  * @brief	The name of the file described by the header
				  */
				char 	name[100];

				/**
				  * @brief	The mode of the file described by the header
				  */
				char 	mode[8];

				/**
				  * @brief	The uid of the file described by the header
				  */
				char 	uid[8];

				/**
				  * @brief	The gid of the file described by the header
				  */
				char 	gid[8];

				/**
				  * @brief	The size of the file described by the header
				  */
				char 	size[12];

				/**
				  * @brief	The mtime of the file described by the header
				  */
				char 	mtime[12];

				/**
				  * @brief	The checksum of the file described by the header
				  */
				char 	chksum[8];

				/**
				  * @brief	The typeflag of the file described by the header
				  */
				char 	typeflag[1];
				
				/**
				  * @brief	The remaining, unsused 355 bytes of the tar-header
				  */
				char	foo[355];
			};

		public:
			/**
			  * @brief	The constructor
			  *
			  * @param	addr		The address of the tar-image
			  */
			tar( uintptr_t addr );

			/**
			  * @brief	Reads the content of an file within the tar
			  *
			  * @param	name		Name of the file to be read
			  * @param	buffer		The buffer, the read data will be stored in
			  * @param	size		Number of bytes to be read
			  * @return			Number of bytes actually read
			  */
			size_t read( const char *name, char *buffer, size_t size );

		private:
			/**
			  * @brief	Number of objects within the tar
			  */
			size_t		objects;

			/**
			  * @brief	The total size of the tar
			  */
			size_t		size;

			/**
			  * @brief	The headers os the tar
			  */
			header_t	**headers;
	};	
}

#endif
