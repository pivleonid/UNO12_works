#include <ctype.h>
#include "circ_buffer.h"
#include "mdlwr/common_lib/atomiccode.h"

/*=============================================================================================================*/

/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
int circbuffer_push_byte_erasing
(
	circ_buffer_t           *const circbuf_desc,
	CircBuffer_datatype     source
)
{
	STORAGE_ATOMIC();

	size_t	tail_store, head_store;
    uint8_t overflow_flag = 0;

	tail_store = circbuf_desc->iTail;
    head_store = circbuf_desc->iHead;

	circbuf_desc->data[tail_store] = source;
	tail_store++;
	if (tail_store == circbuf_desc->szTotal) {
		tail_store = 0;
	}
        if (tail_store == head_store) {
          overflow_flag = 1;
          head_store = tail_store + 1;
          if (head_store == circbuf_desc->szTotal) {
            head_store = 0;
          }
        }

	START_ATOMIC();
	circbuf_desc->iTail =  tail_store;
        if (overflow_flag) {
            circbuf_desc->iHead = head_store;
        }
	circbuf_desc->szActual = (overflow_flag) ? circbuf_desc->szTotal : circbuf_desc->szActual+1;
	STOP_ATOMIC();

	return OK;
}

/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/

int circbuffer_pop_byte
(
	circ_buffer_t           *const circbuf_desc,
	CircBuffer_datatype     *dest
	)
{
	STORAGE_ATOMIC();
	size_t	head_store;

	if ((circbuf_desc->szTotal == 0) || (dest == NULL)) {
		return ERROR;
	}

	head_store = circbuf_desc->iHead;

	*dest = circbuf_desc->data[head_store];
	head_store++;
	if (head_store == circbuf_desc->szTotal) {
		head_store = 0;
	}

	START_ATOMIC();
	circbuf_desc->iHead =  head_store;
	circbuf_desc->szActual--;
	STOP_ATOMIC();

	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
int circbuffer_pop_block
(
	circ_buffer_t                   *circbuf_desc,
	CircBuffer_datatype             *dest,
	size_t                          read_size,
	size_t                          *actual_read_size
	)
{
	STORAGE_ATOMIC();
	size_t          head_store, tail_store;
	size_t          readed_from_buf_data = read_size;   /* для  чтения в выходной буфер если пересекаются границы */
	size_t          readed_data_size;                   /* для подсчета считанного количества данных */


	if ((circbuf_desc->szActual == 0) || (read_size == 0))  {
		if (actual_read_size != NULL)  {
			*actual_read_size = 0;
		}
		return ERROR;
	}

	head_store = circbuf_desc->iHead;
	tail_store = circbuf_desc->iTail;

	if (read_size > circbuf_desc->szActual) {
		read_size = circbuf_desc->szActual;
	}

	/* определяем сколько можем вычитать из буфера */
	readed_from_buf_data = (tail_store > head_store) ?  (tail_store - head_store) : (circbuf_desc->szTotal - head_store);


	/* если количество запрашиваемых данных меньше, читаем только то что запросили */
	if (readed_from_buf_data > read_size) {readed_from_buf_data = 	read_size; }


	memcpy(dest, &(circbuf_desc->data[head_store]), readed_from_buf_data * sizeof circbuf_desc->data[0]);
	head_store += readed_from_buf_data;
	readed_data_size = readed_from_buf_data;

	/* если количество запрашиваемых данных больше чем мы можем вычитать за раз но данные еще есть  */
	if (readed_from_buf_data < read_size)  {
		if (head_store >= circbuf_desc->szTotal) {
			head_store = 0;
		}

		if (head_store != tail_store)  {
			if ((read_size - readed_data_size) >  tail_store)   {
				readed_from_buf_data = 	tail_store;
			}  else  {
				readed_from_buf_data = 	(read_size - readed_data_size);
			}

			memcpy(&dest[readed_data_size], circbuf_desc->data, readed_from_buf_data * sizeof circbuf_desc->data[0]);
			readed_data_size += readed_from_buf_data;
			head_store += readed_from_buf_data;
		}
	}

	*actual_read_size = readed_data_size;

	START_ATOMIC();
	circbuf_desc->iHead = head_store;
	circbuf_desc->szActual -= readed_data_size;
	STOP_ATOMIC();

	return OK;
}

/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
int circbuffer_push_block_erasing
(
	circ_buffer_t                   *circbuf_desc,
	size_t                          *actual_write_size,
	CircBuffer_datatype             *source,
	size_t                          write_size
)
{
	STORAGE_ATOMIC();
	size_t          head_store, tail_store;
	size_t          index               = 0;
	size_t          writed_to_buf_data  = write_size;
    uint8_t         overflow_flag = 0;


	if ((circbuf_desc->szTotal == 0) || (write_size == 0))  {
		if (actual_write_size != NULL)  {
			*actual_write_size = 0;
		}
		return ERROR;
	}

	head_store = circbuf_desc->iHead;
	tail_store = circbuf_desc->iTail;

	if ( write_size > circbuf_desc->szTotal ) {
		writed_to_buf_data = write_size = circbuf_desc->szTotal;
	}

        if ( write_size > circbuffer_get_space_size(circbuf_desc) ) {
           overflow_flag = 1;
        }          
        
	if ((tail_store + write_size) >= circbuf_desc->szTotal) {
            writed_to_buf_data = (circbuf_desc->szTotal - tail_store);
            memcpy(&circbuf_desc->data[tail_store], source, writed_to_buf_data * sizeof circbuf_desc->data[0]);
	    tail_store = 0;
	    index += writed_to_buf_data;
	    writed_to_buf_data = write_size - writed_to_buf_data;
	}
        
	memcpy(&circbuf_desc->data[tail_store], &source[index], writed_to_buf_data * sizeof circbuf_desc->data[0]);
	tail_store += writed_to_buf_data;
        
        if ( overflow_flag == 1 ) {          
            head_store = tail_store + 1;
            if (head_store == circbuf_desc->szTotal) {
                head_store = 0;
            }
        }

	*actual_write_size = write_size;

	START_ATOMIC();
	circbuf_desc->iTail = tail_store;
        if (overflow_flag) {
          circbuf_desc->iHead = head_store;
        }
	circbuf_desc->szActual = (overflow_flag) ? circbuf_desc->szTotal : (circbuf_desc->szActual + write_size) ;
	STOP_ATOMIC();

	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
int circbuffer_push_block
(
	circ_buffer_t                   *circbuf_desc,
	size_t                          *actual_write_size,
	CircBuffer_datatype             *source,
	size_t                          write_size
	)
{
	STORAGE_ATOMIC();
	size_t          head_store, tail_store;
	size_t          index               = 0;
	size_t          writed_to_buf_data  = write_size;

        
	if ((circbuf_desc->szTotal == 0) || (write_size == 0))  {
		if (actual_write_size != NULL)  {
			*actual_write_size = 0;
		}
		return ERROR;
	}

	head_store = circbuf_desc->iHead;
	tail_store = circbuf_desc->iTail;
        

	if ( write_size > circbuf_desc->szTotal ) {
		writed_to_buf_data = write_size = circbuf_desc->szTotal;
	}

        if ( write_size > circbuffer_get_space_size(circbuf_desc) ) {
               write_size = writed_to_buf_data = circbuffer_get_space_size(circbuf_desc);              
        }          
        
	if ((tail_store + write_size) >= circbuf_desc->szTotal) {
            writed_to_buf_data = (circbuf_desc->szTotal - tail_store);
            memcpy(&circbuf_desc->data[tail_store], source, writed_to_buf_data * sizeof circbuf_desc->data[0]);
	    tail_store = 0;
	    index += writed_to_buf_data;
	    writed_to_buf_data = write_size - writed_to_buf_data;
	}
        
	memcpy(&circbuf_desc->data[tail_store], &source[index], writed_to_buf_data * sizeof circbuf_desc->data[0]);
	tail_store += writed_to_buf_data;


	*actual_write_size = write_size;

	START_ATOMIC();
	circbuf_desc->iTail = tail_store;
	circbuf_desc->szActual = circbuf_desc->szActual + write_size;
	STOP_ATOMIC();

    return OK;
}

/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
size_t	circbuffer_get_storage_data_size(const circ_buffer_t *const circbuf_desc)
{
    return circbuf_desc->szActual;
}

/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
size_t	circbuffer_get_space_size(const circ_buffer_t *const circbuf_desc)
{
    return circbuf_desc->szTotal - circbuffer_get_storage_data_size(circbuf_desc);
}

/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
void    circbuffer_set_empty( circ_buffer_t* circbuf_desc )
{
    STORAGE_ATOMIC();
    START_ATOMIC();
    circbuf_desc->iHead = circbuf_desc->iTail;
    circbuf_desc->szActual = 0;
    STOP_ATOMIC();
}
