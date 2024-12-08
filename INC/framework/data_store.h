/**
 ******************************************************************************
 * @file    data_store.h
 * @author  KCE R&D Team
 * @date    2024-11-29
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_DATA_STORE_H_ 
#ifdef INC_DATA_STORE_H_

/* Enum type definitions -----------------------------------------------------*/


/* typedef definitions -----------------------------------------------------*/

/* function protocol -----------------------------------------------*/
void datastore_boot(void);

void get_Pv(void);
void get_RecordLow(void);
void get_RecordHigh(void);
void upload_syscfg_data(int8_t pr_index);

#endif /* INC_DATA_STORE_H_ */