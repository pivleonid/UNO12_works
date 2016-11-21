#ifndef		AD9116_H
#define		AD9116_H

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	cpld_bus.h
 *   \brief  ���� ������� ���������� � ����� � CPLD
 */

   
/*includes==========================================================================================================*/
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include "ad9116r.h"
        
/*defines===========================================================================================================*/
#ifndef  OK
#define  OK     (0)
#endif

#ifndef   ERR
#define   ERR     (-1)
#endif

#define  DAC_ALL	(DAC_01 | DAC_23 | DAC_45 | DAC_67)			/*	����� ��� ��� �������������	*/
#define  DAC_01		(0x01u)
#define  DAC_23		(0x02u)
#define  DAC_45		(0x04u)
#define  DAC_67		(0x08u)
	
	
/*types=============================================================================================================*/
enum AD9116_INTERFACE {
    AD9116_SPI,
    AD9116_PARALLEL
    };
    
typedef	struct _DAC_VALUES
	{
		uint16_t	q;
		uint16_t	i;	
	} dac_values_t;
	
	
/*prototypes========================================================================================================*/
	int     ad9116_open
	    (
	     int dac_index      /*!< [in] ������ ���� 0-3 */
	     );
	
	int     ad9116_close
	    (
	     int dac_index      /*!< [in] ������ ���� 0-3 */
	     );
	
	int     ad9116_write_data
	    (
	     int                    dac_index,  /*!< [in] ������ ���� 0-3                           */
		enum AD9116_INTERFACE   interface,  /*!< [in] ��������� �� �������� ���������� ������   */
		dac_values_t			data		/*!< [in] ������ ��� ������ i � q ����			    */
		);
	
	int     ad9116_write_all_parralel
	    (
	    int                    dac_index,  /*!< [in] ������ ���� 0-3                           */
		uint16_t               *data,	   /*!< [in] ������ ��� ������ i ����				    */
		uint8_t                len		   /*!< [in] ������ ��� ������ q ����    */
		);

	int  ad9116_write_register
		(
			int							dac_index,  /*!< [in] ������ ���� 0-3                           */
			uint8_t						adress,
			void						*data
		);
	
	int  ad9116_read_register
		(
			int							dac_index,  /*!< [in] ������ ���� 0-3                           */
		uint8_t						adress,
		void						*data
		);
	
	
	
//	int  ad9116_write_settings
//		(
//			int							dac_index,  /*!< [in] ������ ���� 0-3                           */
//			const ad9116_settings_t		*settings	/*!< [in]  */
//		);
//	
//	ad9116_settings_t ad9116_read_settings
//		(
//			int							dac_index,  /*!< [in] ������ ���� 0-3                           */
//		);
//	
	
	
	uint8_t     ad9116_read_id
	    (
	     int                    dac_index   /*!< [in] ������ ���� 0-3                           */
	     );
 
#ifdef	__cplusplus
	}
#endif

#endif /* AD9116_H */

