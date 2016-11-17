#ifndef HRDWR_FUNC_H_
#define HRDWR_FUNC_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	hrdwr_func.h
 *   \brief  
 */

/*includes==========================================================================================================*/
#include <stdint.h>
    
    
/*defines===========================================================================================================*/
	

/*types=============================================================================================================*/
typedef enum _BPF_IF_GAIN_CHANNEL
    {
    BPF02 = 0,
    BPF05,
    BPF06,
    BPF08,
    } bpfif_gain_channel_t;

typedef enum _ADC_IF_CHANNEL
    {
    ADC3IF = 0,
    ADC1IF = 1,
    } adcif_channel_t;
    
    
typedef enum  _IF_CISPR
    {
    CISPR_OFF = 0,
    CISPR_BPF9k,
    CISPR_BPF120k,
    CISPR_BPF1000k,
    }if_cispr_t;
    
typedef enum  _HET_MUL
    {
    HETMUL_OFF      = 0,
    HETMUL_X2LOW,
    HETMUL_X2HIGH,
    HETMUL_X4HIGH,
    }het_mul_t;
    
typedef enum _ALARM_CODE
    {
    ALARM_ALL = 0,
    ALARM_CURRENT,
    ALARM_TEMP
    }alarm_code_t;
    
typedef enum _CU_TYPE
    {
    CU_TYPE_PRESELECTOR = 1,
    CU_TYPE_YIG,
    CU_TYPE_HETERODIN,
    CU_TYPE_IF,
    CU_TYPE_NOISE_GEN
    }cu_type_sts_t;
    
    
struct  _ATT_FLAGS
    {
    uint8_t     on : 1;
    uint8_t     lna : 1;
    uint8_t     preslna : 1;
    uint8_t     test : 1;
    } ;
    
struct _IF_FLAGS
    {
    if_cispr_t      cispr_state:2;
    uint8_t         filtr1012_on:1;
    uint8_t         logamp_on:1;
    adcif_channel_t adc_if:1;
    uint8_t         rf_adc:1;         
    };
    
struct _HET_FLAGS
    {
        het_mul_t   mulstate : 2;
    }; 
    
    
struct _CU_STATUS
    {
    uint8_t             alarm:1;
    alarm_code_t        alarm_code:3;
    cu_type_sts_t       type : 4;
    };
    
typedef union 
    {
    struct  _ATT_FLAGS  bitflags;
    uint8_t             byteflags;
    }att_flags_t;
    
typedef union 
    {
    struct  _IF_FLAGS   bitflags;
    uint8_t             byteflags;
    }if_flags_t;
    
typedef union
    {
    struct _HET_FLAGS   bitflags;
    uint8_t             byteflags;       
    }het_flags_t;
    
typedef union
    {
    struct _CU_STATUS   bitflags;
    uint8_t             byteflags;
    }custs_flags_t;
    

/*prototypes========================================================================================================*/

void        set_mechatt_gain( uint8_t gain );
uint8_t     get_mechatt_gain( void );
void        set_mechatt_flags(const struct  _ATT_FLAGS *flags );
uint8_t     get_mechatt_flags( void );
void        set_decoder_code( uint8_t  id, uint8_t code );
void        set_dac_code( uint8_t  id, uint16_t code );
uint16_t	get_dac_code(uint8_t  id);
void        set_dds_code( uint8_t  id, uint32_t freq, uint8_t gain );
void        set_yig_sts( uint8_t on );
uint8_t     get_yig_sts( void );
    
uint8_t		get_het_mul_sts(int het_index);
uint8_t		get_het_gain_sts(int het_index);
uint32_t	get_het_freq_sts(int het_index);  
void		set_het_mul(uint8_t code);
void		set_het_gain(int het_index, uint8_t gain);
void		set_het_freq(int het_index, uint32_t freq);
    
cu_type_sts_t	get_cu_type(void);
void			set_cu_type(cu_type_sts_t type);
	
	
#ifdef	__cplusplus
	}
#endif

#endif /* HRDWR_FUNC_H_ */
