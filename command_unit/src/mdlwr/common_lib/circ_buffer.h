/*!
\file circ_buffer_t.h
\brief circular buffer with static memory allocation first/last packet
 prioritisation. Copypasted from Neva's circ_buffer 
\author Baranov Mikhail, <a href="mailto:baranovm@t8.ru">baranovm@t8.ru</a>
\date may 2012
*/

#ifndef		CIRC_BUFFER_H
#define		CIRC_BUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	cmd_proc.h
 *   \brief  
 */


/*includes==========================================================================================================*/
#include <string.h>
#include <stdint.h> 

/*defines===========================================================================================================*/
#ifndef  OK
#define  OK     (0)
#endif

#ifndef   ERROR
#define   ERROR     (-1)
#endif

//! ���������� ��������� ����� � ���� (�.�. � ����� ��� ���������)
// ������� ��� � ��������������� ����� ������ ��� ����, ����� ��������� t8_memcopy32
#define		CIRCBUFFER(name, size) uint8_t name##_array_[size];\
		circ_buffer_t name = { (CircBuffer_datatype*)name##_array_, \
		size, 0, 0, 0, 0}


/*types=============================================================================================================*/
typedef		uint8_t		CircBuffer_datatype;

////////////////////////////////////////////////////////////////////////////////
//! ���������� ���������� ������
typedef struct 
    {
    	//! ��������� �� ������ � �������
    CircBuffer_datatype*	data; 
    //! ��������� ������ ��������� ������, ����� ������ �� ������ ������� 4
    const size_t			szTotal;	
    //! ������� ���������� ���������� ��������� (CircBuffer_datatype)
    size_t					szActual;
    //! ���-�� ������� ��������� � ���-�� ������������
    size_t					szLostBytes; 
    //! ����� ������� ��������� �������� � ���������
    size_t					iHead;
    //! ����� ���������� �� ��������� �������� ��������� � ���������
    size_t					iTail;
    } circ_buffer_t;


/*prototypes========================================================================================================*/

int circbuffer_pop_byte
( 
    circ_buffer_t           *const circbuf_desc,
    CircBuffer_datatype     *dest
);

int circbuffer_push_byte_erasing
(
	circ_buffer_t           *const circbuf_desc,
	CircBuffer_datatype     source
);

int circbuffer_pop_block
( 
    circ_buffer_t                   *circbuf_desc,
    CircBuffer_datatype             *dest, 
    size_t                          read_size,
    size_t                          *actual_read_size 
);

int circbuffer_push_block
(
	circ_buffer_t                   *circbuf_desc,
    size_t                          *actual_write_size,
	CircBuffer_datatype             *source,
	size_t                          write_size
);

int circbuffer_push_block_erasing
(
	circ_buffer_t                   *circbuf_desc,
	size_t                          *actual_write_size,
	CircBuffer_datatype             *source,
	size_t                          write_size
);

size_t	circbuffer_get_storage_data_size ( const circ_buffer_t* const circbuf_desc );
size_t	circbuffer_get_space_size( const circ_buffer_t* const circbuf_desc );
void    circbuffer_set_empty( circ_buffer_t* circbuf_desc );

#ifdef	__cplusplus
    }
#endif

#endif /* CIRC_BUFFER_H_ */

